"""Add current-build low-drop regressions without rewriting throw findings."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');cases={}
for role in ['corvo','emily']:
 take=f'native-low-drop-{role}-regression-v34';f=P/take;i=read(f/'identity.json');v=read(f/'pickup-drop-validation.json');pickup=read(f/'validation.json');visual=read(f/'visual-review.json')
 assert v['passed'] and pickup['passed'] and visual['inspected']
 assert hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
 row=dict(video=take+'/native.mp4',character=role,label=role.title()+' · Ordinary low-drop regression on high-throw build',
  alias='carry_body_drop_low_sequence',requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=True,complete=False,visual_accepted=False,
  evidence=take+'/pickup-drop-validation.json',visual_review=take+'/visual-review.json',proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],
  actual_capture_hz=v['actual_capture_hz'],scope=v['scope'],checks_passed=True)
 videos=[r for r in videos if r['video']!=row['video']];videos.append(row)
 cases[take]=dict(**row,checks=len(v['checks'])+len(pickup['checks']),video_sha256=v['video_sha256'],final_corpse_distance_from_source_m=v['final_corpse_distance_from_source_m'])
ann['items']['carry_body_lift_idle_move_drop_throw']['high_throw_build_low_drop_regression']=cases
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
state=read(P.parent/'current-task-state.json');state['MovementIntegration']['LowDropRegressionV34']=cases;write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(recordings=len(videos),regression_cases=len(cases))))
