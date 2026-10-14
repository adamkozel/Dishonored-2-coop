from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset';W=R/'work/movement-integration'
p=S/'runtime_probe.cpp';s=p.read_text();s=s.replace('#include "native_player_possession_item.h"','#include "native_player_possession_item.h"\n#include "native_player_possession_target.h"')
anchor='    if(argc==4&&!std::strcmp(argv[2],"--watch-possession-scenes")){'
assert s.count(anchor)==1
block=r'''    if((argc==4&&!std::strcmp(argv[2],"--watch-possession-state"))||(argc==3&&!std::strcmp(argv[2],"--player-possession-state"))){
        unsigned long ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;}
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePossessionTarget p{};const bool valid=readNativePossessionTarget(reader,base,p);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\"internalState\":"<<p.state<<",\"phaseWord\":"<<p.phase<<",\"physicsStance\":"<<p.player.player.current
                <<",\"defaultMode\":"<<(p.player.defaultMode?"true":"false")<<",\"hasTarget\":"<<(p.hasTarget?"true":"false")
                <<",\"handle\":"<<p.handle<<",\"generation\":"<<p.generation<<",\"flags\":"<<p.flags
                <<",\"machine\":\"0x"<<std::hex<<p.player.component<<"\",\"internal\":\"0x"<<p.internal<<"\",\"target\":\"0x"<<p.target<<"\",\"entity\":\"0x"<<p.entity<<"\""<<std::dec;
            std::cout<<"}\n";if(ms)Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''
s=s.replace(anchor,block+anchor);p.write_text(s)
s=(P/'Observe-NativePossessionTarget-v324.ps1').read_text()
s=s.replace('$ownedMonitor=$null;','$ownedMonitor=$null;$targetMonitor=$null;')
anchor=" $w.Restart();$cast=$false"
s=s.replace(anchor,''' $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-state','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/target-state.ndjson" -RedirectStandardError "$out/target-state.stderr" -PassThru
 $w.Restart();$cast=$false;$exitSent=$false''')
anchor="  $entities=Q host '--entities'"
s=s.replace(anchor,"  if($cast-and!$exitSent-and$w.ElapsedMilliseconds-ge7600){I host 255 120;$exitSent=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT120 to leave host body'})}\n"+anchor)
start=s.index(' $possessed=@($rows|');end=s.index(" C host 'coop_possession_fixture off'",start)
s=s[:start]+''' $targetMonitor.WaitForExit(4000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Possession target watcher failed'}
 $targets=@(Get-Content "$out/target-state.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $validTargets=@($targets|Where-Object {$_.valid-and$_.hasTarget})
 $expected=[Convert]::ToUInt32($target.handle.Substring(2),16)
 $possessed=@($validTargets|Where-Object {$_.internalState-eq14-and$_.handle-eq$expected-and$_.physicsStance-eq18-and!$_.defaultMode})
 [ordered]@{ownedTargetSamples=$validTargets.Count;internalState14Samples=$possessed.Count;targetHandle=$expected;internalStates=@($targets|Where-Object valid|ForEach-Object internalState|Sort-Object -Unique);manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;strictPhases=@($accepted.variant|Sort-Object -Unique);nativeScenes=@($scenes.asset|Sort-Object -Unique);scope='Internal state14, physics stance18 and active-state IDs are separate fields; target weak reference validates generation, exact NPCPossession RTTI, entity handle and component membership'}|ConvertTo-Json -Depth 8|Set-Content "$out/target-observation-summary.json"
 if($possessed.Count-lt3-or$manaAfter.currentCandidate-ge$manaBefore.currentCandidate){throw 'Owned human-target internal state/mana evidence not established'}
 if(@($validTargets|Where-Object handle -ne $expected).Count){throw 'Foreign human target observed'}
 if(@($scenes|Where-Object {$_.asset-like'*power_possession_cast_out.midnightscene'-and$_.sceneDurationMs-eq400}).Count-lt2){throw 'Native successful release400ms not repeatedly observed'}
 $final=Q host '--player-states';$final|ConvertTo-Json -Depth 8|Set-Content "$out/after-exit-player.json"
 if(!$final.defaultMode-or$final.stance-ne0){throw 'Native leave-host input did not recover grounded default mode'}
 $checks.Add('Exact human target ownership under internal state14/physics stance18, native release400ms and normal-input exit recovery observed')
''' +s[end:]
s=s.replace('@($capture,$monitor,$ownedMonitor)','@($capture,$monitor,$ownedMonitor,$targetMonitor)')
s=s.replace('transfer ownership, remote movement/VFX/gameplay remain unverified','generation-validated human target, native internal state14, release400ms and normal-input exit investigated; remote movement/VFX/gameplay remain unverified')
(P/'Observe-NativePossessionTarget-v325.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Observe-NativePossessionTarget-v324.ps1','Observe-NativePossessionTarget-v325.ps1');p.write_text(s)
print('Prepared generation-validated read-only target watcher and corrected field-specific native target test')
