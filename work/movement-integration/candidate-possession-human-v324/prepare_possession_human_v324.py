"""Finite native human-Possession observation fixture; no direct target control."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]; S=R/'outputs/d2-coop-lab/src'
p=S/'native_player_agility.h'; s=p.read_text()
s=s.replace('NativePlayerAgility','NativePossessionHumanUpgrade').replace('readNativePlayerAgility','readNativePossessionHumanUpgrade').replace('super_jump.inventoryitem','possession_parasite.inventoryitem').replace('type!=18','type!=20')
s=s.replace('p.player.sword.logic!=1','p.player.sword.logic!=1||p.player.character!=1')
s=s.replace('An enhancement is an inventory declaration, not an equipment entity. Scope\n// is this exact installed Agility item; never grant arbitrary declarations.','Exact installed TYPE_POWER_UPGRADE20: possession_parasite grants\n// ARK_ATTRIBUTE_POSSESSION_ALLOW_HUMANS. Read-only, Corvo-only, no target writes.')
(S/'native_player_possession_upgrade.h').write_text(s)
p=S/'possession_fixture.h';s=p.read_text()
s=s.replace('#include "native_player_possession_item.h"','#include "native_player_possession_item.h"\n#include "native_player_possession_upgrade.h"')
s=s.replace('NativePossessionItem before{},owned{};','NativePossessionItem before{},owned{};NativePossessionHumanUpgrade humanBefore{};bool human=false,humanRestoreSent=false;')
s=s.replace('if(request==1||request==3){NativePossessionItem p{};','if(request==1||request==3||request==4){NativePossessionItem p{};NativePossessionHumanUpgrade human{};')
s=s.replace('request==3?40000:20000','(request==3||request==4)?40000:20000')
s=s.replace('!readNativePossessionItem(memoryRead,base,p)||p.quantity', '!readNativePossessionItem(memoryRead,base,p)||(request==4&&(!readNativePossessionHumanUpgrade(memoryRead,base,human)||human.quantity||!sameSwordEquipmentIdentity(p.player.sword,human.player.sword)))||p.quantity',1)
s=s.replace('f={};f.before=p;f.epoch=', 'f={};f.before=p;f.human=request==4;f.humanBefore=human;f.epoch=',1)
needle='            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);'
assert s.count(needle)==1
s=s.replace(needle,'            if(f.human)reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(human.player.sword.inventory),reinterpret_cast<void*>(human.declaration),1);\n'+needle)
needle='else if(!p.quantity&&!p.player.sword.slots[1]){f={};++possessionFixtureRestored;possessionFixtureResult="quantity_and_selection_restored";return;}'
assert needle in s
s=s.replace(needle,'''else if(!p.quantity&&!p.player.sword.slots[1]){
                if(f.human){NativePossessionHumanUpgrade h{};
                    if(!readNativePossessionHumanUpgrade(memoryRead,base,h)||!sameSwordEquipmentIdentity(h.player.sword,p.player.sword)||
                       h.entry!=f.humanBefore.entry||h.declaration!=f.humanBefore.declaration){possessionFixtureResult="waiting_for_original_human_upgrade";return;}
                    if(h.quantity){
                        if(!f.humanRestoreSent){reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(h.player.sword.inventory),reinterpret_cast<void*>(h.declaration),0);f.humanRestoreSent=true;}
                        possessionFixtureResult="native_human_upgrade_restore_requested";return;
                    }
                }
                f={};++possessionFixtureRestored;possessionFixtureResult="quantity_and_selection_restored";return;
            }''')
s=s.replace('First experiment observes empty-target casting only;','A separate human40000 request acquires the exact native human upgrade;')
p.write_text(s)
p=S/'engine_bridge.cpp';s=p.read_text();needle='if(!std::strcmp(text,"coop_possession_fixture 20000")';assert s.count(needle)==1;s=s.replace(needle,'if(!std::strcmp(text,"coop_possession_fixture human40000")||!std::strcmp(text,"coop_possession_fixture 20000")')
needle='    if(!std::strcmp(request.command,"coop_possession_fixture off")){possessionFixtureRequest=2;return;}'
s=s.replace(needle,'    if(!std::strcmp(request.command,"coop_possession_fixture human40000")){possessionFixtureRequest=4;return;}\n'+needle);p.write_text(s)
p=R/'outputs/d2-coop-lab/Send-D2Command.ps1';s=p.read_text();s=s.replace('^coop_possession_fixture (20000|40000|off)$','^coop_possession_fixture (20000|40000|human40000|off)$');p.write_text(s)
print('Prepared finite human-Possession fixture; normal input only, no target transfer call')
