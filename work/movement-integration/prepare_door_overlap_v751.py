from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorOverlap-v750.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-close-reopen-host-v750','native-door-close-handoff-host-v751')
rep('NativeDoorCloseReopen750','NativeDoorCloseHandoff751')
rep("C $peer 'setviewpos -52.3 -160.75 68.1 0 0'","C $peer 'setviewpos -49.2 -161.4 68.1 163 0'")
rep("      I $peer X 200;$inputs.Add", """      C $source 'setviewpos -44 -161 68.1 220 0'
      C $peer 'setviewpos -50.3 -160.75 68.1 180 0'
      $secondNative=DoorSample $peer
      $secondNative|ConvertTo-Json -Depth 5|Set-Content "$out/native-before-second-use.json"
      if($secondNative.current-ne1-or$secondNative.pending-ne1){throw 'Known use-position handoff missed native Moving window'}
      I $peer X 200;$inputs.Add""")
(N/'Test-DoorOverlap-v751.ps1').write_text(s)
print('Prepared751 shared known use-position handoff during closing')
