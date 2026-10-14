from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';take='possession-visibility-network-v339';T=P/take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
r=read(T/'result.json');e=read(T/'visibility-evidence.json');st=read(T/'occupied-read-stability.json')
assert r['passed'] and len(r['checks'])==14 and e['hiddenSamples']==21 and st['stableOccupied']==16
review=dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Viewed source and peer temporal contact sheets at .8,2.4,3.2,3.8,4.5,5.5,6.3,7,8.5 seconds',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes='Peer shows Corvo casting and holstering, then disappearance by3.8s and absence during occupied4.5/5.5s. Returned body position is occluded or outside the camera; exactly one native show/readback is established numerically, not visually. Source shows native host vision and normal exit. No audio. Remote NPC control, outcome authority, world VFX, exit movement continuity and interrupted visibility remain unverified.')
write(T/'visual-review.json',review)
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_possession_enter_loop_exit']
for suffix,label in [('', 'remote body hides during occupation'),('-source','native human occupation and exit')]:
 v=P/(take+suffix+'.mp4');info=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(v)],text=True));n,d=map(float,info['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=v.name,character='corvo',label='Possession · '+label+' · v339',alias='native_possession_occupied',requirements=['corvo_possession_enter_loop_exit'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=f'{take}/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=hashlib.sha256(v.read_bytes()).hexdigest().upper(),checks_passed=True,scope=review['notes'])
 videos=[x for x in videos if x['video']!=v.name];videos.append(row);item.setdefault('native_integration',{})[v.name]=row
item['notes']='339 passes14 on DLL338/protocol50/bank52: exact human transfer/occupation drives finite Corvo-only clone hide lease,21 hidden observations including16 bracketed stable occupied samples,one hide/show,zero visibility refusals/losses. Peer disappearance viewed; exit flag restored but returned body occluded in this camera. Original native first person retained; remote NPC control/world VFX/outcomes/exit movement recovery remain incomplete. All inventories/health restored by checkpoint; protected14 unchanged.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
write(W/'candidate-possession-release-body-v338/runtime-verification.json',dict(result=r,visibility={k:v for k,v in e.items() if k!='hiddenRows'},occupied=st,visualReview=review,complete=False))
bookmarks=read(R/'work/handoff-project-bookmarks-current.json');add=read(W/'ghidra-bookmarks-additions-v341.json');by={(x['address'],x['category']):x for x in bookmarks}
for x in add:by[x['address'],x['category']]=x
write(R/'work/handoff-project-bookmarks-current.json',sorted(by.values(),key=lambda x:(x['address'],x['category'])))
f=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=f.read_text()
heading='## Occupied-body visibility verified in339'
if heading not in s:
 s+='''

## Occupied-body visibility verified in339

[339 passes14 checks](../../work/natural-moveset/possession-visibility-network-v339/result.json), using archived DLL338, unchanged probe330, protocol50 and bank52/316. The finite cosmetic lease hides only the exact remote Corvo clone during validated human transfer and occupation. There are21 hidden samples, including16 independently bracketed stable occupied observations; the native hide/show calls each occur once, with no visibility refusal, lost restoration or local/Emily visibility call. The native human target remains untouched by the cosmetic implementation.

[The visual review](../../work/natural-moveset/possession-visibility-network-v339/visual-review.json) confirms disappearance in the peer view. The return position is occluded/outside the camera, so native visibility-flag restoration does **not** yet establish a convincing visible return. Source first-person host vision and native exit remain intact. Audio is absent. Remote NPC authority, host outcome replication, world VFX and uninterrupted post-exit player movement remain unfinished.

The source Possession component is unselected/stopped during transfer but unselected/**started** during occupation.336 incorrectly required stopped throughout;338 corrects this using the exact native internal-state distinction. Invalid exit-boundary reads are reported separately, never counted as zero inventory quantity. Both complete inventories and health restore through checkpoint; protected14 hashes remain unchanged.

340 stopped before casting because the native AI isolation lease had refused an attacking guard.341 adds explicit isolation-readiness evidence; it stopped earlier because the scripted guard stayed outside the measured seven-metre fixture. These failed setups are preserved.342 tries bounded player-only floor measurements around the exact existing guard after acquiring safe AI references; it never relocates the NPC. Safety and health checks remain required.
'''
f.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(videos),bookmarks=len(by),passed=14)))
