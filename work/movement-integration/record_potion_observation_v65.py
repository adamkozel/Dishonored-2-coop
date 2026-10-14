"""Index inspected source-only consumable evidence; no remote binding claim."""
import argparse,datetime,json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
a=argparse.ArgumentParser();a.add_argument('take');a.add_argument('--notes',required=True);args=a.parse_args()
t=P/args.take;r=read(t/'result.json');sheet=t/'source-sheet.jpg';assert sheet.exists()
review=f'{args.take}/visual-review.json'
network=r.get('networkContinuity',False)
if network:assert (t/'native-sheet.jpg').exists()
write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Inspected actual-game temporal sheets',evidence=[sheet.name]+(['native-sheet.jpg'] if network else []),visual_accepted=False,complete=False,notes=args.notes))
video=P/(args.take+'.mp4');assert video.exists()
meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
with video.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
req=f"{r['kind']}_elixir"
row=dict(video=video.name,character='corvo' if r['source']=='host' else 'emily',label=f'Native elixir source observation · {args.take}',alias=req+'_native_observation',requirements=[req],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{args.take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=r['passed'],scope=r['scope'])
videos=[v for v in read(P/'runtime-video-manifest.json') if v['video']!=video.name];videos.append(row);write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json');item=ann['items'][req];item.setdefault('native_integration',{})[video.name]=row
if network:
    peer=P/(args.take+'-receiver.mp4');assert peer.exists()
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(peer)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
    with peer.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
    receiver={**row,'video':peer.name,'label':f'Remote movement during native {r["kind"]} use · {args.take}','alias':r['kind']+'_use_movement_continuity','actual_capture_hz':n/d,'video_sha256':digest,'gameplay_bound':bool(r['passed'])}
    videos=[v for v in videos if v['video']!=peer.name];videos.append(receiver);write(P/'runtime-video-manifest.json',videos)
    item['native_integration'][peer.name]=receiver
if r['passed']:
    network_verified=any(v.get('checks_passed') and v.get('gameplay_bound') and v.get('alias','').endswith('_use_movement_continuity') for v in item['native_integration'].values())
    item['runtime_binding']=f'native_network_movement_during_{r["kind"]}_use_partial_verified' if network_verified else 'native_source_consumption_observed_remote_pending'
    item['notes']='Native source consumption observed with inventory, resource and checkpoint restoration checks. '+('Recorded Steam receiver walking/stopping remains continuous through native consumption. ' if network_verified else '')+'No FP drinking gesture is visible after wheel closure in the inspected source recordings. The queued native event is a drink sound, not a body scene; wheel logic suppresses it. Authored drinking clip remains unused; remote drink sound and resource synchronization remain unimplemented. Fine visual quality and untested actions are not accepted.'
write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(take=args.take,passed=r['passed'],source_only=not network,videos=len(videos))))
