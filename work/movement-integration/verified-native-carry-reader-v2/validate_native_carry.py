"""Check captured native source semantics. Does not claim remote gameplay coverage."""
import argparse, collections, hashlib, json, subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(path): return json.loads(path.read_text(encoding='utf-8-sig'))
def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest().upper()
def write(path, value):
    tmp=path.with_suffix(path.suffix+'.tmp');tmp.write_text(json.dumps(value,indent=2),encoding='utf-8');tmp.replace(path)
parser=argparse.ArgumentParser();parser.add_argument('take');parser.add_argument('--character',choices=['corvo','emily'],required=True);parser.add_argument('--release',type=int,choices=[4,5,6],required=True)
args=parser.parse_args();folder=P/args.take
rows=read(folder/'observations.json');identity=read(folder/'identity.json');before=read(folder/'before.json');after=read(folder/'after.json')
expected=1 if args.character=='corvo' else 2
checks=[]
def check(test,label):
    if not test: raise RuntimeError(label)
    checks.append(label)
check(identity['executableSha256']=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180','Exact verified Steam executable identity')
check(identity['proxySha256']=='BD99C7A6E327292B2493DC354538EFD2F35C06C4800297BFE5476BC848D1A706','Existing BD99 proxy preserved; only the read-only probe changed')
valid=[r for r in rows if (r.get('native') or {}).get('carrySceneValid')]
counts=collections.Counter(r['native']['carryScene']['variant'] for r in valid)
check(all(counts[v]>=3 for v in [1,2,3,args.release]),'Owned native pickup, idle, walk and selected release each sampled at least three times')
check(all(r['native']['carryScene']['character']==expected for r in valid),'All accepted scenes resolve to the intended native character')
check(all(sum(r['native']['carryScene']['stance']==s and r['native']['carryScene']['variant']==3 for r in valid)>=3 for s in [0,1]),'Carry walking independently observed while standing and crouched')
check(len({r['native']['carryScene']['corpseHandle'] for r in valid})==1,'Pickup, carry and drop retain the same source corpse generation')
independent=0;retirement_boundaries=[];indices={id(row):i for i,row in enumerate(rows)}
for row in valid:
    s=row['native']['carryScene']
    for native in row['carryScenes']:
        if native['id']!=s['handle']: continue
        check_data=(native['asset']==s['asset'] and native['subscene']==s['subscene'] and native['sceneDurationMs']==s['durationMs'])
        if not check_data: raise RuntimeError('Independent native scene disagrees with owned reader')
        if native['state']!=3:
            # The two probes run serially, not atomically. Corvo sample76
            # observes valid idle, then its retired instance, then owned walk.
            # Retired evidence is explicitly excluded from matched coverage.
            i=indices[id(row)];following=rows[i+1] if i+1<len(rows) else {}
            next_scene=(following.get('native') or {}).get('carryScene',{})
            boundary=(native['state']==10 and not native['bindingsResearch'] and following.get('ms',0)-row['ms']<=250
                      and next_scene.get('handle') and next_scene['handle']!=s['handle']
                      and next_scene['corpseHandle']==s['corpseHandle'] and next_scene['character']==s['character'])
            if not boundary: raise RuntimeError('Unexplained independent scene retirement')
            retirement_boundaries.append(dict(index=i,ms=row['ms'],retired_handle=s['handle'],next_handle=next_scene['handle']))
            continue
        bindings=native['bindingsResearch']
        if not all(b['reciprocal'] for b in bindings): raise RuntimeError('Independent binding lost reciprocal ownership')
        handles=[b['handle'] for b in bindings]
        if handles.count(1)!=1 or handles.count(s['corpseHandle'])!=1: raise RuntimeError('Independent corpse/player binding mismatch')
        independent+=1
check(independent>=len(valid)*.95,'At least 95% of accepted samples agree with a subsequent independent scene and binding read')
outside=[r for r in rows if r.get('native') and not any(a['id'] in [45,46] for a in r['native']['active'])]
check(len(outside)>=5 and all(not r['native']['carrySceneValid'] for r in outside),'Carry reader refuses samples outside native carry/drop states')
check(not any(a['id'] in [45,46] for a in before['active']+after['active']) and not after['carrySceneValid'],'Native pickup begins from free movement and release returns to free movement')
health=read(folder/'health-after.json')
check(all(health[k]==identity['healthBefore'][k] for k in ['actor','handle','current','maximum','baseMaximum']),'Native source player health and identity preserved')
info=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames,avg_frame_rate','-of','json',str(folder/'native.mp4')]))['streams'][0]
fps=int(info['nb_frames'])/float(info['duration'])
check(fps>=24 and 23.5<=float(info['duration'])<=24.5,'Real-time native recording retains at least 24 captured frames per second')
result=dict(passed=True,complete=False,remote_gameplay_bound=False,visual_accepted=False,scope='Native source-reader proof only. Remote full-body/corpse attachment, physics and release binding remain incomplete.',character=args.character,take=args.take,checks=checks,samples=len(rows),accepted_samples=len(valid),variants=dict(counts),independent_scene_matches=independent,serial_probe_retirement_boundaries=retirement_boundaries,actual_capture_hz=fps,video_sha256=sha(folder/'native.mp4'),observations_sha256=sha(folder/'observations.json'),identity=identity)
write(folder/'result.json',result)
print(json.dumps({k:result[k] for k in ['take','passed','samples','accepted_samples','variants','independent_scene_matches','actual_capture_hz']}))
