from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
read=lambda p:p.read_text(encoding='utf-8-sig')
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=read(N/'Observe-NativeDoppelgangerHolstered-v493.ps1')
s=s.replace("asset-like'*power_decoy_cast*'", "asset-like'*power_decoy*'")
s=once(s,"[string]$Stance='standing')","[string]$Stance='standing',[switch]$Holstered)")
a=s.index(' # Native X can interact;'); b=s.index(' $context=Q client',a)
s=s[:a]+' if($Holstered){\n'+s[a:b]+' }\n'+s[b:]
s=s.replace('or$sword.equipped){throw \'Native grounded holstered context not established\'}',"or$sword.equipped-eq[bool]$Holstered){throw 'Native grounded equipment context not established'}")
s=s.replace('$cast=$false;$entityNext=5000','$cast=$false;$cancelled=$false;$entityNext=5000')
s=once(s,"if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Emily LT1800 from initially holstered ground stance then release'})}",
'''if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 6000;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT6000 aim hold, to be overridden by cancel input'})}
  if($cast-and!$cancelled-and$w.ElapsedMilliseconds-ge2400){
   & "$lab/Send-D2Input.ps1" -Role client -Button Neutral -LeftTrigger 255 -RightTrigger 255 -DurationMs 250|Out-Null
   $cancelled=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native RT cancel while LT remains held; both released after250ms'})
  }''')
s=once(s,"foreach($part in 'in','loop','out')", "foreach($part in 'in','loop')")
s=s.replace('Native Doppelganger entry/loop/out scenes','Native Doppelganger entry/loop and observed cancellation/recovery scenes')
s=s.replace('Native input produces reciprocal Doppelganger entry/loop/out scenes with observed clocks and state sets','Native input produces reciprocal Doppelganger entry/loop and observed cancellation/recovery scenes with clocks and state sets')
a=s.index(' $nativeCasting='); b=s.index(' $scenes|ConvertTo-Json',a)
s=s[:a]+''' $nativeCasting=@($strict|Where-Object {$_.playerStatesValid-and$_.stance-eq$expectedStance-and$groundState-in$_.activeStates-and63-in$_.activeStates})
 $clones=@($rows.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\\.def$'})
 $finalPlayer=Q client '--player-states';$finalSword=Q client '--player-sword'
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;cancelInputSent=$cancelled;castingSamples=$nativeCasting.Count;productionAccepted=@($strict|Where-Object valid).Count;acceptedVariants=@($strict|Where-Object valid|ForEach-Object variant|Sort-Object -Unique);rawSceneSamples=$scenes.Count;nativeClonesObserved=$clones.Count;rawScenePaths=@($scenes.asset|Sort-Object -Unique);finalPlayer=$finalPlayer;finalSword=$finalSword;scope='Native RT cancellation input observation; no remote cancellation verification yet.'}|ConvertTo-Json -Depth 12|Set-Content "$out/cancel-observations.json"
 if(!$cancelled-or$nativeCasting.Count-lt3){throw 'Cancellation input or native aim evidence absent'}
 if($clones.Count){throw 'A clone was spawned despite the attempted native cancellation'}
 if($finalPlayer.stance-ne$expectedStance-or@($finalPlayer.active|Where-Object id -eq63).Count-or$finalSword.equipped-eq[bool]$Holstered){throw 'Cancellation did not restore native ground/equipment state'}
 $checks.Add('Native RT cancels aimed Doppelganger before a native clone is enumerated; original stance/equipment recover and actual exit scene evidence is retained')

'''+s[b:]
s=s.replace("stance=$Stance;initiallyHolstered=$true;checks=$checks", "stance=$Stance;holstered=[bool]$Holstered;cancel=$true;checks=$checks")
s=s.replace('Native source initially holstered Emily Doppelganger acquisition','Native source Emily Doppelganger aim/cancellation observation; stance and holstered parameters define the context. Native acquisition')
new(N/'Observe-NativeDoppelgangerCancel-v499.ps1',s)
w=read(W/'Run-DoppelgangerHolsteredObserve-v493.ps1').replace('native-doppelganger-holstered-standing-v493','native-doppelganger-cancel-standing-v499').replace('Observe-NativeDoppelgangerHolstered-v493','Observe-NativeDoppelgangerCancel-v499')
w=once(w,"[string]$Stance='standing')","[string]$Stance='standing',[switch]$Holstered)")
w=once(w,'-Take $Take -Stance $Stance\n','-Take $Take -Stance $Stance -Holstered:$Holstered\n')
new(W/'Run-DoppelgangerCancelObserve-v499.ps1',w)
print('Prepared source-only normal RT cancellation observation; no production changes.')
