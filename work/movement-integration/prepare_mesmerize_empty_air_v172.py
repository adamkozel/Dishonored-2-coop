"""Observe native airborne Mesmerize with the sword holstered before casting."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
for case,source in [('Entry',162),('Release',157)]:
    s=(P/f'Observe-NativeMesmerizeAir{case}-v{source}.ps1').read_text(encoding='utf-8-sig')
    anchor=" $checks.Add('Standing native source starts on the proven floor before the bounded jump')"
    assert s.count(anchor)==1
    s=s.replace(anchor,anchor+'''
 & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null
 Start-Sleep -Milliseconds 1100;$w.Restart();$emptyReady=$false
 do{
  $emptySword=Q client '--player-sword';$emptyState=Q client '--player-states'
  $emptyReady=!$emptySword.equipped-and$emptyState.defaultMode-and$emptyState.motionValid-and$emptyState.motion-eq0-and@($emptyState.active|Where-Object {$_.id-ne0}).Count-eq0
  if($emptyReady){break};Start-Sleep -Milliseconds 60
 }while($w.ElapsedMilliseconds-lt5000)
 [ordered]@{ready=$emptyReady;sword=$emptySword;player=$emptyState}|ConvertTo-Json -Depth 10|Set-Content "$out/empty-ready.json"
 if(!$emptyReady){throw 'Native X did not establish idle holstered equipment before airborne observation'}
 $checks.Add('Native X establishes confirmed holstered sword and settled standing before the jump/cast')
''')
    a="player=(Q client '--player-states');midnight=$scene;"
    assert s.count(a)==1
    s=s.replace(a,"player=(Q client '--player-states');sword=(Q client '--player-sword');midnight=$scene;")
    anchor=" $airRows=@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in2,3}).Count})"
    assert s.count(anchor)==1
    s=s.replace(anchor,anchor+'''
 $unexpectedDraw=@($rows|Where-Object {$_.sword.equipped})
 [ordered]@{samples=$rows.Count;airSamples=$airRows.Count;unexpectedDrawCount=$unexpectedDraw.Count;equipment=@($rows|Select-Object ms,sword)}|ConvertTo-Json -Depth 10|Set-Content "$out/air-equipment-observations.json"
 if($unexpectedDraw.Count){throw 'Native airborne cast drew the sword; this is not empty-hand evidence'}
 $checks.Add('Direct native equipment observations retain the holstered sword through airborne phases and recovery')
''')
    s=s.replace('Current strict reader still refuses airborne Mesmerize,','Current strict reader still refuses holstered airborne Mesmerize,')
    s=s.replace('Native source in-place jump and Mesmerize aim begin together','Native source sword is holstered before the in-place jump and Mesmerize aim begin together')
    s=s.replace('Native source jump during Mesmerize aim','Native source sword is holstered before jumping during Mesmerize aim')
    (P/f'Observe-NativeMesmerizeEmptyAir{case}-v172.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
s=s.replace("'MesmerizeContext','MesmerizeAirNetwork'","'MesmerizeContext','MesmerizeEmptyAirObserve','MesmerizeAirNetwork'")
anchor="  'MesmerizeAirNetwork'";assert s.count(anchor)==1
s=s.replace(anchor,"  'MesmerizeEmptyAirObserve'{if($AirCase-eq'entry'){& \"$root/work/natural-moveset/Observe-NativeMesmerizeEmptyAirEntry-v172.ps1\" -Take $Take -Pitch $Pitch}else{& \"$root/work/natural-moveset/Observe-NativeMesmerizeEmptyAirRelease-v172.ps1\" -Take $Take -Pitch $Pitch}}\n"+anchor)
runner.write_text(s,encoding='utf-8')
