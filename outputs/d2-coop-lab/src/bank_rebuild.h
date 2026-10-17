#pragma once
// #30 (stage 7 release): rebuilds the movement banks from the player's own animations. The release ships no
// bank; it ships a bank patch (tools/release/build_bank_patch.py, format D2BNK001) that names native assets and,
// per clip, frame and body part, which of them to sample (the bind pose, a native clip retargeted onto the rig or
// copied by joint name, optionally mirrored, or an already rebuilt bank clip) at which time, blended with which
// weight. The base pose this gives is completed by a quantized residual (our retargets, edits and authored motion)
// and written in the bank format movement_bank.h loads (D2MOVE71).
// The math follows our Python tools: retarget_native.py (retarget, helpers), the builders' mirrored(),
// retarget_native.py's additive composition, in double precision. Native samples come from the player's
// Dishonored2.exe Edge evaluator, which uses RCPPS/RSQRTPS (confirmed static), so a rebuilt bank can differ from
// the lab's in the last bits on another CPU; the installer checks per-clip sums with a tolerance instead of a hash.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace d2bank {

using Bytes=std::vector<std::uint8_t>;
using Q=std::array<double,4>;   // x, y, z, w
using V=std::array<double,3>;
using M=std::array<double,9>;   // row major
using Value=std::array<double,7>;  // quaternion, translation
using Pose=std::vector<Value>;
using Track=std::vector<Pose>;

// ---- Edge skeleton (.edgeskl): our edge_data.py reader. ----
struct Joint {std::string name;int parent=-1;std::array<float,12> bind{};};
inline std::vector<Joint> readSkeleton(const Bytes& b){
    auto u16=[&](std::size_t o){if(o+2>b.size())throw std::runtime_error("skeleton: truncated");return unsigned(b[o])|(unsigned(b[o+1])<<8);};
    auto u32=[&](std::size_t o){if(o+4>b.size())throw std::runtime_error("skeleton: truncated");std::uint32_t v;std::memcpy(&v,b.data()+o,4);return v;};
    if(b.size()<0x48||std::memcmp(b.data(),"60SE",4)||u32(4)!=b.size())throw std::runtime_error("skeleton: bad header");
    const unsigned count=u16(0x10);if(!count||count>512)throw std::runtime_error("skeleton: joint count");
    std::size_t p=0x30+u32(0x30);
    auto str=[&](){const auto n=u32(p);p+=4;if(n>4096||p+n>b.size())throw std::runtime_error("skeleton: string");std::string s(reinterpret_cast<const char*>(b.data()+p),n);p+=n;return s;};
    p+=4;const auto inherit=str();if(!inherit.empty())p+=2;
    p+=4;const unsigned declared=u16(p),levels=u16(p+2);p+=4;
    if(declared!=count||levels>4)throw std::runtime_error("skeleton: levels");
    if(levels)p+=2*(levels+count);
    std::vector<Joint> out(count);for(auto& j:out)j.name=str();
    const std::size_t bindAt=0x18+u32(0x18),parentMap=0x1c+u32(0x1c);
    if(bindAt+std::size_t(count)*48>b.size())throw std::runtime_error("skeleton: bind pose");
    for(unsigned i=0;i<count;++i){
        const unsigned mapped=u16(parentMap+2*i),parent=u16(0x42+4*std::size_t(mapped))&0x7fff;
        out[i].parent=parent>=0x4000?-1:int(parent);if(out[i].parent>=int(i))throw std::runtime_error("skeleton: parent order");
        std::memcpy(out[i].bind.data(),b.data()+bindAt+48*std::size_t(i),48);
    }
    return out;
}

