#include "native_skeleton.h"
#include "body_retarget.h"
#include "arm_retarget.h"
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
void armRetargetTests(){
    NativeSkeleton source{},target{};source.playerArm.side=2;
    strcpy_s(source.skeletonName,"models/characters/player/emily/player_rightarm_emily.md6skl");
    strcpy_s(target.skeletonName,"models/characters/female/emily/emily.md6skl");
    for(unsigned j=0;j<69;++j){source.jointIds.push_back(static_cast<std::uint16_t>(100+j));source.parents.push_back(j?0:-1);source.bindLocal.push_back({0,0,0,1,0,.01f,0,0,1,1,1,1});}
    source.parents[2]=1;source.parents[3]=2;source.parents[4]=3;source.parents[59]=4;
    for(unsigned j=0;j<24;++j){target.jointIds.push_back(static_cast<std::uint16_t>(300+j));target.parents.push_back(j?0:-1);target.bindLocal.push_back({0,0,0,1,0,.03f,0,0,1,1,1,1});}
    for(unsigned k=0;k<5;++k){target.jointIds[13+k]=source.jointIds[kArmCoreJoints[k]];target.parents[13+k]=k?static_cast<std::int16_t>(12+k):5;}
    ArmRetarget rig{};require(prepareArmRetarget(source,target,rig),"five semantically matched arm joints");
    std::vector<BodyMatrix> result,global;
    require(retargetArm(rig,rig.sourceBindGlobal,rig.targetBindLocal,result)&&bodyForward(rig.parents,result,global),"arm bind roundtrip");
    for(unsigned j=0;j<24;++j)for(unsigned c=0;c<4;++c)for(unsigned a=0;a<3;++a)
        require(std::fabs(global[j].v[c*4+a]-rig.targetBindGlobal[j].v[c*4+a])<.00001f,"native target arm bind pose preserved");
    auto pose=rig.sourceBindGlobal;const auto rotation=BodyMatrix{{0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,0}};
    for(auto& p:pose)p=bodyMultiply(rotation,p);
    auto native=rig.targetBindLocal;native[20]=rotation;native[14].v[13]+=.01f;
    require(retargetArm(rig,pose,native,result),"real arm rotation basis applies");
    require(std::fabs(result[13].v[1])>.9f,"mapped arm rotates");
    for(unsigned j=0;j<24;++j){
        if(j<13||j>17)require(!std::memcmp(&result[j],&native[j],sizeof(BodyMatrix)),"body, other arm and finger joints remain native");
        for(unsigned a=0;a<3;++a)require(result[j].v[12+a]==native[j].v[12+a],"arm application preserves native target translations");
    }
    auto badPose=pose;badPose[2].v[0]=2;require(!retargetArm(rig,badPose,native,result),"scaled arm pose refused");
    auto badRig=rig;badRig.targets[3]=badRig.targets[2];require(!retargetArm(badRig,pose,native,result),"duplicate arm destinations refused");
    auto bad=source;bad.parents[59]=3;require(!prepareArmRetarget(bad,target,rig),"unexpected first-person weapon parent refused");
    bad=source;bad.playerArm.side=1;require(!prepareArmRetarget(bad,target,rig),"side/skeleton mismatch refused");
    auto badTarget=target;badTarget.parents[15]=5;require(!prepareArmRetarget(source,badTarget,rig),"incompatible native arm hierarchy refused");
    badTarget=target;badTarget.jointIds[23]=target.jointIds[13];require(!prepareArmRetarget(source,badTarget,rig),"ambiguous native arm name refused");
    std::cout<<"PASS: five-joint arm retarget bind roundtrip, native translations and unmapped poses, finite rigid inputs and exact chain identity\n";
    for(bool corvo:{false,true}){
        auto handSource=source,handTarget=target;
        strcpy_s(handTarget.skeletonName,corvo?"models/characters/tall/str/corvo/t_str_corvo01.md6skl":"models/characters/female/emily/emily.md6skl");
        const unsigned count=corvo?215:219,distal=corvo?63:60;
        for(unsigned j=24;j<count;++j){handTarget.jointIds.push_back(static_cast<std::uint16_t>(300+j));handTarget.parents.push_back(0);handTarget.bindLocal.push_back({0,0,0,1,0,.02f,0,0,1,1,1,1});}
        const int sourceParents[]={4,5,6,15,16,17,26,27,28,37,38,39,4,48,49,50};
        const unsigned mapped[]={38,39,distal,40,41,distal+2,42,43,distal+4,44,45,distal+6,46,47,48,distal+8};
        const int targetParents[]={16,38,39,16,40,41,16,42,43,16,44,45,16,46,47,48};
        for(unsigned i=0;i<16;++i){handSource.parents[kArmStreamJoints[i+5]]=static_cast<std::int16_t>(sourceParents[i]);handTarget.parents[mapped[i]]=static_cast<std::int16_t>(targetParents[i]);}
        for(unsigned j:{15u,26u,37u})handSource.parents[j]=4;
        ArmRetarget hand{};require(prepareArmRetarget(handSource,handTarget,hand,true)&&hand.count==21,"known Corvo/Emily phalanx mapping");
        require(retargetArm(hand,hand.sourceBindGlobal,hand.targetBindLocal,result)&&bodyForward(hand.parents,result,global),"full hand bind roundtrip");
        for(unsigned j=0;j<count;++j)for(unsigned k=0;k<16;++k)require(std::fabs(result[j].v[k]-hand.targetBindLocal[j].v[k])<.00001f,"native full hand bind pose preserved");
        auto curl=hand.sourceBindGlobal;
        for(unsigned k=5;k<21;++k)curl[k]=bodyMultiply(rotation,curl[k]);
        require(retargetArm(hand,curl,hand.targetBindLocal,result)&&bodyForward(hand.parents,result,global),"full finger rotation applies");
        for(unsigned k=5;k<21;++k){
            const auto expected=bodyMultiply(rotation,hand.targetBindGlobal[hand.targets[k]]);
            for(unsigned c=0;c<3;++c)for(unsigned a=0;a<3;++a)require(std::fabs(global[hand.targets[k]].v[c*4+a]-expected.v[c*4+a])<.00001f,"finger rotation inherited through mapped native parents");
        }
        for(unsigned j=0;j<count;++j){
            for(unsigned a=0;a<3;++a)require(result[j].v[12+a]==hand.targetBindLocal[j].v[12+a],"all native finger lengths remain intact");
            if(std::find(hand.targets.begin(),hand.targets.end(),j)==hand.targets.end())require(!std::memcmp(&result[j],&hand.targetBindLocal[j],sizeof(BodyMatrix)),"unmapped hand helpers and other body joints remain native");
        }
        auto wrong=handTarget;wrong.parents[mapped[2]]=16;require(!prepareArmRetarget(handSource,wrong,hand,true),"wrong distal finger parent refused");
        auto wrongSource=handSource;wrongSource.parents[16]=4;require(!prepareArmRetarget(wrongSource,handTarget,hand,true),"missing native metacarpal refused");
        wrong=handTarget;wrong.parents.pop_back();wrong.jointIds.pop_back();wrong.bindLocal.pop_back();require(!prepareArmRetarget(handSource,wrong,hand,true),"unknown full-body hand rig refused");
    }
    std::cout<<"PASS: both native hand rigs, extra metacarpal mapping, 21-joint bind/curl, bone-length preservation and hierarchy refusal\n";
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
    {
        constexpr std::uintptr_t player=actor+0x10000,children=player+0x2000,armDecl=player+0x3000,bodyDecl=player+0x4000,labels=player+0x5000;
        constexpr std::uint32_t armHandle=0x10002;
        auto arm=m;arm.q(game+0x3130b0,player);arm.q(game+0x3130b8,actor);arm.d(actor+0xa8,armHandle);arm.d(actor+0xac,1);
        arm.q(player,base+0x1c5e428);arm.d(player+0xa8,1);arm.q(player+0xb0,children);arm.d(player+0xb8,1);arm.q(children,actor);
        arm.q(actor+0x20,labels);arm.q(actor+0x2e8,armDecl);arm.q(armDecl,base+0x1ab58f8);arm.q(armDecl+8,labels+0x100);
        arm.q(player+0x2e8,bodyDecl);arm.q(bodyDecl,base+0x1ab58f8);arm.q(bodyDecl+8,labels+0x200);
        auto string=[&](Memory& memory,std::uintptr_t at,const std::string& value){char data[160]{};std::memcpy(data,value.c_str(),value.size()+1);memory.put(at,data,sizeof(data));};
        auto inspectArm=[&](Memory& memory){return readNativeSkeleton([&](auto p,auto d,auto n){return memory.read(p,d,n);},base,armHandle,s,NativeSkeletonScope::PlayerArm);};
        for(const char* role:{"corvo","emily"})for(bool right:{false,true}){
            const std::string prefix=std::string("models/characters/player/")+role;
            string(arm,labels,right?"right_arm_2":"left_arm_1");string(arm,labels+0x100,prefix+(right?"/right_arm.def":"/left_arm.def"));
            string(arm,labels+0x200,prefix+"/body.def");string(arm,name,prefix+(right?"/right_arm.md6":"/left_arm.md6"));
            require(inspectArm(arm)&&s.playerArm.side==(right?2u:1u)&&s.playerArm.character==(!std::strcmp(role,"corvo")?1u:2u),"both separate arm rigs require their matching native player role");
        }
        require(!readNativeSkeleton([&](auto p,auto d,auto n){return arm.read(p,d,n);},base,armHandle,s),"ordinary actor scope still rejects an arkPlayer arm entity");
        require(!readNativeSkeleton([&](auto p,auto d,auto n){return m.read(p,d,n);},base,1,s,NativeSkeletonScope::PlayerArm),"arm scope rejects the main player");
        auto broken=arm;broken.d(actor+0xac,2);require(!inspectArm(broken),"arm parent must be the local player");
        broken=arm;broken.q(children,player);require(!inspectArm(broken),"arm must appear in native player's child list");
        broken=arm;broken.d(player+0xb8,2);broken.q(children+8,actor);require(!inspectArm(broken),"duplicate child identity rejected");
        broken=arm;broken.d(player+0xb8,33);require(!inspectArm(broken),"player child list bounded");
        broken=arm;string(broken,labels+0x100,"models/characters/player/corvo/right_arm.def");require(!inspectArm(broken),"arm from another role rejected");
        broken=arm;string(broken,labels,"unrelated_player");require(!inspectArm(broken),"unrecognized player-like child rejected");
        broken=arm;string(broken,name,"models/characters/player/emily/left_arm.md6");require(!inspectArm(broken),"render model must match the observed arm side");
        broken=arm;broken.d(actor+0xa8,armHandle+1);require(!inspectArm(broken),"reused entity slot rejected");
        broken=arm;broken.d(stack,15);require(!inspectArm(broken),"arm observation refused during loading");
        broken=arm;reads=0;
        require(!readNativeSkeleton([&](auto p,auto d,auto n){const bool ok=broken.read(p,d,n);if(p==children&&++reads==1)broken.q(children,player);return ok;},base,armHandle,s,NativeSkeletonScope::PlayerArm),"arm detached during snapshot rejected");
        std::cout<<"PASS: separate player arm skeleton scope validates both roles/sides, reciprocal parent, exact assets, generation and concurrent detachment\n";
    }
    retargetTests();
    armRetargetTests();
}


