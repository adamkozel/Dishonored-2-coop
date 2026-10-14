from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
def replace(s,a,b):
    assert a in s,a[:140]
    return s.replace(a,b)
p=S/'npc_combat_protocol.h';s=p.read_text()
s=replace(s,'// Reliable, ordered ordinary-sword events, independently scoped from pose','// Reliable native melee damage events, independently scoped from pose')
s=replace(s,'#pragma pack(push,1)','enum class NpcCombatDamage:unsigned {None=0,Sword,Fatality,Assassination};\n#pragma pack(push,1)')
s=replace(s,'version=1,','version=2,');s=replace(s,'unsigned sourceRole=2;','unsigned sourceRole=2;\n    NpcCombatDamage damage=NpcCombatDamage::None;')
s=replace(s,'sizeof(CoopNpcCombat)==220','sizeof(CoopNpcCombat)==224')
s=replace(s,'inline bool npcCombatHealth', '''inline bool npcCombatGeometry(NpcCombatDamage damage,const EnemyMeleeGeometry& g){
    if(damage==NpcCombatDamage::Assassination)return emptyEnemyMeleeGeometry(g);
    return (damage==NpcCombatDamage::Sword||damage==NpcCombatDamage::Fatality)&&npcSwordGeometry(g);
}
inline bool npcCombatPotentiallyFatal(NpcCombatDamage damage,float before){return damage!=NpcCombatDamage::Sword||before<=10;}
inline bool npcCombatHealth''')
s=replace(s,'p.version!=1','p.version!=2')
s=replace(s,'p.sourceRole==2&&','p.sourceRole==2&&p.damage==NpcCombatDamage::None&&')
s=replace(s,'if(!npcSwordGeometry(p.geometry))return false;','if(!npcCombatGeometry(p.damage,p.geometry))return false;')
s=replace(s,'    // This first adapter is nonfatal. Native death requires retiring pose/AI\n    // ownership before callbacks, and is deliberately not simulated with HP.','    // Terminal native outcomes retain a scoped tombstone after pose/AI retirement.')
s=replace(s,'npcCombatHealth(p.after,p.maximum)&&std::isfinite(p.inflicted)&&p.inflicted>0&&p.after<p.before&&p.inflicted==p.before-p.after;', '''std::isfinite(p.after)&&p.after>=0&&p.after<p.before&&std::isfinite(p.inflicted)&&p.inflicted>0&&
        (p.after>0 ? p.damage==NpcCombatDamage::Sword&&p.inflicted==p.before-p.after :
        npcCombatPotentiallyFatal(p.damage,p.before)&&p.inflicted<=p.before&&p.before-p.inflicted<0.5f);''')
