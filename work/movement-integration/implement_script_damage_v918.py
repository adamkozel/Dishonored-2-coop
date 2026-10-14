"""Reproducible bounded map-trigger damage adapter; source candidate915 preserved."""
from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,old,new):
 p=S/name;t=p.read_text();assert t.count(old)==1,(name,old[:90],t.count(old));p.write_text(t.replace(old,new),newline='\n')
names=[]
for batch in ('npc-damage-families-v902','npc-world-damage-v904'):
 m=json.loads((R/f'outputs/d2-coop-lab/research/asset-exports/{batch}/manifest.json').read_text(encoding='utf-8-sig'))
 def walk(x):
  if isinstance(x,dict):
   if isinstance(x.get('Name'),str) and x['Name'].endswith('.damage'):names.append(x['Name'])
   for v in x.values():walk(v)
  elif isinstance(x,list):
   for v in x:walk(v)
 walk(m)
names=sorted(set(names));assert len(names)==85
def key(s):
 h=14695981039346656037
 for b in s.encode():h=((h^b)*1099511628211)&((1<<64)-1)
 return h
assert len({key(s) for s in names})==len(names)
(S/'npc_damage_declarations.h').write_text('#pragma once\n// Exact-installation Void exports902/904. Lookup identities, not runtime coverage.\nstruct NpcDamageDeclarationName {std::uint64_t key;const char* name;};\nconstexpr NpcDamageDeclarationName kNpcDamageDeclarationNames[]={\n'+''.join('    {0x%xull,%s},\n'%(key(s),json.dumps(s)) for s in names)+'};\ninline const char* npcDamageDeclarationName(std::uint64_t key){for(const auto& p:kNpcDamageDeclarationNames)if(p.key==key)return p.name;return nullptr;}\n')
edit('npc_combat_protocol.h','#include "enemy_melee_protocol.h"','#include "enemy_melee_protocol.h"\n#include "npc_damage_declarations.h"')
edit('npc_combat_protocol.h','Assassination,Corpse};','Assassination,Corpse,MapTrigger};\n#pragma pack(push,1)\nstruct NpcCombatScriptDamage {std::uint64_t trigger=0,declaration=0;float amount=0;unsigned reserved=0;};\n#pragma pack(pop)\ninline bool emptyNpcScript(const NpcCombatScriptDamage& s){return !s.trigger&&!s.declaration&&s.amount==0&&!s.reserved;}\ninline bool validNpcScript(const NpcCombatScriptDamage& s){return s.trigger&&npcDamageDeclarationName(s.declaration)&&std::isfinite(s.amount)&&s.amount>=0&&s.amount<=1000000&&!s.reserved;}\ninline bool npcCombatWorldDamage(NpcCombatDamage d){return d==NpcCombatDamage::Corpse||d==NpcCombatDamage::MapTrigger;}')
edit('npc_combat_protocol.h','version=3,size','version=4,size')
edit('npc_combat_protocol.h','    std::uint64_t sourceActorArm=0;','    NpcCombatScriptDamage script{};\n    std::uint64_t sourceActorArm=0;')
edit('npc_combat_protocol.h','sizeof(CoopNpcCombat)==232','sizeof(CoopNpcCombat)==256')
edit('npc_combat_protocol.h','if(damage==NpcCombatDamage::Assassination)return','if(damage==NpcCombatDamage::Assassination||damage==NpcCombatDamage::MapTrigger)return')
edit('npc_combat_protocol.h','p.version!=3','p.version!=4')
edit('npc_combat_protocol.h','    if(p.kind==NpcCombatMessage::Hello)return','    if(p.damage==NpcCombatDamage::MapTrigger?!validNpcScript(p.script):!emptyNpcScript(p.script))return false;\n    if(p.kind==NpcCombatMessage::Hello)return')
edit('npc_combat_protocol.h','const bool world=p.damage==NpcCombatDamage::Corpse;\n    if(world?(p.sourceRole!=2||!p.sourceActorArm):(p.sourceRole>1||p.sourceActorArm))return false;','const bool world=npcCombatWorldDamage(p.damage);\n    if(world?(p.sourceRole!=2||(p.damage==NpcCombatDamage::Corpse?!p.sourceActorArm:p.sourceActorArm!=0)):(p.sourceRole>1||p.sourceActorArm))return false;')
edit('npc_combat.h','#pragma once','#pragma once\n#include "native_trigger_identity.h"')
edit('npc_combat.h','    NativeHealth deathBefore{},deathAfter{};float deathInflicted=0;','    NativeHealth deathBefore{},deathAfter{};float deathInflicted=0;NpcCombatScriptDamage deathScript{};')
edit('npc_combat.h','    std::vector<NpcCombatChannel> channels;','    std::vector<NpcCombatChannel> channels;\n    std::vector<NativeDamageTrigger> triggers;')
anchor='bool npcCombatExtract(NpcAuthorityChannel& c,void* info,NpcCombatDamage& damage,EnemyMeleeGeometry& g,const float* capturedPosition,std::uint64_t& sourceArm,bool& sourcePending)'
helpers='''bool npcCombatTrigger(std::uint64_t key,unsigned handle,NativeDamageTrigger& out){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(const auto& saved:npcCombat.triggers)if(saved.key&&(key?saved.key==key:saved.handle==handle)){
        NativeDamageTrigger fresh{};
        if(!readNativeDamageTrigger(memoryRead,base,observedGame,saved.handle,fresh)||!sameNativeDamageTrigger(saved,fresh))return false;
        out=std::move(fresh);return true;
    }return false;
}
bool npcCombatScriptDeclaration(std::uintptr_t decl,const NpcCombatScriptDamage& script,std::uint64_t& flags,unsigned* reactions){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,manager=0,name=0;float maximum=0;std::string path;
    const char* expected=npcDamageDeclarationName(script.declaration);
    return validNpcScript(script)&&expected&&decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&
        memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==expected&&
        memoryRead(decl+0x5c,&maximum,4)&&std::isfinite(maximum)&&maximum>=script.amount&&maximum<=1000000&&
        memoryRead(decl+0x60,&flags,8)&&memoryRead(decl+0x68,reactions,8);
}
'''
edit('npc_combat.h',anchor,helpers+anchor[:-1]+',NpcCombatScriptDamage& script)')
edit('npc_combat.h','    if(amount==1&&flags==0x12&&attacker==0x3fff0010){','''    NativeDamageTrigger trigger{};
    if(inflictor==0x3fff0010&&npcCombatTrigger(0,attacker,trigger)){
        std::uintptr_t name=0;std::string path;std::uint64_t declaredFlags=0;unsigned declaredReactions[2]{},actualReactions[2]{};
        if(!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,path))return false;
        script={trigger.key,npcDamageKey(path),amount,0};std::memcpy(actualReactions,raw.data()+0x18,8);
        if(!npcCombatScriptDeclaration(decl,script,declaredFlags,declaredReactions)||flags!=declaredFlags||
           actualReactions[0]!=declaredReactions[0]||actualReactions[1]!=declaredReactions[1])return false;
        unsigned regions=0,region=0;float values[9]{};std::memcpy(&regions,raw.data()+0x20,4);std::memcpy(&region,raw.data()+0x4c,4);std::memcpy(values,raw.data()+0x24,36);
        if(regions||region!=1||values[0]!=1e30f||values[1]!=1e30f||values[2]!=1e30f)return false;
        for(unsigned i=3;i<9;++i)if(values[i]!=0)return false;
        damage=NpcCombatDamage::MapTrigger;g={};return true;
    }
    if(amount==1&&flags==0x12&&attacker==0x3fff0010){''')
