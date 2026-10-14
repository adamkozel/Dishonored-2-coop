#pragma once
#include <bcrypt.h>
// A fixed developer fixture, pre-staged in each isolated profile. No packet
// controls a path, save slot, file name, or file write.
char checkpointDirectory[MAX_PATH]{};
bool checkpointDigest(unsigned char result[32]) {
    if(!*checkpointDirectory)return false;
    BCRYPT_ALG_HANDLE algorithm=nullptr;BCRYPT_HASH_HANDLE hash=nullptr;
    if(BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_SHA256_ALGORITHM,nullptr,0)<0)return false;
    if(BCryptCreateHash(algorithm,&hash,nullptr,0,nullptr,0,0)<0){BCryptCloseAlgorithmProvider(algorithm,0);return false;}
    bool ok=true;
    for(const char* name:{"checkpoint.dat","game.details"}) {
        char path[MAX_PATH]{};
        if(std::snprintf(path,sizeof(path),"%s\\%s",checkpointDirectory,name)>=sizeof(path)){ok=false;break;}
        HANDLE file=CreateFileA(path,GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,nullptr);
        if(file==INVALID_HANDLE_VALUE){ok=false;break;}
        LARGE_INTEGER size{};
        if(!GetFileSizeEx(file,&size)||size.QuadPart<=0||size.QuadPart>64*1024*1024||
           BCryptHashData(hash,reinterpret_cast<PUCHAR>(&size.QuadPart),8,0)<0)ok=false;
        unsigned char buffer[32768]{};DWORD count=0;LONGLONG total=0;
        while(ok&&total<size.QuadPart) {
            if(!ReadFile(file,buffer,sizeof(buffer),&count,nullptr)||!count||BCryptHashData(hash,buffer,count,0)<0){ok=false;break;}
            total+=count;
        }
        if(total!=size.QuadPart)ok=false;
        CloseHandle(file);if(!ok)break;
    }
    if(ok)ok=BCryptFinishHash(hash,result,32,0)>=0;
    BCryptDestroyHash(hash);BCryptCloseAlgorithmProvider(algorithm,0);return ok;
}
