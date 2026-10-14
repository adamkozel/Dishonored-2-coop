"""Read exact native scene clocks for sliding/window victim references."""
import json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
from inspect_midnight_events import parse,walk
manifest=json.loads((P/'pair-research/native-scenes/manifest.json').read_text());out=[]
for r in manifest:
 if not r['asset'].endswith(('choke_slide_back.midnightscene','assassination_slide_back.midnightscene','choke_window_back.midnightscene','assassination_window_back.midnightscene')):continue
 path=P/r['file'];d=parse(path.read_text());groups={v['m_UID']['m_objUID']:v['m_name']for k,v in d['edit']['m_groupsInfo'].items()if k.startswith('item[')};mapping={};rows=[]
 for obj in walk(d):
  if 'm_md6'in obj and 'm_nextInfo'in obj:mapping[obj['m_nextInfo']['m_nextCPoint']['sceneUID']]=obj['m_md6']
  if 'm_keyProp'not in obj:continue
  prop=obj['m_keyProp'];cls=prop.get('m_class','');h=obj['m_handle']
  if groups.get(h.get('groupUID'),'').startswith('AI')and('StartRagdoll'in cls or cls=='arkMidnightKeyPropAnim'):
   rows.append(dict(scene=h['sceneUID'],group_name=groups[h['groupUID']],class_name=cls,start_ms=obj['m_startTime'],duration_ms=obj['m_duration'],properties=prop.get('m_object',{})))
 for row in rows:row['player_body']=mapping.get(row['scene'])
 out.append(dict(asset=r['asset'],sha256=hashlib.sha256(path.read_bytes()).hexdigest(),rows=rows))
 print(r['asset'],[(x['player_body'],x['class_name'],x['start_ms'],x['duration_ms'])for x in rows])
(P/'pair-research/native-context-event-times.json').write_text(json.dumps(out,indent=2))
