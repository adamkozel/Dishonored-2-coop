from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoFinalSelect-v239.ps1').read_text(encoding='utf-8-sig')
s=s.replace("if($finalRows.Count-lt4-or@($finalRows|Where-Object {$_.variant-ne21-or$_.durationMs-ne400-or!$_.playerStatesValid-or!($_.activeStates-contains72)}).Count){throw 'Exact native final_Select state72/400ms not verified'}", """if($finalRows.Count-lt8-or@($finalRows|Where-Object {$_.variant-ne21-or$_.durationMs-ne400-or!$_.playerStatesValid-or$_.stance-ne0-or$_.timeMs-gt400}).Count){throw 'Exact native final_Select400ms not verified'}
   $recover=@($finalRows|Where-Object {$_.activeStates.Count-eq1-and$_.activeStates[0]-eq0-and$_.timeMs-ge150})
   if($recover.Count-lt4-or($recover.timeMs|Measure-Object -Maximum).Maximum-lt300-or!@($finalRows|Where-Object {$_.activeStates-contains72}).Count){throw 'Final scene not preserved after native casting-state retirement'}
""")
s=s.replace('Exact native400ms final_Select is observed with state72 and reciprocal owned arm/equipment;', 'Exact native400ms final_Select is observed with reciprocal owned arm/equipment both during state72 and after its native retirement past300ms;')
(P/'Test-DominoFinalRecovery-v241.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-DominoFinalSelect-v239.ps1','Test-DominoFinalRecovery-v241.ps1');p.write_text(s,encoding='utf-8')
s=(W/'Archive-DominoFinal-v237.ps1').read_text(encoding='utf-8-sig').replace('candidate-domino-final-v237','candidate-domino-recovery-v241').replace('domino-final-build-v237','domino-recovery-build-v241').replace('domino-final-identity-v237','domino-recovery-identity-v241').replace('domino-final-wire-v237','domino-recovery-wire-v241').replace('domino-final-smoke-v237','domino-recovery-smoke-v241').replace('prepare_domino_final_binding_v238.py','prepare_domino_final_recovery_v241.py').replace('Test-DominoFinalSelect-v238.ps1','Test-DominoFinalRecovery-v241.ps1')
s=s.replace('under unchanged state72/owned-arm guards.', 'under exact owned-arm/scene guards; observed final recovery may continue with ground state0 after native state72 retires, bounded to400ms. v239 supplied raw phase and native states evidence.')
s=s.replace('Candidate222 preserved.', 'Candidates222 and237 preserved.')
(W/'Archive-DominoRecovery-v241.ps1').write_text(s,encoding='utf-8')
