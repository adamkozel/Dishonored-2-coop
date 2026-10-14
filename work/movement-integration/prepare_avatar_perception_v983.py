from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab'
p=L/'src/engine_bridge.cpp';s=p.read_text()
a='#include "avatar_ai.h"';assert s.count(a)==1;s=s.replace(a,a+'\n#include "avatar_perception.h"')
a='    if(!std::strcmp(text,"coop_probe_follow on")';i=s.index(a);s=s[:i]+'    if(!std::strcmp(text,"coop_avatar_perception on")||!std::strcmp(text,"coop_avatar_perception off"))return true;\n'+s[i:]
a='    if(!std::strcmp(request.command,"coop_probe_follow on")';i=s.index(a);s=s[:i]+'    if(!std::strcmp(request.command,"coop_avatar_perception on")||!std::strcmp(request.command,"coop_avatar_perception off")){avatarPerceptionEnabled=!std::strcmp(request.command,"coop_avatar_perception on");writeAvatarPerception();return;}\n'+s[i:]
a='''                reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(
                    reinterpret_cast<void*>(probeActor),position,sampled.orientation,true,0);'''
assert s.count(a)==1;s=s.replace(a,'                dispatchAvatarTransform(position,sampled.orientation);')
a='case 71:timedNativeService("writeDoorSync",[&]{writeDoorSync();writeCoinSync();writeUsableSync();});break;';assert a in s;s=s.replace(a,a.replace('writeUsableSync();','writeUsableSync();writeAvatarPerception();'))
a='if(!installNativeSwitchTrace()';assert a in s;s=s.replace(a,'if(!installAvatarPerception()||!installNativeSwitchTrace()',1);p.write_text(s)
p=L/'Send-D2Command.ps1';s=p.read_text();s=s.replace('|coop_probe_follow|','|coop_probe_follow|coop_avatar_perception|',1);p.write_text(s)
for src,dst in [('Build-NpcCombat-v966.ps1','Build-AvatarPerception-v983.ps1'),('Deploy-NpcCombat-v966.ps1','Deploy-AvatarPerception-v983.ps1'),('stage_npc_combat_v966.py','stage_avatar_perception_v983.py')]:
    s=(W/src).read_text().replace('v966','v983').replace('Deploy966','Deploy983').replace('Build966','Build983').replace('Build-NpcCombat','Build-AvatarPerception').replace('Deploy-NpcCombat','Deploy-AvatarPerception').replace('stage_npc_combat','stage_avatar_perception')
    s=s.replace("P=W/'candidate-npc-combat-v954'","P=W/'candidate-npc-combat-v966'")
    if src.endswith('.py'):
        start=s.index("scope='Retains954");end=s.index("')",start)
        s=s[:start]+"scope='Retains966. Opt-in owned-avatar continuous transform suppresses only its same-area Observable refresh; native registration count and every other actor/path untouched. Native large jump and area-change behavior retained. Candidate983 perception experiment runtime pending; complete enemy damage relay remains absent."+s[end:]
    (W/dst).write_text(s)
print('Prepared candidate983; verified966 preserved')
