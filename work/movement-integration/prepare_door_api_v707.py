from pathlib import Path
import re
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
p=S/'engine_bridge.cpp';s=p.read_text();assert 'door_fixture.h' not in s
s=s.replace('#include "climb_fixture.h"','#include "climb_fixture.h"\n#include "door_fixture.h"')
s=s.replace('    if(!std::strcmp(text,"coop_climb_fixture spawn"))return true;','    if(!std::strcmp(text,"coop_door_fixture open")||!std::strcmp(text,"coop_door_fixture close"))return true;\n    if(!std::strcmp(text,"coop_climb_fixture spawn"))return true;')
s=s.replace('    if(!std::strcmp(request.command,"coop_climb_fixture spawn"))','    if(!std::strcmp(request.command,"coop_door_fixture open")){doorFixtureRequest=2;return;}\n    if(!std::strcmp(request.command,"coop_door_fixture close")){doorFixtureRequest=0;return;}\n    if(!std::strcmp(request.command,"coop_climb_fixture spawn"))')
s=s.replace('serviceClimbFixture(tick);','serviceClimbFixture(tick);serviceDoorFixture(tick);');p.write_text(s)
p=R/'outputs/d2-coop-lab/Send-D2Command.ps1';s=p.read_text().replace('|coop_climb_fixture|','|coop_climb_fixture|coop_door_fixture|');s+="\n";p.write_text(s)
for old,new in [('Build-DiagnosticSlices-v702.ps1','Build-DoorApi-v707.ps1'),('Deploy-DiagnosticSlices-v702.ps1','Deploy-DoorApi-v707.ps1'),('stage_diagnostic_slices_v702.py','stage_door_api_v707.py')]:
 t=(W/old).read_text().replace('DiagnosticSlices','DoorApi').replace('diagnostic-slices-v702','door-api-v707').replace('stage_diagnostic_slices_v702','stage_door_api_v707').replace('v702','v707').replace('Build702','Build707').replace('Deploy702','Deploy707')
 if new.endswith('.py'):
  t=t.replace('candidate-native-timing-v700','candidate-diagnostic-slices-v702')
  t=re.sub(r"scope='[^']*'","scope='Strict native Openable reader and explicit isolated ordinary-door native request proof. No network door synchronization yet. Retains702 scheduling, protocol69 and bank67/357.'",t)
 (W/new).write_text(t)
p=R/'documentation/handoff-2026-10-11/47-native-door-interactions.md';s=p.read_text().replace('B671F8249422862BF557D00AAB020B9D05EB04990D582457207FF0294B0F625B5EE','B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180');p.write_text(s)
print('Prepared707 engine-native proof commands; no networking yet')
