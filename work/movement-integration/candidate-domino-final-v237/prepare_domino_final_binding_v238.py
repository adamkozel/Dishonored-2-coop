from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeDeselect-v235.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$cancelMonitor=$null;', '$finalMonitor=$null;$cancelMonitor=$null;',1)
key=" $cancelMonitor=Start-Process"
idx=s.index(key)
s=s[:idx]+''' $finalMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-final-scenes.ndjson" -RedirectStandardError "$out/native-final-scenes.stderr" -PassThru
'''+s[idx:]
key=" $checks.Add('Native target reselection removes only guard1, leaving guard2 marked; native Steam retires the exact beam and removed mark while preserving the surviving mark incarnation and full body/root stream')"
assert key in s
s=s.replace(key,key+'''
 $finalPeers=@($deselectRows|Where-Object {$_.movement.powerVariant-eq21-and$_.movement.powerWeight-ge.25})
 if(!$finalPeers.Count){throw 'Native final-select full-body gesture did not reach peer'}
 if(@($deselectRows|Where-Object {$_.movement.clockRejection.count-ne0}).Count){throw 'Final selection rejected body source clock'}
 $finalPeers|ConvertTo-Json -Depth 16|Set-Content "$out/final-select-peer-proof.json"
''',1)
key=" if($cancelMonitor){$cancelMonitor.WaitForExit(10000)"
idx=s.index(key)
s=s[:idx]+''' if($finalMonitor){
  $finalMonitor.WaitForExit(12000)|Out-Null
  try{
   if(!$finalMonitor.HasExited-or$finalMonitor.ExitCode){throw 'Native final phase observer failed'}
   $finalRows=@(Get-Content "$out/native-final-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object {$_.valid-and$_.subscene-eq'Cast_final_Select'})
   if($finalRows.Count-lt4-or@($finalRows|Where-Object {$_.variant-ne21-or$_.durationMs-ne400-or!$_.playerStatesValid-or!($_.activeStates-contains72)}).Count){throw 'Exact native final_Select state72/400ms not verified'}
   if(@($finalRows.armHandle|Sort-Object -Unique).Count-ne1-or@($finalRows.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Final selection arm/equipment incarnation changed'}
   $finalRows|ConvertTo-Json -Depth 8|Set-Content "$out/final-select-native-proof.json"
   $checks.Add('Exact native400ms final_Select is observed with state72 and reciprocal owned arm/equipment; shared native selection clip maps to peer whole-body selection gesture and native idle recovery without root-clock rejection')
  }catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 }
'''+s[idx:]
s=s.replace('Native cast subscenes are retained for separate body coverage analysis; this test does not claim unobserved phase animation support.', 'The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported.')
(P/'Test-DominoFinalSelect-v238.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
s=s.replace("'DominoCancel','DominoDeselect'","'DominoCancel','DominoFinalSelect','DominoDeselect'",2)
key="  'DominoDeselect'{";assert key in s;s=s.replace(key,"  'DominoFinalSelect'{& \"$root/work/natural-moveset/Test-DominoFinalSelect-v238.ps1\" -Take $Take -Pitch $Pitch -AimGuard}\n"+key)
runner.write_text(s,encoding='utf-8')
