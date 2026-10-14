from pathlib import Path
root=Path(__file__).resolve().parents[2]
path=root/'work/natural-moveset/Test-DominoWorld-v215.ps1'
s=path.read_text(encoding='utf-8-sig')
old=" if(!$grounded.motionValid-or$grounded.motion-ne0-or[Math]::Abs([double]$grounded.position[2]-66.4370575)-gt.03){throw 'Source aim placement has not returned to proven native floor'}"
new=""" $floorSamples=@($grounded)
 foreach($n in 1..2){Start-Sleep -Milliseconds 100;$floorSamples+=Q client '--player-states'}
 $floorSamples|ConvertTo-Json -Depth 8|Set-Content "$out/source-floor-stability.json"
 foreach($sample in $floorSamples){
  if(!$sample.motionValid-or$sample.motion-ne0-or$sample.stance-ne0-or[Math]::Abs([double]$sample.position[2]-66.4036484)-gt.03){throw 'Source has not settled on the observed v215 floor at the closer aim position'}
  for($axis=0;$axis-lt3;$axis++){if([Math]::Abs([double]$sample.position[$axis]-[double]$grounded.position[$axis])-gt.002){throw 'Source position changed during pre-stream floor stability check'}}
 }
"""
assert s.count(old)==1
s=s.replace(old,new).replace('Unchanged proven source floor; only yaw points toward read-only verified native guard position','Closer v214 source position; v215 measured floor and three stable native motion samples required before streaming')
(root/'work/natural-moveset/Test-DominoWorld-v216.ps1').write_text(s,encoding='utf-8')
runner=root/'work/movement-integration/Invoke-NativeLabOperation.ps1'
text=runner.read_text(encoding='utf-8-sig')
assert 'Test-DominoWorld-v215.ps1' in text
runner.write_text(text.replace('Test-DominoWorld-v215.ps1','Test-DominoWorld-v216.ps1'),encoding='utf-8')
