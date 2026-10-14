"""Append four pure body reactions; native source scene owns the 800 ms clock."""
import json,struct,hashlib
from pathlib import Path
import numpy as np
P=Path(__file__).resolve().parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
rows=read(P/'library-retarget-manifest.json');out=P/'stun-banks-v61';out.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'grenade-banks-v56'/f'{role}-grenade-bank.bin').read_bytes()
 magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert (magic,v,count)==(b'D2MOVE42',42,260)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE43',43,n,264,fps,layout)+data[32:]);sources=[]
 for variant,direction in enumerate(('right','front','left','back'),1):
  m=next(r for r in rows if r['role']==role and r['selection']['alias']=='sword_stinghit_'+direction)
  raw=(P/m['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==m['sha256']
  assert m['joints']==n and m['layout_hash']==layout and not m['first_person_constraints'] and not m['root_writes'] and not m['events']
  poses=np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7).astype(np.float64)
  identity=np.array([0,0,0,1,0,0,0]);assert np.max(np.abs(poses[:,:2]-identity))<1e-6
  at=np.linspace(0,len(poses)-1,120);lo=at.astype(int);hi=np.minimum(lo+1,len(poses)-1);t=(at-lo)[:,None,None]
  x=poses[lo].copy();y=poses[hi].copy();y[...,:4]*=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1)
  p=x+(y-x)*t;p[...,:4]/=np.linalg.norm(p[...,:4],axis=-1,keepdims=True)
  assert np.isfinite(p).all()
  bank.extend(struct.pack('<IIIIff',259+variant,121,0,0,0,.8));bank.extend(p.astype('<f4').tobytes());bank.extend(p[0].astype('<f4').tobytes())
  sources.append(dict(variant=variant,alias=m['selection']['alias'],source=m['binary'],sha256=m['sha256'],native_asset=m['source']['asset'],source_duration=m['duration'],presentation_duration=.8,visual_accepted=False))
 assert bank[32:len(data)]==data[32:]
 path=out/f'{role}-stun-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE43',clips=264,previous_260_clips_unchanged=True,sources=sources,events=False,root_writes=False,runtime_verified=False,visual_accepted=False))
(P/'stun-bank-manifest-v61.json').write_text(json.dumps(reports,indent=2))
print('D2MOVE43: four source-clock body reaction candidates per role; prior 260 clips unchanged')
