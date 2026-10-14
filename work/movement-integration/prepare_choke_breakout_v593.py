from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-ChokeNetwork-v589.ps1').read_text()
def change(a,b):
 global s
 assert a in s,a;s=s.replace(a,b)
change("[string]$Source='host')","[string]$Source='host',[ValidateSet('finish','breakout')][string]$Outcome='breakout')")
change('ChokeNetwork589','ChokeNetwork593');change('candidate-choke-network-v584','candidate-choke-breakout-v592');change('archived584','archived592')
change('choke-bank-manifest-v581','choke-breakout-bank-manifest-v592');change('version-eq63','version-eq64');change('version-ne63','version-ne64')
change("'-t','18'","'-t','30'");change("'--watch-midnight','17000'","'--watch-midnight','29000'");change('Milliseconds-lt16000','Milliseconds-lt28000')
change('foreach($side in @(2,-2,0)){',"foreach($trial in @(@(2,0),@(2,2),@(2,-2),@(-2,2),@(-2,-2),@(-2,0))){`n".replace('`n','\n')+'      $ahead=[double]$trial[0];$side=[double]$trial[1]')
change('$ox=$px-2*$fx-$side*$fy;$oy=$py-2*$fy+$side*$fx','$ox=$px+$ahead*$fx-$side*$fy;$oy=$py+$ahead*$fy+$side*$fx')
change('@{side=$side;x=$ox','@{ahead=$ahead;side=$side;x=$ox')
change('I $source RightShoulder 6500',"I $source RightShoulder $(if($Outcome-eq'finish'){6500}else{1200})")
change('foreach($v in 1,2,3){',"$expected=if($Outcome-eq'finish'){@(1,2,3)}else{@(1,2,4)}\n   foreach($v in $expected){")
change("if(!@($samples|Where-Object {$_.ms-gt9000-and$_.movement.chokeWeight-lt.05-and$_.movement.weight-gt.85}).Count)","$endPhase=if($Outcome-eq'finish'){3}else{4};$lastEnd=($samples|Where-Object {$_.player.chokeSceneValid-and$_.player.chokeScene.variant-eq$endPhase}|Measure-Object ms -Maximum).Maximum\n   if(!@($samples|Where-Object {$_.ms-gt($lastEnd+300)-and$_.movement.chokeWeight-lt.05-and$_.movement.weight-gt.85}).Count)")
change('Native RightShoulder held6500ms behind exact authored guard2; paired scene owner verification and visual review remain required.','Native ordinary RightShoulder behind exact authored guard2; selected finish/breakout outcome observed independently. Visual review remains required.')
change('Source-timed remote actor body through native Steam.','Source-timed remote actor body through native Steam, with distinct1500ms combat-lose branch. Native cancellation400 is not represented.')
(N/'Test-ChokeNetwork-v593.ps1').write_text(s)
print('Harness593 prepared for breakout and finish regression, longer capture and opposite-side camera candidates.')
