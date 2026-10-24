#pragma once
#include "native_usable.h"
inline bool nativeUsableIndexAvailable(){
    const unsigned char prefix[]={0x85,0xd2,0x78,0x75,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0x41,0x08,0x48,0x8b,0xd9,0x8b,0x48,0x78,0x81,0xe1,0xff,0xff,0xff,0x00,0x3b,0xd1,0x7d,0x57,0x45,0x84,0xc0};
    return !std::memcmp(gameImage+0x77b920,prefix,sizeof(prefix));
}
// Native indexed request preserves transition queuing, animation, collision
// servicing and state notifications. Callers own gameplay/session eligibility.
// 1214jh (#187): the state application 0x14077a880 emits the usable's script events (0x1439acf70, 0x1439acfc0) with the
// instigator at component+0xd0, which the native use method (vtable slot 30, 0x14077ae00) sets to the user's handle
// before cycling. With asLocalPlayer an empty instigator (0x3fff0010) becomes this world's local player (handle 1), so a
// lever's script runs as if this world's player had used it.
constexpr unsigned kNativeNoEntityHandle=0x3fff0010u,kNativeLocalPlayerHandle=1u;
bool requestNativeUsableIndex(const NativeUsable& binding,unsigned target,NativeUsable& after,bool asLocalPlayer=false){
    if(!nativeUsableIndexAvailable()||!nativeUsableTargetValid(binding,target))return false;
    NativeUsable fresh{};
    if(!readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,binding.handle,fresh)||
       !sameNativeUsableIdentity(binding,fresh)||!nativeUsableTargetValid(fresh,target))return false;
    if(asLocalPlayer&&fresh.instigator==kNativeNoEntityHandle)*reinterpret_cast<unsigned*>(fresh.component+0xd0)=kNativeLocalPlayerHandle;
    reinterpret_cast<void(*)(void*,int,unsigned char)>(gameImage+0x77b920)(reinterpret_cast<void*>(fresh.component),static_cast<int>(target),0);
    return readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,binding.handle,after)&&
        sameNativeUsableIdentity(fresh,after)&&(after.current==static_cast<int>(target)||after.pending==static_cast<int>(target));
}
// 1214ji (#187): the native use method, vtable slot 30 of arkCpntUsableWithStateList (0x14077ae00). It reads the user's
// handle through info+8 into component+0xd0, notifies 0x14080cd60(game+0x1f52e8, user, owner) and cycles the state
// (0x14077aec0) outside a transition. Used by the replica of a two-state usable with this world's player as the user,
// so a lever's script runs in both worlds (confirmed static, transcripts native-interaction-families-v827).
inline bool nativeUsableUseAvailable(){
    const unsigned char prefix[]={0x40,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0x42,0x08,0x48,0x8b,0xd9,0x8b,0x08,0x89,0x8b,0xd0,0x00,0x00,0x00};
    return !std::memcmp(gameImage+0x77ae00,prefix,sizeof(prefix));
}
// 1214jm (#187): the entity event "use" (descriptor 0x1439aa130: 'e', idEntity*, _user, one argument; confirmed runtime)
// posted through 0x140884920 (entity, result, event, argument). 0x140882fa0 builds the entity argument (type 'e', the
// entity's handle) and 0x140882fc0 releases the result, as the lock routine 0x14074e4c0 does with its events. The
// post variant may carry the lab event trace's jump (1214jl).
inline bool nativeUseEventAvailable(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const unsigned char variant[]={0xc6,0x01,0x65,0x48,0x85,0xd2,0x74,0x0d,0x8b,0x82,0xa8,0x00,0x00,0x00,0x89,0x41,0x08,0x48,0x8b,0xc1,0xc3};
    const unsigned char release[]={0x40,0x53,0x48,0x83,0xec,0x20,0x0f,0xb6,0x01,0x48,0x8b,0xd9,0x3c,0x39,0x75,0x09};
    const unsigned char post[]={0x4c,0x89,0x4c,0x24,0x20,0x48,0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xec,0x40};
    if(std::memcmp(gameImage+0x882fa0,variant,sizeof(variant))||std::memcmp(gameImage+0x882fc0,release,sizeof(release))||
       (std::memcmp(gameImage+0x884920,post,sizeof(post))&&!(gameImage[0x884920]==0xff&&gameImage[0x884921]==0x25)))return false;
    const auto event=base+0x39aa130;std::uintptr_t p=0;std::string text;unsigned args=0;
    const auto check=[&](unsigned offset,const char* expected){return memoryRead(event+offset,&p,8)&&worldIdentityString(memoryRead,p,text)&&text==expected;};
    return check(0,"use")&&check(8,"e")&&check(0x10,"idEntity*;")&&check(0x18,"_user;")&&memoryRead(event+0x24,&args,4)&&args==1;
}
bool postNativeUseEvent(std::uintptr_t usableEntity){
    if(!usableEntity||!observedPlayer||!nativeUseEventAvailable())return false;
    alignas(16) unsigned char argument[24]{},result[24]{};
    auto* a=reinterpret_cast<void*(*)(void*,void*)>(gameImage+0x882fa0)(argument,reinterpret_cast<void*>(observedPlayer));
    reinterpret_cast<void*(*)(void*,void*,void*,void*)>(gameImage+0x884920)(reinterpret_cast<void*>(usableEntity),result,gameImage+0x39aa130,a);
    reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(result);
    return true;
}
// 1214jn (#187): the use mirror's apply for any eligible usable (one or two states): the "use" event, then the use
// method unless the event already started the change (a one-state usable never changes, so its use method always runs).
bool applyNativeUseAsLocalPlayer(const NativeUsable& binding){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeUsable fresh{},posted{};std::uintptr_t vt=0,slot=0;
    if(!nativeUsableUseAvailable()||!readNativeUsable(memoryRead,base,observedGame,binding.handle,fresh)||!sameNativeUsableIdentity(binding,fresh)||
       fresh.pending!= -1||fresh.transitioning()||!memoryRead(fresh.component,&vt,8)||vt!=base+0x1c5bfc8||!memoryRead(vt+30*8,&slot,8)||slot!=base+0x77ae00)return false;
    if(!postNativeUseEvent(fresh.actor))return false;
    if(!readNativeUsable(memoryRead,base,observedGame,binding.handle,posted)||!sameNativeUsableIdentity(fresh,posted))return false;
    if(posted.current!=fresh.current||posted.pending!= -1||posted.transitioning())return true;
    const unsigned user=kNativeLocalPlayerHandle;struct {std::uintptr_t unused;const unsigned* handle;} info{0,&user};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x77ae00)(reinterpret_cast<void*>(fresh.component),&info);
    return true;
}
bool useNativeUsableAsLocalPlayer(const NativeUsable& binding,unsigned target,NativeUsable& after){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!nativeUsableUseAvailable()||binding.stateCount!=2||!nativeUsableTargetValid(binding,target))return false;
    NativeUsable fresh{};std::uintptr_t vt=0,slot=0;
    if(!readNativeUsable(memoryRead,base,observedGame,binding.handle,fresh)||!sameNativeUsableIdentity(binding,fresh)||!nativeUsableTargetValid(fresh,target)||
       fresh.current==static_cast<int>(target)||fresh.pending!= -1||fresh.transitioning()||
       !memoryRead(fresh.component,&vt,8)||vt!=base+0x1c5bfc8||!memoryRead(vt+30*8,&slot,8)||slot!=base+0x77ae00)return false;
    // 1214jm: the "use" event first, as the player's use posts it; the use method runs only if that did not already
    // start the change (the component may handle the event itself).
    if(!postNativeUseEvent(fresh.actor))return false;
    NativeUsable posted{};
    if(!readNativeUsable(memoryRead,base,observedGame,binding.handle,posted)||!sameNativeUsableIdentity(fresh,posted))return false;
    if(posted.current==static_cast<int>(target)||posted.pending==static_cast<int>(target)||posted.transitioning()){after=posted;return true;}
    const unsigned user=kNativeLocalPlayerHandle;struct {std::uintptr_t unused;const unsigned* handle;} info{0,&user};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x77ae00)(reinterpret_cast<void*>(fresh.component),&info);
    return readNativeUsable(memoryRead,base,observedGame,binding.handle,after)&&sameNativeUsableIdentity(fresh,after)&&
        (after.current==static_cast<int>(target)||after.pending==static_cast<int>(target)||after.transitioning());
}
