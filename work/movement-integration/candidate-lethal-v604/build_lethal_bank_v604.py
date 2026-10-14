from pathlib import Path
import numpy as np
import json,struct,hashlib
P=Path(__file__).resolve().parent;O=P/'lethal-banks-v604';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));reports=[]
for role in ('corvo','emily'):
 old=(P/'choke-breakout-banks-v592'/f'{role}-choke-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE64',64,352)
 p=P/'paired-native-v1'/f'paired_fast_back_tall-{role}.bin';m=rd(p.with_suffix('.json'));raw=p.read_bytes();assert hashlib.sha256(raw).hexdigest()==m['sha256'];assert(m['joints'],m['layout_hash'])==(n,layout);assert not m['root_writes'] and not m['events'] and not m['first_person_constraints']
 a=np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7).astype(float);at=np.linspace(0,len(a)-1,120);lo=at.astype(int);hi=np.minimum(lo+1,len(a)-1);t=(at-lo)[:,None,None];x=a[lo].copy();y=a[hi].copy();y[...,:4]*=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1);q=x+(y-x)*t;q[...,:4]/=np.linalg.norm(q[...,:4],axis=-1,keepdims=True);q=q.astype('<f4')
 assert np.isfinite(q).all() and np.max(np.abs(q[:,:2]-[0,0,0,1,0,0,0]))<1e-6
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE65',65,n,353,fps,layout)+old[32:]);bank.extend(struct.pack('<IIIIff',352,121,0,0,0,1.333));bank.extend(q.tobytes());bank.extend(q[0].tobytes());assert bank[32:len(old)]==old[32:]
 out=O/f'{role}-choke-bank.bin';out.write_bytes(bank);reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=65,clips=353,prior352PayloadsUnchanged=True,variant=5,nativeScene='assassination_fast_back.midnightscene/Npc/1333',source=str(p.relative_to(P)),sourceSha256=m['sha256'],sourceDuration=m['duration'],conversion='Existing native Doppelganger paired actor donor;120 samples plus guard. Network root retained; source root track and all events excluded. Native player clock1333ms.',root_writes=False,events=False,runtime_verified=False,visual_accepted=False,complete=False))
(P/'lethal-bank-manifest-v604.json').write_text(json.dumps(reports,indent=2));print('Bank65/353: existing paired fast-back actor, previous352 unchanged.')
