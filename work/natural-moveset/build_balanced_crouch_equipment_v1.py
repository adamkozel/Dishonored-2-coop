"""Replace only crouched sword draw/holster in the v30 candidate bank.

Use the current balanced stance, preserve native contact phases, and solve
body-relative hand/holster contact without stretching the anatomical rig.
"""
import sys, struct, json, hashlib, math
from pathlib import Path
P=Path(__file__).resolve().parent; sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json','stance-retarget-manifest.json'],'balanced-crouch-equipment-sources-v1')
O=P/'crouched-ranged-banks-v4'; O.mkdir(exist_ok=True)
attachments=read(P/'native-shadow-equipment-v6/samples.json')
original=read(P/'equipment-bank-manifest-v3.json'); reports=[]
for role in ('corvo','emily'):
    rig=rig_for(role); n=len(rig.joints)
    source=P/'crouched-ranged-banks-v3'/f'{role}-crouched-ranged-bank.bin'
    data=source.read_bytes(); assert struct.unpack_from('<8sIIIIQ',data)[:4]==(b'D2MOVE30',30,n,225)
    bank=bytearray(data); entry=24+121*n*28
    def previous(cid,frame=0):
        return [qts(struct.unpack_from('<7f',data,32+cid*entry+24+(frame*n+j)*28)) for j in range(n)]
    stand=previous(0); low=previous(17); sg=rig.fk(stand); lg=rig.fk(low)
    roleSamples='client' if role=='corvo' else 'host'
    drawn=next(x['attachment'] for x in attachments if x['role']==roleSamples and x['label']=='expired-restored')
    holstered=next(x['attachment'] for x in attachments if x['role']==roleSamples and x['label']=='holstered')
    def attachment(a):
        q=a['quaternion'];return qts([-q[0],-q[1],-q[2],q[3]]+a['offset'])
    D=attachment(drawn); H=attachment(holstered)
    contacts=[]
    for cid,alias in ((87,'sword_equipsword'),(88,'sword_unequipsword')):
        m,fs=L.load(role,alias); poses=[]
        for f in range(120):
            donor=sample(fs,f/119); dg=rig.fk(donor); p=copy_pose(donor)
            p[2].translation+=low[2].translation-stand[2].translation
            for j in (2,3,4,5):
                q=low[j].to_quaternion().slerp(p[j].to_quaternion(),.35)
                t=p[j].translation.copy();p[j]=q.normalized().to_matrix().to_4x4();p[j].translation=t
            pin_feet(rig,p,[dg[j].translation+lg[j].translation-sg[j].translation for j in (21,25)],
                     [dg[j].to_3x3() for j in (21,25)])
            poses.append(p)
        # Preserve the live DLL's handoff phase exactly, not a newly fitted
        # nearest frame that would disagree with its attachment transition.
        old=next(c for r in original if r['role']==role for c in r['contacts'] if c['clip']==cid)
        nearest=old['frame']
        def target(p):return rig.fk(p)[195]@H@D.inverted()@p[17].inverted()
        peak=copy_pose(poses[nearest]); torso=Quaternion()
        axes=[Vector((1,0,0)),Vector((0,1,0)),Vector((0,0,1))]
        reach=rig.bind[15].translation.length+rig.bind[16].translation.length-.012
        def chest(q,p):
            p=copy_pose(p);t=p[4].translation.copy()
            p[4]=(q@p[4].to_quaternion()).normalized().to_matrix().to_4x4();p[4].translation=t
            return p
        def score(q):
            p=chest(q,peak);g=rig.fk(p);excess=max(0,(target(p).translation-g[14].translation).length-reach)
            return excess*excess+q.angle*q.angle*.00002
        for step in (6,3,1):
            for iteration in range(14):
                options=[torso]+[(Quaternion(a,math.radians(step*sign))@torso).normalized() for a in axes for sign in (-1,1)]
                options=[q for q in options if q.angle<math.radians(28)]
                best=min(options,key=score)
                if best==torso:break
                torso=best
        errors=[]; angles=[]; heights=[]; feet=[]
        for f,p in enumerate(poses):
            before=rig.fk(p);w=ease(1-abs(f-nearest)/23)
            if w:
                p=chest(Quaternion().slerp(torso,w),p);poses[f]=p
                desired=target(p);g=rig.fk(p)
                rotation=g[16].to_quaternion().slerp(desired.to_quaternion(),w).to_matrix()
                rig.solve(p,14,15,16,g[16].translation.lerp(desired.translation,w),g[15].translation)
                rig.orient(p,16,rotation);preview_helpers(rig,p)
            g=rig.fk(p);heights.append(p[2].translation.z)
            feet.append(max((g[j].translation-before[j].translation).length for j in (21,25)))
            assert max((p[j].translation-rig.bind[j].translation).length for j in range(3,27) if j!=17)<1e-5
            if f==nearest:
                a=g[17]@D;b=g[195]@H
                errors.append((a.translation-b.translation).length)
                angles.append(math.degrees(a.to_quaternion().rotation_difference(b.to_quaternion()).angle))
        assert max(errors)<.02,(role,alias,'contact',errors)
        assert max(feet)<1e-5 and min(heights)>.62,(role,alias,feet,heights)
        payload=b''.join(struct.pack('<7f',*row(v)) for p in poses+[poses[0]] for v in p)
        start=32+cid*entry+24;bank[start:start+len(payload)]=payload
        duration=struct.unpack_from('<f',data,32+cid*entry+20)[0]
        L.save(rig,role,'balanced_crouch_'+alias,poses,[identity() for _ in poses],'equipment',
               'Balanced complete crouch with native body-relative sword handoff. Original native contact phase retained; no anatomical stretching or first-person anchors.',
               [m['sha256'],hashlib.sha256(data).hexdigest()],contact_frame=nearest,contact_amount=nearest/119,
               contact_position_error=max(errors),contact_rotation_error=max(angles),minimum_pelvis_height=min(heights),native_duration_ms=round(duration*1000))
        contacts.append(dict(clip=cid,frame=nearest,amount=nearest/119,position_error=max(errors),rotation_error=max(angles),minimum_pelvis_height=min(heights)))
    for c in range(225):
        if c not in (87,88):assert bank[32+c*entry:32+(c+1)*entry]==data[32+c*entry:32+(c+1)*entry]
    path=O/f'{role}-crouched-ranged-bank.bin';path.write_bytes(bank)
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),parent=hashlib.sha256(data).hexdigest(),
                        changed_clips=[87,88],contacts=contacts,format='D2MOVE30',clips=225,runtime_verified=False,visual_accepted=False))
L.finish('balanced-crouch-equipment-source-manifest-v1.json')
(P/'crouched-ranged-bank-manifest-v4.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('BALANCED_CROUCH_EQUIPMENT 87/88 only, native handoff phases preserved',flush=True)
