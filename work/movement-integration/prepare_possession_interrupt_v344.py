from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-PossessionVisibilityNetwork-v342.ps1').read_text()
s=s.replace('$returnFramed=$false', '$returnFramed=$false;$interrupted=$false;$interruptAt=0',1)
anchor='  if($cast-and!$exitSent-and$w.ElapsedMilliseconds-ge5200)'
assert s.count(anchor)==1
s=s.replace(anchor,'''  if($cast-and!$interrupted-and$w.ElapsedMilliseconds-ge4100){
   $occupied=Q host '--player-states';$hiddenNow=S client '.possession-body.json'
   if($occupied.defaultMode-or$occupied.stance-ne18-or!$hiddenNow.hidden-or!$hiddenNow.active){throw 'Interruption requires verified occupied source and hidden peer body'}
   [ordered]@{source=$occupied;visibility=$hiddenNow;stream=(S client '.shadow-stream.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/before-interruption.json"
   C client 'coop_shadow_stream off';$interruptAt=$w.ElapsedMilliseconds;$interrupted=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Stop receiver semantic stream during verified human occupation; do not rearm'})
  }
'''+anchor)
old="if($occupiedRows.Count-lt3-or@($occupiedRows|Where-Object {!$_.visibility.hidden-or!$_.visibility.active-or!$_.stream.ready-or$_.stream.remoteState-ne17}).Count){throw 'Remote body visible or stream stale during bracketed native human occupation'}"
new="""$beforeInterrupt=@($occupiedRows|Where-Object {$_.ms-lt$interruptAt})
 $afterInterrupt=@($rows|Where-Object {$_.ms-gt$interruptAt+500})
 $stillOccupied=@($occupiedRows|Where-Object {$_.ms-gt$interruptAt+500})
 [ordered]@{interruptAt=$interruptAt;beforeHidden=$beforeInterrupt.Count;afterSamples=$afterInterrupt.Count;sourceStillOccupied=$stillOccupied.Count;after=$afterInterrupt|ForEach-Object {[ordered]@{ms=$_.ms;visibility=$_.visibility;ready=$_.stream.ready;armed=$_.stream.armed;sourceStance=$_.player.stance}}}|ConvertTo-Json -Depth 12|Set-Content "$out/interruption-evidence.json"
 if(!$interrupted-or$beforeInterrupt.Count-lt3-or@($beforeInterrupt|Where-Object {!$_.visibility.hidden-or!$_.visibility.active-or!$_.stream.ready-or$_.stream.remoteState-ne17}).Count){throw 'No uninterrupted hidden baseline before receiver stop'}
 if(!$stillOccupied.Count-or$afterInterrupt.Count-lt3-or@($afterInterrupt|Where-Object {$_.visibility.hidden-or$_.visibility.active-or$_.visibility.restoring-or$_.stream.ready-or$_.stream.armed}).Count){throw 'Receiver interruption failed to restore exact clone visibility and retire stream'}"""
assert s.count(old)==1;s=s.replace(old,new)
s=s.replace("All bracketed stable occupied-player observations retain hidden clone and fresh semantic17; transition read gaps reported separately", "Receiver-stop during native occupation restores exact clone visibility within500ms; source remains possessed, receiver remains disarmed, no implicit rearm or leaked visibility lease")
s=s.replace("interruption/alternate contexts pending.","Receiver-stop interruption tested during occupation; native menu/newworld/alternate contexts pending.")
(P/'Test-PossessionInterruption-v344.ps1').write_text(s,encoding='utf-8')
print('344 receiver-stop test prepared; runner unchanged until current operation completes')
