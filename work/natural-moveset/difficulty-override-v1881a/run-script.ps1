param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='difficulty-override-v1881a',[string]$Candidate='candidate-npc-combat-v1214jt',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateRange(0,4)][int]$Override=2)
# 1881 (#177): does a map load honour ark_difficulty_level_override? On the host instance alone: read the active preset
# (probe --difficulty, the difficulty manager at game+0x1f7b90, preset at +0x10) and the difficulty cvars (the probe's
# cvar reader), set the override, read again at once, load the mission with the developer campaign loader and read
# again; then clear the override (-1), read, load again and read. The client stays in the Dunwall fixture. Every read is
# kept in reads.json. The lab is restored at the end (Deploy-SourceClock).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100){try{& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}catch{}}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
$pids=@{};foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}
function Q($r,$mode){for($a=0;$a-lt8;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native read failed $r $mode"}
# The probe's cvar reader prints "<name> name_field_rva ..." and then "  current_text: <text> integer: <n>".
function Cvars($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r];$o=[ordered]@{};$cur=$null
 foreach($l in $raw){if($l-match'^(ark_difficulty_\w+) name_field_rva'){$cur=$Matches[1]}elseif($cur-and$l-match'current_text: (.*) integer: (-?\d+)'){$o[$cur]=[ordered]@{text=$Matches[1].Trim();integer=[int]$Matches[2]};$cur=$null}};$o}
function Probe($label){$d=Q host '--difficulty';$s=S host;$x=[ordered]@{label=$label;preset=$d.preset;cvars=(Cvars host);map=$s.localMapId;epoch=$s.localEpoch;managerVtRva=$d.managerVtRva;presetVtRva=$d.presetVtRva;words=$d.words}
 $notes.reads+=@($x);Save "$out/reads.json" @($notes.reads);$x}
function WaitHost($ms){$w=[Diagnostics.Stopwatch]::StartNew();$last=0
 do{Start-Sleep -Milliseconds 500;$s=S host
  if($w.ElapsedMilliseconds-$last-ge1500){switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I host A 300;$last=$w.ElapsedMilliseconds}};5{I host X 3000;$last=$w.ElapsedMilliseconds};12{I host B 200;$last=$w.ElapsedMilliseconds}}}
 }while(($s.localGameLogicState-ne1-or$s.localPendingGameLogicState)-and$w.ElapsedMilliseconds-lt$ms);$s.localGameLogicState-eq1-and!$s.localPendingGameLogicState}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;override=$Override;reads=@()}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;override=$Override})
 $e0=(S host).localEpoch;$null=Probe 'fixture'
 C host "ark_difficulty_level_override $Override";Start-Sleep -Seconds 2;$null=Probe 'override-set'
 C host "campaign $MapInfo low corvo";Start-Sleep -Seconds 3;if(!(WaitHost 300000)){throw 'The mission load with the override did not reach gameplay'};Start-Sleep -Seconds 5;$null=Probe 'override-loaded'
 C host 'ark_difficulty_level_override -1';Start-Sleep -Seconds 2;$null=Probe 'override-cleared'
 C host "campaign $MapInfo low corvo";Start-Sleep -Seconds 3;if(!(WaitHost 300000)){throw 'The mission load without the override did not reach gameplay'};Start-Sleep -Seconds 5;$null=Probe 'cleared-loaded'
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){try{C host 'ark_difficulty_level_override -1'}catch{}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 $mutex.Dispose()
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='Read-only difficulty reads around the native override cvar on the host instance; lab restored.'})
 if($failure){throw $failure}
}
