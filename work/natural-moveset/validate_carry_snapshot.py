"""Bounded snapshot/attachment handoff evidence, not animation acceptance."""
import json,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
folder=P/sys.argv[1];rows=read(folder/'observations.json');base=read(folder/'validation.json')
captured=[r for r in rows if r.get('attachment',{}).get('snapshotCaptured')]
early=[r for r in captured if 50<=r['attachment']['snapshotElapsedMs']<=400]
pending=[r for r in early if r['attachment'].get('attachmentPending')]
attached=[r for r in captured if r.get('retarget',{}).get('count')==1]
stamps=[r['attachment'] for r in captured if r['attachment'].get('attachmentStartedAfterMs',0)>0]
initial_z=rows[0]['corpsePose']['root'][2]
raw_root_shift=max((abs(r['corpsePose']['root'][2]-initial_z) for r in early),default=999)
# Capture transforms the old floor pose into the carrier root before that root
# is installed. Compare against that intended root, not the preceding ragdoll
# origin; the latter differs in height even when world-space joints are held.
early_deviation=max((abs(r['corpsePose']['root'][2]-r['observerRoot']['probeRenderPosition'][2]) for r in early),default=999)
checks=[
    dict(check='base ownership and restoration checks pass',passed=base['mechanical_checks_passed']),
    dict(check='native snapshot bound during early blend',passed=len(early)>=2 and all(r['attachment']['snapshotBound']for r in early)),
    dict(check='retarget setup waits until blend completes',passed=len(pending)>=2 and all(r['retarget']['valid']and r['retarget']['count']==0 for r in pending)),
    dict(check='early victim root stays within 12 cm of intended carrier root',passed=early_deviation<=.12),
    # The raw status/probe pair straddles a 250ms reporting interval. Use the
    # transition stamps recorded on the engine thread for exact ordering.
    dict(check='recorded activation follows snapshot restoration and 850ms gate',passed=bool(stamps) and all(800<=a['snapshotRestoredAfterMs']<=a['attachmentStartedAfterMs'] and a['attachmentStartedAfterMs']>=850 for a in stamps)),
    dict(check='final fixture and snapshot bindings retired',passed=not rows[-1]['attachment']['active'] and not rows[-1]['attachment']['snapshotBound']),
]
out=dict(checks=checks,passed=all(c['passed']for c in checks),early_samples=len(early),
    early_root_vertical_deviation_m=early_deviation,raw_root_shift_from_ragdoll_origin_m=raw_root_shift,
    first_attachment_status_age_ms=attached[0]['attachment']['snapshotElapsedMs']if attached else None,
    transition_stamps=[dict(restored_ms=a,attached_ms=b) for a,b in sorted({(a['snapshotRestoredAfterMs'],a['attachmentStartedAfterMs'])for a in stamps})],
    video_sha256=base['video_sha256'],actual_capture_hz=base['actual_capture_hz'],
    complete=False,visual_accepted=False,scope='Native pose blend and deferred attachment only. Carrier lift acting and automatic native pickup timing remain unimplemented.')
(folder/'snapshot-validation.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out))
