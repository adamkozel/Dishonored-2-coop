from pathlib import Path
import json,subprocess,sys
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';A=W/'candidate-possession-exit-v343'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
notes='DLL343: normal human Possession exit and native backward walking pass343/345(15 checks each), receiver-stop344 and native receiver/source pause347 pass14 each. Exact clone visibility restores once; streams remain deliberately disarmed after interruption.345 visibly shows masked Corvo returning, but native guard overlap obscures later movement. Close full-body staging, remote NPC authority, target VFX/outcomes and alternate hosts remain unfinished.346 failed a test timing condition; retained without relabeling. Complete checkpoint inventories/health and protected14 restore in all completed cases.'
cases=[
 ('possession-exit-close-v345','close exit and movement v345',15,'1,2.6,4.4,6.4,7.9,9.8','Peer shows masked Corvo casting behind the guard, disappearing during occupation, and visibly returning at6.4s. The guard blocks much of the view and overlaps Corvo during the later half-metre backward step. Source shows untouched native cast/host vision/equip/exit. This supports visible return plus numerical movement continuity, but not a polished complete-body sequence or synchronized host NPC.'),
 ('possession-receiver-pause-v347','native receiver pause v347',14,'1,3.4,5.1,5.7,7.8,9.8','Peer shows the native Emily pause menu at5.1s and native resume by5.7s. Source remains in native occupied vision through that pause/resume and later exits by normal input. Remote return area remains obscured/overlapped with the native guard; exact visibility cleanup is supported by independent telemetry, not complete visual acceptance.'),
 ('possession-source-pause-v347','native source pause v347',14,'1,3.4,5.1,5.7,7.8,9.8','Source shows the native Corvo pause menu at5.1s, resumes occupied vision, then exits normally. Peer continues rendering and later camera reframe shows the host area. A distinct full-body restored Corvo is not clearly framed; native visibility readback and stream retirement are independently verified. This is lifecycle cleanup evidence, not finished host/world presentation.')]
receipts={}
for take,label,count,times,visual in cases:
 T=P/take;r=read(T/'result.json');assert r['passed'] and len(r['checks'])==count
 review=dict(inspected=True,method='Viewed both temporal contact sheets at'+times+'seconds',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes=visual+' No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented.')
 write(T/'visual-review.json',review)
 subprocess.run([sys.executable,str(W/'record_reviewed_possession_take.py'),take,'--label',label,'--notes',notes],check=True)
 receipts[take]=dict(result=r,visualReview=review)
 if 'pause' in take:
  e=read(T/'interruption-evidence.json');receipts[take]['interruption']={k:v for k,v in e.items() if k!='after'}
 else:receipts[take]['continuity']=read(T/'exit-continuity.json')
write(A/'runtime-continuation-v347.json',dict(cases=receipts,complete=False))
bookmarks=read(R/'work/handoff-project-bookmarks-current.json');by={(x['address'],x['category']):x for x in bookmarks}
for x in read(W/'ghidra-bookmarks-additions-v345.json'):by[x['address'],x['category']]=x
write(R/'work/handoff-project-bookmarks-current.json',sorted(by.values(),key=lambda x:(x['address'],x['category'])))
f=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=f.read_text();heading='## Close exit view and native menus,345–347'
if heading not in s:s+='''

## Close exit view and native menus,345–347

[345 passes15](../../work/natural-moveset/possession-exit-close-v345/result.json): two owned paired draw observations, five remote draw samples,17 recovered rows, five walking samples and0.483628m native backward displacement. Both semantic streams remain ready without rearm. [The close video](../../work/natural-moveset/possession-exit-close-v345/visual-review.json) shows Corvo's masked body returning at6.4s; a native guard blocks much of the cast and overlaps the returned player. The body lifecycle and movement continue, but host/world presentation is incomplete.

[Receiver pause347](../../work/natural-moveset/possession-receiver-pause-v347/result.json) and [source pause347](../../work/natural-moveset/possession-source-pause-v347/result.json) pass14 checks each. Actual native menus appear in their source/peer recordings. Visibility is restored while paused and both cases retain the retired receiver stream after resume. Receiver pause provides eight pre-interruption occupied/hidden samples and22 restored/disarmed rows afterward, including seven while the source is still possessed. Source pause provides six baseline samples and23 restored/disarmed rows, including seven after resume while still possessed. Normal native exit, full checkpoint inventories/health and protected14 checks pass.

346 remains a failed run: its pause cleanup succeeded, but the script issued leave-host input immediately after resuming, leaving no sampled post-menu occupied interval.347 preserves an1800ms interval after menu entry before exit, retaining all assertions. This is a harness timing correction; no production binary changed between343 and347. Source-pause views still do not clearly frame a distinct returned Corvo; the native visibility assertion is not promoted to complete visual acceptance.

[Probe348 research](../../work/movement-integration/candidate-possession-fx-research-v348/identity.json) keeps production DLL343 and bank52/316 unchanged. Its external read-only watcher inventories exact Possession FX and correlates candidate controller slots, avoiding a new broad scan on the game's render/update thread. Native host/camera effects must be distinguished from suitable remote target effects before playback is introduced. Research runtime results remain separately attributed.
'''
f.write_text(s,encoding='utf-8');print(json.dumps(dict(bookmarks=len(by),cases=len(cases))))
