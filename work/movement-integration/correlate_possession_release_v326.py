from pathlib import Path
import json,bisect
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset/possession-human-v326'
read=lambda name:[json.loads(x)for x in(P/name).read_text(encoding='utf-8-sig').splitlines()if x.strip()]
target=read('target-state.ndjson');states=read('owned-scenes.ndjson');ts=[x['tick']for x in target];ss=[x['tick']for x in states]
def nearest(rows,ticks,tick):
 i=bisect.bisect_left(ticks,tick);idx=min(range(max(0,i-1),min(len(rows),i+1)),key=lambda j:abs(ticks[j]-tick));return rows[idx],ticks[idx]-tick
out=[]
for frame in read('midnight.ndjson'):
 for scene in frame.get('instances',[]):
  if scene.get('state')!=3 or not scene.get('asset','').endswith('/power_possession_cast_out.midnightscene'):continue
  t,td=nearest(target,ts,frame['tick']);s,sd=nearest(states,ss,frame['tick'])
  out.append(dict(tick=frame['tick'],scene=scene['id'],clock=scene['clockWords'][0],duration=scene['sceneDurationMs'],bindings=scene.get('bindingsResearch'),targetOffsetMs=td,target=t,stateOffsetMs=sd,state=s))
assert out
(P/'release-correlations.json').write_text(json.dumps(dict(nonAtomic=True,scope='Nearest independent watcher samples, diagnostic correlation only; strict co-owned reader still required',samples=out),indent=2))
contexts=set((x['target'].get('internalState'),x['target'].get('physicsStance'),tuple(x['state'].get('activeStates',[])))for x in out)
print(json.dumps(dict(samples=len(out),contexts=sorted(contexts,key=str),maxOffsetMs=max(max(abs(x['targetOffsetMs']),abs(x['stateOffsetMs']))for x in out))))
