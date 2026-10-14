from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=S/'movement_playback.h';s=p.read_text()
needle='struct MovementPlayback {\n'
assert needle in s
s=s.replace(needle,needle+'''    // Fixed-size evidence for presentation eligibility failures. No relaxed leases.
    struct EligibilityRecord {
        double now=0,poseTime=0,firstSemantic=0,lastSemantic=0;
        unsigned mask=0,state=0,motion=0,stance=0,semanticCount=0;
        ULONGLONG stateAge=0;bool motionValid=false;
    };
    std::array<EligibilityRecord,16> eligibilityRecords{};
    std::uint64_t eligibilityFailures=0,eligibilityRecordCount=0;
    unsigned eligibilityMask=0;
''')
start=s.index('        const bool eligible=semanticValid&&')
end=s.index('\n        if(p.secondJump.advance',start)
old=s[start:end]
split='&&shadowStream.until>tick&&shadowStream.peerReady&&\n            shadowStream.lastState&&tick-shadowStream.lastState<250&&'
assert split in old
physical,actions=old.split(split)
physical=physical.removeprefix('        const bool eligible=semanticValid&&')
actions=actions.removesuffix('&&!shadowAction.actor;')
replacement='        const bool physicalEligible='+physical+';\n        const bool actionEligible='+actions+';\n'+'''        const unsigned eligibilityMask=(!semanticValid?1u:0u)|(!physicalEligible?2u:0u)|
            (shadowStream.until<=tick?4u:0u)|(!shadowStream.peerReady?8u:0u)|
            (!shadowStream.lastState||tick-shadowStream.lastState>=250?16u:0u)|(!actionEligible?32u:0u)|(shadowAction.actor?64u:0u);
        const bool eligible=eligibilityMask==0;
        if(!eligible){
            ++p.eligibilityFailures;
            const auto count=shadowStream.movement.count;
            const auto lastIndex=p.eligibilityRecordCount?(p.eligibilityRecordCount-1)%p.eligibilityRecords.size():0;
            if(p.eligibilityMask!=eligibilityMask||!p.eligibilityRecordCount||now-p.eligibilityRecords[lastIndex].now>=250.){
                p.eligibilityRecords[p.eligibilityRecordCount++%p.eligibilityRecords.size()]={now,pose.time,
                    count?shadowStream.movement.samples[0].time:0,count?shadowStream.movement.samples[count-1].time:0,
                    eligibilityMask,semantic.state,pose.motion,pose.stance,count,
                    shadowStream.lastState?tick-shadowStream.lastState:0,pose.motionValid};
            }
        }
        p.eligibilityMask=eligibilityMask;'''
s=s[:start]+replacement+s[end:]
needle='    fprintf(f,"\\\"carryPlaybackStance\\\":%u,",p.carryStance);'
assert needle in s
s=s.replace(needle,'''    fprintf(f,"\\\"eligibility\\\":{\\\"mask\\\":%u,\\\"failures\\\":%llu,\\\"recordCount\\\":%llu,\\\"records\\\":[",p.eligibilityMask,p.eligibilityFailures,p.eligibilityRecordCount);
    const auto eligibilityCount=(std::min)(p.eligibilityRecordCount,std::uint64_t(p.eligibilityRecords.size()));
    for(std::uint64_t i=0;i<eligibilityCount;++i){const auto& e=p.eligibilityRecords[(p.eligibilityRecordCount-eligibilityCount+i)%p.eligibilityRecords.size()];
        fprintf(f,"%s{\\\"now\\\":%.6f,\\\"poseTime\\\":%.6f,\\\"firstSemantic\\\":%.6f,\\\"lastSemantic\\\":%.6f,\\\"mask\\\":%u,\\\"state\\\":%u,\\\"motion\\\":%u,\\\"stance\\\":%u,\\\"semanticCount\\\":%u,\\\"stateAge\\\":%llu,\\\"motionValid\\\":%s}",i?",":"",e.now,e.poseTime,e.firstSemantic,e.lastSemantic,e.mask,e.state,e.motion,e.stance,e.semanticCount,e.stateAge,e.motionValid?"true":"false");
    }
    fputs("]},",f);
'''+needle)
p.write_text(s)
# Reduce research inventory polling to 10Hz, adding scoped ownership metadata.
p=S/'runtime_probe.cpp';s=p.read_text();start=s.index('    if(argc==4 && std::strcmp(argv[2],"--watch-swarm-effects")==0)');end=s.index('    if(argc==4 && std::strcmp(argv[2],"--loading-world")',start)
chunk=s[start:end]
chunk=chunk.replace('const auto started=GetTickCount64();','NativeSwarmItem known{};bool discovered=false;const auto started=GetTickCount64();')
needle='            std::cout<<"{\\\"readOnly\\\":true,'
assert needle in chunk
chunk=chunk.replace(needle,'''            NativeSwarmItem equipment{};unsigned worldHandle=0,ownerAgain=0;
            const bool equipmentValid=discovered?readCurrentNativeSwarmEquipment(reader,base,known,equipment):readNativeSwarmItem(reader,base,equipment);
            if(equipmentValid&&equipment.instantiated){known=equipment;discovered=true;reader(equipment.component+0xd4,&worldHandle,4);}
            if(!equipmentValid)discovered=false;
            const bool ownershipStable=equipmentValid&&equipment.instantiated&&reader(equipment.component+0xd4,&ownerAgain,4)&&worldHandle==ownerAgain;
            std::cout<<"{\\\"equipmentValid\\\":"<<(ownershipStable?"true":"false")<<",\\\"equipmentHandle\\\":"<<equipment.handle<<",\\\"worldHandle\\\":"<<worldHandle<<",\\\"readOnly\\\":true,''')
chunk=chunk.replace('std::cout.flush();Sleep(20);','std::cout.flush();Sleep(100);')
s=s[:start]+chunk+s[end:];p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Test-SwarmRatVisualNetwork-v312.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v312.ps1','Test-SwarmRatVisualNetwork-v313.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v312.ps1').read_text().replace('-Role client -Pitch 45','-Role client -Pitch 20')
(P/'Test-SwarmRatVisualNetwork-v313.ps1').write_text(s)
print('Prepared313: eligibility evidence only; existing guards unchanged. Observer poll100ms with equipment binding metadata; camera20 degrees.')
