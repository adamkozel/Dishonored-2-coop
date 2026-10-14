param([string]$Take='use-focus-v1133',[string]$Role='host')
# Read-only search for the native interaction focus: aim at a Dunwall window
# (proven prompt, chapter 832 geometry), dump the player actor and every
# player component, then aim away and dump again. Words that equal the window
# entity handle / actor / usable component only while focused locate the focus.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($cmd){& "$lab/Send-D2Command.ps1" -Role $Role -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function Q($mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $script:procId $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $script:procId $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $mode"}
function Dump($tag){
 $pl=Q '--player';$ents=(Q '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player)
 if($me.Count-ne1){throw 'Player entity not unique'}
 $objs=[ordered]@{player=(Q '--object' $pl.player)}
 foreach($c in (Q '--components' $me[0].handle).components){$objs["$($c.type)@$($c.pointer)"]=Q '--object' $c.pointer}
 Save "$out/dump-$tag.json" $objs;return $objs}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json;$script:procId=$l.pid
 $win=@((Q '--entities').entities|Where-Object name -eq 'ld_rich_small_01_sndctr_4')[0]
 $usable=@((Q '--components' $win.handle).components|Where-Object type -eq '.?AVarkCpntUsableWithStateList@@')[0]
 $keys=@{handle=$win.handle;actor=$win.pointer;component=$usable.pointer};Save "$out/keys.json" $keys
 $dirty=$true
 C 'setviewpos -27.85 -149.123 67.0 0 0';Start-Sleep -Milliseconds 1500
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Role -X -26.5714 -Y -149.123 -Z 68.0 -Evidence "$out/aim-focus.json";Start-Sleep -Milliseconds 400
 $hwnd=(Get-Process -Id $script:procId).MainWindowHandle.ToInt64()
 & ffmpeg -hide_banner -loglevel error -y -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 "$out/focus.png"
 $a=Dump 'focus-a';Start-Sleep -Milliseconds 300;$b=Dump 'focus-b'
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Role -X -26.5714 -Y -153.5 -Z 64.0 -Evidence "$out/aim-away.json";Start-Sleep -Milliseconds 400
 & ffmpeg -hide_banner -loglevel error -y -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 "$out/away.png"
 $c=Dump 'away'
 $hits=[Collections.Generic.List[object]]::new()
 $hex=@{};foreach($k in $keys.Keys){$v=[Convert]::ToUInt64(($keys[$k] -replace '^0x',''),16);$hex[$k]=$v}
 foreach($name in $a.Keys){foreach($w in $a[$name].words){$v=[Convert]::ToUInt64($w.word.Substring(2),16)
   foreach($k in $hex.Keys){if($v-eq$hex[$k]-or($v-band[uint64]4294967295)-eq$hex[$k]-or($v-shr32)-eq$hex[$k]){
     $wb=@($b[$name].words|Where-Object offset -eq $w.offset)[0].word;$wc=@($c[$name].words|Where-Object offset -eq $w.offset)[0].word
     $hits.Add(@{object=$name;offset=$w.offset;key=$k;focusA=$w.word;focusB=$wb;away=$wc})}}}}
 Save "$out/hits.json" @($hits)
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){$errors=[Collections.Generic.List[string]]::new()
  if($dirty){try{& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null;& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null}catch{$errors.Add($_.Exception.Message)}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure};$mutex.ReleaseMutex()};$mutex.Dispose()}
Get-Content "$out/result.json";if($failure){throw $failure}
