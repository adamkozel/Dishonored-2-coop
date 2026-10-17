#pragma once
// #30 (stage 7 release): rebuilds the native tables (native_tables.h) from the player's animations, so the DLL
// carries no game-derived data. Parameters come from tools/release/build_native_tables.py (D2NTP001):
//   kind 0 / 1: a skeleton's parents (i16) / bind pose (f32 x 7 per joint), read from the player's .edgeskl;
//   kind 2: an exact Edge sample of a prop animation (f32 frames x joints x 7);
//   kind 3: ranged hand tracks, the prop attachment relative to the wrist in first-person samples at given times,
//           rotated by the hand calibration between the first-person and body skeletons (author_items.py),
//           completed by a quantized residual.
// The release ships a recipe (D2NTR001): the input hashes, per-table sums, then a zlib body with the parameters
// and the kind 3 residuals. Samples may differ in their last bits between CPUs (bank_rebuild.h), so the installer
// checks sums with a tolerance.
#include "bank_rebuild.h"

namespace d2tables {

using d2bank::Bytes;using d2bank::Hash;using d2bank::In;using d2bank::Out;using d2bank::M;using d2bank::V;using d2bank::Q;

struct Params {
    struct Choice {std::uint32_t asset=0;float u=0;};
    struct Entry {std::string name;std::uint8_t kind=0;std::uint32_t source=0,rig=0,fp=0,tracks=0,frames=0;std::vector<Choice> choices;};
    std::vector<std::string> skeletons;std::vector<std::pair<std::string,std::uint32_t>> assets;std::vector<Entry> entries;
};
inline Params readParams(In& r){
    r.need(8);if(std::memcmp(r.d.data()+r.at,"D2NTP001",8))throw std::runtime_error("table parameters: bad magic");r.at+=8;
    Params p;for(auto n=r.get<std::uint32_t>();n--;)p.skeletons.push_back(r.str());
    for(auto n=r.get<std::uint32_t>();n--;){auto name=r.str();const auto s=r.get<std::uint32_t>();if(s>=p.skeletons.size())throw std::runtime_error("table parameters: asset skeleton");p.assets.emplace_back(std::move(name),s);}
    for(auto n=r.get<std::uint32_t>();n--;){
        Params::Entry e;e.name=r.str();e.kind=r.get<std::uint8_t>();
        if(e.kind<=1){e.source=r.get<std::uint32_t>();if(e.source>=p.skeletons.size())throw std::runtime_error("table parameters: skeleton");}
        else if(e.kind==2){e.source=r.get<std::uint32_t>();if(e.source>=p.assets.size())throw std::runtime_error("table parameters: asset");}
        else if(e.kind==3){
            e.rig=r.get<std::uint32_t>();e.fp=r.get<std::uint32_t>();e.tracks=r.get<std::uint32_t>();e.frames=r.get<std::uint32_t>();
            if(e.rig>=p.skeletons.size()||e.fp>=p.skeletons.size()||!e.tracks||e.tracks>64||!e.frames||e.frames>4096)throw std::runtime_error("table parameters: hand tracks");
            e.choices.resize(std::size_t(e.tracks)*e.frames);
            for(auto& c:e.choices){c.asset=r.get<std::uint32_t>();c.u=r.get<float>();if(c.asset>=p.assets.size()||p.assets[c.asset].second!=e.fp||!(c.u>=0&&c.u<100000))throw std::runtime_error("table parameters: hand track choice");}
        }else throw std::runtime_error("table parameters: kind");
        p.entries.push_back(std::move(e));
    }
    return p;
}

// A rebuilt table: f32 or i16 values and their dimensions.
struct Table {std::string name;bool i16=false;std::vector<std::uint32_t> dims;std::vector<float> f;std::vector<std::int16_t> s;};

struct Recipe {
    Hash exe{};std::vector<Hash> skeletons,assets;std::vector<std::array<double,2>> sums;std::uint32_t step=20;
    Params params;std::vector<std::vector<std::uint8_t>> flips;std::vector<std::vector<std::int32_t>> residual;  // per entry; empty unless kind 3
};
inline Recipe readRecipe(const Bytes& d,const std::function<Bytes(const std::uint8_t*,std::size_t,std::size_t)>& inflate){
    In r{d};r.need(8);if(std::memcmp(d.data(),"D2NTR001",8))throw std::runtime_error("table recipe: bad magic");r.at=8;
    Recipe x;x.exe=r.hash();for(auto n=r.get<std::uint32_t>();n--;)x.skeletons.push_back(r.hash());for(auto n=r.get<std::uint32_t>();n--;)x.assets.push_back(r.hash());
    for(auto n=r.get<std::uint32_t>();n--;)x.sums.push_back({r.get<double>(),r.get<double>()});
    x.step=r.get<std::uint32_t>();if(x.step<8||x.step>30)throw std::runtime_error("table recipe: step");
    const auto bodySize=r.get<std::uint64_t>();const auto streamSize=r.get<std::uint32_t>();r.need(streamSize);
    if(bodySize>(std::uint64_t(1)<<28))throw std::runtime_error("table recipe: body size");
    const Bytes body=inflate(d.data()+r.at,streamSize,std::size_t(bodySize));r.at+=streamSize;if(r.at!=d.size())throw std::runtime_error("table recipe: trailing bytes");
    In b{body};x.params=d2tables::readParams(b);
    if(x.skeletons.size()!=x.params.skeletons.size()||x.assets.size()!=x.params.assets.size()||x.sums.size()!=x.params.entries.size())throw std::runtime_error("table recipe: counts");
    for(const auto& e:x.params.entries){
        if(e.kind!=3){x.flips.emplace_back();x.residual.emplace_back();continue;}
        const std::size_t n=std::size_t(e.tracks)*e.frames;b.need((n+7)/8);x.flips.emplace_back(body.begin()+std::ptrdiff_t(b.at),body.begin()+std::ptrdiff_t(b.at+(n+7)/8));b.at+=(n+7)/8;
        b.need(n*28);std::vector<std::int32_t> res(n*7);std::memcpy(res.data(),body.data()+b.at,n*28);b.at+=n*28;x.residual.push_back(std::move(res));
    }
    if(b.at!=body.size())throw std::runtime_error("table recipe: body trailing bytes");
    return x;
}
inline Bytes writeRecipe(const Recipe& x,const Bytes& paramsBytes){
    Out body;body.raw(paramsBytes.data(),paramsBytes.size());
    for(std::size_t k=0;k<x.params.entries.size();++k)if(x.params.entries[k].kind==3){body.raw(x.flips[k].data(),x.flips[k].size());body.raw(x.residual[k].data(),x.residual[k].size()*4);}
    const Bytes stream=d2bank::storedZlib(body.d);
    Out o;o.raw("D2NTR001",8);o.raw(x.exe.data(),32);
    o.put(std::uint32_t(x.skeletons.size()));for(const auto& h:x.skeletons)o.raw(h.data(),32);
    o.put(std::uint32_t(x.assets.size()));for(const auto& h:x.assets)o.raw(h.data(),32);
    o.put(std::uint32_t(x.sums.size()));for(const auto& s:x.sums){o.put(s[0]);o.put(s[1]);}
    o.put(x.step);o.put(std::uint64_t(body.d.size()));o.put(std::uint32_t(stream.size()));o.raw(stream.data(),stream.size());
    return o.d;
}
inline std::array<double,2> sums(const Table& t){std::array<double,2> s{};for(float v:t.f){s[0]+=v;s[1]+=double(v)*v;}for(auto v:t.s){s[0]+=v;s[1]+=double(v)*v;}return s;}

// Output file (D2NTB001), read by the DLL.
inline Bytes writeTables(const std::vector<Table>& tables){
    Out o;o.raw("D2NTB001",8);o.put(std::uint32_t(tables.size()));
    for(const auto& t:tables){o.put(std::uint32_t(t.name.size()));o.raw(t.name.data(),t.name.size());o.put(std::uint8_t(t.i16?1:0));o.put(std::uint32_t(t.dims.size()));
        for(auto d:t.dims)o.put(d);if(t.i16)o.raw(t.s.data(),t.s.size()*2);else o.raw(t.f.data(),t.f.size()*4);}
    return o.d;
}

// The palm frame of a hand: wrist-local positions of the index, pinky and middle joints (finger_reference.py palm()).
inline M palm(const d2bank::Rig& r,int w,int i,int p,int m){
    auto local=[&](int k){const V d={r.GT[k][0]-r.GT[w][0],r.GT[k][1]-r.GT[w][1],r.GT[k][2]-r.GT[w][2]};return d2bank::mul(d2bank::tr(r.GR[w]),d);};
    const V a=local(i),b=local(p),c=local(m);V x={a[0]-b[0],a[1]-b[1],a[2]-b[2]};
    auto norm=[](V& v){const double n=std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);if(!(n>1e-12))throw std::runtime_error("palm: degenerate");for(auto& e:v)e/=n;};
    norm(x);const double cx=c[0]*x[0]+c[1]*x[1]+c[2]*x[2];V y={c[0]-x[0]*cx,c[1]-x[1]*cx,c[2]-x[2]*cx};norm(y);
    const V z={x[1]*y[2]-x[2]*y[1],x[2]*y[0]-x[0]*y[2],x[0]*y[1]-x[1]*y[0]};
    return {x[0],y[0],z[0],x[1],y[1],z[1],x[2],y[2],z[2]};
}
// Rotation from first-person hand space to body hand space (HandReference.invcal).
inline M handCalibration(const d2bank::Rig& fp,const d2bank::Rig& rig){
    auto f=[](const d2bank::Rig& r,const char* n){const int k=r.find(n);if(k<0)throw std::runtime_error(std::string("hand calibration: no ")+n);return k;};
    const M sp=palm(fp,4,f(fp,"left_index_02_jnt"),f(fp,"left_pinky_02_jnt"),f(fp,"left_medium_02_jnt"));
    const M tp=palm(rig,9,f(rig,"left_index01_jnt"),f(rig,"left_pinky02_jnt"),f(rig,"left_middle01_jnt"));
    return d2bank::mul(tp,d2bank::tr(sp));
}
// The prop attachment relative to the wrist (joint 4) of a first-person sample at fractional frame u.
inline d2bank::Value attachment(const d2bank::Sample& s,const d2bank::Rig& fp,const M& cal,double u){
    const unsigned g=std::min(unsigned(std::floor(u)),s.frames-1),h=std::min(g+1,s.frames-1);const double t=h>g?u-g:0.0;
    const int att=fp.find("left_hand_attachment_jnt");if(att<0)throw std::runtime_error("hand track: no attachment joint");
    std::vector<M> R(fp.size()),GR(fp.size());std::vector<V> T(fp.size()),GT(fp.size());
    for(unsigned j=0;j<fp.size();++j){
        const float* a=s.at(g,j);const float* b=s.at(h,j);double dot=0;for(int k=0;k<4;++k)dot+=double(a[k])*b[k];const double sign=dot<0?-1:1;
        Q q;double n=0;for(int k=0;k<4;++k){q[k]=a[k]*(1-t)+b[k]*sign*t;n+=q[k]*q[k];}n=std::sqrt(n);for(auto& v:q)v/=n;
        R[j]=d2bank::q2m(q);for(int k=0;k<3;++k)T[j][k]=a[4+k]*(1-t)+b[4+k]*t;
    }
    fp.fk(R,T,GR,GT);
    const M r=d2bank::mul(d2bank::tr(GR[4]),GR[att]);const V d={GT[att][0]-GT[4][0],GT[att][1]-GT[4][1],GT[att][2]-GT[4][2]};const V rt=d2bank::mul(d2bank::tr(GR[4]),d);
    const Q q=d2bank::m2q(d2bank::mul(cal,r));const V tt=d2bank::mul(cal,rt);
    return {q[0],q[1],q[2],q[3],tt[0],tt[1],tt[2]};
}

