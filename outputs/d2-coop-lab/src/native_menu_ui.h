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
    const char* result="idle";std::uint64_t attempts=0,inserts=0,failures=0,coopSelected=0,campaignSelected=0,sessionRequests=0;
    int lastCode=0,entries=0;std::uintptr_t file=0;ULONGLONG lastTry=0;char failedStep[32]{};
};
CoopMenuState coopMenu{};bool coopMenuEnabled=true;std::atomic<bool> coopModeSelected{false};
// 1214ht (#156): coop-saveflow-v1768a: back on the main menu after a co-op game, the second CO-OP visit listed no co-op
// saves (LOAD locked) although the game had written SLOT21 and SLOT03 to savegame_coop: without a mode change no switch
// ran, and the screen came from a stale list. Every visit to the Co-op or Campaign screen now re-lists the saves.
std::atomic<bool> coopSavesRefresh{false};
inline bool coopInSession(){return onlineSession.active;} // 1214ht (#157)
std::atomic<bool> coopLeaveRequested{false}; // 1214ht (#157): set by the hub, served by serviceCoopPause
// 1214hv (#157): the pause menu or its quit dialog is open (logic 2, or 3 entered from 2); the Leave co-op text and
// the BackToMain rename apply only there, so a GameOver screen (logic 13) keeps its native Back to main menu.
std::atomic<bool> coopPauseContext{false};
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
                if(n==static_cast<int>(sizeof(kCoopMenuEvent)-1)&&!std::memcmp(text,kCoopMenuEvent,n)){coopModeSelected=true;coopSavesRefresh=true;++coopMenu.coopSelected;
                    // The length follows the text only where it was the text length (with or without the NUL).
                    // 1214hq: the length is the low 32 bits (coop-menu-v1759a: raw 0xC6_0000000E for 14 characters).
                    const auto old=static_cast<std::uint32_t>(a->length);const std::uint32_t delta=sizeof(kCampaignMenuEvent)-sizeof(kCoopMenuEvent);
                    a->payload=a->type==5?reinterpret_cast<std::uint64_t>(kCampaignMenuEvent):reinterpret_cast<std::uint64_t>(kCampaignMenuEvent16);
                    if(old==sizeof(kCoopMenuEvent)-1||old==sizeof(kCoopMenuEvent))a->length=(a->length&0xffffffff00000000ull)|(old+delta);}
                else if(n==static_cast<int>(sizeof(kCampaignMenuEvent)-1)&&!std::memcmp(text,kCampaignMenuEvent,n)){coopModeSelected=false;coopSavesRefresh=true;++coopMenu.campaignSelected;}}}}
    // 1214ht (#157): Leave co-op (see serviceCoopPause).
    if(c&&namePtr&&coopInSession()&&coopPauseContext.load()){char name[16]{};memoryRead(namePtr,name,sizeof(name)-1);
        if(!std::strcmp(name,"BackToMain")){static const char kResume[]="ResumeGame";
            *reinterpret_cast<const char**>(c)=kResume; // the same length (10) as BackToMain
            coopLeaveRequested=true;}}
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
    std::uint64_t switches=0,failures=0,rebuilds=0,profileLoads=0,profileSeeds=0,refreshes=0,profileSaves=0,profileSkips=0;std::uint32_t listed=0;int lastDirty=-1;
    char folder[MAX_PATH]{};const char* original=nullptr;};
CoopSaveSwitch coopSaves{};bool coopSavesEnabled=true;
static const char kCoopSaveRoot[]="savegame_coop";
inline void coopSaveFolder(char* out,std::size_t size){
    out[0]=0;if(productMode){char profile[MAX_PATH]{};if(GetEnvironmentVariableA("USERPROFILE",profile,sizeof(profile)))
        std::snprintf(out,size,"%s\\Saved Games\\Arkane Studios\\Dishonored2\\base\\%s",profile,kCoopSaveRoot);return;}
    // Lab: the instance folder is the game's fs_savepath (the status file sits in it).
    char dir[MAX_PATH]{};std::snprintf(dir,sizeof(dir),"%s",statusPath);char* slash=std::strrchr(dir,'\\');if(!slash)slash=std::strrchr(dir,'/');
    if(slash){*slash=0;std::snprintf(out,size,"%s\\base\\%s",dir,kCoopSaveRoot);}}
