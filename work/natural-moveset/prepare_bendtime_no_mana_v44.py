"""Observe native insufficient-mana response without editing resource values."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Observe-BendTime.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'native-bendtime-corvo-v1'","'native-bendtime-corvo-no-mana-v44'")
s=s.replace("$LeftTrigger $trigger", "$LeftTrigger $trigger")
old='-LeftTrigger $trigger|Out-Null}'
new='-LeftTrigger $trigger|ForEach-Object {$script:lastInputSequence=$_.Sequence}}'
assert old in s;s=s.replace(old,new)
s=s.replace("'-t','12'","'-t','16'").replace('$times=@(1000,5000)','$times=@(1000,3000,6000,8000,11000)').replace('$watch.ElapsedMilliseconds-lt9500','$watch.ElapsedMilliseconds-lt14500')
s=s.replace("{I host 255 $TriggerMs;$events.Add", "{if($next-in@(0,2,4)){$mana=& \"$PSScriptRoot/Read-NativeMana.ps1\" -Role host -Evidence \"$out/mana-before-attempt-$next.json\";if($next-eq4){$noManaBefore=$mana}};I host 255 $TriggerMs;$events.Add")
s=s.replace('action="Native LT press ${TriggerMs}ms";index=$next', 'action="Native LT press ${TriggerMs}ms";index=$next;ms=$watch.ElapsedMilliseconds;inputSequence=$script:lastInputSequence')
s=s.replace(";$entities=Q host '--entities'",'').replace("$wind=@($entities.entities|Where-Object {$_.definitionName-match'bendtime'})",'$wind=@()')
s=s.replace("component=(Q host '--object' $power.component)","power=(Q host '--player-bendtime')")
old=" if(!$sceneCount){throw 'No native BendTime cast scene observed'};"
new=""" $firstMana=Get-Content \"$out/mana-before-attempt-0.json\" -Raw|ConvertFrom-Json;$secondMana=Get-Content \"$out/mana-before-attempt-2.json\" -Raw|ConvertFrom-Json
 $nativeCost=$firstMana.currentCandidate-$secondMana.currentCandidate
 if($nativeCost-le0-or$noManaBefore.currentCandidate-ge$nativeCost){throw 'Third activation lacks measured insufficient-mana precondition'}
 $checks.Add('Normal native activations consume mana; the third request has less than the measured activation cost')
 $late=@($rows|Where-Object {$_.ms-gt($events[-1].ms+350)})
 if($late.Count-lt3-or@($late|Where-Object {$_.power.powerActive}).Count){throw 'No-mana request unexpectedly activated the power or lacks observations'}
 if(@($late|Where-Object {@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-match'power_bendtime_cast'}).Count}).Count){throw 'No-mana request unexpectedly created a BendTime cast scene'}
 $loader=Get-Content \"$root/work/lab-instances/host/loader.log\" -Tail 2000
 foreach($event in $events){$line=@($loader|Where-Object {$_-match(\"Consumed simulated controller packet \"+$event.inputSequence+\"[.]\")});if(!$line.Count){throw 'Native controller consumption not confirmed'};$line|Add-Content \"$out/controller-acknowledgements.txt\"}
 $checks.Add('All five controller requests are consumed; the final insufficient-mana request produces no power activation or BendTime scene')
 $noManaAfter=& \"$PSScriptRoot/Read-NativeMana.ps1\" -Role host -Evidence \"$out/mana-after-failed-attempt.json\"
 if($noManaAfter.currentCandidate-lt$noManaBefore.currentCandidate){throw 'Failed attempt unexpectedly spent additional mana'}
 $checks.Add('Failed request spends no further measured native mana')
 if(!$sceneCount){throw 'No native BendTime cast scene observed'};"""
assert old in s;s=s.replace(old,new)
s=s.replace("source='host';crouched=","source='host';noManaExperiment=$true;crouched=")
s=s.replace('Native BendTime investigation only;','Native BendTime insufficient-mana investigation only;')
target=P/'Observe-BendTimeNoMana.ps1';assert not target.exists();target.write_text(s,encoding='utf-8');print(target)
