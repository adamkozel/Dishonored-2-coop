"""Keep the v151 observation, but release LT early enough to test air release."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeMesmerizeAir-v150.ps1').read_text(encoding='utf-8-sig')
s=s.replace('-Button A -LeftTrigger 255 -DurationMs 600','-Button A -LeftTrigger 255 -DurationMs 180')
s=s.replace('LT600ms','LT180ms').replace('crouch provenance','air provenance').replace('rejects crouched Mesmerize','rejects airborne Mesmerize').replace('Raw native crouched','Raw native airborne')
at=" $checks.Add('Native cast phases retain reciprocal arm binding during bounded jump input; raw player/scene overlap is recorded rather than assumed')"
assert s.count(at)==1
s=s.replace(at,""" $airRelease=@($airRows|Where-Object {@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-eq'midnight/gameplay/player/base/powers/power_mesmerize_cast_out.midnightscene'-and$_.sceneDurationMs-eq666-and@($_.bindingsResearch).Count-eq1-and$_.bindingsResearch[0].reciprocal}).Count})
 if(!$airRelease.Count){throw 'No directly sampled native airborne release with reciprocal left-arm scene; do not infer release from inputs'}
 $checks.Add('Direct native Jump/Fall samples overlap the exact666ms reciprocal Mesmerize release scene')
"""+at)
(P/'Observe-NativeMesmerizeAirRelease-v157.ps1').write_text(s,encoding='utf-8')
