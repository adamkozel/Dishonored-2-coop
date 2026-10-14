from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';P=R/'work/natural-moveset'
# Preserve the raw discovery and its incomplete transitional binding sample.
t=P/'possession-empty-v321b';rows=[json.loads(x)for x in(t/'midnight.ndjson').read_text().splitlines()if x.strip()]
scenes=[s for r in rows for s in r.get('instances',[])if s.get('state')==3 and 'power_possession_cast_'in s.get('asset','')]
complete=[s for s in scenes if len(s.get('bindingsResearch',[]))==1 and all(b.get('reciprocal')for b in s['bindingsResearch'])]
summary=dict(scope='Post-run read-only interpretation of failed321b raw discovery; not a passing runtime receipt',rawActiveSamples=len(scenes),completeBindingSamples=len(complete),incomplete=[s for s in scenes if s not in complete],phases=[])
for asset in sorted({s['asset']for s in complete}):
 v=[s for s in complete if s['asset']==asset];summary['phases'].append(dict(asset=asset,samples=len(v),durationMs=sorted({s['sceneDurationMs']for s in v}),handles=sorted({s['id']for s in v}),clockMin=min(s['clockWords'][0]for s in v),clockMax=max(s['clockWords'][0]for s in v),armHandles=sorted({b['handle']for s in v for b in s['bindingsResearch']})))
(t/'read-only-analysis.json').write_text(json.dumps(summary,indent=2))
p=S/'native_player_possession_scene.h';assert not p.exists()
s=(S/'native_player_swarm_scene.h').read_text().replace('Swarm','Possession').replace('swarm','possession')
start=s.index('// v266');end=s.index('inline unsigned',start)
s=s[:start]+'''//321b empty-target cast: native Corvo Running0+UsePower63 owns one
//PlayerLeftArm binding. In333ms and loop200ms observed. Reject partially
//initialized/recycled scenes. Successful-target out/transfer is not admitted
//until separately observed. Read-only; no remote power or damage events.
'''+s[end:]
s=s.replace('duration==400)return 23','duration==333)return 26').replace('duration==333)return 24','duration==200)return 27')
s=s.replace('    if(asset==prefix+"out.midnightscene"&&duration==1600)return 25;\n','')
s=s.replace('states.player.current>1','states.player.current!=0')
s=s.replace('if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))','if(a.id==0&&a.vtable==base+0x1c85628)')
s=s.replace('if(ground!=1||casting>1)return false;','if(ground!=1||casting!=1)return false;')
start=s.index('    // v266');end=s.index('    std::uintptr_t again=0;',start)
s=s[:start]+'''    if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||
       (s.variant!=27&&s.timeMs>s.durationMs)||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=1)return false;
    std::uintptr_t actor=0;unsigned handle=0;
    if(!read(bindings+8,&actor,8)||!actor||!read(actor+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actor||
       !readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actor||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=1)return false;
'''+s[end:]
s=s.replace('    for(unsigned i=0;i<2;++i){\n        if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;', '    if(!read(bindings+8,&again,8)||again!=actor)return false;\n    for(unsigned i=0;i<2;++i){')
assert 'actors[' not in s and 'variant!=25'not in s and 'variant!=24'not in s
p.write_text(s)
p=S/'runtime_probe.cpp';s=p.read_text();assert '#include "native_player_possession_scene.h"' not in s
s=s.replace('#include "native_player_swarm_scene.h"','#include "native_player_swarm_scene.h"\n#include "native_player_possession_scene.h"')
s=s.replace('(argc==4&&std::strcmp(argv[2],"--watch-swarm-effects")','(argc==4&&std::strcmp(argv[2],"--watch-possession-scenes")&&std::strcmp(argv[2],"--watch-swarm-effects")')
start=s.index('    if(argc==4&&!std::strcmp(argv[2],"--watch-swarm-scenes"))');end=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-possession"))',start)
block=s[start:end].replace('Swarm','Possession').replace('swarm','possession');s=s[:start]+block+s[start:];p.write_text(s)
p=P/'Observe-NativePossession-v322.ps1';assert not p.exists();s=(P/'Observe-NativePossession-v321.ps1').read_text()
s=s.replace('$capture=$null;$monitor=$null;', '$capture=$null;$monitor=$null;$ownedMonitor=$null;')
anchor=' $w.Restart();$cast=$false';assert anchor in s
s=s.replace(anchor,''' $ownedMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/owned-scenes.ndjson" -RedirectStandardError "$out/owned-scenes.stderr" -PassThru
'''+anchor)
old=" if(@($scenes|Where-Object {!$_.bindingsResearch.Count-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Native Possession scene bindings not reciprocal'}"
assert old in s
s=s.replace(old,''' $ownedMonitor.WaitForExit(4000)|Out-Null;if(!$ownedMonitor.HasExited-or$ownedMonitor.ExitCode){throw 'Strict Possession watcher incomplete'}
 $owned=@(Get-Content "$out/owned-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $accepted=@($owned|Where-Object valid)
 foreach($variant in 26,27){if(@($accepted|Where-Object variant -eq $variant).Count-lt2){throw "Strict native phase$variant not repeatedly observed"}}
 if(@($accepted|Where-Object {$_.armHandle-le1-or$_.equipmentHandle-ne$power.handle-or($_.activeStates|Sort-Object)-join','-ne'0,63'}).Count){throw 'Strict reader ownership/state mismatch'}
 $accepted|ConvertTo-Json -Depth 8|Set-Content "$out/accepted-scenes.json"
 $incomplete=@($scenes|Where-Object {$_.bindingsResearch.Count-ne1-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count})
 [ordered]@{rawActive=$scenes.Count;incompleteRaw=$incomplete.Count;strictAccepted=$accepted.Count;strictRejected=($owned.Count-$accepted.Count);scope='Raw observations may catch incomplete loop initialization. Only double-validated arm/equipment scenes are accepted.'}|ConvertTo-Json|Set-Content "$out/reader-summary.json"
 $checks.Add('Strict reader repeatedly accepts native333ms entry and200ms aim with exact Corvo arm/equipment ownership and Running0+UsePower63')''')
s=s.replace('@($capture,$monitor)', '@($capture,$monitor,$ownedMonitor)')
# Use a parenthesized state comparison rather than PowerShell join precedence.
s=s.replace("($_.activeStates|Sort-Object)-join','-ne'0,63'", "(($_.activeStates|Sort-Object)-join',')-ne'0,63'")
p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Observe-NativePossession-v321.ps1'in s;p.write_text(s.replace('Observe-NativePossession-v321.ps1','Observe-NativePossession-v322.ps1'))
s=(W/'archive_possession_fixture_v321.py').read_text().replace('v321','v322').replace('prepare_possession_fixture_v322.py','prepare_possession_scene_v322.py')
s=s.replace("R/'work/natural-moveset/Observe-NativePossession-v322.ps1'", "R/'work/natural-moveset/Observe-NativePossession-v322.ps1',S/'Send-D2Command.ps1'")
s=s.replace('Candidate321:','Candidate322: strict read-only entry333/loop200 scene reader rejects incomplete bindings with repeated ownership/state checks. Raw321b failed one incomplete loop binding; native acquisition/restoration succeeded. Sender allowlist fixed321b. ')
(W/'archive_possession_fixture_v322.py').write_text(s)
print(json.dumps(summary))
