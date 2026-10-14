"""Extend the constant native compact pose across physics handoff, clone only."""
import json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;L=P.parents[1]/'outputs/d2-coop-lab'
source=L/'assets/carry_victim_drop_fetal_pose.midnightscene'
assert hashlib.sha256(source.read_bytes()).hexdigest().upper()=='F75A1098E7FE1C3BF7D14CC906D58867EB015726A26211885C35F0AFB58DF4BF'
s=source.read_text()
assert s.count('m_duration = 166;')==2 and s.count('m_isStretched = false;')==1
s=s.replace('m_duration = 166;','m_duration = 666;').replace('m_isStretched = false;','m_isStretched = true;').replace('D2Coop_Victim_drop_fetal_pose','D2Coop_Victim_drop_fetal_hold')
assert 'EMTT_EVENT' not in s
out=L/'assets/carry_victim_drop_fetal_hold.midnightscene';out.write_text(s)
install=(L/'Build-D2CarryFetalAssets.ps1').read_text(encoding='utf-8-sig').replace('carry-fetal','carry-compact-hold-v38').replace('carry_victim_drop_fetal_pose','carry_victim_drop_fetal_hold')
(L/'Build-D2CarryCompactHoldAssets.ps1').write_text(install)
manifest=dict(source='carry_victim_drop_fetal_pose.midnightscene',generated=str(out),sha256=hashlib.sha256(out.read_bytes()).hexdigest().upper(),
 native_clip='generic_carrycorpse_dropfoetal',native_duration_ms=166,hold_duration_ms=666,stretched=True,
 justification='All six native source frames have the same compact pose; keep it available while mode2 transitions to ragdoll',
 events=False,visual_accepted=False,complete=False)
(P/'compact-hold-v38.json').write_text(json.dumps(manifest,indent=2));print(json.dumps(manifest))
