from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; S=R/'outputs/d2-coop-lab/src'; P=R/'work/natural-moveset'
p=S/'shadow_stream.h'; s=p.read_text()
anchor='const char* shadowStreamResult="idle";'
assert anchor in s and 'shadowReadTimingNames' not in s
s=s.replace(anchor,'''// Read-only profiling on the native engine thread; reset with stream startup.
const char* shadowReadTimingNames[]={"states","stun","power","ranged","grenadeScene","carry","carryPreparation","sword","heldRanged","heldGrenade","muzzle"};
std::array<double,11> shadowReadTimingMax{};
std::array<std::uint64_t,11> shadowReadTimingCount{};
template<class Read> bool shadowTimedRead(unsigned stage,Read read){
    const auto start=presentationClock();const bool valid=read();
    shadowReadTimingMax[stage]=(std::max)(shadowReadTimingMax[stage],presentationClock()-start);
    ++shadowReadTimingCount[stage];return valid;
}
'''+anchor)
expressions=[
'readNativePlayerStates(memoryRead,base,states)',
'readNativePlayerStunScene(memoryRead,base,states,stun)',
'(readNativePlayerPowerScene(memoryRead,base,states,power,true)||readNativePlayerDominoScene(memoryRead,base,states,power)||readNativePlayerSwarmScene(memoryRead,base,states,power))',
'readNativePlayerRangedScene(memoryRead,base,states,ranged)',
'readNativePlayerGrenadeScene(memoryRead,base,states,grenade)',
'readNativePlayerCarryScene(memoryRead,base,states,carry,true)',
'readNativePlayerCarryPreparation(memoryRead,base,states,carryPreparation)',
'readNativePlayerSword(memoryRead,base,sword)',
'readNativePlayerRangedItem(memoryRead,base,1,held,true)||readNativePlayerRangedItem(memoryRead,base,2,held,true)',
'readNativePlayerGrenade(memoryRead,base,held,nullptr,true)',
'readNativeRangedFxRecords(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,muzzleRecords)']
for i,x in enumerate(expressions):
 assert s.count(x)==1,(i,s.count(x))
 s=s.replace(x,f'shadowTimedRead({i},[&]{{return {x};}})')
s=s.replace('shadowSourceReadMaxMs=0;shadowSendGapMax=', 'shadowSourceReadMaxMs=0;shadowReadTimingMax.fill(0);shadowReadTimingCount.fill(0);shadowSendGapMax=')
anchor='    fprintf(f,"\\"sourceAttempts\\"'
at=s.index(anchor)
s=s[:at]+'''    fputs("\\"sourceReadTimings\\":{",f);
    for(unsigned i=0;i<shadowReadTimingMax.size();++i)fprintf(f,"%s\\"%s\\":{\\"count\\":%llu,\\"maxMs\\":%.6f}",i?",":"",shadowReadTimingNames[i],shadowReadTimingCount[i],shadowReadTimingMax[i]);
    fputs("},",f);
'''+s[at:]
p.write_text(s)
p=S/'movement_playback.h'; s=p.read_text()
s=s.replace('ULONGLONG stateAge=0;bool motionValid=false;', 'ULONGLONG stateAge=0;bool motionValid=false;\n        unsigned rootCount=0;std::uint64_t rootResets=0;\n        double firstRoot=0,lastRoot=0,renderTime=0;')
old='shadowStream.lastState?tick-shadowStream.lastState:0,pose.motionValid};'
assert old in s
s=s.replace(old,'''shadowStream.lastState?tick-shadowStream.lastState:0,pose.motionValid,
                    poseInterpolation.count,poseInterpolation.resets,
                    poseInterpolation.count?poseInterpolation.samples[0].time:0,
                    poseInterpolation.count?poseInterpolation.samples[poseInterpolation.count-1].time:0,poseInterpolation.renderTime};''')
old='\\"motionValid\\":%s}",i?'
assert old in s
s=s.replace(old,'\\"motionValid\\":%s,\\"rootCount\\":%u,\\"rootResets\\":%llu,\\"firstRoot\\":%.6f,\\"lastRoot\\":%.6f,\\"renderTime\\":%.6f}",i?')
s=s.replace('e.motionValid?"true":"false");','e.motionValid?"true":"false",e.rootCount,e.rootResets,e.firstRoot,e.lastRoot,e.renderTime);')
p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Test-SwarmRatVisualNetwork-v319.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v319.ps1','Test-SwarmRatVisualNetwork-v320.ps1'))
(P/'Test-SwarmRatVisualNetwork-v320.ps1').write_text((P/'Test-SwarmRatVisualNetwork-v319.ps1').read_text())
s=(W/'archive_swarm_trails_v319.py').read_text().replace('v319','v320').replace("W/'prepare_swarm_trails_v320.py'","W/'prepare_source_reader_v320.py'")
start=s.index("scope='Candidate319:");end=s.index("')\n",start)
s=s[:start]+"scope='Candidate320: reduce idle grenade inventory reads without relaxing scene, ownership, freshness or native event checks. Full reader remains for active scenes including unequip and empty recovery; selected-only reader double-validates slot1 and unique equipment membership for idle held presentation. Per-reader timing and root-buffer diagnostic evidence added. Trails319 completed7/7 without refusal;319 failed semantic starvation before cast. Runtime and grenade regressions pending."+s[end:]
(W/'archive_swarm_trails_v320.py').write_text(s)
print('Prepared320 reader optimization and diagnostic evidence')
