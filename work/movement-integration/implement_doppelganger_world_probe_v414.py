from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
def one(s,a,b):
 assert s.count(a)==1,(a,s.count(a))
 return s.replace(a,b)
p=S/'world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig')
s=one(s,'#include "native_swarm_world_tests.h"','#include "native_swarm_world_tests.h"\n#include "native_doppelganger_world_tests.h"')
s=one(s,'                            require(inspectDoppelganger(doppelganger)&&item.player.character==2', '                            doppelgangerWorldTests(doppelganger,base,game,pc);\n                            require(inspectDoppelganger(doppelganger)&&item.player.character==2')
p.write_text(s,encoding='utf-8')
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=one(s,'#include "native_player_doppelganger_scene.h"','#include "native_player_doppelganger_scene.h"\n#include "native_doppelganger_world.h"')
s=one(s,'&&std::strcmp(argv[2],"--watch-doppelganger-scenes")','&&std::strcmp(argv[2],"--watch-doppelganger-world")&&std::strcmp(argv[2],"--watch-doppelganger-scenes")')
block=r'''
    if((argc==3&&!std::strcmp(argv[2],"--doppelganger-world"))||(argc==4&&!std::strcmp(argv[2],"--watch-doppelganger-world"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>45000)return 2;}
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeDoppelgangerWorld s{};unsigned stage=0;LARGE_INTEGER begin{},end{},freq{};QueryPerformanceFrequency(&freq);QueryPerformanceCounter(&begin);
            const bool valid=readNativeDoppelgangerWorld(reader,base,s,&stage);QueryPerformanceCounter(&end);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage<<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/freq.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(s.present?"true":"false")<<",\"count\":"<<(s.arrayFlags&0xffffff)<<",\"preview\":"<<s.preview<<",\"last\":"<<s.last<<",\"equipmentHandle\":"<<s.equipment.handle;
                if(s.present){std::cout<<",\"handle\":"<<s.handle<<",\"generation\":"<<s.generation<<",\"stateCandidate\":"<<s.state<<",\"actor\":\"0x"<<std::hex<<s.actor<<"\",\"component\":\"0x"<<s.component<<"\",\"transform\":["<<std::dec;for(unsigned i=0;i<12;++i){if(i)std::cout<<',';std::cout<<s.transform[i];}std::cout<<']';}}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(50);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''
# Raw string contains normal C++ escaped JSON literals; command strings need no escaping.
block=block.replace('\\"--','"--').replace('world\\"','world"')
a='    if(argc==4&&!std::strcmp(argv[2],"--watch-doppelganger-scenes"))'
s=one(s,a,block+'\n'+a);p.write_text(s,encoding='utf-8')
build='''$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe d2_world_identity_tests *> "$PSScriptRoot/doppelganger-world-build-v414.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/doppelganger-world-build-v414.log" -Tail 30;throw 'Build failed'}
 & "$root/work/build/Release/d2_world_identity_tests.exe" *> "$PSScriptRoot/doppelganger-world-tests-v414.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/doppelganger-world-tests-v414.log" -Tail 20;throw 'World tests failed'}
 'Read-only clone ownership probe414 builds and ownership/refusal tests pass; runtime pending'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
'''
(W/'Build-DoppelgangerWorldProbe-v414.ps1').write_text(build,encoding='utf-8')
print('Read-only clone ownership probe414 and tests prepared; live mod unchanged')
