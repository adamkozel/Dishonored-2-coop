from pathlib import Path
import json,math,datetime
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
result=dict(at=datetime.datetime.now().astimezone().isoformat(),candidate='candidate-auto-crouch-v651',passed=False,complete=False,checks=[],cases=[])
for role,character,take in [('host','corvo','native-auto-crouch-corvo-v653'),('client','emily','native-auto-crouch-emily-v654'),('host','corvo','native-auto-crouch-corvo-v655')]:
 d=N/take;r=rd(d/f'{role}-auto-crouch-sill-observations.json');receipt=rd(d/'result.json');late=[x for x in r if x['ms']>500]
 held=[x for x in r if 5500<x['ms']<10000];end=[x for x in r if x['ms']>16500]
 mantle=[x for x in r if x['player'].get('traversalSceneValid')];equip=[x for x in r if x['player'].get('equipmentSceneValid')]
 err=[math.dist(x['source']['localPosition'],x['peer']['probeRenderPosition'])for x in late]
 checks={
  'restoration_receipt_passed':receipt['passed'],
  'native_steam_and_both_streams_remain_ready':all(x['stream']['ready']and x['sourceStream']['ready']and x['source']['connectionState']==3 and x['peer']['connectionState']==3 for x in late),
  'owned_automatic_mantle_scene_observed':len(mantle)>=2 and all(x['player']['stance']==2 for x in mantle),
  'remote_existing_crouch_mantle_reached':any(x['movement']['traversalVariant']==2 and x['movement']['traversalWeight']>.8 for x in r),
  'native_fast_recovery_owned':len(equip)>=2 and all(x['player']['equipmentScene']['variant']==6 and x['player']['equipmentScene']['durationMs']==333 for x in equip),
  'receiver_accepts_recovery_action':any(x['stream']['remoteState']==7 for x in r),
  'held_auto_crouch_has_exact_state':len(held)>10 and all(x['player']['stance']==2 and [a['id']for a in x['player']['active']]==[10]and x['player']['motionValid']for x in held),
  'wire_maps_only_presentation_to_crouch':all(x['source']['localPlayerStanceValid']and x['source']['localPlayerStance']==1 and x['peer']['remotePlayerStanceValid']and x['peer']['remotePlayerStance']==1 for x in held),
  'held_crouch_body_and_bank_remain_active':all(x['movement']['active']and x['movement']['version']==67 and x['movement']['weight']>.99 and x['movement']['stanceMix']>.99 and x['movement']['traversalWeight']<.01 for x in held),
  'native_sword_and_presentation_recover':all(x['sword']['equipped']and x['movement']['presentedDrawn']and x['movement']['armedMix']>.99 for x in held+end),
  'native_standing_and_body_recover_after_backout_and_B':len(end)>10 and all(x['player']['stance']==0 and x['player']['motionValid']and x['movement']['weight']>.99 and x['movement']['stanceMix']<.01 for x in end),
  'bounded_root_following':max(err)<1.0,
 }
 result['checks']+=list(dict(take=take,name=k,passed=bool(v))for k,v in checks.items())
 result['cases'].append(dict(take=take,character=character,samples=len(r),checks=checks,rootErrorMax=max(err),rootErrorMean=sum(err)/len(err),bodyWeightMin=min(x['movement']['weight']for x in late),heldWeightMin=min(x['movement']['weight']for x in held),nativeMantleSamples=len(mantle),nativeEquipmentSamples=len(equip)))
result['passed']=all(x['passed']for x in result['checks'])
result['limits']=['Closed window sill only; no window open/exit or shared world interaction claim.', 'Fine hand/foot/sill contacts and crouch acting remain polish. Brief body-blend dips during entry or retreat are recorded on both characters; sustained held crouch and final standing are checked separately.', 'Fast draw variants5/6 intentionally use existing full-body traversal recovery and native sword contact, not an additional hip-draw clip.', 'No AutoCrouch combat/power overlays or high automatic mantle binding claimed. The two normal0/1 stance and the protocol69/bank67 layouts remain unchanged.']
(R/'work/movement-integration/auto-crouch-audit-v656.json').write_text(json.dumps(result,indent=2))
print(json.dumps(dict(passed=result['passed'],checks=len(result['checks']),failures=[x for x in result['checks']if not x['passed']],cases=result['cases']),indent=2))
if not result['passed']:raise SystemExit(1)

