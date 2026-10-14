"""Audit ordinary crouched coin and food use against exact native states."""
from pathlib import Path
import json, datetime, argparse
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
p=argparse.ArgumentParser();p.add_argument('--food',action='store_true');a=p.parse_args()
pin=rd(W/'candidate-crouch-loot-v693/identity.json');cases=[]
kind='food' if a.food else 'coin';stance=0 if a.food else 1;ground=0 if a.food else 6
takes=[('native-interactions-corvo-food-v696','host'),('native-interactions-emily-food-v697','client')] if a.food else [('native-interactions-corvo-crouch-coin-v699','host'),('native-interactions-emily-crouch-coin-v695','client')]
for take,role in takes:
 f=N/take;r=rd(f/'result.json');o=rd(f/f'{kind}-observations.json');watch=rd(f/f'{kind}-state-transitions.json');checks=[];failures=[]
 def require(ok,label):
  (checks if ok else failures).append(label)
 require(r['passed'] and r['dll']==pin['dll'] and r['source']==role,'Exact candidate completed with both inventories, health and14 protected files restored')
 require(watch['readOnly'] and watch['elapsedMs']>=13900 and watch['samples']>=500 and watch['samples']==watch['validSamples'],'Compact native state reader remained valid for the entire14-second observation')
 loot=[s for s in watch['records'] if any(x['id']==47 for x in s['active'])]
 require(loot and all(s['defaultMode'] and s['stance']==stance and next(x for x in s['active'] if x['id']==47)['vtableRva']=='0x1c7bcc8' and ground in [x['id'] for x in s['active']] for s in loot),'Native LootObject47 overlaps the matching actual ground stance')
 require(all(s['motionValid'] and s['motion']==stance for s in watch['records']),'Native movement remains recognized throughout loot, undraw and recovery')
 require(len(o)>=85 and all(x['source']['localGameLogicState']==1 and not x['source']['localPendingGameLogicState'] for x in o),'Source remains in active native gameplay')
 require(all(x['sourceStream']['ready'] and x['stream']['ready'] for x in o),'Both native Steam presentation streams remain ready throughout')
 require(all(x['movement']['active'] and x['movement']['weight']>.999 for x in o),'Remote complete-body bank remains active and fully weighted')
 require(all(abs(x['movement']['stanceMix']-stance)<.001 for x in o),'Remote body preserves the source ground stance')
 require(rd(f/f'{kind}-target-after.json')==[],'Exact source pickup is removed by ordinary native interaction')
 require(watch['records'][-1]['active']==[{'id':ground,'vtableRva':'0x1c85628' if ground==0 else '0x1c84258'}],'Native source returns to its original ground state')
 hb=rd(f/f'{kind}-health-before.json');ha=rd(f/f'{kind}-health-after.json')
 require(hb['source']['current']==56 and hb['peer']['current']==ha['peer']['current']==56 and ha['source']['current']>=56 and ha['source']['current']<=ha['source']['maximum'],'Source health stays valid and peer health is unchanged')
 cases.append(dict(take=take,role=role,passed=not failures,checks=checks,failures=failures,samples=len(o),compactSamples=watch['samples'],minimumBodyWeight=min(x['movement']['weight'] for x in o),bodyWeightDips=[dict(ms=x['ms'],weight=x['movement']['weight'],eligible=x['movement'].get('eligibility',{}).get('mask')) for x in o if x['movement']['weight']<=.999],sourceHealthBefore=hb['source']['current'],sourceHealthAfter=ha['source']['current'],nativeLootStates=[s['active'] for s in loot]))
audit=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=all(x['passed'] for x in cases),candidate='candidate-crouch-loot-v693',dll=pin['dll'],probe=pin['probe'],cases=cases,checks=sum(len(x['checks']) for x in cases),failedChecks=sum(len(x['failures']) for x in cases),complete=False,visualAccepted=False,scope='Exact native crouched coin pickup or standing grapes consumption, as selected; existing body continuity only. No new reach/eating gesture, shared inventory, shared healing, receiver item removal, general stealing or complete movement claim.')
path=W/f'{"food-continuity" if a.food else "crouch-loot"}-audit-v698.json';path.write_text(json.dumps(audit,indent=2),encoding='utf-8')
print(json.dumps(dict(passed=audit['passed'],kind=kind,checks=audit['checks'],failedChecks=audit['failedChecks'],cases=[dict(take=x['take'],samples=x['samples'],compactSamples=x['compactSamples'],minimumBodyWeight=x['minimumBodyWeight'],sourceHealthAfter=x['sourceHealthAfter'],failures=x['failures']) for x in cases])))
if not audit['passed']:raise SystemExit(1)