// ---- Rotation math. ----
inline M mul(const M& a,const M& b){M r{};for(int i=0;i<3;++i)for(int j=0;j<3;++j)r[3*i+j]=a[3*i]*b[j]+a[3*i+1]*b[3+j]+a[3*i+2]*b[6+j];return r;}
inline V mul(const M& a,const V& v){return {a[0]*v[0]+a[1]*v[1]+a[2]*v[2],a[3]*v[0]+a[4]*v[1]+a[5]*v[2],a[6]*v[0]+a[7]*v[1]+a[8]*v[2]};}
inline M tr(const M& a){return {a[0],a[3],a[6],a[1],a[4],a[7],a[2],a[5],a[8]};}
inline M eye(){return {1,0,0,0,1,0,0,0,1};}
inline M q2m(Q q){
    const double n=std::sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);for(auto& v:q)v/=n;
    const double x=q[0],y=q[1],z=q[2],w=q[3];
    return {1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w),2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w),2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)};
}
inline Q m2q(const M& a){
    const double t=a[0]+a[4]+a[8];Q q;
    if(t>0){const double s=std::sqrt(t+1)*2;q={(a[7]-a[5])/s,(a[2]-a[6])/s,(a[3]-a[1])/s,s/4};}
    else if(a[0]>a[4]&&a[0]>a[8]){const double s=std::sqrt(1+a[0]-a[4]-a[8])*2;q={s/4,(a[1]+a[3])/s,(a[2]+a[6])/s,(a[7]-a[5])/s};}
    else if(a[4]>a[8]){const double s=std::sqrt(1+a[4]-a[0]-a[8])*2;q={(a[1]+a[3])/s,s/4,(a[5]+a[7])/s,(a[2]-a[6])/s};}
    else{const double s=std::sqrt(1+a[8]-a[0]-a[4])*2;q={(a[2]+a[6])/s,(a[5]+a[7])/s,s/4,(a[3]-a[1])/s};}
    const double n=std::sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);for(auto& v:q)v/=n;
    if(q[3]<0)for(auto& v:q)v=-v;
    return q;
}
inline Q qmul(const Q& a,const Q& b){
    return {a[3]*b[0]+a[0]*b[3]+a[1]*b[2]-a[2]*b[1],a[3]*b[1]-a[0]*b[2]+a[1]*b[3]+a[2]*b[0],
            a[3]*b[2]+a[0]*b[1]-a[1]*b[0]+a[2]*b[3],a[3]*b[3]-a[0]*b[0]-a[1]*b[1]-a[2]*b[2]};
}

struct Rig {
    std::vector<std::string> names;std::vector<int> parent;std::vector<M> R,GR;std::vector<V> T,GT;
    explicit Rig(const std::vector<Joint>& joints){
        for(const auto& j:joints){names.push_back(j.name);parent.push_back(j.parent);
            R.push_back(q2m({j.bind[0],j.bind[1],j.bind[2],j.bind[3]}));T.push_back({j.bind[4],j.bind[5],j.bind[6]});}
        fk(R,T,GR,GT);
    }
    void fk(const std::vector<M>& r,const std::vector<V>& t,std::vector<M>& gr,std::vector<V>& gt) const {
        gr.resize(r.size());gt.resize(t.size());
        for(std::size_t i=0;i<r.size();++i){const int p=parent[i];
            if(p<0){gr[i]=r[i];gt[i]=t[i];}else{gr[i]=mul(gr[p],r[i]);const V v=mul(gr[p],t[i]);gt[i]={v[0]+gt[p][0],v[1]+gt[p][1],v[2]+gt[p][2]};}}
    }
    int find(const std::string& n) const {for(std::size_t i=0;i<names.size();++i)if(names[i]==n)return int(i);return -1;}
    std::size_t size() const {return names.size();}
};

// Native skin helper joints follow their driver joint's motion from the bind pose (retarget_native.py helpers()).
inline void helpers(const Rig& rig,std::vector<M>& R,std::vector<V>& T){
    std::vector<M> GR(R.size());std::vector<V> GT(R.size());
    for(std::size_t i=0;i<R.size();++i){
        const auto& n=rig.names[i];const int p=rig.parent[i];
        const std::string side=n.rfind("left",0)==0?"left":n.rfind("right",0)==0?"right":"";std::string driver;
        auto has=[&](const char* s){return n.find(s)!=std::string::npos;};
        if(!side.empty()&&has("helper")){
            if(has("upper_leg")||has("thigh"))driver=side+"_femur_jnt";
            else if(has("knee"))driver=side+"_tibia_jnt";
            else if(has("arm01")&&!has("forearm"))driver=side+"_arm_jnt";
            else if(has("elbow")||has("forearm"))driver=side+"_elbow_jnt";
            else if(has("wrist"))driver=side+"_wrist_jnt";
            else if(has("ankle"))driver=side+"_ankle_jnt";
        }
        const int k=driver.empty()?-1:rig.find(driver);
        if(k>=0&&std::size_t(k)<i&&p>=0){
            // local = inverse(G[p]) * G[k] * inverse(BG[k]) * BG[i], all rigid.
            const M a=mul(GR[k],tr(rig.GR[k]));const V bt={rig.GT[i][0]-rig.GT[k][0],rig.GT[i][1]-rig.GT[k][1],rig.GT[i][2]-rig.GT[k][2]};
            const M g=mul(a,rig.GR[i]);const V gb=mul(GR[k],mul(tr(rig.GR[k]),bt));const V gt={gb[0]+GT[k][0],gb[1]+GT[k][1],gb[2]+GT[k][2]};
            R[i]=mul(tr(GR[p]),g);T[i]=mul(tr(GR[p]),V{gt[0]-GT[p][0],gt[1]-GT[p][1],gt[2]-GT[p][2]});
        }
        if(p<0){GR[i]=R[i];GT[i]=T[i];}else{GR[i]=mul(GR[p],R[i]);const V v=mul(GR[p],T[i]);GT[i]={v[0]+GT[p][0],v[1]+GT[p][1],v[2]+GT[p][2]};}
    }
}
inline Pose toPose(const std::vector<M>& R,const std::vector<V>& T){
    Pose out(R.size());for(std::size_t i=0;i<R.size();++i){const Q q=m2q(R[i]);out[i]={q[0],q[1],q[2],q[3],T[i][0],T[i][1],T[i][2]};}return out;
}
inline Pose bindPose(const Rig& rig){return toPose(rig.R,rig.T);}

