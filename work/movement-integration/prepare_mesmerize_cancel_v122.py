"""Derive a cancellation experiment from the established isolated lifecycle harness."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeLinks-v106.ps1').read_text(encoding='utf-8-sig')
start=s.index(' $w.Restart();$cast=')
end=s.index('}catch{$failure=')
prefix=s[:start].replace("'-t','34'","'-t','12'").replace("'--watch-mesmerize-targets','28000'","'--watch-mesmerize-targets','10000'").replace("'--watch-power-scenes','15000'","'--watch-power-scenes','10000'")
body=r''' $w.Restart();$aimed=$false;$cancelled=$false
 while($w.ElapsedMilliseconds-lt10000){
  if(!$aimed-and$w.ElapsedMilliseconds-ge1000){I client 255 6000;$aimed=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT hold begins; no release-to-cast'})}
  if(!$cancelled-and$w.ElapsedMilliseconds-ge3200){
   $aim=Q client '--player-mesmerize';$aim|ConvertTo-Json -Depth 8|Set-Content "$out/native-before-cancel.json"
   & "$lab/Send-D2Input.ps1" -Role client -Button X -LeftTrigger 255 -DurationMs 250|Out-Null
   Start-Sleep -Milliseconds 300;I client;$cancelled=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X cancel while LT held, followed by neutral'})
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;nativeWorld=(Q client '--mesmerize-world');world=(S host '.mesmerize-world-stream.json');visual=(S host '.mesmerize-visual.json');eyes=(S host '.mesmerize-eyes.json');links=(S host '.mesmerize-links.json')})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw 'Native movement or transport retired during cancellation'}
  Start-Sleep -Milliseconds 25
 }
 I client
 $manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($fastScenes|Where-Object {$_.valid-and$_.variant-in16,17,18})
 foreach($variant in 16,17){
  if(!@($scenes|Where-Object variant -eq $variant).Count-or!@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}).Count){throw "Native and remote aim phase $variant not both observed"}
 }
 $scenes|Select-Object variant,asset,handle,armSide,character,armHandle -Unique|ConvertTo-Json -Depth 5|Set-Content "$out/phase-incarnations.json"
 if(@($scenes|Where-Object {$_.armSide-ne1-or$_.character-ne2-or!$_.armHandle}).Count){throw 'Emily source scene ownership changed'}
 $checks.Add('Native Emily enter and aim scenes drive corresponding complete-body remote phases through Steam')
 if(!$aimed-or!$cancelled-or$m.powerWeight-gt.02-or$m.weight-lt.9){throw 'Cancelled aim did not recover the standing movement bank'}
 if(@($rows|Where-Object {$_.ms-gt6500-and$_.movement.powerWeight-gt.02}).Count){throw 'Cancelled power animation returned after recovery'}
 $checks.Add('Native X while LT remains held cancels the aim; remote body returns to its normal movement bank without stale power weight')
 if([Math]::Abs($manaAfter.currentCandidate-$manaBefore.currentCandidate)-gt.05){throw 'Aim cancellation changed native mana'}
 $checks.Add('Cancelled aim consumes no source mana; no resource writes are used')
 $visual=S host '.mesmerize-visual.json'
 if($visual.created-ne$visualBefore.created-or$visual.fxStarted-ne$visualBefore.fxStarted-or$visual.active-or$visual.fxActive){throw 'Cancelled aim created a cosmetic world or effect'}
 if(@($rows|Where-Object {$_.nativeWorld.present-or$_.world.leased-or$_.visual.active}).Count){throw 'Cancelled aim committed a native or remote world'}
 foreach($r in 'host','client'){
  $fx=Q $r '--mesmerize-effects';$fx|ConvertTo-Json -Depth 8|Set-Content "$out/effects-after-$r.json"
  if(@($fx.effects).Count){throw 'Native effect remains after cancellation'}
  $h=Q $r '--health' (S $r).probeActorHandle;$h|ConvertTo-Json -Depth 6|Set-Content "$out/avatar-health-after-$r.json"
  if($h.current-ne40-or(Q $r '--health' 1).current-ne56){throw 'Player or avatar health changed'}
 }
 $checks.Add('Cancelled aim creates neither source-native nor cosmetic world, target links or particles; native players and avatars retain health')
 if(!(Q host '--avatar-sword' (S host).probeActorHandle).equipped-or!(Q client '--player-sword').equipped){throw 'Sword equipment changed'}
 $checks.Add('Native and remote sword selections remain equipped after cancellation')
 C client 'coop_mesmerize_fixture off';$w.Restart()
 do{$f=S client '.mesmerize-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Mesmerize inventory did not retire'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
 }
 $checks.Add('Both native inventory quantities restore exactly before checkpoint cleanup')
'''
tail=s[end:]
a=tail.index("scope='")+len("scope='");b=tail.index("'}|ConvertTo-Json",a)
tail=tail[:a]+"Standing armed Emily Mesmerize native aim cancellation via controller X while LT remains held. Confirms native first-person enter/aim provenance, remote body recovery and absence of a committed native/cosmetic power or mana cost. Visual acceptance, other stances, natural lifetime and shared NPC outcomes remain separate. Both isolated checkpoints restore and protected originals remain unchanged."+tail[b:]
(P/'Test-MesmerizeCancel-v122.ps1').write_text(prefix+body+tail,encoding='utf-8')
