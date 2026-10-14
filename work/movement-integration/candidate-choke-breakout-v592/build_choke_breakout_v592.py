"""One bounded actor-only breakout candidate; existing authoring format and donors."""
from pathlib import Path
import numpy as np
import struct,json,hashlib
P=Path(__file__).resolve().parent;O=P/'choke-breakout-banks-v592';O.mkdir(exist_ok=False)
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda b:hashlib.sha256(b).hexdigest()
rows=rd(P/'library-retarget-manifest.json');reports=[]
def blend(a,b,t):
 a=np.array(a,dtype=float);b=np.array(b,dtype=float)
 b[...,:4]*=np.where(np.sum(a[...,:4]*b[...,:4],axis=-1,keepdims=True)<0,-1,1)
 out=a+(b-a)*t;out[...,:4]/=np.linalg.norm(out[...,:4],axis=-1,keepdims=True);return out
def sample(a,t):
 at=np.clip(t,0,len(a)-1);lo=int(at);return blend(a[lo],a[min(lo+1,len(a)-1)],at-lo)
def ease(t):
 t=np.clip(t,0,1);return t*t*(3-2*t)
for role in ('corvo','emily'):
 old=(P/'choke-banks-v581'/f'{role}-choke-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE63',63,351)
 release=P/'choke-sequences-v1'/f'nonlethal_back_release_sequence-{role}.bin';m=rd(release.with_suffix('.json'));raw=release.read_bytes();assert sha(raw)==m['sha256'];assert(m['joints'],m['layout_hash'])==(n,layout)
 frames=np.frombuffer(raw,dtype='<f4',offset=28).reshape(m['frames'],n,7);mark=next(x for x in m['phase_marks'] if x['phase']=='choke_cancel');start=round(mark['at']*m['fps']);releaseFrames=frames[start:]
 donor=next(r for r in rows if r['role']==role and r['selection']['alias']=='sword_stinghit_front');raw=(P/donor['binary']).read_bytes();assert sha(raw)==donor['sha256'] and donor['joints']==n and donor['layout_hash']==layout
 assert not donor['first_person_constraints'] and not donor['root_writes'] and not donor['events'];reaction=np.frombuffer(raw,dtype='<f4',offset=28).reshape(donor['frames'],n,7)
 values=[]
 for seconds in np.linspace(0,1.5,120):
  if seconds<.28:p=sample(releaseFrames,seconds/.28*(len(releaseFrames)-1))
  else:
   p=sample(reaction,(seconds-.28)/1.1*(len(reaction)-1));p=blend(releaseFrames[-1],p,ease((seconds-.28)/.10))
   p=blend(p,releaseFrames[-1],ease((seconds-1.35)/.15))
  values.append(p)
 values=np.array(values,dtype='<f4');assert np.isfinite(values).all();assert np.max(np.abs(values[:,:2]-[0,0,0,1,0,0,0]))<1e-6
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE64',64,n,352,fps,layout)+old[32:]);bank.extend(struct.pack('<IIIIff',351,121,0,0,0,1.5));bank.extend(values.tobytes());bank.extend(values[0].tobytes());assert bank[32:len(old)]==old[32:]
 out=O/f'{role}-choke-bank.bin';out.write_bytes(bank)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=sha(bank).upper(),version=64,clips=352,prior351PayloadsUnchanged=True,variant=4,nativeScene='choke_combat_lose.midnightscene/Npc/1500',sources=[dict(binary=str(release.relative_to(P)),sha256=m['sha256'],phase='authored release from native-timed choke study'),dict(binary=donor['binary'],sha256=donor['sha256'],asset=donor['source']['asset'])],conversion='release0-.28s; native front-recoil .28-1.38s,100ms entry blend; recovery to authored hands-free idle1.35-1.5s;120 samples+guard',root_writes=False,events=False,runtime_verified=False,visual_accepted=False,complete=False))
(P/'choke-breakout-bank-manifest-v592.json').write_text(json.dumps(reports,indent=2));print('Bank64/352: bounded hands-free release/recoil candidate, prior351 unchanged.')
