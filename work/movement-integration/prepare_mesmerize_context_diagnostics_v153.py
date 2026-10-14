"""Preserve the original failed take and instrument context setup/recovery."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeContextNetwork-v145.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep('  Start-Sleep -Milliseconds 1300','  Start-Sleep -Milliseconds 150')
rep(' $expectedStance=if', ' $setupRows=[Collections.Generic.List[object]]::new()\n $expectedStance=if')
rep("  $m=S host '.movement.json';$native=Q client '--player-states';$nativeSword=Q client '--player-sword'",r'''  $m=S host '.movement.json';$native=Q client '--player-states';$nativeSword=Q client '--player-sword'
  $setupRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;native=$native;sword=$nativeSword;movement=$m;source=(S client '.shadow-stream.json');peer=(S host '.shadow-stream.json');midnight=(Q client '--midnight')})''')
rep(' }while($w.ElapsedMilliseconds-lt3500)',r''' }while($w.ElapsedMilliseconds-lt3500)
 $setupRows|ConvertTo-Json -Depth 18|Set-Content "$out/context-setup-samples.json"''')
rep("input='Native crouched forward input12000 with LT held1800ms then released'", 'input="Native $Stance input LeftY=$Move with LT held1800ms then released"')
rep("input='Continue native crouched forward movement through release/recovery for1400ms then stop'", 'input="Continue native $Stance forward input LeftY=$Move for1400ms then stop"')
rep("input='Native stand after crouched cast recovery'", 'input=$(if($Stance-eq\'crouched\'){\'Native stand after crouched cast recovery\'}else{\'Standing recovery observation; no stance input\'})')
rep("}catch{$failure=$_.Exception.Message}finally{",r'''}catch{$failure=$_.Exception.Message;[ordered]@{failure=$failure;at=(Get-Date).ToString('o')}|ConvertTo-Json|Set-Content "$out/failure-before-cleanup.json"}finally{''')
(P/'Test-MesmerizeContextNetwork-v153.ps1').write_text(s,encoding='utf-8')
