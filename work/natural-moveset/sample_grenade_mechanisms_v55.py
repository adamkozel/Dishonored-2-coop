"""Offline native grenade model animation, with no events or gameplay."""
import json,hashlib,subprocess
from pathlib import Path
from edge_data import skeleton
P=Path(__file__).resolve().parent;R=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
assets=read(R/'outputs/d2-coop-lab/research/asset-exports/grenade-model-reference-v54/manifest.json')['Resources']
sk=next(r for r in assets if r['Name']=='models/weapons/grenade/grenade.md6skl')
(P/'item-mechanisms/grenade-skeleton.json').write_text(json.dumps(skeleton(sk['ExportFile']),indent=2))
out=P/'grenade-mechanisms-v55';out.mkdir(exist_ok=True);report=[]
for a in assets:
 if not a['Name'].endswith('md6anim') or '/sticky/' in a['Name']:continue
 alias=Path(a['Name']).stem;target=out/(alias+'.edgepose')
 r=subprocess.run([str(P/'build/Release/d2_sample_edge.exe'),'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe',a['ExportFile'],sk['ExportFile'],str(target)],capture_output=True,text=True,timeout=40)
 assert r.returncode==0,(a['Name'],r.stderr)
 report.append(dict(id=alias,kind='grenade',asset=a['Name'],asset_sha256=a['Sha256'],skeleton=sk['Name'],skeleton_sha256=sk['Sha256'],sampling=json.loads(r.stdout),sample_file=str(target.relative_to(P)),sample_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),events=False))
(P/'grenade-mechanism-manifest-v55.json').write_text(json.dumps(report,indent=2));print('Grenade mechanism clips sampled:',len(report))
