from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativeDominoCrouch-v245.ps1').read_text(encoding='utf-8-sig')
fixture=(P/'Test-DominoFinalSelect-v239.ps1').read_text(encoding='utf-8-sig')
a=fixture.index(' $dirty=$true\n # Suspending AI');b=fixture.index(" $before=Q client '--player-domino'",a)
placement=fixture[a:b]
placement+=''' $initialTarget=$initialTargets[0];$yaw=[Math]::Atan2([double]$initialTarget.position[1]-$firstY,[double]$initialTarget.position[0]-$firstX)*180/[Math]::PI
 C client ('setviewpos {0} {1} 68.1 {2} 0'-f$firstX.ToString('F4',$ci),$firstY.ToString('F4',$ci),$yaw.ToString('F4',$ci))
 Start-Sleep -Milliseconds 600
 & "$PSScriptRoot/Aim-NativeDominoGuard-v218.ps1" -Role client -Evidence "$out/native-target-aim.json"
'''
a=s.index(" $dirty=$true;C client 'setviewpos");b=s.index(" $before=Q client '--player-domino'",a);s=s[:a]+placement+s[b:]
a=s.index('   if($AimGuard){');b=s.index('   I client 255 1800;',a);s=s[:a]+s[b:]
s=s.replace("if(@($strict|Where-Object valid).Count){throw 'Standing-only strict reader unexpectedly accepts unimplemented crouch context'}", """foreach($variant in 19,20,22){if(!@($strict|Where-Object {$_.valid-and$_.variant-eq$variant}).Count){throw 'Previously observed native crouch context regression'}}
 $selection=@($rawScenes|Where-Object {$_.subscene-eq'Cast_Select'-and$_.sceneDurationMs-eq400})
 if($selection.Count-lt4){throw 'Crouched target selection not observed in raw native scenes'}
 if(@($strict|Where-Object {$_.valid-and$_.variant-eq21}).Count){throw 'Unimplemented crouched selection unexpectedly accepted'}
 $selection|ConvertTo-Json -Depth 14|Set-Content "$out/crouched-selection-raw-proof.json"
""")
s=s.replace('current standing-only production reader correctly refuses this context','existing reader accepts In/Loop/Out and refuses separately recorded native Select400ms; new phase remains research-only')
s=s.replace('Crouched native Domino observation (no remote binding)', 'Targeted crouched native Domino selection observation (no remote binding)')
(P/'Observe-NativeDominoCrouchSelection-v249.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoCrouchNetwork'","'DominoObserve','DominoCrouchSelectionObserve','DominoCrouchNetwork'")
s=s.replace("  'DominoCrouchObserve'{", "  'DominoCrouchSelectionObserve'{& \"$root/work/natural-moveset/Observe-NativeDominoCrouchSelection-v249.ps1\" -Take $Take -Pitch $Pitch}\n  'DominoCrouchObserve'{");p.write_text(s,encoding='utf-8')
