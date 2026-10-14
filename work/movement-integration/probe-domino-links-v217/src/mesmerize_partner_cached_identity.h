#pragma once
#include "mesmerize_partner_identity.h"
#include "native_mesmerize_world_owner.h"
template<class Read> bool currentMesmerizePartnerOwner(Read read,std::uintptr_t base,
 const CoopDamageAvatar& a,const NativeMesmerizeWorld& w,std::uintptr_t component,std::uintptr_t candidate){
 if(!candidate||candidate!=a.actor||component!=w.component||w.equipment.player.sword.game!=a.game||
    !validCoopDamageAvatar(read,base,a)||!currentNativeMesmerizeWorldOwner(read,base,w))return false;
 unsigned state=0;
 return read(component+0x2c,&state,4)&&(state==1||state==3)&&validCoopDamageAvatar(read,base,a);
}