// A native sample: frames x joints x 12 floats (quaternion, translation + pad, scale + pad) from the Edge evaluator.
struct Sample {unsigned joints=0,frames=0;float fps=0;std::vector<float> values;const float* at(unsigned f,unsigned j) const {return values.data()+(std::size_t(f)*joints+j)*12;}};

// Native additive clips: local Q = reference Q * delta Q, T = reference T + delta T (retarget_native.py).
inline void composeAdditive(Sample& s,const std::vector<std::array<double,7>>& ref){
    if(ref.size()!=s.joints)throw std::runtime_error("additive reference joint count");
    for(unsigned f=0;f<s.frames;++f)for(unsigned j=0;j<s.joints;++j){
        float* v=s.values.data()+(std::size_t(f)*s.joints+j)*12;const Q q=qmul({ref[j][0],ref[j][1],ref[j][2],ref[j][3]},{v[0],v[1],v[2],v[3]});
        for(int k=0;k<4;++k)v[k]=float(q[k]);for(int k=0;k<3;++k)v[4+k]=float(ref[j][4+k]+v[4+k]);
    }
}
// sword_*_add: sword_neutral frame 0; empty_*_add: empty_walk_n frame 0 composed with the inverse of empty_walk_n_add frame 0.
inline std::vector<std::array<double,7>> additiveReference(const Sample& a,const Sample* inverse){
    std::vector<std::array<double,7>> out(a.joints);
    for(unsigned j=0;j<a.joints;++j){const float* o=a.at(0,j);Q q={o[0],o[1],o[2],o[3]};V t={o[4],o[5],o[6]};
        if(inverse){const float* d=inverse->at(0,j);q=qmul(q,{-double(d[0]),-double(d[1]),-double(d[2]),double(d[3])});t={t[0]-d[4],t[1]-d[5],t[2]-d[6]};}
        out[j]={q[0],q[1],q[2],q[3],t[0],t[1],t[2]};}
    return out;
}

