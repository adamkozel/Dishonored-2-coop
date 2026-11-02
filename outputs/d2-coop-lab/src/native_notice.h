#pragma once
// 2092 (#243): co-op notices on screen. A pending notice (coopNoticePending, 1: the friend runs a different co-op version,
// set by the control channel's hello check and the lobby's version tag) opens a one-button engine message box, built
// as the Character dialog's (coop_character.h), in the pause menu or the main menu when no other co-op box is
// open. The button is the game's ui_common_confirm. Lab command coop_notice_test raises one for a visual check.
// 2100: the question is the box's own text. A question id (+0x14) is resolved to its localized text natively before
// the movie sees it (FUN_140462210 -> FUN_1400288e0 -> FUN_1401653e0, confirmed static), so the translation hook never
// read the borrowed Character key (notice-eotw-v2097a: the box read "Character"). With id 0 the box sends its idStr at
// +0x18 (built empty by the box constructor 0x460e20) through FUN_140028870 instead.
static const char kCoopNoticeVersion[]="Your friend's co-op mod is a different version. Both players need the same version to play together.";
// 2108 (ch122 still to do): notice 2, on the partner's result screen when her own Replay or Continue is held for the host.
static const char kCoopNoticeWaitHost[]="The host chooses what happens next. Your game follows when your friend continues.";
struct CoopNotice {int handle=-1;std::uint64_t shown=0,answered=0,failures=0;const char* result="idle";};
CoopNotice coopNotice{};CoopBoxCallback coopNoticeCallback{};
inline void coopNoticeAnswered(void*,int handle,int result){if(handle!=coopNotice.handle)return;coopNotice.handle=-1;++coopNotice.answered;coopNotice.result="answered";consoleOutput("COOP: notice answered (result %d)\n",result);}
inline bool openCoopNotice(const char* text){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t game=0,lib=0;
    if(!memoryRead(base+0x39a5290,&game,8)||!game)return false;const auto mgr=game+0x30f260;if(!memoryRead(mgr+0x18,&lib,8))return false;
    const auto id=[](const char* key){std::uint64_t v=0;reinterpret_cast<void*(*)(void*,const char*)>(gameImage+0x165c80)(&v,key);return static_cast<std::uint32_t>(v);};
    const std::uint32_t ok=id("ui_common_confirm");if(!ok)return false;
    coopNoticeCallback.vtable=gameImage+0x1b698b8;coopNoticeCallback.object=&coopNotice;coopNoticeCallback.function=&coopNoticeAnswered;
    coopProfileLocked(true);
    auto* box=static_cast<unsigned char*>(reinterpret_cast<void*(*)(std::size_t)>(gameImage+0xf13f0)(0x730));
    if(!box){coopProfileLocked(false);return false;}
    reinterpret_cast<void*(*)(void*,std::uintptr_t,std::uintptr_t)>(gameImage+0x460e20)(box,lib,mgr);
    {const std::uint32_t noId=0;std::memcpy(box+0x14,&noId,4);reinterpret_cast<void(*)(void*)>(gameImage+0x13fd10)(box+0x18);
     reinterpret_cast<void*(*)(void*,const char*)>(gameImage+0x13f9d0)(box+0x18,text);}
    const float none=-1.0f;std::memcpy(box+0x10,&none,4);
    {std::uint64_t v=ok;reinterpret_cast<void(*)(void*,void*)>(gameImage+0x194900)(box+0x2f8,&v);
     alignas(16) unsigned char action[0x30]{};reinterpret_cast<void(*)(void*,const char*)>(gameImage+0x13f9d0)(action,"");
     reinterpret_cast<void(*)(void*,void*)>(gameImage+0x461a80)(box+0x318,action);reinterpret_cast<void(*)(void*)>(gameImage+0x13fd10)(action);}
    void* callback=&coopNoticeCallback;std::memcpy(box+0x3c8,&callback,8);
    coopNotice.handle=reinterpret_cast<int(*)(std::uintptr_t,void*,int)>(gameImage+0x462b00)(mgr,box,0);
    coopProfileLocked(false);
    if(coopNotice.handle<0)return false;
    return true;}
void serviceCoopNotice(){
    if(!coopNoticePending.load()||coopNotice.handle>=0||coopChoice.handle>=0||GetCurrentThreadId()!=engineThread.load())return;
    // 2097: not in gameplay. A box opened there closed at once, unseen (notice-eotw-v2092a: answered three log lines after it
    // opened, no box in the frames); a pending notice waits for the pause menu.
    // 2108: notice 2 is for the result screen (logic 11, or 3 after a shared mission end) and is dropped once she left it.
    const int kind=coopNoticePending.load();const unsigned logic=outgoing.gameLogicState;if(outgoing.pendingGameLogicState)return;
    if(kind==2){if(logic!=11&&logic!=3){coopNoticePending.store(0);coopNotice.result="wait_hint_dropped";return;}}
    else if(logic!=2&&logic!=6)return;
    if(openCoopNotice(kind==2?kCoopNoticeWaitHost:kCoopNoticeVersion)){coopNoticePending.store(0);++coopNotice.shown;coopNotice.result=kind==2?"shown_wait_host":"shown";
        consoleOutput("COOP: notice %d shown in logic %u\n",kind,logic);}else{++coopNotice.failures;coopNotice.result="open_failed";}
}
