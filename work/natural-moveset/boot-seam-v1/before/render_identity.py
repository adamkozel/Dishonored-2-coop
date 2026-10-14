"""Pin the animation, trajectories, props, models and renderer behind a video."""
import json, hashlib, functools
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(value):return hashlib.sha256(json.dumps(value,sort_keys=True,separators=(',',':')).encode()).hexdigest()
@functools.lru_cache(None)
def file_sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def data_identity(pair):
 data={role:{k:m.get(k)for k in ['sha256','root_track','props','equipment_policy','frames','fps']}for role,m in pair.items()}
 for role,m in pair.items():
  if 'preview_placement'in m:data[role]['preview_placement']=m['preview_placement']
  if 'preview_view'in m:data[role]['preview_view']=m['preview_view']
  if 'preview_caption'in m:data[role]['preview_caption']=m['preview_caption']
 return digest(data)
def fingerprint(pair,overrides=None):
 models={}
 for role in ['corvo','emily']:
  for part in ['body','head']:
   path=(overrides or {}).get(role+'-'+part,(P.parent/'fullbody-proof'if role=='corvo'else P.parent/'moveset-growth')/f'{role}-{part}.json')
   models[role+'-'+part]=dict(path=str(Path(path).relative_to(P.parent)),sha256=file_sha(str(path)))
 models['sword']=dict(path='fullbody-proof/sword.json',sha256=file_sha(str(P.parent/'fullbody-proof/sword.json')))
 for role,m in pair.items():
  for prop in m.get('props',[]):
   for field in ['model','skeleton']:
    path=P/prop[field];models[role+'-'+prop['kind']+'-'+field]=dict(path=str(path.relative_to(P.parent)),sha256=file_sha(str(path)))
   mechanisms=[prop.get('mechanism')]+[s.get('mechanism')for s in prop.get('mechanism_segments',[])]
   for mechanism in mechanisms:
    if mechanism:
     # The sampled mechanism includes its own hash in the animation metadata.
     models[role+'-'+mechanism['id']]=dict(mechanism=mechanism)
 scripts={name:file_sha(str(P/name))for name in ['render_library.py','preview_natural_walk.py','native_prop_preview.py','render_identity.py']}
 inputs=dict(data_identity=data_identity(pair),models=models,scripts=scripts)
 return dict(render_identity=digest(inputs),render_inputs=inputs)

if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('manifest');p.add_argument('folder');p.add_argument('--high-detail-emily',action='store_true');args=p.parse_args()
 rows=json.loads((P/args.manifest).read_text(encoding='utf-8-sig'));groups={}
 for row in rows:groups.setdefault(row['selection']['alias'],{})[row['role']]=row
 overrides={'emily-body':P/'item-models/emily-body-high.json'}if args.high_detail_emily else{}
 for alias,pair in groups.items():
  path=P/args.folder/(alias+'.json');meta=json.loads(path.read_text());assert 'render_identity'not in meta
  meta.update(fingerprint(pair,overrides));meta['provenance_note']='Fingerprint recorded immediately after the completed render, before renderer modification.'
  path.write_text(json.dumps(meta,indent=2))
 print('PINNED_RENDER_INPUTS',len(groups))