// retarget_native.py: global rotations relative to the bind pose, matched by joint name; the root keeps the
// native translation offset scaled by leg length.
inline Track retarget(const Sample& s,const Rig& src,const Rig& tgt){
    if(s.joints!=src.size())throw std::runtime_error("retarget: sample joint count");
    const bool body=src.size()>21&&src.names[19]=="left_femur_jnt"&&tgt.size()>21;
    auto len=[](const V& a,const V& b){return std::sqrt((a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1])+(a[2]-b[2])*(a[2]-b[2]));};
    const double scale=body?len(tgt.GT[19],tgt.GT[21])/len(src.GT[19],src.GT[21]):1.0;
    std::vector<int> map(tgt.size(),-1);for(std::size_t i=0;i<tgt.size();++i)map[i]=src.find(tgt.names[i]);
    Track out;
    for(unsigned f=0;f<s.frames;++f){
        std::vector<M> SR(src.size()),SGR;std::vector<V> ST(src.size()),SGT;
        for(unsigned j=0;j<src.size();++j){const float* v=s.at(f,j);SR[j]=q2m({v[0],v[1],v[2],v[3]});ST[j]={v[4],v[5],v[6]};}
        SR[0]=SR[1]=eye();ST[0]=ST[1]=V{};src.fk(SR,ST,SGR,SGT);
        std::vector<M> R=tgt.R,GR(tgt.size());std::vector<V> T=tgt.T;
        for(std::size_t i=0;i<tgt.size();++i){
            const int p=tgt.parent[i],k=map[i];
            if(k>=0&&i>=2&&tgt.names[i].find("helper")==std::string::npos){
                R[i]=mul(tr(GR[p]),mul(mul(SGR[k],tr(src.GR[k])),tgt.GR[i]));
                if(i==2&&body){const float* v=s.at(f,unsigned(k));for(int c=0;c<3;++c)T[i][c]=tgt.T[i][c]+(double(v[4+c])-src.T[k][c])*scale;}
            }
            GR[i]=p<0?R[i]:mul(GR[p],R[i]);
        }
        R[0]=R[1]=eye();T[0]=T[1]=V{};helpers(tgt,R,T);out.push_back(toPose(R,T));
    }
    return out;
}
// Local rotations copied by joint name (from joint 3 on); the bind pose elsewhere.
inline Track byName(const Sample& s,const Rig& src,const Rig& tgt){
    const Pose bind=bindPose(tgt);Track out(s.frames,bind);
    for(std::size_t j=3;j<tgt.size();++j){const int k=src.find(tgt.names[j]);if(k<0)continue;
        for(unsigned f=0;f<s.frames;++f){const float* v=s.at(f,unsigned(k));double q[4]={v[0],v[1],v[2],v[3]};
            const double n=std::sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);const double sign=q[3]<0?-1.0:1.0;
            for(int c=0;c<4;++c)out[f][j][c]=sign*q[c]/n;}}
    return out;
}
// Left/right mirror across the rig's Y axis (the builders' mirrored()).
inline Track mirror(const Track& in,const Rig& rig){
    const std::size_t J=rig.size();std::vector<int> pairs(J);
    for(std::size_t i=0;i<J;++i){const auto& n=rig.names[i];std::string o=n.rfind("left",0)==0?"right"+n.substr(4):n.rfind("right",0)==0?"left"+n.substr(5):n;
        const int k=rig.find(o);pairs[i]=k>=0?k:int(i);}
    const M F={1,0,0,0,-1,0,0,0,1};Track out;
    for(const auto& pose:in){
        std::vector<M> R(J),old(J),outR=rig.R,G(J);std::vector<V> outT=rig.T;
        for(std::size_t i=0;i<J;++i){R[i]=q2m({pose[i][0],pose[i][1],pose[i][2],pose[i][3]});const int p=rig.parent[i];old[i]=p<0?R[i]:mul(old[p],R[i]);}
        for(std::size_t i=0;i<J;++i){const int p=rig.parent[i];
            if(i<2){outR[i]=eye();outT[i]=V{};}
            else{const int k=pairs[i];const M target=mul(mul(mul(mul(F,old[k]),tr(rig.GR[k])),F),rig.GR[i]);outR[i]=mul(tr(G[p]),target);
                if(i==2)outT[i]=mul(F,V{pose[2][4],pose[2][5],pose[2][6]});}
            G[i]=p<0?outR[i]:mul(G[p],outR[i]);
        }
        helpers(rig,outR,outT);out.push_back(toPose(outR,outT));
    }
    return out;
}
// Normalized linear blend with hemisphere alignment; translations blend linearly.
inline Value nlerp(const Value& a,const Value& b,double t){
    const double dot=a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3],sign=dot<0?-1.0:1.0;Value v;double n=0;
    for(int k=0;k<4;++k){v[k]=a[k]*(1-t)+b[k]*sign*t;n+=v[k]*v[k];}n=std::sqrt(n);for(int k=0;k<4;++k)v[k]/=n;
    for(int k=4;k<7;++k)v[k]=a[k]*(1-t)+b[k]*t;return v;
}
// A track at a fractional frame.
inline Value at(const Track& t,double u,std::size_t joint){
    if(t.empty())throw std::runtime_error("empty track");
    if(!(u>=0))u=0;const auto g=std::size_t(std::floor(u));
    if(g+1>=t.size())return nlerp(t.back()[joint],t.back()[joint],0);
    return nlerp(t[g][joint],t[g+1][joint],u-double(g));
}

