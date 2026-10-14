import json,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2))
cases=['native-crouched-to-standing-carry-corvo-v30-sampled','native-standing-to-crouched-carry-emily-v30']
notes=[
 'Actual D2 temporal sheet inspected: crouched pickup, brief walk, standing recovery and low drop. The victim follows the shoulder rather than riding high as in v29. Native drop completes. Final shoulder target residual 1.71cm; supporting wrist target residual 10.52cm still needs grip refinement and closer surface inspection. Not visual acceptance of complete carry.',
 'Actual D2 temporal sheet inspected: standing pickup, walk, crouched load and low drop. The body stays over the shoulder as the carrier lowers; the prior v29 offset is corrected. Final shoulder target residual 1.68cm and wrist target residual 3.90cm. This camera does not establish exact palm/finger surface contact; full visual acceptance remains pending.'
]
for take,note in zip(cases,notes):
 for f in ['validation.json','pickup-drop-validation.json','orientation-validation.json','load-contact-validation.json']:assert read(P/take/f)['passed']
 write(P/take/'visual-review.json',dict(inspected=True,accepted=False,complete=False,method='Actual D2 close-view temporal sheet plus independently read final-published geometry',observations=note))
i=P.parent/'movement-integration/candidate-carry-stance-anchor-v30/identity.json';v=read(i);v.update(runtimeValidated=True,runtimeCases=cases,visualAccepted=False,limitations=['Selected Mayhew fixture only','Corvo standing supporting wrist residual 10.52cm','Turning, arbitrary-body ownership, throws, full contact and complete movement unverified']);write(i,v)
s=read(P.parent/'current-task-state.json');m=s['MovementIntegration'];dll=v['sha256'];m['current']='D2MOVE35 / 233 clips; v30 stance-aware native carry anchor, scoped two-role tests pass; full phase incomplete';m['liveBuild']=dll
l=m['LatestContinuation'];l.update(live=dll,liveBuild=dll,in_progress='Native body throw observation; carry stance-anchor v30 passed both selected role/stance directions. Full hand contact and general ownership remain incomplete.',next='Inspect native throw identity/timing; expand remaining carry transitions and contact refinement.',bank='D2MOVE35 / 233 clips, crouched-pickup contact v3',last_validated_source_fix=dll)
m['CarryStanceAnchorV30']=dict(dll=dll,cases=cases,complete=False,firstCorvoTake='native-crouched-to-standing-carry-corvo-v30: preparation_transmitted sample absent; preserved failed, rerun sampled earlier and passes',loadErrorMeters=[.017075473325087714,.01684610246458013],wristTargetErrorMeters=[.10524061543786233,.03895035871190016]);s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',s)
