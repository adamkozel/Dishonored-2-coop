from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-RangedProp.ps1').read_text(encoding='utf-8-sig')
s=s.replace("[ValidateSet('gun','crossbow')][string]$Kind='gun',",'').replace(',[switch]$AuthoredGrip', '')
s=s.replace("$ErrorActionPreference='Stop';", "$Kind='grenade';$ErrorActionPreference='Stop';",1)
s=s.replace("-Command $c -WaitConsumed", "-Command $c -WaitConsumed -ReceiptTimeoutMs 8000")
s=s.replace("New-Item -ItemType Directory -Path $out|Out-Null", "New-Item -ItemType Directory -Path $out|Out-Null;Copy-Item $PSCommandPath \"$out/run-script.ps1\"")
a=s.index(' $clip=Get-Content');b=s.index(' $clip|Select-Object',a)
s=s[:a]+''' $clip=Get-Content "$PSScriptRoot/grenade-authoring-manifest-v55.json" -Raw|ConvertFrom-Json|Where-Object {$_.role-eq$Character-and$_.selection.alias-eq'grenade_idle'}
'''+s[b:]
s=s.replace("'coop_ranged_prop off'", "'coop_ranged_prop off','coop_grenade_prop off'",1)
s=s.replace('C $target "coop_ranged_prop $Kind"','C $target "coop_grenade_prop 15000"')
s=s.replace("'.ranged-prop.json'", "'.grenade-prop.json'").replace("'native_visual_prop_attached'", "'native_grenade_prop_attached'")
s=s.replace("  if(!$played-and$watch.ElapsedMilliseconds-gt5000){C $target 'coop_review_clip off';C $target 'coop_review_clip play';$played=$true}", "  # Hold the authored idle for a clear contact/attachment comparison.")
s=s.replace('after the whole-body donor returns to native locomotion','while the authored idle remains held')
s=s.replace("if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}", "if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player/avatar health changed'}")
s=s.replace("try{C $r 'coop_ranged_prop off';", "try{C $r 'coop_grenade_prop off';")
s=s.replace('Bounded cosmetic attachment and donor-pose experiment. No source weapon ownership or firing; not gameplay-integrated ranged presentation. Visual acceptance separate.','Bounded native grenade cosmetic attachment and authored idle experiment. No source grenade grant, projectile, native gameplay event or network grenade binding. Visual acceptance separate.')
s=s.replace('RANGED_PROP_VERIFIED','GRENADE_PROP_VERIFIED')
(P/'Test-GrenadeProp-v55.ps1').write_text(s,encoding='utf-8');print('Prepared isolated grenade prop/idle harness')
