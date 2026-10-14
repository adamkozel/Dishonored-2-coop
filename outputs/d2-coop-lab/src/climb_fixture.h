#pragma once
// Isolated Dunwall discovery only. One original native climbable per world,
// spawned through the existing entity API. Native physics/interaction owns it.
// Checkpoint reload is the cleanup boundary: never delete a chain underneath
// an attached player, force a climbing state, or touch the original assets.
struct ClimbFixture {
    std::uintptr_t game=0,definition=0,actor=0;
    std::uint64_t epoch=0;
    unsigned handle=0,stage=0;
    bool attempted=false;
    const char* result="idle";
} climbFixture;
bool climbFixtureRequest=false;
constexpr const char* kClimbFixtureDefinition="models/environment/gameplay/climbable/chain_01/usable_sources/chain_01.def";
void writeClimbFixture(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};
    std::snprintf(path,sizeof(path),"%s.climb-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=climbFixture;
    fprintf(f,"{\"localExperiment\":true,\"attempted\":%s,\"actor\":\"0x%llx\",\"declaration\":\"0x%llx\",\"handle\":%u,\"worldEpoch\":%llu,\"stage\":%u,\"result\":\"%s\"}\n",p.attempted?"true":"false",static_cast<unsigned long long>(p.actor),static_cast<unsigned long long>(p.definition),p.handle,p.epoch,p.stage,p.result);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
bool climbFixtureApiMatches(){
    const unsigned char parameters[]={0xc7,0x01,0,0,0,0,0xc7,0x41,0x04,0x01,0x01,0,0,0x66,0xc7,0x41};
    const unsigned char position[]={0x8b,0x02,0x89,0x41,0x78,0x8b,0x42,0x04,0x89,0x41,0x7c,0x8b,0x42,0x08,0x89,0x81};
    const unsigned char axis[]={0x8b,0x02,0x89,0x81,0x84,0,0,0,0x8b,0x42,0x04,0x89,0x81,0x88,0,0};
    const unsigned char name[]={0x48,0x8b,0xc4,0x55,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8d,0x68,0x98,0x48};
    const unsigned char update[]={0x40,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0xd9,0x48,0x8b,0x89,0xf0,0,0,0};
    return powerFixtureApiMatches()&&rangedPropApiMatches()&&
        !std::memcmp(gameImage+0x146520,parameters,sizeof(parameters))&&
        !std::memcmp(gameImage+0x87c320,position,sizeof(position))&&
        !std::memcmp(gameImage+0x87c2c0,axis,sizeof(axis))&&
        !std::memcmp(gameImage+0x87b780,name,sizeof(name))&&
        !std::memcmp(gameImage+0x87c5d0,update,sizeof(update));
}
void serviceClimbFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& p=climbFixture;
    if(p.attempted&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};p.result="world_retired";writeClimbFixture();}
    if(!climbFixtureRequest)return;climbFixtureRequest=false;
    const auto refuse=[&](const char* reason){p.result=reason;writeClimbFixture();};
    if(p.attempted){refuse("already_attempted_in_world");return;}
    if(!climbFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!partnerAssetsEnabled||
       !coopGameplayReady(outgoing)||!observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+25000){refuse("unsupported_scope");return;}
    const auto definition=lookupEntityDefinition(kClimbFixtureDefinition);unsigned char flags=0;
    if(!definition||!memoryRead(definition+0xf0,&flags,1)||!(flags&0x20)){refuse("native_definition_not_spawnable");return;}
    NativePlayerStates player{};
    if(!readNativePlayerStates(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),player)||
       player.player.game!=observedGame||!nativePlayerGroundIdleStates(player,reinterpret_cast<std::uintptr_t>(gameImage))){refuse("native_player_not_grounded");return;}
    // Preserve an attempt record before calling into native code; an ambiguous
    // result never triggers a duplicate creation in this world.
    p.attempted=true;p.game=observedGame;p.epoch=outgoing.worldEpoch;p.definition=definition;p.stage=1;p.result="creating";writeClimbFixture();
    unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
    void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(p.game),reinterpret_cast<void*>(definition),parameters);
    p.actor=reinterpret_cast<std::uintptr_t>(actor);if(!actor){refuse("native_creation_refused");return;}
    p.stage=2;p.result="created_before_init";writeClimbFixture();
    const float position[3]{-33.0f,-151.8f,70.0f},axis[9]{1,0,0,0,1,0,0,0,1};
    reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,position);
    reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,axis);
    reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(p.game),actor,-1);
    p.stage=3;
    std::uintptr_t defAgain=0;
    if(!memoryRead(p.actor+0xa8,&p.handle,4)||!p.handle||p.handle==1||
       worldIdentityEntity(memoryRead,p.game,p.handle)!=p.actor||
       !memoryRead(p.actor+0x2e8,&defAgain,8)||defAgain!=p.definition){refuse("initialized_identity_unconfirmed");return;}
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0x87b780)(actor,"d2coop_climb_fixture");
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(actor);
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(actor);
    p.stage=4;p.result="native_fixture_initialized";writeClimbFixture();
    consoleOutput("COOP: original native climb fixture handle=%08x initialized; checkpoint reload required for removal\n",p.handle);
}
