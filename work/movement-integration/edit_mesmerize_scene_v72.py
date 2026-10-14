from pathlib import Path
R=Path(__file__).resolve().parents[2]; S=R/'outputs/d2-coop-lab/src'
p=S/'runtime_probe.cpp';s=p.read_text()
s=s.replace('(argc==4&&std::strcmp(argv[2],"--watch-midnight")','(argc==4&&std::strcmp(argv[2],"--watch-power-scenes")&&std::strcmp(argv[2],"--watch-midnight")',1)
marker='    if((argc==3&&!std::strcmp(argv[2],"--midnight"))'
assert marker in s
s=s.replace(marker,r'''    if(argc==4&&!std::strcmp(argv[2],"--watch-power-scenes")){
        char* end=nullptr;const auto milliseconds=std::strtoul(argv[3],&end,10);
        if(!end||*end||milliseconds<100||milliseconds>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePlayerStates states{};NativePlayerPowerScene scene{};
            const bool valid=readNativePlayerStates(reader,base,states)&&readNativePlayerPowerScene(reader,base,states,scene,true);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\"variant\":"<<scene.variant<<",\"handle\":"<<scene.handle<<",\"timeMs\":"<<scene.timeMs<<",\"durationMs\":"<<scene.durationMs
                <<",\"armHandle\":"<<scene.arm.handle<<",\"armSide\":"<<scene.arm.side<<",\"character\":"<<scene.arm.character<<",\"asset\":"<<jsonString(scene.asset);
            std::cout<<"}\n";Sleep(15);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
'''+marker,1)
p.write_text(s)
p=S/'world_identity_tests.cpp';s=p.read_text();marker='                    for(unsigned role=1;role<=2;++role)for(unsigned kind=1;kind<=2;++kind){\n                        auto ranged=cast;'
assert marker in s
test=r'''                    {
                        auto mesmer=cast;
                        mesmer.str(names+0x400,"models/characters/player/emily/body.def");mesmer.str(names+0x600,"models/characters/player/emily/right_arm.def");mesmer.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                        mesmer.str(strings,"models/powers/mesmerize/mesmerize.inventoryitem");mesmer.str(strings+0x100,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");
                        mesmer.q(pc,base+0x1b9bea8);mesmer.q(ped,base+0x1b9bd68);
                        const auto inspect=[&](Memory& mem,bool allow=true){const auto rd=[&](auto a,auto d,auto n){return mem.read(a,d,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerPowerScene(rd,base,st,observedCast,allow);};
                        const char* phases[]={"in","loop","out"};const unsigned durations[]={466,1200,666};
                        for(unsigned k=0;k<3;++k){
                            auto m=mesmer;m.str(texts+0x200,std::string("midnight/gameplay/player/base/powers/power_mesmerize_cast_")+phases[k]+".midnightscene");m.d(scene+0x28,durations[k]);
                            require(inspect(m)&&observedCast.variant==16+k&&observedCast.arm.character==2&&observedCast.arm.side==1,"Mesmerize exact phase binds only the native owned Emily left arm");
                            require(!inspect(m,false),"Mesmerize remains opt-in until production transport/playback validation");
                            auto bad=m;bad.q(bindings+8,actor);require(!inspect(bad),"body cannot impersonate Mesmerize first-person arm binding");
                            bad=m;bad.q(bindings+8,entity);require(!inspect(bad),"Mesmerize equipment cannot impersonate arm binding");
                            bad=m;bad.d(leftArm+0xac,2);require(!inspect(bad),"foreign Mesmerize arm refused");
                            bad=m;bad.d(scene+0x28,durations[k]+1);require(!inspect(bad),"unobserved Mesmerize scene duration refused");
                            bad=m;bad.d(instance+0x18,2);require(!inspect(bad),"extra Mesmerize binding refused");
                            bad=m;bad.d(instance+0x27c,5);require(!inspect(bad),"retired Mesmerize scene refused");
                            bad=m;bad.d(physics+0x50,1);require(!inspect(bad),"unobserved crouched Mesmerize remains refused");
                            bad=m;bad.d(pe+0x20,0);require(!inspect(bad),"unowned Mesmerize cannot source scene");
                            bad=m;unsigned reads=0;NativePlayerStates st{};require(readNativePlayerStates([&](auto a,auto d,auto n){return bad.read(a,d,n);},base,st),"Mesmerize concurrent test initial state");
                            require(!readNativePlayerPowerScene([&](auto a,auto d,auto n){const bool ok=bad.read(a,d,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast,true),"recycled Mesmerize scene incarnation refused");
                        }
                        std::cout<<"PASS: Mesmerize three exact phase identities, reciprocal Emily arm, scope, stale scene and concurrent incarnation guards\n";
                    }
'''
s=s.replace(marker,test+marker,1);p.write_text(s)
