from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoppelgangerAir-v506.ps1').read_text(encoding='utf-8-sig')
a=s.index(' $context=Q client');s=s[:a]+''' & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
 & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/holster-look-return.json" -Quick
'''+s[a:]
s=s.replace('or!$sword.equipped','or$sword.equipped').replace('grounded armed context','grounded holstered context')
a=s.index('  if(!$cast-and');b=s.index("  $scene=Q client '--midnight'",a)
s=s[:a]+'''  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs 700|Out-Null;$cast=$true;$jumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native A+LT700 from holstered standing; observe entry while jumping, then natural release'})
  }
'''+s[b:]
s=s.replace('$airRelease', '$airEntry').replace('power_decoy_cast_out.midnightscene\'-and$_.sceneDurationMs-eq666', 'power_decoy_cast_in.midnightscene\'-and$_.sceneDurationMs-eq466')
s=s.replace('airReleaseSamples=', 'airEntrySamples=').replace('releaseAfterJumpMs=$ReleaseAfterJumpMs','jumpAndAimMs=700;holstered=$true')
s=s.replace('No directly sampled native airborne release; inputs alone do not prove airborne casting','No directly sampled native airborne entry; inputs alone do not prove airborne casting')
s=s.replace('reciprocal666ms Doppelganger release','reciprocal466ms Doppelganger entry')
s=s.replace('Native source Emily Doppelganger aim then bounded jump/release observation.', 'Native source holstered Emily Doppelganger simultaneous jump/aim entry observation.')
p=N/'Observe-NativeDoppelgangerAirEntry-v507.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
w=(W/'Run-DoppelgangerAirObserve-v506.ps1').read_text(encoding='utf-8-sig').replace('native-doppelganger-air-release-v506','native-doppelganger-air-entry-holstered-v507').replace('Observe-NativeDoppelgangerAir-v506','Observe-NativeDoppelgangerAirEntry-v507')
p=W/'Run-DoppelgangerAirEntryObserve-v507.ps1';assert not p.exists();p.write_text(w,encoding='utf-8')
print('Prepared complementary native holstered air entry observation.')
