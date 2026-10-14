"""Create separate high-throw asset installer from the established safe recipe.

No original resource is replaced. The native victim's animation/look-at tracks
are retained; event tracks are removed so the owned fixture can release once.
"""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'build_carry_release_scenes.py').read_text()
source=source.replace("[('drop_low_stand',4,656),('drop_low_sneak',6,500)]", "[('drop_high',8,280)]")
source=source.replace("carry-release-scenes-v1.json", "carry-high-throw-scenes-v31.json")
source=source.replace("s=s.replace('carry-victim','carry-release')", "s=s.replace('carry-victim','carry-high-throw')")
source=source.replace("Build-D2CarryReleaseAssets.ps1", "Build-D2CarryHighThrowAssets.ps1")
exec(compile(source,str(P/'build_high_throw_experiment_v31.py'),'exec'))
