import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['choke-victim-retarget-manifest.json'],'pair-research');rig=rig_for('corvo')
for role in ['corvo','emily']:
 m,poses=L.load('corvo','generic_'+role+'_choke_loop')
 for f in [0,len(poses)//2,len(poses)-1]:
  g=rig.fk(poses[f]);print(role,f,json.dumps({j:[round(v,3)for v in g[j].translation]for j in [2,8,9,11,12,16,21,25]}),'root',m['root_track'][f])
L=Library(['choke-contact-manifest-v1.json','choke-contact-victims-v1.json'],'pair-research')
for role in ['corvo','emily']:
 rig=rig_for(role);m,ps=L.load(role,'paired_nonlethal_hold');vm,vs=L.load('corvo','victim_'+role+'_choke_hold');g=rig.fk(ps[0]);vg=rig_for('corvo').fk(vs[0]);neck=Vector((1,0,0))+vg[11].translation
 print('CONTACT',role,'neck',list(neck),'arms',json.dumps({j:[round(v,3)for v in g[j].translation]for j in [7,8,9,14,15,16]}),'lengths',[rig.bind[j].translation.length for j in [8,9,15,16]])
L=Library(['library-retarget-manifest.json'],'pair-research')
for role in ['corvo','emily']:
 rig=rig_for(role)
 for name in ['empty_idle','sword_doppelganger_assassination_fastback']:
  m,p=L.load(role,name);g=rig.fk(sample(p,.24));print('BODY',role,name,json.dumps({j:[round(v,3)for v in g[j].translation]for j in [2,7,12,14,21,25]}))
