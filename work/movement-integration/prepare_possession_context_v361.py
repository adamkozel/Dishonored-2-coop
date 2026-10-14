from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativePossession-v322.ps1').read_text()
s=s.replace("[int]$Pitch=30)","[int]$Pitch=30,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered)")
s=s.replace('position[2]-66.4370575)-gt.03','position[2]-66.4)-gt.1')
sw=(P/'Observe-NativeSwarmContext-v275.ps1').read_text();a=sw.index(" $expectedStance=if($Stance-eq'crouched')");b=sw.index(' $hwnd=',a)
setup=sw[a:b]
a=s.index(' $hwnd=');s=s[:a]+setup+s[a:]
s=s.replace("'-t','16'","'-t','11'").replace("'15000'","'10000'").replace('ElapsedMilliseconds-lt14000','ElapsedMilliseconds-lt9000')
a=s.index(' foreach($variant in 26,27)');b=s.index(' $accepted|ConvertTo-Json',a)
s=s[:a]+''' if(@($accepted|Where-Object {$_.armHandle-le1-or$_.equipmentHandle-ne$power.handle}).Count){throw 'Accepted reader sample has wrong native owner'}
'''+s[b:]
s=s.replace("$checks.Add('Strict reader repeatedly accepts native333ms entry and200ms aim with exact Corvo arm/equipment ownership and Running0+UsePower63')","$checks.Add('Raw native scenes and accepted/rejected ownership reader samples recorded without assuming context support')")
a="  $checks.Add('Both isolated checkpoints restore original health/inventory and clear the temporary Possession acquisition')"
s=s.replace(a,'''  foreach($r in 'host','client'){
   $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
   if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
  }
'''+a)
s=s.replace("source='host';checks=$checks", "source='host';stance=$Stance;holstered=[bool]$Holstered;checks=$checks")
s=s.replace('empty-target cast only; valid-target possession, remote movement/VFX/gameplay remain unverified.','empty-target stance/equipment observation only. Existing standing human-target remote evidence is separate; no new remote context or gameplay claim.')
(P/'Observe-NativePossessionContext-v361.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();a="'PossessionObserve'{& \"$root/work/natural-moveset/Observe-NativePossession-v322.ps1\" -Take $Take -Pitch $Pitch}";assert a in s;s=s.replace(a,"'PossessionObserve'{& \"$root/work/natural-moveset/Observe-NativePossessionContext-v361.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered}");p.write_text(s)
print('361 native stance/equipment observation harness prepared; no production binding changes')