// ---- Bank parameters (D2BPR001, tools/release/fit_bank_bases.py). ----
constexpr unsigned frames=121;
using Hash=std::array<std::uint8_t,32>;
struct In {
    const Bytes& d;std::size_t at=0;
    void need(std::size_t n) const {if(at+n>d.size()||at+n<at)throw std::runtime_error("bank patch: truncated");}
    template<class T>T get(){need(sizeof(T));T v;std::memcpy(&v,d.data()+at,sizeof(T));at+=sizeof(T);return v;}
    std::string str(){const auto n=get<std::uint32_t>();if(n>4096)throw std::runtime_error("bank patch: string");need(n);std::string s(reinterpret_cast<const char*>(d.data()+at),n);at+=n;return s;}
    Hash hash(){need(32);Hash h;std::memcpy(h.data(),d.data()+at,32);at+=32;return h;}
};
struct Out {
    Bytes d;
    template<class T>void put(const T& v){const auto* p=reinterpret_cast<const std::uint8_t*>(&v);d.insert(d.end(),p,p+sizeof(T));}
    void raw(const void* p,std::size_t n){const auto* b=static_cast<const std::uint8_t*>(p);d.insert(d.end(),b,b+n);}
};
struct Params {
    struct Asset {std::string name;std::uint32_t skeleton=0;std::uint8_t additive=0;std::uint32_t ref[2]{};};
    struct Source {std::uint8_t kind=0;std::uint32_t index=0;std::uint8_t variant=0,mirrored=0;};
    struct Choice {std::uint16_t a=0;float ua=0;std::uint16_t b=0;float ub=0;float w=0;};
    struct Clip {bool loop=false;std::vector<Source> sources;std::vector<Choice> choices;};
    std::string role;std::uint32_t joints=0,parts=0,rig=0;std::vector<std::uint8_t> part;
    std::vector<std::string> skeletons;std::vector<Asset> assets;std::vector<Clip> clips;
};
inline Params readParams(In& r){
    r.need(8);if(std::memcmp(r.d.data()+r.at,"D2BPR001",8))throw std::runtime_error("bank parameters: bad magic");r.at+=8;
    Params p;p.role=r.str();p.joints=r.get<std::uint32_t>();p.parts=r.get<std::uint32_t>();
    if(p.joints<3||p.joints>512||!p.parts||p.parts>64)throw std::runtime_error("bank parameters: dimensions");
    for(std::uint32_t j=0;j<p.joints;++j){const auto v=r.get<std::uint8_t>();if((v==255)!=(j<2)||(v!=255&&v>=p.parts))throw std::runtime_error("bank parameters: part map");p.part.push_back(v);}
    for(auto n=r.get<std::uint32_t>();n--;)p.skeletons.push_back(r.str());
    p.rig=r.get<std::uint32_t>();if(p.rig>=p.skeletons.size())throw std::runtime_error("bank parameters: rig");
    for(auto n=r.get<std::uint32_t>();n--;){Params::Asset a;a.name=r.str();a.skeleton=r.get<std::uint32_t>();a.additive=r.get<std::uint8_t>();a.ref[0]=r.get<std::uint32_t>();a.ref[1]=r.get<std::uint32_t>();
        if(a.skeleton>=p.skeletons.size()||a.additive>2)throw std::runtime_error("bank parameters: asset");p.assets.push_back(std::move(a));}
    for(const auto& a:p.assets)for(int k=0;k<a.additive;++k)if(a.ref[k]>=p.assets.size())throw std::runtime_error("bank parameters: additive reference");
    const auto clips=r.get<std::uint32_t>();if(clips>4096)throw std::runtime_error("bank parameters: clip count");
    for(std::uint32_t c=0;c<clips;++c){
        Params::Clip clip;clip.loop=r.get<std::uint8_t>()!=0;
        for(auto n=r.get<std::uint32_t>();n--;){Params::Source s;s.kind=r.get<std::uint8_t>();s.index=r.get<std::uint32_t>();s.variant=r.get<std::uint8_t>();s.mirrored=r.get<std::uint8_t>();
            if((s.kind==1&&s.index>=p.assets.size())||(s.kind==2&&s.index>=c)||(s.kind!=1&&s.kind!=2)||s.variant>1||s.mirrored>1)throw std::runtime_error("bank parameters: source");
            clip.sources.push_back(s);}
        clip.choices.resize(std::size_t(frames)*p.parts);
        for(auto& ch:clip.choices){ch.a=r.get<std::uint16_t>();ch.ua=r.get<float>();ch.b=r.get<std::uint16_t>();ch.ub=r.get<float>();ch.w=r.get<float>();
            if(ch.a>clip.sources.size()||ch.b>clip.sources.size()||!std::isfinite(ch.ua)||!std::isfinite(ch.ub)||!(ch.w>=0&&ch.w<=1))throw std::runtime_error("bank parameters: choice");}
        p.clips.push_back(std::move(clip));
    }
    return p;
}

