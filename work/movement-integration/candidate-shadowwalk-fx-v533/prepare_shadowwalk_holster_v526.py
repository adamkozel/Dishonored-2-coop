from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src';B=W/'shadowwalk-holster-pre-v526';B.mkdir(exist_ok=False)
def edit(name,pairs):
 p=S/name;shutil.copy2(p,B/name);s=p.read_text(encoding='utf-8-sig')
 for a,b in pairs:
  assert s.count(a)==1,(name,a);s=s.replace(a,b)
 p.write_text(s,encoding='utf-8')
edit('native_player_equipment_scene.h',[
 ('"sword_equip_sneak_fast"};','"sword_equip_sneak_fast","sword_unequip_fast"};'),('i<6;++i','i<7;++i'),
 ('    if((s.variant==5&&s.durationMs!=500)', '''    //525 observed Emily preparation: standing default mode0+53, exact
    //333ms fast holster with its owned right arm alone. Preserve actual
    //native sword contact; this is not permission to activate Shadow Walk.
    if(s.variant==7&&(s.character!=2||states.player.current!=0||states.active.size()!=2||powerState||drawRequest||possessionCast||s.durationMs!=333||s.timeMs>333))return false;
    if((s.variant==5&&s.durationMs!=500)''')])
edit('equipment_presentation.h',[
 ('s.variant>6','s.variant>7'),
 ('(s.variant?(!s.duration', '(s.variant==7&&(s.duration!=333||s.elapsed>333))||\n            (s.variant?(!s.duration')])
edit('shadow_action_protocol.h',[
 ('version=55,','version=56,'),('p.version==55','p.version==56'),('p.equipmentVariant<=6','p.equipmentVariant<=7'),
 ('const bool equipment=(p.kind==0?', 'const bool equipment=(p.equipmentVariant!=7||(p.role==1&&p.character==2&&p.equipmentDurationMs==333&&p.equipmentTimeMs<=333))&&(p.kind==0?')])
edit('movement_playback.h',[
 ('equipment.variant>=1&&equipment.variant<=4&&pose.motionValid', '(equipment.variant>=1&&equipment.variant<=4||equipment.variant==7)&&pose.motionValid'),
 ('p.equipmentClip=84+equipment.variant;', 'p.equipmentClip=equipment.variant==7?86:84+equipment.variant;'),
 ('if(equipmentActive){const float contact=', 'if(equipmentActive&&equipment.variant!=7){const float contact=')])
edit('world_identity_tests.cpp',[
 ('                auto fast=eq;fast.str(strings,', '''                {auto quick=other;quick.str(strings,"midnight/gameplay/player/base/combat/sword_unequip_fast.midnightscene");
                 quick.d(instance+0x18,1);quick.d(scene+0x28,333);quick.d(instance+0x160,120);
                 require(inspectEquipment(quick)&&result.variant==7&&result.character==2,"observed Emily333ms fast holster uses owned right arm");
                 auto bad=quick;bad.d(scene+0x28,334);require(!inspectEquipment(bad),"fast holster exact source clock required");
                 bad=quick;bad.q(bindings+8,weapon);require(!inspectEquipment(bad),"fast holster cannot bind sword instead of arm");
                 bad=quick;bad.d(instance+0x160,334);require(!inspectEquipment(bad),"expired fast holster refused");
                 bad=quick;bad.str(names+0x400,"models/characters/player/corvo/body.def");require(!inspectEquipment(bad),"foreign character cannot borrow Emily holster");
                 bad=quick;bad.d(instance+0x27c,2);require(!inspectEquipment(bad),"retired fast holster refused");}
                auto fast=eq;fast.str(strings,''')])
edit('shadow_action_protocol_tests.cpp',[
 ('equip.equipmentVariant=7;check(!validShadowAction(equip,c,h),"unknown equipment action refused");', 'equip.equipmentVariant=8;check(!validShadowAction(equip,c,h),"unknown equipment action refused");'),
 ('      equip.equipmentDurationMs=10001;', '''      {auto quick=equip;auto ec=c,eh=h;ec.role=0;eh.role=1;quick.role=1;quick.character=2;quick.equipmentVariant=7;quick.equipmentDurationMs=333;quick.equipmentTimeMs=150;
       check(validShadowAction(quick,ec,eh),"observed Emily quick holster packet accepted");
       auto bad=quick;bad.equipmentDurationMs=334;check(!validShadowAction(bad,ec,eh),"quick holster exact duration required");
       bad=quick;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"unobserved Corvo quick holster refused");
       bad=quick;bad.version=55;check(!validShadowAction(bad,ec,eh),"old peer cannot accept new equipment semantic");}
      equip.equipmentDurationMs=10001;'''),
 ('      t.clear();check(t.push({100,1,2,6,100,333})', '''      t.clear();check(t.push({100,1,2,7,100,333})&&t.push({150,2,1,7,150,333})&&t.sample(150,o,a)&&o.sword==1&&o.variant==7,"quick holster respects native sword contact event");
      t.clear();check(!t.push({100,1,1,7,334,333}),"expired quick holster cannot enter timeline");
      t.clear();check(t.push({100,1,2,6,100,333})''')])
s=(W/'Build-ShadowWalkBody-v524.ps1').read_text(encoding='utf-8-sig').replace('v524','v526').replace('Build524','Build526')
(W/'Build-ShadowWalkBody-v526.ps1').write_text(s,encoding='utf-8')
s=(W/'archive_shadowwalk_body_v524.py').read_text(encoding='utf-8-sig')
s=s.replace("A=W/'candidate-shadowwalk-body-v524'","A=W/'candidate-shadowwalk-body-v526'").replace("previous=W/'candidate-shadowwalk-inventory-v515'","previous=W/'candidate-shadowwalk-body-v524'")
s=s.replace("assert rd(R/'work/natural-moveset/native-shadowwalk-scenes-v522/result.json')['passed']", "assert not rd(R/'work/natural-moveset/native-shadowwalk-body-v525/result.json')['passed']")
s=s.replace("v524.log'", "v526.log'").replace('protocol=55','protocol=56')
s=s.replace("scope='Source522", "scope='525 failed on unrecognized native333ms fast sword holster before entry;526 adds observed Emily equipment7 with exact scene/owner/clock and actual native attachment state. Timeout unchanged. Source522")
s=s.replace("W/'prepare_shadowwalk_body_v524.py',W/'Build-ShadowWalkBody-v524.ps1'", "W/'prepare_shadowwalk_body_v524.py',W/'prepare_shadowwalk_holster_v526.py',W/'Build-ShadowWalkBody-v526.ps1'")
s=s.replace("replace('shadowwalk-inventory-v515','shadowwalk-body-v524').replace('ShadowWalkInventoryDeploy515','ShadowWalkBodyDeploy524')", "replace('shadowwalk-inventory-v515','shadowwalk-body-v526').replace('ShadowWalkInventoryDeploy515','ShadowWalkBodyDeploy526')")
s=s.replace("W/'Deploy-ShadowWalkBody-v524.ps1'", "W/'Deploy-ShadowWalkBody-v526.ps1'")
(W/'archive_shadowwalk_body_v526.py').write_text(s,encoding='utf-8')
s=(W/'Stage-ShadowWalkBody-v524.ps1').read_text(encoding='utf-8-sig').replace('archive_shadowwalk_body_v524','archive_shadowwalk_body_v526')
(W/'Stage-ShadowWalkBody-v526.ps1').write_text(s,encoding='utf-8')
print('Prepared bounded fast-holster526 repair')
