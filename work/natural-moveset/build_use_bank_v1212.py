"""Bank71/374: authored interaction sequences for native use (player state 75):
369 button, 370 lever, 371 crank, 372 wheel, 373 key. Prior369 unchanged.
Earlier header (bank70): authored ladder-contact climb donors 365-368."""
from pathlib import Path
import numpy as np
import json,struct,hashlib
P=Path(__file__).resolve().parent;O=P/'use-banks-v1212';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));reports=[]
donors=((369,'mechanisms-v4','interact_button_sequence','native use: button/press or generic reach (state 75)'),
        (370,'mechanisms-v4','interact_lever_sequence','native use: lever'),
        (371,'mechanisms-v4','interact_crank_sequence','native use: crank'),
        (372,'mechanisms-v4','interact_wheel_sequence','native use: wheel/valve'),
        (373,'mechanisms-v4','interact_key_sequence','native use: key/lock'))
for role in ('corvo','emily'):
 old=(P/'climb-banks-v1042'/f'{role}-choke-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE70',70,369)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE71',71,n,374,fps,layout)+old[32:]);clips=[]
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
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=71,clips=374,prior369PayloadsUnchanged=True,source_bank_sha256=hashlib.sha256(old).hexdigest().upper(),added=clips,
  conversion='Existing authored mechanism studies (mechanisms-v4: reach, operate, recover with full-body weight transfer) resampled to 120 frames plus guard; played once per native use event on its own clip duration; root network-owned, no events.',root_writes=False,events=False,runtime_verified=False,visual_accepted=False,complete=False))
(P/'use-bank-manifest-v1212.json').write_text(json.dumps(reports,indent=2));print(json.dumps([(r['role'],r['sha256']) for r in reports]))