// 1214hr (#156): co-op has its own profile.bin in savegame_coop (chapter 107: the profile follows the root, so the root
// swap alone would have let a co-op New Game reset the player's campaign keys). A switch now: waits until the save
// manager and both profile processors are idle; saves the profile under the current root (SaveProfile FUN_1400d3bd0,
// asynchronous) and waits for it; swaps the root; resets the in-memory profile (FUN_14005a8c0; LoadProfile merges into
// memory) and, for a first co-op profile, keeps only HasSetGamma; loads the target profile (LoadProfile FUN_1400d37b0,
// synchronous, result at pm+0x20; a missing file is written from memory); on a refused load it rolls back to the old
// root and profile. Then the saves are re-listed, the campaign index and the lifetime stats refreshed (FUN_140660210
// with GetCampaignIndex FUN_140657380; FUN_14067d3f0(game+0x1f77c8)) and the Campaign screen rebuilt.
struct CoopProfileHandles {std::uintptr_t session=0,sgm=0,pm=0,profile=0;};
inline bool coopProfileHandles(std::uintptr_t base,CoopProfileHandles& h){
    if(!memoryRead(base+0x2287c78,&h.session,8)||!h.session||!memoryRead(h.session+0x20,&h.sgm,8)||!h.sgm)return false;
    std::uintptr_t vt=0,fn=0;if(!memoryRead(h.session,&vt,8)||!vt||!memoryRead(vt+0x38,&fn,8)||!fn)return false;
    const auto signin=reinterpret_cast<std::uintptr_t(*)(void*)>(fn)(reinterpret_cast<void*>(h.session));if(!signin)return false;
    std::uintptr_t svt=0,count=0,user=0;if(!memoryRead(signin,&svt,8)||!svt||!memoryRead(svt+0x18,&count,8)||!memoryRead(svt+0x28,&user,8)||!count||!user)return false;
    if(reinterpret_cast<int(*)(void*)>(count)(reinterpret_cast<void*>(signin))<=0)return false;
    const auto lu=reinterpret_cast<std::uintptr_t(*)(void*,int)>(user)(reinterpret_cast<void*>(signin),0);if(!lu)return false;
    h.pm=lu+0x10;return memoryRead(h.pm+0x18,&h.profile,8)&&h.profile;}
inline bool coopProfileIdle(const CoopProfileHandles& h){
    std::int32_t busy=0;std::uintptr_t saver=0,loader=0;unsigned char a=1,b=1;
    return memoryRead(h.sgm,&busy,4)&&!busy&&memoryRead(h.pm,&saver,8)&&saver&&memoryRead(saver+0x3a11,&a,1)&&!a&&
        memoryRead(h.pm+8,&loader,8)&&loader&&memoryRead(loader+0x3a11,&b,1)&&!b;}
inline void coopProfileLocked(bool lock){std::uintptr_t obj=reinterpret_cast<std::uintptr_t(*)()>(gameImage+0x1548e0)(),vt=0,fn=0;
    if(obj&&memoryRead(obj,&vt,8)&&vt&&memoryRead(vt+(lock?0x48:0x50),&fn,8)&&fn)reinterpret_cast<void(*)(void*,int)>(fn)(reinterpret_cast<void*>(obj),0);}
inline bool coopProfileLoad(const CoopProfileHandles& h,bool seedCoop){
    coopProfileLocked(true);reinterpret_cast<void(*)(void*)>(gameImage+0x5a8c0)(reinterpret_cast<void*>(h.profile));
    if(seedCoop)reinterpret_cast<char(*)(void*,const char*,const char*)>(gameImage+0x105040)(reinterpret_cast<void*>(h.profile+0x28),"HasSetGamma","1");
    coopProfileLocked(false);
    std::int32_t sentinel=0x7eadbeef;std::memcpy(reinterpret_cast<void*>(h.pm+0x20),&sentinel,4);
    reinterpret_cast<void(*)(void*)>(gameImage+0xd37b0)(reinterpret_cast<void*>(h.pm));
    std::int32_t after=0;memoryRead(h.pm+0x20,&after,4);return after!=sentinel;}
