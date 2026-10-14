from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=R/'outputs/d2-coop-lab/src/runtime_probe.cpp'
s=p.read_text(encoding='utf-8-sig')
old='(argc==4&&std::strcmp(argv[2],"--watch-shadowwalk-state")'
assert s.count(old)==1
s=s.replace(old,'(argc==4&&std::strcmp(argv[2],"--material-params")&&std::strcmp(argv[2],"--watch-shadowwalk-state")')
old='    if(argc==4 && std::strcmp(argv[2],"--components")==0) {'
assert s.count(old)==1
s=s.replace(old,'''    if(argc==4 && std::strcmp(argv[2],"--material-params")==0){
        const auto handle=static_cast<unsigned>(std::strtoul(argv[3],nullptr,0));std::uintptr_t game=0,vt=0;
        if(!read(process.value,base+0x39a5290,&game,8)||!game||!read(process.value,game,&vt,8)||vt!=base+0x1c940a8)return 68;
        std::cout<<materialParmResearch(process.value,base,game,handle)<<'\\n';return 0;
    }
'''+old)
p.write_text(s,encoding='utf-8')
print('Read-only material probe539 prepared; source snapshot533 preserves original probe implementation.')
