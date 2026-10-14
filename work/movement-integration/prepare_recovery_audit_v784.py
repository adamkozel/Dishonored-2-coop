from pathlib import Path
W=Path(__file__).resolve().parent
s=(W/'audit_continuous_recovery_v781.py').read_text(encoding='utf-8-sig').replace('native-continuous-recovery-v780','native-continuous-recovery-v783').replace('candidate-recovery-v779/identity.json','candidate-death-barrier-v782/identity.json').replace('continuous-recovery-audit-v781','continuous-recovery-audit-v784')
(W/'audit_continuous_recovery_v784.py').write_text(s,encoding='utf-8')
R=W.parents[1];D=R/'documentation/handoff-2026-10-11'
p=D/'61-door-native-menu-resume.md';t=p.read_text()
old='Gameplay1 and existing native-menu states pause2, ReadNote4 and wheel13 are eligible for retention, including their pending transitions. Cinematic nodes, death, loading, unknown states and a pending load are excluded. Runtime coverage in this chapter is pause2 only; ReadNote4/wheel13 have policy/unit coverage, not new runtime acceptance.'
assert old in t
t=t.replace(old,'Candidate769 retained Gameplay1, pause2, ReadNote4 and state13, including pending transitions. **Correction from the subsequent death test:** state13 is native GameOver, not the weapon wheel; the old label and assertion that all death states were excluded were wrong. Candidate779 removes13 from ordinary-menu retention, and782 handles pending/committed death through the separate checkpoint barrier. Candidate769 runtime coverage in this chapter remains pause2 only. State8 was observed during the native weapon wheel in older66 evidence, but wheel retention is still not runtime accepted. See [continuous recovery](62-continuous-checkpoint-death-recovery.md).')
p.write_text(t)
p=D/'52-native-pickups-and-readable-recovery.md';t=p.read_text();t=t.replace('The prior fixture and movement-resume policies covered pause2 and wheel13 only.','The prior fixture and movement-resume policies covered pause2 and state13 only. The label "wheel13" was later found incorrect:13 is native GameOver. Candidate779 removes it; see [the correction and checkpoint barrier](62-continuous-checkpoint-death-recovery.md).')
p.write_text(t)
print('Prepared independent784 audit and corrected historical state labels')
