#pragma once
#include "native_doppelganger_world.h"
// Exact native runtime437/440. Scene pointers are pooled and reused between
// spawn and dispawn: identity includes scene handle, declaration and clone.
struct NativeDoppelgangerFxScene {
    std::uintptr_t instance=0,declaration=0,scene=0,bindings=0,group=0,track=0;
    unsigned id=0,variant=0,timeMs=0,durationMs=0;
    std::array<unsigned,5> helpers{};
};
inline unsigned nativeDoppelgangerFxVariant(const std::string& asset){
    return asset=="midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene"?1:
        asset=="midnight/gameplay/human/common/doppelganger/doppelganger_dispawn.midnightscene"?2:0;
}
template<class Read> bool readNativeDoppelgangerFxScene(Read read,std::uintptr_t base,
    const NativeDoppelgangerWorld& world,NativeDoppelgangerFxScene& out,unsigned* failureLine=nullptr){
    out={};if(failureLine)*failureLine=0;
    const auto fail=[&](unsigned n){if(failureLine)*failureLine=n;return false;};
    if(!world.present||!world.actor)return fail(__LINE__);
    const auto game=world.equipment.player.sword.game;
    const auto eq=[&](std::uintptr_t p,std::uintptr_t wanted){std::uintptr_t x=0;return read(p,&x,8)&&x==wanted;};
    if(!game||!eq(base+0x39a5290,game)||!eq(game,base+0x1c940a8)||worldIdentityEntity(read,game,world.handle)!=world.actor)return fail(__LINE__);
    NativeDoppelgangerFxScene s{};std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> counts{};
    std::vector<std::pair<std::uintptr_t,std::uintptr_t>> membership;
    for(unsigned list=0;list<2;++list){const auto at=game+0x1f5338+(list?0x610:0);
        if(!read(at,&lists[list],8)||!read(at+8,&counts[list],4)||(counts[list]&0xffffff)>512||((counts[list]&0xffffff)&&!lists[list]))return fail(__LINE__);
        for(unsigned i=0;i<(counts[list]&0xffffff);++i){
            std::uintptr_t instance=0,decl=0,name=0;unsigned state=0;std::string asset;
            if(!read(lists[list]+i*8,&instance,8)||!instance||!read(instance+0x27c,&state,4))return fail(__LINE__);
            membership.push_back({lists[list]+i*8,instance});
            if(state!=3)continue;
            if(!read(instance,&decl,8)||!decl||!eq(decl,base+0x1affbc8)||!read(decl+8,&name,8)||!worldIdentityString(read,name,asset))return fail(__LINE__);
            const auto variant=nativeDoppelgangerFxVariant(asset);if(!variant)continue;
            std::uintptr_t bindings=0,actor=0;unsigned bindingFlags=0;
            if(!read(instance+0x10,&bindings,8)||!bindings||!read(instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=1||!read(bindings+8,&actor,8))return fail(__LINE__);
            if(actor!=world.actor)continue;
            if(s.instance){if(s.instance==instance)continue;return fail(__LINE__);}
            s.instance=instance;s.declaration=decl;s.bindings=bindings;s.variant=variant;s.durationMs=variant==1?1000:466;
        }
    }
    if(s.instance){
        std::uintptr_t name=0,tracks=0;unsigned duration=0,flags=0;std::string text;
        if(!read(s.instance+8,&s.scene,8)||!s.scene||!eq(s.scene,base+0x1b00198)||!read(s.scene+0x28,&duration,4)||duration!=s.durationMs||
           !read(s.scene+0x30,&name,8)||!worldIdentityString(read,name,text)||text!="Scene_1"||
           !read(s.instance+0x158,&s.id,4)||!s.id||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>s.durationMs||
           !read(s.bindings,&s.group,8)||!s.group||!eq(s.group,base+0x1c6ca98)||!eq(s.group+0xc8,s.instance)||!eq(s.group+0xd0,world.actor)||
           !read(s.group+0x20,&tracks,8)||!tracks||!read(s.group+0x28,&flags,4)||(flags&0xffffff)!=(s.variant==1?5u:4u))return fail(__LINE__);
        std::vector<std::uintptr_t> trackList(flags&0xffffff);
        if(!read(tracks,trackList.data(),trackList.size()*8))return fail(__LINE__);
        for(auto track:trackList){std::uintptr_t vt=0;
            if(!track||!read(track,&vt,8)||!eq(track+0x10,s.group))return fail(__LINE__);
            if(vt==base+0x1c7f5a8){if(s.track)return fail(__LINE__);s.track=track;}
        }
        struct Record{unsigned key,handle,stop;};static_assert(sizeof(Record)==12);
        struct Key{std::uintptr_t pointer;unsigned id,a,b,c;};static_assert(sizeof(Key)==24);
        std::uintptr_t records=0,keys=0;unsigned recordFlags=0,keyFlags=0;
        std::array<Record,5> rs{},rsAgain{};std::array<Key,5> ks{};
        if(!s.track||!read(s.track+0xc0,&records,8)||!records||!read(s.track+0xc8,&recordFlags,4)||(recordFlags&0xffffff)!=5||
           !read(s.track+0x18,&keys,8)||!keys||!read(s.track+0x20,&keyFlags,4)||(keyFlags&0xffffff)!=5||
           !read(records,rs.data(),sizeof(rs))||!read(keys,ks.data(),sizeof(ks)))return fail(__LINE__);
        constexpr const char* sockets[]={"decoy_root","sl_l_arm","sl_r_arm","decoy_spawn_l_hand","decoy_spawn_r_hand"};
        for(const auto& r:rs){
            // Packed key identifiers observed in both native scenes; record order differs.
            if(r.key<67428353||r.key>67428357||r.stop!=1)return fail(__LINE__);
            const unsigned slot=r.key-67428353;if(s.helpers[slot])return fail(__LINE__);s.helpers[slot]=r.handle;
            std::uintptr_t key=0,prop=0,decl=0,helper=worldIdentityEntity(read,game,r.handle);
            unsigned matches=0;for(const auto& k:ks)if(k.id==r.key){key=k.pointer;++matches;}
            unsigned id=0;
            if(matches!=1||!key||!read(key+8,&id,4)||id!=r.key||!read(key+0x28,&prop,8)||!prop||!eq(prop,base+0x1afdfd0)||
               !read(prop+8,&decl,8)||!decl||!eq(decl,base+0x1ab3138)||!read(decl+8,&name,8)||!worldIdentityString(read,name,text))return fail(__LINE__);
            const std::string expected=std::string("models/effects/gameplay/powers/doppelganger/decoy/spawn_despawn/decoy_")+
                (s.variant==1?"spawn":"despawn")+(slot?"_body":"")+"_01.fx";
            if(text!=expected||!read(prop+0x10,&name,8)||!worldIdentityString(read,name,text)||text!=sockets[slot]||!helper||helper==world.actor||!eq(helper,base+0x1c87d58))return fail(__LINE__);
            for(unsigned j=0;j<5;++j)if(j!=slot&&s.helpers[j]==r.handle)return fail(__LINE__);
        }
        unsigned n=0;std::vector<std::uintptr_t> trackAgain(trackList.size());
        if(!eq(s.instance,s.declaration)||!eq(s.instance+8,s.scene)||!eq(s.instance+0x10,s.bindings)||!eq(s.bindings,s.group)||!eq(s.bindings+8,world.actor)||
           !eq(s.group+0xc8,s.instance)||!eq(s.group+0xd0,world.actor)||!eq(s.group+0x20,tracks)||!read(s.group+0x28,&n,4)||n!=flags||
           !read(tracks,trackAgain.data(),trackAgain.size()*8)||trackAgain!=trackList||!eq(s.track+0xc0,records)||!read(s.track+0xc8,&n,4)||n!=recordFlags||
           !read(records,rsAgain.data(),sizeof(rsAgain))||std::memcmp(rs.data(),rsAgain.data(),sizeof(rs))||
           !read(s.instance+0x158,&n,4)||n!=s.id||!read(s.instance+0x27c,&n,4)||n!=3)return fail(__LINE__);
    }
    for(unsigned i=0;i<2;++i){unsigned n=0;const auto at=game+0x1f5338+(i?0x610:0);if(!eq(at,lists[i])||!read(at+8,&n,4)||n!=counts[i])return fail(__LINE__);}
    for(const auto& m:membership)if(!eq(m.first,m.second))return fail(__LINE__);
    if(worldIdentityEntity(read,game,world.handle)!=world.actor)return fail(__LINE__);
    out=s;return true; // A complete, stable list with no matching scene is valid absence.
}
