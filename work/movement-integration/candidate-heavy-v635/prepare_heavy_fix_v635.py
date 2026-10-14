from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';N=R/'work/natural-moveset'
shutil.copytree(S,W/'heavy-pre-v635')
def edit(name,a,b):
 p=S/name;s=p.read_text();assert a in s,(name,a);p.write_text(s.replace(a,b))
edit('native_player_combat_scene.h',
 'if(s.variant==18&&(s.subscene!=(s.character==1?"Scene_2":"Scene_1")||s.durationMs!=(s.character==1?1000u:833u)))return false;',
 '''// Corvo634 selected Scene_1 too: these are attack variants, not roles.
    if(s.variant==18&&!((s.subscene=="Scene_1"&&s.durationMs==833)||(s.subscene=="Scene_2"&&s.durationMs==1000)))return false;''')
edit('shadow_action_protocol.h','version=68,','version=69,');edit('shadow_action_protocol.h','p.version==68','p.version==69')
edit('shadow_action_protocol.h','p.combatDurationMs==(p.combatVariant==18?(p.character==1?1000u:833u):p.combatVariant==17?800u:p.combatVariant==15?866u:666u)',
 '(p.combatVariant==18?(p.combatDurationMs==833||p.combatDurationMs==1000):p.combatDurationMs==(p.combatVariant==17?800u:p.combatVariant==15?866u:666u))')
edit('world_identity_tests.cpp','auto wrong=heavy;wrong.str(names+0x400,"Scene_1");require(!combat(wrong),"other character heavy subscene refused");',
 '''auto other=heavy;other.str(names+0x400,"Scene_1");other.d(scene+0x28,833);require(combat(other)&&result.character==1,"Corvo634 selects native Scene_1 too");
            auto wrong=heavy;wrong.str(names+0x400,"Scene_1");require(!combat(wrong),"mismatched heavy subscene timing refused");''')
edit('world_identity_tests.cpp','"other character heavy duration refused"','"wrong heavy subscene duration refused"')
edit('shadow_action_protocol_tests.cpp','bad.version=67;check(!validShadowAction(bad,c,h),"previous combat wire refused")','bad.version=68;check(!validShadowAction(bad,c,h),"previous combat wire refused")')
edit('shadow_action_protocol_tests.cpp','heavy.combatDurationMs=ch==1?1000:833;','heavy.combatDurationMs=833;')
edit('shadow_action_protocol_tests.cpp','"character-specific crouched heavy release wire"','"native crouched heavy release wire"')
edit('shadow_action_protocol_tests.cpp','auto bad=heavy;bad.combatDurationMs=ch==1?833:1000;check(!validShadowAction(bad,context,peer),"wrong role heavy duration refused");',
 '''auto other=heavy;other.combatDurationMs=1000;check(validShadowAction(other,context,peer),"either native heavy variant duration accepted by wire");
       auto bad=heavy;bad.combatDurationMs=834;check(!validShadowAction(bad,context,peer),"unknown heavy duration refused");''')
for old,new in [('Build-Heavy-v633.ps1','Build-Heavy-v635.ps1'),('Deploy-Heavy-v633.ps1','Deploy-Heavy-v635.ps1')]:
 s=(W/old).read_text().replace('v633.log','v635.log').replace('Build633','Build635').replace('HeavyDeploy633','HeavyDeploy635').replace('stage_heavy_v633.py','stage_heavy_v635.py');(W/new).write_text(s)
s=(W/'stage_heavy_v633.py').read_text().replace("A=W/'candidate-heavy-v633';P=W/'candidate-kick-v625'","A=W/'candidate-heavy-v635';P=W/'candidate-heavy-v633'")
s=s.replace('v633.log','v635.log').replace('prepare_heavy_v633','prepare_heavy_fix_v635').replace('Build-Heavy-v633','Build-Heavy-v635').replace('Deploy-Heavy-v633','Deploy-Heavy-v635').replace('Test-CrouchedHeavy-v634','Test-CrouchedHeavy-v636').replace('protocol=68','protocol=69')
s=s.replace('role-specific scene/duration','exact subscene/duration pairs; Corvo634 disproves role-specific selection').replace('protocol68/bank67','protocol69/bank67');(W/'stage_heavy_v635.py').write_text(s)
s=(N/'Test-CrouchedHeavy-v634.ps1').read_text().replace('CrouchedHeavy634','CrouchedHeavy636').replace('candidate-heavy-v633','candidate-heavy-v635').replace('archived633','archived635')
s=s.replace("$expectedDuration=if($source-eq'host'){1000}else{833}","$expectedDuration=@($release.player.combatScene.durationMs|Sort-Object -Unique)")
s=s.replace('$_.player.combatScene.durationMs-ne$expectedDuration','$_.player.combatScene.durationMs-notin833,1000').replace('exact role-specific release','exact observed subscene release')
(N/'Test-CrouchedHeavy-v636.ps1').write_text(s)
print('Corrected heavy subscene selection: exact two timing pairs, no role assumption, existing633 bank retained.')
