"""Read-only sequential-snapshot diagnostics, not visual acceptance."""
import json,math,sys,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def world(s,name):
    p=next(j['globalPose'][12:15]for j in s['joints']if j['nativeName']==name);r=s['root']
    return [r[k]+sum(r[3+c*3+k]*p[c]for c in range(3))for k in range(3)]
def local(s,p):
    r=s['root'];d=[p[k]-r[k]for k in range(3)]
    return [sum(r[3+c*3+k]*d[k]for k in range(3))for c in range(3)]
for take in sys.argv[1:]:
    f=P/take;trace=read(f/'contact-trace.json');rows=[];rejected=[]
    for r in trace:
        c=r['carrier'];v=r['victim'];before=r['movementBefore'];after=r['movementAfter']
        assert c['stable'] and v['stable']
        if after['carryVariant']not in [7,1] or after['carryAuthoredAmount']<before['carryAuthoredAmount']:
            rejected.append(dict(beginMs=r['beginMs'],reason='Crossed final hoist boundary; cannot average wrapped amount'));continue
        h=world(v,'left_femur_jnt');k=world(v,'left_tibia_jnt')
        left=local(c,[h[i]+.72*(k[i]-h[i])for i in range(3)])
        right=local(c,world(v,'center_spine03_jnt'))
        left=[left[i]+[.015,-.065,.035][i]for i in range(3)]
        right=[right[i]+[-.015,-.035,-.05][i]for i in range(3)]
        lw=local(c,world(c,'left_wrist_jnt'));rw=local(c,world(c,'right_wrist_jnt'))
        rows.append(dict(phase=(before['carryAuthoredAmount']+after['carryAuthoredAmount'])/2,
            phase_span=after['carryAuthoredAmount']-before['carryAuthoredAmount'],read_span_ms=r['endMs']-r['beginMs'],
            native_read_span_ms=r['readEndTick']-r['readBeginTick']if 'readEndTick'in r else None,
            left_target=left,right_target=right,left_wrist=lw,right_wrist=rw,left_error_m=math.dist(lw,left),right_error_m=math.dist(rw,right)))
    out=dict(take=take,source_sha256=hashlib.sha256((f/'contact-trace.json').read_bytes()).hexdigest(),samples=rows,
        rejected=rejected,scope='Sequential runtime skeleton reads, expressed in carrier model coordinates. Palm offsets are authoring hypotheses. No atomic-pose or mesh-contact claim.',
        visual_accepted=False,complete=False)
    (f/'contact-analysis.json').write_text(json.dumps(out,indent=2))
    print(json.dumps(dict(take=take,samples=len(rows),max_read_span_ms=max(r['read_span_ms']for r in rows),max_left_error_m=max(r['left_error_m']for r in rows),rows=rows)))