edit('npc_combat.h','float& inflicted,std::uint64_t sourceArm=0){','float& inflicted,std::uint64_t sourceArm=0,const NpcCombatScriptDamage& script={}){')
edit('npc_combat.h','if(!swordDamageApiMatches()||!npcCombatGeometry(damage,g)||(before.ownerFlags&8)||!npcCombatSourceNear(c,sourceRole,sourceArm))return false;','''const bool scripted=damage==NpcCombatDamage::MapTrigger;NativeDamageTrigger trigger{};
    if(!swordDamageApiMatches()||!npcCombatGeometry(damage,g)||(before.ownerFlags&8)||
       (scripted?!npcCombatTrigger(script.trigger,0,trigger):!npcCombatSourceNear(c,sourceRole,sourceArm)))return false;''')
edit('npc_combat.h','const bool world=damage==NpcCombatDamage::Corpse;if(world!=(sourceRole==2))return false;','const bool world=npcCombatWorldDamage(damage);if(world!=(sourceRole==2))return false;')
edit('npc_combat.h','unsigned attacker=world?0x3fff0010:sourceRole==localRole?1:avatar.handle;','unsigned attacker=scripted?trigger.handle:world?0x3fff0010:sourceRole==localRole?1:avatar.handle;')
edit('npc_combat.h','npcCombatDeclName(damage),false,false);\n    if(!npcCombatDeclaration(decl,damage))return false;','scripted?npcDamageDeclarationName(script.declaration):npcCombatDeclName(damage),false,false);\n    std::uint64_t declaredFlags=0;unsigned declaredReactions[2]{};\n    if(scripted?!npcCombatScriptDeclaration(decl,script,declaredFlags,declaredReactions):!npcCombatDeclaration(decl,damage))return false;')
edit('npc_combat.h','if(damage!=NpcCombatDamage::Assassination){std::memcpy','if(damage!=NpcCombatDamage::Assassination&&!scripted){std::memcpy')
edit('npc_combat.h','    if(world){NativeHealth source{};bool pending=false;','    if(scripted){std::memcpy(params.data()+8,&script.amount,4);}\n    else if(world){NativeHealth source{};bool pending=false;')
edit('npc_combat.h','return application.seen&&!application.failed&&npcCombatReadResult','return (application.seen||world)&&!application.failed&&npcCombatReadResult')
edit('npc_combat.h','float inflicted,std::uint64_t sourceArm=0){\n    auto p=','float inflicted,std::uint64_t sourceArm=0,const NpcCombatScriptDamage& script={}){\n    auto p=')
edit('npc_combat.h','    p.sourceActorArm=sourceArm;','    p.sourceActorArm=sourceArm;p.script=script;')
edit('npc_combat.h','float inflicted,std::uint64_t sourceArm=0){\n    b.deathPending','float inflicted,std::uint64_t sourceArm=0,const NpcCombatScriptDamage& script={}){\n    b.deathPending')
edit('npc_combat.h','    b.deathSourceArm=sourceArm;','    b.deathSourceArm=sourceArm;b.deathScript=script;')
edit('npc_combat.h','b.deathAfter,b.deathInflicted,b.deathSourceArm);','b.deathAfter,b.deathInflicted,b.deathSourceArm,b.deathScript);')
edit('npc_combat.h','std::uint64_t sourceArm=0;bool sourcePending=false;','std::uint64_t sourceArm=0;bool sourcePending=false;NpcCombatScriptDamage script{};')
edit('npc_combat.h','capture.position,sourceArm,sourcePending))','capture.position,sourceArm,sourcePending,script))')
edit('npc_combat.h','        const bool world=damage==NpcCombatDamage::Corpse;','        const bool world=npcCombatWorldDamage(damage);')
edit('npc_combat.h','if(!npcCombatSourceNear(*c,world?2:localRole,sourceArm))','if(damage!=NpcCombatDamage::MapTrigger&&!npcCombatSourceNear(*c,world?2:localRole,sourceArm))')
for head in ('npcCombatApply(*c,before,world?2:0,damage,geometry,after,inflicted','npcCombatDeferDeath(b,world?2:0,0,0,damage,geometry,before,after,inflicted','npcCombatCommit(*c,b,world?2:0,0,damage,geometry,before,after,inflicted'):
 edit('npc_combat.h',head+',sourceArm)',head+',sourceArm,script)')
edit('npc_combat.h','    if(npcCombat.channels.empty())stopNpcCombat("no_ready_native_actors");','    if(!readNativeDamageTriggers(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,npcCombat.triggers)){stopNpcCombat("trigger_catalog_unavailable");return;}\n    if(npcCombat.channels.empty())stopNpcCombat("no_ready_native_actors");')
edit('npc_combat.h','p.damage,p.geometry,after,inflicted,sourceArm)','p.damage,p.geometry,after,inflicted,sourceArm,p.script)')
edit('npc_combat.h','p.damage,p.geometry,before,after,inflicted,sourceArm)','p.damage,p.geometry,before,after,inflicted,sourceArm,p.script)')
print('Implemented map-trigger native damage descriptor, wire4/256, exact unique trigger identities,85 declaration lookup names; runtime coverage pending.')
