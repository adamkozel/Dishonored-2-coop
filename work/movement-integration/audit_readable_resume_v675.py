"""Native note UI, incarnation-pinned recovery and optional body-visibility audit."""
from pathlib import Path
import json, argparse, datetime
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'; W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
p=argparse.ArgumentParser();p.add_argument('--visibility',action='store_true');a=p.parse_args()
version=675 if a.visibility else 672; candidate='candidate-readable-body-v673' if a.visibility else 'candidate-read-note-v670'; pin=rd(W/candidate/'identity.json')
takes=[('native-interactions-corvo-note-v674','host'),('native-interactions-emily-note-v675','client')] if a.visibility else [('native-interactions-corvo-note-v671','host'),('native-interactions-emily-note-v672','client')]
cases=[]
for take,role in takes:
 f=N/take;r=rd(f/'result.json');o=rd(f/'note-observations.json');checks=[]
 def require(v,label):
  assert v,(take,label)
  checks.append(label)
 require(r['passed'] and r['dll']==pin['dll'] and r['source']==role,'Exact build completed with both inventories, health and14 protected files restored')
 reading=[x for x in o if x['logic']['stack'][-1]['id']==4]
 require(len(reading)>=15 and all(x['logic']['pending']==0 and x['logic']['stack'][-1]['vtableRva']=='0x1c61e68' and x['logic']['stack'][-1]['type']=='.?AVarkGameLogicStateReadNote@@' for x in reading),'Actual native ReadNote4 and exact RTTI/vtable observed for sustained UI interval')
 before=[x for x in o if x['ms']<reading[0]['ms']];after=[x for x in o if x['ms']>reading[-1]['ms'] and x['logic']['stack'][-1]['id']==1]
 require(before and after and o[0]['sourceStream']['ready'] and o[0]['stream']['ready'],'Ordinary gameplay and live presentation preceded reading')
 require(all(len({x[side][key] for x in o})==1 for side in ['source','peer'] for key in ['localEpoch','probeActorHandle','localMapId']),'Both actor handles, world epochs and maps retained through native UI and recovery')
 stable=[x for x in reading if x['ms']>=reading[0]['ms']+400]
 require(stable and all(x['resume']['result']=='native_menu_suspended' and not x['movement']['active'] and not x['stream']['ready'] for x in stable),'Movement/action presentation remained suspended while native note UI was open')
 last=o[-1];first=o[0]
 require(last['sourceStream']['ready'] and last['stream']['ready'] and last['resume']['result']=='same_world_movement_restored' and last['resume']['resumed']>first['resume']['resumed'],'Both streams and receiver movement resumed automatically after closing native UI')
 require(last['stream']['token']!=first['stream']['token'] and last['stream']['peerToken']!=first['stream']['peerToken'],'Recovery uses fresh source and receiver handshake tokens')
 require(last['movement']['active'] and last['movement']['weight']>.999 and last['movement']['version']==67,'Unchanged bank67 recovered to full standing presentation')
 require(last['resume']['refused']==first['resume']['refused'] and last['resume']['checksum']==first['resume']['checksum'],'No resume fault or substituted animation bank')
 require(rd(f/'note-target-after.json')==[],'Exact source note collected through native UI interaction')
 if a.visibility:
  require(all(x['observerAvatar']['current']==40 and not x['observerAvatar']['explicitHidden'] for x in o),'Remote native body remained visible and its health unchanged throughout capture')
  require(all(x['sourceIsolation']['ready'] and x['peerIsolation']['ready'] and x['peer']['avatarAiHeld'] for x in o),'NPC test isolation and native partner AI ownership retained throughout pause/recovery')
 body=next(x for x in after if x['movement']['active'] and x['resume']['result']=='same_world_movement_restored')
 fully=next(x for x in after if x['movement']['active'] and x['movement']['weight']>.999 and x['resume']['result']=='same_world_movement_restored')
 cases.append(dict(take=take,role=role,checks=checks,samples=len(o),nativeNoteSamples=len(reading),bodyResumeAfterFirstGameplayMs=body['ms']-after[0]['ms'],fullWeightAfterFirstGameplayMs=fully['ms']-after[0]['ms'],bodyVisibilityVerified=a.visibility,passed=True))
audit=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=True,candidate=candidate,dll=pin['dll'],probe=pin['probe'],cases=cases,checks=sum(len(x['checks']) for x in cases),bodyVisibilityVerified=a.visibility,complete=False,visualAccepted=False,scope='Ordinary native note UI and same-world automatic body recovery only. No authored reading gesture, document-state replication, shared UI, load/death recovery or complete movement claim.')
(W/f'readable-resume-audit-v{version}.json').write_text(json.dumps(audit,indent=2),encoding='utf-8')
print(json.dumps(audit))
