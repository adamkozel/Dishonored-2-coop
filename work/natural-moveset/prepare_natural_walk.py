"""Build a separate reproducible authoring script from the preserved walking pipeline.

Only source motion drives the whole shoulder/arm chain. The rigid sword binding
stays local to the native wrist; it must not constrain the wrist in world space.
"""
from pathlib import Path
P=Path(__file__).resolve().parent
src=(P.parent/'moveset-growth/retarget_walk_bank.py').read_text()
begin=src.index("bpy.ops.wm.open_mainfile")
end=src.index("mapping=",begin)
src=src[:begin]+"import sys\nsys.path.insert(0,str(P))\nfrom rig_math import read,qts,ease,NativeRig,header_floats\n"+src[end:]
src=src.replace("P/'donor-analysis.json'", "P.parent/'moveset-growth/donor-analysis.json'")
src=src.replace("P/'donors'", "P.parent/'moveset-growth/donors'")
src=src.replace(";BG=fk(B)", ";rig=NativeRig(J);fk=rig.fk;solve=rig.solve;orient=rig.orient;BG=fk(B)")
src=src.replace(" prefix='kMixamo'", " floats=lambda name:header_floats(gripText,name)\n prefix='kMixamo'")
begin=src.index('   pos,wr,blade=fpSmooth')
end=src.index('   for k,i in enumerate(fingerIds):',begin)
src=src[:begin]+'''   # The entire source shoulder/arm/wrist motion survives retargeting.
   # Native fingers and attachment follow the moving wrist without arm IK.
'''+src[end:]
src=src.replace("'weapon_reference':'Preserved D2 Corvo first-person ready blade, adjusted to this native rig and socket'", "'weapon_reference':'Native local grip/socket only; full-body source drives both arms', 'wrist_constraint':'none', 'source_first_person_constraints':False")
src=src.replace("Reversible two-rig, eight-direction walking experiment", "Natural two-rig full-body walking revision")
(P/'retarget_natural_walk.py').write_text(src)
preview=(P.parent/'moveset-growth/preview_walk_bank.py').read_text()
preview=preview.replace("else P)/f'{role}-{part}.json'", "else P.parent/'moveset-growth')/f'{role}-{part}.json'")
(P/'preview_natural_walk.py').write_text(preview)
print('Natural authoring pipeline prepared without first-person reference loading or wrist IK.')
