from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src';B=W/'shadowwalk-fx-pre-v533';B.mkdir(exist_ok=False)
def edit(name,pairs):
 p=S/name;shutil.copy2(p,B/name);s=p.read_text(encoding='utf-8-sig')
 for a,b in pairs:
  assert s.count(a)==1,(name,a);s=s.replace(a,b)
 p.write_text(s,encoding='utf-8')
edit('native_power_fx.h',[
 ('        kind==22?"models/effects/d2coop/possession_target_remote.fx":', '''        kind==22?"models/effects/d2coop/possession_target_remote.fx":
        kind==23?"models/effects/gameplay/powers/shadow_walk/ply_arms/idle/ply_arm_idle_01.fx":
        kind==24?"models/effects/gameplay/powers/shadow_walk/ply_arms/idle/ply_hand_idle_01.fx":
        kind==25?"models/effects/gameplay/powers/shadow_walk/ply_arms/loco/ply_arm_loco_01.fx":
        kind==26?"models/effects/gameplay/powers/shadow_walk/ply_arms/loco/ply_arm_loco_02.fx":
        kind==27?"models/effects/gameplay/powers/shadow_walk/ply_arms/loco/ply_hand_loco_01.fx":
        kind==28?"models/effects/gameplay/powers/shadow_walk/ply_arms/transformation/ply_arm_ashes_01.fx":''')])
edit('engine_bridge.cpp',[
 ('#include "native_power_fx.h"','#include "native_power_fx.h"\n#include "shadowwalk_fx.h"'),
 ('serviceNativePowerFx(tick);servicePowerFxStream(tick);','serviceShadowWalkFx(tick);serviceNativePowerFx(tick);servicePowerFxStream(tick);'),
 ('    writeNativePowerFx();','    writeNativePowerFx();writeShadowWalkFx();')])
s=(W/'Build-ShadowWalkBody-v526.ps1').read_text(encoding='utf-8-sig').replace('v526','v533').replace('Build526','Build533')
(W/'Build-ShadowWalkFx-v533.ps1').write_text(s,encoding='utf-8')
s=(W/'archive_shadowwalk_body_v526.py').read_text(encoding='utf-8-sig')
s=s.replace("candidate-shadowwalk-body-v526'","candidate-shadowwalk-fx-v533'",1).replace("previous=W/'candidate-shadowwalk-body-v524';probe=W/'candidate-shadowwalk-probe-v522'","previous=W/'candidate-shadowwalk-body-v526';probe=previous")
s=s.replace("assert not rd(R/'work/natural-moveset/native-shadowwalk-body-v525/result.json')['passed']","assert rd(R/'work/natural-moveset/native-shadowwalk-travel-v532/result.json')['passed']")
s=s.replace('v526.log','v533.log').replace("scope='525 failed", "scope='533 adds native arm/hand idle/movement/ashes FX placed on the current published peer skeleton with350ms renewable lease and native record identity. No screen effects, shader changes, damage or power calls. Candidate runtime/visual acceptance pending. Body527/529/532 pass. 525 failed")
s=s.replace("W/'Build-ShadowWalkBody-v526.ps1'", "W/'Build-ShadowWalkFx-v533.ps1',W/'prepare_shadowwalk_fx_v533.py'")
s=s.replace("'shadowwalk-body-v526').replace('ShadowWalkInventoryDeploy515','ShadowWalkBodyDeploy526')", "'shadowwalk-fx-v533').replace('ShadowWalkInventoryDeploy515','ShadowWalkFxDeploy533')")
s=s.replace("W/'Deploy-ShadowWalkBody-v526.ps1'", "W/'Deploy-ShadowWalkFx-v533.ps1'")
(W/'archive_shadowwalk_fx_v533.py').write_text(s,encoding='utf-8')
s=(W/'Stage-ShadowWalkBody-v526.ps1').read_text(encoding='utf-8-sig').replace('archive_shadowwalk_body_v526','archive_shadowwalk_fx_v533')
(W/'Stage-ShadowWalkFx-v533.ps1').write_text(s,encoding='utf-8')
print('Prepared native cosmetic particles533; not built or deployed')
