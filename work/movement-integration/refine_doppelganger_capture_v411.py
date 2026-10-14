from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-DoppelgangerNetwork-v409.ps1').read_text(encoding='utf-8-sig')
# The initial466ms phase must not be hidden by full entity enumeration.
# Keep normal50ms status sampling throughout all three body phases; collect
# slower clone object evidence after release while the native clone is alive.
s=s.replace('$cast=$false;$worldNext=0;', '$cast=$false;$worldNext=4600;')
s=s.replace('$worldNext=$w.ElapsedMilliseconds+500', '$worldNext=$w.ElapsedMilliseconds+2000')
p=N/'Test-DoppelgangerNetwork-v411.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerNetwork-v409.ps1').read_text(encoding='utf-8-sig').replace('v409','v411')
(W/'Run-DoppelgangerNetwork-v411.ps1').write_text(s,encoding='utf-8')
print('411 preserves high-frequency body observations and defers slow clone catalog reads until release')
