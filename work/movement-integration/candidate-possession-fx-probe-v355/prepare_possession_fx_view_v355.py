from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
s=(P/'Test-PossessionTargetFxView-v354.ps1').read_text()
s=s.replace('@(@(0.0,-1.5),@(-1.5,0.0),@(1.5,0.0),@(0.0,1.5),@(0.0,-3.0),@(-3.0,0.0),@(3.0,0.0))','@(@(-1.5,0.0),@(1.5,0.0),@(0.0,1.5),@(0.0,-1.5),@(-2.0,-2.0),@(2.0,-2.0),@(-2.0,2.0),@(2.0,2.0))')
a="  if($observerHealth.current-ne56){throw 'Observer floor test changed health'}"
assert a in s
s=s.replace(a,a+'''
  $ax=[double]$target.position[0]-[double]$observerGround.position[0];$ay=[double]$target.position[1]-[double]$observerGround.position[1]
  $bx=$sourceX-[double]$observerGround.position[0];$by=$sourceY-[double]$observerGround.position[1]
  $den=[Math]::Sqrt(($ax*$ax+$ay*$ay)*($bx*$bx+$by*$by))
  if($den-lt.1-or($ax*$bx+$ay*$by)/$den-gt.9396926){continue} # at least20 degrees between source body and target
''')
s=s.replace('$fxMonitor=$null;', '$fxMonitor=$null;$peerFxMonitor=$null;')
a=' $w.Restart();$cast=$false;'
assert a in s
s=s.replace(a,''' $peerFxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-possession-effects','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-effects.ndjson" -RedirectStandardError "$out/peer-effects.stderr" -PassThru
'''+a)
s=s.replace('$targetMonitor,$fxMonitor)', '$targetMonitor,$fxMonitor,$peerFxMonitor)')
a=" $checks.Add('Exact native human target FX travels through Steam; one native-derived no-rumble particle effect starts on owned Corvo presentation, updates and retires without NPC gameplay invocation')"
assert a in s
s=s.replace(a,a+'''
 $peerFxMonitor.WaitForExit(4000)|Out-Null;if(!$peerFxMonitor.HasExited-or$peerFxMonitor.ExitCode){throw 'Independent peer effect inventory incomplete'}
 $peerInventory=@(Get-Content "$out/peer-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $peerEffects=@($peerInventory|Where-Object valid|ForEach-Object effects|Where-Object name -eq 'models/effects/d2coop/possession_target_remote.fx')
 [ordered]@{samples=$peerInventory.Count;observed=$peerEffects.Count;ids=@($peerEffects.id|Sort-Object -Unique);owners=@($peerEffects.owner|Sort-Object -Unique);origins=@($peerEffects.origin|Select-Object -First 9)}|ConvertTo-Json -Depth 6|Set-Content "$out/independent-peer-fx.json"
 if($peerEffects.Count-lt3-or@($peerEffects|Where-Object {$_.id-notin$activeFx.peerFx.nativeId-or$_.owner-notin$activeFx.peerFx.nativeOwner}).Count){throw 'Peer native target-effect inventory does not match owned stream lifecycle'}
 $checks.Add('Independent external peer FX inventory confirms exact declaration, receiver effect ID and clone owner')
''')
(P/'Test-PossessionTargetFxView-v355.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionTargetFxView-v354.ps1')==1;s=s.replace('Test-PossessionTargetFxView-v354.ps1','Test-PossessionTargetFxView-v355.ps1');p.write_text(s)
p=S/'runtime_probe.cpp';s=p.read_text();a='return name.rfind("models/effects/gameplay/powers/possession/",0)==0;';assert s.count(a)==1;s=s.replace(a,'return name.rfind("models/effects/gameplay/powers/possession/",0)==0||name=="models/effects/d2coop/possession_target_remote.fx";');p.write_text(s)
print('355 side view and independent read-only receiver FX inventory; DLL351 unchanged')
