from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'build_balanced_crouch_powers_v2.py').read_text()
s=s.replace('balanced-crouch-powers-v2','balanced-crouch-powers-v3').replace('balanced-crouch-power-banks-v2','balanced-crouch-power-banks-v3').replace('balanced-crouch-power-source-manifest-v2','balanced-crouch-power-source-manifest-v3').replace('balanced-crouch-power-bank-manifest-v2','balanced-crouch-power-bank-manifest-v3')
a=s.index('     shift=p[2].translation-source[2].translation')
b=s.index('    pin_feet(rig,p,targets,rotations);',a)
s=s[:a]+'''     # Preserve changing native upper-arm/forearm/hand directions in actor
     # space. Reparenting these orientations to the moving crouched shoulders
     # preserves every bone length without unreachable positional targets.
     for j in (7,8,9,14,15,16):
      g=rig.fk(p);rotation=g[j].to_quaternion().slerp(sourceG[j].to_quaternion(),weight).to_matrix();rig.orient(p,j,rotation)
'''+s[b:]
(P/'build_balanced_crouch_powers_v3.py').write_text(s)
