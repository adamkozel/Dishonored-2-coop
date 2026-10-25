#pragma once
// 1214kj (#220): the host's story flags are the decisions of both worlds.
// The storyflags/*.kiscule script instances on the worldspawn hold the campaign's decisions as bool variables
// (chapter 110: alive and non-lethal outcomes, robberies, items, Meagan's lines); the next maps and the endings read
// them. Layout (confirmed static and runtime, d2_kiscule_vars.py): the worldspawn's Kiscule instance (kisculeInstanceFor)
// keeps the flag instances in its table +0x88 (bucket count +0x90; chains {key, instance +8, next +0x10}); an instance's
// decl is at +0x68 (name +8) and its graph at +0x70 lists the variables at +0x40 (count +0x48); a variable keeps its name
// at +8 and, for a bool, the default at +0x18 and the current value at +0x19.
// Every 2 s in gameplay (and at once on a change) the host sends its bool flags by name hash (D2SF). The partner's world
// writes each differing value byte to the host's. The raw write fires no script update; later reads see the host's
// decisions. A partner-made outcome counts when it reaches the host's world (NPC outcomes, carried bodies, shared
// usables, story grants); one that stays in her world is replaced by the host's value.
// Lab: coop_story_flags on|off.
#include "story_flag_protocol.h"
constexpr std::uintptr_t kWorldspawnVtableRva=0x1c89868,kKisculeBoolVariableVtableRva=0x1cc4388; // confirmed runtime (Dunwall: 23 bools; ints 0x1c9f998, lists 0x1cd9bc8)
struct StoryFlag {std::uint32_t decl=0,name=0;std::uintptr_t var=0;std::uint8_t value=0;};
struct StoryFlagSync {
    bool enabled=true;std::uint64_t sequence=0,peerSequence=0,sent=0,received=0,refused=0,writes=0,unknown=0,readFailures=0;
    ULONGLONG sentAt=0,receivedAt=0,appliedAt=0;std::uint32_t sentDigest=0;unsigned local=0,host=0,differing=0;const char* result="idle";
    StoryFlagPacket last{};bool lastValid=false;
};
StoryFlagSync storyFlagSync{};
// The local world's bool story flags. Reads only; false when the worldspawn or its flags are unreadable.
bool readStoryFlags(std::vector<StoryFlag>& out){
    out.clear();const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!observedGame||!kKisculeBoolVariableVtableRva)return false;
    const std::uint32_t worldspawn=0x3ffe0000;std::uintptr_t vt=0;
    const auto actor=worldIdentityEntity(memoryRead,observedGame,worldspawn);
    if(!actor||!memoryRead(actor,&vt,8)||vt!=base+kWorldspawnVtableRva)return false;
    const auto instance=kisculeInstanceFor(worldspawn);std::uintptr_t table=0;std::uint32_t buckets=0;
    if(!instance||!memoryRead(instance+0x88,&table,8)||!table||!memoryRead(instance+0x90,&buckets,4))return false;
    buckets&=0xffffff;if(buckets>4096)return false;
    for(std::uint32_t b=0;b<buckets;++b){std::uintptr_t node=0;if(!memoryRead(table+b*8,&node,8))return false;
        for(unsigned hops=0;node&&hops<64;++hops){
            std::uintptr_t flags=0,decl=0,declName=0,graph=0,vars=0,next=0;std::uint32_t count=0;std::string declText;
            if(!memoryRead(node+8,&flags,8)||!flags||!memoryRead(flags+0x68,&decl,8)||!decl||!memoryRead(decl+8,&declName,8)||
               !worldIdentityString(memoryRead,declName,declText)||!memoryRead(flags+0x70,&graph,8)||!graph||
               !memoryRead(graph+0x40,&vars,8)||!memoryRead(graph+0x48,&count,4))return false;
            count&=0xffffff;if(count>512)return false;const auto declHash=storyFlagHash(declText);
            for(std::uint32_t v=0;v<count;++v){std::uintptr_t var=0,varVt=0,name=0;std::string nameText;std::uint8_t value=0;
                if(!memoryRead(vars+v*8,&var,8)||!var||!memoryRead(var,&varVt,8))return false;
                if(varVt!=base+kKisculeBoolVariableVtableRva)continue;
                if(!memoryRead(var+8,&name,8)||!worldIdentityString(memoryRead,name,nameText)||!memoryRead(var+0x19,&value,1)||value>1)return false;
                if(out.size()>=kStoryFlagMax)return false;
                out.push_back({declHash,storyFlagHash(nameText),var,value});}
            if(!memoryRead(node+0x10,&next,8))return false;node=next;}}
    return true;
}
void receiveStoryFlags(const StoryFlagPacket& p){
    auto& s=storyFlagSync;++s.received;
    if(!validStoryFlags(p)||localRole!=1||p.session!=remote.session||p.sequence<=s.peerSequence){++s.refused;return;}
    s.peerSequence=p.sequence;s.last=p;s.lastValid=true;s.receivedAt=GetTickCount64();s.host=p.count;
}
void serviceStoryFlags(ULONGLONG tick){
    auto& s=storyFlagSync;if(!s.enabled){s.result="off";return;}
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected){s.result="no_session";return;}
    if(outgoing.gameLogicState!=1||outgoing.pendingGameLogicState){s.result="waiting_gameplay";return;}
    if(localRole==0){
        if(tick-s.sentAt<500)return;
        std::vector<StoryFlag> flags;if(!readStoryFlags(flags)){++s.readFailures;s.result="unreadable";return;}
        std::uint32_t digest=2166136261u;for(const auto& f:flags){digest^=f.decl^(f.name*31u)^(f.value*977u);digest*=16777619u;}
        s.local=static_cast<unsigned>(flags.size());if(digest==s.sentDigest&&tick-s.sentAt<2000)return;
        StoryFlagPacket p{};p.session=outgoing.session;p.sequence=++s.sequence;p.count=static_cast<std::uint32_t>(flags.size());
        for(unsigned i=0;i<p.count;++i)p.entries[i]={flags[i].decl,flags[i].name,flags[i].value};
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_Reliable,nullptr)==k_EResultOK){++s.sent;s.sentAt=tick;s.sentDigest=digest;s.result="publishing";}
        return;
    }
    if(!s.lastValid||tick-s.receivedAt>10000){s.result="waiting_host";return;}
    if(tick-s.appliedAt<500)return;s.appliedAt=tick;
    std::vector<StoryFlag> flags;if(!readStoryFlags(flags)){++s.readFailures;s.result="unreadable";return;}
    s.local=static_cast<unsigned>(flags.size());unsigned differing=0;
    for(unsigned i=0;i<s.last.count;++i){const auto& e=s.last.entries[i];bool found=false;
        for(const auto& f:flags)if(f.decl==e.decl&&f.name==e.name){found=true;
            if(f.value!=e.value){*reinterpret_cast<std::uint8_t*>(f.var+0x19)=e.value;++differing;++s.writes;}break;}
        if(!found)++s.unknown;}
    s.differing=differing;s.result=differing?"applied":"matched";
}
void writeStoryFlags(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.story-flags.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=storyFlagSync;
    fprintf(f,"{\"enabled\":%s,\"role\":%u,\"result\":\"%s\",\"local\":%u,\"host\":%u,\"differing\":%u,\"sent\":%llu,\"received\":%llu,\"refused\":%llu,\"writes\":%llu,\"unknown\":%llu,\"readFailures\":%llu}\n",
        s.enabled?"true":"false",localRole,s.result,s.local,s.host,s.differing,s.sent,s.received,s.refused,s.writes,s.unknown,s.readFailures);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
