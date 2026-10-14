from pathlib import Path
import datetime,hashlib,json
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
A=W/'candidate-shadowwalk-sneak-v550';identity=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
names=['native-shadowwalk-sneak-v551','native-shadowwalk-standing-v552','native-shadowwalk-sneak-clear-v554']
cases={name:rd(N/name/'result.json') for name in names}
for name,result in cases.items():
 assert result['passed'] and len(result['checks'])==14 and result['dll']==identity['dll'] and result['probe']==identity['probe']
 assert rd(N/name/'remote-body-observations.json')['clockRefused']==0
 fx=rd(N/name/'remote-fx-observations.json');assert fx['started']==fx['stopped'] and fx['refused']==0 and fx['nativeRecordsRetired']
 assert rd(N/name/'visual-review.json')['inspected']
at=datetime.datetime.now().astimezone().isoformat()
receipt=dict(at=at,candidate=A.name,dll=identity['dll'],probe=identity['probe'],protocol=58,bankVersion=59,clips=336,cases=cases,
 scope='Emily native standing/sneaking activation, fast sword holster, remote full-body entry, forward/reversed crawl cadence, requested crouched exit and six native particle resources through native Steam. Paired source/observer temporal sheets reviewed. Checkpoint/health/inventory/protected files restored.',
 repairedFailures=['547 test mistakenly required standing motion0 while crouched; corrected fixture guard.','547b reader refused native crouched phase2;548 adds exact observed case.','549 missed crouched333ms fast sword holster and retired semantic stream;550 adds exact equipment8, then551/552/554 pass.'],
 unresolved=['Shadow material remains ordinary human;544 rejected and disconnected.','Dedicated sideways/backward/turn clips and final contact quality.','Attack/victim outcomes, upgraded sprint, wall/rat-hole traversal and relevant interruptions.','509 airborne Doppelganger landing dropout and540 stream-start root freshness refusal.','Complete movement, shared world authority and playable co-op.'],visualAccepted=False,complete=False)
receipt['unresolved'].append('554 shows clearer entry/recovery but the body leaves the right frame edge during mid-travel; full travel is not visually certified.')
wr(A/'runtime-verification-v555.json',receipt)
wr(W/'shadowwalk-sneak-verification-v555.json',receipt)
wr(W/'candidate-shadowwalk-sneak-v548/runtime-limitations-v555.json',dict(failedTake='native-shadowwalk-sneak-v549',reason='Native crouched quick holster unrecognized; semantic stream retires before new entry.',resolvedBy='candidate-shadowwalk-sneak-v550',verification='shadowwalk-sneak-verification-v555.json',complete=False))
latest='550/551/552/554: Emily crouched and standing Shadow Walk entry, full-body crawl and requested crouched exit pass14 checks each through native Steam. Native crouched phase2 and trans_in_sneak/emily1000ms added; missing crouched fast sword holster repaired with exact equipment8/333ms. All six native particle resources follow/retire, no body-clock refusals, protected checkpoints/health/inventory retained. Paired temporal sheets viewed; clearer side view554 retained. Shadow material and full Shadow Walk coverage remain unfinished. Protocol58, bank59/336;544 material code disconnected.509 air landing and540 startup freshness remain open.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['ShadowWalkContinuation'].update(latest=latest,receipt='work/movement-integration/shadowwalk-sneak-verification-v555.json',candidate=A.name,materialComplete=False,complete=False,next='Continue functional Shadow Walk direction/sprint, attack/victim and interruption/traversal coverage, then remaining movement and shared NPC/world authority. Keep shader polish deferred with concrete findings. Do not resume rare partner-only edge cases; movement collision allowed. Preserve unresolved509/540 evidence.')
wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');ann['items']['emily_shadow_walk_enter_move_attack_exit'].update(notes=latest,complete=False);wr(N/'review-evidence-annotations.json',ann)
p=D/'36-native-shadow-walk-investigation.md';s=p.read_text(encoding='utf-8')
s+='''

## Verified crouched integration551, standing regression552 and clear view554

[551](../../work/natural-moveset/native-shadowwalk-sneak-v551/result.json), [552](../../work/natural-moveset/native-shadowwalk-standing-v552/result.json) and [554](../../work/natural-moveset/native-shadowwalk-sneak-clear-v554/result.json) each pass14 checks on candidate550. Source phases and reciprocal scenes, Steam-delivered full-body variants, native sword holster, distance-driven crawl, crouched recovery, six owned particle resources and exact effect retirement are verified in these contexts. No body-clock refusals occur. Both isolated checkpoints, health/inventory and all14 protected file hashes restore after each take. Timeout values remain unchanged.

Actual paired temporal sheets were viewed. The initial rear view is partly occluded by a pillar;554 changes only the observer position for a clearer side view. The source retains native first-person shadow vision and sword recovery. The body remains a human crawl representation, so this is scoped functional progress, not visual completion of the power. Continuous foot/hand contact quality, dedicated directional clips, proper shadow material, attacks, special traversal and broader interruptions remain incomplete. [Consolidated receipt555](../../work/movement-integration/shadowwalk-sneak-verification-v555.json) records pins, tests, repaired failures and remaining gaps. Candidate550 is retained for continued development; prior533/548 and rejected544 remain preserved. Normal partner movement collision is allowed; rare partner interaction work stays deferred as requested.
'''
s+='\nThe clearer554 side view still loses the body beyond the right frame edge during mid-travel (around6.8/9s). It improves entry/recovery inspection but does not certify continuous travel.\n'
p.write_text(s,encoding='utf-8')
print(json.dumps(dict(candidate=A.name,tests={k:len(v['checks']) for k,v in cases.items()},complete=False)))
