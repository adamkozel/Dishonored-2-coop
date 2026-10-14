"""A bounded pose-only D2 review reel of all ten power studies."""
import sys
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'power-showcase')
names=['windblast','doppelganger','mesmerize','bend_time','devouring_swarm','dark_vision','blink','possession','far_reach','domino']
for role in ['corvo','emily']:
 rig=rig_for(role);ps=[];marks=[];sources=[]
 for name in names:
  m,frames=L.load(role,'power_'+name+'_sequence')
  if ps:ps.extend(copy_pose(ps[-1])for _ in range(24))
  marks.append(dict(name=name,at=len(ps)/60,duration=m['duration']));sources.append(dict(alias=m['selection']['alias'],sha256=m['sha256']))
  ps.extend(frames)
 assert len(ps)<1801
 L.save(rig,role,'power_cast_showcase',ps,[identity()for _ in ps],'powers','Bounded pose-only reel of ten full-body power studies, separated by short neutral intervals. No effects, targets, movement or gameplay outcomes. Both models are shown for rig review; gameplay power availability remains character-specific.',dict(clips=sources),chapter_marks=marks)
L.finish('power-showcase-manifest.json')
