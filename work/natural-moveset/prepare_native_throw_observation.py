"""Use ordinary input and native observations before adding throw semantics."""
from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'Observe-NativeCarry.ps1').read_text()
t=t.replace("$duration=if($Acquire){24}else{19}","$duration=14")
a=t.index('$schedule=@(');b=t.index('try {',a)
t=t[:a]+"""# The caller prepares a fresh isolated fixture and looks at the floor body.
$schedule=@(@{ms=700;button='X';y=0;duration=900},@{ms=7000;button='Neutral';y=0;duration=500;rt=255})
$leveled=$false
"""+t[b:]
old="  $states=Q '--player-states';$midnight=Q '--midnight'"
new="""  if(!$leveled-and$watch.ElapsedMilliseconds-ge4000){
   $held=Q '--player-states';if(!$held.carrySceneValid-or$held.carryScene.variant-notin @(2,3)){throw 'Native body acquisition did not reach loaded state'}
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Evidence "$out/throw-camera.json"|Out-Null
   $leveled=$true
  }
"""+old
assert old in t;t=t.replace(old,new)
(P/'Observe-NativeBodyThrow-v1.ps1').write_text(t)
print('NATIVE_THROW_OBSERVATION_READY')
