#pragma once
// 2096 (#265): the partner keeps her progression between joins, per character. A join loads the host's mission fresh
// (campaign <mapinfo> <chaos> <character>), so arkCpntInventory's Start gives the mission's default loadout and nothing
// she earned survives a rejoin or a character switch. The game itself carries the player from mission to mission through
// a checkpoint save; arkCpntInventory's save state (savegameState_t, 0x18 bytes: +0 money from +0x16c, +4 the int at +0x170,
// +8 an idList of 0x20-byte records: +0 the declaration name's x31 hash (0x1401431a0), +4 quantity, +8 maximum, +0xc/+0x10
// entry +0x2c/+0x30, +0x14 entry +0x34, +0x15 entry +0x28, +0x16 equipped, +0x1c entry +0x35) is applied by 0x1406182f0 onto
// the existing entries (every declaration is an entry, quantity 0 when not held), with the native change notifications
// (d2-ghidra, confirmed static; stage8-step1-notes/partner-progression-design.md).
// The partner's game keeps her carried entries (the types below) and money in coop-progress-<character>.txt beside the
// co-op saves, captured every 3 s in gameplay once this load's restore has run. After each load the file is applied with
// the native apply, never lowering what she has (each value is the larger of the two): powers, upgrades and enhancements
// after every load; runes, coins, ammunition and the rest only after a join's campaign load (a checkpoint rewind must not
// give back what the world gives again). Lab switch coop_partner_progress on|off.
bool partnerProgressEnabled=true;
struct PartnerProgressEntry {std::string name;std::int32_t quantity=0,maximum=0,f2c=0,f30=0;std::uint8_t f28=0,f34=0,f35=0;};
struct PartnerProgress {
    std::uint64_t restoreEpoch=0,capturedEpoch=0,captures=0,writes=0,restores=0,restoredEntries=0,raisedEntries=0,failures=0;
    int character=0;bool restoredThisLoad=false;ULONGLONG gameplaySince=0,capturedAt=0;
    std::int32_t money=0,lastMoney=0;std::size_t lastCount=0;const char* result="idle";std::string lastText;};
PartnerProgress partnerProgress{};
// Carried: 2 weapons, 4 ammunition, 7 runes, 8 the Outsider rune, 9 powers, 14 elixirs, 16 gear, 18 and 21 enhancements,
// 20 power upgrades, 22 the rewire tool, 26 whalebone; and equipment upgrades (their own declaration class). Not carried:
// the mission's items, notes, keys, maps and audiographs, food, bonecharm templates.
inline int partnerProgressClass(std::uint32_t type,bool upgrade){
    if(upgrade)return 1;
    switch(type){case 9:case 18:case 20:case 21:return 1; // kept after every load
        case 2:case 4:case 7:case 8:case 14:case 16:case 22:case 26:return 2; // kept after a join's load only
        default:return 0;}}
inline std::uint32_t partnerProgressHash(const std::string& s){std::uint32_t h=0;for(const unsigned char c:s)h=h*31u+c;return h;}
inline void partnerProgressPath(int character,char* out,std::size_t size){
    char folder[MAX_PATH]{};coopSaveFolder(folder,sizeof(folder));
    std::snprintf(out,size,"%s\\coop-progress-%s.txt",folder[0]?folder:".",character==1?"corvo":"emily");}
struct PartnerInventoryEntry {std::uintptr_t entry=0;std::string name;int cls=0;std::int32_t quantity=0,maximum=0,f2c=0,f30=0;std::uint8_t f28=0,f34=0,f35=0;};
inline bool readPartnerInventory(std::uintptr_t& inventory,std::int32_t& money,std::vector<PartnerInventoryEntry>& out){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativePlayerSword p{};
    if(!readNativePlayerSword(memoryRead,base,p)||!p.sword.inventory||!p.sword.items||!p.sword.itemCount||p.sword.itemCount>4096)return false;
    inventory=p.sword.inventory;if(!memoryRead(inventory+0x16c,&money,4))return false;out.clear();
    for(unsigned i=0;i<p.sword.itemCount;++i){const auto e=p.sword.items+static_cast<std::uintptr_t>(i)*0x38;
        std::uintptr_t decl=0,vt=0,namePtr=0;std::uint32_t type=0xffffffffu;
        if(!memoryRead(e,&decl,8)||!decl||!memoryRead(decl,&vt,8)||!memoryRead(decl+8,&namePtr,8)||!namePtr)return false;
        const bool upgrade=vt==base+0x1b555a8;if(!upgrade&&(vt!=base+0x1b9ea48||!memoryRead(decl+0x60,&type,4)))return false;
        PartnerInventoryEntry x{};x.entry=e;x.cls=partnerProgressClass(type,upgrade);if(!x.cls)continue;
        char name[192]{};if(!memoryRead(namePtr,name,sizeof(name)-1))return false;name[sizeof(name)-1]=0;x.name=name;
        if(!memoryRead(e+0x20,&x.quantity,4)||!memoryRead(e+0x24,&x.maximum,4)||!memoryRead(e+0x28,&x.f28,1)||!memoryRead(e+0x2c,&x.f2c,4)||
           !memoryRead(e+0x30,&x.f30,4)||!memoryRead(e+0x34,&x.f34,1)||!memoryRead(e+0x35,&x.f35,1))return false;
        out.push_back(std::move(x));}
    return true;}
