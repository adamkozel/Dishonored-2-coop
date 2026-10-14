from pathlib import Path
W=Path(__file__).resolve().parent
for kind in ('Build','Deploy'):
    s=(W/f'{kind}-Recovery-v779.ps1').read_text(encoding='utf-8-sig').replace('Recovery','DeathBarrier').replace('recovery','death_barrier').replace('779','782')
    s=s.replace('candidate-death_barrier','candidate-death-barrier')
    (W/f'{kind}-DeathBarrier-v782.ps1').write_text(s,encoding='utf-8')
s=(W/'stage_recovery_v779.py').read_text(encoding='utf-8-sig').replace("A=W/'candidate-recovery-v779'; P=W/'candidate-door-menu-v769'","A=W/'candidate-death-barrier-v782'; P=W/'candidate-recovery-v779'")
s=s.replace('Recovery-v779','DeathBarrier-v782').replace('stage_recovery_v779','stage_death_barrier_v782').replace('-v779.log','-v782.log')
a=s.index("scope='");b=s.index("')\n(A/",a)
s=s[:a]+"scope='Native pending or committed GameOver13 on either peer blocks door actions before the reliable death protocol becomes active, retaining only session intent. Failed/disabled checkpoint sessions do not retain. Candidate779 state-label correction and isolated note-restoration helper retained. Runtime continuous recovery pending."+s[b:]
(W/'stage_death_barrier_v782.py').write_text(s,encoding='utf-8')
p=W.parent/'natural-moveset/Test-ContinuousRecovery-v778.ps1';s=p.read_text(encoding='utf-8-sig')
old=";if($g.logic-ne13-or$g.pending";assert s.count(old)==1
s=s.replace(old,';Save "$cycleDir/death-screen-$r.json" $screens[$r];if($g.logic-ne13-or$g.pending')
p.write_text(s,encoding='utf-8')
print('Prepared candidate782; added per-role death evidence before assertions')
