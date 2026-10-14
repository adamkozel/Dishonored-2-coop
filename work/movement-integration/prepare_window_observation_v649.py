from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoor-v641.ps1').read_text().replace('native-unlocked-door-corvo-v641','native-window-corvo-v649').replace('NativeDoor641','NativeWindow649')
s=s.replace("function I($r,$b='Neutral',$ms=100){& \"$lab/Send-D2Input.ps1\" -Role $r -Button $b -DurationMs $ms|Out-Null}","function I($r,$b='Neutral',$ms=100,$y=0){& \"$lab/Send-D2Input.ps1\" -Role $r -Button $b -DurationMs $ms -LeftY $y|Out-Null}")
a=s.index('function W(');b=s.index('try{\n try{$owned=',a);s=s[:a]+s[b:]
s=s.replace("name='unlocked-door-tap';yaw=270", "name='window-open-traverse';yaw=0")
s=s.replace('setviewpos -44 -161 68.1 220 0','setviewpos -31.5 -157 68.1 45 0').replace('setviewpos -47.05 -163.5 68.1 270 0','setviewpos -27.6 -153.9 68.1 0 0')
s=s.replace('-Pitch 10 -Evidence', '-Pitch 8 -Evidence')
a=s.index('   $doorBindings=@{}');b=s.index('   $character=',a);s=s[:a]+s[b:]
s=s.replace("$_.type-match'Door'", "$_.type-match'(Door|Usable)'")
s=s.replace('doorSource=(DoorSample $source);doorPeer=(DoorSample $peer);','nativeView=(Q $source \'--player\');')
s=s.replace("if(!$second-and$watch.ElapsedMilliseconds-ge7000){I $source X 250;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='X250';attempt=2});$second=$true}","if(!$second-and$watch.ElapsedMilliseconds-ge5000){I $source A 600 22000;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='A600+forward22000';attempt=2});$second=$true}")
s=s.replace('Two ordinary short-X requests recorded at the adjacent unlocked door; scene/state/door results require inspection', 'Native X250 window use, then A600 with forward input recorded at the existing office window; source scene/state/geometry require inspection')
s=s.replace('Source-only native unlocked door short-X observation with independently read source/peer Openable states.','Source-only native window use and bounded jump/forward traversal attempt at an existing Dunwall office window.')
s=s.replace('Successful script execution does not prove an open/close action or new remote binding.', 'Successful script execution does not prove a completed traversal or new remote binding.')
s=s.replace('PASS_NATIVE_DOOR_OBSERVATION','PASS_NATIVE_WINDOW_OBSERVATION')
p=N/'Observe-NativeWindow-v649.ps1'
if p.exists():raise SystemExit('Unique script exists')
p.write_text(s);print('649 prepared: existing window handle fixture, native open then bounded A+forward attempt; source and observer recording with full restoration.')
