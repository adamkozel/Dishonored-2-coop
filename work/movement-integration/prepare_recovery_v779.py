from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
def edit(path,old,new):
    s=path.read_text(encoding='utf-8-sig');assert s.count(old)==1,(path,old,s.count(old));path.write_text(s.replace(old,new),encoding='utf-8')
edit(S/'movement_resume_policy.h','pause2, ReadNote4 (VT1c61e68), wheel13.','pause2, ReadNote4 (VT1c61e68). GameOver13 is never an ordinary menu.')
edit(S/'movement_resume_policy.h','return state==2||state==4||state==13;','return state==2||state==4;')
edit(S/'movement_resume_tests.cpp','for(unsigned state:{2u,4u,13u})','for(unsigned state:{2u,4u})')
edit(S/'movement_resume_tests.cpp','9u,10u,11u,12u,14u,15u,16u,17u,999u','9u,10u,11u,12u,13u,14u,15u,16u,17u,999u')
edit(S/'door_protocol_tests.h','for(unsigned menu:{2u,4u,13u})','for(unsigned menu:{2u,4u})')
edit(S/'door_protocol_tests.h','for(unsigned state:{0u,3u,5u,15u,99u})','for(unsigned state:{0u,3u,5u,8u,13u,15u,99u})')
edit(S/'engine_bridge.cpp','#include "potion_fixture.h"','#include "potion_fixture.h"\n#include "note_restore_fixture.h"')
edit(S/'engine_bridge.cpp','    if(!std::strcmp(text,"coop_potion_fixture health")','    if(!std::strcmp(text,"coop_note_fixture restore_absent")||!std::strcmp(text,"coop_note_fixture restore_present"))return true;\n    if(!std::strcmp(text,"coop_potion_fixture health")')
edit(S/'engine_bridge.cpp','    if(!std::strcmp(request.command,"coop_potion_fixture health"))','    if(!std::strcmp(request.command,"coop_note_fixture restore_absent")){noteRestoreRequest=0;return;}\n    if(!std::strcmp(request.command,"coop_note_fixture restore_present")){noteRestoreRequest=1;return;}\n    if(!std::strcmp(request.command,"coop_potion_fixture health"))')
edit(S/'engine_bridge.cpp','servicePotionFixture(tick);serviceMesmerizeFixture','servicePotionFixture(tick);serviceNoteRestoreFixture();serviceMesmerizeFixture')
edit(R/'outputs/d2-coop-lab/Send-D2Command.ps1','|coop_potion_fixture|','|coop_potion_fixture|coop_note_fixture|')
for kind in ('Build','Deploy'):
    s=(W/f'{kind}-DoorMenu-v769.ps1').read_text(encoding='utf-8-sig').replace('DoorMenu','Recovery').replace('door-menu','recovery').replace('door_menu','recovery').replace('769','779')
    (W/f'{kind}-Recovery-v779.ps1').write_text(s,encoding='utf-8')
s=(W/'stage_door_menu_v769.py').read_text(encoding='utf-8-sig').replace("A=W/'candidate-door-menu-v769'; P=W/'candidate-door-session-v763'","A=W/'candidate-recovery-v779'; P=W/'candidate-door-menu-v769'")
s=s.replace('DoorMenu-v769','Recovery-v779').replace('stage_door_menu_v769','stage_recovery_v779').replace('-v769.log','-v779.log')
a=s.index("scope='");b=s.index("')\n(A/",a)
s=s[:a]+"scope='Exclude confirmed native GameOver13 from ordinary menu retention. Weapon wheel8 remains excluded pending native runtime evidence. Add an explicit engine-thread developer-only scalar Mayhew-note inventory restoration for isolated cleanup; no pickup/progression authority claim. Continuous recovery runtime pending."+s[b:]
(W/'stage_recovery_v779.py').write_text(s,encoding='utf-8')
p=R/'work/natural-moveset/Test-ContinuousRecovery-v778.ps1'
edit(p,"if($ns.current-eq1-and$np.current-eq1){break}","if($ns.current-eq1-and$np.current-eq1-and(Selected (D $peer)).nativeCalls-eq(Selected $before[$peer].doors).nativeCalls+1){break}")
edit(p,"if($ns.current-ne1-or$np.current-ne1){throw 'Both moving doors not observed before reload'}","if($ns.current-ne1-or$np.current-ne1-or(Selected (D $peer)).nativeCalls-ne(Selected $before[$peer].doors).nativeCalls+1){throw 'Both moving doors and the corresponding receiver queue count not observed before reload'}")
edit(p,'*> "$out/checkpoint-restore.log";foreach($r','*> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration";foreach($r')
print('Prepared recovery779; original778 evidence and769 candidate retained')