p.write_text(s)
p=S/'npc_combat.h';s=p.read_text()
s=replace(s,'// Initial generic NPC combat adapter: repeated nonfatal native sword hits.','// Experimental generic NPC melee authority, including native lethal outcomes.')
s=replace(s,'bool ready=false,enabled=true;','bool ready=false,enabled=true,terminal=false;')
s=replace(s,'std::uint64_t localHits=0,','std::uint64_t deaths=0,preDeathReleased=0,localHits=0,')
at='bool npcCombatExtract(NpcAuthorityChannel& c,void* info,EnemyMeleeGeometry& g,const float* capturedPosition){'
s=replace(s,at,'''constexpr char kNpcFatalityDamage[]="components/characters/player/base/damage/generic_fatality.damage";
const char* npcCombatDeclName(NpcCombatDamage damage){
    return damage==NpcCombatDamage::Sword?kPlayerSwordDamageDecl:damage==NpcCombatDamage::Fatality?kNpcFatalityDamage:damage==NpcCombatDamage::Assassination?kAssassinationDamage:nullptr;
}
bool npcCombatDeclaration(std::uintptr_t decl,NpcCombatDamage damage){
    if(damage==NpcCombatDamage::Sword)return nativePlayerSwordDeclaration(decl);
    if(damage==NpcCombatDamage::Assassination)return nativeAssassinationDeclaration(decl);
    if(damage!=NpcCombatDamage::Fatality)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,manager=0,name=0;std::uint64_t flags=0;float amount=0;unsigned reactions[2]{};std::string path;
    return decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==kNpcFatalityDamage&&
        memoryRead(decl+0x5c,&amount,4)&&amount==1000&&memoryRead(decl+0x60,&flags,8)&&flags==0x84&&
        memoryRead(decl+0x68,reactions,8)&&reactions[0]==2&&reactions[1]==2;
}
bool npcCombatExtract(NpcAuthorityChannel& c,void* info,NpcCombatDamage& damage,EnemyMeleeGeometry& g,const float* capturedPosition){''')
s=replace(s,'    if(!nativePlayerSwordDeclaration(decl)||amount!=10||flags!=0x80||attacker!=1||!enemyPlayerSword(inflictor))return false;', '''    if(attacker!=1||!enemyPlayerSword(inflictor))return false;
    if(amount==10&&flags==0x80)damage=NpcCombatDamage::Sword;
    else if(amount==1000&&flags==0x84)damage=NpcCombatDamage::Fatality;
    else if(amount==0&&flags==0x8c)damage=NpcCombatDamage::Assassination;
    else return false;
    if(!npcCombatDeclaration(decl,damage))return false;
    unsigned reactions[2]{};std::memcpy(reactions,raw.data()+0x18,8);
    const unsigned reaction=damage==NpcCombatDamage::Sword?1:damage==NpcCombatDamage::Fatality?2:0;
    if(reactions[0]!=reaction||reactions[1]!=reaction)return false;
    if(damage==NpcCombatDamage::Assassination){
        // Native absent-hit sentinels: canonicalize to no wire geometry;
        // constructors restore native defaults, never these process bytes.
        unsigned regions=0,region=0;float absent[9]{};std::memcpy(&regions,raw.data()+0x20,4);std::memcpy(&region,raw.data()+0x4c,4);std::memcpy(absent,raw.data()+0x24,36);
        if(regions||region!=1||absent[0]!=1e30f||absent[1]!=1e30f||absent[2]!=1e30f)return false;
        for(unsigned i=3;i<9;++i)if(absent[i]!=0)return false;
        g={};return true;
    }''')
start=s.index('bool npcCombatReadResult(');end=s.index('bool npcCombatApply(',start)
s=s[:start]+'''bool npcCombatRetireBeforeDeath(NpcAuthorityChannel& c){
    // Wait out any in-flight render/capture reader before releasing native
    // references. No Stop is sent ahead of the authoritative death commit.
    AcquireSRWLockExclusive(&npcCombatCaptureLock);
    npcCombatCaptureBindings.erase(std::remove_if(npcCombatCaptureBindings.begin(),npcCombatCaptureBindings.end(),[&](const auto& b){return b.arm==c.arm;}),npcCombatCaptureBindings.end());
    ReleaseSRWLockExclusive(&npcCombatCaptureLock);
    clearNpcPresentation(c.arm);
    AcquireSRWLockExclusive(&npcPresentationLock);
    npcCombatFixtureFrames.erase(std::remove_if(npcCombatFixtureFrames.begin(),npcCombatFixtureFrames.end(),[&](const auto& f){return f.native.npc.actor==c.lease.binding.npc.actor;}),npcCombatFixtureFrames.end());
    ReleaseSRWLockExclusive(&npcPresentationLock);
    auto& f=npcCombatFixture;
    for(auto it=f.leases.begin();it!=f.leases.end();){
        if(it->binding.npc.actor!=c.lease.binding.npc.actor){++it;continue;}
        releaseNpcAiLease(*it);f.released+=it->released;f.lost+=it->lost+it->retired;npcCombat.preDeathReleased+=it->released;
        const bool clean=!it->references&&!it->lost&&!it->retired;it=f.leases.erase(it);if(!clean)return false;
    }
    stopNpcChannel(c,"combat_native_death",false);npcCombat.preDeathReleased+=c.lease.released;
    return !c.lease.references&&!c.lease.lost&&!c.lease.retired;
}
bool npcCombatReadResult(NpcAuthorityChannel& c,const NativeHealth& before,void* result,NativeHealth& after,float& inflicted,bool terminal){
    unsigned protectedRegions=0,generation=0;float mana=0;const auto r=reinterpret_cast<std::uintptr_t>(result);const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!memoryRead(r,&inflicted,4)||!std::isfinite(inflicted)||inflicted<=0||!memoryRead(r+4,&mana,4)||mana!=0||
       !memoryRead(r+8,&protectedRegions,4)||protectedRegions||!readNativeHealth(memoryRead,base,before.handle,after)||
       after.game!=before.game||after.actor!=before.actor||after.component!=before.component||after.declaration!=before.declaration||
       !memoryRead(after.component+0x18,&generation,4)||generation!=c.lease.binding.healthGeneration||after.maximum!=before.maximum)return false;
    if(!terminal)return after.current>0&&before.current-after.current==inflicted&&npcCombatIdentity(c,after);
    // Health zero alone is insufficient: both native AI components must have
    // completed their stopped/dead state with no leftover pause ownership.
    return after.current==0&&inflicted<=before.current&&before.current-inflicted<0.5f&&
        readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[0])&&readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[1]);
}
''' + s[end:]
s=replace(s,'unsigned sourceRole,const EnemyMeleeGeometry& g,NativeHealth& after','unsigned sourceRole,NpcCombatDamage damage,const EnemyMeleeGeometry& g,NativeHealth& after')
s=replace(s,'!npcSwordGeometry(g)||before.current<=10||','!npcCombatGeometry(damage,g)||')
s=replace(s,'kPlayerSwordDamageDecl,false,false);\n    if(!nativePlayerSwordDeclaration(decl))return false;','npcCombatDeclName(damage),false,false);\n    if(!npcCombatDeclaration(decl,damage))return false;')
s=replace(s,'    std::memcpy(params.data()+0x20,&g.regions,4);','    if(damage!=NpcCombatDamage::Assassination){std::memcpy(params.data()+0x20,&g.regions,4);')
s=replace(s,'std::memcpy(params.data()+0x4c,&g.region,4);std::memcpy(params.data()+0x54,&attacker,4);','std::memcpy(params.data()+0x4c,&g.region,4);}\n    std::memcpy(params.data()+0x54,&attacker,4);')
s=replace(s,'    // Explicitly validated authoritative replay.', '''    const bool terminal=npcCombatPotentiallyFatal(damage,before.current);
    if(terminal&&!npcCombatRetireBeforeDeath(c))return false;
    NativeHealth recheck{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),before.handle,recheck)||recheck.actor!=before.actor||
       recheck.component!=before.component||recheck.game!=before.game||recheck.current!=before.current)return false;
    // Explicitly validated authoritative replay.''')
