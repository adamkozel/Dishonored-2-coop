from pathlib import Path
import re
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
p=S/'engine_bridge.cpp';s=p.read_text();assert 'writeDiagnosticSlice' not in s
a=s.index('    timedNativeService("writeNativeMotionRefusals"');b=s.index('    char text[6144]{};',a)
block=s[a:b]
calls=re.findall(r'timedNativeService\("(\w+)",\[&\]\{(\w+\((?:tick)?\);)\}\);',block)
core={'writeMovementResumeStatus','writeShadowStream','writeObserverIsolation'}
ordinary=[x for x in calls if x[0] not in core]
# The inline legacy Mixamo report is one ordinary diagnostic as well.
mi=block.index('    {char path[MAX_PATH]');mj=block.index('\n    timedNativeService("writeSwordSampleStatus"',mi)
mixamo=block[mi:mj].strip()
remaining=re.sub(r'timedNativeService\("\w+",\[&\]\{\w+\((?:tick)?\);\}\);','',block).replace(mixamo,'').strip()
assert not remaining,remaining
n=len(ordinary)+1
helper='#include "diagnostic_schedule.h"\nvoid writeDiagnosticSlice(ULONGLONG tick){\n'
helper+='    if(!*statusPath)return;NativeServiceTimer timer("diagnostic_slice");\n'
helper+=f'    static DiagnosticSchedule<{n}> schedule;const auto start=presentationClock();\n'
helper+='    for(unsigned work=0;work<4;++work){unsigned index=0;if(!schedule.next(tick,index))break;\n        switch(index){\n'
for i,(name,call) in enumerate(ordinary):helper+=f'        case {i}:timedNativeService("{name}",[&]{{{call}}});break;\n'
helper+=f'        case {n-1}:timedNativeService("writeMixamoStatus",[&]{{{mixamo}}});break;\n'
helper+='        }\n        // Soft budget: finish the one atomic file write already in progress.\n        if(presentationClock()-start>=4.)break;\n    }\n}\n'
s=s[:a]+''.join(f'    timedNativeService("{name}",[&]{{{call}}});\n' for name,call in calls if name in core)+s[b:]
s=s.replace('void writeStatus(ULONGLONG tick) {',helper+'void writeStatus(ULONGLONG tick) {',1)
s=s.replace('    if(!*statusPath||tick-lastStatus<250)return;lastStatus=tick;','    writeDiagnosticSlice(tick);\n    if(!*statusPath||tick-lastStatus<250)return;lastStatus=tick;',1)
p.write_text(s)
p=S/'movement_resume_tests.cpp';t=p.read_text().replace('#include <iostream>','#include "diagnostic_schedule.h"\n#include <iostream>')
t=t.replace('int main(){','''int main(){
    {
        DiagnosticSchedule<7> d;unsigned i=99;
        for(unsigned k=0;k<7;++k)check(d.next(100+k,i)&&i==k,"diagnostics visit every writer in order");
        check(!d.next(349,i),"diagnostics do not repeat inside250ms");
        check(d.next(350,i)&&i==0,"first diagnostic becomes due at its own deadline");
        check(!d.next(350,i),"later slots do not burst early with the first slot");
        for(unsigned k=1;k<7;++k)check(d.next(350+k,i)&&i==k,"writers retain fair independent deadlines");
        for(unsigned k=0;k<7;++k)check(d.next(5000,i)&&i==k,"late frame advances one fair cycle");
        check(!d.next(5000,i),"late frame never accumulates missed cycles");
        check(!d.next(4999,i),"clock rollback does not repeat writes");
    }
''',1);p.write_text(t)
for old,new in [('Build-NativeTiming-v700.ps1','Build-DiagnosticSlices-v702.ps1'),('Deploy-NativeTiming-v700.ps1','Deploy-DiagnosticSlices-v702.ps1'),('stage_native_timing_v700.py','stage_diagnostic_slices_v702.py')]:
 t=(W/old).read_text().replace('NativeTiming','DiagnosticSlices').replace('native-timing-v700','diagnostic-slices-v702').replace('stage_native_timing_v700','stage_diagnostic_slices_v702').replace('v700','v702').replace('Build700','Build702').replace('Deploy700','Deploy702')
 if new.endswith('.py'):
  t=t.replace('candidate-crouch-loot-v693','candidate-native-timing-v700')
  t=re.sub(r"scope='[^']*'","scope='Native diagnostic file writes distributed across engine frames: at most4 per frame with4ms soft budget and250ms minimum per writer. Core main/movement/Steam/resume/isolation reports retain cadence. No gameplay, network lease or animation changes.'",t)
 (W/new).write_text(t)
t=(R/'work/natural-moveset/Test-NativeInteractions-v700.ps1').read_text().replace('NativeInteractions700','NativeInteractions702').replace('candidate-native-timing-v700','candidate-diagnostic-slices-v702').replace('Expected700','Expected702')
(R/'work/natural-moveset/Test-NativeInteractions-v702.ps1').write_text(t)
print(f'Prepared702 with{n} staggered writers,3 core reports preserved')
