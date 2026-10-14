from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-DoppelgangerFx-v450.ps1').read_text(encoding='utf-8-sig')
old=' $sent=@($rows|ForEach-Object {$_.cloneSource.history}|Sort-Object sequence -Unique)';assert s.count(old)==1
new=''' # Receiver status may publish two packets after the last sender status read.
 # Capture a sender tail after all sampled receiver histories, before cleanup.
 $senderTail=S client '.doppelganger-world.json';$senderTail|ConvertTo-Json -Depth 12|Set-Content "$out/sender-history-tail.json"
 $sent=@(@($rows|ForEach-Object {$_.cloneSource.history})+@($senderTail.history)|Sort-Object sequence -Unique)'''
s=s.replace(old,new)
p=N/'Test-DoppelgangerFx-v453.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerFx-v450.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-fx-v450','doppelganger-fx-v453').replace('Test-DoppelgangerFx-v450','Test-DoppelgangerFx-v453');(W/'Run-DoppelgangerFx-v453.ps1').write_text(s,encoding='utf-8')
print('453 captures the exact final sender history; source/peer effect checks retained')
