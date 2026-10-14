"""Native in-place jump during Mesmerize aim/release; observation only."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeMesmerizeCrouch-v130.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B')
b=s.index(' $hwnd=',a)
s=s[:a]+r''' $nativeState=Q client '--player-states';$nativeState|ConvertTo-Json -Depth 9|Set-Content "$out/state-before.json"
 if($nativeState.stance-ne0-or!$nativeState.defaultMode){throw 'Native standing context not established'}
 $checks.Add('Standing native source starts on the proven floor before the bounded jump')
'''+s[b:]
rep('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$jumped=$false')
at="  $scene=Q client '--midnight';$entities=Q client '--entities'"
rep(at,r'''  if(!$jumped-and$w.ElapsedMilliseconds-ge2300){
   & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs 600|Out-Null;$jumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native A while aiming, retaining LT600ms then releasing'})
  }
  $scene=Q client '--midnight'
''')
rep(";component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=@($entities.entities|Where-Object {$_.definitionName-match'mesmerize'})",';effects=(OptionalFx client)')
rep("if(@($rows.player|Where-Object {$_.stance-ne1-or!$_.defaultMode-or@($_.active|Where-Object {$_.id-notin6,63}).Count}).Count){throw 'Unexpected native state during crouched cast'}",r'''if(@($rows.player|Where-Object {$_.stance-ne0-or!$_.defaultMode-or@($_.active|Where-Object {$_.id-notin0,2,3,63}).Count}).Count){throw 'Unexpected native state during bounded jump observation'}
 $airRows=@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in2,3}).Count})
 $overlap=@($airRows|Where-Object {@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_mesmerize_cast_*'}).Count})
 $peak=($rows|ForEach-Object {[double]$_.player.position[2]}|Measure-Object -Maximum).Maximum
 if(!$jumped-or$peak-[double]$nativeState.position[2]-gt3-or[Math]::Abs([double]$rows[-1].player.position[2]-$nativeState.position[2])-gt.3){throw 'Native jump did not remain bounded and recover the same floor'}
 [ordered]@{airStateSamples=$airRows.Count;airAndRawSceneSamples=$overlap.Count;peakHeight=$peak;initialHeight=$nativeState.position[2];airRows=$airRows}|ConvertTo-Json -Depth 17|Set-Content "$out/air-context-observations.json"''')
rep("All three native crouched cast phases retain one reciprocal arm binding and observed Sneaking/UsePower state context", "Native cast phases retain reciprocal arm binding during bounded jump input; raw player/scene overlap is recorded rather than assumed")
rep('crouch-scene-observations.json','air-scene-observations.json')
a=s.index("scope='")+len("scope='");b=s.index("'}|ConvertTo-Json",a)
s=s[:a]+"Native source in-place jump while holding Mesmerize aim, releasing LT600ms after A. Raw player states and single-arm Midnight bindings determine actual airborne behavior. Current strict reader still refuses airborne Mesmerize, and no remote body/FX integration is enabled by this observation. Native mana/world behavior and final native floor are recorded. Both isolated checkpoints and protected save hashes restore."+s[b:]
(P/'Observe-NativeMesmerizeAir-v150.ps1').write_text(s,encoding='utf-8')