s=replace(s,'npcCombatReadResult(c,before,result.data(),after,inflicted);','npcCombatReadResult(c,before,result.data(),after,inflicted,terminal);')
s=replace(s,'std::uint64_t request,const EnemyMeleeGeometry& geometry','std::uint64_t request,NpcCombatDamage damage,const EnemyMeleeGeometry& geometry')
s=replace(s,'p.event=++b.event;p.geometry=geometry;','p.event=++b.event;p.damage=damage;p.geometry=geometry;')
s=replace(s,'npcCombat.outgoing.push_back(p);++npcCombat.committed;','npcCombat.outgoing.push_back(p);++npcCombat.committed;if(after.current==0){b.terminal=true;b.pending=0;++npcCombat.deaths;}')
s=replace(s,'if(!b.enabled||b.arm!=capture.arm)','if(!b.enabled||b.terminal||b.arm!=capture.arm)')
s=replace(s,'NativeHealth before{};EnemyMeleeGeometry geometry{};','NativeHealth before{};EnemyMeleeGeometry geometry{};NpcCombatDamage damage=NpcCombatDamage::None;')
s=replace(s,'        else if(before.current<=10)refusal="hit_potentially_fatal";\n','')
s=replace(s,'npcCombatExtract(*c,info,geometry,capture.position)','npcCombatExtract(*c,info,damage,geometry,capture.position)')
s=replace(s,'p.request=++b.request;p.geometry=geometry;','p.request=++b.request;p.damage=damage;p.geometry=geometry;')
s=replace(s,'npcCombatApply(*c,before,0,geometry','npcCombatApply(*c,before,0,damage,geometry')
s=replace(s,'npcCombatCommit(*c,b,0,0,geometry','npcCombatCommit(*c,b,0,0,damage,geometry')
s=replace(s,'if(b.ready&&b.peerNonce==p.sourceNonce)','if(b.terminal){++npcCombat.rejected;return;}\n        if(b.ready&&b.peerNonce==p.sourceNonce)')
s=replace(s,'    if(order!=NpcCombatOrder::Next)', '    if(b.terminal){++npcCombat.rejected;return;}\n    if(order!=NpcCombatOrder::Next)')
s=replace(s,'npcCombatApply(*c,before,p.sourceRole,p.geometry','npcCombatApply(*c,before,p.sourceRole,p.damage,p.geometry')
s=replace(s,'npcCombatCommit(*c,b,1,p.request,p.geometry','npcCombatCommit(*c,b,1,p.request,p.damage,p.geometry')
s=replace(s,'if(p.sourceRole==1)b.pending=0;','if(p.sourceRole==1)b.pending=0;\n        if(after.current==0){b.terminal=true;b.pending=0;++npcCombat.deaths;}')
s=replace(s,'if(!b.enabled)continue;auto* c=npcCombatActor(b.arm);NativeHealth h{};','if(!b.enabled||b.terminal)continue;auto* c=npcCombatActor(b.arm);NativeHealth h{};')
s=replace(s,'"active_nonfatal_sword"','"active_native_melee"')
s=replace(s,'if(b.enabled&&b.ready){auto* c=npcCombatActor(b.arm);','if(b.enabled&&b.ready&&!b.terminal){auto* c=npcCombatActor(b.arm);')
s=replace(s,'b.ready?"true":"false",b.event','b.ready?"true":"false",b.event') # insert diagnostic separately below
s=replace(s,'    fprintf(f,"],\\"captured\\":', '    fprintf(f,"],\\"deaths\\":%llu,\\"preDeathReleased\\":%llu",s.deaths,s.preDeathReleased);\n    fprintf(f,",\\"captured\\":')
s=replace(s,'\\"pending\\":%llu}', '\\"pending\\":%llu,\\"terminal\\":%s}')
s=replace(s,'b.event,b.request,b.peerRequest,b.pending);','b.event,b.request,b.peerRequest,b.pending,b.terminal?"true":"false");')
p.write_text(s)

