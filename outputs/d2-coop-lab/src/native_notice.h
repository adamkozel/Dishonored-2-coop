#pragma once
// 2092 (#243): co-op notices on screen. A pending notice (coopNoticePending, 1: the friend runs a different co-op version,
// set by the control channel's hello check and the lobby's version tag) opens a one-button engine message box, built
// as the Character dialog's (coop_character.h), in gameplay, the pause menu or the main menu when no other co-op box is
// open. Its question is the Character dialog's key, read as the notice text while the box is open (native_menu_ui.h);
// the button is the game's ui_common_confirm. Lab command coop_notice_test raises one for a visual check.
struct CoopNotice {int handle=-1;std::uint64_t shown=0,answered=0,failures=0;const char* result="idle";};
CoopNotice coopNotice{};CoopBoxCallback coopNoticeCallback{};
inline void coopNoticeAnswered(void*,int handle,int){if(handle!=coopNotice.handle)return;coopNotice.handle=-1;++coopNotice.answered;coopNotice.result="answered";coopNoticeShowing.store(false);consoleOutput("COOP: notice answered\n");}
inline bool openCoopNotice(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t game=0,lib=0;
    if(!memoryRead(base+0x39a5290,&game,8)||!game)return false;const auto mgr=game+0x30f260;if(!memoryRead(mgr+0x18,&lib,8))return false;
    const auto id=[](const char* key){std::uint64_t v=0;reinterpret_cast<void*(*)(void*,const char*)>(gameImage+0x165c80)(&v,key);return static_cast<std::uint32_t>(v);};
    const std::uint32_t question=id("ui_mplus_character_label"),ok=id("ui_common_confirm");if(!question||!ok)return false;
    coopNoticeCallback.vtable=gameImage+0x1b698b8;coopNoticeCallback.object=&coopNotice;coopNoticeCallback.function=&coopNoticeAnswered;
    coopProfileLocked(true);
    auto* box=static_cast<unsigned char*>(reinterpret_cast<void*(*)(std::size_t)>(gameImage+0xf13f0)(0x730));
    if(!box){coopProfileLocked(false);return false;}
    reinterpret_cast<void*(*)(void*,std::uintptr_t,std::uintptr_t)>(gameImage+0x460e20)(box,lib,mgr);
    std::memcpy(box+0x14,&question,4);const float none=-1.0f;std::memcpy(box+0x10,&none,4);
    {std::uint64_t v=ok;reinterpret_cast<void(*)(void*,void*)>(gameImage+0x194900)(box+0x2f8,&v);
     alignas(16) unsigned char action[0x30]{};reinterpret_cast<void(*)(void*,const char*)>(gameImage+0x13f9d0)(action,"");
     reinterpret_cast<void(*)(void*,void*)>(gameImage+0x461a80)(box+0x318,action);reinterpret_cast<void(*)(void*)>(gameImage+0x13fd10)(action);}
    void* callback=&coopNoticeCallback;std::memcpy(box+0x3c8,&callback,8);
    coopNoticeShowing.store(true);
    coopNotice.handle=reinterpret_cast<int(*)(std::uintptr_t,void*,int)>(gameImage+0x462b00)(mgr,box,0);
    coopProfileLocked(false);
    if(coopNotice.handle<0){coopNoticeShowing.store(false);return false;}
    return true;}
void serviceCoopNotice(){
    if(!coopNoticePending.load()||coopNotice.handle>=0||coopChoice.handle>=0||GetCurrentThreadId()!=engineThread.load())return;
    const unsigned logic=outgoing.gameLogicState;if(outgoing.pendingGameLogicState||(logic!=1&&logic!=2&&logic!=6))return;
    if(openCoopNotice()){coopNoticePending.store(0);++coopNotice.shown;coopNotice.result="shown";consoleOutput("COOP: notice shown (version mismatch) in logic %u\n",logic);}else{++coopNotice.failures;coopNotice.result="open_failed";}
}
