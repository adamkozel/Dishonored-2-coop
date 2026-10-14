"""Read exact exported power scenes, preserving native phase/event evidence."""
import json
from pathlib import Path
from inspect_midnight_events import parse,walk
P=Path(__file__).resolve().parent
folder=P.parents[1]/'outputs/d2-coop-lab/research/asset-exports/native-bendtime-v1'
m=json.loads((folder/'manifest.json').read_text(encoding='utf-8-sig'));out=[]
for r in m['Resources']:
 if r['Class']!='midnightScene':continue
 d=parse(Path(r['ExportFile']).read_text());scenes=[];clips=[];events=[]
 for o in walk(d):
  if 'm_properties'in o:scenes.append(o['m_properties'])
  if 'm_keyProp'in o:
   v=o['m_keyProp']['m_object'];row=dict(start=o.get('m_startTime'),duration=o.get('m_duration'),body=v)
   if o['m_keyProp']['m_class']=='arkMidnightKeyPropAnim':clips.append(row)
   else:events.append(dict(kind=o['m_keyProp']['m_class'],**row))
 out.append(dict(name=r['Name'],sha256=r['Sha256'],scenes=scenes,clips=clips,other_tracks=events))
(P/'bendtime-scene-summary-v1.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out,indent=2))
