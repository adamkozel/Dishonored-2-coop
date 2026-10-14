from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(W/'archive_shadowwalk_sneak_v550.py').read_text(encoding='utf-8-sig')
s=s.replace("A=W/'candidate-shadowwalk-sneak-v550'","A=W/'candidate-unbound-presentation-v558'").replace("previous=W/'candidate-shadowwalk-sneak-v548'","previous=W/'candidate-shadowwalk-sneak-v550'")
s=s.replace('v550.log','v558.log').replace('protocol=58','protocol=59')
a=s.index("    scope='");b=s.index("\n    banks=",a)
s=s[:a]+"    scope='Explicit state18 for unbound grounded presentation. Known native physical standing/crouching and owned stable sword are revalidated independently; incomplete action fields are discarded. Unsupported gesture cannot stop otherwise valid movement. Air, alternate modes, invalid owners, world changes and timeouts remain rejected. Ground bank59/336 unchanged. Bounded6000ms test command suppresses only known cosmetic equipment binding for an isolated integration test. Full moveset and visual acceptance remain incomplete.',"+s[b:]
a=s.index('for p in [Path(__file__)');b=s.index('\nfor name in',a)
s=s[:a]+"for p in [Path(__file__),W/'prepare_unbound_presentation_v558.py',W/'prepare_unbound_runtime_v559.py',W/'Build-UnboundPresentation-v558.ps1',*logs]:shutil.copy2(p,A/p.name)"+s[b:]
(W/'archive_unbound_presentation_v558.py').write_text(s,encoding='utf-8')
for old,new in [('Stage-ShadowWalkSneak-v550.ps1','Stage-UnboundPresentation-v558.ps1'),('Deploy-ShadowWalkSneak-v550.ps1','Deploy-UnboundPresentation-v558.ps1')]:
 s=(W/old).read_text(encoding='utf-8-sig').replace('archive_shadowwalk_sneak_v550.py','archive_unbound_presentation_v558.py').replace('shadowwalk-sneak-v550','unbound-presentation-v558').replace('ShadowWalkSneakDeploy550','UnboundPresentationDeploy558')
 (W/new).write_text(s,encoding='utf-8')
s=(N/'Test-EquipmentReview-v48.ps1').read_text(encoding='utf-8-sig').replace("'equipment-review-v48'","'unbound-presentation-v559'")
def once(a,b):
 global s
 assert s.count(a)==1,(a[:100],s.count(a));s=s.replace(a,b)
once("$rows=[Collections.Generic.List[object]]::new();", "Copy-Item $PSCommandPath \"$out/run-script.ps1\"\n$identity=Get-Content \"$root/work/movement-integration/candidate-unbound-presentation-v558/identity.json\" -Raw|ConvertFrom-Json\nif((Get-FileHash \"$root/work/asset-game/dinput8.dll\").Hash-ne$identity.dll-or(Get-FileHash \"$lab/bin/d2_runtime_probe.exe\").Hash-ne$identity.probe){throw 'Pinned558 candidate required'}\n$rows=[Collections.Generic.List[object]]::new();")
a=s.index("  $role=if($r-eq'host')");b=s.index("  if((Q $r '--health'",a)
s=s[:a]+'''  $role=if($r-eq'host'){'emily'}else{'corvo'}
  $path="$PSScriptRoot/shadowwalk-sneak-banks-v548/$role-shadowwalk-sneak-bank.bin"
  Copy-Item -LiteralPath $path -Destination "$root/work/lab-instances/$r/coop-status.json.movement.bin"
'''+s[b:]
s=s.replace('$m.version-eq41','$m.version-eq59')
once('stream=$s;native=', 'stream=$s;sourceStream=(S $src);native=')
once("    I $src X $(if($held){650}else{1500});", "    $fallbackBefore=(S $src).unboundSamples\n    C $src $(if($held){'coop_shadow_unbound_probe off'}else{'coop_shadow_unbound_probe 6000'})\n    I $src X $(if($held){650}else{1500});")
a=s.index('    $active=@($samples|Where-Object');b=s.index('    $eq=Q $dst',a)
s=s[:a]+'''    if($held){
     $active=@($samples|Where-Object {$_.movement.equipmentVariant-eq$variant-and$_.movement.equipmentWeight-gt.8-and$_.movement.weight-gt.9})
     if(!$active.Count){throw "Native authored draw did not recover $src stance $stance"}
    }else{
     $active=@($samples|Where-Object {$_.stream.remoteState-eq18-and$_.movement.weight-gt.9-and$_.sourceStream.unboundSamples-gt$fallbackBefore})
     if(!$active.Count){throw "Explicit Steam unbound fallback not observed $src stance $stance"}
     if(@($active|Where-Object {$_.movement.equipmentVariant-or$_.movement.powerVariant-or$_.stream.remoteVariant-or$_.stream.remotePhase}).Count){throw 'Unbound packet retained partial action fields'}
     C $src 'coop_shadow_unbound_probe off'
    }
'''+s[b:]
once('$checks.Add("${src}: stance $stance $trial uses native scene and authored contact; settles to matching native sword state and independent visibility flag")',
 '$checks.Add("${src}: stance $stance $trial $(if($held){\'returns to existing native authored binding\'}else{\'deliberately missing binding uses explicit Steam state18 and continuous ground body\'}); settles to matching native sword and visibility")')
once(" foreach($rec in $captures){if(!$rec.HasExited){$rec.WaitForExit(50000)|Out-Null}}", ''' foreach($rec in $captures){if(!$rec.HasExited){$rec.WaitForExit(50000)|Out-Null}}
 if($pids.Count-eq2){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (B $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed after restore'}}
  $checks.Add('Both isolated checkpoints restore; all14 protected save/settings hashes retained')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}''')
s=s.replace("bankManifest='outofmana-context-bank-manifest-v46.json'","bankManifest='shadowwalk-sneak-bank-manifest-v548.json';probe=$identity.probe")
once("scope='Native source input, strict equipment scene, Steam timing, authored body and native clone sword. Visual hand contact and broader unarmed special actions remain unaccepted.'", "scope='Both-role standing/crouching missing-equipment-binding fault injection. Native X input and verified ground/sword ownership continue; explicit Steam state18 preserves existing locomotion and known draw binding recovers. Four holstered directions sampled. Does not establish complete unsupported-action coverage or visual acceptance. Native first person untouched; checkpoint/protected saves restored.'")
s=s.replace("C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'","C $r 'coop_shadow_unbound_probe off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'")
(N/'Test-UnboundPresentation-v559.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkSneak-v551.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-sneak-v551','unbound-presentation-v559').replace(',[switch]$Standing','').replace('ShadowWalkSneak551','UnboundPresentation559').replace('Test-ShadowWalkSneak-v551','Test-UnboundPresentation-v559').replace(' -Standing:$Standing','')
(W/'Run-UnboundPresentation-v559.ps1').write_text(s,encoding='utf-8')
print('Prepared558 preservation/deploy and batched both-role standing/crouched fault-and-recovery test559')