// ---- Bank patch (D2BNK001): what the release ships.
//   magic, exe SHA-256, u32 n + n skeleton SHA-256, u32 n + n asset SHA-256 (in parameter order), the bank's
//   32-byte header, u32 clips + per clip (f64 sum, f64 sum of squares) of its values, u32 residual step exponent,
//   u64 body size, u32 stream size, a zlib stream of the body: the parameters, then per clip its 24-byte header,
//   per frame a u32 copy reference (clip * frames + frame of an earlier identical frame, or 0xffffffff), the
//   quaternion sign flips (frames x joints bits) and the residual (frames x joints x 7 int32). Copies keep the
//   bank's exact continuity between clips (a clip that ends in another clip's pose).
struct Patch {
    Hash exe{};std::vector<Hash> skeletons,assets;std::array<std::uint8_t,32> header{};std::vector<std::array<double,2>> sums;std::uint32_t step=17;
    Params params;std::vector<std::array<std::uint8_t,24>> clipHeaders;std::vector<std::vector<std::uint8_t>> flips;std::vector<std::vector<std::int32_t>> residual;
    std::vector<std::vector<std::uint32_t>> refs;
};
constexpr std::uint32_t noCopy=0xffffffffu;
inline std::size_t flipBytes(const Params& p){return (std::size_t(frames)*p.joints+7)/8;}
inline Patch readPatch(const Bytes& d,const std::function<Bytes(const std::uint8_t*,std::size_t,std::size_t)>& inflate){
    In r{d};r.need(8);if(std::memcmp(d.data(),"D2BNK001",8))throw std::runtime_error("bank patch: bad magic");r.at=8;
    Patch p;p.exe=r.hash();
    for(auto n=r.get<std::uint32_t>();n--;)p.skeletons.push_back(r.hash());
    for(auto n=r.get<std::uint32_t>();n--;)p.assets.push_back(r.hash());
    r.need(32);std::memcpy(p.header.data(),d.data()+r.at,32);r.at+=32;
    for(auto n=r.get<std::uint32_t>();n--;)p.sums.push_back({r.get<double>(),r.get<double>()});
    p.step=r.get<std::uint32_t>();if(p.step<8||p.step>30)throw std::runtime_error("bank patch: step");
    const auto bodySize=r.get<std::uint64_t>();const auto streamSize=r.get<std::uint32_t>();r.need(streamSize);
    if(bodySize>(std::uint64_t(1)<<31))throw std::runtime_error("bank patch: body size");
    const Bytes body=inflate(d.data()+r.at,streamSize,std::size_t(bodySize));r.at+=streamSize;
    if(r.at!=d.size())throw std::runtime_error("bank patch: trailing bytes");
    In b{body};p.params=readParams(b);const auto& P=p.params;
    if(p.skeletons.size()!=P.skeletons.size()||p.assets.size()!=P.assets.size()||p.sums.size()!=P.clips.size())throw std::runtime_error("bank patch: counts");
    for(std::size_t c=0;c<P.clips.size();++c){
        b.need(24);std::array<std::uint8_t,24> h;std::memcpy(h.data(),body.data()+b.at,24);b.at+=24;p.clipHeaders.push_back(h);
        b.need(frames*4);std::vector<std::uint32_t> refs(frames);std::memcpy(refs.data(),body.data()+b.at,frames*4);b.at+=frames*4;p.refs.push_back(std::move(refs));
        const auto fb=flipBytes(P);b.need(fb);p.flips.emplace_back(body.begin()+std::ptrdiff_t(b.at),body.begin()+std::ptrdiff_t(b.at+fb));b.at+=fb;
        const std::size_t n=std::size_t(frames)*P.joints*7;b.need(n*4);std::vector<std::int32_t> res(n);std::memcpy(res.data(),body.data()+b.at,n*4);b.at+=n*4;p.residual.push_back(std::move(res));
    }
    if(b.at!=body.size())throw std::runtime_error("bank patch: body trailing bytes");
    return p;
}
// The body is written as stored deflate blocks; the release builder recompresses it.
inline Bytes storedZlib(const Bytes& data){
    Bytes out={0x78,0x01};std::size_t at=0;
    do{const std::size_t n=std::min<std::size_t>(65535,data.size()-at);const bool last=at+n==data.size();
        out.push_back(last?1:0);out.push_back(std::uint8_t(n));out.push_back(std::uint8_t(n>>8));out.push_back(std::uint8_t(~n));out.push_back(std::uint8_t((~n)>>8));
        out.insert(out.end(),data.begin()+std::ptrdiff_t(at),data.begin()+std::ptrdiff_t(at+n));at+=n;}while(at<data.size());
    std::uint32_t a=1,b=0;for(auto c:data){a=(a+c)%65521;b=(b+a)%65521;}const std::uint32_t adler=(b<<16)|a;
    for(int k=3;k>=0;--k)out.push_back(std::uint8_t(adler>>(8*k)));
    return out;
}
inline Bytes writePatch(const Patch& p,const Bytes& paramsBytes){
    Out body;body.raw(paramsBytes.data(),paramsBytes.size());
    for(std::size_t c=0;c<p.params.clips.size();++c){body.raw(p.clipHeaders[c].data(),24);body.raw(p.refs[c].data(),p.refs[c].size()*4);body.raw(p.flips[c].data(),p.flips[c].size());body.raw(p.residual[c].data(),p.residual[c].size()*4);}
    const Bytes stream=storedZlib(body.d);
    Out o;o.raw("D2BNK001",8);o.raw(p.exe.data(),32);
    o.put(std::uint32_t(p.skeletons.size()));for(const auto& h:p.skeletons)o.raw(h.data(),32);
    o.put(std::uint32_t(p.assets.size()));for(const auto& h:p.assets)o.raw(h.data(),32);
    o.raw(p.header.data(),32);o.put(std::uint32_t(p.sums.size()));for(const auto& s:p.sums){o.put(s[0]);o.put(s[1]);}
    o.put(p.step);o.put(std::uint64_t(body.d.size()));o.put(std::uint32_t(stream.size()));o.raw(stream.data(),stream.size());
    return o.d;
}

