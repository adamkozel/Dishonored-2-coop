from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorBusy-v758.ps1').read_text(encoding='utf-8-sig')
s=s.replace('native-door-busy-use-host-v758','native-door-session-live-ai-v766').replace('NativeDoorBusyUse758','NativeDoorSessionLiveAI766').replace('candidate-door-side-v755','candidate-door-session-v763').replace('archived755','archived763')
assert ",'coop_movement_fixture 60000'" in s
s=s.replace(",'coop_movement_fixture 60000'",'').replace("C $r 'coop_door_sync on'","C $r 'coop_door_sync session'")
anchor="   $catalogBefore=@{host=$dh;client=$dc};"
assert s.count(anchor)==1
s=s.replace(anchor,"""   foreach($r in 'host','client'){
    $isolation=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json
    $isolation|ConvertTo-Json -Depth 6|Set-Content "$out/native-isolation-before-$r.json"
    if($isolation.active-or$isolation.heldReferences-or!(DAll $r).session.enabled){throw 'Expected live door session without movement isolation'}
    $npcs=Q $r '--world-npcs';$ramsey=@($npcs.npcs|Where-Object spawnerName -eq 'ld_spawner_ramsey_escape')
    if($ramsey.Count-ne1){throw 'Native Ramsey identity unavailable'}
    $ai=Q $r '--ai-decision' $ramsey[0].handle;$ai|ConvertTo-Json -Depth 7|Set-Content "$out/native-ai-before-$r.json"
    if(!$ai.started-or!$ai.schedulerActive-or$ai.pauseReferences){throw 'Native AI is not active at session start'}
   }
"""+anchor)
s=s.replace('Native input during received closing is not accepted as a reversal;', 'Explicit door session with native AI active from initial arming; no movement-isolation fixture. Native input during received closing is not accepted as a reversal;')
(N/'Test-DoorLiveAI-v766.ps1').write_text(s)
print('Prepared766 initial native-AI session use regression')
