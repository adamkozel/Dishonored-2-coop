from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=(N/'Observe-NativeDoppelgangerHolstered-v493.ps1').read_text(encoding='utf-8-sig')
s=once(s,",[ValidateSet('standing','crouched')][string]$Stance='standing')",",[ValidateRange(100,800)][int]$ReleaseAfterJumpMs=180)")
a=s.index(' $expectedStance=');b=s.index(' $hwnd=',a)
s=s[:a]+''' $context=Q client '--player-states';$sword=Q client '--player-sword'
 $context|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne0-or!$context.motionValid-or$context.motion-ne0-or!$sword.equipped){throw 'Native grounded armed context not established'}
'''+s[b:]
s=once(s,'$cast=$false;$entityNext=5000','$cast=$false;$jumped=$false;$entityNext=5000')
s=s.replace('Native Emily LT1800 from initially holstered ground stance then release','Native Emily LT1800 aim; bounded jump input will override duration')
needle='  $entities=$null;if($w.ElapsedMilliseconds-ge$entityNext)'
s=once(s,needle,'''  if($cast-and!$jumped-and$w.ElapsedMilliseconds-ge2300){
   & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs $ReleaseAfterJumpMs|Out-Null;$jumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input="Native A+LT; release LT $ReleaseAfterJumpMs ms after jump"})
  }
  $scene=Q client '--midnight'
'''+needle)
s=once(s,"player=(Q client '--player-states');sword=", "player=(Q client '--player-states');midnight=$scene;sword=")
a=s.index(' $nativeCasting=');b=s.index(' $scenes|ConvertTo-Json',a)
s=s[:a]+''' $airRows=@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in2,3}).Count})
 $overlap=@($airRows|Where-Object {@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_decoy_cast_*'}).Count})
 $airRelease=@($airRows|Where-Object {@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-eq'midnight/gameplay/player/base/powers/power_decoy_cast_out.midnightscene'-and$_.sceneDurationMs-eq666-and@($_.bindingsResearch).Count-eq1-and$_.bindingsResearch[0].reciprocal}).Count})
 $peak=($rows|ForEach-Object {[double]$_.player.position[2]}|Measure-Object -Maximum).Maximum
 $fastAir=@($strict|Where-Object {$_.playerStatesValid-and(2-in$_.activeStates-or3-in$_.activeStates)})
 [ordered]@{jumpInput=$jumped;releaseAfterJumpMs=$ReleaseAfterJumpMs;airStateSamples=$airRows.Count;airAndRawSceneSamples=$overlap.Count;airReleaseSamples=$airRelease.Count;fastAirSamples=$fastAir.Count;strictAirAccepted=@($fastAir|Where-Object valid).Count;strictAccepted=@($strict|Where-Object valid).Count;peakHeight=$peak;initialHeight=$context.position[2];finalHeight=$rows[-1].player.position[2];stateSets=@($strict|Where-Object playerStatesValid|ForEach-Object {($_.activeStates|Sort-Object)-join','}|Sort-Object -Unique);airRows=$airRows;fastAir=$fastAir;scope='Independent native Jump/Fall and raw scene overlap; current production reader may reject airborne casting. No remote air integration claimed.'}|ConvertTo-Json -Depth 20|Set-Content "$out/air-context-observations.json"
 if(@($rows.player|Where-Object {$_.stance-ne0-or!$_.defaultMode-or@($_.active|Where-Object {$_.id-notin0,2,3,63}).Count}).Count){throw 'Unexpected native state during bounded jump observation'}
 if(!$jumped-or!$airRows.Count-or$peak-[double]$context.position[2]-gt3-or[Math]::Abs([double]$rows[-1].player.position[2]-$context.position[2])-gt.3){throw 'Native jump did not remain bounded and recover the same floor'}
 if(!$airRelease.Count){throw 'No directly sampled native airborne release; inputs alone do not prove airborne casting'}
 $checks.Add('Native Jump/Fall directly overlaps reciprocal666ms Doppelganger release; bounded jump returns to the same floor')
 if($manaAfter.currentCandidate-ge$manaBefore.currentCandidate){throw 'Native mana expenditure not observed'}
 $checks.Add('Native source mana decreases after successful cast; replenishment means this is not an exact cost measurement')
'''+s[b:]
s=s.replace("source='client';stance=$Stance;initiallyHolstered=$true;checks=$checks", "source='client';airborne=$true;releaseAfterJumpMs=$ReleaseAfterJumpMs;checks=$checks")
s=s.replace('Native source initially holstered Emily Doppelganger acquisition','Native source Emily Doppelganger aim then bounded jump/release observation. Native acquisition')
new(N/'Observe-NativeDoppelgangerAir-v506.ps1',s)
w=(W/'Run-DoppelgangerHolsteredObserve-v493.ps1').read_text(encoding='utf-8-sig').replace('native-doppelganger-holstered-standing-v493','native-doppelganger-air-release-v506').replace('Observe-NativeDoppelgangerHolstered-v493','Observe-NativeDoppelgangerAir-v506')
w=once(w,",[ValidateSet('standing','crouched')][string]$Stance='standing')",",[ValidateRange(100,800)][int]$ReleaseAfterJumpMs=180)")
w=w.replace('-Take $Take -Stance $Stance','-Take $Take -ReleaseAfterJumpMs $ReleaseAfterJumpMs')
new(W/'Run-DoppelgangerAirObserve-v506.ps1',w)
print('Prepared native armed airborne Doppelganger observation; production unchanged.')
