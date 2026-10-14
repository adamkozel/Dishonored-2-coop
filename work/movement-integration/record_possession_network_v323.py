"""Index the inspected two-instance empty-target test; no target-control claim."""
from pathlib import Path
import datetime, hashlib, json, subprocess
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'; W=R/'work/movement-integration'
take='possession-network-v323'; t=P/take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
r=read(t/'result.json'); phases=read(t/'phase-incarnations.json'); continuity=read(t/'continuity.json')
assert r['passed'] and len(phases)==2 and continuity['rejections']==[0]
assert all((t/(s+'-sheet.jpg')).is_file() for s in ('source','peer'))
now=datetime.datetime.now().astimezone().isoformat()
review=dict(inspected=True,at=now,method='Viewed source and peer temporal sheets at0.8/2.1/2.3/2.8/3.5/4.2/6s',evidence=['source-sheet.jpg','peer-sheet.jpg'],visual_accepted=False,complete=False,notes='Native source left hand rises into aim and lowers after empty-target release. Remote Corvo raises the left arm with a whole-body weight shift, retains the right-hand sword, then recovers to standing. Dark lighting and small on-screen body limit fine grip and deformation assessment. No target transfer, disappearance/reappearance, VFX, sound or shared target outcome established; contact sheets do not establish native-speed subjective acceptance.')
write(t/'visual-review.json',review)
summary=dict(result=r,nativePhases=[dict(variant=p['variant'],nativeScenes=len(p['nativeHandles']),sentEvents=len(p['sourceEvents']),acceptedEvents=len(p['acceptedEvents'])) for p in phases],continuity=continuity,visualReview=review,complete=False)
write(t/'coverage-summary.json',summary)
videos=read(P/'runtime-video-manifest.json'); ann=read(P/'review-evidence-annotations.json'); req='corvo_possession_enter_loop_exit'; item=ann['items'][req]
for source in (False,True):
 video=P/(take+('-source' if source else '')+'.mp4')
 info=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True)); n,d=map(float,info['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='corvo',label='Possession · '+('native source' if source else 'remote full-body')+' · empty target · v323',alias='native_possession_in',requirements=[req],gameplay_bound=not source,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=f'{take}/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=True,scope=r['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
item['runtime_binding']='native_empty_target_entry_aim_remote_body_verified_target_pending'
item['notes']='323 passes10 two-instance checks: exact native333ms entry and200ms aim incarnations drive Corvo remote full-body bank51/315 over Steam protocol48. Body weight remains1 across75 samples with zero clock rejections. Empty-target retirement recovers standing; peer acquires no power and executes no native transfer. Native inventory rollback, both isolated checkpoints and protected14 retained. Valid-target transfer, visibility/reappearance, VFX, alternate contexts, shared target authority and final visual acceptance remain pending.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
write(W/'candidate-possession-body-v323/runtime-verification.json',summary)
doc=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md'
s=doc.read_text(encoding='utf-8');heading='## Remote empty-target body binding, candidate323'
if heading not in s:
 s+='\n\n'+heading+'\n\n'+item['notes']+'\n\n[Runtime and visual receipt](../../work/movement-integration/candidate-possession-body-v323/runtime-verification.json) pins the exact DLL, probe, bank and observed phase/event counts. Both source and peer recordings are indexed in the dashboard; neither is marked complete or visually accepted. Bank51 appends two full-body studies while preserving all313 earlier clip payloads. Protocol48 permits variants26–27 only for grounded Corvo with the sword drawn; old protocol47, Emily, holstered, airborne and unobserved release are refused. Build, protocol guards and both actual315-clip banks passed validation before deployment. Native animation events and root displacement are not executed on the peer.\n\n[Component-reference helpers](../../work/movement-integration/possession-component-reference-decompile-v323.txt) establish a generation at equipment+B8 and component pointer at+C0: native resolution compares component+18 generation and checks component activity. This is static evidence only; target ownership still requires runtime validation.\n'
 doc.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(videos),nativePhases=summary['nativePhases'],continuity=continuity)))
