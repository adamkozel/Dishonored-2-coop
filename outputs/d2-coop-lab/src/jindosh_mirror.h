#pragma once
// Stage 8.4.3 (#239): one Jindosh lock answer for both players (jindosh_protocol.h). Each world rolled its own riddle, so
// the two players read different riddles and had different answers.
// D2JL: in plain gameplay on the lock's map the host sends its five answer lists every 2 s when they change, and at least
// every 6 s. The partner, after 3 s of plain gameplay on the same map and once her own roll has finished (her five lists
// are a valid answer and her riddle list 15 is formatted from them):
//  - writes the host's values into the five lists of the lock's jindosh_enigma.kiscule instance and into the copies in
//    each seat's jindosh_enigma_wheel.kiscule instance (raw ints in the value idList: no Updated event, no reroll);
//  - overwrites in place the six journal lists (the riddle and the solution notes) with the values derived from the
//    host's answer, only where the list's count matches (never the formatter 0x140725a50, which stops the game on an
//    unknown id).
// Never sent or called: the decl's Init, any Enigma2.* event (Cheat.Randomize rerolls), any variable Set path. Each
// player dials their own lock, which now has the host's answer. It follows the safe-code switch (coop_safe_codes,
// d2coop.cfg safe_codes=0): a host with it off sends nothing, so each world keeps its own riddle, as in 0.9.6.
#include "jindosh_protocol.h"
#include <array>
#include <cstring>
#include <string>
#include <vector>
constexpr std::uintptr_t kKisculeListIntVariableVtableRva=0x1cd9bc8; // ListInt (story_flag_mirror.h, confirmed runtime)
struct JindoshLockVars {std::uintptr_t main[kJindoshLists]{};std::vector<std::array<std::uintptr_t,kJindoshLists>> seats;};
struct JindoshSync {
    std::uint64_t sequence=0,peerSequence=0,sent=0,received=0,refused=0,listWrites=0,seatWrites=0,journalWrites=0,
        notReady=0,journalMismatch=0,readFailures=0;
    ULONGLONG sentAt=0,checkedAt=0,receivedAt=0,appliedAt=0,gameplaySince=0;std::uint64_t gameplayMap=0,gameplayEpoch=0;
    std::uint32_t lockHandle=0;std::uint64_t lockEpoch=0;std::uint32_t sentDigest=0;unsigned seats=0;bool lockFound=false,localValid=false;
    std::int32_t local[kJindoshLists][kJindoshListLength]{};std::int32_t riddle[25]{};std::uint32_t riddleCount=0;
    const char* result="idle";JindoshPacket last{};bool lastValid=false;
};
JindoshSync jindoshSync{};
// A ListInt variable of a Kiscule graph by name; its value idList is {int* at +0x28, count at +0x30 (low 24 bits)}.
std::uintptr_t jindoshListVariable(std::uintptr_t graph,const char* wanted){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vars=0;std::uint32_t count=0;
    if(!graph||!memoryRead(graph+0x40,&vars,8)||!vars||!memoryRead(graph+0x48,&count,4))return 0;count&=0xffffff;if(count>512)return 0;
    for(std::uint32_t v=0;v<count;++v){std::uintptr_t var=0,vt=0,name=0;std::string text;
        if(!memoryRead(vars+v*8,&var,8)||!var||!memoryRead(var,&vt,8)||vt!=base+kKisculeListIntVariableVtableRva||!memoryRead(var+8,&name,8)||
           !worldIdentityString(memoryRead,name,text)||text!=wanted)continue;
        return var;}
    return 0;
}
bool jindoshListRead(std::uintptr_t var,std::int32_t (&out)[kJindoshListLength]){
    std::uintptr_t values=0;std::uint32_t count=0;
    if(!var||!memoryRead(var+0x28,&values,8)||!values||!memoryRead(var+0x30,&count,4)||(count&0xffffff)!=kJindoshListLength)return false;
    return memoryRead(values,out,sizeof(out));
}
// Writes the differing ints of a 6-int list in place; returns how many it wrote (0 when the list has another length).
unsigned jindoshListWrite(std::uintptr_t var,const std::int32_t (&value)[kJindoshListLength]){
    std::uintptr_t values=0;std::uint32_t count=0;unsigned writes=0;
    if(!var||!memoryRead(var+0x28,&values,8)||!values||!memoryRead(var+0x30,&count,4)||(count&0xffffff)!=kJindoshListLength)return 0;
    for(unsigned i=0;i<kJindoshListLength;++i){std::int32_t now=0;if(memoryRead(values+i*4ull,&now,4)&&now!=value[i]){*reinterpret_cast<std::int32_t*>(values+i*4ull)=value[i];++writes;}}
    return writes;
}
// The nested decl instances of a Kiscule instance whose decl name ends with the suffix (table +0x88, buckets +0x90).
void jindoshNested(std::uintptr_t instance,const char* suffix,std::vector<std::uintptr_t>& out){
    out.clear();std::uintptr_t table=0;std::uint32_t buckets=0;const std::size_t n=std::strlen(suffix);
    if(!instance||!memoryRead(instance+0x88,&table,8)||!table||!memoryRead(instance+0x90,&buckets,4))return;
    buckets&=0xffffff;if(!buckets||buckets>4096)return;
    for(std::uint32_t b=0;b<buckets;++b){std::uintptr_t node=0;if(!memoryRead(table+b*8,&node,8))return;
        for(unsigned hops=0;node&&hops<64;++hops){std::uintptr_t nested=0,decl=0,declName=0,next=0;std::string text;
            if(memoryRead(node+8,&nested,8)&&nested&&memoryRead(nested+0x68,&decl,8)&&decl&&memoryRead(decl+8,&declName,8)&&
               worldIdentityString(memoryRead,declName,text)&&text.size()>=n&&!text.compare(text.size()-n,n,suffix)&&out.size()<16)out.push_back(nested);
            if(!memoryRead(node+0x10,&next,8))break;node=next;}}
}
// The lock of this world: the entity ld_enigma_v2_kiscule (its handle is cached per world epoch), its
// jindosh_enigma.kiscule instance and that instance's seat instances.
bool readJindoshLock(JindoshLockVars& out){
    auto& s=jindoshSync;out=JindoshLockVars{};if(!observedGame)return false;
    std::uintptr_t actor=0;
    if(s.lockHandle&&s.lockEpoch==outgoing.worldEpoch){const unsigned slot=s.lockHandle>>16;std::uint32_t handle=0;
        if(!memoryRead(observedGame+0x3130b0+slot*8ull,&actor,8)||!actor||!memoryRead(actor+0xa8,&handle,4)||handle!=s.lockHandle)actor=0;}
    if(!actor){s.lockHandle=0;
        if(s.lockEpoch==outgoing.worldEpoch&&!s.lockFound)return false; // this world has no lock (scanned once per epoch)
        s.lockEpoch=outgoing.worldEpoch;s.lockFound=false;
        for(unsigned slot=0;slot<16384&&!actor;++slot){std::uintptr_t a=0,p=0;std::uint32_t handle=0;std::string name;
            if(!memoryRead(observedGame+0x3130b0+slot*8ull,&a,8)||!a||!memoryRead(a+0xa8,&handle,4)||(handle>>16)!=slot||
               !memoryRead(a+0x20,&p,8)||!worldIdentityString(memoryRead,p,name)||name!="ld_enigma_v2_kiscule")continue;
            actor=a;s.lockHandle=handle;}
        if(!actor)return false;s.lockFound=true;}
    std::vector<std::uintptr_t> mains,wheels;jindoshNested(kisculeInstanceFor(s.lockHandle),"jindosh_enigma.kiscule",mains);
    if(mains.size()!=1)return false;std::uintptr_t graph=0;
    if(!memoryRead(mains[0]+0x70,&graph,8)||!graph)return false;
    for(unsigned k=0;k<kJindoshLists;++k)if(!(out.main[k]=jindoshListVariable(graph,kJindoshListNames[k])))return false;
    jindoshNested(mains[0],"jindosh_enigma_wheel.kiscule",wheels);
    for(const auto w:wheels){std::uintptr_t g=0;std::array<std::uintptr_t,kJindoshLists> seat{};
        if(!memoryRead(w+0x70,&g,8)||!g)continue;for(unsigned k=0;k<kJindoshLists;++k)seat[k]=jindoshListVariable(g,kJindoshListNames[k]);
        out.seats.push_back(seat);}
    return true;
}
// This world's local player's journal lists (as readSafeJournal: +0xd8 {int* values, count} per decl index, stride 0x10).
bool readJindoshJournalList(unsigned index,std::uintptr_t& values,std::uint32_t& count){
    values=0;count=0;std::uintptr_t player=0;if(!observedGame||!memoryRead(observedGame+0x3130b0,&player,8)||!player)return false;
    const auto journal=reinterpret_cast<std::uintptr_t(*)(void*,void*,int)>(gameImage+0x877720)(reinterpret_cast<void*>(player),gameImage+0x39c5ac0,2);
    std::uintptr_t lists=0;std::uint32_t listCount=0;
    if(!journal||!memoryRead(journal+0xd8,&lists,8)||!lists||!memoryRead(journal+0xe0,&listCount,4))return false;
    listCount&=0xffffff;if(index>=listCount||listCount>4096)return false;
    if(!memoryRead(lists+index*0x10ull,&values,8)||!memoryRead(lists+index*0x10ull+8,&count,4))return false;count&=0xffffff;return true;
}
void receiveJindosh(const JindoshPacket& p){
    auto& s=jindoshSync;++s.received;
    if(!validJindosh(p)||localRole!=1||p.session!=remote.session||p.sequence<=s.peerSequence){++s.refused;return;}
    s.peerSequence=p.sequence;s.last=p;s.lastValid=true;s.receivedAt=GetTickCount64();
}
void serviceJindosh(ULONGLONG tick){
    auto& s=jindoshSync;if(!safeCodeSync.enabled){s.result="off";return;}
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected){s.result="no_session";return;}
    if(outgoing.gameLogicState!=1||outgoing.pendingGameLogicState){s.gameplaySince=0;s.result="waiting_gameplay";return;}
    if(!s.gameplaySince||s.gameplayMap!=outgoing.mapId||s.gameplayEpoch!=outgoing.worldEpoch){s.gameplaySince=tick;s.gameplayMap=outgoing.mapId;s.gameplayEpoch=outgoing.worldEpoch;}
    if(tick-s.gameplaySince<3000){s.result="settling";return;}
    if(tick-s.checkedAt<(localRole==0?2000u:1000u))return;s.checkedAt=tick;
    JindoshLockVars lock{};
    if(!readJindoshLock(lock)){s.lockFound=s.lockHandle!=0;s.result=s.lockEpoch==outgoing.worldEpoch&&!s.lockFound?"no_lock":"unreadable";return;}
    s.seats=static_cast<unsigned>(lock.seats.size());
    s.localValid=true;for(unsigned k=0;k<kJindoshLists;++k)if(!jindoshListRead(lock.main[k],s.local[k]))s.localValid=false;
    s.localValid=s.localValid&&validJindoshLists(s.local);
    {std::uintptr_t v=0;std::uint32_t n=0;s.riddleCount=0; // this world's riddle as the player reads it, for the take's checks
     if(readJindoshJournalList(15,v,n)&&v&&n==25&&memoryRead(v,s.riddle,sizeof(s.riddle)))s.riddleCount=n;}
    if(localRole==0){
        if(!s.localValid){s.result="host_not_rolled";return;}
        JindoshPacket p{};p.session=outgoing.session;p.mapId=outgoing.mapId;p.epoch=outgoing.worldEpoch;std::memcpy(p.lists,s.local,sizeof(p.lists));
        std::uint32_t digest=2166136261u;const auto* bytes=reinterpret_cast<const unsigned char*>(p.lists);
        for(std::size_t i=0;i<sizeof(p.lists);++i){digest^=bytes[i];digest*=16777619u;}digest^=static_cast<std::uint32_t>(p.mapId);
        if(digest==s.sentDigest&&tick-s.sentAt<6000)return;
        p.sequence=++s.sequence;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_Reliable,nullptr)==k_EResultOK){++s.sent;s.sentDigest=digest;s.result="publishing";}
        s.sentAt=tick;return;
    }
    if(!s.lastValid||tick-s.receivedAt>15000||s.last.mapId!=outgoing.mapId){s.result="waiting_host";return;}
    // Her own roll must be over first (a write during it races the roll's unique AddItem and its RandomDone formatting):
    // her lists are a full answer, or already the host's, and her riddle list is formatted.
    std::uintptr_t riddle=0;std::uint32_t riddleCount=0;
    if(!s.localValid||!readJindoshJournalList(15,riddle,riddleCount)||riddleCount!=25){++s.notReady;s.result="waiting_own_roll";return;}
    for(unsigned k=0;k<kJindoshLists;++k){s.listWrites+=jindoshListWrite(lock.main[k],s.last.lists[k]);
        for(const auto& seat:lock.seats)s.seatWrites+=jindoshListWrite(seat[k],s.last.lists[k]);}
    std::vector<std::int32_t> derived;
    for(const auto index:kJindoshJournalIndex){std::uintptr_t values=0;std::uint32_t count=0;
        if(!readJindoshJournalList(index,values,count)||!values){++s.readFailures;continue;}
        if(count!=jindoshJournalCount(index)){++s.journalMismatch;continue;}
        jindoshJournalValues(s.last.lists,index,derived);
        for(std::uint32_t i=0;i<count;++i){std::int32_t now=0;if(memoryRead(values+i*4ull,&now,4)&&now!=derived[i]){*reinterpret_cast<std::int32_t*>(values+i*4ull)=derived[i];++s.journalWrites;}}}
    for(unsigned k=0;k<kJindoshLists;++k)jindoshListRead(lock.main[k],s.local[k]);
    if(riddle&&memoryRead(riddle,s.riddle,sizeof(s.riddle)))s.riddleCount=25;
    s.appliedAt=tick;s.result="applied";
}
void writeJindosh(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.jindosh.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=jindoshSync;
    fprintf(f,"{\"enabled\":%s,\"role\":%u,\"result\":\"%s\",\"lockFound\":%s,\"seats\":%u,\"localValid\":%s,\"sent\":%llu,\"received\":%llu,\"refused\":%llu,"
        "\"listWrites\":%llu,\"seatWrites\":%llu,\"journalWrites\":%llu,\"notReady\":%llu,\"journalMismatch\":%llu,\"readFailures\":%llu,\"local\":[",
        safeCodeSync.enabled?"true":"false",localRole,s.result,s.lockFound?"true":"false",s.seats,s.localValid?"true":"false",s.sent,s.received,s.refused,
        s.listWrites,s.seatWrites,s.journalWrites,s.notReady,s.journalMismatch,s.readFailures);
    // This world's answer lists at the last service, and the host's last received ones, for the take's checks.
    for(unsigned k=0;k<kJindoshLists;++k){fprintf(f,"%s[",k?",":"");for(unsigned i=0;i<kJindoshListLength;++i)fprintf(f,"%s%d",i?",":"",s.local[k][i]);fputs("]",f);}
    fputs("],\"host\":[",f);
    for(unsigned k=0;k<kJindoshLists;++k){fprintf(f,"%s[",k?",":"");for(unsigned i=0;i<kJindoshListLength;++i)fprintf(f,"%s%d",i?",":"",s.lastValid?s.last.lists[k][i]:0);fputs("]",f);}
    fputs("],\"riddle\":[",f);for(std::uint32_t i=0;i<s.riddleCount;++i)fprintf(f,"%s%d",i?",":"",s.riddle[i]);
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
