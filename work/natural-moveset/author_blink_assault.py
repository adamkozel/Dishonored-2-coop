"""Whole-body Blink Assault acting; no teleport, damage or target events.

The exact-game attribute is confirmed. This joins free caster acting with
native full-body kick/slash acting, without first-person wrist constraints.
Emily is a comparison adaptation, not a claim about base-campaign powers.
"""
import sys
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['power-retarget-manifest-v1.json','library-retarget-manifest.json'],'blink-assault-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);im,idle=L.load(role,'sword_idle')
 for action in ['kick','slash']:
  ps=[];roots=[];marks=[];sources=[];origin=Matrix.Identity(4)
  for alias,duration in [('power_blink_fire_in',None),('power_blink_fire_loop',.55),('sword_attack_kick'if action=='kick'else'sword_attack_fasta',None),('sword_idle',.5)]:
   m,frames=L.load(role,alias);d=m['duration']if duration is None else duration;count=round(d*60)+1;prior=copy_pose(ps[-1])if ps else None;start=len(ps)/60;marks.append(dict(phase=alias,at=start));sources.append(dict(alias=alias,sha256=m['sha256']))
   rt=[[qts(r)]for r in m['root_track']];r0=rt[0][0];loop=alias.endswith(('loop','idle'));base=origin.copy()
   for f in range(count):
    t=f/60;u=t/m['duration'];p=sample(frames,u,loop)
    if prior is not None:p=blend(prior,p,ease(t/.15))
    # Only source body-root displacement is retained as demonstration
    # metadata. No invented teleport distance is baked into the actor.
    delta=r0.inverted()@sample(rt,min(1,u))[0]
    if loop:delta=Matrix.Identity(4)
    preview_helpers(rig,p);ps.append(p);roots.append(row(base@delta))
   origin=qts(roots[-1])
  L.save(rig,role,'blink_assault_'+action+'_sequence',ps,roots,'powers','Blink Assault '+action+' acting: anticipation and held aim join native full-body '+action+' and standing recovery. Wrists remain body-driven. Native blink_assault inventory attribute is confirmed, but this sequence does not demonstrate teleport timing, target contact, damage, upgraded knockdown or game-state binding. Emily is a comparison adaptation; base campaign character availability is not implied.',dict(clips=sources,attribute='components/characters/player/base/attributes/blink_assault.inventoryitem'),phase_marks=marks)
L.finish('blink-assault-manifest-v1.json')
