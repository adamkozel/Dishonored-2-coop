"""Extract native ranged animation/event timings without executing them."""
import json,hashlib
from pathlib import Path
from inspect_midnight_events import parse,walk
P=Path(__file__).resolve().parent;ROOT=P.parents[1];rows=[]
for batch in ('ranged-source-clocks-v1','ranged-fire-fx-reference-v1'):
 manifest=json.loads((ROOT/'outputs/d2-coop-lab/research/asset-exports'/batch/'manifest.json').read_text(encoding='utf-8-sig'))
 for r in manifest['Resources']:
  path=Path(r['ExportFile']);d=parse(path.read_text());groups={v['m_UID']['m_objUID']:v['m_name']for k,v in d['edit']['m_groupsInfo'].items()if k.startswith('item[')}
  scene_names={obj['m_UID']['m_objUID']:obj['m_name']for obj in walk(d)if 'm_UID'in obj and 'm_name'in obj}
  keys=[]
  for obj in walk(d):
   if 'm_keyProp'not in obj:continue
   h=obj['m_handle'];prop=obj['m_keyProp'];keys.append(dict(scene=h['sceneUID'],group=groups.get(h['groupUID']),key=h['keyUID'],start_ms=obj['m_startTime'],duration_ms=obj['m_duration'],kind=prop['m_class'],properties=prop.get('m_object',{})))
  rows.append(dict(asset=r['Name'],sha256=hashlib.sha256(path.read_bytes()).hexdigest(),groups=groups,names=scene_names,keys=keys))
(P/'ranged-native-scene-tracks-v1.json').write_text(json.dumps(rows,indent=2))
for r in rows:
 print(r['asset'])
 for k in r['keys']:
  if k['kind']!='arkMidnightKeyPropAnim':print(k['scene'],k['group'],k['start_ms'],k['kind'],str(k['properties'])[:750])
