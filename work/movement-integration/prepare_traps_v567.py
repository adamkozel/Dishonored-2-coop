from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';B=W/'traps-pre-v567';B.mkdir(exist_ok=False)
for name in ('engine_bridge.cpp','runtime_probe.cpp','grenade_fixture.h'):shutil.copy2(S/name,B/name)
shutil.copy2(R/'outputs/d2-coop-lab/Send-D2Command.ps1',B/'Send-D2Command.ps1')
s=(S/'native_player_heart.h').read_text().replace('Heart','Placeable').replace('heart','placeable')
s=s.replace('unsigned handle=0,flags=0;', 'unsigned kind=0,handle=0,flags=0;')
s=s.replace('NativePlayerPlaceable& p,unsigned* stage', 'unsigned kind,NativePlayerPlaceable& p,unsigned* stage').replace('NativePlayerPlaceable& out,unsigned* stage', 'unsigned kind,NativePlayerPlaceable& out,unsigned* stage')
s=s.replace('    if(stage)*stage=1;', '    if(kind<1||kind>2)return false;p.kind=kind;\n    if(stage)*stage=1;')
s=s.replace('name=="models/items/placeable/placeable.inventoryitem"', 'name==(kind==1?"components/equipment/gadgets/stun_mine.inventoryitem":"components/equipment/gadgets/springrazor.inventoryitem")')
a=s.index('    if(!p.entry||');b=s.index('    if(stage)*stage=3;',a)
s=s[:a]+'''    std::uintptr_t equipmentDef=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=4||!read(p.entry+8,&p.handle,4)||
       !read(p.entry+0x10,&p.component,8)||!read(p.entry+0x18,&p.equipmentDeclaration,8)||
       !read(p.entry+0x20,&p.quantity,4)||!read(p.entry+0x24,&p.maximum,4)||p.maximum!=3||p.quantity<0||p.quantity>p.maximum||
       !read(p.declaration+0x80,&equipmentDef,8)||!equipmentDef||!read(equipmentDef,&vt,8)||vt!=base+0x1ab58f8||
       !read(equipmentDef+8,&text,8)||!worldIdentityString(read,text,name)||
       name!=(kind==1?"models/weapons/gadgets/stun_mine_equipment.def":"models/weapons/gadgets/springrazor_equipment.def"))return false;
    // Native zero-quantity trap entries have no equipment entity yet. This
    // absent form is allowed only for the isolated acquisition reader.
    if(!p.component)return !selectedOnly&&!p.equipmentDeclaration&&!p.quantity&&p.handle==0x3fff0010&&inv.slots[1]!=p.entry;
    if(!p.equipmentDeclaration||!read(p.component,&vt,8)||vt!=base+0x1b54538||
       !read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||!read(p.equipmentDeclaration,&vt,8)||vt!=base+0x1b543f8||
       !read(p.equipmentDeclaration+8,&text,8)||!worldIdentityString(read,text,name)||
       name!=(kind==1?"components/characters/player/base/weapons/stun_mine.cpntweaponplaceable":"components/characters/player/base/weapons/springrazor.cpntweaponplaceable")||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1||!read(p.component+0x1c,&p.flags,4))return false;
'''+s[b:]
s=s.replace('readNativePlayerPlaceableFrame(read,base,a,','readNativePlayerPlaceableFrame(read,base,kind,a,').replace('readNativePlayerPlaceableFrame(read,base,b,','readNativePlayerPlaceableFrame(read,base,kind,b,')
a=s.index('// B671F824');b=s.index('struct NativePlayerPlaceable',a)
s=s[:a]+'''// B671F824 exact Void trap declarations and static RTTI567. Runtime
// ownership and lifecycle are validated before the isolated fixture equips.
// Read-only: no placement, damage or native event calls.
'''+s[b:]
(S/'native_player_placeable.h').write_text(s)
f=(S/'heart_fixture.h').read_text().replace('Heart','Trap').replace('heart','trap').replace('NativePlayerTrap','NativePlayerPlaceable').replace('readNativePlayerTrap','readNativePlayerPlaceable').replace('native_player_trap.h','native_player_placeable.h')
f=f.replace('NativePlayerPlaceable before{};', 'NativePlayerPlaceable before{},acquired{};')
f=f.replace('if(request==1){NativePlayerPlaceable p{};', 'if(request==1||request==2){NativePlayerPlaceable p{};')
f=f.replace('readNativePlayerPlaceable(memoryRead,base,p)', 'readNativePlayerPlaceable(memoryRead,base,request,p)',1)
f=f.replace('request==2||tick>=f.until','request==3||tick>=f.until')
f=f.replace('readNativePlayerPlaceable(memoryRead,base,p)', 'readNativePlayerPlaceable(memoryRead,base,f.before.kind,p)')
f=f.replace('p.component==f.before.component&&p.entity==f.before.entity&&p.handle==f.before.handle&&p.equipmentDeclaration==f.before.equipmentDeclaration', '(!f.acquired.component||(p.component==f.acquired.component&&p.entity==f.acquired.entity&&p.handle==f.acquired.handle&&p.equipmentDeclaration==f.acquired.equipmentDeclaration))')
f=f.replace('    if(!same)trapFixtureResult=', '    if(same&&p.component&&!f.acquired.component)f.acquired=p;\n    if(!same)trapFixtureResult=')
f=f.replace('if(p.quantity==1&&p.equipped&&p.started)', 'if(p.quantity==1&&p.component&&p.equipped&&p.started)').replace('else if(p.quantity==1&&!p.equipped&&!f.equipSent)', 'else if(p.quantity==1&&p.component&&!p.equipped&&!f.equipSent)')
(S/'trap_fixture.h').write_text(f)
p=S/'grenade_fixture.h';s=p.read_text().replace('bool movementHeartFixtureBusy();','bool movementHeartFixtureBusy();\nbool movementTrapFixtureBusy();').replace('||movementHeartFixtureBusy();','||movementHeartFixtureBusy()||movementTrapFixtureBusy();');p.write_text(s)
p=S/'engine_bridge.cpp';s=p.read_text().replace('#include "heart_fixture.h"','#include "heart_fixture.h"\n#include "trap_fixture.h"')
line='    if(!std::strcmp(text,"coop_heart_fixture 20000")||!std::strcmp(text,"coop_heart_fixture off"))return true;';assert line in s
s=s.replace(line,line+'\n    if(!std::strcmp(text,"coop_trap_fixture stun")||!std::strcmp(text,"coop_trap_fixture razor")||!std::strcmp(text,"coop_trap_fixture off"))return true;')
line='    if(!std::strcmp(request.command,"coop_heart_fixture off")){heartFixtureRequest=2;return;}';assert line in s
s=s.replace(line,line+'\n    if(!std::strcmp(request.command,"coop_trap_fixture stun")){trapFixtureRequest=1;return;}\n    if(!std::strcmp(request.command,"coop_trap_fixture razor")){trapFixtureRequest=2;return;}\n    if(!std::strcmp(request.command,"coop_trap_fixture off")){trapFixtureRequest=3;return;}')
s=s.replace('serviceHeartFixture(tick);','serviceHeartFixture(tick);serviceTrapFixture(tick);').replace('writeHeartFixture(tick);','writeHeartFixture(tick);writeTrapFixture(tick);');p.write_text(s)
p=R/'outputs/d2-coop-lab/Send-D2Command.ps1';s=p.read_text().replace('|coop_heart_fixture|','|coop_heart_fixture|coop_trap_fixture|');p.write_text(s)
p=S/'runtime_probe.cpp';s=p.read_text().replace('#include "native_player_heart.h"','#include "native_player_heart.h"\n#include "native_player_placeable.h"')
a=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-heart"))');b=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-inventory"))',a)
t=s[a:b].replace('if(argc==3&&!std::strcmp(argv[2],"--player-heart"))', 'if(argc==3&&(!std::strcmp(argv[2],"--player-stunmine")||!std::strcmp(argv[2],"--player-springrazor")))').replace('NativePlayerHeart','NativePlayerPlaceable').replace('readNativePlayerHeart','readNativePlayerPlaceable').replace('base,p,&stage)', 'base,!std::strcmp(argv[2],"--player-stunmine")?1u:2u,p,&stage)').replace('Heart reader','Placeable reader')
s=s[:b]+t+s[b:];p.write_text(s)
(W/'Build-Traps-v567.ps1').write_text((W/'Build-Heart-v564.ps1').read_text().replace('v564.log','v567.log').replace('Build564','Build567'))
print('Prepared bounded native trap grant/selection/rollback and read-only ownership probes; no placement action or remote binding yet.')
