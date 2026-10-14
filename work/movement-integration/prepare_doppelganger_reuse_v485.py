from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:p.read_text(encoding='utf-8-sig')
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=rd(N/'Test-DoppelgangerFx-v482.ps1')
old="  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}"
assert s.count(old)==1
s=s.replace(old,'''
  $health=[ordered]@{player=(Q $r '--health' 1);partner=(Q $r '--health' (S $r).probeActorHandle);isolation=(S $r '.observer-isolation.json')}
  $health|ConvertTo-Json -Depth 10|Set-Content "$out/health-before-$r.json"
  if($health.player.current-ne56-or$health.partner.current-ne40){throw "Clean living fixture required: $r player=$($health.player.current) partner=$($health.partner.current) isolation=$($health.isolation.result)"}
''')
old=" foreach($r in 'client','host'){try{C $r 'coop_movement_fixture off'}catch{}}"
assert s.count(old)==1
s=s.replace(old,'''
 # Retain the already bounded native AI pause while handing the same world to
 # the immediate second take. The previous harness released it here, leaving
 # nearby native enemies free to attack before the next setup health check.
 # The next setup refreshes its own60s fixture; any failure restores checkpoint.
 foreach($r in 'client','host'){
  if($RetainWorldForNextTake-and!$failure){
   try{$hold=S $r '.observer-isolation.json';$hold|ConvertTo-Json -Depth 8|Set-Content "$out/isolation-handoff-$r.json";
    if(!$hold.ready-or!$hold.movementFixture-or$hold.remainingMs-lt5000){throw 'Finite AI hold lacks time for immediate same-world handoff'}
   }catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  }else{try{C $r 'coop_movement_fixture off'}catch{}}
 }
''')
new(N/'Test-DoppelgangerFx-v485.ps1',s)
w=rd(W/'Run-DoppelgangerFx-v482.ps1').replace('482','485').replace('483','486')
# Persist wrapper-level restoration when second setup fails before its dirty flag.
w=w.replace("if(!$restored){& \"$root/work/PrepareShadowSword.ps1\"}",'''if(!$restored){
    & "$root/work/PrepareShadowSword.ps1" *> "$PSScriptRoot/doppelganger-reuse-wrapper-restore-v485.log"
    if(!$?){throw 'Wrapper checkpoint recovery failed'}
    [ordered]@{restored=$true;at=(Get-Date).ToString('o');reason='Second take did not itself finish checkpoint restoration'}|ConvertTo-Json|Set-Content "$PSScriptRoot/doppelganger-reuse-wrapper-restore-v485.json"
   }''')
new(W/'Run-DoppelgangerFx-v485.ps1',w)
print('Prepared485/486 same-model test; preserve finite AI isolation across immediate handoff')
