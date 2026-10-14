"""Separate compact-release feasibility assets; never aliases the soft drop.

Native victim animation ends at166ms without an explicit ragdoll event.
The fixture will test that endpoint as a bounded handoff, not a verified event.
"""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'build_carry_release_scenes.py').read_text()
s=s.replace("[('drop_low_stand',4,656),('drop_low_sneak',6,500)]", "[('drop_fetal_pose',5,166)]")
s=s.replace("assert len(events)==1 and events[0]['m_startTime']==ragdoll", "assert not events\n    anims=[o for o in walk(victim) if 'm_keyProp' in o and o['m_keyProp']['m_class']=='arkMidnightKeyPropAnim']\n    assert len(anims)==1 and anims[0]['m_duration']==166 and anims[0]['m_keyProp']['m_object']['m_AnimName']=='generic_carrycorpse_dropfoetal'")
s=s.replace('ragdoll_ms=ragdoll,','candidate_handoff_ms=ragdoll,explicit_native_ragdoll_event=False,')
s=s.replace('carry-release-scenes-v1.json','carry-fetal-scenes-v35.json')
s=s.replace("s=s.replace('carry-victim','carry-release')", "s=s.replace('carry-victim','carry-fetal')")
s=s.replace('Build-D2CarryReleaseAssets.ps1','Build-D2CarryFetalAssets.ps1')
exec(compile(s,str(P/'build_fetal_release_v35.py'),'exec'))
