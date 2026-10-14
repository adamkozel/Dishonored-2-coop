"""Record carry phases and ownership markers from exact native scenes."""
import json,hashlib,shutil
from pathlib import Path
from inspect_midnight_events import parse,walk
P=Path(__file__).resolve().parent;out=P/'carry-research';rows=[]
assets=json.loads((P.parent.parent/'outputs/d2-coop-lab/research/asset-exports/natural-carry-scenes/manifest.json').read_text(encoding='utf-8-sig'))['Resources']
for asset in assets:
 if '/carrycorpse/'not in asset['Name']:continue
 path=Path(asset['ExportFile']);d=parse(path.read_text());groups={v['m_UID']['m_objUID']:v['m_name']for k,v in d['edit']['m_groupsInfo'].items()if k.startswith('item[')};events=[];names={}
 for o in walk(d):
  if 'm_properties'in o and'm_groups'in o:names[o['m_objUID']['m_objUID']]=o['m_properties']['m_name']
  if 'm_keyProp'in o:
   prop=o['m_keyProp'];h=o['m_handle'];body=prop.get('m_object',{});events.append(dict(scene=h['sceneUID'],group=groups.get(h['groupUID']),start_ms=o['m_startTime'],duration_ms=o['m_duration'],class_name=prop.get('m_class'),properties=body))
 for e in events:e['scene_name']=names.get(e['scene'])
 local=out/path.name;shutil.copyfile(path,local);rows.append(dict(asset=asset['Name'],sha256=asset['Sha256'],file=str(local.relative_to(P)),rows=events))
 print(asset['Name'])
 for e in events:
  if any(s in e['class_name']for s in ['Anim','Ragdoll','Carry','Event']):print(e['scene_name'],e['group'],e['start_ms'],e['duration_ms'],e['class_name'],{k:v for k,v in e['properties'].items()if k in ['m_AnimName','m_event','m_tag','m_entity']})
(out/'native-carry-scenes.json').write_text(json.dumps(rows,indent=2))
