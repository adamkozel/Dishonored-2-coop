from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
p=S/'world_identity_tests.cpp';s=p.read_text();anchor='#include "native_mesmerize_targets.h"';assert s.count(anchor)==1
s=s.replace(anchor,anchor+'\n#include "native_possession_target_fx.h"')
anchor='#include "native_domino_targets_tests.h"';s=s.replace(anchor,anchor+'\n#include "native_possession_target_fx_tests.h"')
anchor='                    auto wind=activeDark;';assert s.count(anchor)==1
s=s.replace(anchor,'''                    {
                        auto possession=activeDark;
                        possession.str(strings,"models/powers/possession/possession.inventoryitem");
                        possession.str(strings+0x100,"components/characters/player/base/powers/possession/possession.cpntpowerpossession");
                        possession.q(pc,base+0x1b531c8);possession.q(ped,base+0x1b53088);
                        possessionTargetFxTests(possession,base,game,pc);
                    }
'''+anchor);p.write_text(s,encoding='utf-8')
p=S/'runtime_probe.cpp';s=p.read_text();anchor='#include "native_player_possession_target.h"';assert s.count(anchor)==1;s=s.replace(anchor,anchor+'\n#include "native_possession_target_fx.h"')
anchor='            const bool targetRead=readNativePossessionTarget(reader,base,target);';assert s.count(anchor)==1
s=s.replace(anchor,'''            NativePossessionTargetFx scoped{};unsigned scopedStage=0;
            const auto scopedStart=std::chrono::steady_clock::now();
            const bool scopedValid=readNativePossessionTargetFx(reader,base,scoped,&scopedStage);
            const double scopedMs=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-scopedStart).count();
'''+anchor)
anchor='            std::cout<<"{\\\"readOnly\\\":true,\\\"valid\\\":"<<(valid?"true":"false")<<",\\\"tick\\\":"<<sampleStart'
assert s.count(anchor)==1
s=s.replace(anchor,r'''            std::cout<<"{\"scoped\":{\"valid\":"<<(scopedValid?"true":"false")<<",\"stage\":"<<scopedStage
                <<",\"ms\":"<<scopedMs<<",\"selected\":"<<(scopedValid&&scoped.selected?"true":"false")
                <<",\"present\":"<<(scopedValid&&scoped.present?"true":"false")<<",\"target\":"<<scoped.handle
                <<",\"id\":"<<scoped.id<<",\"owner\":"<<scoped.effect.owner<<"},\"readOnly\":true,\"valid\":"<<(valid?"true":"false")<<",\"tick\":"<<sampleStart''')
if '#include <chrono>' not in s:s='#include <chrono>\n'+s
p.write_text(s,encoding='utf-8')
p=W/'Build-PossessionScopedFx-v350.ps1';s=(W/'Build-PossessionFxProbe-v348.ps1').read_text().replace('possession-fx-probe-build-v348.log','possession-scoped-fx-build-v350.log').replace('--target d2_runtime_probe','--target d2_runtime_probe d2_world_identity_tests')
anchor=" 'Read-only probe348 built; live DLL343 retained'";assert anchor in s
s=s.replace(anchor,''' & "$root/work/build/Release/d2_world_identity_tests.exe" *> "$PSScriptRoot/possession-scoped-fx-tests-v350.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/possession-scoped-fx-tests-v350.log" -Tail 12;throw 'Scoped reader tests failed'}
 'Read-only probe350 and native ownership tests passed; live DLL343 retained'
''');p.write_text(s,encoding='utf-8')
print('Prepared scoped reader, probe and native memory ownership tests')
