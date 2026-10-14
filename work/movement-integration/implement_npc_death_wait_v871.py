from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';p=R/'outputs/d2-coop-lab/src/npc_combat.h';s=p.read_text()
def rep(a,b):
 global s
 assert a in s,a[:100]
 s=s.replace(a,b)
rep('bool ready=false,enabled=true,terminal=false;', '''bool ready=false,enabled=true,terminal=false,deathPending=false;
    ULONGLONG deathAt=0;unsigned deathSource=2;std::uint64_t deathRequest=0,deathEvent=0;
    NpcCombatDamage deathDamage=NpcCombatDamage::None;EnemyMeleeGeometry deathGeometry{};
    NativeHealth deathBefore{},deathAfter{};float deathInflicted=0;''')
rep('NpcCombatRuntime npcCombat{};', 'const char* npcCombatNativeStage="idle";float npcCombatNativeInflicted=0,npcCombatNativeAfter=0;unsigned npcCombatNativeGeneration=0;\nNpcCombatRuntime npcCombat{};')
rep('    if(!memoryRead(r,&inflicted,4)', '    npcCombatNativeStage="result_or_health_identity";\n    memoryRead(r,&npcCombatNativeInflicted,4);memoryRead(before.component+0x20,&npcCombatNativeAfter,4);memoryRead(before.component+0x18,&npcCombatNativeGeneration,4);\n    if(!memoryRead(r,&inflicted,4)')
rep('''    // Health zero alone is insufficient: both native AI components must have
    // completed their stopped/dead state with no leftover pause ownership.
    return after.current==0&&inflicted<=before.current&&before.current-inflicted<0.5f&&
        readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[0])&&readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[1]);''', '''    npcCombatNativeStage="await_native_death_completion";
    // This validates only the native damage result. Completion is separately
    // checked over following engine services, while this actor stays retired.
    return after.current==0&&inflicted<=before.current&&before.current-inflicted<0.5f;''')
rep('bool npcCombatApply(NpcAuthorityChannel& c,', '''bool npcCombatDeadProof(NpcAuthorityChannel& c,const NativeHealth& expected){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth h{};unsigned generation=0;
    return readNativeHealth(memoryRead,base,expected.handle,h)&&h.game==expected.game&&h.actor==expected.actor&&h.component==expected.component&&
        h.declaration==expected.declaration&&h.current==0&&h.maximum==expected.maximum&&
        memoryRead(h.component+0x18,&generation,4)&&generation==c.lease.binding.healthGeneration&&
        readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[0])&&readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[1]);
}
bool npcCombatApply(NpcAuthorityChannel& c,''')
rep('    if(!swordDamageApiMatches()||!npcCombatGeometry', '    npcCombatNativeStage="native_preconditions";\n    if(!swordDamageApiMatches()||!npcCombatGeometry')
rep('    if(terminal&&!npcCombatRetireBeforeDeath(c))return false;', '    npcCombatNativeStage="pre_death_ownership";\n    if(terminal&&!npcCombatRetireBeforeDeath(c))return false;')
rep('bool processNpcCombatCapture(NpcCombatCapture& capture){', '''void npcCombatDeferDeath(NpcCombatChannel& b,unsigned source,std::uint64_t request,std::uint64_t event,NpcCombatDamage damage,
        const EnemyMeleeGeometry& geometry,const NativeHealth& before,const NativeHealth& after,float inflicted){
    b.deathPending=true;b.deathAt=GetTickCount64();b.deathSource=source;b.deathRequest=request;b.deathEvent=event;
    b.deathDamage=damage;b.deathGeometry=geometry;b.deathBefore=before;b.deathAfter=after;b.deathInflicted=inflicted;
}
void serviceNpcCombatDeaths(ULONGLONG tick){
    for(auto& b:npcCombat.channels)if(b.enabled&&b.deathPending){
        auto* c=npcCombatActor(b.arm);if(!c){stopNpcCombat("death_binding_retired");return;}
        if(!npcCombatDeadProof(*c,b.deathAfter)){
            if(tick-b.deathAt>1500){stopNpcCombat("native_death_completion_timeout");return;}continue;
        }
        b.deathPending=false;npcCombatNativeStage="native_death_complete";
        if(!localRole){
            if(b.deathSource==1){b.peerRequest=b.deathRequest;++npcCombat.requests;}
            npcCombatCommit(*c,b,b.deathSource,b.deathRequest,b.deathDamage,b.deathGeometry,b.deathBefore,b.deathAfter,b.deathInflicted);
            if(!npcCombat.enabled)return;
        }else{b.event=b.deathEvent;b.terminal=true;b.pending=0;++npcCombat.applied;++npcCombat.deaths;}
    }
}
bool processNpcCombatCapture(NpcCombatCapture& capture){''')
rep('!b.enabled||b.terminal||b.arm!=capture.arm','!b.enabled||b.terminal||b.deathPending||b.arm!=capture.arm')
rep('        npcCombatCommit(*c,b,0,0,damage,geometry,before,after,inflicted);return true;', '''        if(after.current==0)npcCombatDeferDeath(b,0,0,0,damage,geometry,before,after,inflicted);
        else npcCombatCommit(*c,b,0,0,damage,geometry,before,after,inflicted);return true;''')
