from pathlib import Path
import re
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
p=S/'engine_bridge.cpp';s=p.read_text()
assert 'native_service_timing.h' not in s
s=s.replace('void writeStatus(ULONGLONG tick) {','#include "native_service_timing.h"\nvoid writeStatus(ULONGLONG tick) {\n    NativeServiceTimer scope("status_all");',1)
a=s.index('void writeStatus(');b=s.index('void pumpTransport(',a)
block=s[a:b]
block=re.sub(r'\b(write\w+)\((tick)?\);',lambda m:f'timedNativeService("{m[1]}",[&]{{{m[0]}}});',block)
s=s[:a]+block+s[b:]
a=s.index('void pumpTransport(');b=s.index('void executeBuffer(',a)
block=s[a:b].replace('lastTransportPump=tick;','lastTransportPump=tick;NativeServiceTimer scope("transport_all");')
names=['initializeSteam','pumpConnections','sampleLocalPlayer','prepareWorldObservation','exchangePoses','checkpointService','sendWorldObservation','sendArmStream','sendAvatarWeapon','sendShadowStream','sampleNativeBlinkFx','sampleNativeGrappleFx','sampleNativeWindblastFx']
for name in names:
 block=re.sub(r'\b'+name+r'\((tick)?\);',lambda m:f'timedNativeService("{name}",[&]{{{m[0]}}});',block)
s=s[:a]+block+s[b:]
s=s.replace('serviceLoadingProbe(tick);finishCinematicHold(tick);','{NativeServiceTimer scope("native_services_all");serviceLoadingProbe(tick);finishCinematicHold(tick);',1)
s=s.replace('serviceSwordLink(tick);finishTransformTrace(tick);','serviceSwordLink(tick);finishTransformTrace(tick);}',1)
s=s.replace('serviceAuthoringProof(tick);serviceMovementBank(tick);','{NativeServiceTimer scope("presentation_all");serviceAuthoringProof(tick);serviceMovementBank(tick);',1)
s=s.replace('nativeMutationAllowed=false;writeStatus(tick);','nativeMutationAllowed=false;}writeStatus(tick);writeNativeServiceTimings(tick);',1)
s=s.replace('    realExecuteBuffer(system);','    if(thread==engineThread.load()){NativeServiceTimer scope("native_execute_buffer");realExecuteBuffer(system);}else realExecuteBuffer(system);',1)
p.write_text(s)
for old,new in [('Build-CrouchLoot-v693.ps1','Build-NativeTiming-v700.ps1'),('Deploy-CrouchLoot-v693.ps1','Deploy-NativeTiming-v700.ps1'),('stage_crouch_loot_v693.py','stage_native_timing_v700.py')]:
 t=(W/old).read_text().replace('CrouchLoot','NativeTiming').replace('crouch-loot-v693','native-timing-v700').replace('stage_crouch_loot_v693','stage_native_timing_v700').replace('v693','v700').replace('Build693','Build700').replace('Deploy693','Deploy700')
 if new.endswith('.py'):
  t=t.replace("candidate-readable-body-v681","candidate-crouch-loot-v693")
  t=re.sub(r"scope='[^']*'", "scope='Bounded engine-thread service and diagnostic-file timing only. No gameplay, networking, identity, freshness, animation or bank changes.693 crouched loot retained.'",t)
 (W/new).write_text(t)
t=(R/'work/natural-moveset/Test-NativeInteractions-v694.ps1').read_text().replace('NativeInteractions694','NativeInteractions700').replace('candidate-crouch-loot-v693','candidate-native-timing-v700').replace('Expected693','Expected700')
t=t.replace("sourceStream=(S $Source '.shadow-stream.json');stream=(S $peer '.shadow-stream.json');movement=", "sourceTiming=(S $Source '.native-service-timing.json');peerTiming=(S $peer '.native-service-timing.json');sourceStream=(S $Source '.shadow-stream.json');stream=(S $peer '.shadow-stream.json');movement=")
(R/'work/natural-moveset/Test-NativeInteractions-v700.ps1').write_text(t)
print('Prepared candidate700 instrumentation and isolated scripts; gameplay policies unchanged')
