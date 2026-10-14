from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=R/'outputs/d2-coop-lab/src/runtime_probe.cpp'
s=p.read_text(encoding='utf-8-sig')
old='(argc==4&&std::strcmp(argv[2],"--watch-domino-targets")'
assert s.count(old)==1
s=s.replace(old,'(argc==4&&std::strcmp(argv[2],"--watch-domino-links")&&std::strcmp(argv[2],"--watch-domino-targets")')
key='    if((argc==3&&!std::strcmp(argv[2],"--domino-targets"))'
assert s.count(key)==1
code=r'''    if((argc==3&&!std::strcmp(argv[2],"--domino-links"))||(argc==4&&!std::strcmp(argv[2],"--watch-domino-links"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;}
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};const auto started=GetTickCount64();NativeDominoItem known{};bool discovered=false;
        do{NativeDominoLinks s{};unsigned stage=0;LARGE_INTEGER begin{},end{},frequency{};QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&begin);
            const bool cached=discovered;const bool valid=readNativeDominoLinks(reader,base,s,&stage,cached?&known:nullptr);QueryPerformanceCounter(&end);
            if(valid&&s.network.equipment.instantiated){known=s.network.equipment;discovered=true;}else if(!valid&&stage==1)discovered=false;
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage
                <<",\"cached\":"<<(cached?"true":"false")<<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/frequency.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(s.network.present?"true":"false")<<",\"networkHandle\":"<<s.network.handle<<",\"targets\":"<<s.network.targets.size()<<",\"connections\":[";
                bool first=true;for(const auto& x:s.connections){if(!first)std::cout<<',';first=false;
                    std::cout<<"{\"from\":"<<x.from<<",\"to\":"<<x.to<<",\"state\":"<<x.state<<",\"progress\":"<<x.progress<<",\"hasBeam\":"<<(x.hasBeam?"true":"false")
                        <<",\"id\":"<<x.fxId<<",\"owner\":"<<x.owner<<",\"pointer\":\"0x"<<std::hex<<x.pointer<<"\",\"record\":\"0x"<<x.beam.record<<"\""<<std::dec;
                    if(x.hasBeam){std::cout<<",\"start\":[";for(unsigned i=0;i<12;++i){if(i)std::cout<<',';std::cout<<x.start[i];}
                        std::cout<<"],\"end\":[";for(unsigned i=0;i<12;++i){if(i)std::cout<<',';std::cout<<x.end[i];}std::cout<<']';}
                    std::cout<<'}';}std::cout<<']';}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''
p.write_text(s.replace(key,code+key),encoding='utf-8')
