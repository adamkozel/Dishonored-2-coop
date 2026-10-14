#include "native_skeleton.h"
#include "body_retarget.h"
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cstdlib>
struct Memory {
    std::unordered_map<std::uintptr_t,unsigned char> bytes;
    void put(std::uintptr_t p,const void* data,std::size_t n){const auto* b=static_cast<const unsigned char*>(data);for(std::size_t i=0;i<n;++i)bytes[p+i]=b[i];}
    void q(std::uintptr_t p,std::uint64_t n){put(p,&n,8);}void d(std::uintptr_t p,std::uint32_t n){put(p,&n,4);}void w(std::uintptr_t p,std::uint16_t n){put(p,&n,2);}
    bool read(std::uintptr_t p,void* data,std::size_t n){auto* b=static_cast<unsigned char*>(data);for(std::size_t i=0;i<n;++i){auto it=bytes.find(p+i);if(it==bytes.end())return false;b[i]=it->second;}return true;}
};
void require(bool ok,const char* text){if(!ok){std::cerr<<"FAIL: "<<text<<'\n';std::exit(1);}}
void retargetTests(){
    NativeSkeleton s{},t{};s.parents={-1,0,1,2,3,4,5,6,6,1,9,10,11,12,1,14,15,16,17,0};
    for(unsigned i=0;i<20;++i){s.jointIds.push_back(static_cast<std::uint16_t>(i));s.bindLocal.push_back({0,0,0,1,0,0,i?.1f:0.f,0,1,1,1,1});}
    t=s;t.parents.insert(t.parents.end(),{1,5});t.parents[9]=t.parents[14]=20;t.jointIds[8]=888;t.jointIds.insert(t.jointIds.end(),{999,1000});
    t.bindLocal.push_back({0,0,0,1,0,0,.03f,0,1,1,1,1});t.bindLocal.push_back({0,0,0,1,.2f,0,0,0,1,1,1,1});
    BodyRetarget r;require(prepareBodyRetarget(s,t,r),"map body around additional pelvis and arm joints");
    std::array<BodyMatrix,20> pose{};std::copy(r.sourceBindGlobal.begin(),r.sourceBindGlobal.end(),pose.begin());std::vector<BodyMatrix> result,global;
    require(retargetBody(r,pose,r.targetBindLocal,result)&&bodyForward(r.parents,result,global),"retarget source bind pose");
    for(unsigned j=0;j<global.size();++j)for(unsigned c=0;c<4;++c)for(unsigned v=0;v<3;++v)require(std::fabs(global[j].v[c*4+v]-r.targetBindGlobal[j].v[c*4+v])<.00001f,"target bind roundtrip");
    auto native=r.targetBindLocal;native[21]={{0,1,0,17,-1,0,0,18,0,0,1,19,.2f,0,0,20}};
    pose[1].v[14]-=.05f;require(retargetBody(r,pose,native,result),"source body translation");
    require(std::fabs(result[1].v[14]-.05f)<.00001f,"hip translation propagated");
    require(std::memcmp(result[21].v,native[21].v,64)==0,"unmapped native arm and padding untouched");
    for(unsigned j=0;j<result.size();++j)if(j!=1)for(unsigned v=0;v<3;++v)require(result[j].v[12+v]==native[j].v[12+v],"other bone translations preserved");
    auto bad=pose;bad[9].v[0]=2;require(!retargetBody(r,bad,native,result),"scaled or corrupt source pose refused");
    bad=pose;bad[9].v[0]=-1;require(!retargetBody(r,bad,native,result),"reflected source pose refused");
    auto badRig=r;badRig.parents[1]=2;require(!retargetBody(badRig,pose,native,result),"retarget hierarchy cycle refused");
    auto badTarget=t;badTarget.jointIds[9]=888;require(!prepareBodyRetarget(s,badTarget,r),"missing semantic body joint refused");
    auto padded=bodyIdentity();padded.v[3]=padded.v[7]=padded.v[11]=padded.v[15]=std::numeric_limits<float>::quiet_NaN();
    const auto composed=bodyMultiply(padded,bodyIdentity());require(bodyRigid(composed),"unused SIMD lanes cannot contaminate affine math");
    std::cout<<"PASS: retarget bind roundtrip, inserted pelvis, native arm preservation and invalid-transform refusal\n";
}
int main(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000,list=stack+0x1000;
    constexpr std::uintptr_t anim=0x500000000,render=0x600000000,model=0x700000000,skel=0x800000000;
    constexpr std::uintptr_t blob=0x900000000,ids=blob+0x1000,headers=blob+0x2000,remap=blob+0x3000,cache=blob+0x4000,local=blob+0x5000,global=blob+0x6000,name=blob+0x7000;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);
    m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,anim);
    const float root[12]={0,0,0,1,0,0,0,1,0,0,0,1};m.put(actor+0x300,root,sizeof(root));
    m.q(anim,base+0x1b80408);m.q(anim+0x10,actor);m.q(anim+0x38,render);m.q(render,base+0x1b0d628);m.q(render+0x1fe8,model);
    m.q(model,base+0x1b05218);m.q(model+8,name);m.q(model+0x50,skel);m.q(skel,base+0x1b0bd08);m.q(skel+8,name);
    char text[160]="test";m.put(name,text,sizeof(text));m.q(skel+0x70,ids);m.d(skel+0x78,3);m.q(skel+0x158,blob);m.d(skel+0x160,512);
    unsigned char data[512]{};m.put(blob,data,sizeof(data));m.d(blob,0x45533036);m.d(blob+0x10,3);m.d(blob+0x18,0x100-0x18);m.d(blob+0x1c,0x80-0x1c);
    const float qts[12]={0,0,0,1,0,0,1,0,1,1,1,1};const float matrix[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0};
    for(unsigned i=0;i<3;++i){m.w(ids+i*2,static_cast<std::uint16_t>(7+i));m.w(blob+0x80+i*2,static_cast<std::uint16_t>(i));m.w(blob+0x42+i*4,i?static_cast<std::uint16_t>(i-1):0x7fff);m.put(blob+0x100+i*48,qts,sizeof(qts));m.w(remap+i*2,static_cast<std::uint16_t>(i));for(auto p:{cache,local,global})m.put(p+i*64,matrix,64);}
    m.q(anim+0x80,skel);m.q(anim+0x188,local);m.d(anim+0x190,3);m.q(anim+0x198,global);m.d(anim+0x1a0,3);
    m.q(anim+0x2d8,cache);m.d(anim+0x2e0,3);m.q(anim+0x2e8,headers);m.d(anim+0x2f0,1);m.q(headers,remap);m.d(headers+8,3);
    NativeSkeleton s;const auto inspect=[&](Memory& mem){return readNativeSkeleton([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,1,s);};
    require(inspect(m)&&s.stable&&s.parents==std::vector<std::int16_t>{-1,0,1}&&s.jointIds.size()==3,"valid skeleton fixture");
    auto bad=m;bad.q(anim+0x10,actor+8);require(!inspect(bad),"wrong animation owner");
    bad=m;bad.q(render,base+0x1b05218);require(!inspect(bad),"wrong skinned render type");
    bad=m;bad.q(model,base+0x1b0d628);require(!inspect(bad),"wrong model declaration type");
    bad=m;bad.q(anim+0x80,skel+8);require(!inspect(bad),"pose belongs to a different skeleton");
    bad=m;bad.q(anim+0x188,local+1);require(!inspect(bad),"misaligned native SIMD pose storage");
    bad=m;bad.d(anim+0x190,2);require(!inspect(bad),"pose count differs from skeleton");
    bad=m;bad.w(ids+2,7);require(!inspect(bad),"duplicate joint identity");
    bad=m;bad.w(blob+0x42+4,2);require(!inspect(bad),"joint hierarchy cycle");
    bad=m;bad.w(blob+0x42+4,4);require(!inspect(bad),"out-of-range parent");
    bad=m;bad.d(blob+0x18,0xfffffff0);require(!inspect(bad),"overflowed relative bind address");
    bad=m;bad.w(blob+0x80,65535);require(!inspect(bad),"out-of-range parent indirection");
    bad=m;bad.w(remap+2,4);require(!inspect(bad),"out-of-range cache index");
    bad=m;bad.d(stack,15);require(!inspect(bad),"loading state");
    bad=m;bad.d(game+0x1f7400,2);require(!inspect(bad),"pending transition");
    bad=m;const float nan=std::numeric_limits<float>::quiet_NaN();bad.put(local,&nan,4);require(!inspect(bad),"nonfinite pose");
    unsigned reads=0;bad=m;
    require(!readNativeSkeleton([&](auto p,auto d,auto n){bool ok=bad.read(p,d,n);if(p==anim+0x188&&++reads==1)bad.q(p,local+64);return ok;},base,1,s),"pose replacement during read");
    reads=0;bad=m;
    require(!readNativeSkeleton([&](auto p,auto d,auto n){bool ok=bad.read(p,d,n);if(p==blob&&++reads==1)bad.q(skel+0x158,blob+64);return ok;},base,1,s),"bind allocation replaced during read");
    reads=0;bad=m;
    require(readNativeSkeleton([&](auto p,auto d,auto n){bool ok=bad.read(p,d,n);if(p==local&&++reads==1){const float value=.5f;bad.put(p+12*4,&value,4);}return ok;},base,1,s)&&!s.stable,"pose changing in place is marked unstable");
    std::cout<<"PASS: skeleton identity, hierarchy, bounded layout, native pose ownership and concurrent replacement guards\n";
    retargetTests();
}

