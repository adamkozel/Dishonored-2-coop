"""Sample exact installed out-of-mana references; no game-process writes."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
native=read(R/'outputs/d2-coop-lab/research/asset-exports/native-outofmana-v45/manifest.json')
prior=read(R/'outputs/d2-coop-lab/research/asset-exports/natural-power-reference/manifest.json')
sk=next(r for r in prior['Resources'] if r['Name'].endswith('.md6skl'))
assert hashlib.sha256(Path(sk['ExportFile']).read_bytes()).hexdigest().lower()==sk['Sha256'].lower()
out=P/'outofmana-reference-v45';out.mkdir(exist_ok=True);rows=[]
for role in ('corvo','emily'):
 r=next(r for r in native['Resources'] if f'/{role}/leftarm/ply_leftarm_power_outofmana.md6anim' in r['Name'])
 assert hashlib.sha256(Path(r['ExportFile']).read_bytes()).hexdigest().lower()==r['Sha256'].lower()
 target=out/(role+'.edgepose')
 run=subprocess.run([str(P/'build/Release/d2_sample_edge.exe'),'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe',r['ExportFile'],sk['ExportFile'],str(target)],capture_output=True,text=True,timeout=40)
 assert run.returncode==0,run.stdout+run.stderr
 rows.append(dict(role=role,asset=r['Name'],asset_sha256=r['Sha256'],sample=str(target.relative_to(P)),sample_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),sampling=json.loads(run.stdout),skeleton=sk['Name'],skeleton_sha256=sk['Sha256']))
(P/'outofmana-reference-manifest-v45.json').write_text(json.dumps(dict(scope='Native 333 ms scene timing and finger reference only; no first-person wrist constraints. Shared native left-arm reference skeleton, as in the prior power-reference sampler.',rows=rows),indent=2))
print('OUTOFMANA_REFERENCES',len(rows))
