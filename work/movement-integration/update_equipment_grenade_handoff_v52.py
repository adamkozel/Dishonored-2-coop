"""Append scoped continuation state without changing old milestone evidence."""
import datetime,json
from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
state=read(W/'current-task-state.json');m=state['MovementIntegration']
eq=read(P/'equipment-review-both-v48-v4/result.json');assert eq['passed'] and len(eq['checks'])==15
cases={name:read(P/name/'result.json') for name in ['failure-fx-receiver-stop-v51','failure-fx-source-resume-v51','failure-fx-receiver-resume-v51']}
assert all(r['passed'] and len(r['checks'])==8 for r in cases.values())
guard={name:read(P/name/'result.json') for name in ['bendtime-corvo-crouch-guard-v50','bendtime-corvo-crouch-guard-moving-v50']}
assert all(r['passed'] for r in guard.values())
m['EquipmentReviewV48']=dict(result=eq,archive_harness='natural-moveset/equipment-review-both-v48-v4/run-script.ps1',visual_accepted=False,failed_takes_retained=True)
m['CrouchCastGuardV50']=dict(preferred_candidate=True,standard_bank_unchanged=True,manifest='natural-moveset/crouch-power-guard-bank-manifest-v50.json',cases=guard,visual_scope='Actual-game temporal sheets improved over rejected v49 wrist-only correction; close contact and continuous quality remain open',complete=False)
m['FailureFxLifecycleV51']=dict(cases=cases,code='candidate-network-failure-fx-v48',bank='outofmana-context-bank-manifest-v46.json',remaining='Temporary avatar invisibility during pause recovery; death/load/disconnect and arbitrary contexts unverified',complete=False)
m['GrenadeReaderV52']=dict(archive='movement-integration/candidate-grenade-reader-v52',evidence='movement-integration/grenade-research',manifest='outputs/d2-coop-lab/research/asset-exports/native-grenade-v52/manifest.json',read_only_both_roles=True,gameplay_deployed=False,next='Bounded native equip/unequip fixture; exact scene/timing observation before remote binding',complete=False)
m['LatestContinuation'].update(actual_game_videos=len(read(P/'runtime-video-manifest.json')),next='Native grenade equip/unequip observation and remote movement binding, plus remaining full moveset',in_progress='Grenade native identity and scenes; scoped failure lifecycle and equipment recording complete, movement and playable co-op incomplete',complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(W/'current-task-state.json',state)
f=D/'01-current-state.md';s=f.read_text(encoding='utf-8');mark='<!-- EQUIPMENT-GRENADE-V52 -->'
section=f'''{mark}
## Equipment, sword guard and grenade continuation

Updated {state['UpdatedAt']}. Live code and deployed probe still use the v48 pins above. The separate [v52 read-only probe](../../work/movement-integration/candidate-grenade-reader-v52/identity.json) has verified the baseline native grenade identities for both roles, without gameplay deployment.

Both-role sword equip/holster and unarmed travel recording passes fifteen scoped checks. The [v50 crouched cast guard](17-equipment-and-cast-clearance.md) passes stationary and moving tests, and is the preferred comparison candidate over rejected v49; standard failure tests still use v46 banks. Three [actual failure-FX interruption/recovery cases](16-power-failure-and-resume.md) pass eight checks each. The temporary pause visibility gap remains visible.

The dashboard indexes 408 actual-game videos and 105 requirements; those counts are not movement completion. [Native grenade work](18-native-grenade-investigation.md) continues with an equip-only fixture before animation binding. The playable-co-op goal remains active beyond movement.
<!-- /EQUIPMENT-GRENADE-V52 -->'''
if mark in s:
 end='<!-- /EQUIPMENT-GRENADE-V52 -->';s=s[:s.index(mark)]+section+s[s.index(end)+len(end):]
else:s+='\n\n'+section+'\n'
f.write_text(s,encoding='utf-8')
print('Updated scoped equipment, guard, FX lifecycle and grenade reader state')
