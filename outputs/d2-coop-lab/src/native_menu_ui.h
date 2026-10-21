#pragma once
// 1214hn (#156): the Co-op entry under Campaign in the native main menu, and co-op mode.
// The main menu is the AS3 Iggy movie ui/main_menu.iggy (chapter 107, confirmed static). Its Home entries are a
// com.ark.data:UIDataProvider (MainMenuData.mainMenuEntries) built in MainMenuData._initData: Campaign, Tutorial,
// Options, Credits, Quit; MainMenuData._DLCEntry is a MenuItemDefinition that is never added on PC. Choosing a
// MenuItemDefinition navigates to its screenNameLinkage with its gameData and sends its kisculeEvent to the engine
// (GameService.sendNotification("KisculeEvent", name)); Campaign uses com.bsp.main.campaign.CampaignScreen,
// GotoCampaignScreen and gameData = mainMenuData.
// Once per main menu movie the mod turns _DLCEntry into the Co-op entry (label, the Campaign screen, gameData and the
// event GotoCoopScreen) and inserts it at index 1 with the provider's own addItem(item, 1). Iggy API (confirmed
// static, chapter 107): the movie (arkIggyFile) is main menu state+0x20, its Iggy* at file+0x1608, the root value
// path at Iggy+0x2950. The engine's KisculeEvent handler FUN_1407bf050(manager, args) reads args[0] as the event name
// (IggyDataValue type 5, UTF8 pointer at +0x10, length at +0x18): the hook turns GotoCoopScreen into co-op mode and
// passes GotoCampaignScreen on; GotoCampaignScreen turns co-op mode off. Engine thread only, outside the Iggy tick.
struct IggyPath28 {void* f=nullptr;void* parent=nullptr;std::uint64_t name=0;void* ref=nullptr;std::int32_t index=-1;std::int32_t type=0;};
static_assert(sizeof(IggyPath28)==0x28);
struct IggyData20 {std::int32_t type=0;std::int32_t pad=0;std::uint64_t atom=0;std::uint64_t payload=0;std::uint64_t length=0;};
static_assert(sizeof(IggyData20)==0x20);
using IggyMakeNameRef=int(*)(IggyPath28*,IggyPath28*,const char*);
using IggyCreateFastName=std::uint64_t(*)(void*,const char*,std::int32_t);
using IggyGetS32=int(*)(IggyPath28*,std::uint64_t,const char*,std::int32_t*);
using IggyGetString=int(*)(IggyPath28*,std::uint64_t,const char*,std::int32_t,char*,std::int32_t*);
using IggySetString=int(*)(IggyPath28*,std::uint64_t,const char*,const char*,std::int32_t);
using IggySetValueRef=int(*)(IggyPath28*,std::uint64_t,const char*,void*);
using IggyValueRefFromPath=void*(*)(IggyPath28*,int);
using IggyValueRefFree=void(*)(void*,void*);
using IggyCallMethod=int(*)(void*,IggyData20*,IggyPath28*,std::uint64_t,std::int32_t,IggyData20*);
struct CoopMenuState {
    const char* result="idle";std::uint64_t attempts=0,inserts=0,failures=0,coopSelected=0,campaignSelected=0;
    int lastCode=0,entries=0;std::uintptr_t file=0;ULONGLONG lastTry=0;char failedStep[32]{};
};
CoopMenuState coopMenu{};bool coopMenuEnabled=true;std::atomic<bool> coopModeSelected{false};
constexpr char kCoopMenuEvent[]="GotoCoopScreen";constexpr char kCampaignMenuEvent[]="GotoCampaignScreen";
inline bool coopMenuStep(const char* step,bool ok,int code=0){if(!ok){coopMenu.result="step_failed";coopMenu.lastCode=code;
    std::snprintf(coopMenu.failedStep,sizeof(coopMenu.failedStep),"%s",step);++coopMenu.failures;}return ok;}
