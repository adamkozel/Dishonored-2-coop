from pathlib import Path
import json,subprocess,sys,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';A=W/'candidate-possession-crouch-release-v371'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
cases=[]
for v,passed,notes in [
 (371,False,'FAILED final harness assertion still requires standing despite observed crouched recovery.17 checks pass before failure, including owned release, Steam presentation, visibility, particles and recovered walking. Source native transfer/exit and peer particles viewed; close caster poorly framed until exit camera reframe. No complete visual acceptance.'),
 (372,True,'PASS20. Paired native game views show crouched caster body/left-arm gesture, empty-hand release, disappearance, target halo/streaks, reappearance and resumed crouched movement.30 native and7 peer crouched cast samples; release stance1.2 native dual-owned exit observations,7 remote draw samples,4 walking samples and0.354737m source displacement. Close view crops the legs during release; no complete anatomy/fine grip acceptance. Native first-person unchanged; NPC control/outcomes still not replicated.')]:
 take=f'possession-crouch-target-network-v{v}';T=P/take;r=read(T/'result.json');assert r['passed']==passed
 review=dict(inspected=True,method='Viewed paired temporal game sheets:371 at2.9/3.5/4.2/5/7.7/8.5s;372 at2.9/3.5/3.8/4.4/7.6/8.4s',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes=notes+' Both complete checkpoint inventories/health and protected14 restored. No audio.')
 (T/'visual-review.json').write_text(json.dumps(review,indent=2))
 subprocess.run([sys.executable,str(W/'record_reviewed_possession_take.py'),take,'--label',('crouched transfer372' if passed else 'FAILED final stance assertion371'),'--notes','371 code/bank54 verified by372 PASS20: crouched native cast/release, one clone hide/show cycle, target FX, dual sword1133/power333 exit and crouched walk.371 harness failure retained. Shared NPC authority, other contexts and complete visual approval remain unfinished.'],check=True)
 cases.append(dict(take=take,result=r,visualReview=review))
(A/'runtime-verification-v372.json').write_text(json.dumps(dict(cases=cases,complete=False),indent=2))
shutil.copy2(P/'Test-PossessionCrouchTarget-v372.ps1',A/'Test-PossessionCrouchTarget-v372.ps1')
x=read(A/'identity.json');x['runtimeVerified']=True;x['runtimeVerificationScope']='372 native crouched armed human-target lifecycle PASS20; no shared NPC gameplay or full visual acceptance.'
x['scope']=x['scope'].replace('runtime and visual validation pending.','runtime372 PASS20; broader context and full visual validation pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
p=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=p.read_text();h='## Crouched release/occupation/exit verified through Steam,372';assert h not in s
s+='\n\n'+h+'\n\n'+(
 '[371](../../work/natural-moveset/possession-crouch-target-network-v371/result.json) reaches17 checks then fails the final standing-only harness assertion. Production release, visibility, effects, dual draw and recovered crouch movement pass their checks. Failed evidence is retained.372 corrects the final assertion to require exact crouch1/default mode/motion-valid1 and adjusts only the observer camera to include the caster; the371 DLL, probe and bank are unchanged.\n\n'
 '[372 passes20 checks](../../work/natural-moveset/possession-crouch-target-network-v372/result.json). Native release remains crouched1, both native scenes and Steam event identities agree, one owned Corvo clone hide/show cycle covers transfer/internal1 and occupation/internal8, and exact owned particles start/update/stop. Native exit owns separate sword1133/power333 scenes;7 receiver draw samples and4 subsequent walking samples are observed, with0.354737m source displacement and no stream rearm. Both complete checkpoint inventories/health and protected14 hashes restore.\n\n'
 'The paired recordings and temporal sheets were viewed. They show the crouched body/casting arm, empty-hand release, disappearance, particles and reappearance followed by crouched movement. Close release framing crops the legs, so full anatomy and fine grip acceptance stay open. Remote NPC control/outcomes are still not replicated. [Archived runtime receipts](../../work/movement-integration/candidate-possession-crouch-release-v371/runtime-verification-v372.json).\n')
p.write_text(s)
print('371 failed assertion and372 PASS20 recorded; overall movement/co-op incomplete')
