"""Re-render the current catalog with corrected calf helpers; keep old movies.

Separate headless Blender processes have private scenes and output folders.
No game commands, save writes, animation re-authoring or live DLL changes.
"""
import json,sys,subprocess,concurrent.futures,datetime,hashlib,argparse
from pathlib import Path
P=Path(__file__).resolve().parent
BLENDER='C:/Program Files/Blender Foundation/Blender 5.2/blender.exe'
parser=argparse.ArgumentParser();parser.add_argument('--workers',type=int,choices=[1,2],default=1);args=parser.parse_args()
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
catalog=read(P/'boot-seam-v1/before/review-catalog.json')
# Use the exact source precedence of the saved catalog builder, without running
# its output-writing portion. Do not synthesize new animation metadata.
ns={'__file__':str(P/'build_review_catalog.py')}
exec((P/'build_review_catalog.py').read_text().split('motions={};pairs={}')[0],ns)
rows={}
for filename in ns['sources']:
 for r in read(P/filename):rows[(r['selection']['alias'],r['role'])]=r
bins={kind:[dict(seconds=0,rows=[])for _ in range(4)]for kind in ['single','paired']}
for m in sorted(catalog['motions'],key=lambda m:-max(r['duration']for r in m['roles'].values())):
 pair=[rows[(m['id'],role)]for role in ['corvo','emily']]
 for row in pair:
  assert row['sha256']==m['roles'][row['role']]['sha256']==sha(P/row['binary'])
 kind='paired'if 'partner'in pair[0]else'single';slot=min(bins[kind],key=lambda v:v['seconds']);slot['rows']+=pair;slot['seconds']+=max(r['duration']for r in pair)
jobs=[]
for kind,parts in bins.items():
 for i,part in enumerate(parts):
  if not part['rows']:continue
  stem=f'skin-review-v1-{kind}-{i+1}';manifest=stem+'-manifest.json';(P/manifest).write_text(json.dumps(part['rows'],indent=2))
  jobs.append(dict(kind=kind,folder=stem,manifest=manifest,clips=len(part['rows'])//2,duration=part['seconds']))
statuspath=P/'boot-seam-v1/rebuild-status.json'
state=dict(started=datetime.datetime.now().astimezone().isoformat(),complete=False,workers=args.workers,expected_clips=len(catalog['motions']),jobs=jobs,results=[])
statuspath.write_text(json.dumps(state,indent=2))
def run(job):
 script='render_paired.py'if job['kind']=='paired'else'render_library.py'
 command=[BLENDER,'--factory-startup','-b','-t','3','--python-exit-code','1','--python',str(P/script),'--','--manifest',job['manifest'],'--folder',job['folder']]
 if job['kind']=='single':command.append('--high-detail-emily')
 with (P/'boot-seam-v1'/f"{job['folder']}.log").open('ab')as log:
  result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,creationflags=getattr(subprocess,'CREATE_NO_WINDOW',0))
 return dict(folder=job['folder'],exit_code=result.returncode,clips=job['clips'])
with concurrent.futures.ThreadPoolExecutor(max_workers=args.workers)as pool:
 for result in pool.map(run,jobs):
  state['results'].append(result);statuspath.write_text(json.dumps(state,indent=2));print(json.dumps(result),flush=True)
state['complete']=all(r['exit_code']==0 for r in state['results']) and len(state['results'])==len(jobs)
state['finished']=datetime.datetime.now().astimezone().isoformat();statuspath.write_text(json.dumps(state,indent=2));assert state['complete']
print('SKIN_LIBRARY_REBUILD_COMPLETE',state['expected_clips'],flush=True)
