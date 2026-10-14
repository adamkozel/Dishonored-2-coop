from pathlib import Path
import json, collections, subprocess, sys
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';T=P/'possession-fx-observe-v349'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
rows=[json.loads(s) for s in (T/'possession-effects.ndjson').read_text().splitlines()]
effects={};slots={};start=rows[0]['tick']
for row in rows:
 if not row['valid']:continue
 for e in row['effects']:
  key=(e['name'],e['id']);x=effects.setdefault(key,dict(name=e['name'],id=e['id'],owners=set(),parents=set(),samples=0,states=collections.Counter(),firstMs=row['tick']-start,lastMs=0,matchedSlotSamples=0))
  x['samples']+=1;x['lastMs']=row['tick']-start;x['owners'].add(e['owner']);x['parents'].add(e['parent'])
  if row['possessionValid']:
   x['states'][row['internalState']]+=1
   if any(s['valid'] and s['id']==e['id'] and s['name']==e['name'] for s in row['controllerSlotsResearch']):x['matchedSlotSamples']+=1
 for s in row['controllerSlotsResearch']:
  if s['valid'] and s['name']:slots.setdefault(s['index'],set()).add(s['name'])
out=[]
for x in effects.values():
 x['owners']=sorted(x['owners']);x['parents']=sorted(x['parents']);x['states']=dict(x['states']);out.append(x)
write(T/'fx-ownership-analysis-v350.json',dict(samples=len(rows),valid=sum(x['valid'] for x in rows),effects=out,controllerSlots={k:sorted(v) for k,v in slots.items()},scope='Only possessionValid rows attribute native states/slots. Full scans are external research, not production hooks. Target effect has no proven selected-target pointer yet.'))
review=dict(inspected=True,method='Viewed source and peer temporal sheets at 1,2.8,4.4,6.4,7.9,9.8 seconds',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes='Source shows green target particles during native cast, occupied vision and normal equipment recovery. Peer shows Corvo casting, disappearing and returning, but a native guard obscures and overlaps the return; late camera reframe primarily shows a guard. No remote target particles are implemented. No audio captured. This is ownership research and body lifecycle evidence, not finished NPC/world presentation.')
write(T/'visual-review.json',review)
notes='DLL343 passes native paired equipment exit and continued movement343/345, receiver-stop344, both native menu interruptions347, and read-only effects349(16 checks). Probe348 observes distinct target particles and four active player/camera effects; five controller slots are mapped. Remote target FX, NPC control/shared outcomes and full movement remain unfinished. All completed cases restore isolated checkpoints and protected saves.'
subprocess.run([sys.executable,str(R/'work/movement-integration/record_reviewed_possession_take.py'),T.name,'--label','native effect ownership v349','--notes',notes],check=True)
print(json.dumps(dict(effects=out,slots={k:sorted(v) for k,v in slots.items()})))
