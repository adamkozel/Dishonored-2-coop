"""Compare a candidate animated partner frame with existing native actors."""
import sys,json,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses
L=Library(['library-retarget-manifest.json','pair-victim-retarget-manifest.json','contextual-victim-retarget-manifest.json'],'pair-research');reports=[]
for role in ['corvo','emily']:
 for direction in ['front','back']:
  am,_=L.load(role,'sword_doppelganger_assassination_fast'+direction);vm,_=L.load('corvo',f'generic_{role}_doppelganger_assassination_fast{direction}');raw=poses(P/vm['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();rows=[]
  for t in [0,.25,.5,.75,1.]:
   i=round(t*(len(raw['frames'])-1));native=qts(raw['frames'][i][0]);anchor=qts(raw['frames'][i][1]);candidate=place@native@anchor;expected=sample([[qts(r)]for r in am['root_track']],min(1,t*vm['duration']/am['duration']))[0]
   rows.append(dict(phase=t,candidate=row(candidate),actor=row(expected),position_error=(candidate.translation-expected.translation).length,angle_error_deg=math.degrees(candidate.to_quaternion().rotation_difference(expected.to_quaternion()).angle)))
  reports.append(dict(role=role,direction=direction,rows=rows));print(role,direction,'max_errors',max(x['position_error']for x in rows),max(x['angle_error_deg']for x in rows))
(P/'pair-research/animated-partner-anchor.json').write_text(json.dumps(reports,indent=2))
