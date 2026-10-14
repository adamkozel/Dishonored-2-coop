#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <isteamnetworkingsockets.h>

template<class T> T symbol(HMODULE library,const char* name) {return reinterpret_cast<T>(GetProcAddress(library,name));}
int wmain(int argc,wchar_t** argv) {
    if(argc!=2) {std::fprintf(stderr,"Usage: d2_steam_probe full-path-to-original-steam_api64.dll\n");return 2;}
    HMODULE steam=LoadLibraryExW(argv[1],nullptr,LOAD_WITH_ALTERED_SEARCH_PATH);
    if(!steam)return 3;
    auto init=symbol<bool(*)()>(steam,"SteamAPI_Init");
    auto shutdown=symbol<void(*)()>(steam,"SteamAPI_Shutdown");
    auto user=symbol<int(*)()>(steam,"SteamAPI_GetHSteamUser");
    auto pipe=symbol<int(*)()>(steam,"SteamAPI_GetHSteamPipe");
    auto create=symbol<void*(*)(const char*)>(steam,"SteamInternal_CreateInterface");
    auto generic=symbol<void*(*)(void*,int,int,const char*)>(steam,"SteamAPI_ISteamClient_GetISteamGenericInterface");
    if(!init||!shutdown||!user||!pipe||!create||!generic)return 4;
    if(!init()){std::fprintf(stderr,"Steam initialization failed\n");return 5;}
    void* client=create("SteamClient017");
    auto* sockets=client?static_cast<ISteamNetworkingSockets*>(generic(client,user(),pipe(),STEAMNETWORKINGSOCKETS_INTERFACE_VERSION)):nullptr;
    std::printf("{\"interface\":\"%s\",\"available\":%s,\"userHandleValid\":%s,\"pipeHandleValid\":%s",STEAMNETWORKINGSOCKETS_INTERFACE_VERSION,sockets?"true":"false",user()?"true":"false",pipe()?"true":"false");
    bool success=false;
    if(sockets) {
        HSteamNetConnection pair[2]{};
        const bool created=sockets->CreateSocketPair(&pair[0],&pair[1],false,nullptr,nullptr);
        const char message[]="D2 native current Steam API transport probe";
        bool sent=false,received=false;
        if(created) {
            sent=sockets->SendMessageToConnection(pair[0],message,sizeof(message),k_nSteamNetworkingSend_Reliable,nullptr)==k_EResultOK;
            SteamNetworkingMessage_t* incoming=nullptr;
            if(sockets->ReceiveMessagesOnConnection(pair[1],&incoming,1)==1) {
                received=incoming->m_cbSize==sizeof(message)&&!std::memcmp(incoming->m_pData,message,sizeof(message));
                incoming->Release();
            }
            sockets->CloseConnection(pair[0],0,"Probe finished",false);
            sockets->CloseConnection(pair[1],0,"Probe finished",false);
        }
        std::printf(",\"nativeSocketPairCreated\":%s,\"sendPassed\":%s,\"receivePassed\":%s",created?"true":"false",sent?"true":"false",received?"true":"false");
        success=created&&sent&&received;
    }
    std::puts("}");shutdown();FreeLibrary(steam);
    return success?0:6;
}
