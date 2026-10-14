"""Physical load/hand targets in a settled final-published native pose.

These targets diagnose attachment geometry; they do not accept mesh contact.
"""
import json,math,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def world(s,name):
 p=next(j['globalPose'][12:15]for j in s['joints']if j['nativeName']==name);r=s['root']
 return [r[k]+sum(r[3+c*3+k]*p[c]for c in range(3))for k in range(3)]
def local(s,p):
 r=s['root'];d=[p[k]-r[k]for k in range(3)]
 return [sum(r[3+c*3+k]*d[k]for k in range(3))for c in range(3)]
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');raw=read(f/'loaded-stance-contact.json');pair=raw['pair'];c=pair['carrier'];v=pair['victim']
 arm=local(c,world(c,'left_arm_jnt'));abdomen=local(c,world(v,'center_spine02_jnt'))
 goal=[arm[k]+[.025,.19,.115][k]for k in range(3)];load_error=math.dist(abdomen,goal)
 grip=read(f/'final-loaded-contact-analysis.json');stance=int(i['crouchedDrop'])
 checks=dict(completed_carrier_publication=pair['carrierFinalPublicationComplete'],
  bounded_sequential_read=pair['readEndTick']-pair['readBeginTick']<=50,
  expected_native_stance=raw['native']['stance']==stance,
  expected_playback_stance=raw['movement']['carryPlaybackStance']==stance,
  stance_settled=abs(raw['movement']['stanceMix']-stance)<.01,
  load_on_shoulder=load_error<.10,supporting_wrist_near_grip=grip['left_error_m']<.15)
 out=dict(passed=all(checks.values()),checks=checks,load_error_m=load_error,wrist_target_error_m=grip['left_error_m'],
  abdomen_in_carrier=abdomen,shoulder_load_target=goal,dll=i['dllSha256'],bank_sha256=i['bankSha256'],complete=False,visual_accepted=False,
  scope='One settled pose after a short native carry walk/stance change. Shoulder clearance and wrist target are explicit authoring targets, not accepted surface contacts.')
 (f/'load-contact-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
 if not out['passed']:raise SystemExit(1)
