from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
read=lambda p:p.read_text(encoding='utf-8-sig')
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=read(N/'Test-DoppelgangerCrouch-v491.ps1')
s=s.replace("'-t','34'", "'-t','29'").replace("'30000'", "'26000'").replace('ElapsedMilliseconds-lt32000','ElapsedMilliseconds-lt27000')
s=once(s,'[switch]$ExpectVisualReuse)',"[switch]$ExpectVisualReuse,[ValidateSet('standing','crouched')][string]$Stance='standing')")
s=s.replace('doppelganger-crouch-banks-v490','doppelganger-holstered-banks-v495').replace('emily-doppelganger-crouch-bank.bin','emily-doppelganger-holstered-bank.bin').replace('bank56/325','bank57/331').replace('version-eq56','version-eq57').replace('version-ne56','version-ne57')
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B')
b=s.index(' Copy-Item ',a)
s=s[:a]+s[b:]
needle=" $checks.Add('Remote Emily uses exact bank57/331 and native Steam pose transport')"
context=read(N/'Test-DominoContextNetwork-v258.ps1')
a=context.index(' $expectedStance='); b=context.index(' $hwnd=',a)
context=context[a:b].replace('\n if($Holstered){','\n {').replace('$(if($Holstered){0}else{1})','0').replace('[bool]$Holstered','$true').replace(' -Pitch 12 ',' -Pitch $Pitch ').replace('move=$Move;','')
# A standalone brace would be an unevaluated script block in PowerShell.
context=context.replace('\n {\n','\n & {\n')
s=once(s,needle,needle+'\n'+context)
s=once(s,'$cast=$false;$worldNext=4600;', '$cast=$false;$stood=$false;$worldNext=4600;')
needle='  if($cast-and!$cloneSceneCaptured'
s=once(s,needle,'''  if(!$stood-and$w.ElapsedMilliseconds-ge8000){
   if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null}
   $stood=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Return to native standing after completed cast; keep sword sheathed'})
  }
'''+needle)
s=once(s,'player=(Q client \'--player-states\');movement=$m;',"player=(Q client '--player-states');sword=(Q client '--player-sword');movement=$m;")
a=s.index(' $low=@($rows|Where-Object')
b=s.index(' $sourceStream=',a)
s=s[:a]+''' $contextRows=@($rows|Where-Object {$_.movement.powerVariant-in29,30,31-and$_.movement.powerWeight-gt.5})
 if(!$contextRows.Count-or@($contextRows|Where-Object {$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.05-or$_.movement.presentedDrawn-or$_.movement.armedMix-gt.05-or$_.sword.equipped}).Count){throw 'Native/remote cast left observed hands-free stance'}
 $standing=@($rows|Where-Object {$_.ms-ge9500-and$_.player.stance-eq0-and$_.movement.stanceMix-lt.05-and$_.movement.powerWeight-lt.02-and!$_.movement.presentedDrawn-and!$_.sword.equipped})
 if($standing.Count-lt5){throw 'Native and remote standing recovery after cast not observed'}
 [ordered]@{stance=$Stance;holstered=$true;castingSamples=$contextRows.Count;variants=@($contextRows.movement.powerVariant|Sort-Object -Unique);standingRecoverySamples=$standing.Count}|ConvertTo-Json|Set-Content "$out/holstered-verification.json"
 $checks.Add('All three complete-body phases retain sheathed sword and matching stance, then recover to native standing')

'''+s[b:]
s=once(s,"if($m.powerWeight-gt.02-or$m.weight-lt.9-or$m.stanceMix-lt.95-or$stream.remoteState-ne0){throw 'Remote cast did not recover to crouch'}", "if($m.powerWeight-gt.02-or$m.weight-lt.9-or$m.stanceMix-gt.05-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}")
s=once(s,"if(!$sword.equipped-or!(Q client '--player-sword').equipped){throw 'Sword selection changed'}", "if($sword.equipped-or(Q client '--player-sword').equipped){throw 'Sheathed sword unexpectedly drawn'}")
s=s.replace('Remote body recovers to crouch with native sword selection retained','Remote body recovers to standing with sheathed sword retained')
s=s.replace('Native crouched Emily LT1800 then release at grounded floor','Native sheathed-sword Emily LT1800 then release at grounded floor')
s=s.replace("source='client';checks=$checks", "source='client';stance=$Stance;holstered=$true;checks=$checks")
s=s.replace('Crouched armed native Emily Doppelganger phases drive remote full-body bank55 poses','Standing/crouched holstered native Emily Doppelganger phases drive remote full-body bank57 poses')
new(N/'Test-DoppelgangerHolstered-v496.ps1',s)
w=read(W/'Run-DoppelgangerCrouch-v491.ps1').replace('doppelganger-crouch-v491','doppelganger-holstered-standing-v496').replace('Test-DoppelgangerCrouch-v491','Test-DoppelgangerHolstered-v496')
w=once(w,"[string]$Take='doppelganger-holstered-standing-v496')", "[string]$Take='doppelganger-holstered-standing-v496',[ValidateSet('standing','crouched')][string]$Stance='standing')")
w=once(w,'-Take $Take\n','-Take $Take -Stance $Stance\n')
new(W/'Run-DoppelgangerHolstered-v496.ps1',w)
print('Prepared full-body holstered context/recovery and existing clone/FX paired checks.')
