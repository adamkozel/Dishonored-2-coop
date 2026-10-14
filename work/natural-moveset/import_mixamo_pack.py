"""Pin and safely unpack the user-authorized Mixamo body-motion source bundle."""
from pathlib import Path
import zipfile,hashlib,json,shutil
P=Path(__file__).resolve().parent;source=Path.home()/'Downloads/Action Adventure Pack.zip';out=P/'mixamo-adventure';out.mkdir(exist_ok=True)
target=out/'source.zip'
if target.exists():assert target.read_bytes()==source.read_bytes()
else:shutil.copy2(source,target)
rows=[]
with zipfile.ZipFile(target) as z:
 assert len(z.infolist())<100 and sum(e.file_size for e in z.infolist())<100_000_000
 for e in z.infolist():
  name=Path(e.filename);assert name.name==e.filename and name.suffix.lower()=='.fbx'
  data=z.read(e);path=out/name
  if path.exists():assert path.read_bytes()==data
  else:path.write_bytes(data)
  rows.append(dict(file=e.filename,sha256=hashlib.sha256(data).hexdigest(),bytes=len(data)))
report=dict(source_url='https://www.mixamo.com/#/?page=1&query=Crouched+Walking&type=Motion%2CMotionPack',source_title='Action Adventure Pack',advertised_animations=22,character='X Bot',format='FBX Binary',pose='T-pose',fps=60,keyframe_reduction='none',pack_settings='Bundle defaults; individual Posture/In Place controls unavailable for bundle. Original root motion retained only as authoring metadata; must never drive network world placement.',download='2026-10-10',archive_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),files=rows)
(out/'source-manifest.json').write_text(json.dumps(report,indent=2));print('PINNED',len(rows),'FBX files',report['archive_sha256'])
