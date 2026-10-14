from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativeDominoCrouchSelection-v249.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$secondCast=$false')
anchor="  $scene=Q client '--midnight'"
s=s.replace(anchor,"""  if($cast-and!$secondCast-and$w.ElapsedMilliseconds-ge5500){
   $targets=Q client '--domino-targets';$targets|ConvertTo-Json -Depth 10|Set-Content "$out/targets-before-reselection.json"
   if(!$targets.valid-or$targets.targets.Count-ne1){throw 'One native selected target required before crouched reselection'}
   I client 255 1800;$secondCast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Second native LT1800ms at the same already-selected guard while crouched'})
  }
"""+anchor)
s=s.replace('foreach($variant in 19,20,22)','foreach($variant in 19,20,21,22)')
s=s.replace("if(@($strict|Where-Object {$_.valid-and$_.variant-eq21}).Count){throw 'Unimplemented crouched selection unexpectedly accepted'}", """$final=@($rawScenes|Where-Object {$_.subscene-eq'Cast_final_Select'-and$_.sceneDurationMs-eq400})
 if(!$secondCast-or$final.Count-lt8){throw 'Crouched final selection not observed in raw native scenes'}
 if(@($strict|Where-Object {$_.valid-and$_.subscene-eq'Cast_final_Select'}).Count){throw 'Unimplemented crouched final selection unexpectedly accepted'}
 $final|ConvertTo-Json -Depth 14|Set-Content "$out/crouched-final-raw-proof.json"
 $targetsAfter=Q client '--domino-targets';$targetsAfter|ConvertTo-Json -Depth 10|Set-Content "$out/targets-after-reselection.json"
 if(!$targetsAfter.valid-or$targetsAfter.targets.Count){throw 'Native crouched target reselection did not clear the single target'}
""")
s=s.replace('existing reader accepts In/Loop/Out and refuses separately recorded native Select400ms; new phase remains research-only', 'existing reader accepts ordinary crouched phases and refuses separately recorded final_Select400ms; native reselection clears the single target; new final phase remains research-only')
s=s.replace('Targeted crouched native Domino selection observation', 'Targeted crouched native Domino final-selection observation')
(P/'Observe-NativeDominoCrouchFinal-v251.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoCrouchSelectionObserve'","'DominoObserve','DominoCrouchFinalObserve','DominoCrouchSelectionObserve'")
s=s.replace("  'DominoCrouchSelectionObserve'{", "  'DominoCrouchFinalObserve'{& \"$root/work/natural-moveset/Observe-NativeDominoCrouchFinal-v251.ps1\" -Take $Take -Pitch $Pitch}\n  'DominoCrouchSelectionObserve'{");p.write_text(s,encoding='utf-8')
print('Prepared raw crouched final-selection observation v251; no production reader changes.')
