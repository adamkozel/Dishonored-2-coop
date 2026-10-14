"""Select additional native actor interactions, keeping corpse/victim poses out."""
from pathlib import Path
import json
P=Path(__file__).resolve().parent
rows=json.loads((P/'native-expanded-sample-manifest.json').read_text())['rows']
requests={}
def add(family,aliases,purpose):
 for a in aliases.split():requests[a]=dict(family=family,purpose=purpose)
add('stance','empty_distraction_look_keyhole_in_walkright empty_distraction_look_keyhole_loop empty_distraction_look_keyhole_out_fast empty_distraction_look_keyhole_out_walk','Authored full-body keyhole sequence; door contact must be adapted')
add('interactions','empty_distraction_loot_ground_in empty_distraction_loot_ground_loop empty_distraction_loot_ground_out empty_distraction_loot_ground_out_fast','Native ground pickup donor, requiring item placement and contact alignment')
add('interactions','empty_distraction_sitting_bench_in empty_distraction_sitting_bench_in_walkright empty_distraction_sitting_bench_loop empty_distraction_sitting_bench_out empty_distraction_sitting_bench_out_fast empty_distraction_sitting_bench_out_walk','Native sit/stand sequence; seat geometry remains a contact parameter')
add('combat','generic_deathfast_back generic_deathfast_front generic_deathfast_left generic_deathfast_right empty_stinghit_death','Native death reactions; ragdoll handoff and shared death UI remain separate')
add('interactions','empty_distraction_wall_lean_front_a_in_walkleft empty_distraction_wall_lean_front_a_loop empty_distraction_wall_lean_front_a_out_walk','Leaning against a wall; NOT player lateral aiming/peeking')
sources={};missing=[]
for a,request in requests.items():
 candidates=[r for r in rows if a in r['aliases'] and r['status']=='native_pose_sampled_offline']
 if not candidates:missing.append(a);continue
 for role in ('corvo','emily'):
  r=min(candidates,key=lambda r:(r['source_role']!=role,'/high/' not in r['asset'],r['asset']))
  s=sources.setdefault(r['id'],dict(alias=a,**request,targets=[]))
  if role not in s['targets']:s['targets'].append(role)
result=dict(scope='Native actor interaction/death donors; not all contextual variants or final game bindings',sources=sources,missing_requested_aliases=missing)
(P/'interaction-source-selection.json').write_text(json.dumps(result,indent=2))
print('INTERACTION_SOURCES',len(sources),'target clips',sum(len(r['targets'])for r in sources.values()),'missing',missing)