// ---- Rebuilding a bank from parameters and the player's samples. ----
// Each clip's base: per frame and body part a blend of two sources at their times; joints 0-1 identity, joint
// translations from the rig's bind pose except the root's; helper joints recomputed. With `lab` given (encoding),
// the residual and flips are measured against it; otherwise the patch's are applied. A frame with a copy reference
// takes the referenced, already rebuilt frame exactly (its flips and residual stay zero).
class Rebuilder {
public:
    using Sampler=std::function<Sample(std::size_t asset)>;
    Rebuilder(const Params& p,const std::vector<std::vector<Joint>>& skeletons,Sampler sampler):P(p),sample(std::move(sampler)){
        for(const auto& s:skeletons)rigs.emplace_back(s);
        if(rigs.size()!=P.skeletons.size())throw std::runtime_error("rebuild: skeleton count");
        if(rigs[P.rig].size()!=P.joints)throw std::runtime_error("rebuild: rig joint count");
        bind=bindPose(rigs[P.rig]);
        // Last clip that needs each asset (directly or as an additive reference), so caches can be dropped.
        lastUse.assign(P.assets.size(),0);
        for(std::size_t c=0;c<P.clips.size();++c)for(const auto& s:P.clips[c].sources)if(s.kind==1){
            lastUse[s.index]=c;const auto& a=P.assets[s.index];for(int k=0;k<a.additive;++k)lastUse[a.ref[k]]=std::max(lastUse[a.ref[k]],c);}
        clipLastUse.assign(P.clips.size(),0);
        for(std::size_t c=0;c<P.clips.size();++c)for(const auto& s:P.clips[c].sources)if(s.kind==2)clipLastUse[s.index]=c;
    }
    // One clip: values (frames x joints x 7 floats). Encoding fills flips and residual from `lab`.
    std::vector<float> clip(std::size_t c,const float* lab,std::uint32_t step,std::vector<std::uint8_t>& flips,std::vector<std::int32_t>& residual,
                            const std::vector<std::uint32_t>& refs){
        const auto& C=P.clips[c];const auto& rig=rigs[P.rig];const std::size_t J=P.joints;const double unit=std::ldexp(1.0,-int(step));
        std::vector<const Track*> src(C.sources.size()+1,nullptr);
        for(std::size_t k=0;k<C.sources.size();++k)src[k+1]=&track(C.sources[k],c);
        if(lab){flips.assign(flipBytes(P),0);residual.assign(std::size_t(frames)*J*7,0);}
        else if(flips.size()!=flipBytes(P)||residual.size()!=std::size_t(frames)*J*7)throw std::runtime_error("rebuild: residual size");
        if(refs.size()!=frames)throw std::runtime_error("rebuild: copy references");
        std::vector<float> out(std::size_t(frames)*J*7);
        auto value=[&](std::uint16_t s,float u,std::size_t j)->Value{return s?at(*src[s],u,j):bind[j];};
        for(unsigned f=0;f<frames;++f){
            if(refs[f]!=noCopy){
                const std::size_t rc=refs[f]/frames,rf=refs[f]%frames;
                if(rc>c||(rc==c&&rf>=f))throw std::runtime_error("rebuild: copy reference not earlier");
                const float* from=(rc==c?out.data():decoded[rc].data())+rf*J*7;std::memcpy(out.data()+std::size_t(f)*J*7,from,J*7*sizeof(float));
                continue;
            }
            Pose pose=bind;
            for(std::size_t j=2;j<J;++j){const auto& ch=C.choices[std::size_t(f)*P.parts+P.part[j]];pose[j]=nlerp(value(ch.a,ch.ua,j),value(ch.b,ch.ub,j),ch.w);}
            pose[0]=pose[1]=Value{0,0,0,1,0,0,0};for(std::size_t j=3;j<J;++j)for(int k=4;k<7;++k)pose[j][k]=bind[j][k];
            std::vector<M> R(J);std::vector<V> T(J);
            for(std::size_t j=0;j<J;++j){R[j]=q2m({pose[j][0],pose[j][1],pose[j][2],pose[j][3]});T[j]={pose[j][4],pose[j][5],pose[j][6]};}
            helpers(rig,R,T);pose=toPose(R,T);pose[0]=pose[1]=Value{0,0,0,1,0,0,0};
            for(std::size_t j=0;j<J;++j){
                const std::size_t bit=std::size_t(f)*J+j,at7=bit*7;
                if(lab){double dot=0;for(int k=0;k<4;++k)dot+=double(lab[at7+k])*pose[j][k];if(dot<0)flips[bit/8]|=std::uint8_t(1u<<(bit%8));}
                const bool flip=(flips[bit/8]>>(bit%8))&1;double v[7];
                for(int k=0;k<7;++k){
                    const double base=k<4&&flip?-pose[j][k]:pose[j][k];
                    if(lab){const double r=std::nearbyint((double(lab[at7+k])-base)/unit);if(!(std::fabs(r)<2147483647.0))throw std::runtime_error("rebuild: residual range");residual[at7+k]=std::int32_t(r);}
                    v[k]=base+double(residual[at7+k])*unit;
                }
                // Unit quaternions: the bank loader compares loop ends by an unnormalized dot product (>= 0.999999).
                // (A degenerate one stays as is; the loader's norm check refuses it.)
                const double n=std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]),scale=n>.5?1/n:1;
                for(int k=0;k<7;++k)out[at7+k]=float(k<4?v[k]*scale:v[k]);
            }
        }
        if(C.loop)std::memcpy(out.data()+std::size_t(frames-1)*J*7,out.data(),J*7*sizeof(float));
        decoded.push_back(out);release(c);
        return out;
    }
