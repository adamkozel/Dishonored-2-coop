from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeCrouchCombat-v613.ps1').read_text()
s=s.replace('NativeCrouchCombat613','NativeKick623').replace('candidate-combat-target-v610','candidate-crouch-stance-v620').replace('archived610','archived620')
s=s.replace("name='crouch-combat';yaw=90;stance=1","name='close-combat';yaw=90;stance=0")
s=s.replace('@(@(2,.75),@(2,-.75),@(2,2)', '@(@(2,2)')
start=s.index('    $objects=@(');end=s.index('    $samples.Add',start);s=s[:start]+s[end:]
s=s.replace(';sensors=$objects;midnight=(Q $source \'--midnight\')','').replace(';sourceStream=(Get-Content "$root/work/lab-instances/$source/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)','').replace('Start-Sleep -Milliseconds 80','Start-Sleep -Milliseconds 30')
s=s.replace('Ordinary native crouched sword combat not observed','Ordinary native close-range combat not observed').replace('Ordinary crouched RT native combat observed; exact scene contract needs inspection','Ordinary close-range RT native combat observed; exact scene contract needs inspection')
s=s.replace('Native crouched RT250ms facing guard2; source-only combat context research, no new binding claim.','Ordinary close-range RT250ms facing guard2; reproduce611 native kick context if selected, no forced scene or new binding claim.')
s=s.replace('Source-only native crouched targeted combat observation','Source-only native close-range combat observation').replace('No new remote crouched combat binding','No new remote kick binding').replace('PASS_NATIVE_CROUCH_COMBAT','PASS_NATIVE_CLOSE_COMBAT')
(N/'Observe-NativeKick-v623.ps1').write_text(s)
print('Prepared623 ordinary input replay of611 geometry; source observation only.')
