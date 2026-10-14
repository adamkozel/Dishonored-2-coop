from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
p=S/'door_sync.h';t=p.read_text(encoding='utf-8-sig');assert 'constexpr char kDoorSyncName' in t
t=t.replace('// Opt-in, one authored ordinary Dunwall door. This deliberately does not\n// claim key/lock, linked-door, breaking or campaign-wide world authority.\nconstexpr char kDoorSyncName[]="ld_door_350cm_int_rich_wood_01_3";', '// Opt-in authored ordinary-door catalogue for the current paired world.\n// Locked, linked, blocked, broken and ambiguous doors need separate authority.')
t=t.replace('epoch=0,peerEpoch=0,peerSequence=0','epoch=0,peerEpoch=0,map=0,peerSequence=0')
t=t.replace('} doorSync;', '''};
std::vector<DoorSync> doorChannels;
bool doorCatalogEnabled=false;
unsigned doorCatalogAmbiguous=0,doorCatalogIneligible=0,doorCatalogCursor=0,doorCatalogScanMs=0,doorServiceMs=0,doorServiceMaxMs=0;
std::uint64_t doorCatalogIgnored=0,doorCatalogUnmatched=0,doorCatalogRejected=0;
const char* doorCatalogResult="disabled";''')
t=t.replace('CoopDoorPacket doorSyncLastState{},doorSyncLastRequest{};', 'CoopDoorPacket doorSyncLastAction{};')
t=t.replace('bool doorSyncContext(ULONGLONG tick){\n    const auto& s=doorSync;', 'bool doorSyncContext(const DoorSync& s,ULONGLONG tick){')
t=t.replace('outgoing.mapId==coopMapId(kGuardMap)', 'outgoing.mapId==s.map')
t=t.replace('void failDoorSync(const char* reason){doorSync.enabled=doorSync.ready=false;doorSync.result=reason;++doorSync.rejected;}', 'void failDoorSync(DoorSync& s,const char* reason){s.enabled=s.ready=false;s.result=reason;++s.rejected;}')
a=t.index('bool readBoundDoor(');b=t.index('bool doorNativeDesired(',a)
t=t[:a]+'''bool readBoundDoor(const DoorSync& s,NativeOpenable& current){
    const auto& b=s.binding;
    return readNativeOpenable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,b.handle,current)&&sameNativeOpenableIdentity(current,b);
}
'''+t[b:]
a=t.index('void enableDoorSync(');b=t.index('bool sendDoorPacket(',a)
t=t[:a]+'''void enableDoorSync(bool on){
    if(!on){doorCatalogEnabled=false;doorSyncDuplicateRequest=false;for(auto& s:doorChannels){s.enabled=s.ready=false;s.result="disabled";}doorCatalogResult="disabled";return;}
    if(doorCatalogEnabled){++doorCatalogRejected;return;}
    const auto started=GetTickCount64();doorChannels.clear();doorCatalogCursor=0;doorSyncLastAction={};doorSyncDuplicateRequest=false;
    doorCatalogIgnored=doorCatalogUnmatched=doorCatalogRejected=0;doorCatalogAmbiguous=doorCatalogIneligible=0;doorServiceMs=doorServiceMaxMs=0;
    const auto refuse=[](const char* reason){doorCatalogResult=reason;++doorCatalogRejected;};
    std::uintptr_t list=0;unsigned count=0;
    if(!nativeDoorQueueAvailable()||!observedGame||!memoryRead(observedGame+0x601c0,&list,8)||!list||
       !memoryRead(observedGame+0x601c8,&count,4)||(count&0xffffff)>16384){refuse("catalog_unavailable");return;}
    std::vector<unsigned> handles(count&0xffffff);if(!handles.empty()&&!memoryRead(list,handles.data(),handles.size()*4)){refuse("catalog_unreadable");return;}
    std::vector<NativeOpenable> all;
    for(auto h:handles){NativeOpenable n{};if(readNativeOpenable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,h,n))all.push_back(std::move(n));}
    auto catalog=selectNativeOpenableCatalog(std::move(all));doorCatalogAmbiguous=catalog.ambiguous;doorCatalogIneligible=catalog.ineligible;
    if(catalog.entries.empty()||catalog.entries.size()>128){refuse("eligible_catalog_empty_or_overflow");return;}
    for(auto& n:catalog.entries){DoorSync fresh{};fresh.enabled=true;fresh.session=outgoing.session;fresh.peerSession=remote.session;
        fresh.epoch=outgoing.worldEpoch;fresh.peerEpoch=remote.worldEpoch;fresh.map=outgoing.mapId;fresh.binding=std::move(n);
        fresh.observedTarget=fresh.binding.current;fresh.observedSide=fresh.binding.openSide;fresh.arm=++doorSyncArm;fresh.result="awaiting_peer";
        if(!doorSyncContext(fresh,GetTickCount64())){doorChannels.clear();refuse("isolated_pair_context_required");return;}
        doorChannels.push_back(std::move(fresh));
    }
    doorCatalogScanMs=static_cast<unsigned>(GetTickCount64()-started);doorCatalogEnabled=true;doorCatalogResult="ordinary_door_catalog_armed";
}
'''+t[b:]
t=t.replace('bool sendDoorPacket(DoorMessage kind,ULONGLONG tick){\n    auto& s=doorSync;if(!doorSyncContext(tick))return false;', 'bool sendDoorPacket(DoorSync& s,DoorMessage kind,ULONGLONG tick){\n    if(!doorSyncContext(s,tick))return false;')
t=t.replace('if(kind==DoorMessage::State)doorSyncLastState=p;\n    if(kind==DoorMessage::Request)doorSyncLastRequest=p;', 'if(kind==DoorMessage::State||kind==DoorMessage::Request)doorSyncLastAction=p;')
t=t.replace('void receiveDoorPacket(const CoopDoorPacket& p,ULONGLONG tick){\n    auto& s=doorSync;++s.received;', 'void receiveDoorChannel(DoorSync& s,const CoopDoorPacket& p,ULONGLONG tick){\n    ++s.received;')
t=t.replace('doorSyncContext(tick)', 'doorSyncContext(s,tick)').replace('failDoorSync("','failDoorSync(s,"')
t=t.replace('void serviceDoorSync(ULONGLONG tick){\n    if(doorSyncEnableRequest>=0){const bool on=doorSyncEnableRequest!=0;doorSyncEnableRequest=-1;enableDoorSync(on);}\n    auto& s=doorSync;if(!s.enabled)return;', 'void serviceDoorChannel(DoorSync& s,ULONGLONG tick){\n    if(!s.enabled)return;')
a=t.index('    if(doorSyncDuplicateRequest){');b=t.index('    if(tick-s.helloSentAt',a);t=t[:a]+t[b:]
t=t.replace('sendDoorPacket(DoorMessage::', 'sendDoorPacket(s,DoorMessage::').replace('readBoundDoor(n)', 'readBoundDoor(s,n)').replace('readBoundDoor(fresh)', 'readBoundDoor(s,fresh)')
a=t.index('void writeDoorSync(){');old=t[a:]
fmt=old[old.index('    fprintf(f,'):old.index('    const bool ok=')]
fmt=fmt.replace('"{\\"experimental\\":true,\\"role\\":%u,','",\\"role\\":%u,').replace('}\\n",localRole','}",localRole')
t=t[:a]+'''void receiveDoorPacket(const CoopDoorPacket& p,ULONGLONG tick){
    if(!doorCatalogEnabled){++doorCatalogIgnored;return;}
    if(!validCoopDoor(p,outgoing,remote)){++doorCatalogRejected;return;}
    for(auto& s:doorChannels)if(s.binding.name==p.name&&s.binding.definitionName==p.definition){receiveDoorChannel(s,p,tick);return;}
    ++doorCatalogUnmatched;
}
void serviceDoorSync(ULONGLONG tick){
    if(doorSyncEnableRequest>=0){const bool on=doorSyncEnableRequest!=0;doorSyncEnableRequest=-1;enableDoorSync(on);}
    if(!doorCatalogEnabled)return;
    if(doorSyncDuplicateRequest){doorSyncDuplicateRequest=false;bool sent=false;const auto& p=doorSyncLastAction;
        for(auto& s:doorChannels)if(s.binding.name==p.name&&s.binding.definitionName==p.definition&&doorSyncContext(s,tick)&&
            p.sequence&&p.sourceArm==s.arm&&p.targetArm==s.peerArm&&validCoopDoor(p,remote,outgoing)){
            if(sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++s.sent;sent=true;}break;}
        if(!sent)++doorCatalogRejected;
    }
    const auto started=GetTickCount64();
    for(unsigned work=0;work<doorChannels.size();++work){
        auto& s=doorChannels[doorCatalogCursor];doorCatalogCursor=(doorCatalogCursor+1)%static_cast<unsigned>(doorChannels.size());
        serviceDoorChannel(s,tick);if(GetTickCount64()-started>=3)break;
    }
    doorServiceMs=static_cast<unsigned>(GetTickCount64()-started);doorServiceMaxMs=(std::max)(doorServiceMaxMs,doorServiceMs);
    bool live=false;for(const auto& s:doorChannels)live=live||s.enabled;
    if(!live){doorCatalogEnabled=false;doorCatalogResult="all_channels_retired";}
}
void writeDoorSync(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.door-sync.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\\"experimental\\":true,\\"enabled\\":%s,\\"catalogCount\\":%zu,\\"ambiguous\\":%u,\\"ineligible\\":%u,\\"ignoredWhileDisabled\\":%llu,\\"unmatched\\":%llu,\\"rejected\\":%llu,\\"scanMs\\":%u,\\"serviceMs\\":%u,\\"serviceMaxMs\\":%u,\\"result\\":\\"%s\\",\\"entries\\":[",doorCatalogEnabled?"true":"false",doorChannels.size(),doorCatalogAmbiguous,doorCatalogIneligible,doorCatalogIgnored,doorCatalogUnmatched,doorCatalogRejected,doorCatalogScanMs,doorServiceMs,doorServiceMaxMs,doorCatalogResult);
    bool first=true;for(const auto& s:doorChannels){if(!first)fputc(',',f);first=false;fputs("{\\"name\\":",f);writeWorldText(f,s.binding.name.c_str());fputs(",\\"definition\\":",f);writeWorldText(f,s.binding.definitionName.c_str());
'''+fmt+'''    }
    fputs("]}\\n",f);const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
'''
assert 'auto& s=doorSync' not in t and 'kDoorSyncName' not in t and 'failDoorSync("' not in t
p.write_text(t,encoding='utf-8')
p=S/'engine_bridge.cpp';t=p.read_text(encoding='utf-8-sig').replace('else ++doorSync.rejected;', 'else ++doorCatalogRejected;').replace('serviceDoorFixture(tick);serviceDoorSync(tick);','serviceDoorFixture(tick);timedNativeService("serviceDoorSync",[&]{serviceDoorSync(tick);});');p.write_text(t,encoding='utf-8')
for name in ['Build-DoorSync-v716.ps1','Deploy-DoorSync-v716.ps1','stage_door_sync_v716.py']:
    t=(W/name).read_text(encoding='utf-8-sig').replace('716','725').replace('candidate-door-sync-v725','candidate-door-catalog-v725')
    if name.startswith('stage_'):
        t=t.replace("P=W/'candidate-door-api-v713'","P=W/'candidate-door-sync-v716'")
        start=t.index("scope='");end=t.index("')",start);t=t[:start]+"scope='Authored ordinary-door catalog: unique native names and definitions, per-door native Steam host ordering, per-door lifetime and animation-generation pins, bounded round-robin service. No hardcoded door or map name. Locked/blocked/linked/broken/ambiguous doors excluded at arm; explicit isolated fixture still required. Runtime pending. Protocol69 and bank67/357 unchanged."+t[end:]
    (W/name.replace('716','725')).write_text(t,encoding='utf-8')
print('Prepared725 generic authored ordinary-door channels with retained716 archive')
