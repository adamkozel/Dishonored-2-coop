#pragma once
#include "native_player_sword.h"
// Developer cleanup only: restore the one scalar note quantity observed in
// the preserved pre-test inventory. This is not a pickup/progression relay.
struct NativeFixtureNote { NativePlayerSword player{};std::uintptr_t entry=0,decl=0;int quantity=0; };
bool readFixtureNote(NativeFixtureNote& out){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeFixtureNote n{};
    if(!readNativePlayerSword(memoryRead,base,n.player)||n.player.sword.logic!=1)return false;
    for(unsigned i=0;i<n.player.sword.itemCount;++i){
        const auto entry=n.player.sword.items+i*56;std::uintptr_t decl=0,vt=0,name=0;std::string text;
        if(!memoryRead(entry,&decl,8)||!decl||!memoryRead(decl,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,text))return false;
        if(text=="models/interactive/notes/note_dw_frommeagan.inventoryitem"){
            if(n.entry)return false;n.entry=entry;n.decl=decl;
        }
    }
    unsigned type=0,handle=0;int maximum=0;std::uintptr_t component=0,equipment=0;
    if(!n.entry||!memoryRead(n.decl+0x60,&type,4)||type!=12||
       !memoryRead(n.entry+8,&handle,4)||handle!=0x3fff0010||
       !memoryRead(n.entry+0x10,&component,8)||component||!memoryRead(n.entry+0x18,&equipment,8)||equipment||
       !memoryRead(n.entry+0x20,&n.quantity,4)||n.quantity<0||n.quantity>1||
       !memoryRead(n.entry+0x24,&maximum,4)||maximum!=1)return false;
    out=std::move(n);return true;
}
int noteRestoreRequest=-1;std::uint64_t noteRestoreCalls=0,noteRestoreRefused=0;
void serviceNoteRestoreFixture(){
    if(noteRestoreRequest<0||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const int requested=noteRestoreRequest;noteRestoreRequest=-1;NativeFixtureNote before{},again{},after{};
    const unsigned char bytes[]={0x4c,0x8b,0xdc,0x57,0x48,0x83,0xec,0x60,0x49,0xc7,0x43,0xb8,0xfe,0xff,0xff,0xff,0x49,0x89,0x5b,0x10,0x49,0x89,0x6b,0x18,0x49,0x89,0x73,0x20,0x41,0x8b,0xf8,0x48};
    const char* result="scope_refused";bool confirmed=false;
    if(requested<=1&&!std::strcmp(mapName,kGuardMap)&&coopGameplayReady(outgoing)&&observerIsolation.ready&&observerIsolation.movementFixture&&
       !movementBankActive()&&!shadowStream.until&&!reviewClipActive()&&
       !std::memcmp(gameImage+0x618de0,bytes,sizeof(bytes))&&readFixtureNote(before)&&readFixtureNote(again)&&
       before.entry==again.entry&&before.decl==again.decl&&before.quantity==again.quantity&&
       before.player.character==again.player.character&&sameSwordEquipmentIdentity(before.player.sword,again.player.sword)){
        if(before.quantity!=requested){
            ++noteRestoreCalls;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(before.player.sword.inventory),reinterpret_cast<void*>(before.decl),requested);
        }
        confirmed=readFixtureNote(after)&&after.entry==before.entry&&after.decl==before.decl&&
            after.player.character==before.player.character&&sameSwordEquipmentIdentity(after.player.sword,before.player.sword)&&after.quantity==requested;
        result=confirmed?"native_note_quantity_restored":"native_restore_unconfirmed";
    }else ++noteRestoreRefused;
    char path[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.note-restore.json",statusPath);FILE* f=nullptr;
    if(!fopen_s(&f,path,"wb")&&f){std::fprintf(f,"{\"testFixtureOnly\":true,\"confirmed\":%s,\"requested\":%d,\"before\":%d,\"after\":%d,\"nativeCalls\":%llu,\"refused\":%llu,\"result\":\"%s\"}\n",confirmed?"true":"false",requested,before.quantity,after.quantity,noteRestoreCalls,noteRestoreRefused,result);std::fclose(f);}
}
