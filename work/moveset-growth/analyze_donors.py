"""Inspect generated displacement and find comparable left-contact cycle cuts."""
import json,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent
report=[]
for file in sorted((P/'donors').glob('*.json')):
    d=json.loads(file.read_text());fs=d['frames'];names=d['names'];root=Matrix.Identity(4);track=[]
    for f in fs:
        track.append(root.copy());a=f['root_delta']
        root=root@Matrix.LocRotScale(Vector(a[:3]),Quaternion((a[6],*a[3:6])),Vector(a[7:10]))
    delta=track[360].translation-track[120].translation
    intended=Vector((math.sin(math.radians(d['request']['angle'])),math.cos(math.radians(d['request']['angle'])),0))
    travel=delta.length*.01;alignment=delta.normalized().dot(intended) if delta.length>.001 else None
    joints=[names.index(n) for n in ('pelvis','spine_01','spine_05','thigh_l','calf_l','foot_l','thigh_r','calf_r','foot_r')]
    def score(a,b):
        total=0
        for j in joints:
            qa=fs[a]['local'][j][3:7];qb=fs[b]['local'][j][3:7]
            dot=min(1,abs(sum(x*y for x,y in zip(qa,qb))))
            total+=(2*math.acos(dot))**2
        total+=sum((x-y)**2 for x,y in zip(fs[a]['contacts'],fs[b]['contacts']))*.5
        return total
    period=range(42,86) if d['name'].startswith('walk_') else range(32,81)
    candidates=[]
    for a in range(140,270):
        # Align cycles at left support entry to avoid swapping stance feet.
        if not(fs[a]['contacts'][0]>=.70 and fs[a-1]['contacts'][0]<.70):continue
        for n in period:
            b=a+n
            if b>=380:continue
            # Exactly one left support cycle, not two short cycles. Blending
            # a double cycle into a single cycle swaps the planted foot.
            if not(fs[b]['contacts'][0]>=.70 and fs[b-1]['contacts'][0]<.70):continue
            if any(fs[k]['contacts'][0]>=.70 and fs[k-1]['contacts'][0]<.70 for k in range(a+1,b)):continue
            candidates.append((score(a,b),a,b))
    best=min(candidates) if candidates else None
    item={'name':d['name'],'steady_speed_m_s':travel/4,'requested_speed_m_s':d['request']['speed']/100,
      'direction_alignment':alignment,'steady_delta_m':[v*.01 for v in delta],
      'yaw_end_degrees':math.degrees(track[-1].to_euler().z),'loop_candidate':None,'accepted_as_native_animation':False}
    if best:
        cost,a,b=best;dist=(track[b].translation-track[a].translation).length*.01
        item['loop_candidate']={'start':a,'end':b,'frames':b-a,'seconds':(b-a)/60,'stride_m':dist,'pose_cost':cost,
            'contacts_at_start':fs[a]['contacts'],'contacts_at_end':fs[b]['contacts']}
    report.append(item)
(P/'donor-analysis.json').write_text(json.dumps(report,indent=2))
for r in report:print(r['name'],round(r['steady_speed_m_s'],3),None if r['direction_alignment'] is None else round(r['direction_alignment'],3),r['loop_candidate'])
