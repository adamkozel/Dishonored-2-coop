from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeChoke-v574.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a[:80],s.count(a));s=s.replace(a,b)
rep("$capture=$null;$monitor=$null","$capture=$null;$monitor=$null;$peerCapture=$null")
rep('candidate-trap-network-v575','candidate-choke-network-v581');rep('Expected preserved575 candidate','Expected archived581 candidate')
rep("'NativeChokeObserve574'","'ChokeNetwork583'") if s.count("'NativeChokeObserve574'")==1 else None
s=s.replace('NativeChokeObserve574','ChokeNetwork583')
rep("$key=\"$source-$($case.name)\";", '''$character=if($source-eq'host'){'corvo'}else{'emily'}
   $entry=Get-Content "$PSScriptRoot/choke-bank-manifest-v581.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
   $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
   Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force;C $peer 'coop_movement_bank on'
   $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq63){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
   if(!$m.active-or$m.version-ne63){throw 'Choke bank preload failed'}
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
   $key="$source-$($case.name)";''')
rep("   $monitor=Start-Process", '''   $peerHwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();$peerArgs=$args.Clone();$graphIndex=[Array]::FindIndex($peerArgs,[Predicate[object]]{param($x) "$x" -like "gfxcapture=*"});if($graphIndex-lt0){throw "Capture graph absent"};$peerArgs[$graphIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[$peerArgs.Length-1]="$out/observer.mp4"
   $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/observer-capture.log" -RedirectStandardOutput "$out/observer-capture.stdout" -PassThru
   $monitor=Start-Process''')
rep("if(!$sent-and$watch.ElapsedMilliseconds-ge1100){I $source RightShoulder 6500;$sent=$true}",'''if(!$sent-and$watch.ElapsedMilliseconds-ge1100){
     # The client guard can still finish native walking animation while AI
     # decisions are held. Refresh the source fixture position immediately
     # before ordinary input; never move or force the guard itself.
     $fresh=@((Q $source '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq$target.definition})
     if($fresh.Count-ne1){throw 'Guard identity changed'};$g=$fresh[0];$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1]
     $px=[double]$g.position[0]-.9*$fx;$py=[double]$g.position[1]-.9*$fy;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
     C $source ('setviewpos {0} {1} 68.1 {2} 0' -f $px.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$py.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$look.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
     $ox=$px-2*$fx-2*$fy;$oy=$py-2*$fy+2*$fx;$oyaw=[Math]::Atan2($py-$oy,$px-$ox)*180/[Math]::PI
     C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f $ox.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oy.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oyaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
     & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json" -Quick
     Start-Sleep -Milliseconds 90;I $source RightShoulder 6500;$sent=$true
    }''')
rep("midnight=(Q $source '--midnight')",'''midnight=(Q $source '--midnight');movement=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json);stream=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json);sourceStream=(Get-Content "$root/work/lab-instances/$source/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)''')
rep("   $checks.Add('Native RightShoulder", "   $checks.Add('Native RightShoulder") if False else None
rep("   $paired|ConvertTo-Json -Depth 8|Set-Content \"$out/choke-scenes.json\"",'''   $paired|ConvertTo-Json -Depth 8|Set-Content "$out/choke-scenes.json"
   foreach($v in 1,2,3){
    if(!@($samples|Where-Object {$_.player.chokeSceneValid-and$_.player.chokeScene.variant-eq$v}).Count){throw "Owned native choke phase$v absent"};$checks.Add("Owned native choke phase$v read independently")
    if(!@($samples|Where-Object {$_.movement.chokeVariant-eq$v-and$_.movement.chokeWeight-gt.8-and$_.movement.weight-gt.85}).Count){throw "Remote choke phase$v absent"};$checks.Add("Steam remote whole-body choke phase$v observed")
   }
   if(!@($samples|Where-Object {$_.ms-gt9000-and$_.movement.chokeWeight-lt.05-and$_.movement.weight-gt.85}).Count){throw 'Standing recovery absent'};$checks.Add('Native standing recovery restores existing locomotion')
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}''')
rep("foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}", "foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}")
rep("  if($monitor){", "  if($peerCapture){$peerCapture.WaitForExit(15000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; observer capture incomplete').TrimStart('; ')}}\n  if($monitor){")
rep('No remote paired body or shared NPC outcome is claimed.', 'Source-timed remote actor body through native Steam. Remote victim animation/alignment and shared knockout outcomes are not implemented by this binding.')
rep('PASS_NATIVE_CHOKE_OBSERVATION','PASS_CHOKE_NETWORK')
(N/'Test-ChokeNetwork-v583.ps1').write_text(s)
print('Choke network harness583 prepared; no test performed by this generator.')
