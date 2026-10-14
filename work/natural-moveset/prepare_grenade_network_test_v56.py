from pathlib import Path
P=Path(__file__).resolve().parent;s=(P/'Observe-NativeGrenadeActions-v54.ps1').read_text(encoding='utf-8-sig')
def sub(a,b):
 global s
 assert s.count(a)==1,(a[:80],s.count(a));s=s.replace(a,b)
sub("$failure=$null;$capture=$null;$monitor=$null;","$failure=$null;$capture=$null;$sourceCapture=$null;$monitor=$null;")
sub("$pids=@{};","$peer=if($Source-eq'host'){'client'}else{'host'};$pids=@{};")
sub("'coop_grenade_fixture off','coop_movement_fixture off'","'coop_grenade_fixture off','coop_grenade_prop off','coop_review_clip off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off'")
sub(" C $Source 'setviewpos -33 -153 68.1 180 0'", " C $Source 'setviewpos -34.3 -154.8 68.1 180 0';C $peer 'setviewpos -31.7 -152.9 68.1 216 0'\n & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role $peer -Pitch 15 -Evidence \"$out/observer-camera.json\"")
mark=" $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();"
sub(mark,''' $character=if($Source-eq'host'){'corvo'}else{'emily'}
 $entry=Get-Content "$PSScriptRoot/grenade-bank-manifest-v56.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
 $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank identity differs'}
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $peer 'coop_movement_bank on'
 $w.Restart();do{$m=S $peer '.movement.json';$prop=S $peer '.grenade-prop.json';if($m.active-and$m.version-eq42-and$m.grenadeWeight-gt.9-and$prop.active-and$prop.network){break};Start-Sleep -Milliseconds 45}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne42-or$m.grenadeWeight-lt.9-or!$prop.active-or!$prop.network){[ordered]@{movement=$m;prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');player=(Q $Source '--player-states')}|ConvertTo-Json -Depth 10|Set-Content "$out/ready-failure.json";throw 'Grenade network body/prop not ready'}
 $checks.Add('Native Steam handshake and exact candidate bank drive the remote held grenade without a prop command')
 $prop|ConvertTo-Json|Set-Content "$out/prop-initial.json"
 $hwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();''')
sub(' $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();', ''' $sourceWindow=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64()
 $sourceArgs=@($args|ForEach-Object {if($_-like 'gfxcapture=*'){"gfxcapture=hwnd=${sourceWindow}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"}elseif($_-eq"$PSScriptRoot/$Take.mp4"){"$PSScriptRoot/$Take-source.mp4"}else{$_}})
 $sourceCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $sourceArgs -WindowStyle Hidden -RedirectStandardError "$out/capture-source.log" -RedirectStandardOutput "$out/capture-source.stdout" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();''')
sub(' $watch=[Diagnostics.Stopwatch]::StartNew();$fired=$false;$scenes=@{}',' $watch=[Diagnostics.Stopwatch]::StartNew();$fired=$false;$duplicated=$false;$scenes=@{}')
a=s.index('  $midnight=Q $Source');b=s.index('\n }\n if($monitor)',a)
s=s[:a]+'''  $m=S $peer '.movement.json';$prop=S $peer '.grenade-prop.json'
  if(!$duplicated-and$m.grenadeVariant-eq3){C $Source 'coop_shadow_stream_duplicate';$duplicated=$true}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;movement=$m;prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');fixture=(S $Source '.grenade-fixture.json')})
  Start-Sleep -Milliseconds 45'''+s[b:]
sub(" if(!@($rows|Where-Object {$null-ne$_.item-and$_.item.quantity-eq0}).Count){throw 'No native grenade consumption observed'};", " if((Q $Source '--player-grenade').quantity-ne0){throw 'No native grenade consumption observed'};")
mark=" C $Source 'coop_grenade_fixture off';Start-Sleep -Milliseconds 1200"
sub(mark,''' $expected=if($Pitch-ge30){@(3,4,7,8)}else{@(3,4,5,6)}
 foreach($v in $expected){if(!@($rows|Where-Object {$_.movement.active-and$_.movement.grenadeVariant-eq$v-and$_.movement.grenadeWeight-gt.75}).Count){throw "Remote grenade phase $v absent"}}
 $checks.Add('Native cook entry, cook loop, release and empty recovery drive remote whole-body clips on source clock')
 $release=if($Pitch-ge30){7}else{5};$recovery=$release+1
 if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$release-and$_.movement.grenadeAmount-lt.7-and$_.prop.active}).Count){throw 'Remote held prop absent before release'}
 if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$recovery-and$_.movement.grenadeAmount-gt.15-and!$_.prop.active-and!$_.prop.pendingHide}).Count){throw 'Prop remained visible in empty recovery'}
 if(!@($rows|Where-Object {$_.ms-gt6000-and!$_.movement.grenadeHeld-and$_.movement.grenadeWeight-lt.01-and!$_.prop.active}).Count){throw 'Grenade presentation did not recover to standing'}
 $checks.Add('Cosmetic grenade disappears at release and stays hidden through empty recovery and native standing')
 if(!$duplicated){throw 'Duplicate packet stimulus missed'};$checks.Add('Duplicate packet sent during native cook; prop remains one cached native entity')
 if(@($rows|Where-Object {$_.prop.created-ne$prop.created}).Count){throw 'Duplicate prop creation'}
 C $Source 'coop_grenade_fixture off';Start-Sleep -Milliseconds 1200''')
sub("try{I $r;C $r 'coop_grenade_fixture off';C $r 'coop_movement_fixture off'}", "try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_grenade_prop off';C $r 'coop_grenade_fixture off';C $r 'coop_movement_fixture off'}")
sub(' if($monitor){$monitor.WaitForExit', " if($sourceCapture){$sourceCapture.WaitForExit(15000)|Out-Null;if(!$sourceCapture.HasExited-or$sourceCapture.ExitCode){$failure='Source capture incomplete'}}\n if($monitor){$monitor.WaitForExit")
sub("scope='Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed.'", "scope='Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate.'")
sub('"PASS_NATIVE_GRENADE_ACTIONS $Source $($checks.Count) checks"','"PASS_GRENADE_NETWORK $Source $($checks.Count) checks"')
(P/'Test-GrenadeNetwork-v56.ps1').write_text(s)
print('Native grenade network test prepared; always pass -ValidateReader')
