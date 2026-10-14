"""Check corrected recording provenance without changing authored motion data."""
import json,hashlib,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
catalog=read(P/'review-catalog.json');prior=read(P/'boot-seam-v1/before/review-catalog.json');old={m['id']:m for m in prior['motions']};rows=[]
for m in catalog['motions']:
 failures=[]
 if m['roles']!=old[m['id']]['roles']:failures.append('Animation identity or duration changed')
 if len(m['videos'])!=1:failures.append('Expected one selected corrected movie')
 for v in m['videos']:
  meta=read((P/v['path']).with_suffix('.json'))
  previous=old[m['id']]['videos'][0]
  if abs(v['duration']-previous['duration'])>.04 or v['fps']!=previous['fps']:failures.append('Playback timing changed')
  previous_meta=read((P/previous['path']).with_suffix('.json'))
  for part in ['corvo-body','corvo-head','sword']:
   a=meta['render_inputs']['models'][part]['sha256'];b=previous_meta.get('render_inputs',{}).get('models',{}).get(part,{}).get('sha256')
   if b and a!=b:failures.append('Corvo geometry changed: '+part)
  if meta.get('skin_preview_version')!='corvo-shin-anchors-v1':failures.append('Old skin preview selected')
  for name,digest in meta['render_inputs']['scripts'].items():
   if sha(P/name)!=digest:failures.append('Renderer mismatch: '+name)
  for part,source in meta['render_inputs']['models'].items():
   if 'path'in source and sha(P.parent/source['path'])!=source['sha256']:failures.append('Model mismatch: '+part)
  if 'paired_renderer_sha256'in meta and meta['paired_renderer_sha256']!=sha(P/'render_paired.py'):failures.append('Paired renderer mismatch')
 rows.append(dict(alias=m['id'],passed=not failures,failures=failures))
report=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=len(rows)==482 and all(r['passed']for r in rows),clips=len(rows),scope='Every selected movie uses corrected calf helpers and original animation data; exact pinned renderers/models. Does not imply visual acceptance of each motion.',failures=[r for r in rows if not r['passed']])
(P/'boot-seam-v1/rebuild-audit.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2));assert report['passed']
