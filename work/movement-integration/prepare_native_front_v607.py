"""Next bounded context: ordinary RT while facing the same native guard."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeLethal-v596.ps1').read_text().replace('NativeLethal596','NativeFront607').replace("name='rear-lethal'","name='front-lethal'").replace('candidate-choke-breakout-v592','candidate-lethal-v604').replace('archived592','archived604').replace('choke-breakout-bank-manifest-v592','lethal-bank-manifest-v604').replace('version-eq64','version-eq65').replace('version-ne64','version-ne65')
for a,b in [('-1.15*$forwardX','+1.15*$forwardX'),('-1.15*$forwardY','+1.15*$forwardY'),('[Math]::Atan2($forwardY,$forwardX)','[Math]::Atan2(-$forwardY,-$forwardX)'),('-.9*$fx','+.9*$fx'),('-.9*$fy','+.9*$fy'),('-.85*$fx','+.85*$fx'),('-.85*$fy','+.85*$fy'),('[Math]::Atan2($fy,$fx)','[Math]::Atan2(-$fy,-$fx)')]:
 assert a in s,a;s=s.replace(a,b)
s=s.replace('sensors=$objects;midnight=',"sword=(Q $source '--player-sword');sensors=$objects;midnight=")
s=s.replace('behind exact authored guard2','in front of exact authored guard2').replace('behind native guard','in front of native guard').replace('rear assassination','front assassination')
s=s.replace('Source-only native lethal takedown observation','Source-only native frontal lethal takedown observation')
(N/'Observe-NativeFront-v607.ps1').write_text(s);print('Native607 front-context observation prepared; no assumption about resulting scene.')
