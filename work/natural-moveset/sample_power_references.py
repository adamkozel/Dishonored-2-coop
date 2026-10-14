"""Read-only first-person gesture/timing references; no retarget or wrist anchors."""
from pathlib import Path
import json,hashlib,subprocess,argparse
from edge_data import skeleton
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
parser=argparse.ArgumentParser();parser.add_argument('--batch',default='natural-power-reference');parser.add_argument('--selection',default='power-reference-selection.json');parser.add_argument('--samples',default='fp-power-samples');parser.add_argument('--manifest',default='fp-power-reference-manifest.json');parser.add_argument('--skeleton-output',default='fp-leftarm-skeleton.json');args=parser.parse_args()
exports=read(ROOT/'outputs/d2-coop-lab/research/asset-exports'/args.batch/'manifest.json')['Resources'];refs=read(P/args.selection);paths={r['Name']:r for r in sorted(exports,key=lambda r:(r['Patch'],'patch'in r['Catalog']))}
sk=next(r for r in paths.values()if r['Name'].endswith('md6skl'));data=skeleton(sk['ExportFile']);skout=P/args.skeleton_output
if skout.exists():assert read(skout)['sha256']==data['sha256'], 'Refusing to replace another reference skeleton; use --skeleton-output'
skout.write_text(json.dumps(data,indent=2));out=P/args.samples;out.mkdir(exist_ok=True);rows=[]
for asset,r in paths.items():
 if not asset.endswith('md6anim'):continue
 key=hashlib.sha256((asset+'\n'+sk['Name']).encode()).hexdigest()[:16];file=out/(key+'.edgepose');metadata=out/(key+'.json')
 row=dict(id=key,asset=asset,asset_sha256=r['Sha256'],aliases=sorted({x['alias']for x in refs if x['asset']==asset}),skeleton=sk['Name'],skeleton_sha256=sk['Sha256'],status='reference_only',first_person_constraints=False)
 if metadata.exists():
  previous=read(metadata);assert previous['asset_sha256']==row['asset_sha256'];row=previous
 else:
  run=subprocess.run([str(P/'build/Release/d2_sample_edge.exe'),'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe',r['ExportFile'],sk['ExportFile'],str(file)],capture_output=True,text=True,timeout=40)
  if run.returncode==0:row.update(status='sampled_reference_only',sampling=json.loads(run.stdout),sample_file=str(file.relative_to(P)),sample_sha256=hashlib.sha256(file.read_bytes()).hexdigest())
  else:row.update(status='sampling_failed',error=run.stdout+run.stderr)
  metadata.write_text(json.dumps(row,indent=2))
 rows.append(row);print(row['aliases'][0],row['status'],flush=True)
report=dict(scope='First-person timing, palm orientation and finger-shape references only. These are not full-body motion assets and do not impose camera-space wrist constraints.',rows=rows,missing=sorted({r['asset']for r in refs}-set(paths)))
(P/args.manifest).write_text(json.dumps(report,indent=2));print('REFERENCE_SAMPLES',sum(r['status']=='sampled_reference_only'for r in rows))
