from pathlib import Path
p=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src/runtime_probe.cpp'
s=p.read_text()
a='&&std::strcmp(argv[2],"--arm-skeleton")'
assert s.count(a)==1
s=s.replace(a,a+'&&std::strcmp(argv[2],"--prop-skeleton")')
a='std::strcmp(argv[2],"--arm-skeleton")==0)))'
assert s.count(a)==1
s=s.replace(a,'(std::strcmp(argv[2],"--arm-skeleton")==0||std::strcmp(argv[2],"--prop-skeleton")==0))))')
a='std::strcmp(argv[2],"--arm-skeleton")==0?NativeSkeletonScope::PlayerArm:NativeSkeletonScope::Actor'
assert s.count(a)==1
s=s.replace(a,'std::strcmp(argv[2],"--prop-skeleton")==0?NativeSkeletonScope::CosmeticRanged:'+a)
s=s.replace('--arm-skeleton HANDLE|','--arm-skeleton HANDLE|--prop-skeleton HANDLE|')
p.write_text(s)
print('Added exact cosmetic-prop skeleton read mode')
