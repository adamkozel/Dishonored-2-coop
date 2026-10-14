"""Bounded whole-body vertical peeks; native camera is reference, never a target."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
OUT=P/'vertical-peek-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
L=Library([],'vertical-peek-sources-v1')
for role in ('corvo','emily'):
 data=(P/'unarmed-special-banks-v1'/f'{role}-unarmed-special-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE10' and count==107
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE11',11,n,111,fps,layout)+data[32:]);entry=24+121*n*28;rig=rig_for(role);sources=[]
 def initial(cid):return [qts(struct.unpack_from('<7f',data,32+cid*entry+24+j*28))for j in range(n)]
 for k,(standing,crouched)in enumerate(((0,17),(59,76))):
  stand=initial(standing);low=initial(crouched)
  for stance,base in enumerate((stand,low)):
   initial_global=rig.fk(base);poses=[];toe_error=0;foot_error=0;head=[]
   for f in range(120):
    w=f/119
    if stance:
     # Unfold from the common whole-body crouch, retaining its hand-local
     # equipment articulation. No fixed shoulder, wrist, eye or camera anchor.
     p=blend(base,stand,.30*w);p[2].translation.x+=.025*w
     for j in (13,14,15,16,17):p[j]=base[j].copy()
     targets=[initial_global[j].translation.copy()for j in (21,25)]
     pin_feet(rig,p,targets,[initial_global[j].to_3x3()for j in (21,25)])
     g=rig.fk(p);foot_error=max(foot_error,max((g[j].translation-targets[i]).length for i,j in enumerate((21,25))))
    else:
     p=copy_pose(base);p[2].translation.x+=.010*w
     for j,degrees in ((3,-1.5),(4,-1.5),(5,-1.5),(11,-1.5)):
      rig.orient(p,j,Matrix.Rotation(math.radians(degrees*w),3,'Y')@rig.fk(p)[j].to_3x3())
     # Pivot each complete foot about its own ball/toe joint. The heels rise
     # and knees extend; anatomical bones never stretch to reach FP eye height.
     targets=[];rotations=[]
     for ankle,toe in ((21,22),(25,26)):
      before=initial_global[ankle];pivot=initial_global[toe].translation;axis=Vector((0,1,0))
      rotation=Quaternion(axis,math.radians(18*w)).to_matrix()
      # Select the physically upward heel-pivot sign, independently of rig axes.
      opposite=Quaternion(axis,math.radians(-18*w)).to_matrix();v=before.translation-pivot
      if (opposite@v).z>(rotation@v).z:rotation=opposite
      targets.append(pivot+rotation@v);rotations.append(rotation@before.to_3x3())
     p[2].translation.z+=min(targets[i].z-initial_global[j].translation.z for i,j in enumerate((21,25)))
     pin_feet(rig,p,targets,rotations);g=rig.fk(p)
     toe_error=max(toe_error,max((g[j].translation-initial_global[j].translation).length for j in (22,26)))
    preview_helpers(rig,p);poses.append(p);g=rig.fk(p);head.append(list(g[12].translation-initial_global[12].translation))
   assert foot_error<.003 and toe_error<.003,(role,stance,foot_error,toe_error)
   assert 0<head[-1][2]<.4,(role,stance,head[-1])
   cid=107+k*2+stance;poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,0.))
   for p in poses:
    assert max((p[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
    for mat in p:bank.extend(struct.pack('<7f',*row(mat)))
   alias=('unarmed_'if k else'armed_')+('crouched_'if stance else'standing_')+'vertical_peek'
   L.save(rig,role,alias,poses[:-1],[identity()for _ in poses[:-1]],'stance','Whole-body native vertical peek reference: crouch unfolds over planted feet; standing rises onto the balls of the feet. Native camera height is not a body or wrist constraint.',[hashlib.sha256(data).hexdigest()],parameter='native Leaning+44',head_displacement=head[-1],toe_error=toe_error,foot_error=foot_error)
   sources.append(dict(id=cid,alias=alias,head_displacement=head[-1],toe_error=toe_error,foot_error=foot_error))
 path=OUT/f'{role}-vertical-peek-bank.bin';path.write_bytes(bank);reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE11',clips=111,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,events=False,root_writes=False,gameplay_verified=False,visual_accepted=False))
L.finish('vertical-peek-source-manifest-v1.json');(P/'vertical-peek-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
