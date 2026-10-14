"""Pin exact reload phase/event clocks; source metadata is not executed."""
import json,hashlib,shutil
from pathlib import Path
from inspect_midnight_events import parse,walk
P=Path(__file__).resolve().parent;out=P/'item-research';out.mkdir(exist_ok=True);result=[]
manifest=json.loads((P.parent.parent/'outputs/d2-coop-lab/research/asset-exports/natural-crossbow-scenes/manifest.json').read_text(encoding='utf-8-sig'))
for r in manifest['Resources']:
 path=Path(r['ExportFile']);local=out/path.name;shutil.copyfile(path,local);d=parse(path.read_text());groups={v['m_UID']['m_objUID']:v['m_name']for k,v in d['edit']['m_groupsInfo'].items()if k.startswith('item[')};names={};rows=[]
 for o in walk(d):
  if 'm_groups'in o and'm_properties'in o:names[o['m_objUID']['m_objUID']]=o['m_properties']['m_name']
  if 'm_keyProp'in o:
   prop=o['m_keyProp'];h=o['m_handle'];rows.append(dict(scene_id=h['sceneUID'],group=groups.get(h['groupUID']),start_ms=o['m_startTime'],duration_ms=o['m_duration'],class_name=prop.get('m_class'),properties=prop.get('m_object',{})))
 for row in rows:row['scene_name']=names.get(row['scene_id'])
 result.append(dict(asset=r['Name'],sha256=hashlib.sha256(path.read_bytes()).hexdigest(),file=str(local.relative_to(P)),names=names,rows=rows))
(out/'native-crossbow-scenes.json').write_text(json.dumps(result,indent=2))
for r in result:
 print(r['asset'])
 for x in r['rows']:
  if x['properties'].get('m_AnimName')or x['properties'].get('m_event'):print(x['scene_name'],x['group'],x['start_ms'],x['duration_ms'],{k:v for k,v in x['properties'].items()if k in ['m_AnimName','m_event']})
