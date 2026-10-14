"""Allow the stance blend to settle before taking final contact evidence."""
from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'Record-CarryCloseView-v28.ps1').read_text()
for old,new in [('$nextReleasePose=6200','$nextReleasePose=8200'),("if($Paired){'13'}","if($Paired){'15'}"),
 ('if($Paired){10500}','if($Paired){12500}'),('-ge6050','-ge8050'),('-ge6500','-ge8500'),('-lt9300','-lt11300')]:
 assert old in t,old;t=t.replace(old,new)
(P/'Record-CarrySettledStance-v29.ps1').write_text(t)
print('SETTLED_STANCE_HARNESS_READY')
