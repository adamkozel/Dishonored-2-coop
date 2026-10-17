// #30: bank rebuild (bank_rebuild.h) on a synthetic skeleton and samples: the .edgeskl reader, rotation math,
// copies by name, retarget and mirror identities, parameters, and a patch encoded and decoded back with unit
// quaternions whose loop ends pass the bank loader.
#include "bank_rebuild.h"
#include "inflate.h"
#include <cstdio>
#include <cstdlib>

using namespace d2bank;
static void require(bool ok,const char* what){if(!ok){std::fprintf(stderr,"FAIL: %s\n",what);std::exit(1);}}
template<class F>static bool throws(F f){try{f();}catch(const std::runtime_error&){return true;}return false;}
static double angle(const Value& a,const Value& b){double d=0;for(int k=0;k<4;++k)d+=a[k]*b[k];return 2*std::acos(std::min(1.0,std::fabs(d)));}

// origin, anchor, root, spine, left/right arm, left/right elbow, a helper on the left elbow.
static const char* jointNames[]={"origin","anchor_jnt","center_root_jnt","center_spine01_jnt","left_arm_jnt","right_arm_jnt","left_elbow_jnt","right_elbow_jnt","left_helper_elbow_jnt"};
static const int parents[]={-1,0,0,2,3,3,4,5,6};
static Q unit(double x,double y,double z,double w){const double n=std::sqrt(x*x+y*y+z*z+w*w);return {x/n,y/n,z/n,w/n};}
static std::vector<Joint> joints(double tilt){
    std::vector<Joint> out;
    for(int i=0;i<9;++i){Joint j;j.name=jointNames[i];j.parent=parents[i];
        const Q q=i<2?Q{0,0,0,1}:unit(.1*i+tilt,-.05*i,.02*i*tilt,1);
        const double t[3]={i<2?0:.1*i,i==4||i==6?.2:i==5||i==7?-.2:0,i<2?0:.3};
        for(int k=0;k<4;++k)j.bind[k]=float(q[k]);for(int k=0;k<3;++k)j.bind[4+k]=float(t[k]);j.bind[7]=0;for(int k=8;k<12;++k)j.bind[k]=1;
        out.push_back(j);}
    // Mirror pairs share the bind pose reflected across Y.
    for(int k:{4,6}){auto& r=out[k+1];const auto& l=out[k];r.bind[0]=-l.bind[0];r.bind[1]=l.bind[1];r.bind[2]=-l.bind[2];r.bind[3]=l.bind[3];r.bind[4]=l.bind[4];r.bind[5]=-l.bind[5];r.bind[6]=l.bind[6];}
    return out;
}
// A minimal .edgeskl with the fields readSkeleton uses.
static Bytes edgeskl(const std::vector<Joint>& js){
    const std::size_t n=js.size(),table=0x40,names=table+4*n,parentMap=names+0x400,bind=parentMap+2*n+14;
    Bytes b(bind+48*n,0);
    auto put32=[&](std::size_t o,std::uint32_t v){std::memcpy(b.data()+o,&v,4);};auto put16=[&](std::size_t o,std::uint16_t v){std::memcpy(b.data()+o,&v,2);};
    std::memcpy(b.data(),"60SE",4);put32(4,std::uint32_t(b.size()));put16(0x10,std::uint16_t(n));
    put32(0x18,std::uint32_t(bind-0x18));put32(0x1c,std::uint32_t(parentMap-0x1c));put32(0x30,std::uint32_t(names-0x30));
    for(std::size_t i=0;i<n;++i){put16(table+4*i+2,js[i].parent<0?0x4000:std::uint16_t(js[i].parent));put16(parentMap+2*i,std::uint16_t(i));std::memcpy(b.data()+bind+48*i,js[i].bind.data(),48);}
    std::size_t p=names;put32(p,14);p+=4;put32(p,0);p+=4;p+=4;put16(p,std::uint16_t(n));put16(p+2,0);p+=4;
    for(const auto& j:js){put32(p,std::uint32_t(j.name.size()));std::memcpy(b.data()+p+4,j.name.data(),j.name.size());p+=4+j.name.size();}
    return b;
}
static Sample motion(const std::vector<Joint>& js,unsigned count,double phase){
    Sample s;s.joints=unsigned(js.size());s.frames=count;s.fps=30;s.values.assign(std::size_t(count)*s.joints*12,0);
    for(unsigned f=0;f<count;++f)for(unsigned j=0;j<s.joints;++j){
        float* v=s.values.data()+(std::size_t(f)*s.joints+j)*12;const double a=.3*std::sin(.2*f+phase+j);
        const Q q=j<2?Q{0,0,0,1}:qmul({js[j].bind[0],js[j].bind[1],js[j].bind[2],js[j].bind[3]},unit(a,.5*a,-.2*a,1));
        for(int k=0;k<4;++k)v[k]=float(q[k]);for(int k=0;k<3;++k)v[4+k]=js[j].bind[4+k]+(j==2?float(.05*a):0.f);for(int k=8;k<11;++k)v[k]=1;
    }
    return s;
}