private:
    const Params& P;Sampler sample;std::vector<Rig> rigs;Pose bind;
    std::vector<std::size_t> lastUse,clipLastUse;std::vector<std::vector<float>> decoded;
    std::map<std::uint32_t,Sample> samples;std::map<std::uint64_t,Track> tracks;
    // The player's sample of an asset, as the evaluator gives it.
    const Sample& plain(std::uint32_t a){
        auto it=samples.find(a);if(it!=samples.end())return it->second;
        Sample s=sample(a);
        if(s.joints!=rigs[P.assets[a].skeleton].size()||!s.frames||s.values.size()!=std::size_t(s.frames)*s.joints*12)throw std::runtime_error("rebuild: sample size for "+P.assets[a].name);
        return samples.emplace(a,std::move(s)).first->second;
    }
    // By name copies use the evaluator's sample; retargets compose additive clips with their reference first
    // (retarget_native.py). Additive references are plain samples.
    Sample composed(std::uint32_t a){
        Sample s=plain(a);const auto& asset=P.assets[a];
        if(asset.additive==1)composeAdditive(s,additiveReference(plain(asset.ref[0]),nullptr));
        else if(asset.additive==2)composeAdditive(s,additiveReference(plain(asset.ref[0]),&plain(asset.ref[1])));
        return s;
    }
    const Track& track(const Params::Source& s,std::size_t c){
        const std::uint64_t key=(std::uint64_t(s.kind)<<40)|(std::uint64_t(s.index)<<8)|(s.variant<<1)|s.mirrored;
        auto it=tracks.find(key);if(it!=tracks.end())return it->second;
        Track t;const auto& rig=rigs[P.rig];
        if(s.kind==1){const auto& a=P.assets[s.index];const Rig& from=rigs[a.skeleton];t=s.variant?retarget(composed(s.index),from,rig):byName(plain(s.index),from,rig);}
        else{if(s.index>=decoded.size())throw std::runtime_error("rebuild: clip order");const auto& v=decoded[s.index];t.assign(frames,Pose(P.joints));
            for(unsigned f=0;f<frames;++f)for(std::size_t j=0;j<P.joints;++j)for(int k=0;k<7;++k)t[f][j][k]=v[(std::size_t(f)*P.joints+j)*7+k];}
        if(s.mirrored)t=mirror(t,rig);
        (void)c;return tracks.emplace(key,std::move(t)).first->second;
    }
    void release(std::size_t c){
        for(auto it=tracks.begin();it!=tracks.end();){const auto kind=it->first>>40,index=(it->first>>8)&0xffffffffu;
            const bool done=kind==1?lastUse[index]<=c:clipLastUse[index]<=c;it=done?tracks.erase(it):std::next(it);}
        for(auto it=samples.begin();it!=samples.end();)it=lastUse[it->first]<=c?samples.erase(it):std::next(it);
    }
};

}  // namespace d2bank
