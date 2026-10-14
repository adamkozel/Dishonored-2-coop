from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionMenu-v346.ps1').read_text()
old='$w.ElapsedMilliseconds-ge5200';assert s.count(old)==1
s=s.replace(old,'$w.ElapsedMilliseconds-ge[Math]::Max(5200,$interruptAt+1800)')
s=s.replace("throw 'Receiver interruption failed to restore exact clone visibility and retire stream'", "throw 'Post-menu occupied-source interval or restored/disarmed receiver evidence missing'")
(P/'Test-PossessionMenu-v347.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();old="'PossessionNetwork'{& \"$root/work/natural-moveset/Test-PossessionMenu-v346.ps1\" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}";assert old in s
new=''' 'PossessionNetwork'{
   if($Interrupt-eq'none'){& "$root/work/natural-moveset/Test-PossessionExitCloseView-v345.ps1" -Take $Take -Pitch $Pitch}
   elseif($Interrupt-eq'receiver-stop'){& "$root/work/natural-moveset/Test-PossessionInterruption-v344.ps1" -Take $Take -Pitch $Pitch}
   else{& "$root/work/natural-moveset/Test-PossessionMenu-v347.ps1" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}
  }'''
p.write_text(s.replace(old,new),encoding='utf-8')
print('347 preserves occupied-source observation interval after native menu resume; assertions retained')
