#pragma once
// 2084 (#243): the partner's first-mission character choice, answered natively with the pick from our own Character box
// (or the campaign command's character). The native screen is logic state 14 (arkGameLogicStateCharacterChoice, vtable
// 0x1b698d8, live at game+0x1f6f50): +0x48 the selection (0 Emily, 1 Corvo), +0x110 the open box handle (-1 none), +0x114
// the confirmed flag the "Play as ...?" box callback writes (0x1404f0460). Player state 37's update plays the chosen ending
// once +0x114 is set, and the Kiscule node arkKisculeNodeActionPlayerCharacter then calls SwitchCharacter (0x1408a7650)
// itself (d2-ghidra, confirmed static; stage8-step1-notes/native-character-choice.md). The partner only: the node is
// allow-local and runs in both worlds, and the host keeps its own screen. Back in gameplay the native character is checked
// and SwitchCharacter (idempotent) is the backstop. Lab switch coop_native_choice auto|manual.
bool nativeChoiceAuto=true;std::uint64_t nativeChoiceAnswered=0,nativeChoiceBackstops=0,nativeChoiceRefused=0;ULONGLONG nativeChoiceSeenAt=0;
int nativeChoiceAnsweredPick=0;const char* nativeChoiceResult="idle";std::uint64_t nativeChoiceHeld=0,nativeChoiceRetries=0;ULONGLONG nativeChoiceConfirmedAt=0,nativeChoiceFocusAt=0;
void serviceNativeCharacterChoice(ULONGLONG tick){
    if(localRole!=1||!nativeChoiceAuto||!coopNativeChoicePick||!observedGame||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(outgoing.gameLogicState!=14){
        if(nativeChoiceAnsweredPick&&outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState){NativeCharacterState c{};
            if(readNativeCharacter(memoryRead,base,c)&&c.character!=nativeChoiceAnsweredPick){
                reinterpret_cast<void(*)(std::uintptr_t,int)>(gameImage+0x8a7650)(observedGame,nativeChoiceAnsweredPick);++nativeChoiceBackstops;nativeChoiceResult="backstop_switch";}
            else nativeChoiceResult="answered_verified";
            nativeChoiceAnsweredPick=0;}
        nativeChoiceSeenAt=0;return;}
    // 2088: the native sequence (d2-ghidra, confirmed static; stage8-step1-notes/native-character-choice.md). The choice scene
    // has an Emily-focused and a Corvo-focused node; a right press moves it (player state 37's update 0x140681cf0: input off,
    // selection 1, scene variable 1, signal 8b0), and only then does the confirm (+0x114, what a box "yes" writes) reach the
    // Corvo ending. Writing the selection alone left the scene Emily-focused and the Corvo confirm unanswered
    // (nativechoice-dunwall-v2087a). Each step waits for the scene's Inputs On (state 37 +0x58 back to 1).
    if(!nativeChoiceSeenAt){nativeChoiceSeenAt=tick;nativeChoiceResult="seen";nativeChoiceAnsweredPick=coopNativeChoicePick;nativeChoiceFocusAt=0;}
    std::uintptr_t st=0,vt=0,player=0,sm=0,inner=0,ps37=0,pvt=0;int box=0,sel=0,sceneHandle=0;unsigned char confirmed=0,inputs=0;
    if(!memoryRead(observedGame+0x1f6f50,&st,8)||!st||!memoryRead(st,&vt,8)||vt!=base+0x1b698d8||!memoryRead(st+0x110,&box,4)||
       !memoryRead(st+0x114,&confirmed,1)||!memoryRead(st+0x48,&sel,4)){++nativeChoiceRefused;nativeChoiceResult="state_unreadable";return;}
    player=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t)>(gameImage+0x8a8ba0)(observedGame);
    if(player)sm=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,void*,int)>(gameImage+0x877720)(player,gameImage+0x39aed60,2);
    if(!sm||!memoryRead(sm+0x90,&inner,8)||!inner||!memoryRead(inner+0x138,&ps37,8)||!ps37||!memoryRead(ps37,&pvt,8)||pvt!=base+0x1c3aaa8||
       !memoryRead(ps37+0x24,&sceneHandle,4)||!sceneHandle||!memoryRead(ps37+0x58,&inputs,1)){nativeChoiceResult="no_choice_state";return;}
    if(box!=-1||confirmed)return;
    const int want=coopNativeChoicePick==1?1:0;
    if(sel!=want){
        if(nativeChoiceFocusAt&&tick-nativeChoiceFocusAt>3000){ // the focus signal was dropped: reset and press again
            *reinterpret_cast<int*>(st+0x48)=0;*reinterpret_cast<unsigned char*>(ps37+0x58)=1;nativeChoiceFocusAt=0;++nativeChoiceRetries;return;}
        if(!inputs||want!=1||sel!=0)return;
        *reinterpret_cast<unsigned char*>(ps37+0x58)=0;*reinterpret_cast<int*>(st+0x48)=1;
        reinterpret_cast<void(*)(std::uintptr_t,void*,int)>(gameImage+0x854d50)(ps37,gameImage+0x398a8a0,1);
        reinterpret_cast<void(*)(std::uintptr_t,void*)>(gameImage+0x854d20)(ps37,gameImage+0x398a8b0);
        nativeChoiceFocusAt=tick;++nativeChoiceHeld;nativeChoiceResult="focused";return;}
    if(!inputs)return; // the scene is still moving to the pick
    if(tick-nativeChoiceSeenAt<1500)return;
    *reinterpret_cast<unsigned char*>(st+0x114)=1;++nativeChoiceAnswered;nativeChoiceResult="answered";
}
void writeNativeChoice(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.native-choice.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"auto\":%s,\"result\":\"%s\",\"pick\":%d,\"answered\":%llu,\"backstops\":%llu,\"refused\":%llu,\"held\":%llu,\"retries\":%llu,\"role\":%u}\n",
        nativeChoiceAuto?"true":"false",nativeChoiceResult,coopNativeChoicePick,nativeChoiceAnswered,nativeChoiceBackstops,nativeChoiceRefused,nativeChoiceHeld,nativeChoiceRetries,localRole);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