void serviceCoopSaves(ULONGLONG tick){
    if(!coopSavesEnabled)return;auto& s=coopSaves;const bool want=coopModeSelected.load();
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto** root=reinterpret_cast<const char**>(base+0x2286f40);
    if(!s.original){const char* current=nullptr;if(!memoryRead(base+0x2286f40,&current,8)||!current){s.result="no_root";return;}s.original=current;}
    // 1214hs: on the main menu, or in plain gameplay (a partner who joins, or a host who presses F7, mid-mission); never
    // while loading or in another phase.
    if(s.phase==0&&want==s.active){if(!coopSavesRefresh.load()||!sampledMainMenuState)return; // 1214ht: re-list on each visit
        CoopProfileHandles r{};if(!coopProfileHandles(base,r)||!coopProfileIdle(r))return;coopSavesRefresh=false;
        std::uintptr_t vt=0,fn=0;if(!memoryRead(r.session,&vt,8)||!vt||!memoryRead(vt+0xa0,&fn,8)||!fn)return;
        reinterpret_cast<int(*)(void*,void*,void*,void*)>(fn)(reinterpret_cast<void*>(r.session),nullptr,nullptr,nullptr);
        s.target=s.active;s.phase=3;s.startedAt=tick;s.result="refreshing";++s.refreshes;return;}
    if(s.phase==0){if(want==s.active)return;const bool gameplay=outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState;
        if(!sampledMainMenuState&&!gameplay){s.result="waiting_menu_or_gameplay";return;}s.target=want;s.phase=1;s.startedAt=tick;}
    CoopProfileHandles h{};if(!coopProfileHandles(base,h)){s.result="no_profile";if(tick-s.startedAt>15000){s.phase=0;++s.failures;s.result="profile_timeout";}return;}
    std::int32_t enabled=0;memoryRead(base+0x3132e88,&enabled,4);
    if(s.phase==1){if(!enabled||!coopProfileIdle(h)){s.result="save_busy";if(tick-s.startedAt>15000){s.phase=0;++s.failures;s.result="busy_timeout";}return;}
        // 1214hu: only a changed profile is saved before the swap. coop-saveflow-v1769a: the switch rewrote the player's
        // savegame/profile.bin with identical keys (only its 16-byte header differed). The writable profile accessor
        // (FUN_140657ce0, confirmed static) sets the dirty byte at profile+8; while it is clear, memory matches the file.
        unsigned char dirty=1;memoryRead(h.profile+8,&dirty,1);s.lastDirty=dirty;
        if(!dirty){++s.profileSkips;s.phase=2;s.startedAt=tick-300;s.result="profile_clean";return;}
        ++s.profileSaves;
        reinterpret_cast<void(*)(void*)>(gameImage+0xd3bd0)(reinterpret_cast<void*>(h.pm));s.phase=2;s.startedAt=tick;s.result="profile_saving";return;}
    if(s.phase==2){if(tick-s.startedAt<300||!coopProfileIdle(h)){if(tick-s.startedAt>15000){s.phase=0;++s.failures;s.result="profile_save_timeout";}return;}
        // The target root and its profile.
        coopSaveFolder(s.folder,sizeof(s.folder));char target[MAX_PATH]{};
        if(s.target){if(s.folder[0])CreateDirectoryA(s.folder,nullptr);std::snprintf(target,sizeof(target),"%s\\profile.bin",s.folder);}
        else{std::snprintf(target,sizeof(target),"%s",s.folder);const auto n=std::strlen(target);if(n>5&&!std::strcmp(target+n-5,"_coop"))target[n-5]=0;
            std::strncat(target,"\\profile.bin",sizeof(target)-std::strlen(target)-1);}
        const bool exists=GetFileAttributesA(target)!=INVALID_FILE_ATTRIBUTES;
        *root=s.target?kCoopSaveRoot:s.original;
        if(!coopProfileLoad(h,s.target&&!exists)){
            *root=s.target?s.original:kCoopSaveRoot;coopProfileLoad(h,false);s.phase=0;++s.failures;s.result="profile_load_refused";s.target=s.active;return;}
        ++s.profileLoads;if(s.target&&!exists)++s.profileSeeds;
        std::uintptr_t vt=0,fn=0;if(!memoryRead(h.session,&vt,8)||!vt||!memoryRead(vt+0xa0,&fn,8)||!fn){s.result="no_enumerate";s.phase=0;++s.failures;return;}
        reinterpret_cast<int(*)(void*,void*,void*,void*)>(fn)(reinterpret_cast<void*>(h.session),nullptr,nullptr,nullptr);
        s.phase=3;s.startedAt=tick;s.result="listing";return;}
    if(s.phase==3){unsigned char ready=0;memoryRead(h.sgm+0x3920,&ready,1);
        if(!ready){if(tick-s.startedAt>10000){s.result="list_timeout";s.phase=0;++s.failures;s.active=s.target;}return;}
        {std::uint32_t count=0;memoryRead(h.sgm+0xd8,&count,4);s.listed=count&0xffffff;}
        std::uintptr_t game=0;if(memoryRead(base+0x39a5290,&game,8)&&game){auto* cm=reinterpret_cast<void*>(game+0x2e75d8);
            const int index=reinterpret_cast<int(*)(void*)>(gameImage+0x657380)(cm);reinterpret_cast<void(*)(void*,int)>(gameImage+0x660210)(cm,index);
            reinterpret_cast<void(*)(void*)>(gameImage+0x67d3f0)(reinterpret_cast<void*>(game+0x1f77c8));}
        if(const auto state=sampledMainMenuState){std::uintptr_t file=0;memoryRead(state+0x20,&file,8);
            for(std::uintptr_t off=0x98;off<0x100&&file;off+=8){std::uintptr_t section=0,svt=0;
                if(memoryRead(state+off,&section,8)&&section&&memoryRead(section,&svt,8)&&svt==base+0x1b6d2e8){
                    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x4fb3b0)(reinterpret_cast<void*>(section),reinterpret_cast<void*>(file));++s.rebuilds;break;}}}
        s.active=s.target;++s.switches;s.phase=0;s.result=s.active?"coop":"campaign";}
}
bool coopSavesReadyForSession(){return !coopSavesEnabled||(coopSaves.active&&!coopSaves.phase);}
void requestCoopSaves(){if(coopSavesEnabled&&!coopModeSelected.load()){coopModeSelected=true;++coopMenu.sessionRequests;}}
std::uint64_t coopMenuCoopSelections(){return coopMenu.coopSelected;}bool coopModeChosen(){return coopModeSelected.load();} // 1214kg (#222)
// 1214ht (#157): Leave co-op. During an online session the pause menu's Quit Game is LEAVE CO-OP and acts as F8.
// The pause movie (corvo_pause_menu / emily_pause_menu, chapter 107, confirmed static) is the pause state's (logic 2)
// arkIggyFile at state+0x20; its root PauseMenuScene exposes pauseData.quitEntry. Quit opens the native confirmation
// (Localization keys UI_quitgame_question and ui_gameover_button_back_main_menu) and sends BackToMain or QuitGame.
// - The entry label is set through the Iggy API whenever the pause screen shows during a session, and restored after.
// - The question and its button come from the engine's translation callback, which Iggy reaches through the table at
//   0x143ec0920 (UTF16 callback +0, UTF8 +8, user data +0x20, free function +0x28 with its data +0x30; confirmed
//   static, FUN_14102a270 and FUN_14102a190). The mod wraps both pointers: during a session the two keys translate
//   to the co-op text (static strings the wrapped free function skips). The movie's translation cache is cleared
//   (gameService._C_ClearLocalization) when the session state it saw changes.
// - The hub hook turns a confirmed BackToMain during a session into ResumeGame (the pause closes as Resume does) and
//   the session is left as F8 leaves it.
struct IggyText16 {const char16_t* text=nullptr;std::int32_t length=0;};
using IggyTranslate16=int(*)(void*,IggyText16*,IggyText16*);using IggyTranslateFree=void(*)(void*,void*,int);
IggyTranslate16 realTranslate16=nullptr;IggyTranslateFree realTranslateFree=nullptr;bool coopTranslationHooked=false;
static const char16_t kLeaveQuestion16[]=u"Leave the co-op session?";static const char16_t kLeaveButton16[]=u"Leave co-op";
std::atomic<std::uint64_t> coopLeaveTranslations{0};
struct CoopPauseState {const char* result="idle";std::uintptr_t movie=0;bool labelled=false,cacheSession=false;char original[64]{};
    std::uint64_t relabels=0,restores=0,clears=0,leaves=0;};