void serviceCoopMenu(ULONGLONG tick){
    if(!coopMenuEnabled)return;const auto state=sampledMainMenuState;
    if(!state){coopMenu.result="no_menu";return;}
    if(tick-coopMenu.lastTry<300)return;coopMenu.lastTry=tick;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t file=0,iggy=0,home=0,homeVtable=0;unsigned char bound=0;
    if(!memoryRead(state+0x20,&file,8)||!file||!memoryRead(file+0x1608,&iggy,8)||!iggy){coopMenu.result="no_movie";return;}
    // Only once the Home screen is bound (the movie's MainMenuData exists then; 1214ha, chapter 106).
    if(!memoryRead(state+0x98,&home,8)||!home||!memoryRead(home,&homeVtable,8)||homeVtable!=base+0x1b6e698||!memoryRead(home+0x38,&bound,1)||bound!=1){coopMenu.result="home_not_bound";return;}
    const auto at=[&](std::uintptr_t rva){return reinterpret_cast<void*>(base+rva);};
    const auto makeRef=reinterpret_cast<IggyMakeNameRef>(at(0x1009de0));const auto getS32=reinterpret_cast<IggyGetS32>(at(0x1009bf0));
    const auto getString=reinterpret_cast<IggyGetString>(at(0x1009c70));const auto setString=reinterpret_cast<IggySetString>(at(0x100a460));
    const auto setRef=reinterpret_cast<IggySetValueRef>(at(0x100a5c0));const auto refFromPath=reinterpret_cast<IggyValueRefFromPath>(at(0x100a210));
    const auto refFree=reinterpret_cast<IggyValueRefFree>(at(0x100a170));const auto fastName=reinterpret_cast<IggyCreateFastName>(at(0x1009960));
    const auto callMethod=reinterpret_cast<IggyCallMethod>(at(0x100c4c0));
    auto* root=reinterpret_cast<IggyPath28*>(iggy+0x2950);IggyPath28 data{},spare{},entries{};
    if(!coopMenuStep("path_data",makeRef(&data,root,"mainMenuData")!=0)||!coopMenuStep("path_spare",makeRef(&spare,&data,"_DLCEntry")!=0)||
       !coopMenuStep("path_entries",makeRef(&entries,&data,"mainMenuEntries")!=0))return;
    char event[64]{};std::int32_t length=0;
    const int read=getString(&spare,0,"kisculeEvent",static_cast<std::int32_t>(sizeof(event)-1),event,&length);
    std::int32_t count=0;getS32(&entries,0,"length",&count);coopMenu.entries=count;
    if(!read&&length==static_cast<std::int32_t>(sizeof(kCoopMenuEvent)-1)&&!std::memcmp(event,kCoopMenuEvent,length)){coopMenu.result="inserted";coopMenu.file=file;return;}
    ++coopMenu.attempts;
    static const char kLabel[]="CO-OP";static const char kScreen[]="com.bsp.main.campaign.CampaignScreen";
    if(!coopMenuStep("set_label",setString(&spare,0,"label",kLabel,static_cast<std::int32_t>(sizeof(kLabel)-1))!=0)||
       !coopMenuStep("set_screen",setString(&spare,0,"screenNameLinkage",kScreen,static_cast<std::int32_t>(sizeof(kScreen)-1))!=0))return;
    void* gameData=refFromPath(&data,0);if(!coopMenuStep("ref_data",gameData!=nullptr))return;
    const bool dataSet=setRef(&spare,0,"gameData",gameData)!=0;refFree(reinterpret_cast<void*>(iggy),gameData);
    if(!coopMenuStep("set_data",dataSet))return;
    IggyData20 args[3]{};args[0].type=8;args[0].payload=reinterpret_cast<std::uint64_t>(&spare);
    args[1].type=4;const double index=1;std::memcpy(&args[1].payload,&index,8);args[2].type=3;args[2].payload=1;
    IggyData20 result{};const int called=callMethod(reinterpret_cast<void*>(iggy),&result,&entries,fastName(reinterpret_cast<void*>(iggy),"addItem",-1),3,args);
    if(!coopMenuStep("add_item",called==0,called))return;
    // The marker last: a failed insert retries on the next pass.
    if(!coopMenuStep("set_event",setString(&spare,0,"kisculeEvent",kCoopMenuEvent,static_cast<std::int32_t>(sizeof(kCoopMenuEvent)-1))!=0))return;
    getS32(&entries,0,"length",&count);coopMenu.entries=count;++coopMenu.inserts;coopMenu.file=file;coopMenu.result="inserted";
}
// The engine's KisculeEvent handler: args is an idList<IggyDataValue> (data at +0).
std::atomic<std::uint64_t> coopKisculeHandlerCalls{0}; // 1214ho: calls of the hooked handler
using NativeKisculeEvent=void*(*)(void*,void*);NativeKisculeEvent realKisculeEvent=nullptr;bool coopKisculeHooked=false;
inline void* hookKisculeEvent(void* manager,void* args){
    ++coopKisculeHandlerCalls; // 1214hp: counts only; the hub hook sets co-op mode
    return realKisculeEvent(manager,args);
}
inline bool installCoopKisculeHook(){
    static const unsigned char prologue[]={0x48,0x8b,0xc4,0x48,0x89,0x48,0x08,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57};
    coopKisculeHooked=installNpcRemovalHook(0x7bf050,prologue,sizeof(prologue),reinterpret_cast<void*>(&hookKisculeEvent),realKisculeEvent);
    return coopKisculeHooked;
}
// 1214ho: in coop-menu-v1757a (1214hn) the entry was inserted and opened the Campaign screen, but the KisculeEvent
// handler hook never saw GotoCoopScreen or GotoCampaignScreen (no per-movie registration was ruled out). Every AS3
// ExternalInterface.call passes through the Iggy hub FUN_140034800(arkIggyMain*, Iggy*, call*), call = {UTF8 name
// (char*, length), argc@0x10, IggyDataValue args[]@0x18} (confirmed static; inside the Iggy tick, so plain memory
// only). Its hook records the last 16 calls and turns KisculeEvent(GotoCoopScreen) into co-op mode plus
// KisculeEvent(GotoCampaignScreen), in UTF8 (type 5) or UTF16 (type 6); GotoCampaignScreen turns co-op mode off.
struct CoopHubCall {char name[32]{};int argc=0,type=0;char arg[32]{};std::uint64_t rawLength=0;};
std::array<CoopHubCall,16> coopHubCalls{};std::atomic<std::uint64_t> coopHubCount{0};
using NativeIggyHub=std::uint64_t(*)(void*,void*,void*);NativeIggyHub realIggyHub=nullptr;bool coopHubHooked=false;
static const char16_t kCampaignMenuEvent16[]=u"GotoCampaignScreen";
inline std::uint64_t hookIggyHub(void* main,void* iggy,void* call){
    const auto c=reinterpret_cast<std::uintptr_t>(call);std::uintptr_t namePtr=0;std::int32_t argc=0;
    if(c&&memoryRead(c,&namePtr,8)&&namePtr&&memoryRead(c+0x10,&argc,4)){
        auto& r=coopHubCalls[coopHubCount.fetch_add(1)%16];r=CoopHubCall{};memoryRead(namePtr,r.name,sizeof(r.name)-1);r.argc=argc;
        if(argc>=1){auto* a=reinterpret_cast<IggyData20*>(c+0x18);r.type=a->type;
            char text[32]{};int n=0;
            r.rawLength=a->length;
            if(a->type==5&&a->payload){memoryRead(a->payload,text,31);n=static_cast<int>(strnlen(text,31));} // 1214hp
            else if(a->type==6&&a->payload){for(;n<31;++n){char16_t ch=0;if(!memoryRead(a->payload+n*2,&ch,2)||!ch)break;text[n]=ch<128?static_cast<char>(ch):'?';}}
            std::memcpy(r.arg,text,sizeof(r.arg));
            if(!std::strcmp(r.name,"KisculeEvent")){
                if(n==static_cast<int>(sizeof(kCoopMenuEvent)-1)&&!std::memcmp(text,kCoopMenuEvent,n)){coopModeSelected=true;++coopMenu.coopSelected;
                    // The length follows the text only where it was the text length (with or without the NUL).
                    // 1214hq: the length is the low 32 bits (coop-menu-v1759a: raw 0xC6_0000000E for 14 characters).
                    const auto old=static_cast<std::uint32_t>(a->length);const std::uint32_t delta=sizeof(kCampaignMenuEvent)-sizeof(kCoopMenuEvent);
                    a->payload=a->type==5?reinterpret_cast<std::uint64_t>(kCampaignMenuEvent):reinterpret_cast<std::uint64_t>(kCampaignMenuEvent16);
                    if(old==sizeof(kCoopMenuEvent)-1||old==sizeof(kCoopMenuEvent))a->length=(a->length&0xffffffff00000000ull)|(old+delta);}
                else if(n==static_cast<int>(sizeof(kCampaignMenuEvent)-1)&&!std::memcmp(text,kCampaignMenuEvent,n)){coopModeSelected=false;++coopMenu.campaignSelected;}}}}
    return realIggyHub(main,iggy,call);
}
inline bool installCoopHubHook(){
    static const unsigned char prologue[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x6c,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57};
    coopHubHooked=installNpcRemovalHook(0x34800,prologue,sizeof(prologue),reinterpret_cast<void*>(&hookIggyHub),realIggyHub);
    return coopHubHooked;
}
// 1214hq (#156): co-op saves. While co-op mode is on, the game's save root (the pointer at 0x142286f40, normally
// "savegame"; read on every save, load, list and delete; chapter 107, confirmed static) is the sibling folder
// savegame_coop, so the native Campaign, Load and New Game screens, autosaves and checkpoints only see co-op saves (a
// subfolder would show as a slot: the listing is recursive). A switch runs on the engine thread on the main menu when
// no save operation is pending: swap the pointer, re-list (session vtable +0xa0, FUN_1400dae40), wait for the ready
// flag (sgm+0x3920, sgm = session+0x20), rebuild the save manager lists (FUN_140660210(manager, -1)), then the
// Campaign screen data (the Campaign section, vtable 1b6d2e8, slot 5 FUN_1404fb3b0(section, movie)). The profile
// (profile.bin) still follows the root in this candidate (lab test; its keys are decided separately).
struct CoopSaveSwitch {int phase=0;bool target=false,active=false;ULONGLONG startedAt=0;const char* result="idle";
    std::uint64_t switches=0,failures=0,rebuilds=0;char folder[MAX_PATH]{};const char* original=nullptr;};
