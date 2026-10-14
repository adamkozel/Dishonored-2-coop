from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'
p=N/'Test-NpcRangedProfile-v970.ps1'; assert not p.exists()
s=(N/'Test-NpcRangedProfile-v967.ps1').read_text(encoding='utf-8-sig')
anchor=' Start-Sleep -Milliseconds 350\n $peer='
assert s.count(anchor)==1
s=s.replace(anchor,''' Start-Sleep -Milliseconds 350
 # Native teleport pitch is not accepted as proof of actual view aim.
 # Use the existing controller-feedback helper, then record the measured ray.
 Save "$out/camera-before-aim.json" (Q $Source '--player')
 & "$PSScriptRoot/Set-NativeInteractionAim-v663.ps1" -Role $Source -X $t[0] -Y $t[1] -Z ($t[2]+1.05) -Evidence "$out/measured-body-aim.json"
 Save "$out/camera-after-aim.json" (Q $Source '--player')
 $peer=''',1)
anchor='  $old=S host \'.npc-combat.json\';$previous=$old.committed\n'
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''  $view=Q $Source '--player';Save "$out/camera-before-shot-$attempt.json" $view
  $eye=$view.nativeViewPosition;$forward=$view.nativeViewOrientation
  $delta=@(($t[0]-$eye[0]),($t[1]-$eye[1]),($t[2]+1.05-$eye[2]));$length=[Math]::Sqrt($delta[0]*$delta[0]+$delta[1]*$delta[1]+$delta[2]*$delta[2])
  $dot=($delta[0]*$forward[0]+$delta[1]*$forward[1]+$delta[2]*$forward[2])/$length
  if($dot-lt[Math]::Cos(1.5*[Math]::PI/180)){throw 'Actual native camera no longer points at measured body target'}
''',1)
p.write_text(s,encoding='utf-8')
print(p)
