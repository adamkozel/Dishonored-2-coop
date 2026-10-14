"""Verify the exact native standing-coin transition; no gesture/world-sync claim."""
from pathlib import Path
import json, datetime
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'; W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
pin=rd(W/'candidate-loot-motion-v666/identity.json'); cases=[]
baseline=rd(N/'native-interactions-corvo-coin-v664/coin-observations.json')
assert any(x['player'] and any(s['id']==47 for s in x['player']['active']) and not x['player']['motionValid'] for x in baseline)
for take,role in [('native-interactions-corvo-coin-v667','host'),('native-interactions-emily-coin-v668','client')]:
    f=N/take; r=rd(f/'result.json'); o=rd(f/'coin-observations.json'); watch=rd(f/'coin-state-transitions.json'); checks=[]
    def require(condition,label):
        assert condition,(take,label)
        checks.append(label)
    require(r['passed'] and r['dll']==pin['dll'] and r['source']==role,'Exact candidate completed and restored both inventories, player/partner health and 14 protected files')
    require(watch['readOnly'] and watch['elapsedMs']>=9900 and watch['samples']>=400 and watch['validSamples']==watch['samples'],'Ten-second compact reader retained a valid reciprocal native state throughout')
    loot=[s for s in watch['records'] if any(a['id']==47 for a in s['active'])]
    require(len(loot)>=2 and {tuple(a['id'] for a in s['active']) for s in loot}=={(0,47),(0,47,81)},'Both brief native standing loot combinations observed, with and without Undraw81')
    require(all(s['defaultMode'] and s['stance']==0 and s['motionValid'] and s['motion']==0 and next(a for a in s['active'] if a['id']==47)['vtableRva']=='0x1c7bcc8' for s in loot),'Exact native LootObject47 remains recognized as standing motion')
    require(all(s['motionValid'] and s['motion']==0 for s in watch['records']),'Interact, loot, undraw and final running state remain recognized')
    require(len(o)>=90 and all(s['source']['localGameLogicState']==1 and not s['source']['localPendingGameLogicState'] for s in o),'Source remained in active gameplay throughout capture')
    require(all(s['sourceStream']['ready'] and s['stream']['ready'] for s in o),'Both native Steam presentation streams remained ready')
    require(all(s['movement']['active'] and s['movement']['weight']>.999 for s in o),'Remote full-body bank remained active and fully weighted')
    require(rd(f/'coin-target-after.json')==[],'Exact native coin entity removed from source registry after ordinary X input')
    require(watch['records'][-1]['active']==[{'id':0,'vtableRva':'0x1c85628'}] and o[-1]['stream']['remoteState']==0,'Source and receiver returned to ordinary idle')
    cases.append(dict(take=take,role=role,passed=True,checks=checks,samples=len(o),compactSamples=watch['samples'],minimumBodyWeight=min(s['movement']['weight'] for s in o),nativeLootWindowMs=[loot[0]['ms'],loot[-1]['ms']],sourceVideo=take+'/coin-source.mp4',observerVideo=take+'/coin-observer.mp4'))
audit=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=True,candidate='candidate-loot-motion-v666',dll=pin['dll'],probe=pin['probe'],baselineRejectedNativeLoot=True,cases=cases,checks=sum(len(x['checks']) for x in cases),visualAccepted=False,complete=False,scope='Both-role stationary standing coin pickup preserves existing body and first-person behavior. No new reach gesture, crouched pickup, pickpocket, shared currency, inventory or world-removal claim.')
(W/'loot-motion-audit-v668.json').write_text(json.dumps(audit,indent=2),encoding='utf-8')
print(json.dumps(dict(passed=True,checks=audit['checks'],cases=[dict(take=x['take'],samples=x['samples'],compactSamples=x['compactSamples'],minimumBodyWeight=x['minimumBodyWeight']) for x in cases])))
