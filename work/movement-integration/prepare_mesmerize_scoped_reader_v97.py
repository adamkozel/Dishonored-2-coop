from pathlib import Path
R=Path(__file__).resolve().parents[2]
S=R/'outputs/d2-coop-lab/src'
p=S/'mesmerize_partner_cached_identity.h'
s=p.read_text()
start=s.index('template<class Read>')
s=s[start:]
old='''template<class Read> bool currentMesmerizePartnerOwner(Read read,std::uintptr_t base,
 const CoopDamageAvatar& a,const NativeMesmerizeWorld& w,std::uintptr_t component,std::uintptr_t candidate){
 if(!w.present||!w.equipment.instantiated||component!=w.component||candidate!=a.actor||
    w.equipment.player.character!=2||w.equipment.player.sword.game!=a.game||!validCoopDamageAvatar(read,base,a))return false;
 const auto& e=w.equipment;const auto& inv=e.player.sword;'''
new='''template<class Read> bool currentNativeMesmerizeWorldOwner(Read read,std::uintptr_t base,const NativeMesmerizeWorld& w){
 if(!w.present||!w.equipment.instantiated||w.equipment.player.character!=2)return false;
 const auto& e=w.equipment;const auto& inv=e.player.sword;const auto component=w.component;
 std::uintptr_t game=0,vt=0;
 if(!read(base+0x39a5290,&game,8)||!game||game!=inv.game||!read(game,&vt,8)||vt!=base+0x1c940a8)return false;'''
assert old in s
s=s.replace(old,new).replace('a.game','game')
s=s.replace('||(state!=1&&state!=3))return false;', '||state>4)return false;')
s=s.replace('eq(e.entry+0x10,e.component)&&eq(component+0x10,w.actor)&&validCoopDamageAvatar(read,base,a);','eq(e.entry+0x10,e.component)&&eq(component+0x10,w.actor)&&eq(base+0x39a5290,game);')
assert 'CoopDamage' not in s and 'a.' not in s
(S/'native_mesmerize_world_owner.h').write_text('''#pragma once
#include "native_mesmerize_world.h"
#include <algorithm>
// Discovery does not authorize future access. Revalidate this exact local
// Emily equipment, inventory membership, +d4 committed object and generations.
// Does not depend on selected equipment, sword attachments or health state.
'''+s)
p.write_text('''#pragma once
#include "mesmerize_partner_identity.h"
#include "native_mesmerize_world_owner.h"
template<class Read> bool currentMesmerizePartnerOwner(Read read,std::uintptr_t base,
 const CoopDamageAvatar& a,const NativeMesmerizeWorld& w,std::uintptr_t component,std::uintptr_t candidate){
 if(!candidate||candidate!=a.actor||component!=w.component||w.equipment.player.sword.game!=a.game||
    !validCoopDamageAvatar(read,base,a)||!currentNativeMesmerizeWorldOwner(read,base,w))return false;
 unsigned state=0;
 return read(component+0x2c,&state,4)&&(state==1||state==3)&&validCoopDamageAvatar(read,base,a);
}
''')
print('Shared current Mesmerize ownership validation extracted')
