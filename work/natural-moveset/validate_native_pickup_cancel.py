"""Check scoped cancellation before source-timed victim attachment."""
import json,sys,math
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def word(p,off):return int(next(w['word']for w in read(p)['words']if w['offset']==off),16)
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');rows=read(f/'observations.json');v=read(f/'validation.json');cancel=read(f/'cancel-before.json')
 active=[r for r in rows if r['attachment']['snapshotBound']]
 after=[r for r in rows if r['beginMs']>i['cancelMs']+1200]
 initial=rows[0]['attachment'];last=rows[-1]['attachment'];a=cancel['attachment']
 root0=read(f/'corpse-before.json')['root'][:3];root1=read(f/'corpse-after.json')['root'][:3]
 checks=dict(carrier_checks=v['passed'],cancel_during_transfer=a['snapshotBound']and a['attachmentPending']and .05<=a['pickupBlend']<.65,
  no_attachment_created=last['started']==initial['started']and all(not r['attachment']['anchorCalibrated']for r in rows),
  one_release=last['released']==initial['released']+1,
  retired_within_1200ms=bool(after)and all(not r['attachment']['active']and not r['attachment']['snapshotBound']and not r['attachment']['nativeVictimSceneActive']for r in after),
  retarget_absent=read(f/'retarget-after.json')['count']==0,
  native_ragdoll_restored=word(f/'physics-after.json','0x238')==0xffffffff00000000,
  corpse_dead=read(f/'corpse-health-after.json')['current']==0,
  corpse_remains_local=math.dist(root0,root1)<2.5,
  movement_and_stream_retired=not read(f/'stopped.json')['movement']['active']and not read(f/'stopped.json')['stream']['armed'])
 out=dict(passed=all(checks.values()),checks=checks,snapshot_samples=len(active),cancel_ms=i['cancelMs'],
  corpse_root_displacement_m=math.dist(root0,root1),video_sha256=v['video_sha256'],actual_capture_hz=v['actual_capture_hz'],
  scope=i['scope'],complete=False,visual_accepted=False)
 (f/'cancel-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
 if not out['passed']:sys.exit(1)
