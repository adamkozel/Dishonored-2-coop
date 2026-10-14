from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(N/'Test-DoppelgangerNetwork-v408.ps1').read_text(encoding='utf-8-sig')
s=s.replace("-Role host -Pitch -3", "-Role host -Pitch 10")
s=s.replace('$w.Restart();$cast=$false', '$w.Restart();$cast=$false;$worldNext=0;$worldSamples=[Collections.Generic.List[object]]::new();$cloneCaptured=$false')
a="  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'"
assert s.count(a)==1
s=s.replace(a,'''  if($w.ElapsedMilliseconds-ge$worldNext){
   $worldNext=$w.ElapsedMilliseconds+500
   $se=Q client '--entities';$pe=Q host '--entities'
   $clones=@($se.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\\.def$'})
   $peerClones=@($pe.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\\.def$'})
   $obj=Q client '--object' $power.component
   $worldSamples.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;source=$clones;peer=$peerClones;equipment=$obj})
   if($clones.Count-eq1-and!$cloneCaptured){
    Q client '--components' $clones[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/native-clone-components.json"
    Q client '--animation' $clones[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/native-clone-animation.json"
    $cloneCaptured=$true
   }
  }
'''+a)
a=" if(!$nativeClones.Count-or$peerClones.Count){throw 'Expected native source clones only; missing source or duplicated peer doppelganger'}"
assert a in s
s=s.replace(a,''' $worldSamples|ConvertTo-Json -Depth 15|Set-Content "$out/clone-lifecycle-samples.json"
 if(!@($worldSamples|Where-Object {$_.source.Count-gt0}).Count-or@($worldSamples|Where-Object {$_.peer.Count-gt0}).Count-or$peerClones.Count){throw 'Native source clone never observed, or peer duplicated a clone'}''')
s=s.replace('Native source doppelganger remains source-owned; body playback creates no peer clones or duplicate power actions','Native source clone observed during its lifetime; body playback creates no peer clones or duplicate power actions')
# Preserve the sparse lifecycle samples even if a later assertion fails.
s=s.replace(' $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"',' $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"\n if($null-ne$worldSamples){$worldSamples|ConvertTo-Json -Depth 15|Set-Content "$out/clone-lifecycle-samples.json"}')
p=N/'Test-DoppelgangerNetwork-v409.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerNetwork-v408.ps1').read_text(encoding='utf-8-sig').replace('v408','v409')
(W/'Run-DoppelgangerNetwork-v409.ps1').write_text(s,encoding='utf-8')
print('409 records native clone during its lifetime rather than requiring persistence through recording end; improved complete-body camera')
