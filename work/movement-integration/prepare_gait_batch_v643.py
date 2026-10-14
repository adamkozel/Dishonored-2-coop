from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoor-v641.ps1').read_text()
s=s.replace("param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-unlocked-door-corvo-v641',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('finish','breakout')][string]$Outcome='breakout')", "param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-gait-batch-v643')")
s=s.replace('NativeDoor641','GaitBatch643')
a=s.index('function W(');b=s.index('try{\n try{$owned=',a);s=s[:a]+s[b:]
s=s.replace('foreach($source in @($Source))',"foreach($source in @('host','client'))")
s=s.replace("name='unlocked-door-tap';yaw=270;stance=0;distance=1.0", "name='gait-batch';yaw=0;stance=0;distance=0.0")
a=s.index("   C $peer 'setviewpos");b=s.index('   $character=',a)
s=s[:a]+'''   foreach($r in 'host','client'){C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
   C $peer 'setviewpos -28.5 -151.2 68.1 210 0'
   C $source 'setviewpos -34.3 -154.8 68.1 0 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/$source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 11 -Evidence "$out/$source-observer-camera.json" -Quick
'''+s[b:]
# Existing continuous mode refreshes its native bounded lease; arbitrary durations are not commands.
s=s.replace('   $key="$source-$($case.name)";', '''   $ready=[Diagnostics.Stopwatch]::StartNew();do{
    $m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json
    $t=Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
    if($m.active-and$m.weight-gt.95-and$t.ready){break};Start-Sleep -Milliseconds 40
   }while($ready.ElapsedMilliseconds-lt6000)
   if(!$m.active-or$m.weight-le.95-or!$t.ready){throw 'Native Steam/body not ready'}
   $key="$source-$($case.name)";''')
