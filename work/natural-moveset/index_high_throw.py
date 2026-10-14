"""Index completed high-throw takes without equating scoped checks to acceptance."""
import datetime, hashlib, json, shutil, sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-high-throw-v31';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not (backup/name).exists(): shutil.copy2(P/name,backup/name)
videos=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json')
item=annotations['items']['carry_body_lift_idle_move_drop_throw'];cases=item.setdefault('native_high_throw',{})
for take in sys.argv[1:]:
    folder=P/take;i=read(folder/'identity.json');v=read(folder/'high-throw-validation.json');visual=read(folder/'visual-review.json')
    assert i['nativeThrow'] and visual['inspected']
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    row=dict(video=take+'/native.mp4',character=i['character'],
        label=i['character'].title()+' · Native pickup, walk and high throw · '+('scoped checks passed; visual review pending' if v['passed'] else 'REJECTED throw comparison'),
        alias='carry_body_drop_high_sequence',requirements=['carry_body_lift_idle_move_drop_throw'],
        gameplay_bound=True,complete=False,visual_accepted=False,evidence=take+'/high-throw-validation.json',
        visual_review=take+'/visual-review.json',proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],
        actual_capture_hz=v['actual_capture_hz'],scope=v['scope'],checks_passed=v['passed'])
    videos=[r for r in videos if r['video']!=row['video']];videos.append(row)
    cases[take]=dict(**row,checks=len(v['checks']),passed_checks=sum(v['checks'].values()),video_sha256=v['video_sha256'])
item['notes']=item['notes'].split(' Native high-throw continuation:')[0]+' Native high-throw continuation: exact source scene and camera-directed velocity are transmitted. Per-build paired tests and rejected comparisons are preserved here. Numerical success is scoped to the known Mayhew fixture; final contacts, arbitrary corpses, interruptions and campaign/world authority remain unfinished.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',annotations)
print(json.dumps(dict(recordings=len(videos),high_throw_takes=len(cases),complete=False)))
