"""Add native-observed holstered air cases to the unchanged recovery harness."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeAirNetwork-v170.ps1').read_text(encoding='utf-8-sig')
a="[string]$AirCase='entry')\n$Stance='standing';$Holstered=$false;$Move=0"
assert s.count(a)==1
s=s.replace(a,"[string]$AirCase='entry',[switch]$Holstered)\n$Stance='standing';$Move=0")
s=s.replace('Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam.', 'Native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam; the holstered field distinguishes empty-hand and sword contexts.')
(P/'Test-MesmerizeAirNetwork-v178.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
a='Test-MesmerizeAirNetwork-v170.ps1" -Take $Take -Pitch $Pitch -AirCase $AirCase'
assert s.count(a)==1
runner.write_text(s.replace(a,'Test-MesmerizeAirNetwork-v178.ps1" -Take $Take -Pitch $Pitch -AirCase $AirCase -Holstered:$Holstered'),encoding='utf-8')
