from pathlib import Path
p=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src/runtime_probe.cpp'
t=p.read_text()
a='std::strcmp(argv[2],"--prop-skeleton")&&';assert a in t;t=t.replace(a,a+'std::strcmp(argv[2],"--weapon-skeleton")&&')
a='||std::strcmp(argv[2],"--prop-skeleton")==0';assert a in t;t=t.replace(a,a+'||std::strcmp(argv[2],"--weapon-skeleton")==0')
a='s,std::strcmp(argv[2],"--prop-skeleton")==0?';assert a in t;t=t.replace(a,'s,std::strcmp(argv[2],"--weapon-skeleton")==0?NativeSkeletonScope::PlayerRanged:std::strcmp(argv[2],"--prop-skeleton")==0?')
t=t.replace('--prop-skeleton HANDLE|','--prop-skeleton HANDLE|--weapon-skeleton HANDLE|')
p.write_text(t)
