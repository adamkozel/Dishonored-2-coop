from pathlib import Path
p=Path('work/natural-moveset/build_mesmerize_bank_v73.py');s=p.read_text().replace('v73','v77').replace("from clip_io import *", "from clip_io import *\nfrom rig_math import ease")
s=s.replace('pose=sample(fs,f/119)', '''u=f/119;pose=sample(fs,u)
   # Reorient the native complete arm gesture toward the casting direction.
   # This is an authored shoulder rotation, not a first-person wrist target.
   # Native body acting, elbow bend, local bone translations and root stay intact.
   weight=ease(u) if phase=='in' else 1-ease(u) if phase=='out' else 1
   rig.orient(pose,7,Matrix.Rotation(math.radians(65*weight),3,'Y')@rig.fk(pose)[7].to_3x3())''')
s=s.replace('Native support full-body donor with first-person Mesmerize timing/finger inspiration.', 'Native support full-body donor, authored 65-degree forward shoulder gesture, and first-person Mesmerize timing/finger inspiration.')
s=s.replace("sources=sources,native_character='emily'", "sources=sources,shoulder_forward_degrees=65,native_character='emily'")
Path('work/natural-moveset/build_mesmerize_bank_v77.py').write_text(s)
