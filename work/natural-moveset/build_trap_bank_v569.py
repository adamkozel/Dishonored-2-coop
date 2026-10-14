from pathlib import Path
import json,struct,hashlib
import numpy as np
P=Path(__file__).resolve().parent;L=P.parents[1]/'outputs/d2-coop-lab';S=L/'src';O=P/'trap-banks-v569';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def sample(a):
 a=np.asarray(a,dtype=np.float64);at=np.linspace(0,len(a)-1,120);lo=at.astype(int);hi=np.minimum(lo+1,len(a)-1);t=(at-lo).reshape((-1,)+(1,)*(a.ndim-1))
 x=a[lo].copy();y=a[hi].copy();y[...,:4]*=np.where(np.sum(x[...,:4]*y[...,:4],axis=-1,keepdims=True)<0,-1,1)
 z=x+(y-x)*t;z[...,:4]/=np.linalg.norm(z[...,:4],axis=-1,keepdims=True);assert np.isfinite(z).all();return z.astype('<f4')
tracks=[];reports=[]
for role in ('corvo','emily'):
 old=(P/'heart-banks-v564'/f'{role}-heart-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE60',60,340)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE61',61,n,347,fps,layout)+old[32:]);rt=[];sources=[]
 for j,part in enumerate(('idle','equip','unequip','ready_in','ready_loop','place','ooa')):
  p=P/'items-authored-v2'/f'stunmine_{part}-{role}.bin';m=rd(p.with_suffix('.json'));raw=p.read_bytes();assert hashlib.sha256(raw).hexdigest()==m['sha256'];assert(m['joints'],m['layout_hash'])==(n,layout)
  q=sample(np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7));bank.extend(struct.pack('<IIIIff',340+j,121,0,0,0,m['duration']));bank.extend(q.tobytes());bank.extend(q[0].tobytes())
  rt.append(sample(m['props'][0]['attachment_track']).tolist());sources.append(dict(variant=j,source=str(p.relative_to(P)),sha256=m['sha256']))
 assert bank[32:len(old)]==old[32:];p=O/f'{role}-trap-bank.bin';p.write_bytes(bank);tracks.append(rt)
 reports.append(dict(role=role,bank=str(p.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=61,clips=347,sources=sources,prior340PayloadsUnchanged=True,runtime_verified=False,visual_accepted=False))
def cpp(a):return '{'+','.join(cpp(x)if isinstance(x,list)else f'{x:.9f}f' for x in a)+'}'
(S/'trap_hand_tracks.h').write_text('#pragma once\n// Existing authored wrist-relative trap attachments; no gameplay events.\ninline constexpr float trapHandTracks[2][7][120][7]='+cpp(tracks)+';\n')
(P/'trap-bank-manifest-v569.json').write_text(json.dumps(reports,indent=2))
for name,model in (('stunmine','stun_mine'),('springrazor','spring_razor')):
 (L/f'assets/{name}_prop.def').write_text('{\n class = "idEntity";\n edit = {\n  renderModelInfo = {\n   model = "models/weapons/grenade/'+model+'.lwo";\n   m_castShadows = true;\n  }\n }\n}\n')
installer=(L/'Build-D2HeartPropAssets.ps1').read_text(encoding='utf-8-sig').replace('heart','trap').replace('Heart','Trap')
installer=installer.replace("@{Name='models/weapons/d2coop/trap_prop.def';File='assets/trap_prop.def';Kind='entitydef';Class='entityDef'}", "@{Name='models/weapons/d2coop/stunmine_prop.def';File='assets/stunmine_prop.def';Kind='entitydef';Class='entityDef'},\n @{Name='models/weapons/d2coop/springrazor_prop.def';File='assets/springrazor_prop.def';Kind='entitydef';Class='entityDef'}")
(L/'Build-D2TrapPropAssets.ps1').write_text(installer)
print('Bank61/347 and static native gadget cosmetics prepared; no original payload changed.')
