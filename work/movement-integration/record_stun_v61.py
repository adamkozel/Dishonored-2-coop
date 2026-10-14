"""Index inspected native takes with source-only versus network scope explicit."""
import argparse,datetime,json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
a=argparse.ArgumentParser();a.add_argument('take');a.add_argument('--source-only',action='store_true');a.add_argument('--notes',required=True);args=a.parse_args()
t=P/args.take;r=read(t/'result.json');now=datetime.datetime.now().astimezone().isoformat()
sheets=['source-sheet.jpg'] if args.source_only else ['native-sheet.jpg','source-sheet.jpg']
assert all((t/x).exists() for x in sheets)
review=f'{args.take}/visual-review.json'
write(P/review,dict(inspected=True,at=now,method='Inspected actual-game temporal sheets',evidence=sheets,visual_accepted=False,complete=False,notes=args.notes))
videos=read(P/'runtime-video-manifest.json');added=[]
for suffix in ([''] if args.source_only else ['','-source']):
    video=f'{args.take}{suffix}.mp4';f=P/video;assert f.exists()
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
    row=dict(video=video,character='corvo' if r['source']=='host' else 'emily',label=f'Native {"source observation" if args.source_only else "stun network"} · {args.take}{suffix}',alias='hit_react_stun' if 'stun' in args.take else 'grenade_air_observation' if '-air-' in args.take else 'grenade_crouch_observation',requirements=['hit_reactions_directions'] if 'stun' in args.take else ['grenade_equip_aim_throw'],gameplay_bound=bool(r['passed']) and not args.source_only,complete=False,visual_accepted=False,evidence=f'{args.take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=sha(f),checks_passed=r['passed'],scope=r['scope'])
    videos=[x for x in videos if x['video']!=video];videos.append(row);added.append(row)
write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json')
for row in added:
    for req in row['requirements']:
        if req in ann['items']:ann['items'][req].setdefault('native_integration',{})[row['video']]=row
write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(take=args.take,source_only=args.source_only,checks_passed=r['passed'],videos=len(videos),visual_accepted=False)))
