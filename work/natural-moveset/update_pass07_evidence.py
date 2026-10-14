"""Preserve review evidence without promoting prototypes to production."""
import json,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
new=[('trap-surface-v1-pose-test-1','trap-surface-v1-roundtrip.json'),('carry-release-v3-pose-test-1','carry-release-v3-roundtrip.json'),('ladder-top-entry-v1-pose-test-1','ladder-top-entry-v1-roundtrip.json'),('body-lift-v2-pose-test-1','body-lift-v2-roundtrip.json'),('body-throw-v1-pose-test-1','body-throw-v1-roundtrip.json')]
path=P/'preserve_authoring_pass.py';s=path.read_text()
for take,report in new:
 if repr(take)+','not in s:s=s.replace("'stance-pose-test-1','mixamo-v5-pose-test'",repr(take)+",'stance-pose-test-1','mixamo-v5-pose-test'")
 if repr(report)+','not in s:s=s.replace("'gaits-corvo-2/result.json'",repr(report)+",'gaits-corvo-2/result.json'")
if "'trap-wall-emily-video-1',"not in s:s=s.replace("'blink-assault-emily-video-1'","'trap-wall-emily-video-1','blink-assault-emily-video-1'")
path.write_text(s)
qa=dict(at=datetime.datetime.now().astimezone().isoformat(),complete=False,first_person_wrist_constraints=False,live_dll_changed=False,
 evidence=[dict(report=report,passed=json.loads((P/report).read_text())['passed'])for _,report in new],
 visual_inspection={
 'trap_wall':'Native stun-mine and spring-razor props reach and detach onto a neutral wall. Exact native phase/fire timing is retained. Fine finger/blade alignment and real gameplay placement remain pending. Emily D2 body-only footage confirms free torso/arm motion and recovery, with no trap spawned.',
 'carry_release':'v3 corrects high-release shoulder reach and begins low-drop weight transfer earlier. Native targets freeze at the exact ragdoll boundaries; the caption explicitly identifies absent physical continuation.',
 'ladder_top_entry':'v1 reverses the contacted top climb at a slower pace. Standing, bracing and transfer onto the ladder are visible. Direction-specific downward weight and fine rail/rung contact need refinement.',
 'body_lift':'v2 uses complete-body lowering, two-hand gathering, leg drive, shoulder loading and stepping recovery. Corrected v1 knee folding and foot sliding. Head/target overlap during gathering, exact hand surface fit and weight transfer still need revision. No live NPC attachment, damage or physics.',
 'body_throw':'v1 has loaded wind-up, stepping drive, hand release and armed recovery. Target follows a labelled illustrative arc and freezes at first floor contact. Stronger anticipation/follow-through, final target settling and actual native physics remain pending.'},
 rejected_or_superseded=[dict(path='body-lift-videos-v1',reason='Collapsed knee direction, excessively low hip and sliding recovery; replaced for review by v2'),dict(path='carry-release-videos-v2',reason='High-release supporting arm out of reach by about 16cm; replaced by v3')],
 research=dict(elixirs='Exact inventory definitions confirm health/mana quantities and native drink sounds. Sampled animation catalog contains NPC flask donor. No confirmed player potion animation or hand-held mana bottle yet; do not treat name matching as proof. Ghidra _quickPotion xref 1406745d4 is a binding/registration lead, not consumption implementation.'),
 remaining=['Authoring candidates still need visual revision and feedback.','Aerial/sliding contact and several paired-action body intersections are known issues.','Elixir grip/prop and actual native use behavior remain unverified.','No-mana reactions, arbitrary interruption, power upgrades, native Shadow Walk morphology and contextual body-size variants remain incomplete.','Native lifecycle/physics, item and world-action binding are separate uncompleted integration work.'],
 retained_milestone='milestone-CF60',review='review.html',browser_retention='Automatic approval review rejected marking unfinished browser tabs for retention; local artifacts retained instead.')
(P/'quality-review-pass07.json').write_text(json.dumps(qa,indent=2));print('PASS07_EVIDENCE',len(new))
