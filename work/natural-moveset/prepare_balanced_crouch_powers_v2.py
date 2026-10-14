from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'build_balanced_crouch_powers_v1.py').read_text()
s=s.replace('balanced-crouch-powers-v1','balanced-crouch-powers-v2').replace('balanced-crouch-power-banks-v1','balanced-crouch-power-banks-v2').replace('balanced-crouch-power-source-manifest-v1','balanced-crouch-power-source-manifest-v2').replace('balanced-crouch-power-bank-manifest-v1','balanced-crouch-power-bank-manifest-v2')
s=s.replace('errors=[];heights=[];poses=[]','errors=[];heights=[];poses=[];arm_clamps=[]')
needle='    pin_feet(rig,p,targets,rotations);g=rig.fk(p);'
assert needle in s
s=s.replace(needle,'''    if wind:
     # Follow the moving authored arm trajectories in actor space. A local
     # shoulder offset inherited the crouched spine bend twice and pointed
     # the cast at the floor. These are animated donor targets, not fixed
     # first-person wrists; shoulder, torso and pelvis still move together.
     def ease(x):
      x=max(0.,min(1.,x));return x*x*(3-2*x)
     weight=ease(t/.12)*(1-ease((t-.78)/.22))
     shift=p[2].translation-source[2].translation
     for shoulder,elbow,wrist in ((7,8,9),(14,15,16)):
      g=rig.fk(p);goal=restG[wrist].translation.lerp(sourceG[wrist].translation+shift,weight)
      pole=restG[elbow].translation.lerp(sourceG[elbow].translation+shift,weight)
      delta=goal-g[shoulder].translation;reach=(rig.bind[elbow].translation.length+rig.bind[wrist].translation.length)*.985
      excess=max(0.,delta.length-reach);arm_clamps.append(excess)
      if excess:goal=g[shoulder].translation+delta.normalized()*reach
      rig.solve(p,shoulder,elbow,wrist,goal,pole)
      rotation=restG[wrist].to_quaternion().slerp(sourceG[wrist].to_quaternion(),weight).to_matrix();rig.orient(p,wrist,rotation)
    pin_feet(rig,p,targets,rotations);g=rig.fk(p);''')
s=s.replace('minimum_pelvis_height=min(heights))','minimum_pelvis_height=min(heights),maximum_arm_target_clamp=max(arm_clamps,default=0))')
s=s.replace("'Whole-body crouched cast adapted around balanced Posture-100 idle; standing lunge compression and footprint reduced for crouched balance. Native source owns root, clock, damage and effects.'", "'Whole-body crouched cast with animated body-space arm trajectories, balanced Posture-100 stance, and reduced lunge compression/footprint. Native source owns root, clock, damage and effects; no fixed first-person wrist targets.'")
(P/'build_balanced_crouch_powers_v2.py').write_text(s)
