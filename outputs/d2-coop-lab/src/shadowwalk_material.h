#pragma once
#include "native_material_params.h"
// Isolated544 visual feasibility candidate. No shared material edits, model
// changes or attachment propagation. Body particles/native FP remain533.
// 1214di (#11 emily_shadow_walk_enter_move_attack_exit): the product partner wears native material effect 1 while it
// presents Shadow Walk (variants 32-36). The native form has no third-person body; survey 1381b filmed effects 1-8 at
// 50% and 100% on the partner: effect 1 keeps the whole outline as a matte black shadow, 3 dissolves into embers, 4
// is the rejected bright patches, 5 scatters into fragments, 6 glows purple, 3-6 vanish at full fade, and 2, 7 and 8
// change nothing. Only the partner's own render parameters change, and they are restored on exit.
struct ShadowWalkMaterial {
    std::uintptr_t game=0,actor=0,render=0;unsigned handle=0;
    std::array<std::uintptr_t,2> declarations{};std::array<unsigned,2> ids{};
    std::array<std::array<unsigned,4>,2> written{};
    std::uint64_t token=0,peer=0,epoch=0;ULONGLONG deadline=0;
    bool active=false,blocked=false;unsigned variant=0;float fade=0;
    std::uint64_t starts=0,updates=0,restores=0,refused=0,retired=0;
} shadowWalkMaterial;
bool shadowWalkMaterialApi(){
    const unsigned char bytes[]={0x48,0x8b,0xc4,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x40};
    return nativePowerFxApi()&&!std::memcmp(gameImage+0x876030,bytes,sizeof(bytes));
}
bool shadowWalkMaterialIdentity(){
    auto& s=shadowWalkMaterial;std::uintptr_t render=0;
    return s.actor&&s.handle!=1&&s.game==observedGame&&s.epoch==outgoing.worldEpoch&&
        worldIdentityEntity(memoryRead,s.game,s.handle)==s.actor&&memoryRead(s.actor+0xf0,&render,8)&&render==s.render;
}
bool shadowWalkMaterialOwned(NativeMaterialParams& p){
    auto& s=shadowWalkMaterial;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!shadowWalkMaterialIdentity()||!readNativeMaterialParams(memoryRead,base,s.actor,s.render,p))return false;
    for(unsigned i=0;i<2;++i){const int at=p.find(s.ids[i]);if(at<0||p.words[at]!=s.written[i])return false;}return true;
}
void shadowWalkMaterialApply(bool remove,const std::array<std::array<unsigned,4>,2>& words){
    auto& s=shadowWalkMaterial;
    struct Value{std::uintptr_t declaration;std::array<unsigned,4> words;};static_assert(sizeof(Value)==24);
    struct List{const void* data;unsigned count,capacity;};static_assert(sizeof(List)==16);
    const Value values[]={{s.declarations[0],words[0]},{s.declarations[1],words[1]}};
    const List clears{s.declarations.data(),remove?2u:0u,0},sets{values,remove?0u:2u,0};
    // Native routine owns the render lock, parameter allocation and dirty flag.
    reinterpret_cast<void(*)(std::uintptr_t,const List*,const List*)>(gameImage+0x876030)(s.actor,&clears,&sets);
}
void stopShadowWalkMaterial(){
    auto& s=shadowWalkMaterial;if(!s.active)return;
    if(!shadowWalkMaterialIdentity()){s.active=false;++s.retired;return;}
    NativeMaterialParams p{};
    if(!shadowWalkMaterialOwned(p)){s.active=false;s.blocked=true;++s.refused;return;}
    shadowWalkMaterialApply(true,{});
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeMaterialParams after{};
    if(readNativeMaterialParams(memoryRead,base,s.actor,s.render,after)&&after.find(s.ids[0])<0&&after.find(s.ids[1])<0)++s.restores;
    else ++s.refused;
    s.active=false;s.variant=0;s.fade=0;
}
bool startShadowWalkMaterial(ULONGLONG tick){
    auto& s=shadowWalkMaterial;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeSkeleton rig{};
    if(!readNativeSkeleton(memoryRead,base,probeHandle,rig)||!rig.stable||rig.actor!=probeActor||rig.game!=observedGame||!bodyShadowTarget(rig)||rig.parents.size()!=219)return false;
    NativeMaterialParams before{};if(!readNativeMaterialParams(memoryRead,base,rig.actor,rig.render,before))return false;
    constexpr const char* names[]={"materialeffects/effect","materialeffects/fadevalue"};
    std::array<std::uintptr_t,2> declarations{};std::array<unsigned,2> ids{};
    for(unsigned i=0;i<2;++i){
        declarations[i]=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x2433f90,names[i],false,false);
        std::uintptr_t vt=0,info=0,name=0;unsigned flags=0;std::string text;
        if(!declarations[i]||!memoryRead(declarations[i],&vt,8)||vt!=base+0x1b19b48||
           !memoryRead(declarations[i]+0x10,&info,8)||info!=base+0x2433f90||!memoryRead(declarations[i]+8,&name,8)||
           !worldIdentityString(memoryRead,name,text)||text!=names[i]||!memoryRead(declarations[i]+0x48,&ids[i],4)||ids[i]>=65536||
           !memoryRead(declarations[i]+0x4c,&flags,4)||((flags>>4)&3)!=(i?3u:1u)||before.find(ids[i])>=0)return false;
    }
    if(ids[0]==ids[1])return false;
    s.actor=rig.actor;s.handle=rig.handle;s.render=rig.render;s.game=rig.game;s.declarations=declarations;s.ids=ids;
    s.token=shadowStream.token;s.peer=shadowStream.peerToken;s.epoch=outgoing.worldEpoch;s.deadline=tick+60000;
    s.written={};s.written[0][0]=1;shadowWalkMaterialApply(false,s.written);s.active=true;
    NativeMaterialParams after{};if(!shadowWalkMaterialOwned(after)){s.blocked=true;++s.refused;stopShadowWalkMaterial();return false;}
    ++s.starts;return true;
}
void serviceShadowWalkMaterial(ULONGLONG tick){
    if(!shadowWalkMaterialApi())return;auto& s=shadowWalkMaterial;unsigned variant=0;float amount=0;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    if(localRole==0&&p.end>tick&&p.lease>tick&&p.bank.hasShadowWalk()&&p.actor==probeActor&&p.handle==probeHandle&&
       p.clock.weight>.9f&&p.powerWeight>.9f&&p.powerVariant>=32&&p.powerVariant<=36&&!p.presentedDrawn){variant=p.powerVariant;amount=p.powerAmount;}
    // 1214dj: the form's own kill and choke are presented as takedowns (ids 37-44 and 77-84, 1214db), not as power
    // variants; in 1254zc Emily showed her ordinary body for the whole throw. The silhouette holds at full fade through them.
    else if(localRole==0&&p.end>tick&&p.lease>tick&&p.bank.hasShadowWalk()&&p.actor==probeActor&&p.handle==probeHandle&&
       p.clock.weight>.9f&&p.chokeWeight>.5f&&((p.chokeVariant>=37&&p.chokeVariant<=44)||(p.chokeVariant>=77&&p.chokeVariant<=84))){variant=33;amount=1;}
    ReleaseSRWLockShared(&movementLock);
    if(!variant||!coopGameplayReady(outgoing)||!shadowStreamCurrent(tick)||!shadowStream.peerReady){stopShadowWalkMaterial();if(!variant)s.blocked=false;return;}
    if(s.active&&(tick>=s.deadline||s.token!=shadowStream.token||s.peer!=shadowStream.peerToken||s.handle!=probeHandle)){
        stopShadowWalkMaterial();s.blocked=true;return;
    }
    if(!s.active){if(s.blocked)return;if(!startShadowWalkMaterial(tick)){++s.refused;s.blocked=true;return;}}
    NativeMaterialParams before{};if(!shadowWalkMaterialOwned(before)){s.active=false;s.blocked=true;++s.refused;return;}
    // 1214di: effect 1 at full fade is a complete black silhouette (1381b); it fades in over the entry (32) and out
    // over the exit (34), and holds through the crawl, the attack and its recovery (33, 35, 36).
    const float mix=(std::max)(0.f,(std::min)(1.f,amount));
    const float fade=variant==32?mix:variant==34?1.f-mix:1.f;
    auto words=s.written;std::memcpy(&words[1][0],&fade,4);s.variant=variant;
    if(words!=s.written){shadowWalkMaterialApply(false,words);s.written=words;s.fade=fade;++s.updates;
        NativeMaterialParams after{};if(!shadowWalkMaterialOwned(after)){s.active=false;s.blocked=true;++s.refused;}}
}
// 1214dh (#11 Shadow Walk appearance; lab only): "coop_partner_effect E P" applies native material effect E (0-15) at
// fade P% (0-100) to the presented partner body for 6 s through the same native parameter routine, then restores it.
// The native form has no third-person body; 544 tried effect 4 at 45% (bright patches, rejected). The survey films the
// other effects (renderparm materialeffects: burnt, devouring, disapear, electrocution, immolation, layerchange,
// shadowkill, teleportation) to choose one; it changes nothing outside the partner's own render parameters.
unsigned labPartnerEffect=0,labPartnerFadePct=0;ULONGLONG labPartnerEffectUntil=0;std::uint64_t labPartnerEffectApplied=0;
bool labPartnerEffectAllowed(const char* t){unsigned e=0,p=0;char tail=0;
    return std::sscanf(t,"%u %u%c",&e,&p,&tail)==2&&e<=15&&p<=100;}