CoopPauseState coopPause{};
inline bool coopKey16(const IggyText16* in,const char* key){if(!in||!in->text)return false;const auto n=static_cast<std::int32_t>(std::strlen(key));
    if(in->length!=n)return false;for(std::int32_t i=0;i<n;++i)if(in->text[i]!=static_cast<char16_t>(key[i]))return false;return true;}
inline int hookTranslate16(void* data,IggyText16* in,IggyText16* out){
    if(coopInSession()&&coopPauseContext.load()&&out){const char16_t* text=nullptr;
        if(coopKey16(in,"UI_quitgame_question"))text=kLeaveQuestion16;else if(coopKey16(in,"ui_gameover_button_back_main_menu"))text=kLeaveButton16;
        if(text){out->text=text;out->length=static_cast<std::int32_t>(std::char_traits<char16_t>::length(text));++coopLeaveTranslations;return 1;}}
    return realTranslate16?realTranslate16(data,in,out):0;}
// 1214hu (#157): coop-leave-v1770a: the engine registers only the UTF-8 callback (+8), so the UTF-16 wrap never ran
// (translationHooked false) and the dialog kept the stock text. FUN_14102a270 (confirmed static): with +0 empty it
// converts the key to UTF-8, calls +8(user data +0x20, {key,length}, {out,length}) (+0x18 instead when the flag at
// +0x38 is set), converts the result to UTF-16 and frees it with +0x28(+0x30, out, length). The mod now wraps +8
// as well, matches the keys case-insensitively (an optional terminating zero in the length), records the last keys
// it saw and which callbacks the engine set (mode bits: 1 UTF-16, 2 UTF-8, 4 UTF-16 alternative, 8 UTF-8
// alternative; the alternatives are not wrapped).
struct IggyText8 {const char* text=nullptr;std::int32_t length=0;};using IggyTranslate8=int(*)(void*,IggyText8*,IggyText8*);
IggyTranslate8 realTranslate8=nullptr;int coopTranslationMode=0;std::uint32_t coopTranslationFlag=0;
static const char kLeaveQuestion8[]="Leave the co-op session?";static const char kLeaveButton8[]="Leave co-op";
char coopLastKeys[6][48]{};std::atomic<unsigned> coopLastKey{0};std::atomic<std::uint64_t> coopTranslate8Calls{0};
inline bool coopKey8(const IggyText8* in,const char* key){if(!in||!in->text||in->length<=0)return false;const auto n=static_cast<std::int32_t>(std::strlen(key));
    if(in->length!=n&&!(in->length==n+1&&!in->text[n]))return false;return !_strnicmp(in->text,key,static_cast<std::size_t>(n));}
