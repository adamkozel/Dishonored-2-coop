"""Native stance/equipment context cases with bounded optional forward travel."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeCrouchMoving-v143.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep('[int]$Pitch=12)',"[int]$Pitch=12,[ValidateSet('standing','crouched')][string]$Stance='standing',[ValidateRange(0,12000)][int]$Move=0,[switch]$Holstered)")
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null')
b=s.index(' $hwnd=',a)
rep(s[a:b],r''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($Stance-eq'crouched'){
  & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 }
 if($Holstered){
  Start-Sleep -Milliseconds 400
  & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null
  Start-Sleep -Milliseconds 1300
 }
 $w.Restart();do{
  $m=S host '.movement.json';$native=Q client '--player-states';$nativeSword=Q client '--player-sword'
  if($native.stance-eq$expectedStance-and[Math]::Abs($m.stanceMix-$expectedStance)-lt.02-and$m.active-and$m.presentedDrawn-ne[bool]$Holstered-and$nativeSword.equipped-ne[bool]$Holstered-and[Math]::Abs($m.armedMix-$(if($Holstered){0}else{1}))-lt.02){break}
  Start-Sleep -Milliseconds 30
 }while($w.ElapsedMilliseconds-lt3500)
 if($native.stance-ne$expectedStance-or[Math]::Abs($m.stanceMix-$expectedStance)-gt.02-or!$m.active-or$m.presentedDrawn-eq[bool]$Holstered-or$nativeSword.equipped-eq[bool]$Holstered){throw 'Native/remote stance or equipment context not ready'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;move=$Move;native=$native;sword=$nativeSword;remote=$m}|ConvertTo-Json -Depth 8|Set-Content "$out/context-ready.json"
 $checks.Add('Native controller stance and sword selection drive the same remote stance/equipment context before the cast')
''')
s=s.replace('-LeftY 12000','-LeftY $Move')
rep('if(!$releaseWalking-and$w.ElapsedMilliseconds-ge3000)', 'if($Move-and!$releaseWalking-and$w.ElapsedMilliseconds-ge3000)')
rep('   & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;$stood=$true', '   if($Stance-eq\'crouched\'){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null};$stood=$true')
rep('player=(Q client \'--player-states\');movement=$m;',"player=(Q client '--player-states');sword=(Q client '--player-sword');movement=$m;")
rep('$_.player.stance-ne1-or$_.movement.stanceMix-lt.98', '$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.02-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered')
rep("throw 'Power phases did not remain in the native/remote crouched context'", "throw 'Power phases did not retain the native/remote stance and sword context'")
rep(" $checks.Add('All three source-native crouched phases drive the complete-body crouch clips; native standing recovery retains the movement bank')", " $checks.Add('All three native phases retain source/remote stance and sword selection, using the corresponding full-body context clips; standing recovery retains the bank')")
a=s.index(' $moving=@($castRows|Where-Object')
b=s.index(' if($m.powerWeight',a)
s=s[:a]+' if($Move){\n'+s[a:b]+' }\n'+s[b:]
rep("if(!$sword.equipped-or!(Q client '--player-sword').equipped)","if($sword.equipped-eq[bool]$Holstered-or(Q client '--player-sword').equipped-eq[bool]$Holstered)")
rep("source='client';checks=$checks;", "source='client';stance=$Stance;holstered=[bool]$Holstered;move=$Move;checks=$checks;")
a=s.index("scope='")+len("scope='");b=s.index("'}|ConvertTo-Json",a)
s=s[:a]+"Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."+s[b:]
(P/'Test-MesmerizeContextNetwork-v145.ps1').write_text(s,encoding='utf-8')
