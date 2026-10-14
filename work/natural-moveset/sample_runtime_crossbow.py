"""Sample the skeleton referenced by the retail Corvo model, not its namesake."""
import hashlib,json,subprocess
from pathlib import Path
from edge_data import skeleton
P=Path(__file__).resolve().parent
R=P.parents[1]
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
assets=R/'outputs/d2-coop-lab/research/asset-exports'
sk=read(assets/'crossbow-runtime-skeleton-v1/manifest.json')['Resources'][0]
out=P/'item-mechanisms-runtime-v2';out.mkdir(exist_ok=True)
rig=skeleton(sk['ExportFile']);live=read(P.parent/'movement-integration/host-crossbow-skeleton-namecheck-v1.json')
assert live['stable'] and live['skeleton']==sk['Name'] and len(rig['joints'])==len(live['joints'])==15
for a,b in zip(rig['joints'],live['joints']):
 assert a['nativeName']==b['nativeName'] and a['parent']==b['parent']
 assert max(abs(x-y) for x,y in zip(a['bindLocalQTS'],b['bindLocalQTS']))<1e-7
(out/'crossbow_corvo-skeleton.json').write_text(json.dumps(rig,indent=2))
refs=read(assets/'natural-item-mechanisms/manifest.json')['Resources']
for part in ('idle','fire','outofammo'):
 key=f'crossbow_corvo-crossbow_{part}';old=read(P/'item-mechanisms'/f'{key}.json')
 asset=next(a for a in refs if a['Name']==old['asset'])
 path=out/f'{key}.edgepose';assert not path.exists()
 r=subprocess.run([str(P/'build/Release/d2_sample_edge.exe'),'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe',asset['ExportFile'],sk['ExportFile'],str(path)],capture_output=True,text=True,timeout=40)
 assert r.returncode==0,(r.stdout,r.stderr)
 meta=dict(id=key,asset=asset['Name'],asset_sha256=asset['Sha256'],skeleton=sk['Name'],skeleton_sha256=sk['Sha256'],sampling=json.loads(r.stdout),sample_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),events=False)
 path.with_suffix('.json').write_text(json.dumps(meta,indent=2))
 print(key,meta['sampling'],flush=True)
