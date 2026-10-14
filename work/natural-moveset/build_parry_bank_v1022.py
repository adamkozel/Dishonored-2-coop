"""Bank68/359: native NPC deflection donors for the remote player's native
block impact (variants19/20, 533 ms) and defender timed parry (21, 500 ms).
Existing retargeted native clips; roots/events excluded; prior357 unchanged."""
from pathlib import Path
import numpy as np
import json,struct,hashlib
P=Path(__file__).resolve().parent;O=P/'parry-banks-v1022';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));reports=[]
donors=((357,'9089de2b61127bab','sword_parry_ll_out',.533,'native block impact sword_parry_hit_1/hit_2 Scene_1/533'),
        (358,'31f39731b12af7ee','sword_parry_rl_out',.5,'native defender timed parry sword_parry_win Scene_1/500'))
for role in ('corvo','emily'):
 old=(P/'heavy-banks-v633'/f'{role}-choke-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE67',67,357)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE68',68,n,359,fps,layout)+old[32:]);clips=[]
 for clip,donor,name,duration,scene in donors:
  p=P/'native-retargeted'/f'{donor}-{role}.bin';m=rd(p.with_suffix('.json'));raw=p.read_bytes()
  assert hashlib.sha256(raw).hexdigest()==m['sha256'] and m['source']['asset'].endswith(name+'.md6anim')
  assert(m['joints'],m['layout_hash'])==(n,layout) and not m['root_writes'] and not m['events'] and not m['first_person_constraints']
  a=np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7).astype(float);at=np.linspace(0,len(a)-1,120);lo=at.astype(int);hi=np.minimum(lo+1,len(a)-1);t=(at-lo)[:,None,None]
  x=a[lo].copy();y=a[hi].copy();y[...,:4]*=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1);q=x+(y-x)*t;q[...,:4]/=np.linalg.norm(q[...,:4],axis=-1,keepdims=True);q=q.astype('<f4')
  assert np.isfinite(q).all() and np.max(np.abs(q[:,:2]-[0,0,0,1,0,0,0]))<1e-6
  bank.extend(struct.pack('<IIIIff',clip,121,0,0,0,duration));bank.extend(q.tobytes());bank.extend(q[0].tobytes())
  clips.append(dict(clip=clip,donor=str(p.relative_to(P)),donorSha256=m['sha256'],donorAsset=m['source']['asset'],donorDuration=m['duration'],nativeScene=scene,duration=duration))
 assert bank[32:len(old)]==old[32:]
 out=O/f'{role}-choke-bank.bin';out.write_bytes(bank)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=68,clips=359,prior357PayloadsUnchanged=True,source_bank_sha256=hashlib.sha256(old).hexdigest().upper(),added=clips,
  conversion='Existing retargeted native NPC parry deflection (_out) donors resampled to 120 frames plus guard and played on the native defender scene clock. Functional-first donor choice; direction-specific acting, transition from the authored block idle and contact are unreviewed.',root_writes=False,events=False,runtime_verified=False,visual_accepted=False,complete=False))
(P/'parry-bank-manifest-v1022.json').write_text(json.dumps(reports,indent=2));print(json.dumps([(r['role'],r['sha256']) for r in reports]))
