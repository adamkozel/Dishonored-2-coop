import json,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2))
cases=['native-body-throw-corvo-v1','native-body-throw-emily-v1'];findings=[];videos=read(P/'runtime-video-manifest.json')
for case in cases:
 f=P/case;v=read(f/'native-throw-observation-validation.json');rows=read(f/'observations.json');observed=[]
 for r in rows:
  for scene in r['carryScenes']:
   if scene['asset'].endswith('carry_corpse_drop_high.midnightscene'):
    observed.append(dict(ms=r['ms'],stateIds=[a['id']for a in r['native']['active']],sceneTime=scene['clockWords'][0]))
 review=dict(inspected=True,accepted=False,complete=False,method='Actual source first-person D2 temporal sheet at 6.7/7.1/7.3/7.5/7.7/8.2 seconds',observations='The native Throw RT prompt precedes forward body release and sword recovery. This fixture faces a nearby wall: it is not an unobstructed trajectory or remote full-body validation. Native high-drop scene lasts666ms. First sequential state/scene row crosses Carry45->Drop46; later rows correlate Drop46. Strict all-row correlation remains failed and is retained.')
 write(f/'visual-review.json',review)
 findings.append(dict(take=case,role=v['role'],scope=v['scope'],passedAllChecks=v['passed'],checks=v['checks'],sequentialStateSceneObservations=observed,video_sha256=v['video_sha256'],actual_capture_hz=v['actual_capture_hz'],visualReview=review))
 character='corvo'if v['role']=='host'else'emily';video=case+'/native.mp4';videos=[x for x in videos if x['video']!=video]
 videos.append(dict(video=video,character=character,label=character.title()+' · Native first-person throw reference · remote throw not implemented',alias='carry_native_throw_reference',requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=False,complete=False,visual_accepted=False,checks_passed=False,evidence=case+'/native-throw-observation-validation.json',visual_review=case+'/visual-review.json',proxy_sha256=v['proxy_sha256'],actual_capture_hz=v['actual_capture_hz'],scope=v['scope']+' Strict all-row state/scene correlation failed at the sequential transition sample; failed report preserved.'))
record=dict(at=datetime.datetime.now().astimezone().isoformat(),executableSha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180',confirmedObservation='Both normal standing native RT releases use carry_corpse_drop_high, Scene_1, duration666ms; loaded loop precedes release and Carry45/Drop46 retire afterward. Player health unchanged.',staticFinding='140832770 calls1403a6f10 to choose declaration field+70/+78/+80/+88 array/+98 by index0/1/2/3/other. Stores selector in Drop46+54, uses corpse handle+38, starts native Midnight and modifies corpse/physics flags. The scene-name-to-index mapping and native throw impulse still require further evidence.',hypotheses=['High-drop corresponds to selector2; do not extend whitelist based on this unverified mapping','Remote throw trajectory/physics release requires source/victim ownership and independent observations'],cases=findings,remoteThrowImplemented=False,complete=False,next='Bracket native states and scene identity in one read-only probe; verify selector, binding count and release/impulse paths; add source semantics with explicit guards before isolated remote experiment.')
write(P/'carry-research/native-throw-findings-v1.json',record);write(P/'runtime-video-manifest.json',videos)
s=read(P.parent/'current-task-state.json');m=s['MovementIntegration'];m['NativeThrowObservationV1']=record;m['LatestContinuation']['actual_game_videos']=len(videos);m['LatestContinuation']['next']=record['next'];s['UpdatedAt']=record['at'];write(P.parent/'current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=len(cases),remoteThrowImplemented=False)))
