"""Index actually inspected Domino source evidence; no remote/completion claim."""
import argparse,datetime,json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
a=argparse.ArgumentParser();a.add_argument('take');a.add_argument('--notes',required=True);args=a.parse_args()
t=P/args.take;r=read(t/'result.json');assert (t/'source-sheet.jpg').exists()
review=f'{args.take}/visual-review.json'
write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Inspected actual-game temporal sheet',evidence=['source-sheet.jpg'],visual_accepted=False,complete=False,notes=args.notes))
video=P/(args.take+'.mp4');assert video.exists()
meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
with video.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
req='emily_domino_mark_link_cancel'
row=dict(video=video.name,character='emily',label=f'Native Domino source observation · {args.take}',alias='domino_native_observation',requirements=[req],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{args.take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=r['passed'],scope=r['scope'])
videos=[v for v in read(P/'runtime-video-manifest.json') if v['video']!=video.name];videos.append(row);write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json');item=ann['items'][req];item.setdefault('native_integration',{})[video.name]=row
if r['passed'] and item.get('runtime_binding')in('not_verified','native_domino_source_observed_remote_pending'):
 item['runtime_binding']='native_domino_source_observed_remote_pending'
 item['notes']='Native Emily standing armed source observes PhantasmalLink state72, a shared scene incarnation with entry/aim/select/exit subscenes, owned left-arm and player-body bindings and native target effects. Inventory and checkpoint recovery pass. Complete links, cancel/fire contexts, remote body/VFX, target ownership, partner exclusion and shared outcomes remain pending. Authored body candidates are still unbound.'
write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(take=args.take,passed=r['passed'],videos=len(videos))))
