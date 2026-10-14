from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset'
f=S/'runtime_probe.cpp';s=f.read_text(encoding='utf-8-sig')
a='(argc==4&&std::strcmp(argv[2],"--watch-mesmerize-targets")';assert s.count(a)==1
s=s.replace(a,'(argc==4&&std::strcmp(argv[2],"--watch-domino-scenes")&&std::strcmp(argv[2],"--watch-mesmerize-targets")')
a='    if(argc==3&&!std::strcmp(argv[2],"--player-domino-scene")){';assert s.count(a)==1
s=s.replace(a,'''    if(argc==4&&!std::strcmp(argv[2],"--watch-domino-scenes")){
        char* end=nullptr;const auto ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePlayerStates states{};NativePlayerPowerScene s{};
            const bool valid=readNativePlayerStates(reader,base,states)&&readNativePlayerDominoScene(reader,base,states,s);
            std::cout<<"{\\"readOnly\\":true,\\"tick\\":"<<GetTickCount64()<<",\\"valid\\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\\"variant\\":"<<s.variant<<",\\"handle\\":"<<s.handle<<",\\"timeMs\\":"<<s.timeMs<<",\\"durationMs\\":"<<s.durationMs
                <<",\\"subscene\\":"<<jsonString(s.subscene)<<",\\"armHandle\\":"<<s.arm.handle<<",\\"equipmentHandle\\":"<<s.power.handle;
            std::cout<<"}\\n";Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''+a)
f.write_text(s,encoding='utf-8')
s=(P/'Observe-NativeDomino-v182.ps1').read_text(encoding='utf-8-sig').replace('$monitor=$null;','$monitor=$null;$strictMonitor=$null;')
a=' $w.Restart();$cast=$false';assert s.count(a)==1
s=s.replace(a,''' $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-scenes.ndjson" -RedirectStandardError "$out/domino-scenes.stderr" -PassThru
'''+a)
a=" $checks.Add('Controller input produces recorded native PhantasmalLink scenes with reciprocal bindings; phase names and counts are observed, not inferred')";assert s.count(a)==1
s=s.replace(a,a+'''
 $strictMonitor.WaitForExit(4000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){throw 'Strict Domino watcher failed'}
 $strict=@(Get-Content "$out/domino-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object valid)
 foreach($v in 19,20,21,22){if(!@($strict|Where-Object variant -eq $v).Count){throw "Native Domino strict phase $v was not observed"}}
 if(@($strict.armHandle|Sort-Object -Unique).Count-ne1-or@($strict.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Domino arm/equipment identity changed'}
 $checks.Add('Strict read-only Domino observer verifies all four observed phases, exact state72 or native exit recovery, reciprocal player/left arm and one owned equipment incarnation')
''')
a=' $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"';assert s.count(a)==1
s=s.replace(a,''' if($strictMonitor){$strictMonitor.WaitForExit(18000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){$failure=($failure+'; strict watcher failed').TrimStart('; ')}}
'''+a)
(P/'Observe-NativeDomino-v189.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig');a='Observe-NativeDomino-v182.ps1';assert s.count(a)==1;s=s.replace(a,'Observe-NativeDomino-v189.ps1');f.write_text(s,encoding='utf-8')
print('Domino strict watcher and v189 native verification harness ready')
