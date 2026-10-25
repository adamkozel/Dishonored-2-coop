#pragma once
// Whitelisted finite native effects. No power activation or damage calls.
// Original Windblast remains manual-only: it distorts the observer screen.
// Kind8 reuses native particles/sound via a separate validated world-space FX.
struct NativePowerFx {
    std::uintptr_t game=0,actor=0,declaration=0,record=0;unsigned owner=0,id=~0u,kind=0;
    std::uint64_t epoch=0,streamToken=0,peerToken=0;ULONGLONG expires=0;std::array<float,3> handPoint{};
};
std::array<NativePowerFx,8> nativePowerEffects{};
unsigned powerFxRequest=0;
std::uint64_t powerFxStarted=0,powerFxStopped=0,powerFxFinished=0,powerFxRefused=0,powerFxRetired=0;
std::uint64_t powerFxHandUpdates=0;
const char* powerFxResult="idle";
const char* powerFxName(unsigned kind){
    return kind==1?"models/effects/gameplay/powers/doppelganger/decoy/blink/dganger_blink_start_01.fx":
        kind==2?"models/effects/gameplay/powers/doppelganger/decoy/blink/dganger_blink_out_01.fx":
        kind==4?"models/effects/gameplay/powers/shadow_grapple/grapple/sgrapple_body_01.fx":
        kind==5?"models/effects/gameplay/powers/shadow_grapple/grapple/sgrapple_impact_01.fx":
        kind==6?"models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx":
        kind==7?"models/effects/gameplay/powers/windblast/blast_impact/ply/wblast_ply_dust_grnd_wave_01.fx":
        kind==8?"models/effects/d2coop/windblast_remote.fx":
        kind==10?"models/effects/gameplay/powers/bend_time/bend_time_01.fx":
        kind==11?"models/effects/gameplay/player_generic/tatoo/out_of_mana/out_of_mana_01.fx":
        kind==13?"models/effects/gameplay/powers/mesmerize_v2/body/mesmerize_v2_01.fx":
        kind==14?"models/effects/gameplay/powers/mesmerize_v2/eye/mesmerize_v2_eye_01.fx":
        kind==15?"models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_01.fx":
        kind==16?"models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_trail_01.fx":
        kind==17?"models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_head_01.fx":
        kind==18?"models/effects/gameplay/powers/phantasmal_link/target/phlink_validated_01.fx":
        kind==20?"models/effects/d2coop/swarm_spawn_remote.fx":
        kind==22?"models/effects/d2coop/possession_target_remote.fx":
        kind==23?"models/effects/gameplay/powers/shadow_walk/ply_arms/idle/ply_arm_idle_01.fx":
        kind==24?"models/effects/gameplay/powers/shadow_walk/ply_arms/idle/ply_hand_idle_01.fx":
        kind==25?"models/effects/gameplay/powers/shadow_walk/ply_arms/loco/ply_arm_loco_01.fx":
        kind==26?"models/effects/gameplay/powers/shadow_walk/ply_arms/loco/ply_arm_loco_02.fx":
        kind==27?"models/effects/gameplay/powers/shadow_walk/ply_arms/loco/ply_hand_loco_01.fx":
        kind==28?"models/effects/gameplay/powers/shadow_walk/ply_arms/transformation/ply_arm_ashes_01.fx":
        kind==21?"models/effects/gameplay/powers/rat_swarm/rat/rat_spawn_trail_01.fx":
        kind==19?"models/effects/gameplay/powers/phantasmal_link/links/phlink_links_01.fx":
        kind==12?"models/effects/d2coop/grenade_inhand_remote.fx":
        // 1214ej: remote copies of the Shockwave's ground wave and the Greater Windblast blast (native particles and
        // sound; the native screen particle and full-screen quad stay with the caster).
        kind==29?"models/effects/d2coop/windblast_wave_remote.fx":
        kind==30?"models/effects/d2coop/windblast_stronger_remote.fx":
        // 1214fk (#117): native Shadow Kill ash for the partner's copy of a body that turned to ash.
        kind==31?"models/effects/gameplay/shadow_kill/sk_spine_01.fx":
        kind==32?"models/effects/gameplay/shadow_kill/sk_spine_head.fx":
        kind==33?"models/effects/gameplay/shadow_kill/sk_left_femur.fx":
        kind==34?"models/effects/gameplay/shadow_kill/sk_right_femur.fx":
        // Isolated native-derived world effects retain character sound/light
        // and all layers except the long 1p spark streaks (30% length).
        // The source player's native first-person declarations are unchanged.
        kind==9?(remoteIsCorvo()?"models/effects/d2coop/muzzle_remote_corvo.fx":"models/effects/d2coop/muzzle_remote_emily.fx"):nullptr;
}
bool nativePowerFxApi(){
    const unsigned char start[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xec,0x30,0x48,0x83,0x7c,0x24,0x68,0x00,0x8b,0x99,0xa8};
    const unsigned char lookup[]={0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff,0xff,0x48,0x89,0x5c,0x24,0x40,0x48};
    const unsigned char stop[]={0x48,0x89,0x5c,0x24,0x08,0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xec,0x20,0x48,0x8b,0xd9,0xe8,0x9a,0xd2,0xff,0xff,0x48,0x8b};
    const unsigned char getter[]={0x48,0x8d,0x81,0x30,0x84,0x1f,0x00,0xc3};
    return nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&powerFixtureApiMatches()&&
        !std::memcmp(gameImage+0x87cb90,start,sizeof(start))&&!std::memcmp(gameImage+0x1a04a0,lookup,sizeof(lookup))&&
        !std::memcmp(gameImage+0x1a31f0,stop,sizeof(stop))&&!std::memcmp(gameImage+0x8a8a70,getter,sizeof(getter));
}
std::uintptr_t nativePowerFxManager(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t game=0,vt=0,getter=0;
    if(!nativePowerFxApi()||!memoryRead(base+0x39a5290,&game,8)||game!=observedGame||!game||
       !memoryRead(game,&vt,8)||vt!=base+0x1c940a8||!memoryRead(vt+0x388,&getter,8)||getter!=base+0x8a8a70)return 0;
    return game+0x1f8430; // Exact native getter is LEA RAX,[RCX+1f8430]; RET.
}
std::uintptr_t nativePowerFxDefinition(unsigned kind){
    const auto name=powerFxName(kind);if(!name||!nativePowerFxManager())return 0;
    const unsigned char typeBytes[]={0x48,0x89,0x6c,0x24,0x18,0x56,0x48,0x83,0xec,0x20,0x48,0x8b,0xea,0x48,0x8b,0xf1};
    const unsigned char findBytes[]={0x48,0x85,0xd2,0x75,0x03,0x33,0xc0,0xc3,0x45,0x33,0xc9,0xe9,0x60,0xa3,0x01,0x00};
    if(std::memcmp(gameImage+0x1ea30,typeBytes,sizeof(typeBytes))||std::memcmp(gameImage+0x3cc0,findBytes,sizeof(findBytes)))return 0;
    auto type=reinterpret_cast<std::uintptr_t(*)(void*,const char*)>(gameImage+0x1ea30)(gameImage+0x24367e0,"fx");
    std::uintptr_t text=0,vt=0;std::string value;
    if(!type||!memoryRead(type+0x78,&text,8)||!worldIdentityString(memoryRead,text,value)||value!="fx")return 0;
    auto def=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,const char*,bool)>(gameImage+0x3cc0)(type,name,false);
    if(!def||!memoryRead(def,&vt,8)||vt!=reinterpret_cast<std::uintptr_t>(gameImage)+0x1ab3138||
       !memoryRead(def+8,&text,8)||!worldIdentityString(memoryRead,text,value)||value!=name)return 0;
    return def;
}
std::uintptr_t nativePowerFxRecord(const NativePowerFx& e){
    const auto manager=nativePowerFxManager();if(!manager||e.game!=observedGame||e.epoch!=outgoing.worldEpoch||e.id==~0u)return 0;
    auto record=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t)>(gameImage+0x1a04a0)(manager,e.id,0,0);
    unsigned owner=0,id=0;std::uintptr_t declaration=0;
    if(!record||!memoryRead(record,&owner,4)||owner!=e.owner||!memoryRead(record+4,&id,4)||id!=e.id||
       !memoryRead(record+8,&declaration,8)||declaration!=e.declaration||(e.record&&e.record!=record))return 0;
    return record;
}
bool nativeFailureFxHand(float* point,float* axis){
    NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s)||!s.stable||s.logic!=1||
       s.game!=observedGame||s.actor!=probeActor||!bodyShadowTarget(s)||s.parents.size()!=(remoteIsCorvo()?215u:219u)||
       s.parents[9]!=8||s.parents[10]!=9||s.globalPose.size()!=s.parents.size())return false;
    const auto& hand=s.globalPose[9];
    for(unsigned r=0;r<3;++r){point[r]=s.root[r];for(unsigned k=0;k<3;++k)point[r]+=s.root[3+k*3+r]*hand[12+k];
        for(unsigned c=0;c<3;++c){axis[c*3+r]=0;for(unsigned k=0;k<3;++k)axis[c*3+r]+=s.root[3+k*3+r]*hand[c*4+k];}}
    for(unsigned i=0;i<3;++i)if(!std::isfinite(point[i])||std::fabs(point[i])>100000)return false;
    for(unsigned i=0;i<9;++i)if(!std::isfinite(axis[i])||std::fabs(axis[i])>1.001f)return false;
    return true;
}
bool npcCombatRangedFixtureReady(ULONGLONG tick,unsigned minimumMs);
bool nativePowerFxIsolation(unsigned kind,bool network,ULONGLONG tick){
    // 1071: partner (network) presentation is gameplay on any paired map;
    // local fixture casts still require the Dunwall observer isolation.
    if(network&&coopWorldPairReady(outgoing,remote))return true;
    return (observerIsolation.ready&&observerIsolation.movementFixture)||
        (kind==9&&network&&npcCombatRangedFixtureReady(tick,0));
}
bool startNativePowerFx(unsigned kind,const float* origin,ULONGLONG tick,bool network=false,const float* axis=nullptr,unsigned* createdId=nullptr){
    if(createdId)*createdId=~0u;
    const auto reject=[&](const char* reason){++powerFxRefused;powerFxResult=reason;return false;};
    if(network&&(kind==6||kind==7||kind==10))return reject("unscoped_viewer_effect_network_refused");
    // 1214kn: each scope condition names its refusal (all were unsupported_scope; two-account takes refused the host's
    // Blink effects in the partner's game, campaign-palace-vm-v1917b).
    if(!nativePowerFxApi())return reject("scope_native_api");
    if(!network&&std::strcmp(mapName,kGuardMap))return reject("scope_local_cast_off_fixture_map");
    if(!nativePowerFxIsolation(kind,network,tick))return reject("scope_isolation");
    if(!coopGameplayReady(outgoing))return reject("scope_local_not_gameplay");
    if(!coopGameplayReady(remote))return reject("scope_peer_not_gameplay");
    if(!probeDoppelganger)return reject("scope_avatar_not_doppelganger");
    if(!avatarAiHeld)return reject("scope_avatar_ai_not_held");
    if(!probeFollowExact)return reject("scope_avatar_follow_not_exact");
    if(avatarAiTarget.actor!=probeActor)return reject("scope_avatar_ai_target_stale");
    NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSwordEquipment(memoryRead,base,probeHandle,sword,remoteIsCorvo()?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
       sword.actor!=probeActor||sword.game!=observedGame)return reject("avatar_identity_changed");
    NativePowerFx* slot=nullptr;for(auto& e:nativePowerEffects)if(!e.expires){slot=&e;break;}if(!slot)return reject("effect_slots_full");
    float point[3]{};if(origin)std::memcpy(point,origin,12);else if(!memoryRead(probeActor+0x300,point,12))return reject("origin_unavailable");
    float handAxis[9]{};if(kind==11){if(!nativeFailureFxHand(point,handAxis))return reject("native_failure_hand_unavailable");axis=handAxis;}
    if(kind==8&&!origin)point[2]+=1.2f; // Manual visual probe only. Network must use the observed native origin.
    for(float v:point)if(!std::isfinite(v)||std::fabs(v)>100000)return reject("invalid_origin");
    if(axis)for(unsigned i=0;i<9;++i)if(!std::isfinite(axis[i])||std::fabs(axis[i])>1.001f)return reject("invalid_axis");
    NativePowerFx e{};e.game=observedGame;e.actor=probeActor;e.owner=probeHandle;e.epoch=outgoing.worldEpoch;e.kind=kind;
    if(network){if(!shadowStreamCurrent(tick)||!shadowStream.peerReady)return reject("power_stream_changed");e.streamToken=shadowStream.token;e.peerToken=shadowStream.peerToken;}
    e.declaration=nativePowerFxDefinition(kind);if(!e.declaration)return reject("definition_unavailable");
    // ABI verified from machine instructions. Args 3..5 are unused. Explicit
    // origin anchors endpoint effects; kind11 follows its validated hand below.
    using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
    reinterpret_cast<Start>(gameImage+0x87cb90)(probeActor,&e.id,0,0,0,e.declaration,point,axis,0,0,0,0);
    if(e.id==~0u)return reject("native_start_failed");
    for(unsigned k=0;k<3;++k)e.handPoint[k]=point[k];
    e.expires=tick+(kind==21?3000:kind==9?2500:1600);e.record=nativePowerFxRecord(e);*slot=e;++powerFxStarted;
    if(e.record&&createdId)*createdId=e.id;
    powerFxResult=e.record?"native_effect_owned":"native_effect_ownership_unconfirmed";return e.record!=0;
}
void serviceNativePowerFx(ULONGLONG tick){
    if(!nativePowerFxApi())return;const unsigned request=powerFxRequest;powerFxRequest=0;
    for(auto& e:nativePowerEffects)if(e.expires){
        if(e.game!=observedGame||e.epoch!=outgoing.worldEpoch){e={};++powerFxRetired;continue;}
        if(!coopGameplayReady(outgoing))continue; // Resume cleanup after native pause.
        if(!nativePowerFxRecord(e)){e={};++powerFxFinished;continue;}
        if(request==3||tick>=e.expires||!nativePowerFxIsolation(e.kind,e.streamToken!=0,tick)||
           (e.streamToken&&(!shadowStreamCurrent(tick)||shadowStream.token!=e.streamToken||shadowStream.peerToken!=e.peerToken))){
            reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e.id);
            ++powerFxStopped;e={};powerFxResult="owned_effect_stopped";
        }else if(e.kind==11){
            float point[3]{},axis[9]{};const auto record=nativePowerFxRecord(e);std::uintptr_t callback=1;
            if(e.actor!=probeActor||e.owner!=probeHandle||!nativeFailureFxHand(point,axis)||!record||
               !memoryRead(record+0x128,&callback,8)||callback){
                if(record)reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e.id);
                ++powerFxStopped;e={};powerFxResult="failure_hand_owner_retired";
            }else{
                // Exact native start writes world origin+38 and axis+44.
                // Only our current owned record is updated; no FP callbacks.
                std::memcpy(reinterpret_cast<void*>(record+0x38),point,12);std::memcpy(reinterpret_cast<void*>(record+0x44),axis,36);
                for(unsigned k=0;k<3;++k)e.handPoint[k]=point[k];++powerFxHandUpdates;
            }
        }
    }
    if(request==1||request==2||request==4||request==5||request==6||request==7||request==8||request==10||request==11){
        if(startNativePowerFx(request,nullptr,tick)&&request==4){
            for(auto& e:nativePowerEffects)if(e.expires&&e.kind==4){const auto record=nativePowerFxRecord(e);float root[12]{};
                if(record&&memoryRead(e.actor+0x300,root,sizeof(root))){float hand[3]{},end[3]{};
                    for(unsigned k=0;k<3;++k){hand[k]=root[k]+(k==2?1.25f:0);end[k]=hand[k]+root[3+k]*3.f;}
                    std::memcpy(reinterpret_cast<void*>(record+0x38),hand,12);std::memcpy(reinterpret_cast<void*>(record+0x80),end,12);
                }
            }
        }
    }
}
void writeNativePowerFx(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.power-fx.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;unsigned active=0;for(const auto& e:nativePowerEffects)if(e.expires)++active;
    fprintf(f,"{\"handUpdates\":%llu,\"result\":\"%s\",\"active\":%u,\"started\":%llu,\"stopped\":%llu,\"finished\":%llu,\"refused\":%llu,\"retired\":%llu,\"effects\":[",powerFxHandUpdates,powerFxResult,active,powerFxStarted,powerFxStopped,powerFxFinished,powerFxRefused,powerFxRetired);
    bool first=true;for(const auto& e:nativePowerEffects)if(e.expires){fprintf(f,"%s{\"id\":%u,\"owner\":%u,\"kind\":%u,\"record\":\"0x%llx\",\"declaration\":\"0x%llx\",\"handPoint\":[%.6f,%.6f,%.6f]}",first?"":",",e.id,e.owner,e.kind,(unsigned long long)e.record,(unsigned long long)e.declaration,e.handPoint[0],e.handPoint[1],e.handPoint[2]);first=false;}
    fprintf(f,"]}\n");fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
