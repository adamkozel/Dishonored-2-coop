from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
t=P/'domino-beam-network-v223';r=read(t/'result.json');assert r['passed']
v={'recorded':datetime.datetime.now().astimezone().isoformat(),'passed':True,'visualAccepted':False,'complete':False,'take':t.name,'result':r,'endpointComparison':read(t/'beam-endpoint-comparison.json'),'scope':'Persistent native beam and two marks replicated with exact receiver ownership, bounded endpoint comparison and cleanup. Source beam visible; receiver camera obstructed, corrected view pending. NPC matching/authority, actions/outcomes and complete Domino remain unfinished.'}
(R/'work/movement-integration/candidate-domino-links-v222/runtime-verification.json').write_text(json.dumps(v,indent=2),encoding='utf-8')
f=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=f.read_text(encoding='utf-8-sig')+'''

## First paired native beam proof, v223

[v223 final receipt](../../work/natural-moveset/domino-beam-network-v223/result.json) passes twelve checks and completes checkpoint cleanup on candidate222. The receiver starts exactly two marks plus one native beam, uses source FX282 as its opaque incarnation and local FX442 on its owned Emily avatar, updates it91 times, and has zero beam refusals. Both endpoint positions are independently read back: start separation0.2499m and end separation0.000149m across sequential, non-simultaneous source/receiver queries. The native target moves on the stairs during this observation. These bounds verify position following; they do not measure fine temporal alignment or NPC synchronization.

Receiver ownership stops by700ms. Its native beam record is already gone at the first949ms fade query; remaining marks finish by2509ms. No slot restarts or transform updates occur after stop. Both inventories/player health/checkpoints restore and all14 protected hashes pass. [Candidate222 runtime evidence](../../work/movement-integration/candidate-domino-links-v222/runtime-verification.json) records this bounded functional acceptance.

**Visual limitation:** the source beam is visible, but the receiver camera's3.5m lateral offset landed inside level geometry near the stairs. Its inspected recording does not establish visual beam quality. This functional pass is retained with `visualAccepted:false`; v225 changes only the harness camera offset and repeats the checks. The beam starts after both native source casts and source placement settle, so this test does not prove cast-phase continuity through traversal. v218 separately covers standing native body/mark flow.

**AI-hold precision:** the existing movement fixture owns decision/reflex suspension references. It does not freeze all in-flight movement or animation: v223's second guard continued down the stairs after selection. This explains changing mark coordinates and is another reason not to infer shared NPC state from cosmetic beam replication. No additional NPC mutation was introduced by the beam implementation.
'''
f.write_text(s,encoding='utf-8')
ann=read(P/'review-evidence-annotations.json');item=ann['items']['emily_domino_mark_link_cancel']
item['runtime_binding']='native_domino_body_marks_beam_partial_verified'
item['notes']='Standing armed body/mark flow passesv218; exact partner target exclusion passesv212. Native two-target beam reader passesv221. Candidate222 persistent cosmetic beam+two marks via native Steam passesv223 functional ownership/endpoint/retirement checks, but receiver camera is obstructed and visual rerun is pending. NPC authority/alignment, shared outcomes, cancel/final casts, other stances/interruptions and complete visual acceptance remain unfinished.'
(P/'review-evidence-annotations.json').write_text(json.dumps(ann,indent=2),encoding='utf-8')
print('Saved bounded beam runtime proof and visual limitation')
