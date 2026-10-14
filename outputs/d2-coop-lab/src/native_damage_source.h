#pragma once
#include "friendly_fire.h"
#include <cstdio>
// Read-only callback-time identity. Projectiles can disappear before the next
// engine service, so post-callback handle lookup is not historical evidence.
// This snapshot is local diagnostic evidence, never a wire pointer or handle.
struct NativeDamageSource {
    std::uintptr_t actor=0,vtable=0;unsigned handle=0,parent=0;
    bool valid=false,localOwned=false,bytesValid=false;
    std::array<char,96> type{};
    std::array<unsigned char,1024> bytes{};
};
template<class Read> NativeDamageSource readNativeDamageSource(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle){
    NativeDamageSource s{};s.handle=handle;s.actor=worldIdentityEntity(read,game,handle);
    if(!s.actor||!read(s.actor,&s.vtable,8)||s.vtable<base||s.vtable>=base+68988928||!read(s.actor+0xac,&s.parent,4))return s;
    std::uintptr_t locator=0;unsigned typeRva=0;
    if(read(s.vtable-8,&locator,8)&&locator>=base&&locator<base+68988928&&read(locator+12,&typeRva,4)&&typeRva<68988928-112){
        for(unsigned i=0;i<s.type.size()-1;++i){char c=0;if(!read(base+typeRva+16+i,&c,1)||!c)break;
            if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='.'||c=='?'||c=='@'||c=='_'))break;s.type[i]=c;}
    }
    const auto player=worldIdentityEntity(read,game,1);
    s.localOwned=player&&coopDamageSourceOwned(read,game,handle,1,player);
    s.bytesValid=read(s.actor,s.bytes.data(),s.bytes.size());
    s.valid=worldIdentityEntity(read,game,handle)==s.actor;
    if(!s.valid){s.localOwned=false;s.bytesValid=false;}return s;
}
inline void writeNativeDamageSource(FILE* f,const NativeDamageSource& s,std::uintptr_t base){
    fprintf(f,"{\"valid\":%s,\"actor\":\"%llx\",\"vtableRva\":\"%llx\",\"handle\":%u,\"parent\":%u,\"localOwned\":%s,\"type\":\"%s\",\"bytesValid\":%s,\"raw\":\"",
        s.valid?"true":"false",s.actor,s.vtable?s.vtable-base:0,s.handle,s.parent,s.localOwned?"true":"false",s.type.data(),s.bytesValid?"true":"false");
    if(s.bytesValid)for(auto byte:s.bytes)fprintf(f,"%02x",byte);fputs("\"}",f);
}
