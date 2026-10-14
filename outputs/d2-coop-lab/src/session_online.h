#pragma once
// 1178 online session flow through native Steam only (the game's own
// steam_api64, SteamFriends015/SteamMatchMaking009/SteamUser019/SteamUtils008):
//  host: friends-only lobby for two, lobby data + rich presence "connect" so
//        Steam's own friends list / overlay offers Join Game and Invite, the
//        native overlay invite dialog, and a P2P listen socket (virtual port 0);
//  join: Steam "Join Game"/invite acceptance (GameLobbyJoinRequested_t) or the
//        "+connect_lobby <id>" launch argument -> JoinLobby -> ConnectP2P to the
//        lobby owner. Host plays Corvo (role 0), the joiner Emily (role 1).
// Lab mode (loopback IP, fixed roles) is unchanged unless a session command
// switches it; two-account validation is still outstanding.
#pragma pack(push,8)
struct OnlineLobbyCreated {std::int32_t result;std::uint64_t lobby;};
struct OnlineLobbyEnter {std::uint64_t lobby;std::uint32_t permissions;bool locked;std::uint32_t response;};
struct OnlineLobbyJoinRequested {std::uint64_t lobby;std::uint64_t friendId;};
#pragma pack(pop)
static_assert(sizeof(OnlineLobbyCreated)==16&&sizeof(OnlineLobbyJoinRequested)==16);
constexpr int kLobbyCreatedCallback=513,kLobbyEnterCallback=504,kGameLobbyJoinRequestedCallback=333;
struct OnlineSteam {
    bool ready=false;std::intptr_t friends=0,matchmaking=0,user=0,utils=0;
    std::uint64_t (*getSteamID)(std::intptr_t)=nullptr;
    std::uint64_t (*createLobby)(std::intptr_t,int,int)=nullptr;
    std::uint64_t (*joinLobby)(std::intptr_t,std::uint64_t)=nullptr;
    void (*leaveLobby)(std::intptr_t,std::uint64_t)=nullptr;
    std::uint64_t (*lobbyOwner)(std::intptr_t,std::uint64_t)=nullptr;
    int (*lobbyMembers)(std::intptr_t,std::uint64_t)=nullptr;
    bool (*setLobbyData)(std::intptr_t,std::uint64_t,const char*,const char*)=nullptr;
    const char* (*getLobbyData)(std::intptr_t,std::uint64_t,const char*)=nullptr;
    bool (*setRichPresence)(std::intptr_t,const char*,const char*)=nullptr;
    void (*clearRichPresence)(std::intptr_t)=nullptr;
    void (*inviteDialog)(std::intptr_t,std::uint64_t)=nullptr;
    bool (*callCompleted)(std::intptr_t,std::uint64_t,bool*)=nullptr;
    bool (*callResult)(std::intptr_t,std::uint64_t,void*,int,int,bool*)=nullptr;
    void (*registerCallback)(void*,int)=nullptr;void (*unregisterCallback)(void*)=nullptr;
};
OnlineSteam onlineSteam{};
enum class OnlinePhase:unsigned {Off,Creating,Hosting,Joining,Connecting,Connected,Failed};
struct OnlineSession {
    bool active=false,host=false,inviteOpened=false;OnlinePhase phase=OnlinePhase::Off;
    std::uint64_t self=0,lobby=0,owner=0,call=0,requestedLobby=0,joinRequests=0,lobbyMembers=0;
    HSteamListenSocket listener=0;
    const char* result="off";
};
OnlineSession onlineSession{};
// GameLobbyJoinRequested_t arrives through Steam's callback dispatch (the game
// pumps SteamAPI_RunCallbacks). Layout matches CCallbackBase: vptr, flags, id.
class OnlineJoinCallback {
public:
    virtual void Run(void* param){const auto* p=static_cast<const OnlineLobbyJoinRequested*>(param);if(p&&p->lobby){onlineSession.requestedLobby=p->lobby;++onlineSession.joinRequests;}}
    virtual void Run(void* param,bool,std::uint64_t){Run(param);}
    virtual int GetCallbackSizeBytes(){return sizeof(OnlineLobbyJoinRequested);}
    std::uint8_t flags=0;int id=kGameLobbyJoinRequestedCallback;
};
OnlineJoinCallback onlineJoinCallback;bool onlineJoinCallbackRegistered=false;
bool acquireOnlineSteam(){
    if(onlineSteam.ready)return true;
    HMODULE steam=GetModuleHandleW(L"steam_api64.dll");if(!steam)return false;
    auto user=symbol<int(*)()>(steam,"SteamAPI_GetHSteamUser");auto pipe=symbol<int(*)()>(steam,"SteamAPI_GetHSteamPipe");
    auto create=symbol<void*(*)(const char*)>(steam,"SteamInternal_CreateInterface");
    auto getFriends=symbol<std::intptr_t(*)(std::intptr_t,int,int,const char*)>(steam,"SteamAPI_ISteamClient_GetISteamFriends");
    auto getMatch=symbol<std::intptr_t(*)(std::intptr_t,int,int,const char*)>(steam,"SteamAPI_ISteamClient_GetISteamMatchmaking");
    auto getUser=symbol<std::intptr_t(*)(std::intptr_t,int,int,const char*)>(steam,"SteamAPI_ISteamClient_GetISteamUser");
    auto getUtils=symbol<std::intptr_t(*)(std::intptr_t,int,const char*)>(steam,"SteamAPI_ISteamClient_GetISteamUtils");
    if(!user||!pipe||!create||!getFriends||!getMatch||!getUser||!getUtils||!user()||!pipe())return false;
    const auto client=reinterpret_cast<std::intptr_t>(create("SteamClient017"));if(!client)return false;
    auto& s=onlineSteam;
    s.friends=getFriends(client,user(),pipe(),"SteamFriends015");s.matchmaking=getMatch(client,user(),pipe(),"SteamMatchMaking009");
    s.user=getUser(client,user(),pipe(),"SteamUser019");s.utils=getUtils(client,pipe(),"SteamUtils008");
    s.getSteamID=symbol<decltype(s.getSteamID)>(steam,"SteamAPI_ISteamUser_GetSteamID");
    s.createLobby=symbol<decltype(s.createLobby)>(steam,"SteamAPI_ISteamMatchmaking_CreateLobby");
    s.joinLobby=symbol<decltype(s.joinLobby)>(steam,"SteamAPI_ISteamMatchmaking_JoinLobby");
    s.leaveLobby=symbol<decltype(s.leaveLobby)>(steam,"SteamAPI_ISteamMatchmaking_LeaveLobby");
    s.lobbyOwner=symbol<decltype(s.lobbyOwner)>(steam,"SteamAPI_ISteamMatchmaking_GetLobbyOwner");
    s.lobbyMembers=symbol<decltype(s.lobbyMembers)>(steam,"SteamAPI_ISteamMatchmaking_GetNumLobbyMembers");
    s.setLobbyData=symbol<decltype(s.setLobbyData)>(steam,"SteamAPI_ISteamMatchmaking_SetLobbyData");
    s.getLobbyData=symbol<decltype(s.getLobbyData)>(steam,"SteamAPI_ISteamMatchmaking_GetLobbyData");
    s.setRichPresence=symbol<decltype(s.setRichPresence)>(steam,"SteamAPI_ISteamFriends_SetRichPresence");
    s.clearRichPresence=symbol<decltype(s.clearRichPresence)>(steam,"SteamAPI_ISteamFriends_ClearRichPresence");
    s.inviteDialog=symbol<decltype(s.inviteDialog)>(steam,"SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog");
    s.callCompleted=symbol<decltype(s.callCompleted)>(steam,"SteamAPI_ISteamUtils_IsAPICallCompleted");
    s.callResult=symbol<decltype(s.callResult)>(steam,"SteamAPI_ISteamUtils_GetAPICallResult");
    s.registerCallback=symbol<decltype(s.registerCallback)>(steam,"SteamAPI_RegisterCallback");
    s.unregisterCallback=symbol<decltype(s.unregisterCallback)>(steam,"SteamAPI_UnregisterCallback");
    s.ready=s.friends&&s.matchmaking&&s.user&&s.utils&&s.getSteamID&&s.createLobby&&s.joinLobby&&s.leaveLobby&&s.lobbyOwner&&s.lobbyMembers&&
        s.setLobbyData&&s.getLobbyData&&s.setRichPresence&&s.clearRichPresence&&s.inviteDialog&&s.callCompleted&&s.callResult&&s.registerCallback&&s.unregisterCallback;
    if(s.ready&&!onlineJoinCallbackRegistered){s.registerCallback(&onlineJoinCallback,kGameLobbyJoinRequestedCallback);onlineJoinCallbackRegistered=true;}
    return s.ready;
}
void closeOnlineTransport(const char* reason){
    if(sockets){if(connection)sockets->CloseConnection(connection,0,reason,false);if(onlineSession.listener)sockets->CloseListenSocket(onlineSession.listener);}
    connection=0;connectionState=0;lastReceive=0;remote={};onlineSession.listener=0;
}
void stopOnlineSession(const char* reason){
    auto& o=onlineSession;
    if(o.lobby&&onlineSteam.ready){onlineSteam.leaveLobby(onlineSteam.matchmaking,o.lobby);onlineSteam.clearRichPresence(onlineSteam.friends);}
    if(o.active)closeOnlineTransport(reason);
    const auto keep=o.joinRequests;o={};o.joinRequests=keep;o.result=reason;
}
void startOnlineHost(){
    if(!acquireOnlineSteam()){onlineSession.result="steam_interfaces_unavailable";return;}
    stopOnlineSession("restart");if(listener){sockets->CloseListenSocket(listener);listener=0;}
    auto& o=onlineSession;o.active=true;o.host=true;o.phase=OnlinePhase::Creating;o.self=onlineSteam.getSteamID(onlineSteam.user);
    localRole=0;outgoing.role=0;
    o.call=onlineSteam.createLobby(onlineSteam.matchmaking,1/*friends only*/,2);o.result=o.call?"creating_lobby":"create_lobby_refused";
    if(!o.call)o.phase=OnlinePhase::Failed;
}
void startOnlineJoin(std::uint64_t lobby){
    if(!acquireOnlineSteam()||!lobby){onlineSession.result="join_refused";return;}
    stopOnlineSession("restart");if(listener){sockets->CloseListenSocket(listener);listener=0;}
    auto& o=onlineSession;o.active=true;o.host=false;o.phase=OnlinePhase::Joining;o.self=onlineSteam.getSteamID(onlineSteam.user);
    localRole=1;outgoing.role=1;
    o.call=onlineSteam.joinLobby(onlineSteam.matchmaking,lobby);o.result=o.call?"joining_lobby":"join_lobby_refused";
    if(!o.call)o.phase=OnlinePhase::Failed;
}
// Steam launches a joining game with "+connect_lobby <id>" on its command line.
std::uint64_t onlineLaunchLobby(){
    const char* line=GetCommandLineA();const char* at=line?std::strstr(line,"+connect_lobby "):nullptr;
    return at?std::strtoull(at+15,nullptr,10):0;
}
void serviceOnlineSession(ULONGLONG tick){
    auto& o=onlineSession;
    if(o.requestedLobby&&acquireOnlineSteam()){const auto lobby=o.requestedLobby;o.requestedLobby=0;startOnlineJoin(lobby);}
    if(!o.active||!sockets)return;
    bool failed=false;
    if(o.phase==OnlinePhase::Creating&&onlineSteam.callCompleted(onlineSteam.utils,o.call,&failed)){
        OnlineLobbyCreated r{};
        if(failed||!onlineSteam.callResult(onlineSteam.utils,o.call,&r,sizeof(r),kLobbyCreatedCallback,&failed)||failed||r.result!=1||!r.lobby){o.phase=OnlinePhase::Failed;o.result="lobby_create_failed";return;}
        o.lobby=r.lobby;o.owner=o.self;char id[32]{};std::snprintf(id,sizeof(id),"%llu",r.lobby);char connect[64]{};std::snprintf(connect,sizeof(connect),"+connect_lobby %llu",r.lobby);
        onlineSteam.setLobbyData(onlineSteam.matchmaking,o.lobby,"d2coop","1171");onlineSteam.setLobbyData(onlineSteam.matchmaking,o.lobby,"role_host","corvo");
        onlineSteam.setRichPresence(onlineSteam.friends,"connect",connect);onlineSteam.setRichPresence(onlineSteam.friends,"status","Dishonored 2 co-op host (Corvo)");
        SteamNetworkingConfigValue_t options[3];options[0].SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,reinterpret_cast<void*>(&statusCallback));
        options[1].SetInt32(k_ESteamNetworkingConfig_SendRateMin,512*1024);options[2].SetInt32(k_ESteamNetworkingConfig_SendRateMax,512*1024);
        o.listener=sockets->CreateListenSocketP2P(0,3,options);
        o.phase=OnlinePhase::Hosting;o.result=o.listener?"hosting_lobby":"p2p_listen_failed";
        if(!o.listener){o.phase=OnlinePhase::Failed;return;}
        onlineSteam.inviteDialog(onlineSteam.friends,o.lobby);o.inviteOpened=true;
    }
    if(o.phase==OnlinePhase::Joining&&onlineSteam.callCompleted(onlineSteam.utils,o.call,&failed)){
        OnlineLobbyEnter r{};
        if(failed||!onlineSteam.callResult(onlineSteam.utils,o.call,&r,sizeof(r),kLobbyEnterCallback,&failed)||failed||r.response!=1||!r.lobby){o.phase=OnlinePhase::Failed;o.result="lobby_enter_failed";return;}
        o.lobby=r.lobby;o.owner=onlineSteam.lobbyOwner(onlineSteam.matchmaking,o.lobby);
        const char* tag=onlineSteam.getLobbyData(onlineSteam.matchmaking,o.lobby,"d2coop");
        if(!tag||std::strcmp(tag,"1171")){o.phase=OnlinePhase::Failed;o.result="lobby_version_mismatch";return;}
        if(!o.owner||o.owner==o.self){o.phase=OnlinePhase::Failed;o.result="lobby_owner_is_self";return;}
        SteamNetworkingIdentity identity{};identity.SetSteamID64(o.owner);
        SteamNetworkingConfigValue_t options[3];options[0].SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,reinterpret_cast<void*>(&statusCallback));
        options[1].SetInt32(k_ESteamNetworkingConfig_SendRateMin,512*1024);options[2].SetInt32(k_ESteamNetworkingConfig_SendRateMax,512*1024);
        connection=sockets->ConnectP2P(identity,0,3,options);o.phase=connection?OnlinePhase::Connecting:OnlinePhase::Failed;o.result=connection?"connecting_p2p":"connect_p2p_refused";
    }
    if(o.lobby)o.lobbyMembers=static_cast<std::uint64_t>(onlineSteam.lobbyMembers(onlineSteam.matchmaking,o.lobby));
    if((o.phase==OnlinePhase::Connecting||o.phase==OnlinePhase::Hosting)&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected){o.phase=OnlinePhase::Connected;o.result="connected";}
    (void)tick;
}
void writeOnlineSession(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.online.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& o=onlineSession;
    fprintf(f,"{\"steamReady\":%s,\"active\":%s,\"host\":%s,\"phase\":%u,\"result\":\"%s\",\"self\":\"%llu\",\"lobby\":\"%llu\",\"owner\":\"%llu\",\"lobbyMembers\":%llu,\"listener\":%u,\"connection\":%u,\"connectionState\":%d,\"inviteOpened\":%s,\"joinRequests\":%llu,\"launchLobby\":\"%llu\",\"role\":%u}\n",
        onlineSteam.ready?"true":"false",o.active?"true":"false",o.host?"true":"false",static_cast<unsigned>(o.phase),o.result,o.self,o.lobby,o.owner,o.lobbyMembers,o.listener,connection,connectionState,o.inviteOpened?"true":"false",o.joinRequests,onlineLaunchLobby(),localRole);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
