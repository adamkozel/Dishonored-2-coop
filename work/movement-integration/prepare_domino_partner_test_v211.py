from pathlib import Path
R=Path(__file__).resolve().parents[2];p=R/'work/natural-moveset'
s=(p/'Observe-NativeDominoTargets-v199.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$dirty=$true;C client 'setviewpos -48.59 -155.07 68.1 90 0'","$dirty=$true;C client 'setviewpos -33.5 -153 68.1 90 0';C host 'setviewpos -33.5 -150 68.1 270 0'")
s=s.replace(" $manaBefore=&", " $filterBefore=S client '.domino-partner-filter.json';$filterBefore|ConvertTo-Json|Set-Content \"$out/filter-before.json\"\n if(!$filterBefore.installed){throw 'Native Domino picker filter is not installed'}\n $manaBefore=&",1)
s=s.replace(" $hwnd=(Get-Process", " Start-Sleep -Milliseconds 900\n $partner=S client;$partner|ConvertTo-Json -Depth 6|Set-Content \"$out/partner-at-aim.json\"\n if([Math]::Abs([double]$partner.probeRenderPosition[0]+33.5)-gt.15-or[Math]::Abs([double]$partner.probeRenderPosition[1]+150)-gt.15){throw 'Partner is not in the intended native aiming corridor'}\n $hwnd=(Get-Process",1)
old=""" if($markedRows.Count-lt10){throw 'Insufficient strictly owned native Domino target/mark observations'}
 if(@($markedRows|Where-Object {$_.targets.Count-ne1}).Count){throw 'Single-target experiment has an unexpected target count'}
 $checks.Add('Strict reader verifies equipment-derived native network, bounded selected-node pool, reciprocal NPC animation generation and the exact temporary mark owner in at least ten real samples')"""
new=""" $filter=S client '.domino-partner-filter.json';$filter|ConvertTo-Json|Set-Content \"$out/filter-after.json\"
 $validTargets=@($targetRows|Where-Object {$_.valid-and$_.present})
 [ordered]@{samples=$targetRows.Count;validPresent=$validTargets.Count;blocked=$filter.blocked-$filterBefore.blocked;considered=$filter.considered-$filterBefore.considered;readFailed=$filter.readFailed-$filterBefore.readFailed;cacheHit=$filter.cacheHit-$filterBefore.cacheHit;cacheMiss=$filter.cacheMiss-$filterBefore.cacheMiss;readMaxUs=$filter.readMaxUs}|ConvertTo-Json|Set-Content \"$out/partner-exclusion-summary.json\"
 if($filter.blocked-le$filterBefore.blocked-or$filter.readFailed-ne$filterBefore.readFailed){throw 'Native partner picker was not cleanly blocked'}
 if($validTargets.Count-lt50-or@($validTargets|Where-Object {@($_.targets).Count}).Count){throw 'Partner aim must retain strictly valid empty native target network'}
 if(@($rows|ForEach-Object {$_.effects.effects}|Where-Object {$null-ne$_}).Count){throw 'Partner aim spawned native Domino target effects'}
 foreach($row in $rows){$c8=($row.component.words|Where-Object offset -eq '0xc8').word;if($c8-and(([Convert]::ToUInt64($c8.Substring(2),16)-band0xffffffff)-eq$partner.probeActorHandle)){throw 'Partner became native equipment target'} }
 if($manaAfter.currentCandidate-lt$manaBefore.currentCandidate-.01){throw 'Excluded partner consumed native mana'}
 $checks.Add('Native picker considered and blocked exact partner before equipment target or aim FX; at least50 strict network observations retain zero selected targets and mana is unchanged')"""
assert old in s;s=s.replace(old,new)
s=s.replace("scope='Initial native Domino observation:","scope='Bounded native Domino partner aim exclusion; no claim about linked damage or general NPC synchronization. Initial native Domino observation:")
(p/'Test-DominoPartner-v211.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig')
s=s.replace("'DominoWorld','DominoNetwork'","'DominoWorld','DominoPartner','DominoNetwork'")
key="  'DominoWorld'{";assert key in s
s=s.replace(key,"  'DominoPartner'{& \"$root/work/natural-moveset/Test-DominoPartner-v211.ps1\" -Take $Take -Pitch $Pitch}\n"+key)
f.write_text(s,encoding='utf-8')
