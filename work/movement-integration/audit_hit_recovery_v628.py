"""Separate read-only audit of an observed branch in a retained failed kick test."""
from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';F=N/'native-kick-emily-v626'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
r=rd(F/'result.json');assert not r['passed'] and r['failure']=='Native kick scene not selected by ordinary input'
rows=rd(F/'client-close-kick-observations.json')
owned=[x for x in rows if x['player'].get('combatSceneValid') and x['player'].get('combatScene',{}).get('variant')==15]
played=[x for x in rows if x['movement']['combatVariant']==15 and x['movement']['combatWeight']>.65 and x['movement']['weight']>.85]
assert len(owned)>=2 and len(played)>=2
assert all(x['player']['combatScene']['durationMs']==866 for x in owned)
for x in played:
 m=x['movement'];assert 0<=m['combatRecoveryStart']<=m['combatAuthoredAmount']<=1
assert all(b['movement']['combatAuthoredAmount']>=a['movement']['combatAuthoredAmount'] for a,b in zip(played,played[1:]))
end=max(x['ms']for x in owned)
assert any(x['ms']>end+400 and x['movement']['combatWeight']<.05 and x['movement']['weight']>.85 for x in rows)
out=W/'hit-recovery-audit-v628.json';assert not out.exists()
data=dict(at=datetime.datetime.now().astimezone().isoformat(),source=str(F.relative_to(R)),sourceResultSha256=sha(F/'result.json'),observationSha256=sha(F/'client-close-kick-observations.json'),dll=r['dll'],role='emily',variant=15,durationMs=866,nativeOwnedSamples=len(owned),remoteFullWeightSamples=len(played),authoredStart=played[0]['movement']['combatRecoveryStart'],authoredRange=[played[0]['movement']['combatAuthoredAmount'],played[-1]['movement']['combatAuthoredAmount']],passed=True,originalKickTestPassed=False,kickVerified=False,receiverVictimHealthVerified=False,complete=False,visualAccepted=False,scope='Read-only audit confirms observed owned right hit-recovery reaches remote body, authored phase advances monotonically without restarting and returns to locomotion. Observer sheet inspected, clear body strike/follow-through/standing recovery. Original kick test remains FAILED; receiver victim post-health assertion was not reached, no full gameplay/outcome claim. Left recovery16 remains unverified.')
out.write_text(json.dumps(data,indent=2));print(json.dumps(data))
