from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
take='swarm-native-standing-observe-v266';t=P/take;result=read(t/'result.json');assert result['passed'] and (t/'source-sheet.jpg').exists()
rows=read(t/'states.json');observations=read(t/'swarm-observations.json');scenes=read(t/'swarm-scenes.json')
assert all(x['sword']['equipped'] and x['player']['stance']==0 for x in rows)
summary=dict(result=result,observations=observations,rows=len(rows),swordDrawnSamples=len(rows),castingStates=[x['player']['active']for x in rows if any(a['id']==63 for a in x['player']['active'])],complete=False,visualAccepted=False)
write(t/'coverage-summary.json',summary)
review=f'{take}/visual-review.json';write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Viewed actual-game source temporal sheet1,2,2.8,3.8,5,8seconds',evidence=['source-sheet.jpg'],visual_accepted=False,complete=False,notes='Native targeting ring/preview appears while aiming; left-hand release spawns visible rats, which surround a guard. Native sword identity remains equipped in all recorded rows although the blade is out of this camera view. Original first-person acting retained. No peer body or remote swarm is present in this test. AI-held fixture does not establish general swarm behavior or synchronized damage. Source/partner health and original inventory retained, checkpoint retires spawned swarm, protected14 pass.'))
video=P/(take+'.mp4');probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,probe['streams'][0]['avg_frame_rate'].split('/'))
with video.open('rb')as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
req='corvo_devouring_swarm_cast';row=dict(video=video.name,character='corvo',label='Native Devouring Swarm source · standing armed · v266',alias='swarm_native_observation',requirements=[req],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=True,scope=result['scope'])
videos=read(P/'runtime-video-manifest.json');videos=[x for x in videos if x['video']!=video.name];videos.append(row);write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json');item=ann['items'].setdefault(req,{})
item.setdefault('native_integration',{})[video.name]=row;item['runtime_binding']='native_swarm_source_observed_remote_pending'
item['notes']='Native Corvo Swarm v266 passes5 checks: exact native inventory acquisition, armed standing state0+UsePower63, reciprocal arm/body entry400/loop333/out1600 scenes and visible native preview/rats. Mana observed100to80 after replenishment; not proof of network resource accounting. All original inventory/player/partner health, checkpoint cleanup and protected14 pass. Initial v265 run stopped before acquisition because the project command wrapper lacked the new bounded command; corrected and preserved. Authored body candidates, remote rats/VFX/sound, ownership/lifetime, partner exclusion and shared NPC damage remain unfinished.'
write(P/'review-evidence-annotations.json',ann)
write(W/'candidate-swarm-inventory-v265/runtime-verification.json',dict(at=datetime.datetime.now().astimezone().isoformat(),failedBeforeAcquisition=read(P/'swarm-native-standing-observe-v265/result.json'),sourceObservation=summary,complete=False))
print(json.dumps(dict(take=take,passed=True,videos=len(videos),rows=len(rows))))
