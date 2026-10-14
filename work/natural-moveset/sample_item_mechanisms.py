"""Read-only native weapon/Heart mechanism sampling; no animation events."""
import json,hashlib,subprocess
from pathlib import Path
from edge_data import skeleton
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
base=ROOT/'outputs/d2-coop-lab/research/asset-exports'
models=read(base/'natural-item-models/manifest.json')['Resources'];assets=read(base/'natural-item-mechanisms/manifest.json')['Resources']
out=P/'item-mechanisms';out.mkdir(exist_ok=True);report=[]
for asset in assets:
 name=asset['Name'];kind='heart'if'/heart/'in name else 'crossbow_emily'if'/emily/'in name else'crossbow_corvo'
 sk=next(r for r in models if r['Name'].endswith('/'+kind+'.md6skl'))
 (out/(kind+'-skeleton.json')).write_text(json.dumps(skeleton(sk['ExportFile']),indent=2))
 key=kind+'-'+Path(name).stem;path=out/(key+'.edgepose');meta=out/(key+'.json')
 if meta.exists():
  m=read(meta);assert m['asset_sha256']==asset['Sha256'] and m['skeleton_sha256']==sk['Sha256']
 else:
  r=subprocess.run([str(P/'build/Release/d2_sample_edge.exe'),'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe',asset['ExportFile'],sk['ExportFile'],str(path)],capture_output=True,text=True,timeout=40)
  assert r.returncode==0,(name,r.stdout,r.stderr)
  m=dict(id=key,kind=kind,asset=name,asset_sha256=asset['Sha256'],skeleton=sk['Name'],skeleton_sha256=sk['Sha256'],sampling=json.loads(r.stdout),sample_file=str(path.relative_to(P)),sample_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),events=False)
  meta.write_text(json.dumps(m,indent=2))
 report.append(m);print('ITEM_MECHANISM',key,m['sampling']['joints'],m['sampling']['frames'],flush=True)
(P/'item-mechanism-manifest.json').write_text(json.dumps(report,indent=2))