inline int hookTranslate8(void* data,IggyText8* in,IggyText8* out){
    ++coopTranslate8Calls;
    if(in&&in->text&&in->length>0){auto& slot=coopLastKeys[coopLastKey.fetch_add(1)%6];const auto n=static_cast<std::size_t>(in->length<47?in->length:47);
        for(std::size_t i=0;i<n;++i){const char c=in->text[i];slot[i]=(c<32||c=='"'||c=='\\'||c>126)?'_':c;}slot[n]=0;}
    if(coopInSession()&&coopPauseContext.load()&&out){const char* text=nullptr;
        if(coopKey8(in,"UI_quitgame_question"))text=kLeaveQuestion8;else if(coopKey8(in,"ui_gameover_button_back_main_menu"))text=kLeaveButton8;
        if(text){out->text=text;out->length=static_cast<std::int32_t>(std::strlen(text));++coopLeaveTranslations;return 1;}}
    return realTranslate8?realTranslate8(data,in,out):0;}
inline void hookTranslateFree(void* data,void* text,int bytes){
    if(text==kLeaveQuestion16||text==kLeaveButton16||text==kLeaveQuestion8||text==kLeaveButton8)return;if(realTranslateFree)realTranslateFree(data,text,bytes);}
inline void installCoopTranslation(){
    if(coopTranslationHooked)return;auto* table=reinterpret_cast<void**>(gameImage+0x3ec0920);
    if(!table[0]&&!table[1])return; // the engine registers its callback once the UI is up
    coopTranslationMode=(table[0]?1:0)|(table[1]?2:0)|(table[2]?4:0)|(table[3]?8:0);std::memcpy(&coopTranslationFlag,&table[7],4);
    realTranslateFree=reinterpret_cast<IggyTranslateFree>(table[5]);table[5]=reinterpret_cast<void*>(&hookTranslateFree);
    if(table[0]){realTranslate16=reinterpret_cast<IggyTranslate16>(table[0]);table[0]=reinterpret_cast<void*>(&hookTranslate16);}
    if(table[1]){realTranslate8=reinterpret_cast<IggyTranslate8>(table[1]);table[1]=reinterpret_cast<void*>(&hookTranslate8);}
    coopTranslationHooked=true;}
