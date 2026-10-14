from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeDeselect-v235.ps1').read_text(encoding='utf-8-sig')
key='function OptionalFx($r)'
insert='''function TransitionRead($mode){
 $raw=& "$lab/bin/d2_runtime_probe.exe" $pids.client $mode;$code=$LASTEXITCODE
 try{$value=$raw|ConvertFrom-Json}catch{throw "Transition probe $mode returned non-JSON output"}
 if($null-eq$value.valid){throw "Transition probe $mode omitted validity"}
 $value|Add-Member -NotePropertyName probeExitCode -NotePropertyValue $code -Force
 return $value
}
'''
assert key in s;s=s.replace(key,insert+key,1)
s=s.replace("$sourceNow=Q client '--domino-links';$targetsNow=Q client '--domino-targets';", "$sourceNow=TransitionRead '--domino-links';$targetsNow=TransitionRead '--domino-targets';")
key=" if($stable-lt6){throw 'Native target deselection or its replicated world state did not settle'}"
new=''' if($stable-lt6){
  Q client '--player-domino'|ConvertTo-Json -Depth 9|Set-Content "$out/deselect-failed-equipment.json"
  Q client '--object' $networkAddress|ConvertTo-Json -Depth 10|Set-Content "$out/deselect-failed-network.json"
  Q client '--object' ('0x{0:x}'-f([Convert]::ToUInt64($networkAddress.Substring(2),16)+0x15b0))|ConvertTo-Json -Depth 10|Set-Content "$out/deselect-failed-connections.json"
  throw 'Native target deselection or its replicated world state did not settle'
 }'''
assert key in s;s=s.replace(key,new)
(P/'Test-DominoNativeDeselect-v236.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeDeselect-v235.ps1','Test-DominoNativeDeselect-v236.ps1');runner.write_text(s,encoding='utf-8')
