from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
take='swarm-network-standing-v270';t=P/take;result=read(t/'result.json');assert result['passed']
rows=read(t/'states.json');phases=read(t/'phase-incarnations.json');continuity=read(t/'continuity.json')
assert continuity['rejections']==[0] and len(phases)==3
summary=dict(result=result,nativePhases=[dict(variant=x['variant'],nativeScenes=len(x['nativeHandles']),sentEvents=len(x['sourceEvents']),acceptedEvents=len(x['acceptedEvents']),sampledPoseEvents=len(x['sampledPoseEvents']))for x in phases],continuity=continuity,complete=False,visualAccepted=False)
write(t/'coverage-summary.json',summary)
review=f'{take}/visual-review.json';assert all((t/(side+'-sheet.jpg')).exists()for side in ('source','peer'))
write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Viewed paired actual-game temporal sheets1,2.1,2.6,3.6,4.5,5.5seconds',evidence=['source-sheet.jpg','peer-sheet.jpg'],visual_accepted=False,complete=False,notes='Corvo visible from head to feet in peer camera: standing into broad supported full-body cast, sword carried in right hand, left hand casting, then standing recovery. Local native first-person aiming/release and rats visible in source view. Remote rat/target effects intentionally absent from body-only proof. Dark side lighting and temporal sheets do not establish fine grip, close-up hand alignment or subjective native-speed quality. AI-held fixture and bounded standing context only. Native scene handles and all9 sent/accepted phase events match;81 continuity samples body weight1 with rejection0. Both inventory quantities and player/partner health restore, checkpoint retires swarm, protected14 PASS.'))
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');req='corvo_devouring_swarm_cast';item=ann['items'].setdefault(req,{})
for source in (False,True):
 name=take+('-source'if source else'')+'.mp4';video=P/name
 probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));a,b=map(float,probe['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb')as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=name,character='corvo',label='Swarm · '+('native source'if source else'remote full-body')+' · standing armed · v270',alias='native_swarm_in',requirements=[req],gameplay_bound=not source,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=a/b,video_sha256=digest,checks_passed=True,scope=result['scope'])
 videos=[x for x in videos if x['video']!=name];videos.append(row);item.setdefault('native_integration',{})[name]=row
item['runtime_binding']='native_swarm_standing_armed_body_bound_world_pending'
item['notes']='Swarm v270 PASS10: native Corvo armed-standing entry400/loop333/release1600 drives complete-body bank49 through native Steam. Source recreates7 loop scenes; bounded diagnostics confirm all9 sent/accepted power events and native scene handles.81 body continuity samples weight1/rejection0, standing/sword recovery, no duplicated peer rats, inventories/health, checkpoint retirement and protected14 pass. Paired video sheets inspected, fine grip/native-speed visual acceptance still pending. v267 watcher parser failure, v268 one-loop assertion and v269 short-loop sampling ambiguity are preserved; v270 added diagnostic history without altering animation/packets. Crouch/holstered/movement/air/slide, remote rats/VFX/audio, partner exclusion and shared NPC outcomes remain unfinished.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
write(W/'candidate-swarm-history-v270/runtime-verification.json',dict(at=datetime.datetime.now().astimezone().isoformat(),standingArmed=summary,priorFailures=[read(P/f'swarm-network-standing-v{v}/result.json')for v in (267,268,269)],complete=False))
print(json.dumps(dict(take=take,passed=True,videos=len(videos),nativePhases=summary['nativePhases'],bodySamples=continuity['samples'])))
