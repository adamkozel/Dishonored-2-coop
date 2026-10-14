from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab'
p=L/'src/engine_bridge.cpp';s=p.read_text(encoding='utf-8-sig')
for a,b in [('#include "shadowwalk_fx.h"','#include "shadowwalk_fx.h"\n#include "shadowwalk_material.h"'),('writeNativePowerFx();writeShadowWalkFx();','writeNativePowerFx();writeShadowWalkFx();writeShadowWalkMaterial();'),('serviceShadowWalkFx(tick);serviceNativePowerFx(tick);','serviceShadowWalkFx(tick);serviceShadowWalkMaterial(tick);serviceNativePowerFx(tick);')]:
 assert s.count(a)==1,a;s=s.replace(a,b)
p.write_text(s,encoding='utf-8')
p=L/'src/world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig');a='int main() {';assert s.count(a)==1;s=s.replace(a,'#include "native_material_params_tests.h"\nint main() {\n    nativeMaterialParamsTests();');p.write_text(s,encoding='utf-8')
s=(W/'Build-ShadowWalkFx-v533.ps1').read_text(encoding='utf-8-sig').replace('533','544');(W/'Build-ShadowWalkMaterial-v544.ps1').write_text(s,encoding='utf-8')
print('Prepared isolated native material feasibility544 with ownership/drift tests; passing533 preserved.')
