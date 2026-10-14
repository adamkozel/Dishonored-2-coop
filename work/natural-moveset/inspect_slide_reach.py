import sys
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
version=sys.argv[-1] if sys.argv[-1].startswith('v') else 'v4'
L=Library(['slide-pair-manifest-'+version+'.json'],'pair-research')
for role in ['corvo','emily']:
 m,ps=L.load(role,'sliding_nonlethal_back_study');v=m['partner'];T=Library(['slide-pair-victims-'+version+'.json'],'pair-research');vm,vs=T.load('corvo','victim_'+role+'_sliding_nonlethal_back_study');rig=rig_for(role);vrig=rig_for('corvo');worst=(0,None)
 for f,p in enumerate(ps):
  vt=f/60-m['phase_marks'][1]['at'];duration=m['duration']-m['phase_marks'][1]['at']-1.2;u=vt/duration;contact=ease((vt-.16)/.25)*(1-ease((u-.7)/.24))
  if contact<.999:continue
  origin=qts(m['preview_placement'])@qts(m['root_track'][f]);world=qts(v['root_track'][f]);g=rig.fk(p);vg=vrig.fk(vs[f]);neck=(world@vg[11]).translation;chest=(world@vg[5]).translation;goals=[(neck+Vector((-.10,.10,.08)) if role=='corvo' else chest+Vector((-.12,.12,.06)))-origin.translation,neck+Vector((-.12,-.08,.04))-origin.translation]
  for j,a,goal in zip([9,16],[7,14],goals):
   e=(g[j].translation-goal).length
   if e>worst[0]:worst=(e,dict(frame=f,vt=vt,joint=j,origin=list(origin.translation),goal=list(goal),shoulder=list(g[a].translation),wrist=list(g[j].translation),pelvis=list(g[2].translation)))
 print(role,worst)
