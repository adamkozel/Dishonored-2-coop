#pragma once
// Included in the engine bridge's private namespace. This milestone samples,
// sends and resolves authored NPC identities; it never changes game memory.
bool worldObservation=false;
struct WorldObservationEntry {CoopWorldNpcPacket packet{};std::uint32_t localHandle=0;ULONGLONG tick=0;};
std::vector<NativeWorldNpc> worldCatalog;
std::vector<WorldObservationEntry> worldEntries;
std::uint64_t worldSourceSession=0,worldTargetSession=0,worldSourceEpoch=0,worldTargetEpoch=0,worldMap=0;
std::uint64_t worldSequence=0,worldSent=0,worldReceived=0,worldMatched=0,worldRejected=0,worldUnmatched=0;
ULONGLONG worldCatalogTick=0,worldLastSend=0,worldLastDump=0;
unsigned worldUnbound=0,worldScanMs=0,worldScanMaxMs=0;
unsigned worldSampleMs=0,worldSampleMaxMs=0;
void resetWorldObservation() {
    worldCatalog.clear();worldEntries.clear();worldCatalogTick=0;worldUnbound=0;
    worldSourceSession=worldTargetSession=worldSourceEpoch=worldTargetEpoch=worldMap=0;
}
bool worldObservationReady(ULONGLONG tick) {
    return worldObservation&&observedGame&&connectionState==k_ESteamNetworkingConnectionState_Connected&&
        lastReceive&&tick-lastReceive<1000&&coopWorldPairReady(outgoing,remote);
}
void prepareWorldObservation(ULONGLONG tick) {
    if(!worldObservationReady(tick)){resetWorldObservation();return;}
    const auto& source=localRole?remote:outgoing;const auto& target=localRole?outgoing:remote;
    if(worldSourceSession!=source.session||worldTargetSession!=target.session||worldSourceEpoch!=source.worldEpoch||
       worldTargetEpoch!=target.worldEpoch||worldMap!=source.mapId) {
        resetWorldObservation();worldSourceSession=source.session;worldTargetSession=target.session;
        worldSourceEpoch=source.worldEpoch;worldTargetEpoch=target.worldEpoch;worldMap=source.mapId;
    }
    if(tick-worldCatalogTick<500)return;
    worldCatalogTick=tick;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()){resetWorldObservation();return;}
    const auto started=GetTickCount64();
    if(!readWorldNpcCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,worldCatalog,worldUnbound)||worldCatalog.size()>128)
        worldCatalog.clear();
    worldScanMs=static_cast<unsigned>(GetTickCount64()-started);worldScanMaxMs=(std::max)(worldScanMaxMs,worldScanMs);
}
void receiveWorldNpc(const CoopWorldNpcPacket& p,ULONGLONG tick) {
    ++worldReceived;
    if(!worldObservationReady(tick)||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||
       !validCoopWorldNpc(p,outgoing,remote)){++worldRejected;return;}
    // A context change seen during this receive loop invalidates cached local
    // identities even before the next catalog scan.
    if(worldSourceSession!=p.sourceSession||worldTargetSession!=p.targetSession||worldSourceEpoch!=p.sourceWorldEpoch||
       worldTargetEpoch!=p.targetWorldEpoch||worldMap!=p.mapId){++worldRejected;return;}
    NativeWorldNpc current{};unsigned matches=0;
    for(const auto& n:worldCatalog)if(n.unique&&n.spawnerName==p.spawner&&n.definition==p.definition) {
        if(!readWorldNpcIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,n.handle,current)||
           !current.unique||current.spawnerName!=n.spawnerName||current.definition!=n.definition)continue;
        ++matches;
    }
    if(matches!=1){++worldUnmatched;return;}
    for(auto& e:worldEntries)if(!std::strcmp(e.packet.spawner,p.spawner)) {
        if(p.sequence<=e.packet.sequence){++worldRejected;return;}
        e={p,current.handle,tick};++worldMatched;return;
    }
    if(worldEntries.size()>=128){++worldRejected;return;}
    worldEntries.push_back({p,current.handle,tick});++worldMatched;
}
void sendWorldObservation(ULONGLONG tick) {
    if(!worldObservationReady(tick)){resetWorldObservation();return;}
    if(localRole||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||tick-worldLastSend<500||worldCatalog.empty())return;
    if(bulkTransportCongested(tick)){++bulkCongestionSkips;return;}  // 1203e: retry next frame
    worldLastSend=tick;worldEntries.clear();const auto started=GetTickCount64();
    for(const auto& n:worldCatalog) {
        if(!n.unique)continue;
        NativeWorldNpc current{};
        if(!readWorldNpcIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,n.handle,current)||
           !current.unique||current.spawnerName!=n.spawnerName||current.definition!=n.definition)continue;
        CoopWorldNpcPacket p{};p.sequence=++worldSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
        p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceHandle=current.handle;
        std::memcpy(p.spawner,current.spawnerName.c_str(),current.spawnerName.size());
        std::memcpy(p.definition,current.definition.c_str(),current.definition.size());
        std::memcpy(p.position,current.position,sizeof(p.position));std::memcpy(p.orientation,current.orientation,sizeof(p.orientation));
        NativeHealth health{};
        if(readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),current.handle,health)&&
           health.game==observedGame&&health.actor==worldIdentityEntity(memoryRead,observedGame,current.handle)&&health.logic==1){
            p.healthValid=1;p.health=health.current;p.maximumHealth=health.maximum;
        }
        if(!validCoopWorldNpc(p,remote,outgoing))continue;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK) {
            ++worldSent;worldEntries.push_back({p,current.handle,tick});
        }
    }
    worldSampleMs=static_cast<unsigned>(GetTickCount64()-started);worldSampleMaxMs=(std::max)(worldSampleMaxMs,worldSampleMs);
}
void writeWorldText(FILE* file,const char* text) {
    fputc('"',file);for(;*text;++text){if(*text=='"'||*text=='\\')fputc('\\',file);fputc(*text,file);}fputc('"',file);
}
void writeWorldObservation(ULONGLONG tick) {
    if(tick-worldLastDump<250)return;worldLastDump=tick;
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.world.json",statusPath)>=sizeof(path)||
       std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* file=nullptr;if(fopen_s(&file,temp,"wb")||!file)return;
    fprintf(file,"{\"role\":%u,\"enabled\":%s,\"observationOnly\":true,\"protocolVersion\":2,\"packetBytes\":516,\"catalogCount\":%zu,\"unboundNpcs\":%u,\"sent\":%llu,\"received\":%llu,\"matched\":%llu,\"rejected\":%llu,\"unmatched\":%llu,\"scanMs\":%u,\"scanMaxMs\":%u,\"sampleMs\":%u,\"sampleMaxMs\":%u,\"entries\":[",
        localRole,worldObservation?"true":"false",worldCatalog.size(),worldUnbound,worldSent,worldReceived,worldMatched,worldRejected,worldUnmatched,worldScanMs,worldScanMaxMs,worldSampleMs,worldSampleMaxMs);
    bool first=true;
    if(worldObservationReady(tick))for(const auto& e:worldEntries) {
        if(tick-e.tick>=1500)continue;
        if(!first)fputc(',',file);first=false;
        fprintf(file,"{\"sequence\":%llu,\"sourceSession\":\"%llx\",\"targetSession\":\"%llx\",\"sourceEpoch\":%llu,\"targetEpoch\":%llu,\"mapId\":\"%llx\",\"sourceHandle\":%u,\"localHandle\":%u,\"ageMs\":%llu,\"spawner\":",
            e.packet.sequence,e.packet.sourceSession,e.packet.targetSession,e.packet.sourceWorldEpoch,e.packet.targetWorldEpoch,e.packet.mapId,e.packet.sourceHandle,e.localHandle,tick-e.tick);
        writeWorldText(file,e.packet.spawner);fputs(",\"definition\":",file);writeWorldText(file,e.packet.definition);
        fprintf(file,",\"position\":[%.6f,%.6f,%.6f],\"orientation\":[",e.packet.position[0],e.packet.position[1],e.packet.position[2]);
        for(unsigned i=0;i<9;++i)fprintf(file,"%s%.6f",i?",":"",e.packet.orientation[i]);
        fprintf(file,"],\"healthValid\":%s,\"health\":%.9g,\"maximumHealth\":%.9g}",e.packet.healthValid?"true":"false",e.packet.health,e.packet.maximumHealth);
    }
    fputs("]}\n",file);const bool ok=!ferror(file);const int closed=fclose(file);
    if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