void requestLabPartnerEffect(const char* t){unsigned e=0,p=0;if(std::sscanf(t,"%u %u",&e,&p)!=2||e>15||p>100)return;
    labPartnerEffect=e;labPartnerFadePct=p;labPartnerEffectUntil=GetTickCount64()+6000;}
void serviceLabPartnerEffect(ULONGLONG tick){
    if(!labPartnerEffectUntil||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;auto& s=shadowWalkMaterial;
    if(tick>=labPartnerEffectUntil||!shadowWalkMaterialApi()||!coopGameplayReady(outgoing)){labPartnerEffectUntil=0;stopShadowWalkMaterial();s.blocked=false;return;}
    if(!s.active){s.blocked=false;if(!startShadowWalkMaterial(tick)){labPartnerEffectUntil=0;return;}}
    auto words=s.written;words[0][0]=labPartnerEffect;const float fade=labPartnerFadePct/100.f;std::memcpy(&words[1][0],&fade,4);
    if(words!=s.written){shadowWalkMaterialApply(false,words);s.written=words;s.fade=fade;s.variant=labPartnerEffect;++s.updates;++labPartnerEffectApplied;}
}
void writeShadowWalkMaterial(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.shadowwalk-material.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=shadowWalkMaterial;
    fprintf(f,"{\"active\":%s,\"blocked\":%s,\"handle\":%u,\"variant\":%u,\"fade\":%.6f,\"starts\":%llu,\"updates\":%llu,\"restores\":%llu,\"refused\":%llu,\"retired\":%llu}\n",s.active?"true":"false",s.blocked?"true":"false",s.handle,s.variant,s.fade,s.starts,s.updates,s.restores,s.refused,s.retired);
    fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
