"""Append full-body crouched grenade candidates; never writes game assets or root."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['grenade-authoring-manifest-v55.json'],'crouched-grenade-sources-v62')
O=P/'crouched-grenade-banks-v62';O.mkdir(exist_ok=True);reports=[]
parts=['idle','equip','unequip','cooking_in','cooking_idle','throw_high','throw_high_ooa','throw_low','throw_low_ooa']
parents=read(P/'stun-bank-manifest-v61.json')
for role in ('corvo','emily'):
    rig=rig_for(role);n=len(rig.joints);parent=next(m for m in parents if m['role']==role)
    data=(P/parent['bank']).read_bytes();assert hashlib.sha256(data).hexdigest()==parent['sha256']
    magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert (magic,version,count,jn)==(b'D2MOVE43',43,264,n)
    entry=24+121*n*28
    rest=[qts(struct.unpack_from('<7f',data,32+17*entry+24+j*28))for j in range(n)]
    fpPath=P.parent/'mixamo-crouch'/('host-fp-crouch.json' if role=='corvo' else 'client-fp-crouch.json')
    fp=read(fpPath);assert fp['stable'] and fp['armSide']==2 and fp['armCharacter']==(1 if role=='corvo' else 2)
    wrist=fp['joints'][4];assert wrist['nativeName']=='right_wrist_jnt'
    fpPose=Matrix([wrist['localPose'][k:k+4]for k in range(0,16,4)]).transposed()
    rest[16]=(rig.bind[16].to_3x3()@qts(wrist['bindLocalQTS']).to_3x3().inverted()@fpPose.to_3x3()).to_4x4()
    rest[16].translation=rig.bind[16].translation
    restG=rig.fk(rest);right=[]
    for j in range(n):
        k=j
        while k>=0 and k!=16:k=rig.joints[k]['parent']
        if k==16 and j!=16 and 'helper' not in rig.joints[j]['nativeName']:right.append(j)
    idle,reference=L.load(role,'grenade_idle');ref=reference[0];refG=rig.fk(ref)
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE44',44,n,273,fps,layout)+data[32:]);sources=[]
    for index,part in enumerate(parts):
        m,frames=L.load(role,'grenade_'+part);cid=264+index;duration=m['duration'];errors=[];heights=[]
        def authored(t):
            donor=sample(frames,t);donorG=rig.fk(donor);pose=copy_pose(donor)
            # Preserve the authored acting while rebasing spine/pelvis/limbs
            # onto the existing complete Posture-100 crouch. No camera anchors.
            for j in range(2,27):
                if j in (10,17):continue
                delta=ref[j].to_quaternion().inverted()@donor[j].to_quaternion()
                gain=.65 if j in (2,18) else .85 if j in (3,4,5) else 1.
                q=(rest[j].to_quaternion()@Quaternion().slerp(delta,gain)).normalized()
                pose[j]=q.to_matrix().to_4x4();pose[j].translation=donor[j].translation.copy()
            pose[2].translation=rest[2].translation+(donor[2].translation-ref[2].translation)*.6
            for j in right:pose[j]=rest[j].copy()
            pose[17]=rest[17].copy()
            # Throwing arm follows the changing body-space donor directions;
            # native wrist-relative grenade attachment/mechanism tracks survive.
            for j in (7,8,9):rig.orient(pose,j,donorG[j].to_3x3())
            for j,weight in ((11,.6),(12,.85)):
                g=rig.fk(pose);rig.orient(pose,j,g[j].to_quaternion().slerp(donorG[j].to_quaternion(),weight).to_matrix())
            targets=[restG[i].translation+(donorG[i].translation-refG[i].translation)*.25 for i in (21,25)]
            pin_feet(rig,pose,targets,[restG[i].to_3x3()for i in (21,25)])
            g=rig.fk(pose);errors.append(max((g[i].translation-targets[k]).length for k,i in enumerate((21,25))));heights.append(pose[2].translation.z)
            assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
            return pose
        poses=[authored(f/119)for f in range(120)]
        assert max(errors)<.002 and min(heights)>.5,(role,part,max(errors),min(heights))
        bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
        for pose in poses+[poses[0]]:
            for v in pose:bank.extend(struct.pack('<7f',*row(v)))
        preview=[authored(f/max(1,round(duration*60)))for f in range(max(1,round(duration*60))+1)]
        L.save(rig,role,'grenade_crouched_'+part,preview,[identity()for _ in preview],'ranged_and_items',
            'Full-body Posture-100 crouched grenade acting; animated shoulders, spine and native-length legs. Native crouch local sword-wrist baseline, no camera-space anchors. Network owns position; pure pose contains no native grenade events. Visual candidate only.',
            [m['sha256'],idle['sha256'],parent['sha256'],hashlib.sha256(fpPath.read_bytes()).hexdigest()],
            props=m.get('props',[]),native_duration_ms=round(duration*1000),max_foot_error=max(errors),minimum_pelvis_height=min(heights))
        sources.append(dict(clip=cid,variant=index,part=part,duration=duration,source=m['sha256'],max_foot_error=max(errors)))
    path=O/f'{role}-crouched-grenade-bank.bin';path.write_bytes(bank);assert bank[32:len(data)]==data[32:]
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),parent=parent['sha256'],format='D2MOVE44',clips=273,prior_264_payloads_unchanged=True,sources=sources,runtime_verified=False,visual_accepted=False))
L.finish('crouched-grenade-source-manifest-v62.json')
(P/'crouched-grenade-bank-manifest-v62.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('CROUCHED_GRENADE_V44: nine full-body phases per role; visual candidate only',flush=True)
