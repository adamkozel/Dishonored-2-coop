from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
p=R/'work/movement-integration/candidate-domino-partner-v211/runtime-verification.json';v=read(p)
take='domino-guard-feedback-v218';r=read(R/'work/natural-moveset'/take/'result.json');assert r['passed']
v['takes'][take]=r;v['paired_mark_regression_with_candidate211_verified']=True
v['scope']='Exact partner excluded; ordinary two-guard native linking and paired single-guard body/mark flow verified. No shared NPC outcome, beam replication, NPC visual alignment or complete power claim.'
write(p,v)
p=R/'work/handoff-project-bookmarks-current.json';old=read(p);new=read(R/'work/ghidra-bookmarks-export-v220.json')
merged={(b['address'],b['category'],b.get('type','Note')):b for b in old}
for b in new:merged[(b['address'],b['category'],b.get('type','Note'))]=b
write(p,sorted(merged.values(),key=lambda b:(b['address'],b['category'])))
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=p.read_text(encoding='utf-8-sig')+'''

## Paired normal-enemy regression passed, v218

[v218 final receipt](../../work/natural-moveset/domino-guard-feedback-v218/result.json) passes twelve checks on candidate211 with read-only probe217. The harness selects the exact tutorial guard2 and independently verifies native view yaw against his current position before capture. Its two camera samples already matched within0.039degrees, so this take does not yet validate the helper's corrective joystick branch. Three floor samples were stable before stream startup. All four native/remote cast phases agree;66 continuity samples retain weight1.0 with zero source clock rejections. The partner hook forwarded the normal enemy without an ownership failure. Exactly one native receiver mark is created and follows the source transform. Owner retirement occurs within700ms after stream stop; the native record is absent2.508seconds after stop in the recorded timeline. Both checkpoints/inventories and all14 protected hashes pass final cleanup.

Source and receiver temporal sheets are inspected and indexed. Native first-person casting is preserved; the remote full-body phase sequence and persistent mark are visible. The chandelier/railings obscure part of the distant enemy and the remote character is near the right edge. This is functional evidence, not final animation/grip or synchronized enemy-attachment visual acceptance. [Candidate211 runtime verification](../../work/movement-integration/candidate-domino-partner-v211/runtime-verification.json) now records both partner exclusion and normal guard regression as passed.

v217's new connection reader accepted native one-target/zero-connection snapshots, but the take failed before linking because the second guard was atZ65.1569 on the stairs rather than downstairs atZ61.0278. The final receipt and inspected source video are retained. v219 expands only the test's bounded patrol observation and uses a separately proven source anchor for the second cast, with no stream active. Endpoint-reader validation remains pending that take. Source changes for world protocol2 and cosmetic beam presentation are under development and are not yet deployed; candidate211 remains live.
'''
p.write_text(s,encoding='utf-8')
print('Updated candidate211 paired acceptance and merged',len(merged),'bookmarks')
