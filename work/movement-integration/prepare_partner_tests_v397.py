from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
p=P/'Test-PartnerSwordContexts-v397.ps1';assert not p.exists()
s=(P/'Test-PartnerSwordContexts-v392.ps1').read_text(encoding='utf-8-sig').replace('partner-sword-contexts-host-v392','partner-sword-contexts-host-v397').replace("if($dll-ne'440B0F9FD8BE7D24D22B62DE0BD7FB43C2E9C85FE6AB3551DA9A988AA273C6CD'){throw 'Expected archived387 candidate'}","$expected=Get-Content \"$root/work/movement-integration/candidate-partner-interaction-v396/identity.json\" -Raw|ConvertFrom-Json;if($dll-ne$expected.dll){throw 'Expected archived396 candidate'}")
s=s.replace('$watch=[Diagnostics.Stopwatch]::StartNew();', '$interactionBefore=Get-Content "$root/work/lab-instances/$source/coop-status.json.interaction-partner-filter.json" -Raw|ConvertFrom-Json;if(!$interactionBefore.installed){throw "Interaction filter missing"}\n   $watch=[Diagnostics.Stopwatch]::StartNew();')
anchor='if(!$filterAfter.installed-or$filterAfter.blocked-le$filterBefore.blocked-or$filterAfter.readFailed-ne$filterBefore.readFailed)'
idx=s.index(anchor)
s=s[:idx]+'''$interactionAfter=Get-Content "$root/work/lab-instances/$source/coop-status.json.interaction-partner-filter.json" -Raw|ConvertFrom-Json
   [ordered]@{before=$interactionBefore;after=$interactionAfter}|ConvertTo-Json -Depth 6|Set-Content "$out/$key-interaction-filter.json"
   if(!$interactionAfter.installed-or$interactionAfter.fatalityBlocked-le$interactionBefore.fatalityBlocked-or$interactionAfter.swordBlocked-le$interactionBefore.swordBlocked-or$interactionAfter.readFailed-ne$interactionBefore.readFailed){throw 'Both exact partner sword/fatality picks not rejected consistently'}
   '''+s[idx:]
s=s.replace('Scoped partner sword contexts on387:','Scoped partner sword contexts on396:')
s=s.replace('Standing/crouched native sword slash39 preserved while exact partner targeting is rejected;','Standing/crouched native sword slash39 preserved while both exact partner sword/fatality getters reject;')
p.write_text(s,encoding='utf-8')
print('Prepared both-role sword-context test397')
