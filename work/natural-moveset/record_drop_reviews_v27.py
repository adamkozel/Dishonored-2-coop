"""Persist inspected v27 drop comparisons, without accepting complete carry."""
import json, datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v): p.write_text(json.dumps(v,indent=2))
cases=[]
for role in ['corvo','emily']:
 for low in [False,True]:
  case=f'native-pickup-walk-{"crouch-" if low else ""}drop-{role}-v27';cases.append(case)
  f=P/case;v=read(f/'pickup-drop-validation.json');assert v['passed']
  if low:
   write(f/'visual-review.json',dict(inspected=True,accepted=False,complete=False,
    method='Actual D2 temporal sheet at 3.5, 5.9, 6.4, 6.8, 7.2 and 8.1 seconds',
    observations='Native pickup, loaded walk, crouch and low drop connect. The body descends beside the carrier and settles locally in the inspected frames; no upward launch like the rejected v26 Corvo standing case. Carrier remains crouched during release. Palm contact is partly obscured and the body swings during release, so this is scoped lifecycle/physics evidence, not final animation acceptance. Crouched pickup and arbitrary bodies remain untested.'))
archive=P.parent/'movement-integration/candidate-drop-handoff-v27/identity.json'
a=read(archive);a['validatedCases']=cases;a['runtimeValidated']=True;a['complete']=False;write(archive,a)
s=read(P.parent/'current-task-state.json');m=s['MovementIntegration'];c=m['LatestContinuation']
m['current']='D2MOVE34 / 232 clips; v27 native sword visibility and carried-body physics handoff validated in bounded Dunwall cases. Complete movement and final visual acceptance unfinished.'
m['liveBuild']=c['live'];c['next']='Add crouched pickup, then remaining interactions, traversal/combat/items/powers and visual acceptance.'
c['drop_handoff']='Four continuous native-input pickup/walk/standing-or-crouched-drop cases passed 36 checks each, with actual video sheets inspected. Full hand contact and general body ownership remain incomplete.'
s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',s)
print(json.dumps(dict(validated=cases,complete=False)))
