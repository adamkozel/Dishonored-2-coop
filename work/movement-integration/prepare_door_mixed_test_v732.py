from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-DoorCatalog-v729.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-catalog-corvo-v729','native-door-mixed-host-first-v732')
rep('NativeDoorCatalog729','NativeDoorMixed732')
rep("   I $source;I $peer", "   if($initialOpen){throw 'Mixed-use test requires the verified closed closet door'}\n   I $source;I $peer")
rep("   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}", """   $otherCharacter=if($peer-eq'host'){'corvo'}else{'emily'}
   $otherEntry=Get-Content "$PSScriptRoot/heavy-bank-manifest-v633.json" -Raw|ConvertFrom-Json|Where-Object role -eq $otherCharacter
   $otherBank=Join-Path $PSScriptRoot $otherEntry.bank;if((Get-FileHash $otherBank).Hash-ne$otherEntry.sha256){throw 'Second bank hash differs'}
   Copy-Item $otherBank "$root/work/lab-instances/$source/coop-status.json.movement.bin" -Force;C $source 'coop_movement_bank on'
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}""")
rep("$duplicateClose=$false;$inputs=", "$duplicateClose=$false;$swapped=$false;$inputs=")
rep("    if(!$second-and$watch.ElapsedMilliseconds-ge7000){I $source X 200;", """    if(!$swapped-and$watch.ElapsedMilliseconds-ge5800){
     if((DoorSample $source).current-ne2-or(DoorSample $peer).current-ne2){throw 'Both doors must be open before changing operator'}
     I $source;I $peer;C $source 'setviewpos -44 -161 68.1 220 0';C $peer 'setviewpos -47.05 -163.5 68.1 270 0'
     & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 0 -Evidence "$out/second-operator-camera.json" -Quick
     $swapped=$true
    }
    if(!$second-and$watch.ElapsedMilliseconds-ge7600){if(!$swapped){throw 'Operator handoff missing'};I $peer X 200;""")
rep("if(!$duplicateClose-and$watch.ElapsedMilliseconds-ge10000){C $source", "if(!$duplicateClose-and$watch.ElapsedMilliseconds-ge11000){C $peer")
rep("input=$(if($initialOpen){'native close'}else{'native open'});attempt=1", "input='first operator native open';role=$source;attempt=1")
rep("input=$(if($initialOpen){'native open'}else{'native close'});attempt=2", "input='second operator native close';role=$peer;attempt=2")
rep('$ds.nativeCalls-ne0-or$dp.nativeCalls-ne2', '$ds.nativeCalls-ne1-or$dp.nativeCalls-ne1')
rep('$dp.duplicates-lt2', '$dp.duplicates-lt1-or$ds.duplicates-lt1')
rep("if($Source-eq'client'-and($ds.requests-ne2-or$dp.requests-ne2)){throw 'Expected two client requests ordered by host'}", "if($ds.requests-ne1-or$dp.requests-ne1){throw 'Expected one client request ordered by host across the operator handoff'}")
rep('Two ordinary native X interactions replicate both selected door cycles; exactly two remote native calls; duplicate packets do not reapply; no rejected packets during measured actions; both finish host revision3', 'Two different players operate one native door in the same arm: both cycles, one received native call each, one client request, host revision3, replays do not reapply, no measured rejection delta')
rep('Native Steam authored ordinary-door catalog: six eligible channels, four excluded doors, selected open/close cycle via native X, other five unchanged. Locks/scripts/other maps remain unverified.', 'Native Steam mixed-player door interaction in one unchanged arm: first player opens, second closes, one receiver call per process, other five authored channels unchanged. Not simultaneous-conflict or checkpoint-interruption coverage.')
(R/'work/natural-moveset/Test-DoorMixed-v732.ps1').write_text(s,encoding='utf-8')
print('Created mixed-player door test732')
