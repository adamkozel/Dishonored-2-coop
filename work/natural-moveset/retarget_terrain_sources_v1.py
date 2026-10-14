"""Retarget sampled native full-body motion, preserving anatomy and source intent.

Root motion is exported solely as preview/phase metadata. Origin and anchor in
the authored pose remain identity; no events or gameplay effects are included.
"""
import sys,json,math,hashlib,struct,argparse
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read,qts,NativeRig
from edge_data import poses,skeleton
OUT=P/'terrain-retargeted-v1';OUT.mkdir(exist_ok=True)
parser=argparse.ArgumentParser();parser.add_argument('--selection');parser.add_argument('--samples');parser.add_argument('--manifest',default='native-retarget-manifest.json');parser.add_argument('aliases',nargs='*')
args=parser.parse_args(sys.argv[sys.argv.index('--')+1:] if '--' in sys.argv else [])
manifest=read(P/args.samples if args.samples else P/'native-expanded-sample-manifest.json' if args.selection else P/'native-sample-manifest.json')
selection=read(P/args.selection) if args.selection else None
filters=args.aliases or ['sword_run_n','sword_gun_turnright180','sword_attack_fasta']
sources=[r for r in manifest['rows'] if r['status']=='native_pose_sampled_offline' and ((r['id'] in selection['sources']) if selection else (filters==['all'] or any(a in filters for a in r['aliases'])))]

def helpers(rig,local):
    J=rig.joints;BG=rig.global_bind;lookup={j['nativeName']:i for i,j in enumerate(J)};g=[]
    for i,j in enumerate(J):
        n=j['nativeName'];side='left' if n.startswith('left') else 'right' if n.startswith('right') else '';driver=None;p=j['parent']
        if side and 'helper' in n:
            if 'upper_leg' in n or 'thigh' in n:driver=side+'_femur_jnt'
            elif 'knee' in n:driver=side+'_tibia_jnt'
            elif 'arm01' in n and 'forearm' not in n:driver=side+'_arm_jnt'
            elif 'elbow' in n or 'forearm' in n:driver=side+'_elbow_jnt'
            elif 'wrist' in n:driver=side+'_wrist_jnt'
            elif 'ankle' in n:driver=side+'_ankle_jnt'
        if driver and lookup[driver]<i:
            k=lookup[driver];local[i]=g[p].inverted() @ (g[k] @ BG[k].inverted() @ BG[i])
        g.append(local[i] if p<0 else g[p]@local[i])

def row(m):
    q=m.to_quaternion().normalized();return [q.x,q.y,q.z,q.w,*m.translation]

