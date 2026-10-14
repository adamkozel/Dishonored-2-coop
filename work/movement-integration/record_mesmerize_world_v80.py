"""Index inspected source/receiver recordings without claiming full power support."""
import argparse,datetime,json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
a=argparse.ArgumentParser();a.add_argument('take');a.add_argument('--notes',required=True);args=a.parse_args()
t=P/args.take;r=read(t/'result.json');assert all((t/n).exists()for n in ['source-sheet.jpg','peer-sheet.jpg'])
review=f'{args.take}/visual-review.json'
write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Inspected source and receiver actual-game temporal sheets',evidence=['source-sheet.jpg','peer-sheet.jpg'],visual_accepted=False,complete=False,notes=args.notes))
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');req='emily_mesmerize_cast';item=ann['items'][req]
for source in (False,True):
 video=P/(args.take+('-source'if source else'')+'.mp4');assert video.exists()
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb')as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='emily',label=f'Mesmerize '+('native source'if source else'Steam body and cosmetic world')+f' · {args.take}',alias='mesmerize_native_world_network',requirements=[req],gameplay_bound=bool(r['passed']),complete=False,visual_accepted=False,evidence=f'{args.take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=r['passed'],scope=r['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
if r['passed']:
 item['runtime_binding']='native_observed_mesmerize_body_and_cosmetic_world_contexts_partial_verified'
 item['notes']='Native Emily enter/aim/release clocks drive the remote complete body through Steam. Committed source world states drive one cosmetic model, native scenes and owned particles. Individual takes specify cancellation, expiry and recovery coverage. Latest inspected passing take '+args.take+': '+args.notes+' Authoritative NPC outcomes, untested stance/direction and interruption contexts, and full visual acceptance remain incomplete. Individual failed takes and their reasons are retained.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann);print(json.dumps(dict(take=args.take,passed=r['passed'],videos=len(videos))))
