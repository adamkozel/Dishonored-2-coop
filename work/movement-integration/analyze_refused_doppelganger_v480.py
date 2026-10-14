from pathlib import Path
import json, math

R = Path(__file__).resolve().parents[2]
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
take = R / 'work/natural-moveset/doppelganger-fx-v480'
rows = rd(take / 'states.json')
rig = rd(R / 'work/natural-moveset/doppelganger-fx-v469/visual-final-skeleton.json')
samples = {}
for row in rows:
    s = row.get('cloneSource') or {}
    sequence = s.get('refusedSequence', 0)
    if not sequence or sequence in samples:
        continue
    j = s['packetRefusedJoint']
    m = s['refusedJointMatrix']
    cols = [m[n:n+3] for n in (0, 3, 6)]
    dot = lambda a,b: sum(x*y for x,y in zip(a,b))
    det = (cols[0][0]*(cols[1][1]*cols[2][2]-cols[1][2]*cols[2][1])
           -cols[1][0]*(cols[0][1]*cols[2][2]-cols[0][2]*cols[2][1])
           +cols[2][0]*(cols[0][1]*cols[1][2]-cols[0][2]*cols[1][1]))
    samples[sequence] = dict(sequence=sequence, firstObservedVideoTime=row['videoTime'],
        joint=j, name=rig['joints'][j]['nativeName'], parent=rig['joints'][j]['parent'],
        children=[x['index'] for x in rig['joints'] if x['parent']==j], matrix=m,
        finite=all(math.isfinite(x) for x in m),
        rotationGram=[[dot(a,b) for b in cols] for a in cols], determinant=det,
        maxAbsTranslation=max(abs(x) for x in m[9:]),
        packetRefused=s['packetRefused'], sendFailed=s['sendFailed'])
result = dict(take=take.name, scope='Exact rejected source matrices saved inside validation failure; sampled status may omit intermediate rejected sequences.',
    samples=list(samples.values()),
    helperHierarchy=[dict(index=j, name=rig['joints'][j]['nativeName'], parent=rig['joints'][j]['parent'],
        children=[x['index'] for x in rig['joints'] if x['parent']==j]) for j in (1,10,17)],
    result=rd(take/'result.json'))
out = take/'refused-pose-analysis.json'
out.write_text(json.dumps(result,indent=2),encoding='utf-8')
print(json.dumps(dict(path=str(out),helpers=result['helperHierarchy'],
    samples=[{k:s[k] for k in ('sequence','joint','name','children','determinant','maxAbsTranslation')} for s in samples.values()]),indent=2))
