from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoFinalRecovery-v241.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$ci=[Globalization.CultureInfo]::InvariantCulture;$observerX=$sourceX-.4;$observerY=$sourceY-.8", """$ci=[Globalization.CultureInfo]::InvariantCulture
   $currentSource=Q client '--player-states';if(!$currentSource.motionValid-or$currentSource.motion-ne0){throw 'Source left native floor before stream setup'}
   # Native source collision may have pushed away from the west stair wall.
   # Place the observer east of that measured point, never west through it.
   $observerX=[double]$currentSource.position[0]+2.5;$observerY=[double]$currentSource.position[1]-1.8;$observerZ=[double]$currentSource.position[2]+1.7
""")
s=s.replace("$observerY.ToString('F4',$ci),$sourceZ.ToString('F4',$ci)","$observerY.ToString('F4',$ci),$observerZ.ToString('F4',$ci)")
s=s.replace("$observerYaw=[Math]::Atan2([double]$target2.position[1]-$observerY,[double]$target2.position[0]-$observerX)*180/[Math]::PI", "$observerYaw=[Math]::Atan2([double]$currentSource.position[1]-$observerY,[double]$currentSource.position[0]-$observerX)*180/[Math]::PI")
s=s.replace('-Role host -Pitch 0 -Evidence "$out/peer-camera.json"','-Role host -Pitch 12 -Evidence "$out/peer-camera.json"')
anchor='   Copy-Item "$PSScriptRoot/domino-banks-v192/emily-domino-bank.bin"'
at=s.index(anchor)
s=s[:at]+'''   $floorRows=[Collections.Generic.List[object]]::new();$settledFloor=0;$lastFloor=$null;$floorWatch=[Diagnostics.Stopwatch]::StartNew()
   do{
    $nativeFloor=Q host '--player-states';$floorRows.Add($nativeFloor);$same=$nativeFloor.motionValid-and$nativeFloor.motion-eq0-and$nativeFloor.stance-eq0-and$null-ne$lastFloor
    if($same){for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$nativeFloor.position[$k]-[double]$lastFloor.position[$k])-gt.005){$same=$false}}}
    $settledFloor=if($same){$settledFloor+1}else{0};$lastFloor=$nativeFloor;if($settledFloor-ge4){break};Start-Sleep -Milliseconds 125
   }while($floorWatch.ElapsedMilliseconds-lt4000)
   $floorRows|ConvertTo-Json -Depth 9|Set-Content "$out/peer-ground-before-stream.json"
   if($settledFloor-lt4){throw 'Observer not stably grounded before stream; no body or world presentation started'}
''' +s[at:]
s=s.replace("marks=(S host '.domino-world.json');links=(S host '.domino-links.json')", "marks=(S host '.domino-world.json');links=(S host '.domino-links.json');sourceStream=(S client '.shadow-stream.json')")
s=s.replace(" if($networkStarted){$peerRows.Add", " if($networkStarted){$peerRows.Add")
(P/'Test-DominoFinalRecovery-v248.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-DominoFinalRecovery-v241.ps1','Test-DominoFinalRecovery-v248.ps1').replace('Test-DominoFinalRecovery-v244.ps1','Test-DominoFinalRecovery-v248.ps1');p.write_text(s,encoding='utf-8')
