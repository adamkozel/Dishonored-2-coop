from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
helper=(P/'Aim-NativeDominoGuard-v218.ps1').read_text(encoding='utf-8-sig')
helper=helper.replace("[Parameter(Mandatory)][string]$Evidence)","[Parameter(Mandatory)][string]$Evidence,[ValidateSet('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')][string]$TargetSpawner='ld_spawner_guard_tutorial_2')")
helper=helper.replace("$_.spawnerName-eq'ld_spawner_guard_tutorial_2'","$_.spawnerName-eq$TargetSpawner")
helper=helper.replace('Exact same-floor tutorial guard2 unavailable','Exact same-floor tutorial guard unavailable')
(P/'Aim-NativeDominoGuard-v234.ps1').write_text(helper,encoding='utf-8')
s=(P/'Test-DominoNativeCancel-v233.ps1').read_text(encoding='utf-8-sig')
s=s.replace('while($w.ElapsedMilliseconds-lt23000)','while($w.ElapsedMilliseconds-lt18000)')
a=s.index(' & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -35');b=s.index(' $stop.Restart();C host',a)
s=s[:a]+''' & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/deselect-pitch.json" -Quick
 & "$PSScriptRoot/Aim-NativeDominoGuard-v234.ps1" -Role client -TargetSpawner ld_spawner_guard_tutorial_1 -Evidence "$out/deselect-aim.json"
 $beforeDeselect=Q client '--domino-targets';$beforeDeselect|ConvertTo-Json -Depth 10|Set-Content "$out/targets-before-deselect.json"
 $removedHandle=[Convert]::ToUInt32($target2.handle.Substring(2),16);$keptHandle=[Convert]::ToUInt32($target.handle.Substring(2),16)
 $keptBefore=@($peerWorld.marks|Where-Object target -eq $keptHandle)
 if(!$beforeDeselect.valid-or$beforeDeselect.targets.Count-ne2-or$keptBefore.Count-ne1){throw 'Two native targets and retained peer mark required before deselection'}
 $cancelMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-deselect-scenes.ndjson" -RedirectStandardError "$out/native-deselect-scenes.stderr" -PassThru
 I client 255 1800;$stop=[Diagnostics.Stopwatch]::StartNew()
 $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT hold1800ms toward already-linked guard1 then release; native target deselection observed, no direct network/target mutation'})
 $deselectRows=[Collections.Generic.List[object]]::new();$stable=0
 do{
  $sourceNow=Q client '--domino-links';$targetsNow=Q client '--domino-targets';$peerNow=S host '.domino-links.json';$marksNow=S host '.domino-world.json'
  $movementNow=S host '.movement.json';$streamNow=S host '.shadow-stream.json'
  $deselectRows.Add([pscustomobject]@{ms=$stop.ElapsedMilliseconds;source=$sourceNow;targets=$targetsNow;peer=$peerNow;marks=$marksNow;movement=$movementNow;stream=$streamNow;sourceFx=(OptionalFx client);peerFx=(OptionalFx host)})
  $matched=$sourceNow.valid-and$targetsNow.valid-and$sourceNow.targets-eq1-and!$sourceNow.connections.Count-and$targetsNow.targets.Count-eq1-and$targetsNow.targets[0].handle-eq$keptHandle-and!$peerNow.beams.Count-and$marksNow.marks.Count-eq1-and$marksNow.marks[0].target-eq$keptHandle
  $stable=if($matched){$stable+1}else{0};if($stable-ge6){break};Start-Sleep -Milliseconds 100
 }while($stop.ElapsedMilliseconds-lt6500)
 I client
 $deselectRows|ConvertTo-Json -Depth 16|Set-Content "$out/native-deselect-diagnostics.json"
 if($stable-lt6){throw 'Native target deselection or its replicated world state did not settle'}
 if($marksNow.marks[0].id-ne$keptBefore[0].id-or$peerNow.stopped-ne($peerLinks.stopped+1)-or$peerNow.started-ne$peerLinks.started-or$peerNow.refused-ne$peerLinks.refused){throw 'Deselection recreated retained mark or did not stop exact beam'}
 if(@($deselectRows|Where-Object {$_.movement.weight-lt.95-or!$_.movement.active-or!$_.stream.ready-or!$_.stream.armed}).Count){throw 'Target deselection broke body/root stream continuity'}
 $checks.Add('Native target reselection removes only guard1, leaving guard2 marked; native Steam retires the exact beam and removed mark while preserving the surviving mark incarnation and full body/root stream')
'''+s[b:]
# Beam already retired during deselection; presentation stop only clears the surviving mark.
s=s.replace("if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Independent presentation stop did not retire slots'}", "if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Independent presentation stop did not retire remaining mark'}")
s=s.replace("$sourceAfter.targets-ne2-or$sourceAfter.connections.Count-ne2", "$sourceAfter.targets-ne1-or$sourceAfter.connections.Count-ne0")
s=s.replace("Independent peer presentation stop retires cosmetics within bounded native fade while source native links persist; target deselection is a separate case", "Independent peer presentation stop retires the remaining cosmetic mark within bounded native fade while the one native source target persists")
s=s.replace('Native X aim cancellation is verified to preserve established native links, with the peer matching that state and recovering body to idle. Independent peer presentation stop is separately tested; native target deselection remains pending.', 'Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. Native cast subscenes are retained for separate body coverage analysis; this test does not claim unobserved phase animation support.')
(P/'Test-DominoNativeDeselect-v234.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
s=s.replace("'DominoCancel','DominoLinksNetwork'","'DominoCancel','DominoDeselect','DominoLinksNetwork'",2)
key="  'DominoCancel'{";assert key in s;s=s.replace(key,"  'DominoDeselect'{& \"$root/work/natural-moveset/Test-DominoNativeDeselect-v234.ps1\" -Take $Take -Pitch $Pitch -AimGuard}\n"+key)
runner.write_text(s,encoding='utf-8')
