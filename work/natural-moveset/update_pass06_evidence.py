"""Pin the selected pass-six evidence without promoting visual acceptance."""
import json,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
new=[('window-choke-v2-pose-test-2','window-choke-v2-roundtrip.json'),('top-transition-v1-pose-test-1','top-transition-v1-roundtrip.json'),('water-exit-v3-pose-test-1','water-exit-v3-roundtrip.json')]
path=P/'build_review_catalog.py';src=path.read_text();anchor=" for take,report in [('ammo-changes-v1-pose-test-1'"
if "('window-choke-v2-pose-test-2','window-choke-v2-roundtrip.json')"not in src:
 assert src.count(anchor)==1
 src=src.replace(anchor,' for take,report in ['+','.join(repr(v).replace(', ', ',')for v in new)+",('ammo-changes-v1-pose-test-1'")
 path.write_text(src)
path=P/'preserve_authoring_pass.py';src=path.read_text()
for test in ['window-v5-pose-test-1','slide-v4-pose-test-1','ammo-changes-v1-pose-test-1','window-choke-v2-pose-test-1','window-choke-v2-pose-test-2','top-transition-v1-pose-test-1','water-exit-v2-pose-test-1','water-exit-v3-pose-test-1']:
 anchor="'stance-pose-test-1','mixamo-v5-pose-test'"
 if f"'{test}',"not in src:src=src.replace(anchor,repr(test)+','+anchor)
for directory in ['item-research','transition-research','fp-window-body-samples','mixamo-heavy-pickup']:
 anchor="'item-models','item-mechanisms'"
 if f"'{directory}',"not in src:src=src.replace(anchor,repr(directory)+','+anchor)
for report in ['window-v5-roundtrip.json','slide-v4-roundtrip.json','ammo-changes-v1-roundtrip.json',*[v[1]for v in new]]:
 anchor="'gaits-corvo-2/result.json'"
 if f"'{report}',"not in src:src=src.replace(anchor,repr(report)+','+anchor)
path.write_text(src)
reports=['window-v5-roundtrip.json','slide-v4-roundtrip.json','ammo-changes-v1-roundtrip.json',*[v[1]for v in new]]
qa=dict(at=datetime.datetime.now().astimezone().isoformat(),complete=False,first_person_wrist_constraints=False,live_dll_changed=False,
 evidence=[dict(report=r,passed=json.loads((P/r).read_text())['passed'])for r in reports],
 visual_inspection={
 'window_lethal':'v5: braced stance, shoulder/spine action and partner pull are visible. Precise fingers/blade and native target physics remain unverified.',
 'window_nonlethal':'v2: boot stays inside neutral sill; paired entry/hold/finish/release recorded. End target state is native baked motion, not simulated ragdoll.',
 'crossbow_ammo':'Native regular/sleep models and exact tags are attached to native crossbow mechanism. Holster/type-change/equip is an authored presentation hypothesis. Reload phase clocks use native scene data.',
 'ladder_top':'v1: step over a neutral top platform and native standing recovery. Fine palm/toe contact and top descent entry still need work.',
 'water_exit':'v3: swim approach, edge hold, climb and planted standing recovery. Fixed low Corvo recovery in v2; neutral wall boot clearance and palms fitted without changing native lengths. No water simulation or live traversal binding.',
 'sliding_pair':'v4 now passes body publication; existing impact overlap and contact/recovery issues remain.'},
 failed_attempts=[dict(take='window-choke-v2-pose-test-1',reason='Lost command acknowledgement during concurrent rendering; not counted as a pass. Both review leases retired. Fresh serial take 2 passed.')],
 rejected_donors=[dict(source='mixamo-heavy-pickup/source-manifest.json',reason='Single-hand pickup is not shoulder hoist'),dict(source='Mixamo Carrying / Carrying Someone Idle',reason='Browser preview shows front cradling, not D2 shoulder carry; no download or promotion')],
 remaining='Recorded candidates are not complete animation coverage. Body lift/drop/throw, contextual variants, fine contact/weapon fit, animation lifecycle transitions and many gameplay bindings remain incomplete.')
(P/'quality-review-pass06.json').write_text(json.dumps(qa,indent=2));print('PASS06_EVIDENCE',len(reports))
