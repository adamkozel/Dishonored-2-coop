"""Append nine pure-pose grenade tracks; preserve every v46 payload byte."""
import json,struct,hashlib,math
from pathlib import Path
import numpy as np
P=Path(__file__).resolve().parent;S=P.parents[1]/'outputs/d2-coop-lab/src'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
parts=['idle','equip','unequip','cooking_in','cooking_idle','throw_high','throw_high_ooa','throw_low','throw_low_ooa']
def sample(data,n=120):
 a=np.asarray(data,dtype=np.float64);at=np.linspace(0,len(a)-1,n);lo=at.astype(int);hi=np.minimum(lo+1,len(a)-1);t=(at-lo).reshape((-1,)+(1,)*(a.ndim-1))
 x=a[lo].copy();y=a[hi].copy();sgn=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1);y[...,:4]*=sgn
 z=x+(y-x)*t;z[...,:4]/=np.linalg.norm(z[...,:4],axis=-1,keepdims=True);assert np.isfinite(z).all();return z.astype('<f4')
rows=read(P/'grenade-authoring-manifest-v55.json');reports=[];tracks=[];out=P/'grenade-banks-v56';out.mkdir(exist_ok=True)
for role in ('corvo','emily'):
 base=(P/'outofmana-context-banks-v46'/f'{role}-outofmana-context-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',base);assert (magic,v,count)==(b'D2MOVE41',41,251)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE42',42,n,260,fps,layout)+base[32:]);roletracks=[];sources=[]
 for index,part in enumerate(parts):
  m=next(r for r in rows if r['role']==role and r['selection']['alias']=='grenade_'+part);raw=(P/m['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==m['sha256'];assert m['joints']==n and m['layout_hash']==layout
  poses=np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7);p=sample(poses);assert np.max(np.abs(p[:,3:17,4:]-poses[0,3:17,4:]))<1e-5
  bank.extend(struct.pack('<IIIIff',251+index,121,0,0,0,m['duration']));bank.extend(p.tobytes());bank.extend(p[0].tobytes())
  roletracks.append(sample(m['props'][0]['attachment_track']).tolist());sources.append(dict(variant=index,alias=m['selection']['alias'],source=m['binary'],sha256=m['sha256']))
 assert bank[32:len(base)]==base[32:]
 path=out/f'{role}-grenade-bank.bin';path.write_bytes(bank);reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE42',clips=260,prior_payload_unchanged=True,sources=sources,visual_accepted=False,runtime_verified=False))
 tracks.append(roletracks)
def cpp(v):return '{'+','.join(cpp(x)if isinstance(x,list)else f'{x:.9f}f'for x in v)+'}'
(S/'grenade_hand_tracks.h').write_text('#pragma once\n// Generated native wrist-relative prop tracks. No actor root or events.\ninline constexpr float grenadeHandTracks[2][9][120][7]='+cpp(tracks)+';\n')
(P/'grenade-bank-manifest-v56.json').write_text(json.dumps(reports,indent=2));print('Appended nine grenade tracks for each role; previous 251 clips unchanged')