inline bool loadPartnerProgress(int character,std::int32_t& money,std::vector<PartnerProgressEntry>& out){
    char path[MAX_PATH]{};partnerProgressPath(character,path,sizeof(path));FILE* f=nullptr;if(fopen_s(&f,path,"rb")||!f)return false;
    out.clear();money=0;char line[400]{};bool header=false;
    while(fgets(line,sizeof(line),f)){char name[200]{};PartnerProgressEntry e{};unsigned f28=0,f34=0,f35=0;
        if(std::sscanf(line,"money %d",&money)==1){header=true;continue;}
        if(std::sscanf(line,"%199s %d %d %u %d %d %u %u",name,&e.quantity,&e.maximum,&f28,&e.f2c,&e.f30,&f34,&f35)==8){
            e.name=name;e.f28=static_cast<std::uint8_t>(f28);e.f34=static_cast<std::uint8_t>(f34);e.f35=static_cast<std::uint8_t>(f35);out.push_back(std::move(e));}}
    fclose(f);return header;}
inline bool writePartnerProgress(int character,std::int32_t money,const std::vector<PartnerInventoryEntry>& held){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};partnerProgressPath(character,path,sizeof(path));std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    std::string text="money "+std::to_string(money)+"\n";
    for(const auto& x:held){if(!x.quantity&&!x.maximum)continue;char line[400]{};
        std::snprintf(line,sizeof(line),"%s %d %d %u %d %d %u %u\n",x.name.c_str(),x.quantity,x.maximum,x.f28,x.f2c,x.f30,x.f34,x.f35);text+=line;}
    if(text==partnerProgress.lastText)return true;
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return false;const bool ok=fwrite(text.data(),1,text.size(),f)==text.size();
    if(fclose(f)||!ok||!MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING))return false;partnerProgress.lastText=text;++partnerProgress.writes;return true;}
struct PartnerSavedItemState {std::uint32_t hash=0;std::int32_t quantity=0,maximum=0,f0c=0,f10=0;std::uint8_t f14=0,f15=0,equipped=0,pad=0;std::uint32_t f18=0;std::uint8_t f1c=0,pad2[3]{};};
static_assert(sizeof(PartnerSavedItemState)==0x20,"itemState_t is 0x20 bytes");
struct PartnerSavedState {std::int32_t money=0,v170=0;PartnerSavedItemState* items=nullptr;std::uint32_t count=0,capacity=0;};
static_assert(sizeof(PartnerSavedState)==0x18,"savegameState_t is 0x18 bytes");
// Applies the saved entries the current load may take (every load: class 1; a join's load: both classes and money), each
// raised to the larger of the saved and the held value, through the native apply. Returns the number of raised entries.
inline int restorePartnerProgress(bool joinLoad){
    auto& g=partnerProgress;std::int32_t savedMoney=0;std::vector<PartnerProgressEntry> saved;
    if(!loadPartnerProgress(g.character,savedMoney,saved)){g.result="nothing_saved";return 0;}
    std::uintptr_t inventory=0;std::int32_t money=0;std::vector<PartnerInventoryEntry> held;
    if(!readPartnerInventory(inventory,money,held)){++g.failures;g.result="inventory_unreadable";return -1;}
    std::int32_t v170=0;if(!memoryRead(inventory+0x170,&v170,4)){++g.failures;g.result="inventory_unreadable";return -1;}
    std::unordered_map<std::string,const PartnerProgressEntry*> byName;for(const auto& s:saved)byName[s.name]=&s;
    std::vector<PartnerSavedItemState> records;
    for(const auto& x:held){const auto it=byName.find(x.name);if(it==byName.end())continue;if(x.cls==2&&!joinLoad)continue;
        const auto& s=*it->second;if(s.quantity<=x.quantity&&s.maximum<=x.maximum)continue;
        PartnerSavedItemState r{};r.hash=partnerProgressHash(x.name);r.quantity=(std::max)(s.quantity,x.quantity);r.maximum=(std::max)(s.maximum,x.maximum);
        r.f0c=s.f2c;r.f10=s.f30;r.f14=s.f34;r.f15=s.f28;r.f1c=s.f35;records.push_back(r);}
    const std::int32_t newMoney=joinLoad?(std::max)(money,savedMoney):money;
    if(records.empty()&&newMoney==money){g.result="nothing_to_raise";return 0;}
    PartnerSavedState state{};state.money=newMoney;state.v170=v170;state.items=records.data();
    state.count=static_cast<std::uint32_t>(records.size());state.capacity=static_cast<std::uint32_t>(records.size())<<8;
    reinterpret_cast<void(*)(std::uintptr_t,void*)>(gameImage+0x6182f0)(inventory,&state);
    g.raisedEntries+=records.size();g.result=joinLoad?"restored_join":"restored_load";return static_cast<int>(records.size());}
