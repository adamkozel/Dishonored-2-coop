#pragma once
#include "native_player_stance.h"
#include <vector>
#include <cstring>

struct NativePlayerActiveState {std::uint32_t id=0;std::uintptr_t pointer=0,vtable=0;};
struct NativePlayerStates {
    NativePlayerStance player;
    std::uintptr_t component=0,mode=0;
    bool defaultMode=false;
    std::vector<NativePlayerActiveState> active;
};
// Native 0x803cb7 / 0x803ff7 iterate component+0xa8 IDs and resolve each
// through mode+0x10+ID*8. A mode contains 90 authored state slots. Read only.
template<class Read> bool readNativePlayerStates(Read read,std::uintptr_t base,NativePlayerStates& out,bool allowWheel=false) {
    NativePlayerStates s{};
    if(!readNativePlayerStance(read,base,s.player,allowWheel))return false;
    std::uintptr_t list=0,vt=0,owner=0;std::uint32_t componentCount=0;
    if(!read(s.player.player+0x268,&list,8)||!list||!read(s.player.player+0x270,&componentCount,4)||
       !(componentCount&0xffffff)||(componentCount&0xffffff)>256)return false;
    unsigned index=0;
    for(unsigned i=0;i<(componentCount&0xffffff);++i) {
        std::uintptr_t c=0;if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt!=base+0x1c73d58)continue;
        if(s.component||!read(c+0x10,&owner,8)||owner!=s.player.player)return false;
        s.component=c;index=i;
    }
    std::uintptr_t active=0;std::uint32_t count=0;
    if(!s.component||!read(s.component+0x90,&s.mode,8)||!s.mode||!read(s.mode,&owner,8)||owner!=s.component||
       !read(s.component+0xa8,&active,8)||!active||!read(s.component+0xb0,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>90)return false;
    std::uintptr_t defaultMode=0;
    if(!read(s.component+0x28,&defaultMode,8))return false;
    s.defaultMode=defaultMode==s.mode;
    std::vector<std::uint32_t> ids(count&0xffffff);
    if(!read(active,ids.data(),ids.size()*4))return false;
    bool seen[90]{};
    for(auto id:ids) {
        NativePlayerActiveState state{};state.id=id;std::uint32_t actual=0;
        if(id>=90||seen[id]||!read(s.mode+0x10+id*8,&state.pointer,8)||!state.pointer||
           !read(state.pointer,&state.vtable,8)||state.vtable<base+0x1a7d000||state.vtable>=base+68988928||
           !read(state.pointer+8,&actual,4)||actual!=id||!read(state.pointer+0x10,&owner,8)||owner!=s.component)return false;
        seen[id]=true;s.active.push_back(state);
    }
    std::uintptr_t again=0;std::uint32_t number=0;
    std::vector<std::uint32_t> finalIds(ids.size());
    if(!read(base+0x39a5290,&again,8)||again!=s.player.game||!read(s.player.game+0x3130b0,&again,8)||again!=s.player.player||
       !read(s.player.player+0x268,&again,8)||again!=list||!read(s.player.player+0x270,&number,4)||number!=componentCount||
       !read(list+index*8,&again,8)||again!=s.component||!read(s.component,&again,8)||again!=base+0x1c73d58||
       !read(s.component+0x10,&again,8)||again!=s.player.player||!read(s.component+0x90,&again,8)||again!=s.mode||
       !read(s.component+0x28,&again,8)||again!=defaultMode||
       !read(s.mode,&again,8)||again!=s.component||!read(s.component+0xa8,&again,8)||again!=active||
       !read(s.component+0xb0,&number,4)||number!=count||!read(active,finalIds.data(),finalIds.size()*4)||ids!=finalIds)return false;
    for(const auto& state:s.active) {
        if(!read(s.mode+0x10+state.id*8,&again,8)||again!=state.pointer||!read(state.pointer,&again,8)||again!=state.vtable||
           !read(state.pointer+8,&number,4)||number!=state.id||!read(state.pointer+0x10,&again,8)||again!=s.component)return false;
    }
    NativePlayerStance last{};
    if(!readNativePlayerStance(read,base,last)||last.game!=s.player.game||last.player!=s.player.player||last.physics!=s.player.physics)return false;
    out=s;return true;
}
// Exact grounded ranged/equipment overlays. The four-state Draw 65 +
// EquipChange 64/53 overlap was observed during Emily's native crouched
// crossbow draw (ranged-emily-empty-crossbow-cache-d58d-v1, sample 87).
// It changes equipment, not physical stance; weapon/scene ownership remains
// independently required by the ranged gesture reader.
inline bool nativePlayerGroundRangedStates(const NativePlayerStates& s,std::uintptr_t base) {
    if(!s.defaultMode||s.player.current>1||s.active.empty()||s.active.size()>4)return false;
    bool ground=false,draw=false,equip53=false,equip64=false;bool seen[90]{};
    for(const auto& a:s.active){
        if(a.id>=90||seen[a.id])return false;seen[a.id]=true;
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258)){if(ground)return false;ground=true;continue;}
        if(a.id==65&&a.vtable==base+0x1c78a68){draw=true;continue;}
        if((a.id==53||a.id==64)&&a.vtable==base+0x1c79cf8){equip53|=a.id==53;equip64|=a.id==64;continue;}
        if((a.id==56&&a.vtable==base+0x1c849d8)||(a.id==59&&a.vtable==base+0x1c7d608)||
           (a.id==81&&a.vtable==base+0x1c7df88))continue;
        return false;
    }
    return ground&&(draw?(s.active.size()==4&&equip53&&equip64):s.active.size()<=3);
}
// Corvo gun and Emily crossbow air-v1 observations: the same native
// FireWeapon56 can accompany exact Jump2/Fall3 while physics stays standing.
// Corvo gun air-v2 additionally observes Reload59 with both airborne states.
inline bool nativePlayerAirRangedStates(const NativePlayerStates& s,std::uintptr_t base) {
    if(!s.defaultMode||s.player.current!=0||s.active.empty()||s.active.size()>2)return false;
    bool air=false,overlay=false;
    for(const auto& a:s.active){
        if((a.id==2&&a.vtable==base+0x1c85028)||(a.id==3&&a.vtable==base+0x1c845c8)){if(air)return false;air=true;}
        else if((a.id==56&&a.vtable==base+0x1c849d8)||(a.id==59&&a.vtable==base+0x1c7d608)){if(overlay)return false;overlay=true;}
        else return false;
    }
    return air;
}
// Semantic motion enum for the observed default player mode only:
// 0 ground locomotion, 1 sneaking, 2 jumping, 3 falling. Native IDs and
// class tables were independently observed for both Corvo and Emily.
inline bool nativePlayerGroundCarryStates(const NativePlayerStates& s,std::uintptr_t base){
    if(!s.defaultMode||s.player.current>1||s.active.size()<2||s.active.size()>3)return false;
    unsigned grounded=0,carrying=0,overlay=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258)){++grounded;continue;}
        if((a.id==45&&a.vtable==base+0x1c77748)||(a.id==46&&a.vtable==base+0x1c790c8)){++carrying;continue;}
        if((a.id==53&&a.vtable==base+0x1c79cf8)||(a.id==81&&a.vtable==base+0x1c7df88)){++overlay;continue;}
        return false;
    }
    return grounded==1&&carrying==1&&overlay<=1;
}
// Exact native Stun14 and optional fast sword-holster overlay, observed on
// both roles in grenade-stun-scenes-*-v60. No health or raw-input inference.
// 1214de (#11 hit reactions): a stun taken in a guard fight with a power selected also carries the left hand's own
// EquipChange (state 64, the same vtable; 1214cu admitted it beside blocks and swings). Failing closed on it refused
// every such stun (1371f [14,64,53], 1328zzh/zzc/zzf), so the partner dropped out instead of staggering.
inline bool nativePlayerStunStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current!=0||s.active.empty()||s.active.size()>3)return false;
    unsigned stun=0,equipment=0,leftEquip=0;
    for(const auto& a:s.active){
        if(a.id==14&&a.vtable==base+0x1c86768)++stun;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++equipment;
        else if(a.id==64&&a.vtable==base+0x1c79cf8)++leftEquip;
        else return false;
    }
    return stun==1&&equipment<=1&&leftEquip<=1;
}
// 1214dn (#11 stagger_knockdown_recovery): the native knockdown (PLAYER_HITREACT_KNOCKDOWN), KnockedDown (16) alone in
// coverage-v1397f/g (the lab shove), with the stun's optional equipment overlays.
inline bool nativePlayerKnockdownStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.active.empty()||s.active.size()>3)return false;
    unsigned knocked=0,equipment=0,leftEquip=0;
    for(const auto& a:s.active){
        if(a.id==16&&a.vtable==base+0x1c827b8)++knocked;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++equipment;
        else if(a.id==64&&a.vtable==base+0x1c79cf8)++leftEquip;
        else return false;
    }
    return knocked==1&&equipment<=1&&leftEquip<=1;
}
inline bool nativePlayerGroundGrenadeStates(const NativePlayerStates& s,std::uintptr_t base){
    // v54/v56h standing and v60/v62 crouched native action observations.
    // This preserves only motion; the owned native scene grants the gesture.
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.size()!=2)return false;
    unsigned ground=0,throwing=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        if(a.id==62&&a.vtable==base+0x1c7f118)++throwing;
    }
    return ground==1&&throwing==1;
}
inline bool nativePlayerGroundIdleStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.active.size()!=1)return false;
    const auto& a=s.active[0];
    return (s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258);
}
// Native v63 observations: Jump2/Fall3 coexist with ThrowWeapon62 while
// cooking/releasing. The independent owned scene still grants the gesture.
inline bool nativePlayerAirGrenadeStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current!=0||s.active.size()!=2)return false;
    unsigned air=0,throwing=0;
    for(const auto& a:s.active){
        if((a.id==2&&a.vtable==base+0x1c85028)||(a.id==3&&a.vtable==base+0x1c845c8))++air;
        if(a.id==62&&a.vtable==base+0x1c7f118)++throwing;
    }
    return air==1&&throwing==1;
}
inline bool nativePlayerAirIdleStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current!=0||s.active.size()!=1)return false;
    const auto& a=s.active[0];
    return (a.id==2&&a.vtable==base+0x1c85028)||(a.id==3&&a.vtable==base+0x1c845c8);
}
//361/364 Corvo Possession from holstered: a native grounded stance,
//UsePower63 and sword EquipChange53 coexist. This establishes physical
//motion only; both independent scenes must still pass their owned readers.
inline bool nativePlayerGroundPowerDrawStates(const NativePlayerStates& s,std::uintptr_t base){
    //369 source-thread evidence captures the one-update Draw65 overlap too.
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||(s.active.size()!=3&&s.active.size()!=4))return false;
    unsigned ground=0,power=0,draw=0,request=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==63&&a.vtable==base+0x1c80ed8)++power;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++draw;
        else if(a.id==65&&a.vtable==base+0x1c78a68)++request;
        else return false;
    }return ground==1&&power==1&&draw==1&&request<=1;
}
//366: before UsePower63, holstered Possession briefly starts Draw65 plus
//EquipChange53. This exact physical context must survive that handoff.
inline bool nativePlayerGroundDrawRequestStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.size()!=3)return false;
    unsigned ground=0,request=0,equipment=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==65&&a.vtable==base+0x1c78a68)++request;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++equipment;
        else return false;
    }return ground==1&&request==1&&equipment==1;
}
inline bool nativePlayerGroundHeartStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.empty()||s.active.size()>4)return false;
    unsigned ground=0,heart=0,right=0,left=0,draw=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==67&&a.vtable==base+0x1c80b78)++heart;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++right;
        else if(a.id==64&&a.vtable==base+0x1c79cf8)++left;
        else if(a.id==81&&a.vtable==base+0x1c7df88)++draw;
        else return false;
    }
    return ground==1&&heart==1&&right<=1&&left<=1&&draw<=1;
}
// Exact native568 observations for both roles and both placeable gadgets.
inline bool nativePlayerGroundTrapStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.empty()||s.active.size()>5)return false;
    unsigned ground=0,place=0,ready=0,right=0,left=0,draw=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==60&&a.vtable==base+0x1c7c098)++place;
        else if(a.id==61&&a.vtable==base+0x1c3db88)++ready;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++right;
        else if(a.id==64&&a.vtable==base+0x1c79cf8)++left;
        else if(a.id==81&&a.vtable==base+0x1c7df88)++draw;
        else return false;
    }
    return ground==1&&place+ready==1&&right<=1&&left<=1&&draw<=1;
}
// Native rear-choke579/583: exclusive Choke41, standing entry then physical
// choke stance12. The legacy stance wire still excludes12; owned action
// scenes authorize its full-body presentation alongside the independent root.
// This only carries the independently observed root; owned scenes authorize gestures.
inline bool nativePlayerGroundChokeStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.defaultMode&&(s.player.current==0||s.player.current==12)&&s.active.size()==1&&
        s.active[0].id==41&&s.active[0].vtable==base+0x1c77c18;
}
// Native596 on both roles: exclusive Assassination13, standing0/default mode.
// Native1244b on both roles: the same fast-back lethal started from crouch keeps
// physical stance1 for the whole Assassination13 scene (a crouched rear choke
// instead stands the player into Choke41, already admitted above).
inline bool nativePlayerGroundAssassinationStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.defaultMode&&(s.player.current==0||s.player.current==1)&&s.active.size()==1&&
        s.active[0].id==13&&s.active[0].vtable==base+0x1c83468;
}
// Take 1254a, both roles: a slide takedown runs its exclusive Assassination13 (or
// Choke41) scene while the physical stance stays sliding (5).
inline bool nativePlayerSlideTakedownStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.defaultMode&&s.player.current==5&&s.active.size()==1&&
        ((s.active[0].id==13&&s.active[0].vtable==base+0x1c83468)||(s.active[0].id==41&&s.active[0].vtable==base+0x1c77c18));
}
// 1214db (#11 Shadow Walk attack; probe 1350b and take 1254y, confirmed runtime): inside Shadow Walk the player runs the
// form's own state mode (defaultMode false; id 0 is arkPlayerStateShadowWalk, RVA 1c7ed28) at physical stance 2, and the
// form's attack is the exclusive Assassination13 (the same vtable) in that mode, playing assassination_sw_* (ids 37-44).
inline bool nativePlayerShadowWalkAssassinationStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&!s.defaultMode&&s.player.current==2&&s.active.size()==1&&
        s.active[0].id==13&&s.active[0].vtable==base+0x1c83468;
}
// 1214db: the form's choke (choke_sw_*, ids 77-84) holds the exclusive Choke41 at physical stance 2 for its whole length
// (take 1254y, confirmed runtime), in the form's own mode (1254z: defaultMode false, confirmed runtime); either mode
// stays admitted for those ids only.
inline bool nativePlayerShadowWalkChokeStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.player.current==2&&s.active.size()==1&&s.active[0].id==41&&s.active[0].vtable==base+0x1c77c18;
}
// Take 1258b (state table 1259: RVA 1c405e8, arkPlayerStateVersus): a sword lock runs
// the exclusive Versus31 state on the ground.
inline bool nativePlayerVersusStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.defaultMode&&s.player.current<=1&&s.active.size()==1&&s.active[0].id==31&&s.active[0].vtable==base+0x1c405e8;
}
inline bool nativePlayerGroundPairedStates(const NativePlayerStates& s,std::uintptr_t base){
    return nativePlayerGroundChokeStates(s,base)||nativePlayerGroundAssassinationStates(s,base);
}
//649/650 native window-sill observations: physical stance2, AutoCrouch10,
// optionally the independent right-arm EquipChange53. Map presentation only;
// retain actual state IDs and capsule for all ownership checks and gameplay.
inline bool nativePlayerAutoCrouchStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current!=2||s.active.empty()||s.active.size()>2)return false;
    unsigned crouch=0,equip=0;
    for(const auto& a:s.active){
        if(a.id==10&&a.vtable==base+0x1c812b8)++crouch;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++equip;
        else return false;
    }
    return crouch==1&&equip<=1;
}
// Take 1269b (state table: RVA 1c7a998, arkPlayerStateHoldObject): picking up a native
// movable (a bottle) keeps Running0/Sneaking6 and adds HoldObject49, overlapping Undraw81
// while the sword is put away, until the object is thrown or put down.
inline bool nativePlayerGroundHoldObjectStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.size()<2||s.active.size()>3)return false;
    unsigned ground=0,hold=0,undraw=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==49&&a.vtable==base+0x1c7a998)++hold;
        else if(a.id==81&&a.vtable==base+0x1c7df88)++undraw;
        else return false;
    }
    return ground==1&&hold==1&&undraw<=1;
}
//664 standing and692 crouched coin pickup: LootObject47 may overlap Undraw81.
// It retains the matching Running0/Sneaking6 state. Preserve movement while native code
// handles the item; no pickup event, hand gesture or inventory is synthesized.
inline bool nativePlayerGroundLootStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.size()<2||s.active.size()>3)return false;
    unsigned ground=0,loot=0,undraw=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==47&&a.vtable==base+0x1c7bcc8)++loot;
        else if(a.id==81&&a.vtable==base+0x1c7df88)++undraw;
        else return false;
    }
    return ground==1&&loot==1&&undraw<=1;
}
inline bool nativePlayerMotion(const NativePlayerStates& s,std::uintptr_t base,std::uint32_t& motion) {
    if(nativePlayerGroundLootStates(s,base)||nativePlayerGroundHoldObjectStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerAutoCrouchStates(s,base)){motion=1;return true;}
    if(nativePlayerGroundPairedStates(s,base)||nativePlayerSlideTakedownStates(s,base)||nativePlayerVersusStates(s,base)||
       nativePlayerShadowWalkAssassinationStates(s,base)||nativePlayerShadowWalkChokeStates(s,base)){motion=0;return true;}
    if(nativePlayerGroundTrapStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerGroundHeartStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerGroundDrawRequestStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerGroundPowerDrawStates(s,base)){motion=s.player.current;return true;}
    // Native v187/v191/v245: Domino72 overlays Running0 or Sneaking6. Keep source
    // root motion valid; exact equipment and owned scene authorize gestures.
    if(s.player.logic==1&&s.defaultMode&&s.player.current<=1&&s.active.size()==2){
        unsigned ground=0,domino=0;for(const auto& a:s.active){
            ground+=(s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258);domino+=a.id==72&&a.vtable==base+0x1c3eeb8;
        }if(ground==1&&domino==1){motion=s.player.current;return true;}
    }
    if(nativePlayerStunStates(s,base)){motion=0;return true;} // Native Stun inherits Navigation; exact standing class only.
    if(nativePlayerKnockdownStates(s,base)){motion=0;return true;} // 1214dn: the knockdown is presented as stun variant 5.
    if(nativePlayerGroundGrenadeStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerAirGrenadeStates(s,base)){
        for(const auto& a:s.active)if(a.id==2||a.id==3){motion=a.id;return true;}
    }
    if(nativePlayerGroundCarryStates(s,base)){motion=s.player.current;return true;}
    // Observed base gun/crossbow fire, reload and equipment overlays retain
    // standing/sneaking native locomotion. Gesture ownership is checked separately.
    // Crouched observations retain Sneaking 6 with the same ranged overlays.
    if(s.active.size()>=2&&nativePlayerGroundRangedStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerAirRangedStates(s,base)){
        for(const auto& a:s.active)if(a.id==2||a.id==3){motion=a.id;return true;}
    }
    if(!s.defaultMode||s.active.empty()||s.active.size()>2)return false;
    struct Entry {unsigned id;std::uintptr_t table;unsigned motion;};
    bool found=false,overlay=false,equipmentOverlay=false;unsigned resolved=0;
    for(const auto& a:s.active){
        bool known=false;
        // Sprint is still grounded root motion; its distinct presentation
        // travels in the versioned semantic action channel. Observed in
        // native-sprint-observation-v1 on both roles, forward and diagonals.
        for(const auto& e:{Entry{0,0x1c85628,0},Entry{1,0x1c85948,0},Entry{6,0x1c84258,1},Entry{2,0x1c85028,2},Entry{3,0x1c845c8,3}}) {
            if(a.id==e.id&&a.vtable==base+e.table){
                if(found)return false;found=true;known=true;resolved=e.motion;break;
            }
        }
        if(!known){
            // A native Slash/Block can coexist with its movement state. Keep
            // that movement channel valid without guessing from controller input.
            // 1214dz: a parried swing's Recoil 40 (vtable RVA 0x1c83c98) is such an overlay too (1418k).
            if(overlay||!((a.id==39&&a.vtable==base+0x1c85d28)||(a.id==44&&a.vtable==base+0x1c81a68)||(a.id==40&&a.vtable==base+0x1c83c98)||
                (a.id==18&&a.vtable==base+0x1c82b28)||(a.id==63&&a.vtable==base+0x1c80ed8)||
                ((resolved==0||resolved==1)&&a.id==75&&a.vtable==base+0x1c86ab8)||
                ((resolved==0||resolved==1)&&((a.id==53&&a.vtable==base+0x1c79cf8)||(a.id==54&&a.vtable==base+0x1c78a68)||(a.id==81&&a.vtable==base+0x1c7df88)))))return false;
            // Native Windblast observation air-v2 confirms UsePower with
            // both Jump and Fall. Preserve the independent movement state;
            // an owned power scene is still required to publish its gesture.
            overlay=true;equipmentOverlay=a.id==53||a.id==54||a.id==81||a.id==75;
        }
    }
    if(!found||(equipmentOverlay&&resolved>1))return false;motion=resolved;return true;
}

