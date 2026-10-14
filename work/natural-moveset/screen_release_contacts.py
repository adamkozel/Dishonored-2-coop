"""Summarize separately bracketed published skeleton reads; never visual acceptance."""
import json,math,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def bend(a,b,c):
    u=[x-y for x,y in zip(a,b)];v=[x-y for x,y in zip(c,b)]
    den=math.sqrt(sum(x*x for x in u)*sum(x*x for x in v))
    return 180-math.degrees(math.acos(max(-1,min(1,sum(x*y for x,y in zip(u,v))/den)))) if den else None
for take in sys.argv[1:]:
    folder=P/take;rows=json.loads((folder/'contact-trace.json').read_text(encoding='utf-8-sig'));out=[]
    for row in rows:
        m=row['movementBefore']
        if m['carryVariant'] not in [5,8]:continue
        s=row['victim'];j={x['nativeName']:x['globalPose'][12:15] for x in s['joints']}
        out.append(dict(begin_ms=row['beginMs'],end_ms=row['endMs'],variant=m['carryVariant'],amount=m['carrySampleAmount'],
          root=s['root'][:3],anchor=j.get('anchor_jnt'),head=j.get('center_head_jnt'),
          knees={side:bend(*(j[f'{side}_{x}_jnt'] for x in ['femur','tibia','ankle'])) for side in ['left','right']}))
    (folder/'release-contact-screening.json').write_text(json.dumps(dict(atomic=False,visual_accepted=False,samples=out),indent=2))
    print(json.dumps(dict(take=take,samples=out)))
