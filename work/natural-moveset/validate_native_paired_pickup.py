"""Scoped victim ownership and cleanup checks; contact acceptance remains visual."""
import json,sys,math,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def word(obj,off):return int(next(w['word']for w in obj['words']if w['offset']==off),16)
def point(s,name):
    p=next(j['globalPose'][12:15]for j in s['joints']if j['nativeName']==name);r=s['root']
    return [r[k]+sum(r[3+c*3+k]*p[c]for c in range(3))for k in range(3)]
for take in sys.argv[1:]:
    f=P/take;v=read(f/'validation.json');i=read(f/'identity.json');rows=read(f/'observations.json')
    assert i['paired']
    active=[r for r in rows if r['attachment']['active']]
    snapshots=[r for r in active if r['attachment']['snapshotBound']]
    calibrated=[r for r in active if r['attachment']['anchorCalibrated'] and not r['attachment']['restoring']]
    post=[r for r in rows if r['beginMs']>i['cancelMs']+1500]
    checks=dict(carrier_checks=v['passed'],paired_request_made=i['pairMs']is not None,
        preparation_holds_floor=any(r['movement']['carryVariant']==7 and r['attachment']['pickupBlend']==0 for r in snapshots),
        intermediate_source_driven_blend=any(0<r['attachment']['pickupBlend']<1 for r in snapshots),
        no_premature_attachment=all(r['attachment']['attachmentPending']for r in snapshots),
        attachment_after_snapshot_restore=bool(calibrated) and all(r['attachment']['attachmentStartedAfterMs']>=r['attachment']['snapshotRestoredAfterMs']+50 for r in calibrated),
        native_loaded_scene_held=bool(calibrated) and all(r['attachment']['nativeVictimSceneActive']for r in calibrated),
        same_target_throughout=bool(active) and all(r['attachment']['targetHandle']==int(i['corpse'],0)for r in active),
        retired_after_cancel=bool(post) and all(not r['attachment']['active'] and not r['attachment']['snapshotBound'] and not r['attachment']['nativeVictimSceneActive']for r in post),
        retarget_removed=read(f/'retarget-after.json')['count']==0,
        native_ragdoll_restored=word(read(f/'physics-after.json'),'0x238')==0xffffffff00000000,
        corpse_health_preserved=read(f/'corpse-health-after.json')['current']==0)
    a=read(f/'carrier-held.json');b=read(f/'victim-held.json');hand=point(a,'left_wrist_jnt');hip=point(b,'left_femur_jnt');knee=point(b,'left_tibia_jnt')
    segment=[hip[k]+.72*(knee[k]-hip[k])for k in range(3)]
    contact=math.dist(hand,segment)
    out=dict(passed=all(checks.values()),checks=checks,active_samples=len(active),snapshot_samples=len(snapshots),held_samples=len(calibrated),
        support_wrist_to_upper_leg_center_m=contact,contact_metric_scope='Distance to sampled leg centerline, without mesh surface or palm offset. Static held samples only; not proof of grip or transfer quality.',
        video_sha256=v['video_sha256'],actual_capture_hz=v['actual_capture_hz'],complete=False,visual_accepted=False)
    (f/'paired-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
    if not out['passed']:sys.exit(1)
