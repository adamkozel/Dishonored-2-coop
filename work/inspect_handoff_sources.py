import json,os,re,collections
from pathlib import Path
P=Path(__file__).resolve().parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
state=read(P/'work/current-task-state.json')
print('STATE KEYS',list(state))
m=state['MovementIntegration'];print('MOVEMENT KEYS',list(m));print('LATEST',json.dumps(m.get('LatestContinuation'),indent=2)[:9000])
c=read(P/'work/natural-moveset/review-catalog.json')
print('CATALOG KEYS',list(c));print('ITEM FAMILIES',collections.Counter(i['family'] for i in c['items']))
for k,v in c.items():
 if k not in ['items','clips','candidates','videos','recordings']:print(k,str(v)[:500])
roots=['outputs/d2-coop-lab/analysis','outputs/d2-coop-lab/research','work/natural-moveset','work/movement-integration','work/mixamo-crouch','work/moveset-growth','work/sword-grip-fit','work/animgen-feasibility','work/fullbody-proof']
found=[]
skip={'node_modules','.venv','__pycache__','project','snapshots','input','Intermediate','Saved','Binaries','build','src','Engine','Content'}
for root in roots:
 for dp,ds,fs in os.walk(P/root,followlinks=False):
  ds[:]=[d for d in ds if d not in skip and not (Path(dp)/d).is_symlink() and not (Path(dp)/d).is_junction()]
  for name in fs:
   p=Path(dp)/name
   if p.suffix.lower() in {'.json','.txt','.asm','.md','.log'} and re.search('decomp|ghidra|findings|disasm|disassembl',str(p.relative_to(P)),re.I):
    found.append({'path':p.relative_to(P).as_posix(),'bytes':p.stat().st_size})
(P/'work/handoff-decomp-inventory.json').write_text(json.dumps(found,indent=2))
print('DECOMP',len(found),sum(x['bytes'] for x in found));print('LARGEST',sorted(found,key=lambda x:-x['bytes'])[:12])
print('RESEARCH milestone count',len(list((P/'outputs/d2-coop-lab/research/milestones').glob('*'))))
for name in ['Checkpoint','SharedCheckpoint','SharedDeath','FriendlyFire','Doppelganger','Mixamo','NaturalMoveset']:
 for k,v in state.items():
  if name.lower() in k.lower():print(k,json.dumps(v)[:3500])
