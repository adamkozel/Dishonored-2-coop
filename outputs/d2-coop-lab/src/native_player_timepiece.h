#pragma once
#include "native_player_heart_scene.h"
// 1214eu (#102): the native Timepiece (A Crack in the Slab), read like the Heart. Identities confirmed at runtime in
// both roles after the bunker pickup (timepiece-research-v1591b): inventory item
// models/weapons/timeshift_device/timeshift_device.inventoryitem (type 2, maximum 1), component arkCpntTimeShiftDevice
// (VT 1b53ae8) owned by its declaration arkDeclCpntTimeShiftDevice (VT 1b539a8,
// models/weapons/timeshift_device/timeshift_device.cpnttimeshiftdevice, slot 1), entity idEntity (VT 1c87d58, parent
// handle 1, component member), selected in slot 1. No grants, equip calls, time shifts or gameplay writes here.
struct NativePlayerTimepiece {
    NativePlayerSword player{};
    std::uintptr_t entry=0,declaration=0,component=0,equipmentDeclaration=0,entity=0;
    unsigned handle=0,flags=0;int quantity=0,maximum=0,slot=-1;
    bool equipped=false,started=false;
};
template<class Read> bool readNativePlayerTimepieceFrame(Read read,std::uintptr_t base,NativePlayerTimepiece& p,unsigned* stage=nullptr,bool selectedOnly=false){
    if(stage)*stage=1;
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;
    const auto& inv=p.player.sword;
    unsigned first=0,end=inv.itemCount;
    if(selectedOnly){
        const auto selected=inv.slots[1];
        if(selected<inv.items||selected>=inv.items+inv.itemCount*56||(selected-inv.items)%56)return false;
        unsigned membership=0;
        for(unsigned i=0;i<inv.equipmentCount;++i){std::uintptr_t entry=0;
            if(!read(inv.equipments+i*8,&entry,8))return false;if(entry==selected)++membership;}
        if(membership!=1)return false;
        first=static_cast<unsigned>((selected-inv.items)/56);end=first+1;
    }
    for(unsigned i=first;i<end;++i){std::uintptr_t entry=inv.items+i*56,d=0,vt=0,text=0;std::string name;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&text,8)||!worldIdentityString(read,text,name))return false;
        if(name=="models/weapons/timeshift_device/timeshift_device.inventoryitem"){
            if(p.entry)return false;p.entry=entry;p.declaration=d;
        }
    }
    if(stage)*stage=2;
    unsigned type=0,parent=0;std::uintptr_t vt=0,owner=0,text=0;std::string name;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=2||!read(p.entry+8,&p.handle,4)||
       !read(p.entry+0x10,&p.component,8)||!p.component||!read(p.entry+0x18,&p.equipmentDeclaration,8)||!p.equipmentDeclaration||
       !read(p.entry+0x20,&p.quantity,4)||!read(p.entry+0x24,&p.maximum,4)||p.maximum!=1||p.quantity<0||p.quantity>p.maximum||
       !read(p.component,&vt,8)||vt!=base+0x1b53ae8||!read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+0x1b539a8||!read(p.equipmentDeclaration+8,&text,8)||
       !worldIdentityString(read,text,name)||name!="models/weapons/timeshift_device/timeshift_device.cpnttimeshiftdevice"||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1||!read(p.component+0x1c,&p.flags,4))return false;
    if(stage)*stage=3;
    p.entity=worldIdentityEntity(read,inv.game,p.handle);
    if(!p.entity||!read(p.entity,&vt,8)||vt!=base+0x1c87d58||!read(p.entity+0xac,&parent,4)||parent!=1||
       !read(p.component+0x10,&owner,8)||owner!=p.entity)return false;
    if(stage)*stage=4;
    p.equipped=inv.slots[1]==p.entry;p.started=(p.flags&0xc)==8;
    if(((p.flags&0xc)!=0&&!p.started)||p.started!=p.equipped||(selectedOnly&&!p.equipped))return false;
    if(stage)*stage=5;
    std::uintptr_t list=0;unsigned count=0,membership=0;
    if(!read(p.entity+0x268,&list,8)||!list||!read(p.entity+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==p.component)++membership;}
    return membership==1;
}
template<class Read> bool readNativePlayerTimepiece(Read read,std::uintptr_t base,NativePlayerTimepiece& out,unsigned* stage=nullptr,bool selectedOnly=false){
    NativePlayerTimepiece a{},b{};
    if(!readNativePlayerTimepieceFrame(read,base,a,stage,selectedOnly)||!readNativePlayerTimepieceFrame(read,base,b,stage,selectedOnly)||
       !sameSwordEquipmentIdentity(a.player.sword,b.player.sword)||a.player.character!=b.player.character||
       a.entry!=b.entry||a.declaration!=b.declaration||a.component!=b.component||a.equipmentDeclaration!=b.equipmentDeclaration||
       a.entity!=b.entity||a.handle!=b.handle||a.quantity!=b.quantity||a.flags!=b.flags)return false;
    out=std::move(b);return true;
}
// The Timepiece's owned left-arm scenes (takes 1590b/c, both roles): each binds the player's left arm and the exact
// Timepiece entity. Variants as HeartPresentation's held 2: 4 scan_in, 5 unfold_idle_loop, 6 scan_out, 7 scan_loop,
// 8 scan_teleport. Any other gadget_timeshift scene (equip, unequip, slashes, a failed shift, scan_loop's Scene_2) is
// refused, so it is never presented as one of these.
struct NativePlayerTimepieceScene {
    NativePlayerTimepiece timepiece{};NativePlayerArm arm{};
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0,state=0;
    std::string asset,subscene;
};
inline unsigned nativeTimepieceVariant(const std::string& asset,const std::string& sub,unsigned duration){
    const std::string prefix="midnight/gameplay/player/base/gadgets/gadget_timeshift_";
    if(asset.compare(0,prefix.size(),prefix))return 0;const auto name=asset.substr(prefix.size());
    if(name=="scan_in.midnightscene"&&sub=="Scene_1"&&duration==500)return 4;
    if(name=="unfold_idle_loop.midnightscene"&&sub=="Scene_1"&&duration==3333)return 5;
    if(name=="scan_out.midnightscene"&&sub=="Scene_1"&&duration==500)return 6;
    if(name=="scan_loop.midnightscene"&&sub=="Scene_4"&&duration==500)return 7;
    if(name=="scan_teleport.midnightscene"&&sub=="Scene_2"&&duration==500)return 8;
    return 0;
}
template<class Read> bool readNativePlayerTimepieceScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerTimepieceScene& out){
    NativePlayerTimepieceScene s{};
    s.state=nativePlayerGroundTimepieceState(states,base);if(!s.state)return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> counts{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&counts[field],4)||(counts[field]&0xffffff)>512||((counts[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(counts[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned phase=0,duration=0;std::string asset,sub;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&phase,4))return false;
            if(phase!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            if(asset.find("midnight/gameplay/player/base/gadgets/gadget_timeshift_")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||
               !read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const unsigned variant=nativeTimepieceVariant(asset,sub,duration);if(!variant)return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    // The shift's scenes run under UseTimeShiftDevice 68; the unfold, peek and fold under 69 or 70.
    if(!s.variant||((s.variant>=7)!=(s.state==68)))return false;
    if(!readNativePlayerTimepiece(read,base,s.timepiece)||s.timepiece.quantity!=1||!s.timepiece.equipped||
       s.timepiece.player.sword.game!=states.player.game||s.timepiece.player.sword.actor!=states.player.player)return false;
    if(!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=2)return false;
    // Looping scenes (5, 7) wrap their clock; the source sends their own monotonic time (shadow_stream.h).
    if(s.variant!=5&&s.variant!=7&&s.timeMs>s.durationMs)return false;
    std::array<std::uintptr_t,2> actors{};unsigned armCount=0,itemCount=0;
    for(unsigned i=0;i<2;++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.timepiece.entity){if(++itemCount>1||handle!=s.timepiece.handle)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actors[i]||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=s.timepiece.player.character)return false;
    }
    if(armCount!=1||itemCount!=1)return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<2;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;}
    NativePlayerStates finalStates{};NativePlayerTimepiece finalItem{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.player.current!=states.player.current||finalStates.component!=states.component||finalStates.mode!=states.mode||
       !finalStates.defaultMode||finalStates.active.size()!=states.active.size()||!readNativePlayerTimepiece(read,base,finalItem)||
       finalItem.entry!=s.timepiece.entry||finalItem.component!=s.timepiece.component||finalItem.entity!=s.timepiece.entity||finalItem.handle!=s.timepiece.handle||
       finalItem.quantity!=s.timepiece.quantity||finalItem.flags!=s.timepiece.flags||finalItem.equipped!=s.timepiece.equipped||
       !readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    out=std::move(s);return true;
}
