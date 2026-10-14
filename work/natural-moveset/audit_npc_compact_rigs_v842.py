from pathlib import Path
import json,math,struct
P=Path(__file__).resolve().parent/'npc-rig-census-v839'
def quat(m):
    q=[0.]*4;t=m[0]+m[4]+m[8]
    if t>0:
        s=math.sqrt(t+1)*2;q=[(m[5]-m[7])/s,(m[6]-m[2])/s,(m[1]-m[3])/s,s*.25]
    else:
        i=1 if m[4]>m[0] else 0
        if m[8]>m[i*3+i]:i=2
        j=(i+1)%3;k=(i+2)%3;s=math.sqrt(max(0,1+m[i*3+i]-m[j*3+j]-m[k*3+k]))*2
        q[i]=s*.25;q[j]=(m[j*3+i]+m[i*3+j])/s;q[k]=(m[k*3+i]+m[i*3+k])/s;q[3]=(m[j*3+k]-m[k*3+j])/s
    n=math.sqrt(sum(v*v for v in q));return [v/n for v in q]
def rot(q):
    x,y,z,w=q
    return [1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y)]
def round_away(x):return math.floor(x+.5) if x>=0 else math.ceil(x-.5)
rows=[]
for f in sorted(P.glob('*-ld_spawner_*.json')):
    s=json.loads(f.read_text(encoding='utf-8-sig'));position_error=matrix_error=angle_error=0
    for j in s['joints']:
        m=j['localPose'];r=[m[c*4+a] for c in range(3) for a in range(3)];position=m[12:15];q=quat(r)
        encoded=[round_away(v*32767) for v in q]+[round_away(v*10000) for v in position]
        wire=struct.pack('<7h',*encoded);assert len(wire)==14
        decoded=struct.unpack('<7h',wire);dq=[v/32767 for v in decoded[:4]];n=math.sqrt(sum(v*v for v in dq));dq=[v/n for v in dq]
        dr=rot(dq);dp=[v/10000 for v in decoded[4:]]
        position_error=max(position_error,max(abs(a-b) for a,b in zip(position,dp)))
        matrix_error=max(matrix_error,max(abs(a-b) for a,b in zip(r,dr)))
        angle_error=max(angle_error,2*math.acos(min(1,abs(sum(a*b for a,b in zip(q,dq)))))*180/math.pi)
    count=len(s['joints']);assert position_error<=.00005001 and angle_error<.01 and matrix_error<.0001
    rows.append(dict(source=f.name,joints=count,oldBytes=176+48*count,compactBytes=176+14*count,maxTranslationErrorMeters=position_error,maxAngularErrorDegrees=angle_error,maxRotationElementError=matrix_error))
assert len(rows)==10
result=dict(passed=True,scope='Offline wire precision of the ten exact native skeleton snapshots. This does not establish acceptable visual animation or runtime networking.',rows=rows)
(P/'compact-wire-audit-v842.json').write_text(json.dumps(result,indent=2))
print(json.dumps(dict(passed=True,rigSnapshots=len(rows),nativeJoints=sum(r['joints'] for r in rows),maxTranslationErrorMeters=max(r['maxTranslationErrorMeters'] for r in rows),maxAngularErrorDegrees=max(r['maxAngularErrorDegrees'] for r in rows))))
