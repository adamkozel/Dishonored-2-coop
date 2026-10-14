from pathlib import Path
import json,math
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
take='domino-beam-view-v225';r=read(P/take/'result.json');assert r['passed']
ep=read(P/take/'beam-endpoint-comparison.json')
p=R/'work/movement-integration/candidate-domino-links-v222/runtime-verification.json';v=read(p)
v['repeat']={'take':take,'result':r,'endpoints':ep,'nativeBeamVisibleSourceAndReceiver':True,'scope':'Inspected paired native beam visibility and retirement only; not complete power/NPC alignment acceptance.'}
v['scope']='v223 andv225 pass12 functional checks each;v225 supplies clear native source/receiver beam video. NPC authority, combat outcomes, additional power contexts and complete movement remain unfinished.'
write(p,v)
ann=read(P/'review-evidence-annotations.json');item=ann['items']['emily_domino_mark_link_cancel']
item['runtime_binding']='native_domino_body_marks_beam_partial_verified'
item['notes']='Standing body/mark flow passesv218; exact partner target exclusion passesv212. Native two-target beam reader passesv221. Candidate222 persistent native cosmetic beam+two marks via Steam passesv223 andv225 with12 checks each;v225 source and receiver beam are clearly visible and cleanup verified. NPC authority/alignment, shared outcomes, cancellation/final casts, additional stances/interruptions and complete visual acceptance remain unfinished.'
write(P/'review-evidence-annotations.json',ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=p.read_text(encoding='utf-8-sig')+f'''

## Clear paired beam recording, v225

The [v225 final receipt](../../work/natural-moveset/{take}/result.json) repeats all twelve functional checks on candidate222, now with a clear receiver camera. Inspected paired sheets show the native source beam and its receiver counterpart above the lower guard, with remote Emily in the foreground. The receiver effect disappears after the presentation stop while the source effect persists. This resolvesv223's obstructed camera evidence. [Independent endpoint comparison](../../work/natural-moveset/{take}/beam-endpoint-comparison.json) gives{math.sqrt(ep['startSquaredDistance']):.6f}m at the start and{math.sqrt(ep['endSquaredDistance']):.6f}m at the end; queries are sequential, so these are sampled spatial checks rather than synchronization latency measurements.

No duplicate beam or ownership refusal occurs, and both native checkpoints/inventories plus14 protected hashes pass. This validates bounded cosmetic beam presentation and native particle retirement. The apparent attachment to the receiver guard in this take does not establish synchronized NPC identity, movement, AI or damage. Body casting through the second source relocation was deliberately not streamed. `visualAccepted:false` remains appropriate for the overall Domino movement requirement; scoped beam visibility is separately recorded in [candidate222 verification](../../work/movement-integration/candidate-domino-links-v222/runtime-verification.json).

The next native test uses controller X cancellation while LT is held, with both marks already active and the peer body stream left on. This will distinguish source-owned native link cancellation from the presentation-only stop covered above. Unknown cancellation body subscenes remain observations until proven and mapped.
'''
p.write_text(s,encoding='utf-8')
p=R/'work/movement-integration/refresh_grenade_handoff_v58.py';s=p.read_text(encoding='utf-8-sig')
s=s.replace('Candidate222 adds Domino world2/1000-byte cosmetic beam presentation and passes identity/protocol/load checks; consult final paired take receipts for runtime status.','Candidate222 Domino world2/1000-byte cosmetic beam presentation passesv223 andv225 with12 checks each;v225 clearly shows the beam in both native game recordings and verified cleanup.')
s=s.replace('Candidate222 carries native observed beams in Domino world2 and passes synthetic/load checks; individual paired final receipts establish runtime status.','Candidate222 native beam presentation through Domino world2 passesv223 andv225 with12 checks each. Thev225 paired recording clearly shows the native effect in both instances and its independent retirement.')
p.write_text(s,encoding='utf-8')
print('Recorded paired beam visibility and repeat verification')
