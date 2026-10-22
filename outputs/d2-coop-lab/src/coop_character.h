#pragma once
// 1214hx (#158, #159, chapter 108): each player's character, and the partner's choice on every join.
// - The local character (1 Corvo, 2 Emily) is the native player's (readNativePlayerSword, sampled in gameplay). It
//   travels in the pose packet flags (bits 8-9, pose version 12); the peer's arrives with each pose. remoteCharacter()
//   falls back to the role default (the host Corvo, the partner Emily) until the first pose that carries one, so the
//   Corvo-Emily pairing behaves as before; the role-keyed rendering is generalized in later candidates (#159).
// - The partner chooses with the game's own message box before join-follow loads the host's mission (product mode, or
//   the lab toggle coop_character_choice on). The engine message box manager (arkMessageBoxManagerLocal at
//   game+0x30f260; chapter 108, confirmed static) gets a box built as its AS3 request handler (0x1404630f0) builds
//   one, under the lock FUN_1401548e0() vtable +0x48/+0x50: FUN_1400f13f0(0x730), the default constructor
//   FUN_140460e20(box, *(mgr+0x18), mgr), the question's text id at box+0x14, -1.0 at box+0x10, per button a label id
//   (FUN_140165c80 into box+0x2f8) and an empty action (FUN_140461a80 into box+0x318), then FUN_140462b00(mgr, box, 0),
//   which returns the handle. The texts are the game's own (loc-find-v1777b): the question ui_mplus_character_label
//   ("Character"), the buttons Choice_DW_Audience_C ("Play as Corvo Attano") and Choice_DW_Audience_E ("Play as Emily
//   Kaldwin"). The callback at box+0x3c8 is {vtable, object, function}: the manager calls vtable[1](callback, handle,
//   result), and the game's arkMessageBoxCallback vtable (0x141b698b8, slot 1 0x1400c8b10) jumps to
//   function(object, handle, result). The result is the pressed button's index (hypothesis: 0 Corvo, 1 Emily; any
//   other value opens the box again).
struct CoopBoxCallback {void* vtable=nullptr;void* object=nullptr;void(*function)(void*,int,int)=nullptr;};
struct CoopCharacterChoice {int state=0;int handle=-1;int lastResult=-1000;int character=0;ULONGLONG openedAt=0;
    std::uint64_t opens=0,answers=0,others=0,failures=0,joinKey=0;const char* result="idle";};
CoopCharacterChoice coopChoice{};CoopBoxCallback coopChoiceCallback{};
std::atomic<int> coopLocalCharacter{0},coopRemoteCharacter{0};ULONGLONG coopCharacterSampledAt=0;
bool remoteCharacterKnown(){const int c=coopRemoteCharacter.load();return c==1||c==2;}
int remoteCharacter(){const int c=coopRemoteCharacter.load();return c==1||c==2?c:(localRole?1:2);}
int localCharacter(){const int c=coopLocalCharacter.load();return c==1||c==2?c:(localRole?2:1);}
inline void coopStampCharacter(){outgoing.flags=(outgoing.flags&1u)|(static_cast<std::uint32_t>(coopLocalCharacter.load()&3)<<8);}
inline void coopChoiceAnswered(void*,int handle,int result){
    auto& c=coopChoice;if(handle!=c.handle)return;c.handle=-1;c.lastResult=result;++c.answers;
    if(result==0||result==1){c.character=result?2:1;c.state=2;c.result=result?"emily":"corvo";}
    else{++c.others;c.state=0;c.result="no_choice";}}
