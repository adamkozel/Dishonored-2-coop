from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativeBlock-v1008.ps1').read_text()
def rep(old,new,count=1):
    global s
    assert s.count(old)==count,(old[:70],s.count(old));s=s.replace(old,new)
rep('npc-native-block-v1008','npc-native-block-v1013')
rep('candidate-npc-combat-v1002/identity.json','candidate-npc-combat-v1012/identity.json')
# 1012 defended contacts never reach 717950 and do not count as applied damage.
rep("  if($row.incoming.applied-ge3-or$row.clientPlayerHealth.current-le16){",
    "  $defended=$row.incoming.blocked+$row.incoming.parryLose+$row.incoming.parryWin\n  if($defended-ge3-or$row.incoming.applied-ge3-or$row.clientPlayerHealth.current-le16){")
rep("if($hs.event-and$hs.acked-eq$hs.event-and$cs.applied-eq$hs.event-and!$hs.pending){break}",
    "if($hs.event-and$hs.acked-eq$hs.event-and$cs.consumed-eq$hs.event-and!$hs.pending){break}")
rep("$hs.acked-ne$hs.event-or$cs.applied-ne$hs.event-or$hs.pending-or",
    "$hs.acked-ne$hs.event-or$cs.consumed-ne$hs.event-or$hs.pending-or")
rep(""" $defenseEvents=@((S client '.damage.json').events|Where-Object {$_.handle-eq1-and$_.damageType-eq'80'-and$_.protectedRegions-and$_.inflictedDamage-eq0})""",
""" $clientDamage=S client '.damage.json'
 $swordDamage=@($clientDamage.events|Where-Object {$_.handle-eq1-and$_.damageType-eq'80'-and$_.damageAmount-eq20})
 $defended=$cs.blocked+$cs.parryLose+$cs.parryWin
 # Native resolution: every consumed contact is either defended (no 717950
 # call) or one native sword damage application. Health loss must equal the
 # applied subset only; the host must see the same outcomes in its acks.
 $expectedLoss=($swordDamage|Measure-Object -Property inflictedDamage -Sum).Sum;if(!$expectedLoss){$expectedLoss=0}
 $defenseEvents=@(@{defended=$defended;blocked=$cs.blocked;parryLose=$cs.parryLose;parryWin=$cs.parryWin;applied=$cs.applied;consumed=$cs.consumed;swordDamageCalls=$swordDamage.Count;expectedLoss=$expectedLoss;initial=$playerInitial.current;final=$beforeHealth.current;hostBlocked=$hs.blocked;hostParryLose=$hs.parryLose;hostParryWin=$hs.parryWin;hostReactions=$hs.reactions;hostReactionFailures=$hs.reactionFailures})
 if($defended-lt1-or($defended+$cs.applied)-ne$cs.consumed-or$swordDamage.Count-ne$cs.applied-or[Math]::Abs($playerInitial.current-$expectedLoss-$beforeHealth.current)-gt.01-or
    $hs.blocked-ne$cs.blocked-or$hs.parryLose-ne$cs.parryLose-or$hs.parryWin-ne$cs.parryWin-or$hs.reactionFailures-or$hs.reactions-ne($hs.blocked+$hs.parryLose)){$defenseEvents=@()}""")
rep("if($afterDup.duplicates-le$cs.duplicates-or$afterDup.applied-ne$cs.applied-or$dupHealth.current-ne$beforeHealth.current)",
    "if($afterDup.duplicates-le$cs.duplicates-or$afterDup.applied-ne$cs.applied-or($afterDup.blocked+$afterDup.parryLose+$afterDup.parryWin)-ne($cs.blocked+$cs.parryLose+$cs.parryWin)-or$dupHealth.current-ne$beforeHealth.current)")
rep("No native protected zero-damage contact and held-block animation established",
    "No receiver-native defended contact with consistent health, acknowledgement outcomes and held-block animation")
rep("at least one protected zero-damage relayed enemy sword contact",
    "at least one relayed enemy sword contact defended by native 1406357d0-equivalent resolution (no 717950 call), host-acknowledged outcome and native attacker reaction event")
# PowerShell 7 treats $error as the read-only automatic $Error.
rep('$error=WrappedAngle','$yawError=WrappedAngle');rep('yawError=$error;','yawError=$yawError;')
rep('if([Math]::Abs($error)-gt12){','if([Math]::Abs($yawError)-gt12){')
rep('$axis=if($error*$yawRate-gt0)','$axis=if($yawError*$yawRate-gt0)');rep('[Math]::Abs($error/$yawRate)','[Math]::Abs($yawError/$yawRate)')
(N/'Test-NpcNativeBlock-v1013.ps1').write_text(s)
print('Prepared1013')
