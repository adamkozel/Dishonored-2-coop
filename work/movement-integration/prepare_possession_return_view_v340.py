from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionVisibilityNetwork-v339.ps1').read_text().replace('$cast=$false;$exitSent=$false','$cast=$false;$exitSent=$false;$returnFramed=$false')
anchor="  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=";assert s.count(anchor)==1
s=s.replace(anchor,'''  if($exitSent-and!$returnFramed-and$w.ElapsedMilliseconds-ge6500){
   $returned=O host '--player-states'
   if($returned.defaultMode-and$returned.stance-eq0){
    # Previously verified observer floor from323; camera-only reframing.
    $returnYaw=[Math]::Atan2([double]$returned.position[1]+153.27,[double]$returned.position[0]+44.39)*180/[Math]::PI
    C client ('setviewpos -44.39 -153.27 68.1 {0} 0' -f $returnYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
    [ordered]@{source=$returned;observer=(Q client '--player-states');visibility=(S client '.possession-body.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/return-camera.json"
    $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Observer camera-only reframe from previously measured floor after native exit'});$returnFramed=$true
   }
  }
'''+anchor)
s=s.replace(" I host;$manaAfter=", " if(!$returnFramed){throw 'Native exit return not reframed for visual review'}\n I host;$manaAfter=")
(P/'Test-PossessionVisibilityNetwork-v340.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionVisibilityNetwork-v339.ps1','Test-PossessionVisibilityNetwork-v340.ps1');p.write_text(s,encoding='utf-8')
print('340 reuses verified observer floor to record visible native-body return; binary unchanged338')
