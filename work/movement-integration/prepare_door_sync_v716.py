from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
p=S/'engine_bridge.cpp';t=p.read_text(encoding='utf-8-sig');assert '#include "door_sync.h"' not in t
t=t.replace('#include "observer_isolation.h"','#include "observer_isolation.h"\n#include "door_sync.h"')
t=t.replace('        if(magic==0x57433244) {','        if(magic==0x4f443244) {\n            if(messages[i]->m_cbSize==sizeof(CoopDoorPacket)){CoopDoorPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveDoorPacket(p,tick);}\n            else ++doorSync.rejected;\n            messages[i]->Release();continue;\n        }\n        if(magic==0x57433244) {')
t=t.replace('    if(!std::strcmp(text,"coop_door_fixture open")', '    if(!std::strcmp(text,"coop_door_sync on")||!std::strcmp(text,"coop_door_sync off"))return true;\n    if(!std::strcmp(text,"coop_door_fixture open")')
t=t.replace('    if(!std::strcmp(request.command,"coop_door_fixture open"))','    if(!std::strcmp(request.command,"coop_door_sync on")){enableDoorSync(true);return;}\n    if(!std::strcmp(request.command,"coop_door_sync off")){enableDoorSync(false);return;}\n    if(!std::strcmp(request.command,"coop_door_fixture open"))')
t=t.replace('DiagnosticSchedule<71>','DiagnosticSchedule<72>').replace('        case 70:timedNativeService','        case 71:timedNativeService("writeDoorSync",[&]{writeDoorSync();});break;\n        case 70:timedNativeService')
t=t.replace('serviceDoorFixture(tick);','serviceDoorFixture(tick);serviceDoorSync(tick);');p.write_text(t,encoding='utf-8')
p=S/'world_identity_tests.cpp';t=p.read_text();t=t.replace('#include "native_openable_tests.h"','#include "native_openable_tests.h"\n#include "door_protocol_tests.h"').replace('nativeOpenableTests();','nativeOpenableTests();doorProtocolTests();');p.write_text(t)
p=R/'outputs/d2-coop-lab/Send-D2Command.ps1';t=p.read_text(encoding='utf-8-sig');assert 'coop_door_fixture' in t;t=t.replace('coop_door_fixture','coop_door_sync|coop_door_fixture');p.write_text(t,encoding='utf-8')
for name in ['Build-DoorApi-v713.ps1','Deploy-DoorApi-v713.ps1','stage_door_api_v713.py']:
    t=(W/name).read_text(encoding='utf-8-sig').replace('DoorApi','DoorSync').replace('door_api','door_sync').replace('713','716').replace('candidate-door-api-v716','candidate-door-sync-v716')
    if name.startswith('stage_'):
        t=t.replace("P=W/'candidate-door-api-v710'","P=W/'candidate-door-api-v713'")
        start=t.index("scope='");end=t.index("')",start)
        t=t[:start]+"scope='Bounded ordinary Dunwall door native Steam desired-state ordering with session/world/arm checks and duplicate rejection. Local native74a400 queue713 retained. Disabled by default; runtime verification pending. Protocol69 and bank67/357 unchanged."+t[end:]
    (W/name.replace('DoorApi','DoorSync').replace('door_api','door_sync').replace('713','716')).write_text(t,encoding='utf-8')
print('Prepared716 native Steam door experiment and protocol tests')
