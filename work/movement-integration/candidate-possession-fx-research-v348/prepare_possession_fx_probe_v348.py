from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset';W=R/'work/movement-integration'
p=S/'runtime_probe.cpp';s=p.read_text();old='(argc==4&&std::strcmp(argv[2],"--watch-possession-state")';assert s.count(old)==1
s=s.replace(old,'(argc==4&&std::strcmp(argv[2],"--watch-possession-effects")&&std::strcmp(argv[2],"--watch-possession-state")')
anchor='    if(argc==4 && std::strcmp(argv[2],"--watch-swarm-effects")==0){';assert s.count(anchor)==1
code=r'''    // Read-only research inventory: deliberately outside the game hook.
    // Controller slot offsets are static candidates until correlated below.
    if(argc==4 && std::strcmp(argv[2],"--watch-possession-effects")==0){
        char* end=nullptr;const auto duration=std::strtoul(argv[3],&end,10);
        if(!end||*end||duration<1000||duration>20000)return 80;
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        const auto started=GetTickCount64();std::cout.precision(9);
        do{
            const auto sampleStart=GetTickCount64();std::uintptr_t game=0;
            NativePossessionTarget target{},after{};std::vector<NativeWindblastFxRecord> records;unsigned failureLine=0;
            const bool targetRead=readNativePossessionTarget(reader,base,target);
            const bool valid=reader(base+0x39a5290,&game,8)&&readNativeFilteredFxRecords(reader,base,game,records,
                [](const std::string& name){return name.rfind("models/effects/gameplay/powers/possession/",0)==0;},&failureLine);
            struct Slot {std::uintptr_t declaration=0;unsigned id=0;bool valid=false;std::string name;};std::array<Slot,5> slots{};
            if(targetRead)for(unsigned i=0;i<slots.size();++i){auto& x=slots[i];std::uintptr_t vt=0,name=0,declAgain=0;unsigned idAgain=0;
                const auto at=target.internal+0x410+0x2b8+i*0x10;
                x.valid=reader(at,&x.declaration,8)&&reader(at+8,&x.id,4);
                if(x.valid&&x.declaration)x.valid=reader(x.declaration,&vt,8)&&vt==base+0x1ab3138&&reader(x.declaration+8,&name,8)&&worldIdentityString(reader,name,x.name);
                x.valid=x.valid&&reader(at,&declAgain,8)&&declAgain==x.declaration&&reader(at+8,&idAgain,4)&&idAgain==x.id;
            }
            const bool targetStable=targetRead&&readNativePossessionTarget(reader,base,after)&&target.player.player.game==game&&
                after.player.player.game==game&&target.internal==after.internal&&target.state==after.state&&target.phase==after.phase&&
                target.target==after.target&&target.entity==after.entity&&target.handle==after.handle&&target.generation==after.generation&&target.hasTarget==after.hasTarget;
            std::cout<<"{\"readOnly\":true,\"valid\":"<<(valid?"true":"false")<<",\"tick\":"<<sampleStart<<",\"readMs\":"<<(GetTickCount64()-sampleStart)
                <<",\"failureLine\":"<<failureLine<<",\"possessionValid\":"<<(targetStable?"true":"false")<<",\"internalState\":"<<target.state
                <<",\"targetHandle\":"<<target.handle<<",\"targetPresent\":"<<(target.hasTarget?"true":"false")<<",\"controllerSlotsResearch\":[";
            for(unsigned i=0;i<slots.size();++i){const auto& x=slots[i];std::cout<<(i?",":"")<<"{\"index\":"<<i<<",\"valid\":"<<(targetStable&&x.valid?"true":"false")
                <<",\"id\":"<<x.id<<",\"declaration\":\"0x"<<std::hex<<x.declaration<<std::dec<<"\",\"name\":"<<jsonString(x.name)<<"}";}
            std::cout<<"],\"effects\":[";bool first=true;
            if(valid)for(const auto& r:records){std::cout<<(first?"":",")<<"{\"id\":"<<r.id<<",\"owner\":"<<r.owner<<",\"parent\":"<<r.parent
                <<",\"name\":"<<jsonString(r.name)<<",\"ownerDefinition\":"<<jsonString(r.ownerDefinition)<<",\"ownerActor\":\"0x"<<std::hex<<r.ownerActor
                <<"\",\"record\":\"0x"<<r.record<<"\",\"declaration\":\"0x"<<r.declaration<<std::dec<<"\",\"origin\":[";
                for(unsigned i=0;i<3;++i)std::cout<<(i?",":"")<<r.origin[i];std::cout<<"],\"axis\":[";
                for(unsigned i=0;i<9;++i)std::cout<<(i?",":"")<<r.axis[i];std::cout<<"]}";first=false;}
            std::cout<<"]}\n"<<std::flush;Sleep(50);
        }while(GetTickCount64()-started<duration);return 0;
    }
'''
s=s.replace(anchor,code+anchor);p.write_text(s,encoding='utf-8')
s=(P/'Test-PossessionExitCloseView-v345.ps1').read_text().replace('$targetMonitor=$null;', '$targetMonitor=$null;$fxMonitor=$null;',1)
anchor=" $w.Restart();$cast=$false;";assert s.count(anchor)==1
s=s.replace(anchor,''' $fxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-effects','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/possession-effects.ndjson" -RedirectStandardError "$out/possession-effects.stderr" -PassThru
'''+anchor)
s=s.replace('@($capture,$peerCapture,$monitor,$ownedMonitor,$targetMonitor)', '@($capture,$peerCapture,$monitor,$ownedMonitor,$targetMonitor,$fxMonitor)')
anchor=" $checks.Add('Native input produces reciprocal Possession cast scenes; observed phase set recorded without assuming complete coverage')"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $fxMonitor.WaitForExit(4000)|Out-Null;if(!$fxMonitor.HasExited-or$fxMonitor.ExitCode){throw 'Read-only Possession FX watcher incomplete'}
 $fx=@(Get-Content "$out/possession-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $fxValid=@($fx|Where-Object valid);$effectNames=@($fxValid.effects.name|Sort-Object -Unique)
 [ordered]@{samples=$fx.Count;valid=$fxValid.Count;unknown=($fx.Count-$fxValid.Count);names=$effectNames;maxReadMs=($fx.readMs|Measure-Object -Maximum).Maximum;scope='Read-only source FX inventory and candidate controller-slot correlation; no remote playback or game-hook scan'}|ConvertTo-Json -Depth 6|Set-Content "$out/fx-research-summary.json"
 if(!$fxValid.Count-or!$effectNames.Count){throw 'No stable native Possession effects observed'}
 $checks.Add('Read-only source Possession FX records captured with target-context stability and candidate controller slots; no remote VFX invocation')
''')
(P/'Observe-PossessionFx-v348.ps1').write_text(s,encoding='utf-8')
print('348 adds read-only bounded external FX research; production DLL unchanged')
