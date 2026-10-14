from pathlib import Path
R=Path(__file__).resolve().parents[2];p=R/'outputs/d2-coop-lab/src/runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
needle='(argc==4&&std::strcmp(argv[2],"--watch-player-states")'
assert s.count(needle)==1
s=s.replace(needle,'(argc==4&&std::strcmp(argv[2],"--usable")&&std::strcmp(argv[2],"--watch-player-states")',1)
needle='    if((argc==5&&!std::strcmp(argv[2],"--contact-pair"))'
assert s.count(needle)==1
block=r'''    if(argc==4&&!std::strcmp(argv[2],"--usable")){
        const auto handle=static_cast<unsigned>(std::strtoul(argv[3],nullptr,0));std::uintptr_t game=0;NativeUsable u{};unsigned stage=0;
        const auto reader=[&](auto a,auto d,auto n){return read(process.value,a,d,n);};
        if(!reader(base+0x39a5290,&game,8)||!readNativeUsable(reader,base,game,handle,u,&stage)){std::cerr<<"Usable reader refused stage="<<stage<<'\n';return 59;}
        std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"handle\":"<<u.handle<<",\"name\":"<<jsonString(u.name)
            <<",\"definition\":"<<jsonString(u.definitionName)<<",\"declaration\":"<<jsonString(u.declarationName)
            <<",\"current\":"<<u.current<<",\"pending\":"<<u.pending<<",\"initial\":"<<u.initial<<",\"stateCount\":"<<u.stateCount
            <<",\"enabled\":"<<unsigned(u.enabled)<<",\"flags\":"<<unsigned(u.flags)<<",\"transitioning\":"<<(u.transitioning()?"true":"false")
            <<",\"instigator\":"<<u.instigator<<",\"component\":\"0x"<<std::hex<<u.component<<"\",\"animation\":\"0x"<<u.animation<<std::dec<<"\",\"generation\":"<<u.generation<<",\"animationGeneration\":"<<u.animationGeneration<<",\"entries\":[";
        for(unsigned i=0;i<u.stateCount;++i){const auto& e=u.entries[i];std::cout<<(i?",":"")<<"{\"index\":"<<i<<",\"enter\":"<<unsigned(e.enter)<<",\"exit\":"<<unsigned(e.exit)<<",\"seconds\":"<<e.transitionSeconds<<",\"applyOnFinish\":"<<unsigned(e.applyOnFinish)<<",\"customPrompt\":"<<unsigned(e.customPrompt)<<"}";}
        std::cout<<"]}\n";return 0;
    }
'''
s=s.replace(needle,block+needle,1);p.write_text(s,encoding='utf-8')