p=S/'npc_combat_protocol_tests.h';s=p.read_text();s=replace(s,'p.after=47;p.inflicted=10;','p.after=47;p.inflicted=10;p.damage=NpcCombatDamage::Sword;')
s=replace(s,'    require(npcCombatOrder(0,1)', '''    auto fatal=p;fatal.damage=NpcCombatDamage::Fatality;fatal.after=0;fatal.inflicted=fatal.before;
    require(validNpcCombat(fatal,c,h),"native fatality result permits zero health only with full native inflicted outcome");
    auto assassin=fatal;assassin.damage=NpcCombatDamage::Assassination;assassin.geometry={};
    require(validNpcCombat(assassin,c,h),"assassination carries no fabricated hit geometry");
    auto invalid=assassin;invalid.geometry.direction[0]=1;require(!validNpcCombat(invalid,c,h),"assassination does not borrow ordinary geometry");
    invalid=fatal;invalid.after=20;invalid.inflicted=37;require(!validNpcCombat(invalid,c,h),"instant kill cannot carry a nonterminal health outcome");
    invalid=fatal;invalid.damage=NpcCombatDamage::None;require(!validNpcCombat(invalid,c,h),"unknown damage kind rejected");
    invalid=fatal;invalid.version=1;require(!validNpcCombat(invalid,c,h),"older nonfatal protocol cannot be interpreted as lethal protocol");
    auto lastSword=p;lastSword.before=7;lastSword.after=0;lastSword.inflicted=7;require(validNpcCombat(lastSword,c,h),"ordinary sword can deliver native last hit");
    lastSword.inflicted=6;require(!validNpcCombat(lastSword,c,h),"zero health cannot hide insufficient native damage");
    require(npcCombatOrder(0,1)''')
p.write_text(s)

for name in ('Build-NpcCombat-v865.ps1','Deploy-NpcCombat-v865.ps1','stage_npc_combat_v865.py'):
 s=(W/name).read_text().replace('v865','v869').replace('Build865','Build869').replace('Deploy865','Deploy869')
 if name.endswith('.py'):
  s=s.replace("P=W/'candidate-npc-combat-v863'","P=W/'candidate-npc-combat-v865'")
  s=s.replace('Experimental generic NPC ordinary sword authority:', 'Experimental native melee and terminal outcome authority:').replace('repeated nonfatal native damage','ordinary sword, generic fatality and assassination damage').replace('Unsupported or fatal damage stops this bounded adapter.','Native pose/AI references retire before death; scoped terminal tombstones reject duplicate damage. Runtime lethal verification pending.')
 (W/name.replace('v865','v869')).write_text(s)
print('Prepared candidate869 source and build/deploy scripts; no native mutations or deployment yet')
