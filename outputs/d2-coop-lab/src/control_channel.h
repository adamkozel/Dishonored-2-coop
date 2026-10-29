#pragma once
// Stage 8.3 (#236): the control channel, from coop-anything's Steam P2P rules (chapter 114). One small reliable packet:
// - kind 1, every second while connected: the versioned hello and keep-alive. It carries the control protocol, the
//   game build, a hash of this DLL file and a loading flag. A peer with another protocol or DLL is refused with a
//   readable reason ("version_mismatch ...") instead of a silent stream of rejected packets.
// - kind 2, leave: sent before a deliberate close (F8, Leave co-op), so the peer can tell a leave from a crash.
// Liveness: Steam gives no reliable disconnect event, so a peer with no control packet for 20 s (120 s while it says it
// is loading, or before its first hello) is dropped by the application ("keepalive_timeout"). The transport timeout is
// set explicitly (kCoopTransportTimeoutMs). Status: <status>.control.json once a second.
#include <cstdint>
#include <cstdio>
#include <cstring>

constexpr std::uint32_t kCoopControlProtocol=1;   // bump with any packet format change between builds
constexpr ULONGLONG kCoopKeepAliveMs=1000,kCoopPeerSilentMs=20000,kCoopPeerLoadingMs=120000;
// The transport timeout stays at Steam's 10 s default, set explicitly: at 20 s a crashed partner was retired too late
// (peer-loss-dunwall-vm-v2009a), while a 5 s blackout survives 10 s (network-trouble-v2006b). The 20 s application
// drop is the backstop for a hung game whose Steam client still answers.
constexpr int kCoopTransportTimeoutMs=10000;
// Lab test only: D2COOP_TEST_CONTROL_PROTOCOL makes this game announce another protocol, to prove the mismatch refusal.
std::uint32_t coopAnnouncedProtocol=kCoopControlProtocol;

struct CoopControlPacket {
    std::uint32_t magic=0x54433244;   // "D2CT"
    std::uint16_t version=1;
    std::uint8_t kind=1,flags=0;      // flags: 1 loading, 2 host
    std::uint32_t protocol=kCoopControlProtocol,build=0x67a50cde;
    std::uint64_t dllHash=0,session=0;
    std::uint32_t sequence=0,reserved=0;
};
static_assert(sizeof(CoopControlPacket)==40,"control packet layout");

struct CoopControlState {
    HSteamNetConnection forConnection=0;
    ULONGLONG connectedAt=0,lastSent=0,lastReceived=0;
    std::uint32_t sequence=0,sentCount=0,receivedCount=0,refusedCount=0;
    std::uint32_t peerProtocol=0;std::uint64_t peerDll=0;
    bool peerLoading=false,peerLeft=false,helloOk=false;
    char result[160]="idle";
} coopControl;

inline std::uint64_t coopDllHash(){
    static std::uint64_t hash=0;static bool done=false;if(done)return hash;done=true;
    HMODULE module=nullptr;char path[MAX_PATH]{};
    if(!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           reinterpret_cast<LPCSTR>(&coopDllHash),&module)||!GetModuleFileNameA(module,path,MAX_PATH))return hash;
    FILE* f=nullptr;if(fopen_s(&f,path,"rb")||!f)return hash;
    std::uint64_t h=14695981039346656037ull;unsigned char buffer[65536];std::size_t n=0;
    while((n=std::fread(buffer,1,sizeof(buffer),f))>0)for(std::size_t i=0;i<n;++i){h^=buffer[i];h*=1099511628211ull;}
    std::fclose(f);hash=h;return hash;
}
inline bool coopLocalLoading(){return outgoing.pendingGameLogicState!=0;}
inline bool sendCoopControl(std::uint8_t kind){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    CoopControlPacket p{};p.kind=kind;p.flags=static_cast<std::uint8_t>((coopLocalLoading()?1:0)|(localRole==0?2:0));
    p.protocol=coopAnnouncedProtocol;p.dllHash=coopDllHash();p.session=outgoing.session;p.sequence=++coopControl.sequence;
    const bool ok=coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_Reliable,nullptr)==k_EResultOK;
    if(ok)++coopControl.sentCount;return ok;
}
// Sends the leave notice before a deliberate close; the close lingers so the notice is delivered.
inline void sendCoopLeave(){sendCoopControl(2);}

