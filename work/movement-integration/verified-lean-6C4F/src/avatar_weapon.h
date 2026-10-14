#pragma once
#include "native_player_sword.h"
// Fifteen-second, nonrenewable, explicitly armed native partner sword stream.
// Each equipment application also requires a fresh two-token handshake and
// a state packet no older than 250 ms. Uses the existing local native lease.
struct AvatarWeaponState {
    ULONGLONG until=0,lastHello=0,lastSend=0,lastPeer=0,lastState=0;
    std::uint64_t token=0,peerToken=0,epoch=0,peerEpoch=0,peerSession=0,lastSequence=0;
    bool peerReady=false;unsigned state=0;
};
AvatarWeaponState avatarWeapon{};
bool avatarWeaponArmed(){return avatarWeapon.until!=0;}
std::uint64_t avatarWeaponNonce=0,avatarWeaponSequence=0,avatarWeaponStarted=0,avatarWeaponSent=0,avatarWeaponReceived=0,avatarWeaponAccepted=0,avatarWeaponRejected=0,avatarWeaponRefused=0;
CoopAvatarWeaponPacket avatarWeaponLastPacket{};
const char* avatarWeaponResult="idle";
bool avatarWeaponPairReady(ULONGLONG tick){
    return GetCurrentThreadId()==engineThread.load()&&probeFollowExact&&armProbePairReady(tick);
}
bool avatarWeaponCurrent(ULONGLONG tick){
    return avatarWeapon.until>tick&&avatarWeaponPairReady(tick)&&avatarWeapon.epoch==outgoing.worldEpoch&&
        avatarWeapon.peerEpoch==remote.worldEpoch&&avatarWeapon.peerSession==remote.session;
}
void stopAvatarWeapon(const char* reason){
    if(!avatarWeapon.until)return;avatarWeapon.until=0;avatarWeapon.peerReady=false;avatarWeaponResult=reason;
    if(avatarSword.remoteDriven)stopAvatarSword(reason);
}
void beginAvatarWeapon(){
    const auto tick=GetTickCount64();const auto refuse=[&](const char* reason){++avatarWeaponRefused;avatarWeaponResult=reason;};
    if(avatarWeapon.until||avatarSword.until||avatarSword.restoreUntil){refuse("duplicate_or_equipment_busy");return;}
    if(!avatarWeaponPairReady(tick)){refuse("pair_unavailable");return;}
    if(!avatarSwordReady(tick)){refuse("counterpart_unavailable");return;}
    NativeSwordEquipment counterpart{};NativePlayerSword source{};
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSwordEquipment(memoryRead,base,probeHandle,counterpart,localRole?NativeSwordScope::CorvoTutorial:NativeSwordScope::EmilyPartner)||
       counterpart.actor!=probeActor){refuse("counterpart_inventory_unavailable");return;}
    if(!readNativePlayerSword(memoryRead,base,source)||source.character!=localRole+1||source.sword.logic!=1||
       source.sword.game!=observedGame){refuse("source_sword_unavailable");return;}
    avatarWeapon={};avatarWeapon.until=tick+15000;avatarWeapon.token=++avatarWeaponNonce;
    avatarWeapon.epoch=outgoing.worldEpoch;avatarWeapon.peerEpoch=remote.worldEpoch;avatarWeapon.peerSession=remote.session;
    ++avatarWeaponStarted;avatarWeaponResult="awaiting_peer";
}
CoopAvatarWeaponPacket avatarWeaponPacket(unsigned kind){
    CoopAvatarWeaponPacket p{};p.sequence=++avatarWeaponSequence;p.role=localRole;p.character=localRole+1;p.kind=kind;
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;
    p.mapId=outgoing.mapId;p.rootSequence=outgoing.sequence;p.sourceToken=avatarWeapon.token;p.targetToken=avatarWeapon.peerToken;return p;
}
void sendAvatarWeapon(ULONGLONG tick){
    if(!nativeMutationAllowed||!avatarWeaponCurrent(tick))return;
    if(tick-avatarWeapon.lastHello>=100){
        avatarWeapon.lastHello=tick;const auto p=avatarWeaponPacket(0);
        if(validAvatarWeapon(p,remote,outgoing)&&sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++avatarWeaponSent;
    }
    if(!avatarWeapon.peerReady||tick-avatarWeapon.lastSend<33)return;
    avatarWeapon.lastSend=tick;NativePlayerSword s{};
    if(!readNativePlayerSword(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s)||s.character!=localRole+1||s.sword.logic!=1||s.sword.game!=observedGame)return;
    auto p=avatarWeaponPacket(1);p.state=s.sword.equipped?1:0;
    if(validAvatarWeapon(p,remote,outgoing)&&sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){
        ++avatarWeaponSent;avatarWeaponLastPacket=p;avatarWeapon.state=p.state;
    }
}
void duplicateAvatarWeapon(){
    if(avatarWeaponCurrent(GetTickCount64())&&avatarWeaponLastPacket.sequence&&validAvatarWeapon(avatarWeaponLastPacket,remote,outgoing))
        sockets->SendMessageToConnection(connection,&avatarWeaponLastPacket,sizeof(avatarWeaponLastPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void receiveAvatarWeapon(const CoopAvatarWeaponPacket& p,ULONGLONG tick){
    ++avatarWeaponReceived;
    if(!nativeMutationAllowed||!avatarWeaponCurrent(tick)||!validAvatarWeapon(p,outgoing,remote)||p.sequence<=avatarWeapon.lastSequence||
       (avatarWeapon.peerToken&&p.sourceToken!=avatarWeapon.peerToken)||(p.targetToken&&p.targetToken!=avatarWeapon.token)){++avatarWeaponRejected;return;}
    if(p.kind==0){
        avatarWeapon.peerToken=p.sourceToken;avatarWeapon.lastSequence=p.sequence;avatarWeapon.lastPeer=tick;
        if(p.targetToken==avatarWeapon.token){avatarWeapon.peerReady=true;avatarWeaponResult="ready";}return;
    }
    if(!avatarWeapon.peerReady||!avatarWeaponTokensMatch(p,avatarWeapon.token,avatarWeapon.peerToken)||
       !avatarSwordReady(tick)||(avatarSword.until&&!avatarSword.remoteDriven)){++avatarWeaponRejected;return;}
    avatarWeapon.lastSequence=p.sequence;avatarWeapon.lastState=tick;avatarWeapon.state=p.state;++avatarWeaponAccepted;
    if(!p.state){if(avatarSword.remoteDriven)stopAvatarSword("remote_holstered");return;}
    const auto until=(std::min)((std::min)(tick+250,lastReceive+250),avatarWeapon.until);
    if(avatarSword.until&&avatarSword.remoteDriven)avatarSword.until=until;
    else if(!avatarSword.restoreUntil)beginAvatarSword(true,until);
}
void serviceAvatarWeapon(ULONGLONG tick){
    if(!avatarWeapon.until)return;
    if(!avatarWeaponCurrent(tick)||(avatarWeapon.peerReady&&tick-avatarWeapon.lastPeer>350)){
        stopAvatarWeapon(tick>=avatarWeapon.until?"expired":"scope_or_peer_changed");return;
    }
    if(avatarSword.remoteDriven&&avatarSword.until&&(!avatarWeapon.lastState||tick-avatarWeapon.lastState>250))stopAvatarSword("equipment_stream_stale");
}
void writeAvatarWeapon(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.avatar-weapon.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"armed\":%s,\"ready\":%s,\"remoteEquipmentActive\":%s,\"remainingMs\":%llu,\"token\":%llu,\"peerToken\":%llu,\"epoch\":%llu,\"peerEpoch\":%llu,\"peerSession\":%llu,\"lastSequence\":%llu,\"state\":%u,\"started\":%llu,\"sent\":%llu,\"received\":%llu,\"accepted\":%llu,\"rejected\":%llu,\"refused\":%llu,\"result\":\"%s\"}\n",
        avatarWeapon.until>tick?"true":"false",avatarWeapon.peerReady?"true":"false",avatarSword.remoteDriven&&avatarSword.until>tick?"true":"false",avatarWeapon.until>tick?avatarWeapon.until-tick:0,
        avatarWeapon.token,avatarWeapon.peerToken,avatarWeapon.epoch,avatarWeapon.peerEpoch,avatarWeapon.peerSession,avatarWeapon.lastSequence,avatarWeapon.state,
        avatarWeaponStarted,avatarWeaponSent,avatarWeaponReceived,avatarWeaponAccepted,avatarWeaponRejected,avatarWeaponRefused,avatarWeaponResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