inline bool openCoopCharacterChoice(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t game=0,lib=0;
    if(!memoryRead(base+0x39a5290,&game,8)||!game){coopChoice.result="no_game";return false;}
    const auto mgr=game+0x30f260;if(!memoryRead(mgr+0x18,&lib,8)){coopChoice.result="no_manager";return false;}
    const auto id=[](const char* key){std::uint64_t v=0;reinterpret_cast<void*(*)(void*,const char*)>(gameImage+0x165c80)(&v,key);return static_cast<std::uint32_t>(v);};
    const std::uint32_t question=id("ui_mplus_character_label"),corvo=id("Choice_DW_Audience_C"),emily=id("Choice_DW_Audience_E");
    if(!question||!corvo||!emily){coopChoice.result="no_text_ids";return false;}
    coopChoiceCallback.vtable=gameImage+0x1b698b8;coopChoiceCallback.object=&coopChoice;coopChoiceCallback.function=&coopChoiceAnswered;
    coopProfileLocked(true);
    auto* box=static_cast<unsigned char*>(reinterpret_cast<void*(*)(std::size_t)>(gameImage+0xf13f0)(0x730));
    if(!box){coopProfileLocked(false);coopChoice.result="no_box";return false;}
    reinterpret_cast<void*(*)(void*,std::uintptr_t,std::uintptr_t)>(gameImage+0x460e20)(box,lib,mgr);
    std::memcpy(box+0x14,&question,4);const float none=-1.0f;std::memcpy(box+0x10,&none,4);
    for(const std::uint32_t label:{corvo,emily}){std::uint64_t v=label;reinterpret_cast<void(*)(void*,void*)>(gameImage+0x194900)(box+0x2f8,&v);
        alignas(16) unsigned char action[0x30]{};reinterpret_cast<void(*)(void*,const char*)>(gameImage+0x13f9d0)(action,"");
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x461a80)(box+0x318,action);reinterpret_cast<void(*)(void*)>(gameImage+0x13fd10)(action);}
    void* callback=&coopChoiceCallback;std::memcpy(box+0x3c8,&callback,8);
    coopChoice.handle=reinterpret_cast<int(*)(std::uintptr_t,void*,int)>(gameImage+0x462b00)(mgr,box,0);
    coopProfileLocked(false);
    return coopChoice.handle>=0;}
// Join-follow asks before it loads the host's mission: 1 Corvo or 2 Emily once chosen, else 0 (still choosing).
// A new connection is a new join, and every join chooses again.
int coopPartnerCharacterForJoin(ULONGLONG tick,std::uint64_t joinKey){
    auto& c=coopChoice;if(c.joinKey!=joinKey){const auto opens=c.opens,answers=c.answers,others=c.others,failures=c.failures;
        c=CoopCharacterChoice{};c.joinKey=joinKey;c.opens=opens;c.answers=answers;c.others=others;c.failures=failures;}
    if(c.state==2)return c.character;
    if(c.state==0){if(openCoopCharacterChoice()){c.state=1;c.openedAt=tick;++c.opens;c.result="open";}else ++c.failures;}
    return 0;}
void serviceCoopCharacter(ULONGLONG tick){
    if(!connection)coopRemoteCharacter=0;
    if(tick-coopCharacterSampledAt<500||outgoing.gameLogicState!=1||outgoing.pendingGameLogicState)return;coopCharacterSampledAt=tick;
    NativePlayerSword s{};if(readNativePlayerSword(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s)&&(s.character==1||s.character==2))coopLocalCharacter=static_cast<int>(s.character);}
void writeCoopCharacter(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.character.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& c=coopChoice;
    fprintf(f,"{\"local\":%d,\"remote\":%d,\"localUsed\":%d,\"remoteUsed\":%d,\"role\":%u,\"choiceEnabled\":%s,\"choice\":{\"state\":%d,\"result\":\"%s\",\"character\":%d,\"handle\":%d,\"lastResult\":%d,\"opens\":%llu,\"answers\":%llu,\"others\":%llu,\"failures\":%llu}}\n",
        coopLocalCharacter.load(),coopRemoteCharacter.load(),localCharacter(),remoteCharacter(),localRole,(productMode||coopCharacterChoiceEnabled)?"true":"false",
        c.state,c.result,c.character,c.handle,c.lastResult,static_cast<unsigned long long>(c.opens),static_cast<unsigned long long>(c.answers),
        static_cast<unsigned long long>(c.others),static_cast<unsigned long long>(c.failures));
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);}