// The engine-thread service: partner only, in a session, in gameplay.
void servicePartnerProgress(ULONGLONG tick){
    auto& g=partnerProgress;
    if(localRole!=1||!partnerProgressEnabled||!coopInSession()||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    if(outgoing.gameLogicState!=1||outgoing.pendingGameLogicState){g.gameplaySince=0;return;}
    if(!g.gameplaySince){g.gameplaySince=tick;return;}
    const int character=localCharacter();if(character!=1&&character!=2)return;
    if(g.restoreEpoch!=outgoing.worldEpoch||g.character!=character){
        // A new load (or a character switch): wait 2 s of gameplay for the loadout, then raise what was saved.
        if(tick-g.gameplaySince<2000)return;
        g.character=character;const bool joinLoad=partnerProgressJoinPending&&partnerProgressJoinCharacter==character;
        const int raised=restorePartnerProgress(joinLoad);if(raised<0)return;
        if(joinLoad)partnerProgressJoinPending=false;
        g.restoreEpoch=outgoing.worldEpoch;++g.restores;g.restoredEntries=static_cast<std::uint64_t>(raised);g.restoredThisLoad=true;g.capturedAt=tick;return;}
    if(tick-g.capturedAt<3000)return;g.capturedAt=tick;
    std::uintptr_t inventory=0;std::vector<PartnerInventoryEntry> held;
    if(!readPartnerInventory(inventory,g.money,held)){++g.failures;g.result="capture_unreadable";return;}
    ++g.captures;g.lastCount=held.size();g.capturedEpoch=outgoing.worldEpoch;
    if(!writePartnerProgress(character,g.money,held)){++g.failures;g.result="write_failed";return;}
    if(std::strncmp(g.result,"restored",8))g.result="captured";}
inline void writePartnerProgressStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.partner-progress.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& g=partnerProgress;
    fprintf(f,"{\"enabled\":%s,\"role\":%d,\"character\":%d,\"result\":\"%s\",\"restores\":%llu,\"restoredEntries\":%llu,\"raisedEntries\":%llu,\"captures\":%llu,\"writes\":%llu,"
        "\"failures\":%llu,\"carried\":%zu,\"money\":%d,\"joinLoadPending\":%s,\"restoreEpoch\":%llu,\"epoch\":%llu}\n",
        partnerProgressEnabled?"true":"false",localRole,g.character,g.result,static_cast<unsigned long long>(g.restores),static_cast<unsigned long long>(g.restoredEntries),
        static_cast<unsigned long long>(g.raisedEntries),static_cast<unsigned long long>(g.captures),static_cast<unsigned long long>(g.writes),
        static_cast<unsigned long long>(g.failures),g.lastCount,g.money,partnerProgressJoinPending?"true":"false",
        static_cast<unsigned long long>(g.restoreEpoch),static_cast<unsigned long long>(outgoing.worldEpoch));
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);}
