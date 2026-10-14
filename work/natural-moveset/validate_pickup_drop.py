"""Native source-driven drop and victim physics handoff, not throw coverage."""
import json,sys,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def word(obj,off):return int(next(w['word']for w in obj['words']if w['offset']==off),16)
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');v=read(f/'validation.json');rows=read(f/'observations.json');assert i['nativeDrop']
 variant=6 if i['crouchedDrop']else 4
 source=[r for r in rows if r['native'].get('carryScene',{}).get('variant')==variant]
 remote=[r for r in rows if r['movement']['carryVariant']==variant and r['movement']['carryWeight']>.5]
 release=[r for r in rows if r['attachment']['dropStage']==2 and r['attachment']['dropVariant']==variant]
 held=[r for r in rows if r['attachment']['anchorCalibrated']and not r['attachment']['dropStage']]
 post=[r for r in rows if r['beginMs']>i['cancelMs']+1500]
 before=read(f/'sword-before.json');after=read(f/'sword-restored.json');stopped=read(f/'stopped.json')
 source_position=read(f/'source-after-drop.json')['position'];corpse_position=read(f/'corpse-after.json')['root'][:3]
 final_horizontal_distance=math.dist(source_position[:2],corpse_position[:2])
 checks=dict(carrier_passed=v['passed'],source_native_drop=bool(source),remote_authored_drop=bool(remote),
  native_victim_drop=bool(release),loaded_walk=any(r['movement']['speed']>.1 and r['movement']['carryMove']>.5 for r in held),
  same_corpse_generation=all(r['attachment']['targetHandle']==int(i['corpse'],0)for r in rows if r['attachment']['active']),
  source_finished=not read(f/'source-after-drop.json')['carrySceneValid'],
  retired_after_drop=bool(post)and all(not r['attachment']['active']for r in post),
  retarget_removed=read(f/'retarget-after.json')['count']==0,
  native_ragdoll_restored=word(read(f/'physics-after.json'),'0x238')==0xffffffff00000000,
  victim_health_unchanged=read(f/'corpse-health-after.json')['current']==0,
  original_sword_restored=all(before[k]==after[k]for k in ['weaponHandle','equipped','started','explicitHidden','attachedTag']),
  equipment_retired=not stopped['equipment']['active']and not stopped['equipment']['restoring']and not stopped['equipment']['lost'],
  corpse_does_not_fly_away=final_horizontal_distance<2.5)
 out=dict(passed=all(checks.values()),checks=checks,source_drop_samples=len(source),remote_drop_samples=len(remote),victim_drop_samples=len(release),
  video_sha256=v['video_sha256'],actual_capture_hz=v['actual_capture_hz'],dll=i['dllSha256'],complete=False,visual_accepted=False,final_corpse_distance_from_source_m=final_horizontal_distance,
  scope='One continuous native-input '+('crouched' if i['crouched'] else 'standing')+' pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.')
 (f/'pickup-drop-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
 if not out['passed']:raise SystemExit(1)