class Builder {
public:
    using Sampler=std::function<d2bank::Sample(std::size_t asset)>;
    Builder(const Params& p,const std::vector<std::vector<d2bank::Joint>>& skeletons,Sampler sampler):P(p),joints(skeletons),sample(std::move(sampler)){
        if(joints.size()!=P.skeletons.size())throw std::runtime_error("tables: skeleton count");
        for(const auto& s:joints)rigs.emplace_back(s);
    }
    // Encoding passes the reference table to measure kind 3 residuals; decoding passes the recipe's.
    Table build(std::size_t index,const Table* reference,std::uint32_t step,std::vector<std::uint8_t>& flips,std::vector<std::int32_t>& residual){
        const auto& e=P.entries[index];Table t;t.name=e.name;
        if(e.kind==0){t.i16=true;const auto& js=joints[e.source];t.dims={std::uint32_t(js.size())};for(const auto& j:js)t.s.push_back(std::int16_t(j.parent));}
        else if(e.kind==1){const auto& js=joints[e.source];t.dims={std::uint32_t(js.size()),7};for(const auto& j:js)t.f.insert(t.f.end(),j.bind.begin(),j.bind.begin()+7);}
        else if(e.kind==2){const auto& s=cached(e.source);t.dims={s.frames,s.joints,7};
            for(unsigned f=0;f<s.frames;++f)for(unsigned j=0;j<s.joints;++j){const float* v=s.at(f,j);t.f.insert(t.f.end(),v,v+7);}}
        else{
            const auto& fp=rigs[e.fp];const M cal=handCalibration(fp,rigs[e.rig]);const double unit=std::ldexp(1.0,-int(step));
            const std::size_t n=std::size_t(e.tracks)*e.frames;t.dims={e.tracks,e.frames,7};t.f.resize(n*7);
            if(reference){if(reference->f.size()!=n*7)throw std::runtime_error("tables: reference size for "+e.name);flips.assign((n+7)/8,0);residual.assign(n*7,0);}
            else if(flips.size()!=(n+7)/8||residual.size()!=n*7)throw std::runtime_error("tables: residual size for "+e.name);
            for(std::size_t k=0;k<n;++k){
                const auto& c=e.choices[k];const auto& s=cached(c.asset);if(s.joints!=fp.size())throw std::runtime_error("tables: hand sample skeleton");
                const auto v=attachment(s,fp,cal,c.u);
                if(reference){double dot=0;for(int q=0;q<4;++q)dot+=double(reference->f[k*7+q])*v[q];if(dot<0)flips[k/8]|=std::uint8_t(1u<<(k%8));}
                const bool flip=(flips[k/8]>>(k%8))&1;
                for(int q=0;q<7;++q){const double base=q<4&&flip?-v[q]:v[q];
                    if(reference){const double r=std::nearbyint((double(reference->f[k*7+q])-base)/unit);if(!(std::fabs(r)<2147483647.0))throw std::runtime_error("tables: residual range");residual[k*7+q]=std::int32_t(r);}
                    t.f[k*7+q]=float(base+double(residual[k*7+q])*unit);}
            }
        }
        return t;
    }
private:
    const Params& P;std::vector<std::vector<d2bank::Joint>> joints;Sampler sample;std::vector<d2bank::Rig> rigs;std::map<std::uint32_t,d2bank::Sample> samples;
    const d2bank::Sample& cached(std::uint32_t a){
        auto it=samples.find(a);if(it!=samples.end())return it->second;
        auto s=sample(a);if(s.joints!=joints[P.assets[a].second].size())throw std::runtime_error("tables: sample joints for "+P.assets[a].first);
        return samples.emplace(a,std::move(s)).first->second;
    }
};

}  // namespace d2tables
