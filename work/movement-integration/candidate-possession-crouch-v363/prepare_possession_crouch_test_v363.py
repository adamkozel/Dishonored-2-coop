from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionNetwork-v323.ps1').read_text()
s=s.replace('66.4370575)-gt.03','66.4)-gt.1')
s=s.replace('possession-bank-manifest-v323','possession-crouch-bank-manifest-v363').replace('possession-banks-v323/corvo-possession-bank.bin','possession-crouch-banks-v363/corvo-possession-crouch-bank.bin')
s=s.replace('version-eq51','version-eq53').replace('version-ne51','version-ne53').replace('bank51/315','bank53/318').replace('protocol48','protocol50')
a=" $bankPin=Get-Content"
assert s.count(a)==1
s=s.replace(a,''' & "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500
 $context=Q host '--player-states';$context|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne1-or!$context.motionValid-or$context.motion-ne1-or!(Q host '--player-sword').equipped){throw 'Exact native armed crouch not established'}
 $checks.Add('Native Sneaking6 and armed sword verified before crouched cast')
'''+a)
s=s.replace("'Remote body recovers to standing with native sword selection retained'","'Remote body recovers to crouched idle with native sword selection retained'")
s=s.replace("'Remote cast did not recover to standing'","'Remote cast did not recover to crouched idle'")
a=" $sword=Q client '--avatar-sword'"
assert s.count(a)==1
s=s.replace(a,''' $low=@($rows|Where-Object {$_.movement.powerVariant-in26,27-and$_.movement.powerWeight-gt.5})
 if(!$low.Count-or@($low|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.95}).Count){throw 'Crouched source/remote stance diverged during cast'}
 $checks.Add('Both native source and remote body remain crouched throughout sampled cast phases')
'''+a)
a="  $checks.Add('Both isolated checkpoints restore original health/inventory and clear the native Possession acquisition')"
assert a in s
s=s.replace(a,'''  foreach($r in 'host','client'){
   $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
   if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
  }
'''+a.replace('original health/inventory','complete original health/inventory'))
s=s.replace("scope='Standing armed native Corvo", "scope='Crouched armed native Corvo")
s=s.replace('Valid-target transfer, disappearance/reappearance, VFX, alternate contexts and visual acceptance remain pending.', 'Standing human-target lifecycle and target particles have separate evidence. This take tests crouched empty-target entry/aim and recovery only; native draw overlap, crouched successful-host release, shared NPC authority and overall visual acceptance remain pending.')
(P/'Test-PossessionCrouchNetwork-v363.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();s=s.replace("'PossessionFxObserve','PossessionObserve'","'PossessionFxObserve','PossessionCrouchNetwork','PossessionObserve'")
a="  'PossessionObserve'{"
assert s.count(a)==1
s=s.replace(a,"  'PossessionCrouchNetwork'{& \"$root/work/natural-moveset/Test-PossessionCrouchNetwork-v363.ps1\" -Take $Take -Pitch $Pitch}\n"+a)
p.write_text(s)
print('Crouched empty-target runtime test prepared')
