#pragma once
#include "world_identity.h"
#include "native_player_arm.h"
#include "native_cosmetic_ranged.h"
#include "native_player_ranged_item.h"
#include <array>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_set>

// Read-only reconstruction of native 53d6a0 (joint cache), 256040 (joint
// identity), and 24f1d0/24f240 (bind transform/parent). No game calls.
struct NativeSkeletonPhase {
    std::vector<std::int16_t> mapping;
    std::vector<std::array<float,16>> matrices;
};
struct NativeSkeleton {
    unsigned failureLine=0;
    std::uintptr_t game=0,actor=0,animation=0,render=0,model=0,skeleton=0;
    std::uint32_t handle=0,logic=0;
    char modelName[160]{},skeletonName[160]{};
    std::vector<std::uint16_t> jointIds;
    std::vector<std::int16_t> parents;
    std::vector<std::array<float,12>> bindLocal;
    std::vector<std::array<float,16>> localPose,globalPose;
    std::vector<NativeSkeletonPhase> phases;
    float root[12]{};
    bool stable=false;
    NativePlayerArm playerArm{};
};
enum class NativeSkeletonScope { Actor,PlayerArm,CosmeticRanged,PlayerRanged };
template<class Read> bool readNativeSkeleton(Read read,std::uintptr_t base,std::uint32_t handle,NativeSkeleton& out,NativeSkeletonScope scope=NativeSkeletonScope::Actor) {
    NativeSkeleton s{};s.handle=handle;
    auto fail=[&](unsigned line){s.failureLine=line;out=std::move(s);return false;};
    std::uintptr_t vt=0,list=0,owner=0,name=0,cache=0,headers=0,joints=0,blob=0,logicStack=0,local=0,global=0;
    std::uint32_t flags=0,phase=0,cacheCount=0,phaseCount=0,jointCount=0,blobSize=0,logicCount=0,logic=0,pending=0;
    if(!read(base+0x39a5290,&s.game,8)||!s.game||!read(s.game,&vt,8)||vt!=base+0x1c940a8||
       !read(s.game+0x341058,&phase,4)||phase!=3||!read(s.game+0x1f6ed0,&owner,8)||owner!=s.game||
       !read(s.game+0x1f73f0,&logicStack,8)||!logicStack||!read(s.game+0x1f73f8,&logicCount,4)||
       !(logicCount&0xffffff)||(logicCount&0xffffff)>32||!read(logicStack+((logicCount&0xffffff)-1)*4,&logic,4)||
       (logic!=1&&logic!=2)||!read(s.game+0x1f7400,&pending,4)||pending)return fail(__LINE__);
    s.actor=worldIdentityEntity(read,s.game,handle);
    const bool arm=scope==NativeSkeletonScope::PlayerArm;
    const bool cosmetic=scope==NativeSkeletonScope::CosmeticRanged;NativeCosmeticRanged prop{};
    const bool weaponScope=scope==NativeSkeletonScope::PlayerRanged;NativePlayerRangedItem weapon{};
    if(arm&&(!readNativePlayerArm(read,base,s.game,handle,s.playerArm)||s.playerArm.actor!=s.actor))return fail(__LINE__);
    if(cosmetic&&!readNativeCosmeticRanged(read,base,s.game,handle,prop))return fail(__LINE__);
    if(weaponScope&&((!readNativePlayerRangedItem(read,base,1,weapon,false)||weapon.handle!=handle)&&(!readNativePlayerRangedItem(read,base,2,weapon,false)||weapon.handle!=handle)))return fail(__LINE__);
    if(weaponScope&&weapon.entity!=s.actor)return fail(__LINE__);
    if(!s.actor||!read(s.actor,&vt,8)||vt!=base+(cosmetic?0x1c87d58:weaponScope?0x1ce1e98:(handle==1||arm)?0x1c5e428:0x1c5e108)||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&flags,4)||(flags&0xffffff)>256)return fail(__LINE__);
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t c=0;if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8))return fail(__LINE__);
        if(vt==base+0x1b80408){if(s.animation)return fail(__LINE__);s.animation=c;}
    }
    if(!s.animation||!read(s.animation+0x10,&owner,8)||owner!=s.actor||
       !read(s.animation+0x38,&s.render,8)||!s.render||!read(s.render,&vt,8)||vt!=base+0x1b0d628||
       !read(s.render+0x1fe8,&s.model,8)||!s.model||!read(s.model,&vt,8)||vt!=base+0x1b05218||
       !read(s.model+8,&name,8)||!name||!read(name,s.modelName,sizeof(s.modelName)-1)||
       !read(s.model+0x50,&s.skeleton,8)||!s.skeleton||!read(s.skeleton,&vt,8)||vt!=base+0x1b0bd08||
       !read(s.skeleton+8,&name,8)||!name||!read(name,s.skeletonName,sizeof(s.skeletonName)-1)||
       !read(s.skeleton+0x70,&joints,8)||!joints||!read(s.skeleton+0x78,&jointCount,4)||
       !(jointCount&0xffffff)||(jointCount&0xffffff)>1024||!read(s.skeleton+0x158,&blob,8)||!blob||
       !read(s.skeleton+0x160,&blobSize,4)||blobSize<0x40||blobSize>0x100000||
       !read(s.animation+0x2d8,&cache,8)||!cache||!read(s.animation+0x2e0,&cacheCount,4)||
       !(cacheCount&0xffffff)||(cacheCount&0xffffff)>4096||!read(s.animation+0x2e8,&headers,8)||!headers||
       !read(s.animation+0x2f0,&phaseCount,4)||!(phaseCount&0xffffff)||(phaseCount&0xffffff)>8||
       !read(s.actor+0x300,s.root,sizeof(s.root)))return fail(__LINE__);
    if(cosmetic&&(s.animation!=prop.animation||!cosmeticRangedModelMatches(prop,s.modelName,s.skeletonName)||(jointCount&0xffffff)>128))return fail(__LINE__);
    if(weaponScope){NativeCosmeticRanged expected{};expected.character=weapon.player.character;expected.kind=weapon.kind;
        if(!cosmeticRangedModelMatches(expected,s.modelName,s.skeletonName)||(jointCount&0xffffff)>128)return fail(__LINE__);}
    jointCount&=0xffffff;cacheCount&=0xffffff;phaseCount&=0xffffff;
    unsigned localCount=0,globalCount=0;
    if(!read(s.animation+0x80,&owner,8)||owner!=s.skeleton||
       !read(s.animation+0x188,&local,8)||!local||(local&15)||!read(s.animation+0x190,&localCount,4)||(localCount&0xffffff)!=jointCount||
       !read(s.animation+0x198,&global,8)||!global||(global&15)||!read(s.animation+0x1a0,&globalCount,4)||(globalCount&0xffffff)!=jointCount)return fail(__LINE__);
    s.localPose.resize(jointCount);s.globalPose.resize(jointCount);
    if(!read(local,s.localPose.data(),jointCount*64)||!read(global,s.globalPose.data(),jointCount*64))return fail(__LINE__);
    for(const auto* values:{&s.localPose,&s.globalPose})for(const auto& matrix:*values)for(float f:matrix)if(!std::isfinite(f))return fail(__LINE__);
    std::vector<unsigned char> bytes(blobSize);std::vector<float> matrices(cacheCount*16);
    s.jointIds.resize(jointCount);s.parents.resize(jointCount);s.bindLocal.resize(jointCount);
    if(!read(blob,bytes.data(),bytes.size())||!read(joints,s.jointIds.data(),jointCount*2)||!read(cache,matrices.data(),matrices.size()*4))return fail(__LINE__);
    auto word=[&](unsigned offset){unsigned value=0;std::memcpy(&value,bytes.data()+offset,4);return value;};
    if(word(0)!=0x45533036||(word(0x10)&0xffff)!=jointCount)return fail(__LINE__);
    const auto bindOffset=static_cast<std::uint64_t>(word(0x18))+0x18,parentMap=static_cast<std::uint64_t>(word(0x1c))+0x1c;
    if(bindOffset+jointCount*48>blobSize||parentMap+jointCount*2>blobSize)return fail(__LINE__);
    std::unordered_set<unsigned> ids;
    for(unsigned i=0;i<jointCount;++i) {
        if(s.jointIds[i]==0xffff||!ids.insert(s.jointIds[i]).second)return fail(__LINE__);
        unsigned short mapped=0,parent=0;std::memcpy(&mapped,bytes.data()+parentMap+i*2,2);
        if(0x42ull+mapped*4+2>blobSize)return fail(__LINE__);
        std::memcpy(&parent,bytes.data()+0x42+mapped*4,2);parent&=0x7fff;
        s.parents[i]=parent>=0x4000?-1:static_cast<std::int16_t>(parent);
        if(s.parents[i]>=static_cast<int>(jointCount)||s.parents[i]==static_cast<int>(i))return fail(__LINE__);
        std::memcpy(s.bindLocal[i].data(),bytes.data()+bindOffset+i*48,48);
        for(float f:s.bindLocal[i])if(!std::isfinite(f))return fail(__LINE__);
    }
    for(float f:s.root)if(!std::isfinite(f))return fail(__LINE__);
    for(unsigned j=0;j<jointCount;++j) {
        int parent=s.parents[j];unsigned depth=0;
        while(parent>=0){if(parent==static_cast<int>(j)||++depth>=jointCount)return fail(__LINE__);parent=s.parents[parent];}
    }
    std::vector<std::uintptr_t> maps(phaseCount);std::vector<unsigned> counts(phaseCount);
    for(unsigned p=0;p<phaseCount;++p) {
        NativeSkeletonPhase entry{};
        if(!read(headers+p*16,&maps[p],8)||!read(headers+p*16+8,&counts[p],4))return fail(__LINE__);
        counts[p]&=0xffffff;if(counts[p]&&(!maps[p]||counts[p]<jointCount||counts[p]>4096))return fail(__LINE__);
        entry.mapping.resize(counts[p]);entry.matrices.resize(jointCount);
        if(counts[p]&&!read(maps[p],entry.mapping.data(),counts[p]*2))return fail(__LINE__);
        for(unsigned j=0;j<jointCount&&counts[p];++j) {
            const int at=entry.mapping[j];if(at<-1||at>=static_cast<int>(cacheCount))return fail(__LINE__);
            if(at<0)continue;
            std::memcpy(entry.matrices[j].data(),matrices.data()+at*16,64);
            for(float f:entry.matrices[j])if(!std::isfinite(f))return fail(__LINE__);
        }
        s.phases.push_back(std::move(entry));
    }
    // Changed cache bytes are retained as an explicitly unstable observation.
    // Changed owners, mappings, world or native phase invalidate it entirely.
    std::uintptr_t check=0;unsigned number=0;float rootAgain[12]{};
    std::vector<float> again(matrices.size());std::vector<unsigned short> idsAgain(jointCount);
    std::vector<std::array<float,16>> localAgain(jointCount),globalAgain(jointCount);
    if(!read(base+0x39a5290,&check,8)||check!=s.game||!read(s.game+0x341058,&number,4)||number!=phase||
       worldIdentityEntity(read,s.game,handle)!=s.actor||!read(s.actor+0x268,&check,8)||check!=list||
       !read(s.actor+0x270,&number,4)||number!=flags||!read(s.animation,&check,8)||check!=base+0x1b80408||
       !read(s.animation+0x10,&check,8)||check!=s.actor||!read(s.animation+0x38,&check,8)||check!=s.render||
       !read(s.render+0x1fe8,&check,8)||check!=s.model||!read(s.model+0x50,&check,8)||check!=s.skeleton||
       !read(s.animation+0x2d8,&check,8)||check!=cache||!read(s.animation+0x2e0,&number,4)||(number&0xffffff)!=cacheCount||
       !read(s.animation+0x2e8,&check,8)||check!=headers||!read(s.animation+0x2f0,&number,4)||(number&0xffffff)!=phaseCount||
       !read(s.skeleton+0x70,&check,8)||check!=joints||!read(s.skeleton+0x78,&number,4)||(number&0xffffff)!=jointCount||
       !read(s.skeleton+0x158,&check,8)||check!=blob||!read(s.skeleton+0x160,&number,4)||number!=blobSize||
       !read(s.animation+0x80,&check,8)||check!=s.skeleton||!read(s.animation+0x188,&check,8)||check!=local||
       !read(s.animation+0x190,&number,4)||number!=localCount||!read(s.animation+0x198,&check,8)||check!=global||
       !read(s.animation+0x1a0,&number,4)||number!=globalCount||!read(local,localAgain.data(),jointCount*64)||
       !read(global,globalAgain.data(),jointCount*64)||
       !read(joints,idsAgain.data(),jointCount*2)||idsAgain!=s.jointIds||!read(cache,again.data(),again.size()*4)||
       !read(s.actor+0x300,rootAgain,sizeof(rootAgain))||!read(s.game+0x1f73f0,&check,8)||check!=logicStack||
       !read(s.game+0x1f73f8,&number,4)||number!=logicCount||!read(logicStack+((logicCount&0xffffff)-1)*4,&number,4)||number!=logic||
       !read(s.game+0x1f7400,&number,4)||number)return fail(__LINE__);
    for(unsigned p=0;p<phaseCount;++p) {
        std::vector<std::int16_t> remap(counts[p]);
        if(!read(headers+p*16,&check,8)||check!=maps[p]||!read(headers+p*16+8,&number,4)||(number&0xffffff)!=counts[p]||
           (counts[p]&&!read(maps[p],remap.data(),counts[p]*2))||remap!=s.phases[p].mapping)return fail(__LINE__);
    }
    s.stable=std::memcmp(rootAgain,s.root,sizeof(rootAgain))==0&&std::memcmp(again.data(),matrices.data(),matrices.size()*4)==0&&
        std::memcmp(localAgain.data(),s.localPose.data(),jointCount*64)==0&&std::memcmp(globalAgain.data(),s.globalPose.data(),jointCount*64)==0;
    if(arm){
        NativePlayerArm armAgain{};
        if(!readNativePlayerArm(read,base,s.game,handle,armAgain)||!sameNativePlayerArm(s.playerArm,armAgain))return fail(__LINE__);
        const auto expected=std::string("models/characters/player/")+(armAgain.character==1?"corvo":"emily")+(armAgain.side==1?"/left_arm.md6":"/right_arm.md6");
        if(s.modelName!=expected)return fail(__LINE__);
    }
    if(weaponScope){NativePlayerRangedItem again{};
        if(!readNativePlayerRangedItem(read,base,weapon.kind,again,false)||again.entity!=weapon.entity||again.handle!=weapon.handle||again.component!=weapon.component||again.flags!=weapon.flags)return fail(__LINE__);}
    s.logic=logic;out=std::move(s);return true;
}
