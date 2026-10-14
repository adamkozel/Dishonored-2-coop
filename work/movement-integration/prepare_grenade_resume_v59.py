"""Derive an isolated menu recovery experiment from the pinned grenade harness."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]
src=R/'work/natural-moveset/Test-GrenadeNetwork-v56.ps1'
dst=src.with_name('Test-GrenadeResume-v59.ps1')
s=src.read_text(encoding='utf-8-sig')
def replace(a,b):
    global s
    assert s.count(a)==1,(a,s.count(a))
    s=s.replace(a,b)
replace("[ValidateSet('none','receiver-stop','source-pause','receiver-pause')][string]$Interrupt='none'", "[ValidateSet('source-pause','receiver-pause')][string]$Interrupt='source-pause'")
replace("if($Interrupt-ne'none'-and!$ValidateFx){throw 'Interruption requires effect validation'}", "if(!$ValidateFx-or!$ValidateMechanism){throw 'Menu recovery experiment requires effect and mechanism validation'}")
replace("$initialDll=(Get-FileHash", "$resumeBefore=@{};$moved=$false\n$initialDll=(Get-FileHash")
replace("$ms=100,$lt=0)", "$ms=100,$lt=0,$y=0)")
replace("-LeftTrigger $lt|Out-Null", "-LeftTrigger $lt -LeftY $y|Out-Null")
replace(" C $peer 'coop_movement_bank on';$bankWait=", " # Keep the opposite-role bank ready on disk; its presentation starts only after the finite source grant.\n $other=Get-Content \"$PSScriptRoot/grenade-bank-manifest-v56.json\" -Raw|ConvertFrom-Json|Where-Object role -ne $character\n $otherBank=Join-Path $PSScriptRoot $other.bank;if((Get-FileHash $otherBank).Hash-ne$other.sha256){throw 'Source bank identity differs'}\n Copy-Item $otherBank \"$root/work/lab-instances/$Source/coop-status.json.movement.bin\" -Force\n C $peer 'coop_movement_bank on';$bankWait=")
replace(" foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}", " C $Source 'coop_movement_bank on'\n foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}")
replace(" $checks.Add('Native Steam handshake and exact candidate bank drive the remote held grenade without a prop command')", """ $checks.Add('Native Steam handshake and exact candidate bank drive the remote held grenade without a prop command')
 foreach($r in 'host','client'){
  $w.Restart();do{$b=S $r '.movement.json';$s=S $r '.shadow-stream.json';if($b.active-and$b.version-eq42-and$s.ready){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt3000)
  if(!$b.active-or$b.version-ne42-or!$s.ready){throw 'Both live candidate banks required before arming menu recovery'}
  C $r 'coop_movement_resume on';$w.Restart();do{$a=S $r '.movement-resume.json';if($a.enabled-and$a.phase-eq1){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt1000)
  if(!$a.enabled-or$a.phase-ne1){throw 'Native menu recovery did not arm'}
  $resumeBefore[$r]=[ordered]@{resume=$a;stream=(S $r '.shadow-stream.json');bridge=(S $r);bank=(S $r '.movement.json')}
 }
 $resumeBefore|ConvertTo-Json -Depth 10|Set-Content "$out/resume-before.json"
 $checks.Add('Both opposite-role candidate banks and native menu recovery armed before cooking')""")
replace("fixture=(S $Source '.grenade-fixture.json')", "fixture=(S $Source '.grenade-fixture.json');resume=(S $peer '.movement-resume.json');sourceResume=(S $Source '.movement-resume.json');sourceMovement=(S $Source '.movement.json')")
replace("  Start-Sleep -Milliseconds 45\n }\n if($monitor)", """  if($interrupted-and!$moved-and$watch.ElapsedMilliseconds-gt6000){
   $ready=$true
   foreach($r in 'host','client'){$a=S $r '.movement-resume.json';$b=S $r '.movement.json';$ss=S $r '.shadow-stream.json';if($a.phase-ne1-or$a.resumed-le$resumeBefore[$r].resume.resumed-or!$b.active-or!$ss.ready){$ready=$false}}
   if($ready){I $Source Neutral 900 0 12000;$moved=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native forward movement after same-world menu recovery'})}
  }
  Start-Sleep -Milliseconds 45
 }
 if($monitor)""")
replace("  if(!@($rows|Where-Object {$_.ms-gt6000-and!$_.prop.active-and!$_.prop.pendingHide-and!$_.stream.armed}).Count){throw 'Interrupted stream retains prop or ownership'}\n  $checks.Add('Interrupted presentation retires stream ownership and hides its cosmetic prop')", """  if(!@($rows|Where-Object {$_.ms-gt6000-and!$_.prop.active-and!$_.prop.pendingHide}).Count){throw 'Interrupted presentation retains its cosmetic grenade'}
  $recovered=@{}
  foreach($r in 'host','client'){
   $a=S $r '.movement-resume.json';$b=S $r '.movement.json';$ss=S $r '.shadow-stream.json';$br=S $r
   $recovered[$r]=[ordered]@{resume=$a;bank=$b;stream=$ss;bridge=$br}
   if($a.phase-ne1-or$a.resumed-le$resumeBefore[$r].resume.resumed-or!$b.active-or$b.version-ne42-or!$ss.ready){$recovered|ConvertTo-Json -Depth 12|Set-Content "$out/resume-failure.json";throw 'Same-world grenade interruption did not restore both presentations'}
   if($a.checksum-ne$resumeBefore[$r].resume.checksum-or$ss.token-eq$resumeBefore[$r].stream.token-or$ss.peerToken-eq$resumeBefore[$r].stream.peerToken-or$br.probeActorHandle-ne$resumeBefore[$r].bridge.probeActorHandle){throw 'Recovery reused tokens or changed pinned bank/actor'}
  }
  $recovered|ConvertTo-Json -Depth 12|Set-Content "$out/resume-after.json"
  if(!$moved-or!@($rows|Where-Object {$_.ms-gt6000-and$_.movement.active-and$_.movement.speed-gt.2}).Count){throw 'Ordinary movement did not advance the recovered remote body'}
  $checks.Add('Both same-world actors and pinned banks recover with fresh Steam tokens; native input advances the remote gait')""")
replace("try{I $r;C $r 'coop_movement_bank off'", "try{I $r;C $r 'coop_movement_resume off';C $r 'coop_movement_bank off'")
replace("interruption=$Interrupt;checks=", "interruption=$Interrupt;automaticResume=$true;checks=")
replace("PASS_GRENADE_NETWORK", "PASS_GRENADE_RESUME")
dst.write_text(s,encoding='utf-8')
print(dst)
