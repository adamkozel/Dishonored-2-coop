"""Index an already visually inspected Possession take without asserting completion.

The visual-review.json must be written from actual inspection before invoking.
Failed takes remain eligible as labelled evidence; they are never relabelled passes.
"""
from pathlib import Path
import argparse,json,hashlib,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
p=argparse.ArgumentParser();p.add_argument('take');p.add_argument('--label',required=True);p.add_argument('--notes',required=True);a=p.parse_args()
T=P/a.take;assert T.parent==P and T.is_dir()
r=read(T/'result.json');review=read(T/'visual-review.json');assert review['inspected'] and not review.get('complete')
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_possession_enter_loop_exit']
for suffix,role in [('', 'peer'),('-source','source')]:
 v=P/(a.take+suffix+'.mp4');info=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(v)],text=True));n,d=map(float,info['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=v.name,character='corvo',label=f'Possession · {a.label} · {role}',alias='native_possession_lifecycle',requirements=['corvo_possession_enter_loop_exit'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{a.take}/result.json',visual_review=f'{a.take}/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=hashlib.sha256(v.read_bytes()).hexdigest().upper(),checks_passed=r['passed'],scope=review['notes'])
 videos=[x for x in videos if x['video']!=v.name];videos.append(row);item.setdefault('native_integration',{})[v.name]=row
item['notes']=a.notes
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(videos=len(videos),passed=r['passed'],checks=len(r['checks']))))
