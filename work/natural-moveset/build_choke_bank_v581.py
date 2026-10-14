from pathlib import Path
import json,struct,hashlib
import numpy as np
P=Path(__file__).resolve().parent;O=P/'choke-banks-v581';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
reports=[]
for role in ('corvo','emily'):
 old=(P/'trap-banks-v575'/f'{role}-trap-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE62',62,348)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE63',63,n,351,fps,layout)+old[32:])
 p=P/'choke-sequences-v1'/f'nonlethal_back_finish_sequence-{role}.bin';m=rd(p.with_suffix('.json'));raw=p.read_bytes();assert hashlib.sha256(raw).hexdigest()==m['sha256'];assert(m['joints'],m['layout_hash'])==(n,layout)
 values=np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7);parts=[]
 for j,(phase,duration) in enumerate((('chokeback_in',.6),('choke_loop',1.4),('choke_win',1.8))):
  mark=next(x for x in m['phase_marks'] if x['phase']==phase);start=round(mark['at']*m['fps']);end=start+round(duration*m['fps']);assert end<len(values)
  at=np.linspace(start,end,120);lo=at.astype(int);hi=np.minimum(lo+1,end);t=(at-lo)[:,None,None];a=values[lo].astype(float);b=values[hi].astype(float);b[...,:4]*=np.where(np.sum(a[...,:4]*b[...,:4],axis=-1,keepdims=True)<0,-1,1)
  q=a+(b-a)*t;q[...,:4]/=np.linalg.norm(q[...,:4],axis=-1,keepdims=True);assert np.isfinite(q).all();q=q.astype('<f4')
  bank.extend(struct.pack('<IIIIff',348+j,121,0,0,0,duration));bank.extend(q.tobytes());bank.extend(q[0].tobytes())
  parts.append(dict(variant=j+1,phase=phase,startFrame=start,endFrame=end,duration=duration))
 assert bank[32:len(old)]==old[32:];out=O/f'{role}-choke-bank.bin';out.write_bytes(bank)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=63,clips=351,source=str(p.relative_to(P)),sourceSha256=m['sha256'],segments=parts,prior348PayloadsUnchanged=True,runtime_verified=False,visual_accepted=False))
(P/'choke-bank-manifest-v581.json').write_text(json.dumps(reports,indent=2))
print('Bank63/351, existing authored rear entry/loop/finish, prior348 payloads unchanged.')
