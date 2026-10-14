from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-CrouchBalance.ps1').read_text()
s=s.replace("$Take='crouch-balance-corvo-v1'","$Take='grip-closeup-corvo-v1'")
s=s.replace("'setviewpos -30.7 -151.8 68.1 203 0'","'setviewpos -31.8 -152.78 68.1 180 0'").replace('-Pitch 17','-Pitch 25')
s=s.replace("'-t','14'","'-t','5'")
a=s.index(";$watch.Restart();$stage=0")
b=s.index(" foreach($r in 'host','client'){if((Q $r '--health'",a)
s=s[:a]+"\n Start-Sleep -Milliseconds 1500;Pose 'standing-rendered';(Q $peer '--avatar-sword-attachment' $handles[$peer])|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 \"$out/attachment.json\"\n $checks.Add('Stable final native grip and rendered sword attachment captured beside a close game recording')\n"+s[b:]
s=s.replace("scope='Isolated full-body Posture-100 idle comparison, native crouch/forward travel/stop/standing recovery. Existing gait and first-person controls preserved. Candidate not promoted; contextual rebaking and complete visual acceptance still required.'", "scope='Close camera view of standing drawn sword grip, with stable final native joint cache and independent rendered attachment. Partial-body diagnostic view, not full-movement acceptance.'")
s=s.replace('PASS_CROUCH_BALANCE','PASS_GRIP_CLOSEUP')
(P/'Test-GripCloseup.ps1').write_text(s)
