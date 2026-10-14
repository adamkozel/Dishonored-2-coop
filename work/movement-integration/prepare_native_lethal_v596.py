"""Observe the next existing-pipeline action through ordinary native input."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-ChokeNetwork-v595.ps1').read_text()
s=s.replace('ChokeNetwork595','NativeLethal596').replace("name='rear-choke'","name='rear-lethal'")
s=s.replace("I $source RightShoulder $(if($Outcome-eq'finish'){6500}else{1200})","& \"$lab/Send-D2Input.ps1\" -Role $source -RightTrigger 255 -DurationMs 250|Out-Null")
start=s.index("   if(@($samples.player.active|Where-Object")
end=s.index("   $rows.Add",start)
s=s[:start]+'''   $allScenes=@(Get-Content "$out/scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|ForEach-Object {$_.instances})
   $paired=@($allScenes|Where-Object {$_.state-eq3-and$_.asset-match'(?i)assassin'})
   $paired|ConvertTo-Json -Depth 9|Set-Content "$out/lethal-scenes.json"
   $samples|ForEach-Object { [pscustomobject]@{ms=$_.ms;stance=$_.player.stance;mode=$_.player.mode;active=$_.player.active} }|ConvertTo-Json -Depth 9|Set-Content "$out/native-action-states.json"
   if($paired.Count-lt2){throw 'Ordinary native lethal scene not demonstrated'}
   $checks.Add('Ordinary RT behind native guard starts assassination scene; exact state/ownership still requires analysis')
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
''' + s[end:]
s=s.replace("Native ordinary RightShoulder behind exact authored guard2; selected finish/breakout outcome observed independently. Visual review remains required.","Native ordinary RT250ms behind exact authored guard2; source-only action research, not a remote lethal binding.")
a=s.index("scope='Native rear choke");b=s.index("'}|ConvertTo-Json -Depth 10",a)
s=s[:a]+"scope='Source-only native lethal takedown observation, ordinary RT250ms. Captures exact source phases/state/target and actual game video. No new remote lethal binding or shared victim outcome is claimed. All checkpoint inventory/health and14 protected files restored.'"+s[b+1:]
s=s.replace('PASS_CHOKE_NETWORK','PASS_NATIVE_LETHAL')
(N/'Observe-NativeLethal-v596.ps1').write_text(s)
print('Native lethal596 source-only harness prepared; no runtime implementation claim.')
