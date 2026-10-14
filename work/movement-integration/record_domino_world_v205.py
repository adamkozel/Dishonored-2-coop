"""Index inspected native/remote Domino cosmetic mark captures without gameplay claims."""
import argparse,datetime,json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
a=argparse.ArgumentParser();a.add_argument('take');a.add_argument('--notes',required=True);args=a.parse_args()
t=P/args.take;r=read(t/'result.json');assert all((t/n).exists()for n in ['source-sheet.jpg','peer-sheet.jpg'])
review=f'{args.take}/visual-review.json'
write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Inspected source and receiver actual-game temporal sheets',evidence=['source-sheet.jpg','peer-sheet.jpg'],visual_accepted=False,complete=False,notes=args.notes))
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');req='emily_domino_mark_link_cancel';item=ann['items'][req]
for source in (False,True):
 video=P/(args.take+('-source'if source else'')+'.mp4');assert video.exists()
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb')as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='emily',label=f'Domino '+('native source'if source else'Steam body and mark presentation')+f' · {args.take}',alias='domino_native_mark_network',requirements=[req],gameplay_bound=bool(r['passed']),complete=False,visual_accepted=False,evidence=f'{args.take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=r['passed'],scope=r['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
if r['passed']:
 item['runtime_binding']='native_domino_standing_armed_body_and_mark_partial_verified'
 item['notes']='Native Emily phases and strictly owned target marks traverse native Steam. Receiver recreates validated native FX on owned presentation objects, with no native Domino/NPC/damage calls. Latest inspected passing take '+args.take+': '+args.notes+' Receiver NPC attachment alignment, multi-target links, partner exclusion, shared NPC outcomes, additional stances/interruptions and visual acceptance remain incomplete.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann);print(json.dumps(dict(take=args.take,passed=r['passed'],videos=len(videos))))
