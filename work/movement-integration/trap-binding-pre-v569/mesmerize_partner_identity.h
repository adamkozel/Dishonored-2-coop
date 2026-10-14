#pragma once
#include "friendly_fire.h"
#include "native_mesmerize_world.h"
// 'world' must be a successful readNativeMesmerizeWorld snapshot, never wire
// data. Only the local Emily equipment's committed native object is eligible.
template<class Read> bool localMesmerizePartnerTarget(Read read,std::uintptr_t base,const CoopDamageAvatar& a,
 const NativeMesmerizeWorld& world,std::uintptr_t component,std::uintptr_t candidate){
 if(!candidate||candidate!=a.actor||!component||!world.present||component!=world.component||
    world.equipment.player.character!=2||world.equipment.player.sword.game!=a.game||
    !world.equipment.instantiated||
    (world.state!=1&&world.state!=3)||!validCoopDamageAvatar(read,base,a))return false;
 // The committed object outlives deselection and equipment stop. Its verified
 // +d4 ownership relationship, not the currently equipped item, scopes this.
 const auto player=worldIdentityEntity(read,a.game,1);std::uintptr_t vt=0;
 return player&&player==world.equipment.player.sword.actor&&read(player,&vt,8)&&vt==base+0x1c5e428&&
        worldIdentityEntity(read,a.game,world.handle)==world.actor&&validCoopDamageAvatar(read,base,a);
}
