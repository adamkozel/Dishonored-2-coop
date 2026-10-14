"""Candidate 1012: receiver-side native melee defense for relayed enemy sword hits.

Evidence: work/movement-integration/npc-block-native-v1011.json.
"""
from pathlib import Path
R = Path(__file__).resolve().parents[2]
p = R / 'outputs/d2-coop-lab/src/npc_attack.h'
s = p.read_text()


def rep(old, new, count=1):
    global s
    assert s.count(old) == count, (old[:80], s.count(old))
    s = s.replace(old, new)


rep('struct NpcAttackContact {std::uint64_t arm=0,nonce=0;ULONGLONG tick=0;unsigned attacker=0,inflictor=0;float offset[3]{},direction[3]{};};',
    'struct NpcAttackContact {std::uint64_t arm=0,nonce=0;ULONGLONG tick=0;unsigned attacker=0,inflictor=0,flags=0;float offset[3]{},direction[3]{};};')
rep('''    float before=0,after=0,inflicted=0;unsigned lastAttacker=0;''', '''    float before=0,after=0,inflicted=0;unsigned lastAttacker=0;
    // Receiver native defense outcomes and host-side attacker reactions.
    std::uint64_t blocked=0,parryLose=0,parryWin=0,reactions=0,reactionFailures=0;
    NpcAttackDefense lastDefense=NpcAttackDefense::Damage;''')
rep('''bool npcAttackScope(ULONGLONG tick){''', '''// Exact-image entry points used by arkCpntWeaponMeleeNPC victim resolution
// 1406357d0. Static/runtime evidence: work/movement-integration/
// npc-block-native-v1011.json.
bool meleeDefenseApiMatches(){
    struct Entry {std::uintptr_t rva;unsigned char bytes[10];};
    static const Entry entries[]={
        {0x877720,{0x48,0x89,0x5c,0x24,0x10,0x44,0x89,0x44,0x24,0x18}},
        {0x878230,{0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x30}},
        {0x60d750,{0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10}},
        {0x859980,{0x48,0x8b,0xc4,0x55,0x56,0x57,0x41,0x56,0x41,0x57}},
        {0x8112c0,{0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10}},
        {0x810240,{0x48,0x8b,0x05,0x49,0x50,0x19,0x03,0x48,0x05,0x68}}};
    if(!gameImage)return false;
    for(const auto& e:entries)if(std::memcmp(gameImage+e.rva,e.bytes,sizeof(e.bytes)))return false;
    return true;
}
std::uintptr_t nativeComponent(std::uintptr_t entity,std::uintptr_t descriptorRva){
    return reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,void*,int)>(gameImage+0x877720)(entity,gameImage+descriptorRva,2);
}
// Host: the attack bits native 1406357d0 reads from the hitting weapon.
bool nativeMeleeAttackFlags(std::uintptr_t weapon,unsigned& flags){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,owner=0,decl=0;unsigned char blockable=0,state=0;
    if(!weapon||!meleeDefenseApiMatches())return false;
    const auto component=nativeComponent(weapon,0x39ba940); // arkCpntWeaponMeleeNPC
    if(!component||!memoryRead(component,&vt,8)||vt!=base+0x1b9cf48||!memoryRead(component+0x10,&owner,8)||owner!=weapon||
       !memoryRead(component+8,&decl,8)||!decl||!memoryRead(decl+0x180,&blockable,1)||!memoryRead(component+0x3428,&state,1))return false;
    flags=(blockable?kNpcAttackBlockable:0)|((state>>2&1)?kNpcAttackHeavy:0);return true;
}
// Receiver: native block cone (camera-based, requires PlayerStateMachine
// state44) then the actual block state's own resolution, which plays the
// native block impact/parry presentation. Returns false only for a fault.
bool resolveNativeMeleeDefense(std::uintptr_t player,std::uintptr_t attacker,unsigned flags,NpcAttackDefense& outcome){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);outcome=NpcAttackDefense::Damage;
    if(GetCurrentThreadId()!=engineThread.load()||!player||!attacker||!meleeDefenseApiMatches())return false;
    const auto block=nativeComponent(player,0x39edc20); // arkCpntBlock
    if(!block)return true;
    std::uintptr_t vt=0;if(!memoryRead(block,&vt,8)||vt!=base+0x1b99248)return false;
    alignas(16) float center[4]{};reinterpret_cast<float*(*)(std::uintptr_t,float*)>(gameImage+0x878230)(attacker,center);
    for(unsigned i=0;i<3;++i)if(!std::isfinite(center[i]))return false;
    const bool inCone=reinterpret_cast<bool(*)(std::uintptr_t,float*)>(gameImage+0x60d750)(block,center);
    if(!inCone||!(flags&kNpcAttackBlockable))return true;
    const auto machine=nativeComponent(player,0x39aed60); // arkCpntPlayerStateMachine
    std::uintptr_t table=0,state=0;unsigned char active=0;
    if(!machine||!memoryRead(machine,&vt,8)||vt!=base+0x1c73d58||!memoryRead(machine+0x90,&table,8)||!table||
       !memoryRead(table+0x170,&state,8)||!state||!memoryRead(state+0x20,&active,1)||!active)return false;
    const int result=reinterpret_cast<int(*)(std::uintptr_t,std::uintptr_t,bool)>(gameImage+0x859980)(state,attacker,(flags&kNpcAttackHeavy)!=0);
    if(result<0||result>2)return false;
    outcome=static_cast<NpcAttackDefense>(result+1);return true;
}
// Host: native attacker reaction for a receiver block/parry-lose, as posted
// by 1406357d0: event 0xe6 {victim, result} to the attacking NPC. The owned
// clone stands in for the remote victim. Parry-win stagger is not mirrored.
bool postNativeAttackBlocked(unsigned attacker,unsigned victim,NpcAttackDefense outcome){
    if(GetCurrentThreadId()!=engineThread.load()||!meleeDefenseApiMatches()||(outcome!=NpcAttackDefense::Blocked&&outcome!=NpcAttackDefense::ParryLose))return false;
    const auto events=reinterpret_cast<std::uintptr_t(*)()>(gameImage+0x810240)();if(!events)return false;
    auto* payload=reinterpret_cast<std::uint32_t*>(reinterpret_cast<void*(*)(std::uintptr_t,unsigned,unsigned,unsigned)>(gameImage+0x8112c0)(events+0xc8e8,0xe6,8,attacker));
    if(!payload)return false;
    payload[0]=victim;payload[1]=static_cast<unsigned>(outcome)-1;return true;
}
bool npcAttackScope(ULONGLONG tick){''')
rep('''                handled=true;
                if(s.host&&!s.terminal){
                    NpcAttackContact e{};e.arm=b.arm;e.nonce=b.nonce;e.tick=GetTickCount64();e.attacker=attacker;e.inflictor=inflictor;''', '''                handled=true;
                if(s.host&&!s.terminal){
                    NpcAttackContact e{};e.arm=b.arm;e.nonce=b.nonce;e.tick=GetTickCount64();e.attacker=attacker;e.inflictor=inflictor;
                    if(!nativeMeleeAttackFlags(weapon,e.flags)){npcAttackFault.store(1);break;}''')