// 1214hv (#157): coop-leave-v1770b: the question was translated, but the buttons kept Back To Main Menu. The dialog's
// button labels are raw keys (PauseMenuScreen._onQuitGameClicked: ui_common_cancel, ui_gameover_button_back_main_menu)
// that the button text fields resolve through Iggy's second translation table (0x143ec08e0, keys prefixed '@',
// FUN_1400358c0 -> FUN_140034e40), not IggyFunctions.translate. Both tables end in the engine lookup
// FUN_140165420(&0x1432c7e98, key) (confirmed static; it returns the key itself when there is no entry). The mod hooks
// that lookup: in the pause context of a session the two keys resolve to the co-op text (static strings).
// 1214hw (#157): coop-leave-v1770c: that lookup hook never swapped (localizeSwaps 0). The quit dialog is an engine
// message box (arkMessageBoxManagerLocal at game+0x30f260); its AS3 request handler 0x1404630f0 turns each button key
// into a string id with FUN_140165c80(&id, key), and the movie shows the label by id (confirmed static), so a label
// cannot be raw text. The mod hooks that key-to-id function instead (the same 16-byte prologue): in the pause context
// of a session the key ui_gameover_button_back_main_menu becomes the game's own ui_common_confirm, so the dialog reads
// Cancel, Confirm and Back To Desktop under "Leave the co-op session?" (the question keeps hookTranslate8).
using CoopStringId=void*(*)(void*,const char*);CoopStringId realCoopStringId=nullptr;bool coopLocalizeHooked=false;
std::atomic<std::uint64_t> coopLocalizeSwaps{0};
inline void* hookCoopStringId(void* out,const char* key){
    if(key&&coopPauseContext.load()&&coopInSession()&&!_stricmp(key,"ui_gameover_button_back_main_menu")){++coopLocalizeSwaps;key="ui_common_confirm";}
    return realCoopStringId(out,key);}
inline bool installCoopLocalizeHook(){
    static const unsigned char prologue[]={0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x20,0x48,0x8b,0xda,0x48,0x8b,0xf9};
    coopLocalizeHooked=installNpcRemovalHook(0x165c80,prologue,sizeof(prologue),reinterpret_cast<void*>(&hookCoopStringId),realCoopStringId);
    return coopLocalizeHooked;}
