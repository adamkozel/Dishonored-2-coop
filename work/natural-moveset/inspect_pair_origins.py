import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses
L=Library(['library-retarget-manifest.json','pair-victim-retarget-manifest.json'],'pair-research')
report=[]
for attacker in ['corvo','emily']:
 for direction in ['front','back']:
  for victim in ['corvo','emily']:
   am,ap=L.load(attacker,'sword_doppelganger_assassination_fast'+direction);vm,vp=L.load(victim,f'generic_{attacker}_doppelganger_assassination_fast{direction}')
   raw=poses(P/vm['source']['sample_file']);rig=rig_for(victim)
   def compact(p):return {str(j):[round(v,3)for v in rig.fk(p)[j].translation]for j in [2,9,12,16,21,25]}
   report.append(dict(attacker=attacker,victim=victim,direction=direction,attacker_duration=am['duration'],victim_duration=vm['duration'],attacker_root=am['root_track'][0],victim_root=vm['root_track'][0],victim_anchor=raw['frames'][0][1],attacker_positions=compact(ap[0])if attacker==victim else None,victim_positions=compact(vp[0])))
(P/'pair-origin-research.json').write_text(json.dumps(report,indent=2))
for r in report:print(json.dumps(r))
