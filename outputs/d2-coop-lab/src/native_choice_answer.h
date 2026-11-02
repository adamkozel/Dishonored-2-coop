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
int nativeChoiceAnsweredPick=0;const char* nativeChoiceResult="idle";
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
    if(nativeChoiceAnsweredPick)return;
    if(!nativeChoiceSeenAt){nativeChoiceSeenAt=tick;nativeChoiceResult="seen";return;}
    if(tick-nativeChoiceSeenAt<2500)return; // after the choice scene's Inputs On (hypothesis: a settle, as the harness used)
    std::uintptr_t st=0,vt=0;int box=0;unsigned char confirmed=0;
    if(!memoryRead(observedGame+0x1f6f50,&st,8)||!st||!memoryRead(st,&vt,8)||vt!=base+0x1b698d8||!memoryRead(st+0x110,&box,4)||!memoryRead(st+0x114,&confirmed,1)){
        ++nativeChoiceRefused;nativeChoiceResult="state_unreadable";return;}
    if(box!=-1||confirmed){nativeChoiceResult="box_open_or_confirmed";return;}
    *reinterpret_cast<int*>(st+0x48)=coopNativeChoicePick==1?1:0;*reinterpret_cast<unsigned char*>(st+0x114)=1;
    nativeChoiceAnsweredPick=coopNativeChoicePick;++nativeChoiceAnswered;nativeChoiceResult="answered";
}
void writeNativeChoice(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.native-choice.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"auto\":%s,\"result\":\"%s\",\"pick\":%d,\"answered\":%llu,\"backstops\":%llu,\"refused\":%llu,\"role\":%u}\n",
        nativeChoiceAuto?"true":"false",nativeChoiceResult,coopNativeChoicePick,nativeChoiceAnswered,nativeChoiceBackstops,nativeChoiceRefused,localRole);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
