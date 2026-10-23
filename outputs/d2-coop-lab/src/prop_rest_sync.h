#pragma once
#include "prop_rest_protocol.h"
#include <vector>
// 1214ip (#178): map props come to rest in the same place in both worlds (prop_rest_protocol.h).
// - Each world catalogs its arkMovable (vtable RVA 1c5de18) and arkPickup (1c5e258) entities with their native reference
//   frame component (1c573e8); rescanned per world and every 15 s. Positions are polled every 250 ms.
// - A prop that moved is followed until it has been still for 1 s; then, if it is more than 5 cm from its last rest and
//   not held, not attached (1214ir: no local offset to a parent joint) and not quiet, its transform is sent once and
//   becomes its rest. A rest is not applied to a local copy that is attached (a living NPC's belt key in this world).
// - A received rest places the local copy through the native entity transform setter (0x87d200, as the held object
//   presentation), unless the local player holds it; the prop then stays quiet for 2.5 s, so the peer's move is not sent
//   back. Lab and product: coop_prop_rest on|off (product step 22, maintained).
struct PropRestEntry {std::uint32_t handle=0;std::uintptr_t actor=0,frame=0;std::string name;float rest[3]{},last[3]{};ULONGLONG stillSince=0,quietUntil=0;bool moving=false;std::uint64_t sends=0,applied=0;};
struct PropRestRemote {std::uint32_t handle=0;std::string name;float position[3]{},axis[9]{};};
struct PropRestState {
    bool enabled=false;std::uint64_t epoch=0,peerEpoch=0,map=0,session=0,peerSession=0,sequence=0;
    std::vector<PropRestEntry> entries;std::vector<PropRestRemote> pending;std::vector<std::string> recent;
    ULONGLONG scanAt=0,pollAt=0;unsigned scanMs=0,pollMs=0;
    std::uint64_t scans=0,sent=0,sendFailures=0,received=0,rejected=0,ignored=0,unmatched=0,overflow=0,applied=0,applyRefused=0,
        heldSkips=0,attachedSkips=0,quietSkips=0,worldResets=0;
    const char* result="off";const char* lastApply="none";
};
PropRestState propRest;int propRestEnableRequest=-1;
inline bool propRestEntityVtable(std::uintptr_t vt,std::uintptr_t base){return vt==base+0x1c5de18||vt==base+0x1c5e258;}
bool propRestContext(){
    return sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&coopWorldPairReady(outgoing,remote)&&!doorCheckpointBarrierActive();
}
bool propRestPosition(std::uintptr_t actor,std::uint32_t handle,float (&position)[3]){
    std::uint32_t current=0;
    if(!actor||!memoryRead(actor+0xa8,&current,4)||current!=handle||!memoryRead(actor+0x300,position,12))return false;
    for(float v:position)if(!std::isfinite(v))return false;
    return true;
}
void scanPropRest(ULONGLONG tick){
    auto& s=propRest;const auto started=GetTickCount64();const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t game=0,handles=0;std::uint32_t flags=0;s.scanAt=tick;
    if(!memoryRead(base+0x39a5290,&game,8)||!game||!memoryRead(game+0x601c0,&handles,8)||!handles||!memoryRead(game+0x601c8,&flags,4))return;
    const auto count=flags&0xffffff;if(count>16384)return;
    std::vector<PropRestEntry> next;
    for(std::uint32_t i=0;i<count;++i){std::uint32_t handle=0;std::uintptr_t vt=0,text=0;
        if(!memoryRead(handles+i*4,&handle,4)||!handle)continue;
        const auto actor=worldIdentityEntity(memoryRead,game,handle);if(!actor||!memoryRead(actor,&vt,8))continue;
        if(!propRestEntityVtable(vt,base))continue;
        PropRestEntry e{};e.handle=handle;e.actor=actor;
        std::uintptr_t list=0;std::uint32_t components=0;
        if(memoryRead(actor+0x268,&list,8)&&list&&memoryRead(actor+0x270,&components,4)&&(components&0xffffff)<=64)
            for(std::uint32_t k=0;k<(components&0xffffff);++k){std::uintptr_t c=0,cv=0,owner=0;
                if(memoryRead(list+k*8,&c,8)&&c&&memoryRead(c,&cv,8)&&cv==base+0x1c573e8&&memoryRead(c+0x10,&owner,8)&&owner==actor){e.frame=c;break;}}
        if(!memoryRead(actor+0x20,&text,8)||!worldIdentityString(memoryRead,text,e.name)||e.name.empty()||e.name.size()>=192||!propRestPosition(actor,handle,e.rest))continue;
        std::memcpy(e.last,e.rest,12);
        for(const auto& old:s.entries)if(old.handle==handle&&old.name==e.name){
            std::memcpy(e.rest,old.rest,12);std::memcpy(e.last,old.last,12);e.stillSince=old.stillSince;e.quietUntil=old.quietUntil;e.moving=old.moving;e.sends=old.sends;e.applied=old.applied;break;}
        next.push_back(std::move(e));if(next.size()>=4096)break;
    }
    s.entries=std::move(next);++s.scans;s.scanMs=static_cast<unsigned>(GetTickCount64()-started);
}
bool propRestHeld(std::uint32_t handle,ULONGLONG tick){
    if(shadowHeld.handle==handle&&shadowHeld.phase>=1&&shadowHeld.phase<=3)return true;
    const auto r=remoteHeld;return r.handle==handle&&r.phase&&r.at&&tick-r.at<=2000;
}
// Attached: bound to a parent joint (a local offset in the reference frame), or the frame cannot be read (treated as
// attached, never moved).
bool propRestAttached(const PropRestEntry& e){
    std::uintptr_t owner=0;std::uint64_t offset[2]{};
    if(!e.frame||!memoryRead(e.frame+0x10,&owner,8)||owner!=e.actor||!memoryRead(e.frame+0x70,offset,16))return true;
    return propRestBound(offset[0],offset[1]);
}
bool sendPropRest(PropRestEntry& e,const float (&position)[3],const float (&axis)[9]){
    auto& s=propRest;CoopPropRestPacket p{};p.role=localRole;p.handle=e.handle;p.sequence=++s.sequence;
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceEpoch=outgoing.worldEpoch;p.targetEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;
    std::memcpy(p.position,position,12);std::memcpy(p.axis,axis,36);
    if(e.name.size()>=sizeof(p.name))return false;std::memcpy(p.name,e.name.c_str(),e.name.size());
    if(!validCoopPropRest(p,remote,outgoing)||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK){++s.sendFailures;return false;}
    ++s.sent;++e.sends;if(s.recent.size()>=8)s.recent.erase(s.recent.begin());s.recent.push_back(e.name);return true;
}
void applyPropRest(PropRestEntry& e,const PropRestRemote& r,ULONGLONG tick){
    auto& s=propRest;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,text=0;std::string name;
    const auto actor=worldIdentityEntity(memoryRead,observedGame,e.handle);
    if(!actor||!memoryRead(actor,&vt,8)||!propRestEntityVtable(vt,base)||!memoryRead(actor+0x20,&text,8)||!worldIdentityString(memoryRead,text,name)||name!=r.name){
        ++s.applyRefused;s.lastApply="prop_changed";return;}
    if(shadowHeld.handle==e.handle&&shadowHeld.phase>=1&&shadowHeld.phase<=3){++s.heldSkips;s.lastApply="held_locally";return;}
    if(e.actor!=actor||propRestAttached(e)){++s.attachedSkips;s.lastApply="attached_locally";return;}
    reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(actor),r.position,r.axis,true,0);
    e.actor=actor;std::memcpy(e.rest,r.position,12);std::memcpy(e.last,r.position,12);e.moving=false;e.quietUntil=tick+2500;
    ++e.applied;++s.applied;s.lastApply="applied";
}
void receivePropRest(const CoopPropRestPacket& p){
    auto& s=propRest;
    if(!s.enabled){++s.ignored;return;}
    if(!validCoopPropRest(p,outgoing,remote)){++s.rejected;return;}
    ++s.received;if(s.pending.size()>=128){++s.overflow;return;}
    PropRestRemote r{};r.handle=p.handle;std::memcpy(r.position,p.position,12);std::memcpy(r.axis,p.axis,36);r.name.assign(p.name,strnlen(p.name,sizeof(p.name)));
    s.pending.push_back(std::move(r));
}
void servicePropRest(ULONGLONG tick){
    auto& s=propRest;
    if(propRestEnableRequest>=0){s.enabled=propRestEnableRequest!=0;propRestEnableRequest=-1;s.entries.clear();s.pending.clear();s.scanAt=0;s.result=s.enabled?"waiting_world":"off";}
    if(!s.enabled)return;
    if(!propRestContext()||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()){s.result="waiting_world";return;}
    if(s.epoch!=outgoing.worldEpoch||s.peerEpoch!=remote.worldEpoch||s.map!=outgoing.mapId||s.session!=outgoing.session||s.peerSession!=remote.session){
        s.epoch=outgoing.worldEpoch;s.peerEpoch=remote.worldEpoch;s.map=outgoing.mapId;s.session=outgoing.session;s.peerSession=remote.session;
        s.entries.clear();s.pending.clear();s.scanAt=0;++s.worldResets;}
    if(!s.scanAt||tick-s.scanAt>=15000)scanPropRest(tick);
    for(const auto& r:s.pending){bool matched=false;
        for(auto& e:s.entries)if(e.handle==r.handle&&e.name==r.name){matched=true;applyPropRest(e,r,tick);break;}
        if(!matched)++s.unmatched;}
    s.pending.clear();
    if(tick-s.pollAt>=250){s.pollAt=tick;const auto started=GetTickCount64();
        for(auto& e:s.entries){float position[3]{};
            if(!propRestPosition(e.actor,e.handle,position))continue;
            const float dx=position[0]-e.last[0],dy=position[1]-e.last[1],dz=position[2]-e.last[2];
            if(dx*dx+dy*dy+dz*dz>.0001f){e.moving=true;e.stillSince=tick;std::memcpy(e.last,position,12);continue;}
            if(!e.moving||tick-e.stillSince<1000)continue;
            e.moving=false;
            const float rx=position[0]-e.rest[0],ry=position[1]-e.rest[1],rz=position[2]-e.rest[2];
            const float moved=std::sqrt(rx*rx+ry*ry+rz*rz);
            if(moved<=.05f)continue;
            const bool held=propRestHeld(e.handle,tick),quiet=tick<e.quietUntil,attached=propRestAttached(e);
            float axis[9]{};
            if(propRestShouldSend(moved,static_cast<unsigned>(tick-e.stillSince),held,attached,quiet)&&memoryRead(e.actor+0x30c,axis,36)&&
               validPropRestTransform(position,axis)&&sendPropRest(e,position,axis)){std::memcpy(e.rest,position,12);continue;}
            if(held)++s.heldSkips;else if(attached)++s.attachedSkips;else if(quiet)++s.quietSkips;
            std::memcpy(e.rest,position,12);
        }
        s.pollMs=static_cast<unsigned>(GetTickCount64()-started);
    }
    s.result="active";
}
void writePropRest(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.prop-rest.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=propRest;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"lastApply\":\"%s\",\"catalog\":%zu,\"scans\":%llu,\"scanMs\":%u,\"pollMs\":%u,\"sent\":%llu,\"sendFailures\":%llu,"
        "\"received\":%llu,\"rejected\":%llu,\"ignored\":%llu,\"unmatched\":%llu,\"overflow\":%llu,\"applied\":%llu,\"applyRefused\":%llu,\"heldSkips\":%llu,"
        "\"attachedSkips\":%llu,\"quietSkips\":%llu,\"worldResets\":%llu,\"recent\":[",
        s.enabled?"true":"false",s.result,s.lastApply,s.entries.size(),s.scans,s.scanMs,s.pollMs,s.sent,s.sendFailures,s.received,s.rejected,
        s.ignored,s.unmatched,s.overflow,s.applied,s.applyRefused,s.heldSkips,s.attachedSkips,s.quietSkips,s.worldResets);
    for(std::size_t i=0;i<s.recent.size();++i){if(i)fputc(',',f);writeWorldText(f,s.recent[i].c_str());}
    fputs("]}\n",f);const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
