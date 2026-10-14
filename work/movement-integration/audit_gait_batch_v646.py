from pathlib import Path
import json,math,datetime
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset/native-gait-batch-v645'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def d(a,b):return math.dist(a,b)
def yaw(r):
 m=r['nativeView']['nativeViewOrientation'];return math.degrees(math.atan2(m[1],m[0]))
def wrap(x):return(x+180)%360-180
def vector(rows):
 return [rows[-1]['source']['localPosition'][i]-rows[0]['source']['localPosition'][i]for i in range(2)]
result=dict(at=datetime.datetime.now().astimezone().isoformat(),test='native-gait-batch-v645',checks=[],characters=[],passed=False,complete=False,visualAccepted=False)
assert rd(N/'result.json')['passed']
for source,character in [('host','corvo'),('client','emily')]:
 rows=rd(N/f'{source}-gait-batch-observations.json');inputs=rd(N/f'{source}-inputs.json');parts={e['label']:[r for r in rows if r['label']==e['label']]for e in inputs}
 stats={}
 for label,p in parts.items():
  assert len(p)>=2,(character,label,len(p))
  stats[label]=dict(samples=len(p),distance=d(p[0]['source']['localPosition'],p[-1]['source']['localPosition']),yawDelta=wrap(yaw(p[-1])-yaw(p[0])),speedMax=max(x['movement']['speed']for x in p),speedEnd=p[-1]['movement']['speed'],phaseFirst=p[0]['movement']['phase'],phaseLast=p[-1]['movement']['phase'],runMax=max(x['movement']['runMix']for x in p),weightMin=min(x['movement']['weight']for x in p))
 errors=[d(x['source']['localPosition'],x['peer']['probeRenderPosition'])for x in rows];late=[r for r in rows if r['ms']>=500]
 v=vector(parts['walk-before-reverse']);back=vector(parts['reverse-back']);reverseDot=sum(a*b for a,b in zip(v,back))
 diag=vector(parts['diagonal-left']);diagr=vector(parts['diagonal-reverse']);diagDot=sum(a*b for a,b in zip(diag,diagr))
 turn=parts['moving-turn-right'];unarmed=[r for r in rows if 17500<r['ms']<18200];idle=[r for r in rows if r['ms']>26500]
 checks={
  'native_steam_stays_connected':all(r['source']['connectionState']==3 and r['peer']['connectionState']==3 and r['peer']['remoteFresh']for r in late),
  'bank67_stays_active':all(r['movement']['active']and r['movement']['version']==67 for r in late),
  'bounded_remote_root_error_under_one_metre':max(errors)<1.0,
  'walk_and_run_advance_native_root':stats['walk-forward']['distance']>.15 and stats['accelerate-run']['distance']>.35 and stats['accelerate-run']['runMax']>.5,
  'forward_backward_reversal':math.hypot(*v)>.15 and math.hypot(*back)>.15 and reverseDot<0,
  'diagonal_direction_reversal':math.hypot(*diag)>.15 and math.hypot(*diagr)>.15 and diagDot<0,
  'moving_turn_has_native_translation_and_yaw':stats['moving-turn-right']['distance']>.15 and abs(stats['moving-turn-right']['yawDelta'])>10,
  'each_stopped_gait_reaches_zero_speed':all(stats[x]['speedEnd']<.05 for x in ['run-stop','walk-stop','diagonal-stop','turn-stop','unarmed-stop','armed-recovery']),
  'holster_reaches_both_native_swords':len(unarmed)>=2 and all(not r['sword']['equipped']and not r['avatarSword']['equipped']and r['movement']['armedMix']<.05 for r in unarmed),
  'unarmed_walk_advances_root':stats['unarmed-forward']['distance']>.15,
  'redraw_and_idle_recover':len(idle)>=2 and all(r['sword']['equipped']and r['avatarSword']['equipped']and r['movement']['armedMix']>.95 and r['movement']['speed']<.05 for r in idle),
 }
 item=dict(character=character,checks=checks,samples=len(rows),rootErrorMax=max(errors),rootErrorMean=sum(errors)/len(errors),bodyWeightMin=min(r['movement']['weight']for r in late),streamReadySamples=sum(r['stream']['ready']for r in late),lateSamples=len(late),reverseDot=reverseDot,diagonalDot=diagDot,stages=stats)
 result['characters'].append(item);result['checks'].extend(dict(character=character,name=k,passed=v)for k,v in checks.items())
result['passed']=all(c['passed']for c in result['checks'])
result['limitations']=['One fixed Dunwall course, ordinary walk/run inputs and two moving turns; not exhaustive gaits/terrain/collision/powers.', 'One-metre bound detects incorrect or abandoned root following; it is not a low-latency or smoothness acceptance threshold. See actual measured errors.', 'Detailed contact, foot sliding, grip and natural body acting remain animator polish. Observer sheets must be inspected separately.', 'No new production code or clips: current635/67 functional regression and coverage audit. Complete movement is not established.']
p=R/'work/movement-integration/gait-batch-audit-v646.json';p.write_text(json.dumps(result,indent=2))
print(json.dumps(dict(passed=result['passed'],checks=len(result['checks']),characters=[{k:v for k,v in c.items()if k!='stages'}for c in result['characters']]),indent=2))
if not result['passed']:raise SystemExit(1)
