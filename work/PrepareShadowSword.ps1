[CmdletBinding()]
param([switch]$PlaySetupScene,[string]$EvidencePath='')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
Set-Location $root
function C($r,$s){& outputs/d2-coop-lab/Send-D2Command.ps1 -Role $r -Command $s -WaitConsumed|Out-Null}
function S($r){for($attempt=0;$attempt-lt40;$attempt++){try{return(Get-Content "work/lab-instances/$r/coop-status.json" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{if($attempt-eq39){throw};Start-Sleep -Milliseconds 25}}}
& outputs/d2-coop-lab/Restore-D2Lab.ps1 -Doppelganger|Out-Null
foreach($r in 'host','client'){C $r 'setviewpos -39.4 -161.4 68.1 270 20';& outputs/d2-coop-lab/Send-D2Input.ps1 -Role $r -Button X -DurationMs 400|Out-Null}
$seen=@{host=$false;client=$false};$watch=[Diagnostics.Stopwatch]::StartNew()
$skipBefore=$null;$skipAt=$null;$peerInputHash=$null
do{
 $states=@{host=(S host);client=(S client)};$ready=$true
 foreach($r in 'host','client'){$s=$states[$r];if($s.localGameLogicState-eq5){$seen[$r]=$true};if(!$seen[$r]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}}
 if(!$PlaySetupScene-and$null-eq$skipBefore-and!$ready){
  # Ordinary native hold-to-skip, once, only after both games advertise the
  # same skippable segment. The existing Steam relay confirms on the peer.
  # Other scenes or missing metadata simply retain full playback.
  $matching=$true
  foreach($r in 'host','client'){
   $s=$states[$r]
   if($s.localGameLogicState-ne5-or$s.localPendingGameLogicState-or!$s.remoteFresh-or$s.connectionState-ne3-or
      !$s.cinematicSyncEnabled-or$s.remoteCinematicSyncEnabled-ne1-or$s.localCinematicCanSkip-ne1-or$s.remoteCinematicCanSkip-ne1-or
      $s.localSequenceMetadataValid-ne1-or$s.remoteSequenceMetadataValid-ne1-or
      $s.localSkipSceneId-eq'0'-or$s.localSkipSceneId-ne$s.remoteSkipSceneId-or
      $s.localSequenceBoundaryId-eq'0'-or$s.localSequenceBoundaryId-ne$s.remoteSequenceBoundaryId){$matching=$false}
  }
  if($matching-and$states.host.localSkipSceneId-eq$states.client.localSkipSceneId-and$states.host.localSequenceBoundaryId-eq$states.client.localSequenceBoundaryId){
   $skipBefore=$states;$skipAt=$watch.ElapsedMilliseconds
   $peerInputHash=(Get-FileHash 'work/lab-instances/client/input.bin').Hash
   & outputs/d2-coop-lab/Send-D2Input.ps1 -Role host -Button X -DurationMs 3000|Out-Null
  }
 }
 if(!$ready){Start-Sleep -Milliseconds 150}
}while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
if(!$ready){throw 'Both native Mayhew scenes did not complete'}
# Release before repositioning even when the scene ended within the hold.
if($null-ne$skipBefore){& outputs/d2-coop-lab/Send-D2Input.ps1 -Role host -DurationMs 100|Out-Null}
$skipAfter=@{host=(S host);client=(S client)}
$skipConfirmed=$null-ne$skipBefore-and$skipAfter.host.nativeSkipCallbacks-eq$skipBefore.host.nativeSkipCallbacks+1-and
 $skipAfter.host.skipSent-eq$skipBefore.host.skipSent+1-and$skipAfter.client.skipApplied-eq$skipBefore.client.skipApplied+1-and
 $skipAfter.client.nativeSkipCallbacks-eq$skipBefore.client.nativeSkipCallbacks-and$skipAfter.client.skipSent-eq$skipBefore.client.skipSent-and
 $skipAfter.client.skipDropped-eq$skipBefore.client.skipDropped-and(Get-FileHash 'work/lab-instances/client/input.bin').Hash-eq$peerInputHash
if(!$EvidencePath){$EvidencePath='outputs/d2-coop-lab/research/native-sword-setup-latest.json'}
[ordered]@{observedAt=[DateTimeOffset]::Now.ToString('o');mode=$(if($PlaySetupScene){'full_playback'}else{'native_skip_when_ready'});sceneElapsedMs=$watch.ElapsedMilliseconds;skipRequested=($null-ne$skipBefore);skipAtMs=$skipAt;skipConfirmed=$skipConfirmed;before=$skipBefore;after=$skipAfter}|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $EvidencePath
if($null-ne$skipBefore-and!$skipConfirmed){throw 'Setup skip did not produce exactly one native confirmation and one peer application; inspect skip evidence'}
C host 'setviewpos -33 -153 68.1 90 0';C client 'setviewpos -33 -148 68.1 270 0'
foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
'Native sword fixture ready'
