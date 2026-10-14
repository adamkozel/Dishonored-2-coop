from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab'
p=L/'src/engine_bridge.cpp';s=p.read_text();a='#include "npc_combat.h"';assert s.count(a)==1;s=s.replace(a,a+'\n#include "npc_attack.h"')
a='        if(magic==0x434e3244){';assert a in s;s=s.replace(a,'''        if(magic==0x484e3244){
            if(messages[i]->m_cbSize==sizeof(CoopNpcAttack)){CoopNpcAttack p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveNpcAttack(p,tick);}else ++npcAttack.rejected;
            messages[i]->Release();continue;
        }
'''+a)
a='    if(!std::strcmp(text,"coop_avatar_perception on")';i=s.index(a);s=s[:i]+'    if(!std::strcmp(text,"coop_npc_attack on")||!std::strcmp(text,"coop_npc_attack off")||!std::strcmp(text,"coop_npc_attack duplicate"))return true;\n'+s[i:]
a='    if(!std::strcmp(request.command,"coop_avatar_perception on")';i=s.index(a);s=s[:i]+'''    if(!std::strcmp(request.command,"coop_npc_attack on")){npcAttackRequest=1;return;}
    if(!std::strcmp(request.command,"coop_npc_attack off")){npcAttackRequest=0;return;}
    if(!std::strcmp(request.command,"coop_npc_attack duplicate")){repeatNpcAttack();return;}
'''+s[i:]
a='serviceNpcCombat(tick);';assert s.count(a)==1;s=s.replace(a,a+'serviceNpcAttack(tick);')
a='writeAvatarPerception();});break;';assert a in s;s=s.replace(a,'writeAvatarPerception();writeNpcAttack();});break;');p.write_text(s)
p=L/'src/damage_observer.h';s=p.read_text();a='bool routeNpcCombatDamage(';i=s.index(a);s=s[:i]+'bool routeNpcAttackDamage(void* component,void* info,void* result);\n'+s[i:];a='    if(routeNpcCombatDamage(component,info,result))return;';assert a in s;s=s.replace(a,'    if(routeNpcAttackDamage(component,info,result))return;\n'+a);p.write_text(s)
p=L/'src/npc_authority_stream.h';s=p.read_text();a='void stopNpcStream(const char* reason){';assert a in s;s=s.replace(a,'void stopNpcAttack(const char* reason);\n'+a+'\n    stopNpcAttack(reason);');p.write_text(s)
p=L/'Send-D2Command.ps1';s=p.read_text();s=s.replace('|coop_npc_combat|','|coop_npc_combat|coop_npc_attack|',1);assert 'coop_npc_attack|' in s;p.write_text(s)
p=L/'src/world_identity_tests.cpp';s=p.read_text();a='#include "npc_combat_protocol_tests.h"';assert a in s;s=s.replace(a,a+'\n#include "npc_attack_protocol_tests.h"');a='    npcCombatProtocolTests();';assert a in s;s=s.replace(a,a+'\n    npcAttackProtocolTests();');p.write_text(s)
for src,dst in [('Build-AvatarPerception-v983.ps1','Build-NpcAttack-v988.ps1'),('Deploy-AvatarPerception-v983.ps1','Deploy-NpcAttack-v988.ps1'),('stage_avatar_perception_v983.py','stage_npc_attack_v988.py')]:
    s=(W/src).read_text().replace('v983','v988').replace('Deploy983','Deploy988').replace('Build983','Build988').replace('Build-AvatarPerception','Build-NpcAttack').replace('Deploy-AvatarPerception','Deploy-NpcAttack').replace('stage_avatar_perception','stage_npc_attack')
    s=s.replace("P=W/'candidate-npc-combat-v966'","P=W/'candidate-npc-combat-v983'")
    if src.endswith('.py'):
        start=s.index("scope='Retains966");end=s.index("')",start)
        s=s[:start]+"scope='Retains983 opt-in perception. Adds opt-in generic bound-NPC sword attack capture and native remote-player damage over Steam, packet176 v1, handshake/nonces/duplicate ACK order. No direct health writes. Experimental runtime pending; other damage types, parry/block fidelity, full lifecycle and combat remain unaccepted."+s[end:]
    (W/dst).write_text(s)
print('Prepared988, preserves983 and966')
