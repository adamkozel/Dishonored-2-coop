#pragma once
#include <cstdint>
#include <cstring>
struct NativeCharacterState {
    std::uintptr_t game=0,settings=0,player=0,definition=0;
    int character=0;
    unsigned queued=0;
    char definitionName[128]{};
    std::uintptr_t variants[3]{};
    char variantNames[3][128]{};
};
template<class Read> bool readNativeCharacter(Read read,std::uintptr_t base,NativeCharacterState& s,std::uintptr_t ownerGame=0) {
    s={};std::uintptr_t vt=0,name=0,descriptor=0,entries=0;unsigned handle=0;
    s.game=ownerGame;
    if((!s.game&&!read(base+0x39a5290,&s.game,8))||!s.game||!read(s.game,&vt,8)||vt!=base+0x1c940a8||
       !read(s.game+0x1e8948,&s.settings,8)||!s.settings||!read(s.settings+0x4c0,&s.character,4)||s.character<1||s.character>2||
       !read(s.game+0x3130b0,&s.player,8)||!s.player||!read(s.player,&vt,8)||vt!=base+0x1c5e428||
       !read(s.player+0xa8,&handle,4)||handle!=1||!read(s.player+0x2e8,&s.definition,8)||!s.definition||
       !read(s.definition+8,&name,8)||!name||!read(name,s.definitionName,sizeof(s.definitionName)-1)||
       !read(s.game+0x60278,&s.queued,4)||!read(base+0x32cdf30,&descriptor,8)||!descriptor||
       !read(descriptor+0x10,&entries,8)||!entries)return false;
    s.queued&=0xffffff;
    bool ended=false;
    for(unsigned i=0;i<8;++i) {
        char first=0;std::int64_t value=0;
        if(!read(entries+i*16,&name,8)||!name||!read(name,&first,1))return false;
        if(!first){ended=true;break;}
        if(!read(entries+i*16+8,&value,8))return false;
        if(value<1||value>2)continue;
        if(s.variants[value]||!read(s.settings+0x4c8+i*8,&s.variants[value],8)||!s.variants[value]||
           !read(s.variants[value]+8,&name,8)||!name||!read(name,s.variantNames[value],127))return false;
    }
    return ended&&s.queued<=32&&
        !std::strcmp(s.variantNames[1],"models/characters/player/corvo/body.def")&&
        !std::strcmp(s.variantNames[2],"models/characters/player/emily/body.def");
}
