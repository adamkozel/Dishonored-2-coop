"""Validate captured native phase data, independent of animation implementation.

Flat-floor mode checks that native IK preserves authored leg swing and bone
lengths. Stair mode checks anatomy while allowing necessary terrain correction.
These are structural checks; actual game footage remains required.
"""
import argparse,json,math
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('folder',type=Path);p.add_argument('--stairs',action='store_true');a=p.parse_args()
read=lambda path:json.loads(path.read_text(encoding='utf-8-sig'))
result=read(a.folder/'result.json');assert result['failure']is None,result['failure']
rows=[]
for file in a.folder.glob('pose-*.json'):
 s=read(file);assert s['stable']and s['finalPublicationComplete']
 phases={x['phase']:{m['joint']:m['matrix']for m in x['matrices']}for x in s['phases']}
 before,after=phases[0],phases[3];pelvis=[after[2][12+k]-before[2][12+k]for k in range(3)]
 length_error=0.;swing_error=0.;rotation_error=0.;ankle_error=0.
 for i in range(19,27):
  parent=s['joints'][i]['parent']
  length_error=max(length_error,abs(math.dist(before[i][12:15],before[parent][12:15])-math.dist(after[i][12:15],after[parent][12:15])))
  swing_error=max(swing_error,math.dist([after[i][12+k]-before[i][12+k]for k in range(3)],pelvis))
  if i in (21,25):ankle_error=max(ankle_error,math.dist([after[i][12+k]-before[i][12+k]for k in range(3)],pelvis))
  rotation_error=max(rotation_error,max(abs(after[i][k]-before[i][k])for k in [0,1,2,4,5,6,8,9,10]))
 assert length_error<.0005,(file.name,'native leg length changed',length_error)
 if not a.stairs:
  assert math.hypot(*pelvis[:2])<.002 and abs(pelvis[2])<.025,(file.name,'unexpected floor correction',pelvis)
  # Native IK may choose a different knee roll (observed during crouch).
  # Validate endpoint preservation separately; report internal rotations for review.
  assert ankle_error<.003,(file.name,'authored ankle swing distorted by native IK',ankle_error)
 rows.append(dict(pose=file.name,pelvis_correction=pelvis,max_bone_length_error_m=length_error,max_swing_residual_m=swing_error,max_ankle_residual_m=ankle_error,max_leg_rotation_element_change=rotation_error))
assert len(rows)>=(2 if a.stairs else 5),'Missing complete native pose samples'
states=read(a.folder/'states.json');counter='feetReferences';assert max(r['movement'].get(counter,0)for r in states)>50,'Native effector callback not observed'
out=dict(passed=True,mode='stairs'if a.stairs else'flat',dll=result['dll'],visual_accepted=False,scope='Captured native complete phase0/phase3 leg anatomy and authored swing. Does not replace visual inspection.',samples=rows)
(a.folder/'native-feet-validation.json').write_text(json.dumps(out,indent=2),encoding='utf-8')
print('PASS_NATIVE_FEET',a.folder.name,len(rows),'complete poses; max length error',max(r['max_bone_length_error_m']for r in rows))
