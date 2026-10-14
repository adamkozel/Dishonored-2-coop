"""Review-only top descent adapted from the contacted top climb.

The reversal is explicitly recorded as a source adaptation, not a dedicated
descent capture. Body travel remains offline metadata, with no root writes.
"""
import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['top-transitions-manifest-v1.json'],'ladder-top-entry-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);m,source=L.load(role,'ladder_top_exit_sequence')
 duration=5.6;ps=[];roots=[];tracks=[]
 for f in range(round(duration*60)+1):
  t=f/60;u=1-t/duration;p=sample(source,u)
  root=sample([[qts(r)]for r in m['root_track']],u)[0]
  preview_helpers(rig,p);ps.append(p);roots.append(row(root));tracks.append(row(root.inverted()))
 prop=dict(m['props'][0]);prop['attachment_track']=tracks;prop['visible']=[True]*len(ps)
 L.save(rig,role,'ladder_top_descent_entry_sequence',ps,roots,'traversal',
  'Standing-to-ladder descent entry adapted by reversing the contacted top-climb sequence at a slower 5.6-second pace. Both arms, trunk and feet move together. Neutral platform and rail contacts accompany the body. This is an explicit reversed-source candidate: downward weight transfer, fine palm/toe contact, native geometry and live transition binding need review.',
  dict(top_exit_sha256=m['sha256'],adaptation='time_reversed_with_slower_pace'),
  props=[prop],preview_placement=roots[0],platform_height_m=m['platform_height_m'],
  phase_marks=[dict(phase='standing_on_platform',at=0),dict(phase='turn_and_find_rails',at=.8),dict(phase='transfer_onto_ladder',at=2.5),dict(phase='descent_loop_entry',at=duration)])
 L.reports[-1]['equipment_policy']='hands_free'
 (P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
L.finish('ladder-top-entry-manifest-v1.json')
