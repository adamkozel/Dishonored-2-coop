from pathlib import Path
import shutil,json
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src';N=R/'work/natural-moveset'
assert json.loads((W/'native-operation-owner.json').read_text(encoding='utf-8-sig'))['state']=='finished'
B=W/'shadowwalk-sneak-holster-pre-v550';B.mkdir(exist_ok=False)
def edit(name,pairs):
 p=S/name;shutil.copy2(p,B/name);s=p.read_text(encoding='utf-8-sig')
 for a,b in pairs:
  assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s,encoding='utf-8')
edit('native_player_equipment_scene.h',[
 ('"sword_unequip_fast"};','"sword_unequip_fast","sword_unequip_sneak_fast"};'),('i<7;++i','i<8;++i'),
 ('    if((s.variant==5&&s.durationMs!=500)', '''    //549 native crouched preparation: Sneaking6+EquipChange53,
    //Scene_1/333ms with the owned right arm alone. Actual sword contact wins.
    if(s.variant==8&&(s.character!=2||states.player.current!=1||states.active.size()!=2||powerState||drawRequest||possessionCast||s.durationMs!=333||s.timeMs>333))return false;
    if((s.variant==5&&s.durationMs!=500)''')])
edit('equipment_presentation.h',[('s.variant>7','s.variant>8'),('s.variant==7&&','s.variant>=7&&')])
edit('shadow_action_protocol.h',[
 ('version=57,','version=58,'),('p.version==57','p.version==58'),('p.equipmentVariant!=7||','p.equipmentVariant<7||'),('p.equipmentVariant<=7','p.equipmentVariant<=8')])
edit('movement_playback.h',[
 ('equipment.variant<=4||equipment.variant==7','equipment.variant<=4||equipment.variant==7||equipment.variant==8'),
 ('equipment.variant==7?86:84+equipment.variant','equipment.variant==7?86:equipment.variant==8?88:84+equipment.variant'),
 ('equipmentActive&&equipment.variant!=7','equipmentActive&&equipment.variant<=4')])
edit('world_identity_tests.cpp',[
 ('                auto fast=eq;fast.str(strings,', '''                {auto quick=other;quick.str(strings,"midnight/gameplay/player/base/combat/sword_unequip_sneak_fast.midnightscene");
                 quick.d(physics+0x50,1);quick.d(physics+0x54,1);quick.d(active,6);quick.q(mode+0x10+6*8,ground);quick.q(ground,base+0x1c84258);quick.d(ground+8,6);
                 quick.d(instance+0x18,1);quick.d(scene+0x28,333);quick.d(instance+0x160,120);
                 require(inspectEquipment(quick)&&result.variant==8&&result.character==2,"549 crouched fast holster owns native Emily arm");
                 auto bad=quick;bad.d(scene+0x28,334);require(!inspectEquipment(bad),"crouched fast holster exact clock required");
                 bad=quick;bad.q(bindings+8,weapon);require(!inspectEquipment(bad),"crouched fast holster requires arm not sword");
                 bad=quick;bad.d(physics+0x50,0);require(!inspectEquipment(bad),"crouched fast holster cannot borrow standing stance");
                 bad=quick;bad.d(instance+0x160,334);require(!inspectEquipment(bad),"expired crouched holster refused");
                 bad=quick;bad.d(arm+0xac,2);require(!inspectEquipment(bad),"foreign crouched holster arm refused");}
                auto fast=eq;fast.str(strings,''')])
edit('shadow_action_protocol_tests.cpp',[
 ('equip.equipmentVariant=8;check(!validShadowAction','equip.equipmentVariant=9;check(!validShadowAction'),
 ('      {auto quick=equip;auto ec=c,eh=h;', '      for(unsigned variant:{7u,8u}){auto quick=equip;auto ec=c,eh=h;'),
 ('quick.equipmentVariant=7;quick.equipmentDurationMs=333;', 'quick.equipmentVariant=variant;quick.equipmentDurationMs=333;'),
 ('bad.version=55;check(!validShadowAction(bad,ec,eh),"old peer cannot accept new equipment semantic")','bad.version=57;check(!validShadowAction(bad,ec,eh),"old peer cannot accept new equipment semantic")'),
 ('      t.clear();check(!t.push({100,1,1,7,334,333})', '''      t.clear();check(t.push({100,1,2,8,100,333})&&t.push({150,2,1,8,150,333})&&t.sample(150,o,a)&&o.sword==1&&o.variant==8,"crouched quick holster respects native sword contact");
      t.clear();check(!t.push({100,1,1,8,334,333}),"expired crouched quick holster refused");
      t.clear();check(!t.push({100,1,1,7,334,333})''')])
s=(W/'Build-ShadowWalkSneak-v548.ps1').read_text(encoding='utf-8-sig').replace('v548.log','v550.log').replace('Build548','Build550')
(W/'Build-ShadowWalkSneak-v550.ps1').write_text(s,encoding='utf-8')
s=(W/'archive_shadowwalk_sneak_v548.py').read_text(encoding='utf-8-sig').replace("A=W/'candidate-shadowwalk-sneak-v548'","A=W/'candidate-shadowwalk-sneak-v550'")
s=s.replace("previous=W/'candidate-shadowwalk-fx-v533';probe=W/'candidate-material-probe-v541'","previous=W/'candidate-shadowwalk-sneak-v548';probe=previous")
s=s.replace('v548.log','v550.log').replace('protocol=57','protocol=58').replace("W/'Build-ShadowWalkSneak-v548.ps1'","W/'Build-ShadowWalkSneak-v550.ps1',W/'prepare_shadowwalk_sneak_holster_v550.py'")
s=s.replace("scope='Crouched Shadow Walk", "scope='550 adds observed native crouched fast holster equipment8/333ms, exact Emily Sneaking6+EquipChange53 and right-arm owner. Previous549 stream retired after the missing holster left semantic state stale266ms; timeout unchanged. Crouched Shadow Walk")
(W/'archive_shadowwalk_sneak_v550.py').write_text(s,encoding='utf-8')
for kind in ('Stage','Deploy'):
 s=(W/f'{kind}-ShadowWalkSneak-v548.ps1').read_text(encoding='utf-8-sig').replace('v548','v550').replace('Deploy548','Deploy550')
 (W/f'{kind}-ShadowWalkSneak-v550.ps1').write_text(s,encoding='utf-8')
s=(N/'Test-ShadowWalkSneak-v549.ps1').read_text(encoding='utf-8-sig').replace('candidate-shadowwalk-sneak-v548','candidate-shadowwalk-sneak-v550').replace('Pinned548','Pinned550').replace("scope='548 complete-body","scope='550 complete-body")
(N/'Test-ShadowWalkSneak-v551.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkSneak-v549.ps1').read_text(encoding='utf-8-sig').replace('v549','v551').replace('Sneak549','Sneak551')
(W/'Run-ShadowWalkSneak-v551.ps1').write_text(s,encoding='utf-8')
print('Prepared550 native crouched quick-holster repair; timeout unchanged')
