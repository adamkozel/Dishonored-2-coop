import json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
report=[]
for role in ('corvo','emily'):
 for empty in (False,True):
  path=P/(f'native-high-mantle-{role}-empty-v1'if empty else f'native-high-mantle-{role}-v3')/'states.json'
  rows=json.loads(path.read_text(encoding='utf-8-sig'))
  for trial in ('high-standing-a','high-standing-b','high-crouched-a','high-crouched-b'):
   crouched='crouched'in trial;captures=[]
   for row in rows:
    if row['trial']!=trial:continue
    state=row['state'];active=next((a for a in state['active']if a['id']==8),None)
    if not active:continue
    a=active['mantleResearch'];scene=next((s for s in row['midnight']['instances']if s['id']==a['scene24']and s['state']==3),None)
    if not scene:continue
    assert a['kindd8']==2 and a['nextb8']==(6 if crouched else 0)
    assert scene['asset']==f"midnight/gameplay/player/base/mobility/mantle_{'sneak'if crouched else'idle'}.midnightscene"
    assert scene['sceneDurationMs']==833 and scene['subscene']==('Scene_2'if role=='corvo'else'Scene_1')
    binds=scene['bindingsResearch'];assert sum(s['handle']==1 for s in binds)==1 and all(s['reciprocal']for s in binds)
    captures.append(dict(time=scene['clockWords'][0],bindings=[s['handle']for s in binds],start=a['startE8'],goal=a['goalBc']))
   assert len(captures)>=6,(role,empty,trial,len(captures))
   report.append(dict(role=role,unarmed=empty,trial=trial,samples=len(captures),binding_counts=sorted(set(len(s['bindings'])for s in captures)),min_clock=min(s['time']for s in captures),max_clock=max(s['time']for s in captures),source=str(path.relative_to(P)),sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
out=dict(executable_sha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180',approach=[-30.7,-153.6,68.1,270],native_input='A held1300ms, LeftY22000; optional native crouch and holster inputs',cases=report,scope='Native high-mantle activation and scene observations only. Not authored remote presentation acceptance.')
(P.parent/'movement-integration/high-mantle-research/runtime-coverage-v1.json').write_text(json.dumps(out,indent=2))
print('NATIVE_HIGH_MANTLE',len(report),'cases',sum(r['samples']for r in report),'matched scene samples')
