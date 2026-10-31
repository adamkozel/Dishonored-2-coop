#pragma once
// Stage 8.4.3 (#239): one safe code for both players. Every safe code is a Kiscule int rolled by a script in each world
// (the 3_digits_lock.kiscule decl nested in the safe's entity instance, or the level script that writes its Combination;
// stage8-step1-notes/safe-code-map.md), so each world had its own code, its own note text and its own dial start
// positions (the safe-dial differences of every two-account world diff, chapter 119).
// D2SC: in plain gameplay the host sends, every 2 s, each lock's Combination, Position and PositionDigit1-3, each safe
// dial's state (a 10-state usable named *combinaison_numbers*), and its journal's formatted digit lists (three ints 0..9,
// the note and clue texts of safe codes). The partner, after 3 s of plain gameplay on the same map:
//  - writes the host's Combination into its own lock and the safe entity's Combination whenever they differ (a raw int
//    write at +0x1c: no Updated event, so no reroll);
//  - once per map load, writes the host's Position and PositionDigit1-3 and puts each dial on the host's state with the
//    quiet native state application (0x14077a880 with transition 0, quiet 1: no script event), so the start positions
//    match; after that each player turns their own dials and opens their own safe;
//  - overwrites in place each journal digit list whose text id and length match its own (never a foreign id: the native
//    formatter stops the game on an unknown one).
// Lab: coop_safe_codes on|off (on by default); off restores 0.9.6 (per-world codes). The host decides: a host with it
// off sends nothing.
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
constexpr std::uint32_t kSafeCodeMagic=0x43533244; // "D2SC"
constexpr unsigned kSafeLockMax=32,kSafeDialMax=96,kSafeJournalMax=32;
constexpr std::uintptr_t kKisculeIntVariableVtableRva=0x1c9f998; // story_flag_mirror.h (confirmed runtime)
#pragma pack(push,1)
struct SafeLockEntry {std::uint32_t entity=0;std::int32_t combination=0,position=0,digit[3]{};};
struct SafeDialEntry {std::uint32_t name=0;std::int32_t state=0;};
struct SafeJournalEntry {std::uint32_t id=0,count=0;std::int32_t value[4]{};};
struct SafeCodePacket {
    std::uint32_t magic=kSafeCodeMagic;std::uint16_t version=1,size=sizeof(SafeCodePacket);
    std::uint32_t role=0,locks=0,dials=0,journal=0;std::uint64_t session=0,sequence=0,mapId=0,epoch=0;
    SafeLockEntry lock[kSafeLockMax]{};SafeDialEntry dial[kSafeDialMax]{};SafeJournalEntry text[kSafeJournalMax]{};
};
#pragma pack(pop)
inline bool validSafeCodes(const SafeCodePacket& p){
    if(p.magic!=kSafeCodeMagic||p.version!=1||p.size!=sizeof(p)||p.role!=0||!p.sequence||p.locks>kSafeLockMax||p.dials>kSafeDialMax||p.journal>kSafeJournalMax)return false;
    for(unsigned i=0;i<p.locks;++i){const auto& l=p.lock[i];if(!l.entity||l.combination<0||l.combination>999||l.position<0||l.position>999)return false;
        for(int d:l.digit)if(d<0||d>9)return false;}
    for(unsigned i=0;i<p.dials;++i)if(!p.dial[i].name||p.dial[i].state<0||p.dial[i].state>9)return false;
    for(unsigned i=0;i<p.journal;++i){const auto& t=p.text[i];if(t.count<1||t.count>4)return false;for(unsigned k=0;k<t.count;++k)if(t.value[k]<0||t.value[k]>9)return false;}
    return true;
}
struct SafeLockVars {std::uint32_t entity=0;std::uintptr_t combination=0,position=0,digit[3]{},entityCombination=0;};
struct SafeCodeSync {
    bool enabled=true;std::uint64_t sequence=0,peerSequence=0,sent=0,received=0,refused=0,codeWrites=0,positionWrites=0,dialWrites=0,
        journalWrites=0,unknownLocks=0,unknownDials=0,unknownTexts=0,readFailures=0;
    ULONGLONG sentAt=0,checkedAt=0,receivedAt=0,appliedAt=0,gameplaySince=0;std::uint64_t gameplayMap=0,gameplayEpoch=0,startAppliedMap=0,startAppliedEpoch=0;
    std::uint32_t sentDigest=0;unsigned locks=0,dials=0,texts=0;const char* result="idle";
    SafeCodePacket last{};bool lastValid=false;
    std::vector<std::array<std::int32_t,3>> detail; // entity hash, Combination, Position of this world's locks at the last service
};
SafeCodeSync safeCodeSync{};
std::uint32_t safeHash(const std::string& s){std::uint32_t h=2166136261u;for(unsigned char c:s){h^=c;h*=16777619u;}return h?h:1;}
// An int variable of a Kiscule graph by name: its value lives at +0x1c (the default at +0x18, init 0x1408e1ec0).
std::uintptr_t safeIntVariable(std::uintptr_t graph,const char* wanted){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vars=0;std::uint32_t count=0;
    if(!graph||!memoryRead(graph+0x40,&vars,8)||!vars||!memoryRead(graph+0x48,&count,4))return 0;count&=0xffffff;if(count>512)return 0;
    for(std::uint32_t v=0;v<count;++v){std::uintptr_t var=0,vt=0,name=0;std::string text;
        if(!memoryRead(vars+v*8,&var,8)||!var||!memoryRead(var,&vt,8)||vt!=base+kKisculeIntVariableVtableRva||!memoryRead(var+8,&name,8)||
           !worldIdentityString(memoryRead,name,text)||text!=wanted)continue;
        return var;}
    return 0;
}
bool safeIntRead(std::uintptr_t var,std::int32_t& value){return var&&memoryRead(var+0x1c,&value,4);}
// Every safe lock of this world: an entity whose top-level Kiscule instance holds a nested 3_digits_lock instance.
bool readSafeLocks(std::vector<SafeLockVars>& out){
    out.clear();if(!observedGame)return false;
    for(unsigned slot=0;slot<16384;++slot){std::uintptr_t actor=0,p=0;std::uint32_t handle=0;
        if(!memoryRead(observedGame+0x3130b0+slot*8,&actor,8)||!actor||!memoryRead(actor+0xa8,&handle,4)||(handle>>16)!=slot)continue;
        const auto instance=kisculeInstanceFor(handle);std::uintptr_t table=0,graph=0;std::uint32_t buckets=0;
        if(!instance||!memoryRead(instance+0x88,&table,8)||!table||!memoryRead(instance+0x90,&buckets,4))continue;
        buckets&=0xffffff;if(!buckets||buckets>4096)continue;
        for(std::uint32_t b=0;b<buckets;++b){std::uintptr_t node=0;if(!memoryRead(table+b*8,&node,8))break;
            for(unsigned hops=0;node&&hops<64;++hops){std::uintptr_t nested=0,decl=0,declName=0,lockGraph=0,next=0;std::string declText;
                if(memoryRead(node+8,&nested,8)&&nested&&memoryRead(nested+0x68,&decl,8)&&decl&&memoryRead(decl+8,&declName,8)&&
                   worldIdentityString(memoryRead,declName,declText)&&declText.size()>=21&&!declText.compare(declText.size()-21,21,"3_digits_lock.kiscule")&&
                   memoryRead(nested+0x70,&lockGraph,8)&&lockGraph){
                    std::string name;if(!memoryRead(actor+0x20,&p,8)||!worldIdentityString(memoryRead,p,name))break;
                    SafeLockVars l{};l.entity=safeHash(name);l.combination=safeIntVariable(lockGraph,"Combination");l.position=safeIntVariable(lockGraph,"Position");
                    l.digit[0]=safeIntVariable(lockGraph,"PositionDigit1");l.digit[1]=safeIntVariable(lockGraph,"PositionDigit2");l.digit[2]=safeIntVariable(lockGraph,"PositionDigit3");
                    if(memoryRead(instance+0x70,&graph,8)&&graph)l.entityCombination=safeIntVariable(graph,"Combination");
                    if(l.combination&&out.size()<kSafeLockMax)out.push_back(l);}
                if(!memoryRead(node+0x10,&next,8))break;node=next;}}}
    return true;
}
// Every safe dial of this world: a 10-state usable whose name holds combinaison_numbers.
bool readSafeDials(std::vector<std::pair<std::uint32_t,NativeUsable>>& out){
    out.clear();const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned count=0;
    if(!observedGame||!memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||!(count&0xffffff)||(count&0xffffff)>16384)return false;
    std::vector<unsigned> handles(count&0xffffff);if(!memoryRead(list,handles.data(),handles.size()*4))return false;
    for(auto h:handles){NativeUsable u{};if(!readNativeUsable(memoryRead,base,observedGame,h,u)||u.stateCount!=10||u.name.find("combinaison_numbers")==std::string::npos)continue;
        if(out.size()<kSafeDialMax)out.push_back({safeHash(u.name),u});}
    return true;
}
// This world's local player's journal: its decl's text ids (+8 -> +0x68, stride 0x10, count +0x70) and the formatted
// int lists (+0xd8: one {int* values, count} per text id, stride 0x10). Read only; the layout is checked as it goes.
struct SafeJournalList {std::uint32_t id=0;std::uintptr_t values=0;std::uint32_t count=0;};
bool readSafeJournal(std::vector<SafeJournalList>& out){
    out.clear();if(!observedGame)return false;std::uintptr_t player=0;
    if(!memoryRead(observedGame+0x3130b0,&player,8)||!player)return false;
    const auto journal=reinterpret_cast<std::uintptr_t(*)(void*,void*,int)>(gameImage+0x877720)(reinterpret_cast<void*>(player),gameImage+0x39c5ac0,2);
    std::uintptr_t decl=0,ids=0,lists=0;std::uint32_t idCount=0,listCount=0;
    if(!journal||!memoryRead(journal+8,&decl,8)||!decl||!memoryRead(decl+0x68,&ids,8)||!ids||!memoryRead(decl+0x70,&idCount,4)||
       !memoryRead(journal+0xd8,&lists,8)||!memoryRead(journal+0xe0,&listCount,4))return false;
    idCount&=0xffffff;listCount&=0xffffff;if(idCount>4096||listCount>idCount)return false;
    for(std::uint32_t i=0;i<listCount&&lists;++i){SafeJournalList l{};std::uint32_t n=0;
        if(!memoryRead(ids+i*0x10ull,&l.id,4)||!memoryRead(lists+i*0x10ull,&l.values,8)||!memoryRead(lists+i*0x10ull+8,&n,4))return false;
        l.count=n&0xffffff;if(!l.values||l.count<1||l.count>4)continue;
        bool digits=true;for(std::uint32_t k=0;k<l.count;++k){std::int32_t v=0;if(!memoryRead(l.values+k*4ull,&v,4)||v<0||v>9)digits=false;}
        if(digits&&l.count==3)out.push_back(l);}
    return true;
}
void receiveSafeCodes(const SafeCodePacket& p){
    auto& s=safeCodeSync;++s.received;
    if(!validSafeCodes(p)||localRole!=1||p.session!=remote.session||p.sequence<=s.peerSequence){++s.refused;return;}
    s.peerSequence=p.sequence;s.last=p;s.lastValid=true;s.receivedAt=GetTickCount64();
}
void serviceSafeCodes(ULONGLONG tick){
    auto& s=safeCodeSync;if(!s.enabled){s.result="off";return;}
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected){s.result="no_session";return;}
    if(outgoing.gameLogicState!=1||outgoing.pendingGameLogicState){s.gameplaySince=0;s.result="waiting_gameplay";return;}
    if(!s.gameplaySince||s.gameplayMap!=outgoing.mapId||s.gameplayEpoch!=outgoing.worldEpoch){s.gameplaySince=tick;s.gameplayMap=outgoing.mapId;s.gameplayEpoch=outgoing.worldEpoch;}
    if(tick-s.gameplaySince<3000){s.result="settling";return;}
    if(localRole==0){
        // safe-codes-v2038a: the 2 s read and the 6 s resend need their own clocks; with one, a host whose safes did not
        // change sent once, and the partner fell back to waiting_host after 15 s.
        if(tick-s.checkedAt<2000)return;s.checkedAt=tick;
        std::vector<SafeLockVars> locks;std::vector<std::pair<std::uint32_t,NativeUsable>> dials;std::vector<SafeJournalList> texts;
        if(!readSafeLocks(locks)||!readSafeDials(dials)){++s.readFailures;s.result="unreadable";return;}
        if(!readSafeJournal(texts)){++s.readFailures;texts.clear();}
        SafeCodePacket p{};p.session=outgoing.session;p.mapId=outgoing.mapId;p.epoch=outgoing.worldEpoch;
        for(const auto& l:locks){SafeLockEntry e{};e.entity=l.entity;
            if(!safeIntRead(l.combination,e.combination)||!safeIntRead(l.position,e.position)||e.combination<0||e.combination>999||e.position<0||e.position>999){++s.readFailures;continue;}
            bool ok=true;for(int k=0;k<3;++k)ok=ok&&safeIntRead(l.digit[k],e.digit[k])&&e.digit[k]>=0&&e.digit[k]<=9;if(!ok){++s.readFailures;continue;}
            p.lock[p.locks++]=e;}
        for(const auto& [name,u]:dials)if(u.current>=0&&u.current<=9&&p.dials<kSafeDialMax)p.dial[p.dials++]={name,u.current};
        for(const auto& t:texts)if(p.journal<kSafeJournalMax){SafeJournalEntry e{};e.id=t.id;e.count=t.count;for(std::uint32_t k=0;k<t.count;++k)memoryRead(t.values+k*4ull,&e.value[k],4);p.text[p.journal++]=e;}
        s.locks=p.locks;s.dials=p.dials;s.texts=p.journal;s.detail.clear();
        for(unsigned i=0;i<p.locks;++i)s.detail.push_back({static_cast<std::int32_t>(p.lock[i].entity),p.lock[i].combination,p.lock[i].position});
        if(!p.locks&&!p.dials){s.result="no_safes";return;}
        std::uint32_t digest=2166136261u;const auto* bytes=reinterpret_cast<const unsigned char*>(&p.locks);
        for(std::size_t i=0;i<sizeof(p)-offsetof(SafeCodePacket,locks);++i){if(i>=12&&i<44)continue;digest^=bytes[i];digest*=16777619u;}
        if(digest==s.sentDigest&&tick-s.sentAt<6000)return;
        p.sequence=++s.sequence;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_Reliable,nullptr)==k_EResultOK){++s.sent;s.sentDigest=digest;s.result="publishing";}
        s.sentAt=tick;return;
    }
    if(!s.lastValid||tick-s.receivedAt>15000||s.last.mapId!=outgoing.mapId){s.result="waiting_host";return;}
    if(tick-s.appliedAt<1000)return;s.appliedAt=tick;
    std::vector<SafeLockVars> locks;std::vector<std::pair<std::uint32_t,NativeUsable>> dials;
    if(!readSafeLocks(locks)||!readSafeDials(dials)){++s.readFailures;s.result="unreadable";return;}
    const bool start=s.startAppliedMap!=outgoing.mapId||s.startAppliedEpoch!=outgoing.worldEpoch;bool startDone=true;
    const auto write=[&](std::uintptr_t var,std::int32_t value,std::uint64_t& counter){std::int32_t now=0;if(var&&safeIntRead(var,now)&&now!=value){*reinterpret_cast<std::int32_t*>(var+0x1c)=value;++counter;}};
    for(unsigned i=0;i<s.last.locks;++i){const auto& e=s.last.lock[i];bool found=false;
        for(const auto& l:locks)if(l.entity==e.entity){found=true;write(l.combination,e.combination,s.codeWrites);write(l.entityCombination,e.combination,s.codeWrites);
            if(start){write(l.position,e.position,s.positionWrites);for(int k=0;k<3;++k)write(l.digit[k],e.digit[k],s.positionWrites);}break;}
        if(!found)++s.unknownLocks;}
    if(start)for(unsigned i=0;i<s.last.dials;++i){const auto& e=s.last.dial[i];bool found=false;
        for(const auto& [name,u]:dials)if(name==e.name){found=true;
            if(u.current!=e.state){if(u.transitioning()||!nativeUsableTargetValid(u,static_cast<unsigned>(e.state))){startDone=false;break;}
                reinterpret_cast<void(*)(void*,int,char,char)>(gameImage+0x77a880)(reinterpret_cast<void*>(u.component),e.state,0,1);++s.dialWrites;}
            break;}
        if(!found)++s.unknownDials;}
    std::vector<SafeJournalList> texts;
    if(readSafeJournal(texts)){for(unsigned i=0;i<s.last.journal;++i){const auto& e=s.last.text[i];bool found=false;
            for(const auto& t:texts)if(t.id==e.id&&t.count==e.count){found=true;for(std::uint32_t k=0;k<t.count;++k){std::int32_t now=0;
                if(memoryRead(t.values+k*4ull,&now,4)&&now!=e.value[k]){*reinterpret_cast<std::int32_t*>(t.values+k*4ull)=e.value[k];++s.journalWrites;}}break;}
            if(!found)++s.unknownTexts;}}
    else ++s.readFailures;
    if(start&&startDone){s.startAppliedMap=outgoing.mapId;s.startAppliedEpoch=outgoing.worldEpoch;}
    s.detail.clear();for(const auto& l:locks){std::int32_t c=-1,q=-1;safeIntRead(l.combination,c);safeIntRead(l.position,q);s.detail.push_back({static_cast<std::int32_t>(l.entity),c,q});}
    s.locks=static_cast<unsigned>(locks.size());s.dials=static_cast<unsigned>(dials.size());s.texts=static_cast<unsigned>(texts.size());s.result="applied";
}
void writeSafeCodes(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.safe-codes.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=safeCodeSync;
    fprintf(f,"{\"enabled\":%s,\"role\":%u,\"result\":\"%s\",\"locks\":%u,\"dials\":%u,\"texts\":%u,\"hostLocks\":%u,\"hostDials\":%u,\"hostTexts\":%u,\"sent\":%llu,\"received\":%llu,\"refused\":%llu,"
        "\"codeWrites\":%llu,\"positionWrites\":%llu,\"dialWrites\":%llu,\"journalWrites\":%llu,\"unknownLocks\":%llu,\"unknownDials\":%llu,\"unknownTexts\":%llu,\"readFailures\":%llu,\"locksDetail\":[",
        s.enabled?"true":"false",localRole,s.result,s.locks,s.dials,s.texts,s.lastValid?s.last.locks:0u,s.lastValid?s.last.dials:0u,s.lastValid?s.last.journal:0u,
        s.sent,s.received,s.refused,s.codeWrites,s.positionWrites,s.dialWrites,s.journalWrites,s.unknownLocks,s.unknownDials,s.unknownTexts,s.readFailures);
    // This world's locks at the last service (the host's sent values, or the partner's after its writes), for the take's checks.
    bool first=true;for(const auto& d:s.detail){fprintf(f,"%s{\"entity\":%u,\"combination\":%d,\"position\":%d}",first?"":",",static_cast<std::uint32_t>(d[0]),d[1],d[2]);first=false;}
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
