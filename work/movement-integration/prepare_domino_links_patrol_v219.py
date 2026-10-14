from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDominoLinks-v217.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')","$_.spawnerName-eq'ld_spawner_guard_tutorial_2'")
s=s.replace("-and[Math]::Abs([double]$_.position[2]-61.0278)-lt.5", "-and[double]$_.position[2]-ge60.5-and[double]$_.position[2]-le67-and[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)-lt400")
s=s.replace('Exact lower-floor tutorial guard unavailable','Exact tutorial guard1 outside observed upper/lower patrol area')
s=s.replace('$dx=[double]$target2.position[0]+44.39;$dy=[double]$target2.position[1]+153.27', '$dx=[double]$target2.position[0]+48.59;$dy=[double]$target2.position[1]+155.07')
s=s.replace("if($pitch2-lt0-or$pitch2-gt40)","if($pitch2-lt-15-or$pitch2-gt40)")
s=s.replace("('setviewpos -44.39 -153.27 68.1 {0} 0'-f$yaw2", "('setviewpos -48.59 -155.07 68.1 {0} 0'-f$yaw2")
s=s.replace('Native controller aim from unchanged upper balcony source; no NPC relocation or spawn','Source-only observation moves to the separately proven original balcony anchor for tutorial guard1; bounded current native position supplies pitch. No stream active, no NPC relocation or spawn.')
s=s.replace('Second native LT hold1800ms toward lower tutorial guard','Second native LT hold1800ms toward tutorial guard1 at its current native patrol position')
(P/'Observe-NativeDominoLinks-v219.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=runner.read_text(encoding='utf-8-sig');assert 'Observe-NativeDominoLinks-v217.ps1' in s
runner.write_text(s.replace('Observe-NativeDominoLinks-v217.ps1','Observe-NativeDominoLinks-v219.ps1'),encoding='utf-8')
