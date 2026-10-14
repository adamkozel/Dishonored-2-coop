from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';F=N/'choke-network-emily-v589'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat();r=rd(F/'result.json');assert r['passed'] and len(r['checks'])==12 and (F/'observer-sheet.jpg').is_file()
note='Emily589 repeats all12 native/Steam/root/recovery checks successfully. Same-floor camera selection fixes the earlier586 recording. Actual observer temporal sheet viewed: full-body entry, held struggle, finish and standing recovery are visible. Receiver guard remains standing and independent; victim synchronization, contact refinement and visual acceptance remain unfinished.'
receipt=dict(at=at,passed=True,bodyVisuallyObserved=True,visualAccepted=False,complete=False,result=str((F/'result.json').relative_to(R)),priorReceipt='work/movement-integration/choke-verification-v587.json',dll=r['dll'],notes=note)
wr(W/'choke-camera-verification-v589.json',receipt);wr(W/'candidate-choke-network-v584/runtime-camera-verification-v589.json',receipt)
vs=rd(N/'runtime-video-manifest.json')
for view,file in (('source','client-rear-choke.mp4'),('observer','observer.mp4')):
 p=F/file;rel=p.relative_to(N).as_posix();assert not any(v['video']==rel for v in vs)
 review=dict(at=at,inspected=view=='observer',visualAccepted=False,complete=False,sheets=['observer-sheet.jpg'] if view=='observer' else [],observations=[note] if view=='observer' else ['Native source recording, no separate visual acceptance.'])
 rp=F/f'{view}-visual-review.json';wr(rp,review)
 m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
 vs.append(dict(video=rel,character='emily',label='Emily · rear choke · same-floor '+view,alias='nonlethal_takedown_front_back_sides',requirements=['nonlethal_takedown_front_back_sides'],gameplay_bound=True,complete=False,visual_accepted=False,evidence=F.name+'/result.json',visual_review=F.name+'/'+rp.name,proxy_sha256=r['dll'],video_sha256=hashlib.sha256(p.read_bytes()).hexdigest().upper(),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=True,scope=r['scope']+' '+note))
wr(N/'runtime-video-manifest.json',vs)
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['ChokeContinuation']['cameraReceipt']='work/movement-integration/choke-camera-verification-v589.json';s['ChokeContinuation']['latest']+=' '+note;s['MovementExecutionPriority']['current']=s['ChokeContinuation']['latest'];wr(p,s)
p=N/'review-evidence-annotations.json';s=rd(p);s['items']['nonlethal_takedown_front_back_sides']['notes']+=' '+note;wr(p,s)
print(json.dumps(dict(recordings=len(vs),bodyVisuallyObserved=True,visualAccepted=False)))
