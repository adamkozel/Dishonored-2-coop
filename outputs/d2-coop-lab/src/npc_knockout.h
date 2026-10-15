#pragma once
// 1213s (#46): a native NPC knockout through the game's own sleep damage. A
// choke leaves its victim unconscious with no damage declaration (take 1250:
// health swaps to the body's suppressed maximum, 40/40 -> 10/10, actor flag
// 0x20 at +0xd1, AI trees destroyed), so a client's choke on a host-owned NPC
// cannot ride the existing damage relay. The host instead applies
// sleep_no_reaction_damage (DAMAGETYPE_SLEEP | DAMAGETYPE_NO_REACTION) through
// the same native entry the relay uses. Unconscious = flag 0x20 with health > 0.
constexpr char kNpcKnockoutDecl[]="components/base/damage/sleep_no_reaction_damage.damage";
inline bool nativeNpcUnconscious(const NativeHealth& h){return (h.ownerFlags&0x20)&&h.current>0;}
bool applyNativeKnockout(const NativeHealth& before,unsigned attacker,NativeHealth& after,const char* declName=kNpcKnockoutDecl){
    if(!realNativeDamage||!before.component||nativeNpcUnconscious(before)||before.current<=0)return false;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,declName,false,false);
    std::uintptr_t vt=0;if(!decl||!memoryRead(decl,&vt,8)||vt!=reinterpret_cast<std::uintptr_t>(gameImage)+0x1c5d138)return false;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    std::memcpy(params.data()+0x54,&attacker,4);
    NativeHealth recheck{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),before.handle,recheck)||recheck.actor!=before.actor||
       recheck.component!=before.component||recheck.game!=before.game)return false;
    realNativeDamage(reinterpret_cast<void*>(before.component),params.data(),result.data());
    return readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),before.handle,after)&&after.actor==before.actor;
}
// Lab-only tests (coop_npc_combat knockout_test / knockout_reaction_test / knockout_generic_test): knock out the Dunwall upper-floor
// tutorial guard 2 once and record its health before and after.
struct NpcKnockoutTest{bool requested=false;const char* decl=kNpcKnockoutDecl;const char* result="idle";NativeHealth before{},after{};std::uint32_t handle=0;std::uint64_t runs=0;};
NpcKnockoutTest npcKnockoutTest;
void serviceNpcKnockoutTest(){
    auto& t=npcKnockoutTest;if(!t.requested)return;t.requested=false;++t.runs;t.result="refused";t.before={};t.after={};t.handle=0;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!observedGame){t.result="context_refused";return;}
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0;
    if(!readWorldNpcCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,unbound)){t.result="catalog_refused";return;}
    for(const auto& n:catalog)if(n.spawnerName=="ld_spawner_guard_tutorial_2"){if(t.handle){t.result="ambiguous_target";return;}t.handle=n.handle;}
    if(!t.handle){t.result="target_absent";return;}
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),t.handle,t.before)){t.result="health_refused";return;}
    if(!applyNativeKnockout(t.before,1,t.after,t.decl)){t.result="apply_refused";return;}
    t.result=nativeNpcUnconscious(t.after)?"unconscious":t.after.current<=0?"dead":"unchanged";
}
// Lab-only (coop_npc_combat hostworld_test, 1214r): host-world damage on a bound NPC
// entering the installed damage observer exactly as a creature's hit does (take 1270e:
// bloodfly damage_periodic, attacker and inflictor another NPC). Target: the bound,
// ready, conscious combat NPC nearest the host player; source: another streamed NPC.
constexpr char kNpcHostWorldTestDecl[]="components/characters/animals/bloodfly/damage_periodic.damage";
// 1214x (coop_npc_combat hostnative_test): the host player's own grenade explosion
// (explosion_grenade, attacker and inflictor the local player, native amount) on the
// same target: player damage the combat replay cannot reproduce (1214w).
constexpr char kNpcHostNativeTestDecl[]="components/equipment/damage/explosion_grenade.damage";
struct NpcHostWorldTest{bool requested=false,player=false;const char* result="idle";std::uint32_t target=0,source=0;NativeHealth before{},after{};float amount=0;std::uint64_t runs=0;};
NpcHostWorldTest npcHostWorldTest;
void serviceNpcHostWorldTest(){
    auto& t=npcHostWorldTest;if(!t.requested)return;t.requested=false;++t.runs;t.result="refused";t.before={};t.after={};t.target=t.source=0;t.amount=0;
    if(localRole||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!observedGame||!npcCombat.enabled){t.result="context_refused";return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);float best=1e12f;NpcAuthorityChannel* target=nullptr;
    for(const auto& b:npcCombat.channels){if(!b.enabled||!b.ready||b.terminal||b.deathPending)continue;auto* c=npcCombatActor(b.arm);NativeHealth h{};
        if(!c||c->unconscious||!npcCombatIdentity(*c,h)||h.current<=3)continue;
        float root[3]{};if(!memoryRead(c->lease.binding.npc.actor+0x300,root,12))continue;
        float d=0;for(unsigned i=0;i<3;++i){const float x=root[i]-outgoing.position[i];d+=x*x;}
        if(d<best){best=d;target=c;}}
    if(!target){t.result="no_bound_target";return;}
    if(t.player)t.source=1;
    else for(const auto& c:npcChannels)if(c.enabled&&c.arm!=target->arm&&c.lease.binding.npc.handle){t.source=c.lease.binding.npc.handle;break;}
    if(!t.source){t.result="no_source_npc";return;}
    t.target=target->lease.binding.npc.handle;
    if(!readNativeHealth(memoryRead,base,t.target,t.before)||t.before.actor!=target->lease.binding.npc.actor){t.result="health_refused";return;}
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,t.player?kNpcHostNativeTestDecl:kNpcHostWorldTestDecl,false,false);
    std::uintptr_t vt=0;if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1c5d138){t.result="declaration_refused";return;}
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    std::memcpy(&t.amount,params.data()+8,4);if(!(t.amount>0)||(!t.player&&t.amount>=t.before.current)){t.amount=t.player?5.f:2.f;std::memcpy(params.data()+8,&t.amount,4);}
    std::memcpy(params.data()+0x50,&t.source,4);std::memcpy(params.data()+0x54,&t.source,4);
    observeNativeDamage(reinterpret_cast<void*>(t.before.component),params.data(),result.data());
    if(!readNativeHealth(memoryRead,base,t.target,t.after)){t.result="after_refused";return;}
    t.result=t.after.current<t.before.current?"damaged":"unchanged";
}
void writeNpcHostWorldTest(){
    if(!*statusPath||!npcHostWorldTest.runs)return;const auto& t=npcHostWorldTest;
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-hostworld-test.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"runs\":%llu,\"mode\":\"%s\",\"result\":\"%s\",\"target\":%u,\"source\":%u,\"amount\":%.4f,\"before\":%.4f,\"after\":%.4f,\"maximum\":%.3f}",
        static_cast<unsigned long long>(t.runs),t.player?"hostnative":"hostworld",t.result,t.target,t.source,t.amount,t.before.current,t.after.current,t.before.maximum);
    const bool ok=!ferror(f);if(!fclose(f)&&ok)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
void writeNpcKnockoutTest(){
    if(!*statusPath||!npcKnockoutTest.runs)return;const auto& t=npcKnockoutTest;
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-knockout.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"runs\":%llu,\"decl\":\"%s\",\"result\":\"%s\",\"handle\":%u,\"before\":{\"current\":%.3f,\"maximum\":%.3f,\"ownerFlags\":%u},\"after\":{\"current\":%.3f,\"maximum\":%.3f,\"ownerFlags\":%u}}",
        static_cast<unsigned long long>(t.runs),t.decl,t.result,t.handle,t.before.current,t.before.maximum,unsigned(t.before.ownerFlags),t.after.current,t.after.maximum,unsigned(t.after.ownerFlags));
    const bool ok=!ferror(f);if(!fclose(f)&&ok)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
