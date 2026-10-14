"""Keep native victim clips explicitly separate from remote-player actor clips."""
import json
from pathlib import Path
P=Path(__file__).resolve().parent
rows=json.loads((P/'native-allcatalog-sample-manifest.json').read_text())['rows'];sources={}
for attacker in ['corvo','emily']:
 for direction in ['front','back']:
  alias=f'generic_{attacker}_doppelganger_assassination_fast{direction}'
  for victim in ['corvo','emily']:
   row=next(r for r in rows if alias in r['aliases']and r['source_role']==victim and r['status']=='native_pose_sampled_offline')
   sources[row['id']]=dict(alias=alias,family='paired_victim_reference',purpose='Native VICTIM response to the named attacker. Never a player attacker motion.',targets=[victim])
(P/'pair-victim-selection.json').write_text(json.dumps(dict(sources=sources),indent=2));print('SELECTED',len(sources),'victim sources')