s=s.replace("'-t','20'", "'-t','30'")
s=s.replace('$out/observer.mp4','$out/$source-observer.mp4').replace('$out/observer-capture.', '$out/$source-observer-capture.')
a=s.index('   $monitor=Start-Process');b=s.index('   $filterBefore=',a);s=s[:a]+s[b:]
a=s.index('   $watch=[Diagnostics.Stopwatch]');b=s.index('   $capture.WaitForExit',a)
s=s[:a]+'''   # Timed ordinary controller inputs; every pulse expires without further input.
   # Same known clear-room course as the established gait sequence, with a moving turn and native holster/redraw.
   $events=@(
    @(0,0,0,0,100,'Neutral','armed-idle'),
    @(1200,0,14000,0,900,'Neutral','walk-forward'),
    @(2200,0,32767,0,650,'Neutral','accelerate-run'),
    @(2950,0,0,0,100,'Neutral','run-stop'),
    @(4200,0,14000,0,750,'Neutral','walk-before-reverse'),
    @(4950,0,-14000,0,850,'Neutral','reverse-back'),
    @(6000,0,0,0,100,'Neutral','walk-stop'),
    @(7200,-14000,14000,0,700,'Neutral','diagonal-left'),
    @(7900,14000,-14000,0,700,'Neutral','diagonal-reverse'),
    @(8800,0,0,0,100,'Neutral','diagonal-stop'),
    @(10000,0,14000,24000,800,'Neutral','moving-turn-right'),
    @(10900,0,-14000,-24000,800,'Neutral','moving-turn-back'),
    @(11900,0,0,0,100,'Neutral','turn-stop'),
    @(13500,0,0,0,1500,'X','native-holster'),
    @(17000,0,0,0,100,'Neutral','unarmed-idle'),
    @(18500,0,14000,0,650,'Neutral','unarmed-forward'),
    @(19300,0,-14000,0,650,'Neutral','unarmed-back'),
    @(20300,0,0,0,100,'Neutral','unarmed-stop'),
    @(21800,0,0,0,650,'X','native-redraw'),
    @(25000,0,0,0,100,'Neutral','armed-recovery'))
   $events|ConvertTo-Json -Depth 4|Set-Content "$out/$source-planned-inputs.json"
   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$inputs=[Collections.Generic.List[object]]::new();$next=0
   while($watch.ElapsedMilliseconds-lt28000){
    if($next-lt$events.Count-and$watch.ElapsedMilliseconds-ge$events[$next][0]){
     $e=$events[$next];& "$lab/Send-D2Input.ps1" -Role $source -LeftX $e[1] -LeftY $e[2] -RightX $e[3] -DurationMs $e[4] -Button $e[5]|Out-Null
     $inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;planned=$e[0];label=$e[6];x=$e[1];y=$e[2];turn=$e[3];duration=$e[4];button=$e[5]});$next++
    }
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;label=$events[[Math]::Max(0,$next-1)][6];player=(Q $source '--player-states');nativeView=(Q $source '--player');source=(S $source);peer=(S $peer);sword=(Q $source '--player-sword');avatarSword=(Q $peer '--avatar-sword' (S $peer).probeActorHandle);movement=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json);stream=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)})
    Start-Sleep -Milliseconds 30
   }
'''+s[b:]
a=s.index('   $allScenes=@(');b=s.index("   C $peer 'coop_movement_bank off'",a)
s=s[:a]+'''   $inputs|ConvertTo-Json -Depth 4|Set-Content "$out/$source-inputs.json"
   if($inputs.Count-ne$events.Count-or$samples.Count-lt100){throw 'Batched input/sampling incomplete'}
   if(($samples.movement.runMix|Measure-Object -Maximum).Maximum-lt.5){throw 'Native run blend not exercised'}
   $unarmed=@($samples|Where-Object {$_.ms-gt17500-and$_.ms-lt18200})
   if($unarmed.Count-lt2-or@($unarmed|Where-Object {$_.sword.equipped-or$_.avatarSword.equipped-or$_.movement.armedMix-gt.05}).Count){throw 'Native holster did not reach both source and remote'}
   $recovery=@($samples|Where-Object {$_.ms-gt26500})
   if($recovery.Count-lt2-or@($recovery|Where-Object {!$_.sword.equipped-or!$_.avatarSword.equipped-or$_.movement.armedMix-lt.95-or$_.movement.speed-gt.05}).Count){throw 'Armed stationary recovery incomplete'}
   $checks.Add("$source: walk/run, unarmed native idle and equipped stationary recovery present; detailed direction/root checks follow independent audit")
   $peerCapture.WaitForExit(6000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){throw 'Observer capture failed'};$peerCapture=$null
'''.replace('$source:','${source}:')+s[b:]
s=s.replace(';sensors=@($sensors.type)','')
s=s.replace("$checks.Add('Native source observation only; no forced door state, native scene, inventory grant or new binding claim.')", "$checks.Add('Both roles recorded in one isolated native operation with one checkpoint restoration; existing movement clips and native first person retained.')")
s=s.replace("scope='Source-only native unlocked door short-X observation with independently read source/peer Openable states. Both actual views, scene/state trace and before/after door entities retained. Successful script execution does not prove an open/close action or new remote binding. Restoration is established only by checks/failure.'", "scope='Batched native locomotion transitions on both roles using candidate635 and existing bank67: walk/run/stop, reverse, diagonal direction changes, moving turns, holstered idle/movement and armed recovery. Both native views and Steam/body telemetry retained. Detailed direction/root/phase checks and visual inspection recorded separately; no new animation authoring or final visual acceptance.'")
s=s.replace('PASS_NATIVE_DOOR_OBSERVATION','PASS_NATIVE_GAIT_BATCH')
p=N/'Test-GaitBatch-v643.ps1'
if p.exists(): raise SystemExit('Unique test already exists')
p.write_text(s)
print('643 prepared: both roles,20 input stages each, existing gait path, single checkpoint restoration.')
