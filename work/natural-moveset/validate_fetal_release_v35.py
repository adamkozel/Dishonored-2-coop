"""Check contextual release identity, source clock and zero-impulse lifecycle."""
import json,sys,math
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def word(obj,offset):return int(next(x['word']for x in obj['words']if x['offset']==offset),16)
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');rows=read(f/'observations.json');pickup=read(f/'validation.json')
 before=read(f/'attachment-before.json');after=read(f/'attachment-after.json')
 source=[r['native']['carryScene']for r in rows if r['native'].get('carryScene',{}).get('variant')==5]
 remote=[r for r in rows if r['movement']['carryVariant']==5 and r['movement']['carryWeight']>.5]
 victim=[r for r in rows if r['attachment']['dropVariant']==5 and r['attachment']['dropStage']==2 and r['attachment']['nativeVictimSceneActive']]
 post=[r for r in rows if r['beginMs']>i['cancelMs']+1500]
 initial=read(f/'sword-before.json');restored=read(f/'sword-restored.json');stopped=read(f/'stopped.json')
 points=[x['pose']['root'][:3]for x in read(f/'release-poses.json')if x['endMs']>i['cancelMs']]
 checks=dict(pickup_checks_passed=pickup['passed'],native_fetal_release_observed=len(source)>=3,
  exact_source_state=bool(source)and all(s['variant']==5 and s['stateId']==46 and s['phase']==4 and s['durationMs']==666 and s['stance']==0 for s in source),
  authored_remote_release=len(remote)>=3,victim_only_fetal_scene=bool(victim),
  no_throw_impulse=after['throwQueued']==before['throwQueued'],no_throw_attempt_refusals=after['throwRefused']==before['throwRefused'],
  same_corpse_generation=all(r['attachment']['targetHandle']==int(i['corpse'],0)for r in rows if r['attachment']['active']),
  source_finished=not any(x['id']in[45,46]for x in read(f/'source-after-drop.json')['active']),
  released_after_drop=bool(post)and all(not r['attachment']['active']and not r['attachment']['restoring']for r in post),
  retarget_removed=read(f/'retarget-after.json')['count']==0,
  native_ragdoll_restored=word(read(f/'physics-after.json'),'0x238')==0xffffffff00000000,
  victim_remains_dead=read(f/'corpse-health-after.json')['current']==0,
  health_preserved=all(i['healthBefore'][k]['current']==i['healthAfter'][k]['current']for k in ['source','observer','avatar']),
  original_sword_restored=all(initial[k]==restored[k]for k in ['weaponHandle','equipped','started','explicitHidden','attachedTag']),
  equipment_retired=not stopped['equipment']['active']and not stopped['equipment']['restoring']and not stopped['equipment']['lost'],
  old_clip_payload_retained=next(x for x in read(P/i['bankManifest'])if x['role']==i['character'])['prior_payload_unchanged'],
  finite_physical_trajectory=bool(points)and all(math.isfinite(v)for p in points for v in p),
  no_unbounded_launch=bool(points)and max(math.dist(points[0],p)for p in points)<3)
 result=dict(passed=all(checks.values()),checks=checks,source_samples=len(source),remote_samples=len(remote),victim_scene_samples=len(victim),
  impulses_queued=after['throwQueued']-before['throwQueued'],video_sha256=pickup['video_sha256'],actual_capture_hz=pickup['actual_capture_hz'],
  dll_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],complete=False,visual_accepted=False,
  scope='Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.')
 (f/'fetal-release-validation.json').write_text(json.dumps(result,indent=2));print(json.dumps(dict(take=take,**result)))
 if not result['passed']:raise SystemExit(1)
