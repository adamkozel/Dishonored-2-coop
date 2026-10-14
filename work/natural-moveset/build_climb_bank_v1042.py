"""Bank70/369: authored ladder-contact climb donors for native chain climbing (state23): 365 ascend, 366 descend, 367 bottom enter, 368 bottom exit. Prior365 unchanged.
359 tread loop, 360 stroke loop, 361 surface/up, 362 dive/down (363/364 reserved
copies of 359/360 keep the clip count aligned with the loader). Prior359 unchanged."""
from pathlib import Path
import numpy as np
import json,struct,hashlib
P=Path(__file__).resolve().parent;O=P/'climb-banks-v1042';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));reports=[]
donors=((365,'ladder-contact-v2','ladder_ascend_contact','native climbing ascend cycle (stand-in for chain)'),
        (366,'ladder-contact-v2','ladder_descend_contact','native climbing descend cycle'),
        (367,'ladder-contact-v2','ladder_bottom_enter_contact','climb entry (reserved)'),
        (368,'ladder-contact-v2','ladder_bottom_exit_contact','climb exit (reserved)'))
for role in ('corvo','emily'):
 old=(P/'swim-banks-v1037'/f'{role}-choke-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE69',69,365)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE70',70,n,369,fps,layout)+old[32:]);clips=[]
 for clip,folder,name,purpose in donors:
  p=P/folder/f'{name}-{role}.bin';m=rd(p.with_suffix('.json'));raw=p.read_bytes()
  assert raw[:8]==b'D2NATV01' and (m['joints'],m['layout_hash'])==(n,layout) and not m['root_writes'] and not m['events'] and not m['first_person_constraints']
  frames=m['frames'];a=np.frombuffer(raw,dtype='<f4',offset=28).reshape(frames,n,7).astype(float)
  at=np.linspace(0,len(a)-1,120);lo=at.astype(int);hi=np.minimum(lo+1,len(a)-1);t=(at-lo)[:,None,None]
  x=a[lo].copy();y=a[hi].copy();y[...,:4]*=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1);q=x+(y-x)*t;q[...,:4]/=np.linalg.norm(q[...,:4],axis=-1,keepdims=True);q=q.astype('<f4')
  assert np.isfinite(q).all() and np.max(np.abs(q[:,:2]-[0,0,0,1,0,0,0]))<1e-5,name
  bank.extend(struct.pack('<IIIIff',clip,121,0,0,0,float(m['duration'])));bank.extend(q.tobytes());bank.extend(q[0].tobytes())
  clips.append(dict(clip=clip,donor=str(p.relative_to(P)),donorSha256=hashlib.sha256(raw).hexdigest(),duration=m['duration'],purpose=purpose))
 assert bank[32:len(old)]==old[32:]
 out=O/f'{role}-choke-bank.bin';out.write_bytes(bank)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=70,clips=369,prior365PayloadsUnchanged=True,source_bank_sha256=hashlib.sha256(old).hexdigest().upper(),added=clips,
  conversion='Existing authored ladder-contact studies (ladder-contact-v2) resampled to 120 frames plus guard; climb cycle advanced by replicated vertical distance (assumed 1.5 m per cycle), ascend/descend by vertical direction; root network-owned. Earlier note: existing authored swim studies (mixamo-retargeted-v5 treading, water-movement-v1 forward/up/down) resampled to 120 frames plus guard; loops sampled by time, stroke rate scaled with replicated horizontal speed, up/down blended by replicated vertical velocity. Roots/events excluded; root and depth stay network-owned.',root_writes=False,events=False,runtime_verified=False,visual_accepted=False,complete=False))
(P/'climb-bank-manifest-v1042.json').write_text(json.dumps(reports,indent=2));print(json.dumps([(r['role'],r['sha256']) for r in reports]))
