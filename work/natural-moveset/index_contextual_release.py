"""Index inspected contextual-release comparisons without accepting motion quality."""
import json,sys,hashlib,shutil,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-contextual-release-v35';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json']:
 if not(backup/name).exists():shutil.copy2(P/name,backup/name)
videos=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json')
item=annotations['items']['carry_body_lift_idle_move_drop_throw'];cases=item.setdefault('native_compact_release',{})
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');v=read(f/'fetal-release-validation.json');visual=read(f/'visual-review.json')
 assert visual['inspected'] and hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
 row=dict(video=take+'/native.mp4',character=i['character'],label=i['character'].title()+' · Contextual compact release · '+('scoped checks pass; visual issues remain'if v['passed']else'REJECTED comparison'),
  alias='carry_fetal_release_runtime',requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=True,complete=False,visual_accepted=False,
  evidence=take+'/fetal-release-validation.json',visual_review=take+'/visual-review.json',proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],
  actual_capture_hz=v['actual_capture_hz'],scope=v['scope'],checks_passed=v['passed'])
 videos=[r for r in videos if r['video']!=row['video']];videos.append(row)
 cases[take]=dict(**row,checks=len(v['checks']),passed_checks=sum(v['checks'].values()),video_sha256=v['video_sha256'])
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',annotations)
s=read(P.parent/'current-task-state.json');m=s['MovementIntegration'];dll=hashlib.sha256((P.parents[1]/'work/asset-game/dinput8.dll').read_bytes()).hexdigest().upper()
m['NativeCompactReleaseV35']=dict(updated=datetime.datetime.now().astimezone().isoformat(),dll='DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589',
 cases=cases,complete=False,visualAccepted=False,correction='Variant5 fetal release differs from drop_soft; no native ragdoll event in166ms victim clip',
 limitations=['Victim straightens above carrier during release','Initial camera framing failed visual coverage','166ms handoff is a hypothesis','Standing known Mayhew only','Arbitrary victims, interruption coverage and consistent world outcomes remain incomplete'])
m['current']='D2MOVE37 /235clips; contextual fallback lifecycle tested, victim release transition under investigation';m['liveBuild']=dll
m['LatestContinuation'].update(live=dll,liveBuild=dll,bank='D2MOVE37 /235clips',actual_game_videos=len(videos),in_progress='Native victim release pose transition; v35 compact lifecycle passes both roles but upright body pop fails visual acceptance',next='Verify native release clip output and snapshot transition; repair contacts then continue full movement checklist',complete=False)
s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),compact_cases=len(cases),live=dll,complete=False)))
