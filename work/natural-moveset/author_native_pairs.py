"""Bind native attacker/victim references into event-free offline choreography.

The victim anchor is treated as a candidate partner frame, checked visually.
It is not an anatomical parent and is never copied into the gameplay pose.
"""
import sys,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses as raw_poses
L=Library(['library-retarget-manifest.json','pair-victim-retarget-manifest.json'],'paired-native-v1')
for role in ['corvo','emily']:
 rig=rig_for(role)
 for direction in ['front','back']:
  am,ap=L.load(role,'sword_doppelganger_assassination_fast'+direction)
  for victim in ['corvo','emily']:
   vm,vp=L.load(victim,f'generic_{role}_doppelganger_assassination_fast{direction}')
   raw=raw_poses(P/vm['source']['sample_file']);anchor=qts(raw['frames'][0][1]);placement=anchor.inverted()
   # Keep the authored native partner distance. Scaling the victim's body
   # does not imply scaling the distance between two independent actors.
   count=round(am['duration']*60)+1;frames=[];roots=[]
   for f in range(count):
    u=f/(count-1);frames.append(sample(ap,u));roots.append(row(sample([[qts(v)]for v in am['root_track']],u)[0]))
   scene=next((P.parent.parent/'outputs/d2-coop-lab/research/asset-exports/natural-pair-scenes').rglob('*small_assassination_'+direction+'..midnightscene.decl'))
   alias='paired_fast_'+direction+'_'+('tall'if victim=='corvo'else'female')
   partner=dict(role=victim,id=vm['id'],binary=vm['binary'],sha256=vm['sha256'],frames=vm['frames'],fps=vm['fps'],duration=vm['duration'],root_track=vm['root_track'],placement=row(placement),source=vm['source']['asset'],appearance='Neutral-grey native-rig stand-in, not the co-op partner',clock='Original native seconds; hold final victim pose after its clip ends',placement_basis='Hypothesis: inverse initial victim auxiliary anchor represents initial partner-to-victim transform; visual contact review required')
   L.save(rig,role,alias,frames,roots,'paired_actions','Native front/back Doppelganger paired study with the matching victim response, original timing and neutral-grey target. No damage, ragdoll or game-state events. Partner placement/contact and size adaptation require visual and runtime validation.',dict(actor_sha256=am['sha256'],victim_sha256=vm['sha256'],scene=str(scene.relative_to(P.parent.parent)),scene_sha256=hashlib.sha256(scene.read_bytes()).hexdigest()),partner=partner)
L.finish('paired-native-manifest-v1.json')