rep('''    npcAttack.before=h.current;npcAttack.lastAttacker=c.lease.binding.npc.handle;
    npcAttackApplying=true;''', '''    npcAttack.before=h.current;npcAttack.lastAttacker=c.lease.binding.npc.handle;
    // A native block is resolved by the attacker's weapon before any damage
    // event (1406357d0). Run that resolution against the local replica of the
    // authoritative attacker; a defended contact never reaches 717950.
    if(source.actor!=c.lease.binding.npc.actor||!resolveNativeMeleeDefense(h.actor,source.actor,p.flags,npcAttack.lastDefense))return false;
    if(npcAttack.lastDefense!=NpcAttackDefense::Damage){
        npcAttack.inflicted=0;npcAttack.after=h.current;
        (npcAttack.lastDefense==NpcAttackDefense::Blocked?npcAttack.blocked:npcAttack.lastDefense==NpcAttackDefense::ParryLose?npcAttack.parryLose:npcAttack.parryWin)++;
        return true;
    }
    npcAttackApplying=true;''')
rep('''        npcAttack.acked=p.event;npcAttack.pending.erase(npcAttack.pending.begin());return;''', '''        npcAttack.acked=p.event;npcAttack.pending.erase(npcAttack.pending.begin());
        const auto outcome=static_cast<NpcAttackDefense>(p.flags);npcAttack.lastDefense=outcome;
        if(outcome==NpcAttackDefense::Blocked)++npcAttack.blocked;else if(outcome==NpcAttackDefense::ParryLose)++npcAttack.parryLose;else if(outcome==NpcAttackDefense::ParryWin)++npcAttack.parryWin;
        if(outcome==NpcAttackDefense::Blocked||outcome==NpcAttackDefense::ParryLose){
            NativeHealth attacker{};
            if(npcCombatIdentity(*c,attacker)&&probeHandle&&postNativeAttackBlocked(c->lease.binding.npc.handle,probeHandle,outcome))++npcAttack.reactions;
            else ++npcAttack.reactionFailures;
        }
        return;''')
rep('''        std::memcpy(p.offset,e.offset,12);std::memcpy(p.direction,e.direction,12);''', '''        std::memcpy(p.offset,e.offset,12);std::memcpy(p.direction,e.direction,12);p.flags=e.flags;''')
rep('''    auto ack=npcAttackPacket(*c,*b,NpcAttackMessage::Ack);ack.event=p.event;''', '''    auto ack=npcAttackPacket(*c,*b,NpcAttackMessage::Ack);ack.event=p.event;
    if(p.event==npcAttack.consumed)ack.flags=static_cast<unsigned>(npcAttack.lastDefense);''')
rep('\\"inflicted\\":%.9g,\\"lastAttacker\\":%u,\\"entries\\":[",',
    '\\"inflicted\\":%.9g,\\"lastAttacker\\":%u,\\"blocked\\":%llu,\\"parryLose\\":%llu,\\"parryWin\\":%llu,\\"reactions\\":%llu,\\"reactionFailures\\":%llu,\\"lastDefense\\":%u,\\"entries\\":[",')
rep('npcAttack.inflicted,npcAttack.lastAttacker);',
    'npcAttack.inflicted,npcAttack.lastAttacker,npcAttack.blocked,npcAttack.parryLose,npcAttack.parryWin,npcAttack.reactions,npcAttack.reactionFailures,static_cast<unsigned>(npcAttack.lastDefense));')
p.write_text(s)
print('patched', p)
