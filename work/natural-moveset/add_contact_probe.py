"""Apply the bounded read-only contact-pair probe extension once."""
from pathlib import Path
p=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src/runtime_probe.cpp'
s=p.read_text()
old='(argc==5&&std::strcmp(argv[2],"--skeleton-final"))'
assert s.count(old)==1
s=s.replace(old,'(argc==5&&std::strcmp(argv[2],"--skeleton-final")&&std::strcmp(argv[2],"--contact-pair"))')
needle='    if(argc==3&&(!std::strcmp(argv[2],"--windblast-effects")'
assert s.count(needle)==1
code=r'''    if(argc==5&&!std::strcmp(argv[2],"--contact-pair")){
        // Compact read-only observation of two known entity generations.
        // Sequential reads remain explicitly non-atomic. Do not suspend the game.
        NativeSkeleton pair[2]{};const auto begin=GetTickCount64();
        auto reader=[&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);};
        for(unsigned i=0;i<2;++i){
            char* end=nullptr;const auto value=std::strtoull(argv[i+3],&end,0);
            if(!end||*end||value>0xffffffffull||!value||!readNativeSkeleton(reader,base,unsigned(value),pair[i])||!pair[i].stable)return 53;
        }
        const auto finish=GetTickCount64();
        if(pair[0].game!=pair[1].game||pair[0].actor==pair[1].actor||finish-begin>50)return 53;
        for(const auto& s:pair)if(worldIdentityEntity(reader,s.game,s.handle)!=s.actor)return 53;
        std::cout.precision(9);std::cout<<"{\"readBeginTick\":"<<begin<<",\"readEndTick\":"<<finish<<",\"atomic\":false";
        for(unsigned k=0;k<2;++k){const auto& s=pair[k];
            const auto count=(std::min)(size_t(27),s.jointIds.size());
            std::vector<std::uint16_t> ids(s.jointIds.begin(),s.jointIds.begin()+count);std::vector<std::string> names;
            if(!jointNames(process.value,base,ids,names))return 53;
            std::cout<<(k?",\"victim\":{":",\"carrier\":{")<<"\"handle\":"<<s.handle<<",\"stable\":true,\"root\":[";
            for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<s.root[i];std::cout<<"],\"joints\":[";
            for(unsigned j=0;j<count;++j){std::cout<<(j?",":"")<<"{\"index\":"<<j<<",\"nativeName\":"<<jsonString(names[j])<<",\"globalPose\":[";
                for(unsigned i=0;i<16;++i)std::cout<<(i?",":"")<<s.globalPose[j][i];std::cout<<"]}";
            }std::cout<<"]}";
        }
        std::cout<<"}\n";return 0;
    }
'''
s=s.replace(needle,code+needle)
p.write_text(s)
print('Read-only compact paired skeleton probe added')
