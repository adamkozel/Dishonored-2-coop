from pathlib import Path
import json,struct,hashlib
import numpy as np
P=Path(__file__).resolve().parent;L=P.parents[1]/'outputs/d2-coop-lab';S=L/'src';O=P/'heart-banks-v564';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def sample(data):
 a=np.asarray(data,dtype=np.float64);at=np.linspace(0,len(a)-1,120);lo=at.astype(int);hi=np.minimum(lo+1,len(a)-1);t=(at-lo).reshape((-1,)+(1,)*(a.ndim-1))
 x=a[lo].copy();y=a[hi].copy();y[...,:4]*=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1)
 z=x+(y-x)*t;z[...,:4]/=np.linalg.norm(z[...,:4],axis=-1,keepdims=True);assert np.isfinite(z).all();return z.astype('<f4')
tracks=[];reports=[]
for role in ('corvo','emily'):
 old=(P/'shadowwalk-sneak-banks-v548'/f'{role}-shadowwalk-sneak-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE59',59,336)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE60',60,n,340,fps,layout)+old[32:]);rt=[];sources=[]
 for j,part in enumerate(('idle','equip','unequip','use')):
  p=P/'items-authored-v2'/f'heart_{part}-{role}.bin';m=rd(p.with_suffix('.json'));raw=p.read_bytes();assert hashlib.sha256(raw).hexdigest()==m['sha256'];assert(m['joints'],m['layout_hash'])==(n,layout)
  q=sample(np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7));bank.extend(struct.pack('<IIIIff',336+j,121,0,0,0,m['duration']));bank.extend(q.tobytes());bank.extend(q[0].tobytes())
  rt.append(sample(m['props'][0]['attachment_track']).tolist());sources.append(dict(variant=j,source=str(p.relative_to(P)),sha256=m['sha256']))
 assert bank[32:len(old)]==old[32:];p=O/f'{role}-heart-bank.bin';p.write_bytes(bank);tracks.append(rt)
 reports.append(dict(role=role,bank=str(p.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=60,clips=340,sources=sources,prior336PayloadsUnchanged=True,runtime_verified=False,visual_accepted=False))
def cpp(a):return '{'+','.join(cpp(x)if isinstance(x,list)else f'{x:.9f}f' for x in a)+'}'
(S/'heart_hand_tracks.h').write_text('#pragma once\n// Existing authored native wrist-relative Heart attachments; no gameplay events.\ninline constexpr float heartHandTracks[2][4][120][7]='+cpp(tracks)+';\n')
(P/'heart-bank-manifest-v564.json').write_text(json.dumps(reports,indent=2))
(L/'assets/heart_prop.def').write_text((L/'assets/grenade_prop.def').read_text().replace('models/weapons/grenade/grenade.md6','models/weapons/heart/heart.md6'))
installer=(L/'Build-D2GrenadePropAssets.ps1').read_text(encoding='utf-8-sig').replace('grenade','heart').replace('Grenade','Heart')
(L/'Build-D2HeartPropAssets.ps1').write_text(installer)
print('Heart bank60/340 prepared, with existing attachment tracks and isolated render-only prop asset.')
