param([ValidateSet('host','client')][string]$Role='host',[Parameter(Mandatory)][string]$Evidence)
# Read-only research: no power acquisition, resource writes or gameplay calls.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$probe=Join-Path $root 'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
$process=Get-Process -Id $launch.pid
if(!$launch.assetExperiment-or$process.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
function Query($mode,$handle){$raw=& $probe $launch.pid $mode $handle;if($LASTEXITCODE){throw "Read failed: $mode"};return($raw|ConvertFrom-Json)}
function Bits($word){return [Convert]::ToUInt64(($word-replace'^0x',''),16)}
$components=Query '--components' 1
$matches=@($components.components|Where-Object {$_.type-eq'.?AVarkCpntMana@@'-and$_.vtableRva-eq'0x1c55ed8'})
if($matches.Count-ne1){throw 'Expected one exact native Mana component'};$c=$matches[0]
$object=Query '--object' $c.pointer
if($object.pointer-ne$c.pointer-or$object.type-ne'.?AVarkCpntMana@@'-or(Bits ($object.words|Where-Object offset -eq '0x10').word)-ne(Bits $c.owner)){throw 'Mana object owner changed'}
$valueWord=($object.words|Where-Object offset -eq '0x20').word
$bits=[uint32]((Bits $valueWord)-band[uint64]4294967295);$current=[BitConverter]::ToSingle([BitConverter]::GetBytes($bits),0)
if(![float]::IsFinite($current)-or$current-lt0-or$current-gt10000){throw 'Invalid mana observation'}
$again=Query '--components' 1;$same=@($again.components|Where-Object {$_.pointer-eq$c.pointer-and$_.owner-eq$c.owner-and$_.vtableRva-eq$c.vtableRva-and$_.type-eq$c.type})
if($same.Count-ne1-or(Get-Process -Id $launch.pid).StartTime-ne$process.StartTime){throw 'Player component or process identity changed'}
$record=[ordered]@{at=[DateTimeOffset]::Now.ToString('o');role=$Role;pid=$launch.pid;component=$c;currentCandidate=$current;offset='0x20';word=$valueWord;repeatedMembershipConfirmed=$true;scope='Read-only candidate mana observation, supported by exact-PE initialization decompilation; repeated membership and owner guarded. No maximum/cost or write API inferred.'}
$record|ConvertTo-Json -Depth 5|Set-Content -LiteralPath $Evidence -Encoding utf8
[pscustomobject]$record
