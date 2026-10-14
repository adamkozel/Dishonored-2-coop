"""Add loaded-contact measurements to the sequential hoist trace."""
import json,hashlib,math,sys
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
 f=P/take;c=read(f/'carrier-held.json');v=read(f/'victim-held.json');assert c['stable']and v['stable']
 a=world(v,'left_femur_jnt');b=world(v,'left_tibia_jnt');goal=local(c,[a[k]+.72*(b[k]-a[k])for k in range(3)])
 goal=[goal[k]+[.015,-.065,.035][k]for k in range(3)];hand=local(c,world(c,'left_wrist_jnt'))
 out=dict(left_target=goal,left_wrist=hand,left_error_m=math.dist(goal,hand),victim_root_in_carrier=local(c,v['root'][:3]),
  source={name:hashlib.sha256((f/name).read_bytes()).hexdigest()for name in ['carrier-held.json','victim-held.json']},
  scope='Sequential held skeletons; authoring palm offsets remain hypotheses. No surface contact acceptance.')
 (f/'held-contact-analysis.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
 if (f/'loaded-stance-contact.json').exists():
  raw=read(f/'loaded-stance-contact.json');pair=raw['pair'];c=pair['carrier'];v=pair['victim']
  assert pair['carrierFinalPublicationComplete'] and c['stable'] and v['stable']
  a=world(v,'left_femur_jnt');b=world(v,'left_tibia_jnt');goal=local(c,[a[k]+.72*(b[k]-a[k])for k in range(3)])
  goal=[goal[k]+[.015,-.065,.035][k]for k in range(3)];hand=local(c,world(c,'left_wrist_jnt'));shoulder=local(c,world(c,'left_arm_jnt'))
  final=dict(left_target=goal,left_wrist=hand,left_shoulder=shoulder,left_error_m=math.dist(goal,hand),
   support_delta=[goal[k]-shoulder[k]for k in range(3)],stance=raw['native']['stance'],movement_stance=raw['movement']['carryPlaybackStance'],
   read_span_ms=pair['readEndTick']-pair['readBeginTick'],publication_sequence=pair['publicationSequence'],
   source_sha256=hashlib.sha256((f/'loaded-stance-contact.json').read_bytes()).hexdigest(),
   scope='Completed carrier publication and stable victim sampled sequentially. Palm offsets are authoring hypotheses, not mesh-contact acceptance.')
  (f/'final-loaded-contact-analysis.json').write_text(json.dumps(final,indent=2));print(json.dumps(dict(take=take,final_published_contact=final)))