void serviceCoopPause(ULONGLONG tick){
    installCoopTranslation();
    {const unsigned logic=outgoing.gameLogicState;if(logic==2)coopPauseContext=true;else if(logic!=3)coopPauseContext=false;}
    if(coopLeaveRequested.exchange(false)){++coopPause.leaves;++productArm.leaveKeys;stopOnlineSession("left_by_player");productArm={};coopPause.result="left_by_pause";}
    const auto state=sampledPauseState;if(!state){coopPause.result=coopPause.leaves?"left_by_pause":"no_pause";return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t file=0,iggy=0;
    if(!memoryRead(state+0x20,&file,8)||!file||!memoryRead(file+0x1608,&iggy,8)||!iggy){coopPause.result="no_movie";return;}
    const auto at=[&](std::uintptr_t rva){return reinterpret_cast<void*>(base+rva);};
    const auto makeRef=reinterpret_cast<IggyMakeNameRef>(at(0x1009de0));const auto getString=reinterpret_cast<IggyGetString>(at(0x1009c70));
    const auto setString=reinterpret_cast<IggySetString>(at(0x100a460));const auto callMethod=reinterpret_cast<IggyCallMethod>(at(0x100c4c0));
    const auto fastName=reinterpret_cast<IggyCreateFastName>(at(0x1009960));
    auto* root=reinterpret_cast<IggyPath28*>(iggy+0x2950);IggyPath28 data{},quit{},service{};
    if(!makeRef(&data,root,"pauseData")||!makeRef(&quit,&data,"quitEntry")){coopPause.result="no_quit_entry";return;}
    if(coopPause.movie!=file){coopPause.movie=file;coopPause.labelled=false;coopPause.cacheSession=false;coopPause.original[0]=0;}
    const bool session=coopInSession();
    if(session!=coopPause.cacheSession){ // the dialog texts are cached per movie by Localization.translate
        if(makeRef(&service,root,"gameService")){IggyData20 result{};if(!callMethod(reinterpret_cast<void*>(iggy),&result,&service,fastName(reinterpret_cast<void*>(iggy),"_C_ClearLocalization",-1),0,nullptr))++coopPause.clears;}
        coopPause.cacheSession=session;}
    static const char kLeaveLabel[]="LEAVE CO-OP";
    if(session&&!coopPause.labelled){char label[64]{};std::int32_t length=0;
        if(!getString(&quit,0,"label",static_cast<std::int32_t>(sizeof(label)-1),label,&length)&&std::strcmp(label,kLeaveLabel))std::memcpy(coopPause.original,label,sizeof(label));
        if(setString(&quit,0,"label",kLeaveLabel,static_cast<std::int32_t>(sizeof(kLeaveLabel)-1))){coopPause.labelled=true;++coopPause.relabels;coopPause.result="leave_coop";}}
    else if(!session&&coopPause.labelled){if(coopPause.original[0]&&setString(&quit,0,"label",coopPause.original,static_cast<std::int32_t>(std::strlen(coopPause.original)))){++coopPause.restores;}
        coopPause.labelled=false;coopPause.result="quit_game";}
}
// 1214hw (#158, lab): coop_loc_find <text> writes <status>.loc-find.json with the localization entries whose text contains
// <text> (ASCII, case-insensitive), to find the keys the partner's character choice uses as message box button labels
// (labels are string ids; the keys are not in the executable). The table (0x1432c7e98, chapter 108, confirmed static):
// 0x58-byte entries at *table, count in the low 24 bits at +8; per entry the id (FNV-1a of the lowercased key) at +0,
// the key idStr at +8 and the text idStr at +0x30 (an idStr is {char* text, u32 length, ...}).
inline void coopLocJson(FILE* f,const char* s,std::uint32_t n){fputc('"',f);
    for(std::uint32_t i=0;i<n&&s[i];++i){const unsigned char c=static_cast<unsigned char>(s[i]);if(c=='"'||c=='\\'){fputc('\\',f);fputc(c,f);}else if(c<32)fputc(' ',f);else fputc(c,f);}fputc('"',f);}
void coopLocFind(const char* needle){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.loc-find.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    const auto table=reinterpret_cast<std::uintptr_t>(gameImage)+0x32c7e98;std::uintptr_t entries=0;std::uint32_t count=0;
    memoryRead(table,&entries,8);memoryRead(table+8,&count,4);count&=0xffffff;const auto nlen=std::strlen(needle);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"needle\":");coopLocJson(f,needle,static_cast<std::uint32_t>(nlen));fprintf(f,",\"count\":%u,\"matches\":[",count);
    unsigned matches=0;
    for(std::uint32_t i=0;entries&&i<count&&matches<40;++i){const auto e=entries+static_cast<std::uintptr_t>(i)*0x58;
        std::uint32_t id=0,klen=0,tlen=0;std::uintptr_t kp=0,tp=0;
        if(!memoryRead(e,&id,4)||!memoryRead(e+8,&kp,8)||!memoryRead(e+0x10,&klen,4)||!memoryRead(e+0x30,&tp,8)||!memoryRead(e+0x38,&tlen,4)||!kp||!tp)continue;
        char text[200]{},key[96]{};const auto tn=tlen<sizeof(text)-1?tlen:static_cast<std::uint32_t>(sizeof(text)-1);if(!memoryRead(tp,text,tn))continue;
        bool hit=false;for(std::size_t p=0;!hit&&p+nlen<=tn;++p)hit=!_strnicmp(text+p,needle,nlen);if(!hit)continue;
        const auto kn=klen<sizeof(key)-1?klen:static_cast<std::uint32_t>(sizeof(key)-1);memoryRead(kp,key,kn);
        fprintf(f,"%s{\"id\":%u,\"key\":",matches?",":"",id);coopLocJson(f,key,kn);fprintf(f,",\"text\":");coopLocJson(f,text,tn);fputc('}',f);++matches;}
    fprintf(f,"],\"found\":%u}\n",matches);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);}
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
    fprintf(f,"],\"saves\":{\"result\":\"%s\",\"active\":%s,\"phase\":%d,\"switches\":%llu,\"failures\":%llu,\"rebuilds\":%llu,\"profileLoads\":%llu,\"profileSeeds\":%llu,\"sessionRequests\":%llu,\"refreshes\":%llu,\"listed\":%u,\"profileSaves\":%llu,\"profileSkips\":%llu,\"lastDirty\":%d}",coopSaves.result,
        coopSaves.active?"true":"false",coopSaves.phase,static_cast<unsigned long long>(coopSaves.switches),static_cast<unsigned long long>(coopSaves.failures),
        static_cast<unsigned long long>(coopSaves.rebuilds),static_cast<unsigned long long>(coopSaves.profileLoads),static_cast<unsigned long long>(coopSaves.profileSeeds),
        static_cast<unsigned long long>(coopMenu.sessionRequests),static_cast<unsigned long long>(coopSaves.refreshes),coopSaves.listed,
        static_cast<unsigned long long>(coopSaves.profileSaves),static_cast<unsigned long long>(coopSaves.profileSkips),coopSaves.lastDirty);
    fprintf(f,",\"pause\":{\"result\":\"%s\",\"inSession\":%s,\"labelled\":%s,\"relabels\":%llu,\"restores\":%llu,\"clears\":%llu,\"leaves\":%llu,\"translationHooked\":%s,\"translations\":%llu,\"translationMode\":%d,\"translationFlag\":%u,\"translate8Calls\":%llu,\"localizeHooked\":%s,\"localizeSwaps\":%llu,\"pauseContext\":%s,\"lastKeys\":[\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"]}",
        coopPause.result,coopInSession()?"true":"false",coopPause.labelled?"true":"false",static_cast<unsigned long long>(coopPause.relabels),static_cast<unsigned long long>(coopPause.restores),
        static_cast<unsigned long long>(coopPause.clears),static_cast<unsigned long long>(coopPause.leaves),coopTranslationHooked?"true":"false",static_cast<unsigned long long>(coopLeaveTranslations.load()),
        coopTranslationMode,coopTranslationFlag,static_cast<unsigned long long>(coopTranslate8Calls.load()),
        coopLocalizeHooked?"true":"false",static_cast<unsigned long long>(coopLocalizeSwaps.load()),coopPauseContext.load()?"true":"false",
        coopLastKeys[0],coopLastKeys[1],coopLastKeys[2],coopLastKeys[3],coopLastKeys[4],coopLastKeys[5]);
    fputs("}\n",f);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