rep('    npcCombatLastInfo={};npcCombatLastOwnerFlags=0;', '    npcCombatNativeStage="idle";npcCombatNativeInflicted=npcCombatNativeAfter=0;npcCombatNativeGeneration=0;\n    npcCombatLastInfo={};npcCombatLastOwnerFlags=0;')
rep('if(b.terminal){++npcCombat.rejected;return;}','if(b.terminal||b.deathPending){++npcCombat.rejected;return;}')
rep('    const auto order=npcCombatOrder(', '''    if(b.deathPending){
        if((p.kind==NpcCombatMessage::Request&&p.request==b.deathRequest)||(p.kind==NpcCombatMessage::Commit&&p.event==b.deathEvent))++npcCombat.duplicates;
        else ++npcCombat.rejected;return;
    }
    const auto order=npcCombatOrder(''')
rep('    if(p.kind==NpcCombatMessage::Request){b.peerRequest=', '''    if(p.kind==NpcCombatMessage::Commit&&(after.current!=p.after||inflicted!=p.inflicted)){stopNpcCombat("native_replica_result_diverged");return;}
    if(after.current==0){npcCombatDeferDeath(b,p.sourceRole,p.request,p.event,p.damage,p.geometry,before,after,inflicted);return;}
    if(p.kind==NpcCombatMessage::Request){b.peerRequest=''')
rep('    // Drain commits before any new handshake health comparison.', '''    serviceNpcCombatDeaths(tick);if(!npcCombat.enabled)return;
    // Drain commits before any new handshake health comparison.''')
rep('if(!b.enabled||b.terminal)continue;','if(!b.enabled||b.terminal||b.deathPending)continue;')
rep('if(b.enabled&&b.ready&&!b.terminal)', 'if(b.enabled&&b.ready&&!b.terminal&&!b.deathPending)')
rep('\\"terminal\\":%s}', '\\"terminal\\":%s,\\"deathPending\\":%s}')
rep('b.terminal?"true":"false");','b.terminal?"true":"false",b.deathPending?"true":"false");')
rep('    const auto& fixture=npcCombatFixture;', '''    fprintf(f,",\\"nativeOutcome\\":{\\"stage\\":\\"%s\\",\\"inflicted\\":%.9g,\\"after\\":%.9g,\\"healthGeneration\\":%u}",npcCombatNativeStage,npcCombatNativeInflicted,npcCombatNativeAfter,npcCombatNativeGeneration);
    const auto& fixture=npcCombatFixture;''')
p.write_text(s)
for name in ('Build-NpcCombat-v869.ps1','Deploy-NpcCombat-v869.ps1','stage_npc_combat_v869.py'):
 s=(W/name).read_text().replace('v869','v871').replace('Build869','Build871').replace('Deploy869','Deploy871')
 if name.endswith('.py'):s=s.replace("P=W/'candidate-npc-combat-v865'","P=W/'candidate-npc-combat-v869'")
 (W/name.replace('v869','v871')).write_text(s)
p=R/'work/natural-moveset/Test-NpcCombat-v870.ps1';s=p.read_text().replace('candidate-npc-combat-v869','candidate-npc-combat-v871').replace('npc-combat-host-v870','npc-combat-host-v872')
s=s.replace("try{Snap 'failure-state'|Out-Null}catch{}", "try{Snap 'failure-state'|Out-Null;foreach($r in 'host','client'){foreach($ai in 'decision','reflex'){Save \"$out/failure-$r-$ai.json\" (Q $r \"--ai-$ai\" $handles[$r])}}}catch{}")
p.with_name('Test-NpcCombat-v872.ps1').write_text(s)
print('Prepared871 deferred native death completion and872 diagnostic harness')