void endCoopConnection(ULONGLONG tick,const char* reason,const char* sessionResult);

inline void receiveCoopControl(const void* data,std::uint32_t size,ULONGLONG tick){
    CoopControlPacket p{};
    if(size!=sizeof(p)){++coopControl.refusedCount;return;}
    std::memcpy(&p,data,sizeof(p));
    if(p.version!=1||p.reserved){++coopControl.refusedCount;return;}
    ++coopControl.receivedCount;coopControl.lastReceived=tick;coopControl.peerProtocol=p.protocol;coopControl.peerDll=p.dllHash;
    coopControl.peerLoading=(p.flags&1)!=0;
    if(p.kind==2){coopControl.peerLeft=true;std::snprintf(coopControl.result,sizeof(coopControl.result),"partner_left");return;}
    if(p.protocol!=kCoopControlProtocol||p.build!=0x67a50cde||p.dllHash!=coopDllHash()){
        char reason[160]{};
        std::snprintf(reason,sizeof(reason),"version_mismatch protocol %u/%u dll %016llx/%016llx",kCoopControlProtocol,p.protocol,
            static_cast<unsigned long long>(coopDllHash()),static_cast<unsigned long long>(p.dllHash));
        std::snprintf(coopControl.result,sizeof(coopControl.result),"%s",reason);
        consoleOutput("COOP: %s\n",reason);
        endCoopConnection(tick,reason,"version_mismatch");return;
    }
    if(!coopControl.helloOk){coopControl.helloOk=true;std::snprintf(coopControl.result,sizeof(coopControl.result),"live");}
}

inline void serviceCoopControl(ULONGLONG tick){
    auto& c=coopControl;
    if(!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected){c.forConnection=0;return;}
    if(c.forConnection!=connection){
        const auto keep=c.sequence;c=CoopControlState{};c.sequence=keep;c.forConnection=connection;c.connectedAt=tick;
        std::snprintf(c.result,sizeof(c.result),"waiting_hello");
    }
    if(tick-c.lastSent>=kCoopKeepAliveMs){c.lastSent=tick;sendCoopControl(1);}
    const ULONGLONG since=c.lastReceived?c.lastReceived:c.connectedAt;
    const ULONGLONG limit=(!c.lastReceived||c.peerLoading)?kCoopPeerLoadingMs:kCoopPeerSilentMs;
    if(tick-since>limit){
        std::snprintf(c.result,sizeof(c.result),"keepalive_timeout");
        consoleOutput("COOP: partner silent for %llu ms, connection dropped\n",static_cast<unsigned long long>(tick-since));
        endCoopConnection(tick,"keepalive_timeout","keepalive_timeout");
    }
}

inline void writeCoopControlStatus(ULONGLONG tick){
    if(!*statusPath)return;
    static ULONGLONG lastWrite=0;if(tick-lastWrite<1000)return;lastWrite=tick;
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.control.json",statusPath)>=static_cast<int>(sizeof(path)))return;
    std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    const auto& c=coopControl;FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    std::fprintf(f,"{\"protocol\":%u,\"dllHash\":\"%016llx\",\"connection\":%u,\"result\":\"%s\",\"helloOk\":%s,\"peerLeft\":%s,\"peerLoading\":%s,"
        "\"peerProtocol\":%u,\"peerDll\":\"%016llx\",\"sent\":%u,\"received\":%u,\"refused\":%u,\"silentMs\":%llu}\n",
        kCoopControlProtocol,static_cast<unsigned long long>(coopDllHash()),connection,c.result,c.helloOk?"true":"false",c.peerLeft?"true":"false",
        c.peerLoading?"true":"false",c.peerProtocol,static_cast<unsigned long long>(c.peerDll),c.sentCount,c.receivedCount,c.refusedCount,
        static_cast<unsigned long long>(c.forConnection?tick-(c.lastReceived?c.lastReceived:c.connectedAt):0));
    std::fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
