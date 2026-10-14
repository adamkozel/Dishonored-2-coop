import json
from pathlib import Path
P=Path(__file__).resolve().parent;rows=json.loads((P/'native-allcatalog-sample-manifest.json').read_text())['rows'];sources={}
for attacker in ['corvo','emily']:
 for phase in ['chokeback_in','chokefront_in','chokeleft_in','chokeright_in','choke_loop','choke_win','choke_cancel']:
  alias='generic_'+attacker+'_'+phase
  row=next(r for r in rows if alias in r['aliases']and r['source_role']=='corvo'and r['status']=='native_pose_sampled_offline')
  sources[row['id']]=dict(alias=alias,family='paired_victim_reference',purpose='Native nonlethal VICTIM response, never the player attacker.',targets=['corvo'])
(P/'choke-victim-selection.json').write_text(json.dumps(dict(sources=sources),indent=2));print('SELECTED',len(sources))
