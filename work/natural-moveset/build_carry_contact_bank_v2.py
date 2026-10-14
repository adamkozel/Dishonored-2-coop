"""Separate v32 crouched-contact experiment; v1 banks remain byte-identical.

Retain the crouch base's world-space weapon arm and gaze. Standing local arm
rotations previously inherited the deep crouch spine bend. Supporting fingers
use the authored native grasp, and the hand seeks the measured victim leg.
No first-person wrist anchors, scale changes, root motion or native events.
"""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from contact_hand import descendants
from rig_math import ease
L=Library([],'carry-contact-bank-sources-v2');O=P/'carry-contact-banks-v2';O.mkdir(exist_ok=True)
victim_file=P/'carry-victim-native-emily-crouched-v13/corpse-skeleton-held.json'
victim=read(victim_file);assert victim['stable']
V={j['nativeName']:Vector(j['globalPose'][12:15])for j in victim['joints']}
leg=V['left_femur_jnt'].lerp(V['left_tibia_jnt'],.72)
support_delta=Vector((.025,.19,.115))-V['center_spine02_jnt']+leg+Vector((.015,-.065,.035))
reports=[]
for role in ['corvo','emily']:
    rig=rig_for(role);n=len(rig.joints)
    source=P/'carry-release-banks-v1'/f'{role}-carry-release-bank.bin';data=source.read_bytes()
    assert struct.unpack_from('<8sIIII',data)==(b'D2MOVE32',32,n,231,60)
    entry=24+121*n*28;bank=bytearray(data)
    def pose_at(clip,u):
        x=max(0,min(1,u))*119;a=int(x);b=min(119,a+1)
        return [blend([qts(struct.unpack_from('<7f',data,32+clip*entry+24+(a*n+j)*28))],
                      [qts(struct.unpack_from('<7f',data,32+clip*entry+24+(b*n+j)*28))],x-a)[0]for j in range(n)]
    loaded=pose_at(225,0);loadedG=rig.fk(loaded);left_children=descendants(rig,9);right_children=descendants(rig,16)
    metrics=[]
    for cid in [227,228,230]:
        frames=[];errors=[];arm_diffs=[];baseclip=18 if cid==228 else 17
        for f in range(120):
            u=f/119;p=pose_at(cid,u);low=pose_at(baseclip,u if cid==228 else 0);lowG=rig.fk(low)
            # Keep the head looking along the original crouch gaze, and keep
            # the sword arm in the authored crouch guard through release.
            for j in [11,12,13,14,15,16]:rig.orient(p,j,lowG[j].to_3x3())
            for j in right_children:p[j]=low[j].copy()
            if cid!=230:
                g=rig.fk(p);goal=g[7].translation+support_delta
                rig.solve(p,7,8,9,goal,g[7].translation+Vector((.15,.25,-.35)))
                rig.orient(p,9,loadedG[9].to_3x3())
                for j in left_children:p[j]=loaded[j].copy()
                errors.append((rig.fk(p)[9].translation-goal).length)
            else:
                # Smoothly leave the corrected support pose into the existing
                # release candidate; this does not invent a different drop.
                fixed=copy_pose(p);g=rig.fk(fixed);goal=g[7].translation+support_delta
                rig.solve(fixed,7,8,9,goal,g[7].translation+Vector((.15,.25,-.35)))
                rig.orient(fixed,9,loadedG[9].to_3x3())
                for j in left_children:fixed[j]=loaded[j].copy()
                weight=1-ease(u/.35)
                for j in [7,8,9,*left_children]:p[j]=blend([p[j]],[fixed[j]],weight)[0]
            preview_helpers(rig,p);frames.append(p)
        offset=32+cid*entry+24
        for f,p in enumerate(frames+[frames[0]]):
            for j,mat in enumerate(p):struct.pack_into('<7f',bank,offset+(f*n+j)*28,*row(mat))
        alias={227:'crouched_carry_idle_contact',228:'crouched_carry_walk_contact',230:'crouched_carry_low_drop_contact'}[cid]
        L.save(rig,role,alias,frames,[identity()for _ in frames],'interactions',
            'Crouched carry contact candidate. Full crouched torso/legs retained; gaze and sword arm keep authored crouch orientation, supporting hand uses measured native victim geometry. Runtime comparison pending.',
            dict(parent_sha256=hashlib.sha256(data).hexdigest(),victim_snapshot_sha256=hashlib.sha256(victim_file.read_bytes()).hexdigest()),
            bank_clip=cid,max_support_error_m=max(errors,default=0))
        metrics.append(dict(clip=cid,max_support_error_m=max(errors,default=0)))
    for cid in range(231):
        if cid not in [227,228,230]:assert bank[32+cid*entry:32+(cid+1)*entry]==data[32+cid*entry:32+(cid+1)*entry]
    target=O/f'{role}-carry-contact-bank.bin';target.write_bytes(bank)
    reports.append(dict(role=role,bank=str(target.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),
        parent_sha256=hashlib.sha256(data).hexdigest().upper(),changed_clips=[227,228,230],other_clips_unchanged=True,
        metrics=metrics,visual_accepted=False,runtime_verified=False))
L.finish('carry-contact-bank-source-manifest-v2.json')
(P/'carry-contact-bank-manifest-v2.json').write_text(json.dumps(reports,indent=2))
print('CARRY_CONTACT_V2',json.dumps(reports),flush=True)