int main(){
    try{
        // Rotation math.
        for(double a=-3;a<3;a+=.37){const Q q=unit(std::sin(a),std::cos(2*a),.3,a>0?.7:-.7);Q r=m2q(q2m(q));if(q[3]<0)for(auto& v:r)v=-v;
            double e=0;for(int k=0;k<4;++k)e=std::max(e,std::fabs(r[k]-q[k]));require(e<1e-12,"matrix and quaternion round trip");}
        // Skeleton file.
        const auto js=joints(.2);const auto parsed=readSkeleton(edgeskl(js));
        require(parsed.size()==9&&parsed[8].name=="left_helper_elbow_jnt"&&parsed[8].parent==6&&parsed[0].parent==-1&&parsed[4].bind[5]==js[4].bind[5],"edgeskl names, parents and bind");
        auto bad=edgeskl(js);bad[0]='x';require(throws([&]{readSkeleton(bad);}),"edgeskl magic checked");
        const Rig rig(parsed);
        // Copies by name and retargets onto the same skeleton keep the sample's local rotations.
        const Sample s=motion(js,12,0);const Track named=byName(s,rig,rig),moved=retarget(s,rig,rig);
        double e1=0,e2=0;
        for(unsigned f=0;f<s.frames;++f)for(unsigned j=3;j<8;++j){const float* v=s.at(f,j);const double n=std::sqrt(double(v[0])*v[0]+double(v[1])*v[1]+double(v[2])*v[2]+double(v[3])*v[3]);
            const Value x{v[0]/n,v[1]/n,v[2]/n,v[3]/n,0,0,0};e1=std::max(e1,angle(named[f][j],x));e2=std::max(e2,angle(moved[f][j],x));}
        require(e1<1e-6&&e2<1e-6,"by name and retarget onto the same skeleton");
        require(angle(moved[0][0],Value{0,0,0,1})<1e-12&&moved[0][2][4]==double(js[2].bind[4]),"retarget keeps joints 0-1 identity and bind translations without a body rig");
        // A helper follows its driver from the bind pose.
        {std::vector<M> R=rig.R;std::vector<V> T=rig.T;helpers(rig,R,T);require(angle(toPose(R,T)[8],bindPose(rig)[8])<1e-9,"helper stays at bind for a bind pose");}
        // Mirroring twice gives the pose back.
        const Track twice=mirror(mirror(moved,rig),rig);double e3=0;
        for(unsigned f=0;f<s.frames;++f)for(unsigned j=2;j<8;++j)e3=std::max(e3,angle(twice[f][j],moved[f][j]));
        require(e3<1e-6,"mirror is an involution");
        const Track once=mirror(moved,rig);require(angle(once[3][4],moved[3][4])>1e-3,"mirror swaps sides");
        // Fractional time.
        require(angle(at(moved,2.5,5),nlerp(moved[2][5],moved[3][5],.5))<1e-15&&angle(at(moved,99,5),moved.back()[5])<1e-12,"track time");
        // Parameters: one asset (by name and retargeted), clip 1 builds on mirrored clip 0.
        Out p;p.raw("D2BPR001",8);p.put(std::uint32_t(5));p.raw("corvo",5);p.put(std::uint32_t(9));p.put(std::uint32_t(2));
        const std::uint8_t part[9]={255,255,0,0,1,1,1,1,1};p.raw(part,9);
        p.put(std::uint32_t(1));p.put(std::uint32_t(4));p.raw("rig0",4);p.put(std::uint32_t(0));
        p.put(std::uint32_t(1));p.put(std::uint32_t(4));p.raw("anim",4);p.put(std::uint32_t(0));p.put(std::uint8_t(0));p.put(std::uint32_t(~0u));p.put(std::uint32_t(~0u));
        p.put(std::uint32_t(2));
        for(int c=0;c<2;++c){
            p.put(std::uint8_t(c==0));
            if(c==0){p.put(std::uint32_t(2));p.put(std::uint8_t(1));p.put(std::uint32_t(0));p.put(std::uint8_t(0));p.put(std::uint8_t(0));p.put(std::uint8_t(1));p.put(std::uint32_t(0));p.put(std::uint8_t(1));p.put(std::uint8_t(0));}
            else{p.put(std::uint32_t(1));p.put(std::uint8_t(2));p.put(std::uint32_t(0));p.put(std::uint8_t(0));p.put(std::uint8_t(1));}
            for(unsigned f=0;f<frames;++f)for(int k=0;k<2;++k){const float u=float(f%11)+.25f;
                p.put(std::uint16_t(1));p.put(u);p.put(std::uint16_t(c==0?2:1));p.put(c==0?u*.5f:float(f));p.put(c==0&&k?.25f:0.f);}
        }
        In in{p.d};const Params params=readParams(in);
        require(in.at==p.d.size()&&params.clips.size()==2&&params.clips[1].sources[0].kind==2&&params.clips[0].loop&&params.parts==2,"parameters read");
        // A "lab bank": each clip's base plus edits and sign flips. Encoding against zeros measures the base.
        const std::uint32_t step=17;const double unit=std::ldexp(1.0,-int(step));std::vector<std::vector<float>> lab;
        auto sampler=[&](std::size_t){return motion(js,12,0);};
        const std::vector<std::uint32_t> none(frames,noCopy);
        // Clip 1 frame 7 repeats clip 0 frame 3; clip 1 frame 9 repeats its own frame 2.
        std::vector<std::uint32_t> copies=none;copies[7]=3;copies[9]=frames+2;
        auto measured=[&](Rebuilder& r,std::size_t c){std::vector<std::uint8_t> fl;std::vector<std::int32_t> res;std::vector<float> zero(std::size_t(frames)*9*7,0.f);
            r.clip(c,zero.data(),step,fl,res,none);std::vector<float> v(zero.size());for(std::size_t k=0;k<v.size();++k)v[k]=float(-double(res[k])*unit);
            for(std::size_t k=0;k<v.size();k+=7)if((k/7)%5==3&&(k/7)%9>=2)for(int q=0;q<4;++q)v[k+q]=-v[k+q];
            v[7*20+4]+=.01f;
            for(std::size_t k=0;k<v.size();k+=7){double n=0;for(int q=0;q<4;++q)n+=double(v[k+q])*v[k+q];n=std::sqrt(n);for(int q=0;q<4;++q)v[k+q]=float(v[k+q]/n);}  // banks hold unit quaternions
            if(params.clips[c].loop)std::memcpy(v.data()+std::size_t(frames-1)*9*7,v.data(),9*7*4);return v;};
        {Rebuilder x(params,{parsed},sampler);lab.push_back(measured(x,0));}
        {Rebuilder y(params,{parsed},sampler);std::vector<std::uint8_t> fl;std::vector<std::int32_t> res;y.clip(0,lab[0].data(),step,fl,res,none);lab.push_back(measured(y,1));}
        std::memcpy(lab[1].data()+7*9*7,lab[0].data()+3*9*7,9*7*4);std::memcpy(lab[1].data()+9*9*7,lab[1].data()+2*9*7,9*7*4);  // the copies
        Rebuilder enc(params,{parsed},sampler);Patch patch;patch.params=params;double worst=0;
        for(std::size_t c=0;c<2;++c){std::vector<std::uint8_t> fl;std::vector<std::int32_t> res;auto v=enc.clip(c,lab[c].data(),step,fl,res,c?copies:none);patch.refs.push_back(c?copies:none);
            for(std::size_t k=0;k<v.size();++k)worst=std::max(worst,double(std::fabs(v[k]-lab[c][k])));
            patch.flips.push_back(fl);patch.residual.push_back(res);patch.clipHeaders.push_back({});patch.sums.push_back({0,0});patch.skeletons.push_back({});lab[c]=v;}
        patch.skeletons.resize(1);patch.assets.resize(1);
        require(worst<=unit*1.5*1.01,"encoded values within three half-steps of the lab bank (quaternions are renormalized)");
        require(!std::memcmp(lab[0].data(),lab[0].data()+std::size_t(frames-1)*9*7,9*7*4),"loop clip repeats its first frame");
        for(std::size_t k=0;k<lab[1].size();k+=7){double n=0;for(int q=0;q<4;++q)n+=double(lab[1][k+q])*lab[1][k+q];require(std::fabs(n-1)<1e-6,"rebuilt quaternions are unit");}
        // Patch file round trip, then decoding gives the encoder's values exactly.
        const Bytes file=writePatch(patch,p.d);
        const Patch back=readPatch(file,[](const std::uint8_t* d,std::size_t n,std::size_t e){return d2inflate::zlib(d,n,e);});
        require(back.residual==patch.residual&&back.flips==patch.flips&&back.params.clips.size()==2,"patch round trip");
        Rebuilder dec(back.params,{parsed},[&](std::size_t){return motion(js,12,0);});
        for(std::size_t c=0;c<2;++c){auto fl=back.flips[c];auto res=back.residual[c];auto v=dec.clip(c,nullptr,back.step,fl,res,back.refs[c]);require(v==lab[c],"decoded clip equals the encoded clip");}
        require(!std::memcmp(lab[1].data()+7*9*7,lab[0].data()+3*9*7,9*7*4)&&!std::memcmp(lab[1].data()+9*9*7,lab[1].data()+2*9*7,9*7*4),"copied frames are exact");
        Rebuilder short_(back.params,{parsed},[&](std::size_t){return motion(js,12,0);});std::vector<std::uint8_t> fl;std::vector<std::int32_t> res(3);
        require(throws([&]{short_.clip(0,nullptr,17,fl,res,none);}),"residual size checked");
        {Rebuilder late(back.params,{parsed},sampler);auto f2=back.flips[0];auto r2=back.residual[0];auto later=none;later[4]=6;
            require(throws([&]{late.clip(0,nullptr,17,f2,r2,later);}),"copy from a later frame refused");}
        auto truncated=file;truncated.pop_back();require(throws([&]{readPatch(truncated,[](const std::uint8_t* d,std::size_t n,std::size_t e){return d2inflate::zlib(d,n,e);});}),"truncated patch refused");
        std::puts("d2_bank_rebuild_tests passed");
        return 0;
    }catch(const std::exception& e){std::fprintf(stderr,"FAIL: uncaught %s\n",e.what());return 1;}
}
