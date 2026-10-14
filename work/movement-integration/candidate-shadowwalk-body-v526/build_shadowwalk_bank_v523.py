"""Bounded first runtime Shadow Walk body candidate. No gameplay/root/events."""
from pathlib import Path
import json,hashlib,struct
import numpy as np
P=Path(__file__).resolve().parent;OUT=P/'shadowwalk-banks-v523';OUT.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
reports=[]
def resample(data,u):
    f=u*(len(data)-1);a=np.floor(f).astype(int);b=np.minimum(a+1,len(data)-1);t=(f-a)[:,None,None]
    x=data[a];y=data[b].copy();dot=np.sum(x[:,:,:4]*y[:,:,:4],axis=2,keepdims=True)
    y[:,:,:4]=np.where(dot<0,-y[:,:,:4],y[:,:,:4]);r=x+(y-x)*t
    r[:,:,:4]/=np.linalg.norm(r[:,:,:4],axis=2,keepdims=True)
    return r.astype('<f4')
for role in ('corvo','emily'):
    old=(P/'doppelganger-holstered-banks-v495'/f'{role}-doppelganger-holstered-bank.bin').read_bytes()
    magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old)
    assert (magic,version,count,fps)==(b'D2MOVE57',57,331,60)
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE58',58,n,335,fps,layout)+old[32:]);clips=[]
    for offset,(alias,duration,end) in enumerate([('enter',1.,1.),('idle',2.8,1.),('move_forward',None,1.),('exit',1.133,.6)]):
        p=P/'shadow-body-v1'/f'shadow_walk_{alias}-{role}.bin';meta=rd(p.with_suffix('.json'));raw=p.read_bytes()
        assert hashlib.sha256(raw).hexdigest()==meta['sha256'] and meta['joints']==n and meta['layout_hash']==layout
        poses=np.frombuffer(raw,dtype='<f4',offset=28).reshape(meta['frames'],n,7)
        sampled=resample(poses,np.linspace(0,end,120))
        # The exit study reverses entry; its first60% ends at crouch, matching
        # native natural/requested exit. The replicated actor owns root travel.
        sampled=np.concatenate((sampled,sampled[:1]))
        duration=duration or meta['duration'];stride=meta['contact_speed_estimate_mps']*meta['duration'] if alias=='move_forward' else 0.
        assert np.all(np.isfinite(sampled)) and np.max(np.abs(sampled))<=3
        bank.extend(struct.pack('<IIIIff',331+offset,121,0,0,stride,duration));bank.extend(sampled.tobytes())
        clips.append(dict(index=331+offset,alias=alias,source=str(p.relative_to(P)),sha256=meta['sha256'],duration=duration,stride=stride,sourceEnd=end))
    path=OUT/f'{role}-shadowwalk-bank.bin';path.write_bytes(bank)
    assert bank[32:len(old)]==old[32:]
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),format='D2MOVE58',clips=335,prior_payload_unchanged=True,sources=clips,scope='Initial hands-free human-rig low-crawl body candidate for Emily native Shadow Walk. Entry/exit use observed clocks, exit ends crouched. Movement cadence will follow replicated distance. Native shadow material/VFX, attack, directional moves and final visual quality remain unfinished. Corvo is offline comparison only.',runtime_verified=False,visual_accepted=False))
(P/'shadowwalk-bank-manifest-v523.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print(json.dumps(reports,indent=2))