rows=[]
for source in sources:
    part='tall' if source['source_role']=='corvo' else 'female'
    sd=read(P/f'base-{part}.json');sr=NativeRig(sd['joints']);sp=poses(P/source['sample_file']);assert sp['joints']==len(sr.joints)
    source_fps=sp['fps'];canonical_fps=round(source_fps)
    assert canonical_fps in (30,60) and abs(canonical_fps-source_fps)<.001
    sp['fps']=canonical_fps
    additive=None
    if any(a.endswith('_add') for a in source['aliases']):
        empty=all(__import__('re').fullmatch(r'empty_(walk|run)_[nesw]_add',a) for a in source['aliases'])
        if empty:
            assert source['source_role']=='corvo', 'Unproven female additive reference'
            reference=read(P/'empty-additive-reference.json');assert len(reference['validation'])==2
            assert all(r['maximum_degrees']<.12 and r['maximum_translation']<.00011 for r in reference['validation'])
            ref=reference['reference'];reference={'id':'empty-additive-reference.json'}
        else:
            assert all(__import__('re').fullmatch(r'sword_(walk|run)_[nesw]_add',a) for a in source['aliases']), 'Unproven additive reference'
            reference=next(r for r in manifest['rows'] if 'sword_neutral' in r['aliases'] and r['skeleton']==source['skeleton'] and r['status']=='native_pose_sampled_offline')
            ref=poses(P/reference['sample_file'])['frames'][0]
        for f in sp['frames']:
            for i,v in enumerate(f):
                a,b=ref[i],v[:];q=Quaternion((a[3],*a[:3]))@Quaternion((b[3],*b[:3]));v[:4]=[q.x,q.y,q.z,q.w]
                v[4:7]=[a[k]+b[k] for k in range(4,7)]
        additive=dict(reference=reference['id'],composition='local Q = neutral Q * delta Q; local T = neutral T + delta T',evidence='empty-additive-reference.json: complete independent walk/run comparison within 0.12 degree / 0.11 millimetre' if empty else 'additive-analysis.json: reproduces independently sampled walk/run forward within 0.056 degree / 3e-8 metre')
    source_indices={j['nativeName']:i for i,j in enumerate(sr.joints)}
    for role in (selection['sources'][source['id']]['targets'] if selection else ['corvo','emily']):
        target=NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints']);J=target.joints;B=target.bind;BG=target.global_bind
        scale=(BG[19].translation-BG[21].translation).length/(sr.global_bind[19].translation-sr.global_bind[21].translation).length
        mapping={i:source_indices[j['nativeName']] for i,j in enumerate(J) if j['nativeName'] in source_indices}
        frames=[];roots=[];max_length=0
        for frame in sp['frames']:
            # center_root is parented to origin (0), NOT auxiliary anchor (1).
            # Multiplying the anchor into this trajectory cancels run travel.
            sl=[qts(v) for v in frame];root=sl[0].copy();root.translation*=scale;roots.append(row(root))
            sl[0]=Matrix.Identity(4);sl[1]=Matrix.Identity(4);sg=sr.fk(sl)
            local=[m.copy() for m in B];g=[]
            for i,j in enumerate(J):
                p=j['parent']
                if i in mapping and i>=2 and 'helper' not in j['nativeName']:
                    k=mapping[i];rotation=sg[k].to_quaternion().to_matrix() @ sr.global_bind[k].to_quaternion().to_matrix().inverted() @ BG[i].to_3x3()
                    local[i]=(g[p].to_3x3().inverted() @ rotation).to_4x4();local[i].translation=B[i].translation
                    if i==2:local[i].translation+=(sl[k].translation-sr.bind[k].translation)*scale
                g.append(local[i] if p<0 else g[p]@local[i])
            local[0]=Matrix.Identity(4);local[1]=Matrix.Identity(4)
            helpers(target,local)
            max_length=max(max_length,max((local[i].translation-B[i].translation).length for i in range(3,27) if i!=17))
            frames.append([row(m) for m in local])
        layout='\n'.join(f"{j['nativeName']}:{j['parent']}" for j in J);layout_hash=int.from_bytes(hashlib.sha256(layout.encode()).digest()[:8],'little')
        key=source['id']+'-'+role;binary=OUT/(key+'.bin')
        payload=bytearray(struct.pack('<8sIIfQ',b'D2NATV01',len(J),len(frames),sp['fps'],layout_hash))
        for frame in frames:
            for v in frame:payload.extend(struct.pack('<7f',*v))
        binary.write_bytes(payload)
        meta=dict(id=key,role=role,source=source,frames=len(frames),fps=sp['fps'],duration=(len(frames)-1)/sp['fps'],joints=len(J),layout_hash=layout_hash,source_scale=scale,root_track=roots,native_length_error=max_length,sha256=hashlib.sha256(payload).hexdigest(),binary=str(binary.relative_to(P)),first_person_constraints=False,events=False,root_writes=False,visual_status='unreviewed_candidate',runtime_verified=False,helper_preview='Approximation; exact native correctives must be checked in D2',additive_reconstruction=additive)
        if selection:
            meta['selection']=selection['sources'][source['id']]
            alias=meta['selection']['alias']
            if alias.startswith(('empty_','activity_','gesture_','posture_')):meta['equipment_policy']='hands_free'
        meta['root_track_semantics']='origin joint 0 only; auxiliary anchor 1 is not an anatomical ancestor'
        (OUT/(key+'.json')).write_text(json.dumps(meta,indent=2));rows.append(meta)
        print('RETARGET',role,source['aliases'][0],len(frames),'frames',flush=True)
(P/args.manifest).write_text(json.dumps(rows,indent=2))