CoopSaveSwitch coopSaves{};bool coopSavesEnabled=true;
static const char kCoopSaveRoot[]="savegame_coop";
inline void coopSaveFolder(char* out,std::size_t size){
    out[0]=0;if(productMode){char profile[MAX_PATH]{};if(GetEnvironmentVariableA("USERPROFILE",profile,sizeof(profile)))
        std::snprintf(out,size,"%s\\Saved Games\\Arkane Studios\\Dishonored2\\base\\%s",profile,kCoopSaveRoot);return;}
    // Lab: the instance folder is the game's fs_savepath (the status file sits in it).
    char dir[MAX_PATH]{};std::snprintf(dir,sizeof(dir),"%s",statusPath);char* slash=std::strrchr(dir,'\\');if(!slash)slash=std::strrchr(dir,'/');
    if(slash){*slash=0;std::snprintf(out,size,"%s\\base\\%s",dir,kCoopSaveRoot);}}
void serviceCoopSaves(ULONGLONG tick){
    if(!coopSavesEnabled)return;auto& s=coopSaves;const bool want=coopModeSelected.load();
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto** root=reinterpret_cast<const char**>(base+0x2286f40);
    if(!s.original){const char* current=nullptr;if(!memoryRead(base+0x2286f40,&current,8)||!current){s.result="no_root";return;}s.original=current;}
    if(s.phase==0){if(want==s.active)return;if(!sampledMainMenuState){s.result="waiting_main_menu";return;}s.target=want;s.phase=1;s.startedAt=tick;}
    std::uintptr_t session=0,sgm=0;if(!memoryRead(base+0x2287c78,&session,8)||!session||!memoryRead(session+0x20,&sgm,8)||!sgm){s.result="no_session";return;}
    if(s.phase==1){std::int32_t busy=0;if(!memoryRead(sgm,&busy,4)||busy){s.result="save_busy";if(tick-s.startedAt>15000){s.phase=0;++s.failures;s.result="busy_timeout";}return;}
        if(s.target){coopSaveFolder(s.folder,sizeof(s.folder));if(s.folder[0])CreateDirectoryA(s.folder,nullptr);}
        *root=s.target?kCoopSaveRoot:s.original;
        std::uintptr_t vt=0,fn=0;if(!memoryRead(session,&vt,8)||!vt||!memoryRead(vt+0xa0,&fn,8)||!fn){s.result="no_enumerate";s.phase=0;++s.failures;return;}
        reinterpret_cast<int(*)(void*,void*,void*,void*)>(fn)(reinterpret_cast<void*>(session),nullptr,nullptr,nullptr);
        s.phase=2;s.startedAt=tick;s.result="listing";return;}
    if(s.phase==2){unsigned char ready=0;memoryRead(sgm+0x3920,&ready,1);
        if(!ready){if(tick-s.startedAt>10000){s.result="list_timeout";s.phase=0;++s.failures;s.active=s.target;}return;}
        std::uintptr_t game=0;if(memoryRead(base+0x39a5290,&game,8)&&game)reinterpret_cast<void(*)(void*,int)>(gameImage+0x660210)(reinterpret_cast<void*>(game+0x2e75d8),-1);
        // The Campaign screen data, when the main menu is up.
        if(const auto state=sampledMainMenuState){std::uintptr_t file=0;memoryRead(state+0x20,&file,8);
            for(std::uintptr_t off=0x98;off<0x100&&file;off+=8){std::uintptr_t section=0,svt=0;
                if(memoryRead(state+off,&section,8)&&section&&memoryRead(section,&svt,8)&&svt==base+0x1b6d2e8){
                    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x4fb3b0)(reinterpret_cast<void*>(section),reinterpret_cast<void*>(file));++s.rebuilds;break;}}}
        s.active=s.target;++s.switches;s.phase=0;s.result=s.active?"coop":"campaign";}
}
void writeCoopMenu(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.coop-menu.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"enabled\":%s,\"hooked\":%s,\"menu\":%s,\"result\":\"%s\",\"failedStep\":\"%s\",\"lastCode\":%d,\"entries\":%d,\"attempts\":%llu,\"inserts\":%llu,\"failures\":%llu,"
        "\"coopSelected\":%llu,\"campaignSelected\":%llu,\"coopMode\":%s}\n",coopMenuEnabled?"true":"false",coopKisculeHooked?"true":"false",sampledMainMenuState?"true":"false",
        coopMenu.result,coopMenu.failedStep,coopMenu.lastCode,coopMenu.entries,static_cast<unsigned long long>(coopMenu.attempts),static_cast<unsigned long long>(coopMenu.inserts),
        static_cast<unsigned long long>(coopMenu.failures),static_cast<unsigned long long>(coopMenu.coopSelected),static_cast<unsigned long long>(coopMenu.campaignSelected),
        coopModeSelected.load()?"true":"false");
    fseek(f,-2,SEEK_CUR);
    fprintf(f,",\"hubHooked\":%s,\"hubCalls\":%llu,\"kisculeHandlerCalls\":%llu,\"recentCalls\":[",coopHubHooked?"true":"false",
        static_cast<unsigned long long>(coopHubCount.load()),static_cast<unsigned long long>(coopKisculeHandlerCalls.load()));
    {const auto n=coopHubCount.load();bool first=true;for(std::uint64_t i=n>16?n-16:0;i<n;++i){const auto& r=coopHubCalls[i%16];char name[32]{},arg[32]{};
        for(int k=0;k<31&&r.name[k];++k)name[k]=(r.name[k]=='"'||r.name[k]=='\\'||r.name[k]<32)?'_':r.name[k];
        for(int k=0;k<31&&r.arg[k];++k)arg[k]=(r.arg[k]=='"'||r.arg[k]=='\\'||r.arg[k]<32)?'_':r.arg[k];
        fprintf(f,"%s{\"name\":\"%s\",\"argc\":%d,\"type\":%d,\"arg\":\"%s\",\"rawLength\":%llu}",first?"":",",name,r.argc,r.type,arg,static_cast<unsigned long long>(r.rawLength));first=false;}}
    fprintf(f,"],\"saves\":{\"result\":\"%s\",\"active\":%s,\"phase\":%d,\"switches\":%llu,\"failures\":%llu,\"rebuilds\":%llu}",coopSaves.result,
        coopSaves.active?"true":"false",coopSaves.phase,static_cast<unsigned long long>(coopSaves.switches),static_cast<unsigned long long>(coopSaves.failures),
        static_cast<unsigned long long>(coopSaves.rebuilds));
    fputs("}\n",f);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
