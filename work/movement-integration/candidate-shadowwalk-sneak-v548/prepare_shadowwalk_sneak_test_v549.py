from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-ShadowWalkFx-v535.ps1').read_text(encoding='utf-8-sig')
def once(a,b):
    global s
    assert s.count(a)==1,(a,s.count(a))
    s=s.replace(a,b)
once('[int]$Pitch=30)', '[int]$Pitch=30,[switch]$Standing)')
once("$initialDll=(Get-FileHash", "$entryVariant=if($Standing){32}else{35};$expectedStance=if($Standing){0}else{1}\n$initialDll=(Get-FileHash")
once("try{\n foreach($r in 'host','client'){", """try{
 $candidate=Get-Content "$root/work/movement-integration/candidate-shadowwalk-sneak-v548/identity.json" -Raw|ConvertFrom-Json
 if($initialDll-ne$candidate.dll-or$initialProbe-ne$candidate.probe){throw 'Pinned548 candidate required'}
 foreach($r in 'host','client'){""")
once(" $context=Q client '--player-states';$sword=Q client '--player-sword'", """ if(!$Standing){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 100|Out-Null;Start-Sleep -Milliseconds 800}
 $context=Q client '--player-states';$sword=Q client '--player-sword'""")
once("if($context.stance-ne0-or!$context.motionValid-or$context.motion-ne0-or!$sword.equipped)", "if($context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or!$context.defaultMode-or[Math]::Abs([double]$context.position[2]-66.4370575)-gt.03-or!$sword.equipped)")
s=s.replace('shadowwalk-banks-v523/emily-shadowwalk-bank.bin','shadowwalk-sneak-banks-v548/emily-shadowwalk-sneak-bank.bin').replace('version-eq58','version-eq59').replace('version-ne58','version-ne59').replace('bank58/335','bank59/336').replace('bank58','bank59')
once(" if(!$m.active-or$m.version-ne59", """ foreach($r in 'host','client'){
  S $r|ConvertTo-Json -Depth 10|Set-Content "$out/startup-$r-status.json"
  S $r '.shadow-stream.json'|ConvertTo-Json -Depth 18|Set-Content "$out/startup-$r-stream.json"
 }
 if(!$m.active-or$m.version-ne59""")
assert s.count('in 32,33,34')==3
s=s.replace('in 32,33,34','in $entryVariant,33,34')
once("if(!$recovery.Count){throw 'Remote body did not recover after native exit'}", """if(!$recovery.Count){throw 'Remote body did not recover after native exit'}
 if(@($recovery|Where-Object {$_.player.stance-ne1-or[Math]::Abs($_.movement.stanceMix-1)-gt.02}).Count){throw 'Native exit and remote body did not recover crouched'}""")
once("source='client';checks=$checks", "source='client';crouched=(!$Standing);entryVariant=$entryVariant;checks=$checks")
once("scope='Body-only native Shadow Walk entry, idle, bounded forward/backward crawl and requested exit through native Steam. Original FP preserved. Material, VFX, attacks and untested traversal remain unfinished. Checkpoint and protected saves restored; actual remote/source videos require visual review.'", "scope='548 complete-body standing/sneaking entry, forward/reverse distance-driven crawl, native particles and crouched exit recovery through native Steam. Exact native source context, original first person, isolated checkpoints and protected files checked. Correct shadow material, dedicated direction clips, attack, traversal and broader interruptions remain unfinished. Video review required.'")
target=N/'Test-ShadowWalkSneak-v549.ps1';assert not target.exists();target.write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkFx-v535.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-fx-v535','native-shadowwalk-sneak-v549').replace('Test-ShadowWalkFx-v535','Test-ShadowWalkSneak-v549').replace("operation='ShadowWalkBody'","operation='ShadowWalkSneak549'")
s=s.replace("[string]$Take='native-shadowwalk-sneak-v549')", "[string]$Take='native-shadowwalk-sneak-v549',[switch]$Standing)")
s=s.replace('-Take $Take','-Take $Take -Standing:$Standing')
(W/'Run-ShadowWalkSneak-v549.ps1').write_text(s,encoding='utf-8')
print('Prepared paired native Steam standing/sneaking test549 with source, body, equipment and particle evidence')
