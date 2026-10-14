"""Recordable bank motion at declared world speeds, not normalized-cycle time."""
import json,math,struct,hashlib,sys
from pathlib import Path
from mathutils import Quaternion,Matrix
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read
O=P/'gait-review';O.mkdir(exist_ok=True)
reports=[]
def blend(a,b,t):
 out=[]
 for x,y in zip(a,b):
  q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized();out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
 return out
def sample(fs,u):
 f=(u%1)*(len(fs)-1);a=int(f);return blend(fs[a],fs[min(a+1,len(fs)-1)],f-a)
def save(role,label,poses,roots,layout,source,notes):
 n=len(poses[0]);b=bytearray(struct.pack('<8sIIfQ',b'D2NATV01',n,len(poses),60,layout))
 for f in poses:
  for v in f:b.extend(struct.pack('<7f',*v))
 key=label+'-'+role;path=O/(key+'.bin');path.write_bytes(b)
 m=dict(id=key,role=role,binary=str(path.relative_to(P)),sha256=hashlib.sha256(b).hexdigest(),frames=len(poses),fps=60,duration=(len(poses)-1)/60,joints=n,layout_hash=layout,root_track=roots,source=source,selection=dict(alias=label,family='locomotion',purpose=notes),root_track_semantics='Declared demonstration speed integrated into preview root; actual D2 root remains native Steam replicated player position',first_person_constraints=False,events=False,root_writes=False,equipment_policy='sword_ready',visual_accepted=False,runtime_verified=False)
 (O/(key+'.json')).write_text(json.dumps(m,indent=2));reports.append(m)
for role in ['corvo','emily']:
 bank=read(P/f'native-gaits/{role}-manifest.json');clips={};layout=None
 for c in bank['clips']:
  b=(P/f'native-gaits/{role}-{c["name"]}.bin').read_bytes();magic,n,nf,fps,layout=struct.unpack_from('<8sIIfQ',b);assert magic==b'D2NATV01'
  fs=[[struct.unpack_from('<7f',b,28+(f*n+j)*28)for j in range(n)]for f in range(nf)];clips[c['name']]=dict(**c,frames=fs)
  speed=0 if c['name']=='armed_idle' else 1.2 if c['name'].startswith('walk') else 3.3
  seconds=2 if not speed else c['stride']/speed*2;nf=round(seconds*60)+1;poses=[];roots=[]
  for f in range(nf):
   t=f/60;phase=t/2 if not speed else t*speed/c['stride'];poses.append(sample(fs,phase));roots.append([0,0,0,1,math.cos(c['angle'])*speed*t,math.sin(c['angle'])*speed*t,0])
  save(role,c['name'],poses,roots,layout,dict(bank_sha256=bank['sha256'],stride=c['stride'],review_speed_mps=speed),f'Exact D2MOVE02 bank pose at {speed:g} m/s; two cycles. Authoring replay, not a new game recording.')
 poses=[];roots=[];phase=0;runmix=0;weight=0;x=0
 keys=[(0,0),(1,1.2),(3,1.2),(4,3.3),(6,3.3),(7,0),(8,0),(9,1.2),(10,1.2),(11,0)]
 for f in range(661):
  t=f/60;k=next((i for i in range(len(keys)-1)if t<=keys[i+1][0]),len(keys)-2);ta,sa=keys[k];tb,sb=keys[k+1];u=max(0,min(1,(t-ta)/(tb-ta)));u=u*u*(3-2*u);speed=sa+(sb-sa)*u
  mix=max(0,min(1,(speed-1.7)/1.3));runmix+=(mix-runmix)*(1-math.exp(-1/60/.12));weight+=(min(1,speed/.25)-weight)*(1-math.exp(-1/60/.15))
  stride=clips['walk_forward']['stride']*(1-runmix)+clips['run_forward']['stride']*runmix;phase+=speed/60/stride;x+=speed/60
  moving=blend(sample(clips['walk_forward']['frames'],phase),sample(clips['run_forward']['frames'],phase),runmix)
  poses.append(blend(sample(clips['armed_idle']['frames'],t/2),moving,weight));roots.append([0,0,0,1,x,0,0])
 save(role,'gait_sequence',poses,roots,layout,dict(bank_sha256=bank['sha256'],speed_keys=keys), 'Distance-driven bank demonstration: idle, walk, run, stop, walk and standing recovery. No first-person wrist constraints; dedicated authored acceleration/braking steps remain a refinement task.')
(P/'gait-review-manifest.json').write_text(json.dumps(reports,indent=2));print('GAIT_REVIEW_CLIPS',len(reports))
