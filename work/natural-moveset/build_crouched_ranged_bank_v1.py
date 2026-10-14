"""Reversible full-body crouched ranged candidates; retain all v29 clips bytewise."""
import sys, struct, json, hashlib
from pathlib import Path
P=Path(__file__).resolve().parent; sys.path.insert(0,str(P))
from clip_io import *
L=Library(['ranged-bank-source-manifest-v1.json'],'crouched-ranged-sources-v1')
O=P/'crouched-ranged-banks-v1'; O.mkdir(exist_ok=True); reports=[]
parts=['idle','ooa','fire','reload','equip','unequip']
for role in ('corvo','emily'):
    rig=rig_for(role); n=len(rig.joints)
    data=(P/'ranged-banks-v1'/f'{role}-ranged-bank.bin').read_bytes()
    magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE29' and version==29 and count==213 and jn==n
    entry=24+121*n*28
    rest=[qts(struct.unpack_from('<7f',data,32+17*entry+24+j*28)) for j in range(n)]
    restG=rig.fk(rest); bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE30',30,n,225,fps,layout)+data[32:]); sources=[]
    right=[]
    for j in range(n):
        k=j
        while k>=0 and k!=16:k=rig.joints[k]['parent']
        if k==16 and j!=16 and 'helper' not in rig.joints[j]['nativeName']:right.append(j)
    for kind in ('gun','crossbow'):
        idle,reference=L.load(role,'ranged_native_'+kind+'_idle'); ref=reference[0]; refG=rig.fk(ref)
        for slot,part in enumerate(parts):
            m,fs=L.load(role,'ranged_native_'+kind+'_'+part)
            cid=213+(0 if kind=='gun' else 6)+slot
            duration=struct.unpack_from('<f',data,32+(cid-12)*entry+20)[0]
            errors=[]; heights=[]
            def authored(t):
                donor=sample(fs,t); donorG=rig.fk(donor); pose=copy_pose(donor)
                # Rebase the changing whole-body performance onto the balanced
                # crouch. Do not scale bones or pin wrists to first-person space.
                for j in range(2,27):
                    if j in (10,17):continue
                    delta=ref[j].to_quaternion().inverted() @ donor[j].to_quaternion()
                    gain=.65 if j in (2,18) else .85 if j in (3,4,5) else 1.
                    q=(rest[j].to_quaternion() @ Quaternion().slerp(delta,gain)).normalized()
                    pose[j]=q.to_matrix().to_4x4(); pose[j].translation=donor[j].translation.copy()
                pose[2].translation=rest[2].translation+(donor[2].translation-ref[2].translation)*.6
                for j in right:pose[j]=rest[j].copy()
                pose[17]=rest[17].copy()
                # Gun hand follows animated authoring-space directions, reparented
                # to the moving crouched shoulders. Local grip/prop tracks stay intact.
                for j in (7,8,9):rig.orient(pose,j,donorG[j].to_3x3())
                targets=[restG[i].translation+(donorG[i].translation-refG[i].translation)*.25 for i in (21,25)]
                rotations=[restG[i].to_3x3() for i in (21,25)]
                pin_feet(rig,pose,targets,rotations); g=rig.fk(pose)
                errors.append(max((g[i].translation-targets[k]).length for k,i in enumerate((21,25))))
                heights.append(pose[2].translation.z)
                assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27) if j!=17)<1e-5
                return pose
            poses=[authored(f/119) for f in range(120)]
            assert max(errors)<.002 and min(heights)>.5,(role,part,max(errors),min(heights))
            bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
            for pose in poses+[poses[0]]:
                for v in pose:bank.extend(struct.pack('<7f',*row(v)))
            preview=[authored(f/max(1,round(duration*60))) for f in range(max(1,round(duration*60))+1)]
            L.save(rig,role,'ranged_crouched_'+kind+'_'+part,preview,[identity() for _ in preview],'ranged_and_items',
                'Complete crouched body adapted from animated ranged acting, balanced Posture-100 stance and native anatomical lengths. Animated gun-arm directions, changing spine/pelvis and planted feet; no fixed first-person wrists. Native source owns position, clock and gameplay. Visual candidate only.',
                [m['sha256'],idle['sha256'],hashlib.sha256(data).hexdigest()],native_duration_ms=round(duration*1000),max_foot_error=max(errors),minimum_pelvis_height=min(heights))
            sources.append(dict(clip=cid,kind=kind,part=part,duration=duration,source=m['sha256']))
    path=O/f'{role}-crouched-ranged-bank.bin';path.write_bytes(bank)
    assert bank[32:len(data)]==data[32:]
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),parent=hashlib.sha256(data).hexdigest(),format='D2MOVE30',clips=225,prior_payload_unchanged=True,sources=sources,runtime_verified=False,visual_accepted=False))
L.finish('crouched-ranged-source-manifest-v1.json')
(P/'crouched-ranged-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('CROUCHED_RANGED_V30 12 new complete-body clips per role; visual review pending',flush=True)
