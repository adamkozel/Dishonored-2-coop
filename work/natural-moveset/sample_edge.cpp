// Offline pose sampling using the exact installed executable's pure Edge math.
// DONT_RESOLVE_DLL_REFERENCES: no entry point, import initialization, actor, Steam,
// animation graph, event dispatcher, game process access or asset writes.
// The executable and input data are read only. Output belongs in the lab.
#include <windows.h>
#include <bcrypt.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <sstream>

using Bytes=std::vector<unsigned char>;
Bytes file(const std::filesystem::path& p) {
    std::ifstream f(p,std::ios::binary|std::ios::ate);
    if(!f)throw std::runtime_error("Input open failed");
    auto n=f.tellg();if(n<0||n>128*1024*1024)throw std::runtime_error("Input size");
    Bytes b(static_cast<size_t>(n));f.seekg(0);f.read(reinterpret_cast<char*>(b.data()),b.size());
    if(!f)throw std::runtime_error("Input read failed");return b;
}
std::string sha(const Bytes& bytes) {
    BCRYPT_ALG_HANDLE h=nullptr;BCRYPT_HASH_HANDLE hash=nullptr;DWORD n=0,size=0;
    if(BCryptOpenAlgorithmProvider(&h,BCRYPT_SHA256_ALGORITHM,nullptr,0))throw std::runtime_error("SHA provider");
    BCryptGetProperty(h,BCRYPT_OBJECT_LENGTH,reinterpret_cast<PUCHAR>(&size),4,&n,0);
    Bytes object(size);std::array<unsigned char,32> digest{};
    if(BCryptCreateHash(h,&hash,object.data(),size,nullptr,0,0)||
       BCryptHashData(hash,const_cast<PUCHAR>(bytes.data()),static_cast<ULONG>(bytes.size()),0)||
       BCryptFinishHash(hash,digest.data(),32,0))throw std::runtime_error("SHA failed");
    BCryptDestroyHash(hash);BCryptCloseAlgorithmProvider(h,0);
    std::ostringstream s;s<<std::hex<<std::setfill('0');for(auto c:digest)s<<std::setw(2)<<unsigned(c);return s.str();
}
template<class T>T at(const Bytes& b,size_t o) {
    if(o+sizeof(T)>b.size())throw std::runtime_error("Truncated header");
    T v;std::memcpy(&v,b.data()+o,sizeof(v));return v;
}
struct Aligned {
    unsigned char* data;size_t size;
    explicit Aligned(size_t n):size((n+31)&~size_t(15)){data=static_cast<unsigned char*>(_aligned_malloc(size,16));if(!data)throw std::bad_alloc();std::memset(data,0,size);}
    ~Aligned(){_aligned_free(data);}
};
using Evaluate=void(*)(const void*,const void*,void**,float);
DWORD exceptionCode=0;void* exceptionAddress=nullptr;
int captureException(EXCEPTION_POINTERS* e){exceptionCode=e->ExceptionRecord->ExceptionCode;exceptionAddress=e->ExceptionRecord->ExceptionAddress;return EXCEPTION_EXECUTE_HANDLER;}
bool safeEvaluate(Evaluate eval,const void* a,const void* s,void** out,float time) {
    __try{eval(a,s,out,time);return true;}__except(captureException(GetExceptionInformation())){return false;}
}
template<class T>void write(std::ofstream& out,const T& value){out.write(reinterpret_cast<const char*>(&value),sizeof(value));}
int wmain(int argc,wchar_t** argv) {
    try {
        if(argc!=5){std::cerr<<"d2_sample_edge EXE EDGEANIM EDGESKL OUTPUT\n";return 2;}
        const std::filesystem::path exe=argv[1],animation=argv[2],skeleton=argv[3],output=argv[4];
        if(std::filesystem::exists(output))throw std::runtime_error("Output exists; use a new file");
        const auto executable=file(exe);const std::string digest=sha(executable);
        if(digest!="b671f8249422862bf557d00aab2f4bb671ed4515cc10ad8a8a9decad84516180")throw std::runtime_error("Wrong executable hash");
        auto a=file(animation),s=file(skeleton);
        if(at<unsigned>(a,0)!=0x45413038||at<unsigned>(s,0)!=0x45533036)throw std::runtime_error("Wrong Edge format");
        const unsigned joints=at<unsigned short>(s,0x10),curves=at<unsigned short>(s,0x12),frames=at<unsigned short>(a,0x10);
        const unsigned animJoints=at<unsigned short>(a,0xe),chunks=at<unsigned short>(a,0x12);
        const float fps=at<float>(a,8);
        if(!joints||joints>512||animJoints>joints||!frames||frames>18000||!chunks||chunks>frames||curves>4096||!std::isfinite(fps)||fps<1||fps>240)throw std::runtime_error("Invalid dimensions");
        const auto bindOffset=at<unsigned>(s,0x18)+0x18;
        if(bindOffset+size_t(joints)*48>s.size())throw std::runtime_error("Invalid skeleton rest data");
        for(auto o:{0x38,0x3c,0x40,0x44,0x48,0x4c,0x50}){const auto v=at<unsigned>(a,o);if(v&&v+size_t(o)>=a.size())throw std::runtime_error("Invalid animation relative offset");}
        if(size_t(at<unsigned>(a,0x38))+0x38+chunks*8>a.size()||size_t(at<unsigned>(a,0x3c))+0x3c+chunks*4>a.size())throw std::runtime_error("Invalid chunk tables");
        // Aligned copies preserve embedded relative offsets and read-ahead padding.
        Aligned ab(a.size()+256),sb(s.size()+256),pose((joints+8)*48),curve((curves+8)*4),masks(32768),flags(16);
        std::memcpy(ab.data,a.data(),a.size());std::memcpy(sb.data,s.data(),s.size());
        HMODULE mapped=LoadLibraryExW(exe.c_str(),nullptr,DONT_RESOLVE_DLL_REFERENCES);
        if(!mapped)throw std::runtime_error("Read-only executable mapping failed");
        auto eval=reinterpret_cast<Evaluate>(reinterpret_cast<unsigned char*>(mapped)+0x12e2c30);
        // Six pointers mirror the result of native Edge stack GetPose(0).
        void* buffers[6]={pose.data,masks.data,curve.data,masks.data+8192,flags.data,masks.data+16384};
        std::ofstream out(output,std::ios::binary);if(!out)throw std::runtime_error("Output open failed");
        out.write("D2EDGE01",8);write(out,joints);write(out,frames);write(out,fps);unsigned reserved=0;write(out,reserved);
        for(unsigned frame=0;frame<frames;++frame) {
            if(!safeEvaluate(eval,ab.data,sb.data,buffers,float(frame)/fps)) {
                std::cerr<<"Native sampler exception 0x"<<std::hex<<exceptionCode<<" at RVA 0x"<<(reinterpret_cast<uintptr_t>(exceptionAddress)-reinterpret_cast<uintptr_t>(mapped))<<" frame "<<std::dec<<frame<<"\n";
                out.close();return 3;
            }
            const auto values=reinterpret_cast<const float*>(pose.data);
            for(unsigned j=0;j<joints;++j) {
                float norm=0;for(unsigned k=0;k<12;++k){float v=values[j*12+k];if(!std::isfinite(v)||std::fabs(v)>100000)throw std::runtime_error("Nonfinite/implausible pose");if(k<4)norm+=v*v;}
                if(std::fabs(norm-1)>.03)throw std::runtime_error("Invalid sampled quaternion");
            }
            out.write(reinterpret_cast<char*>(pose.data),size_t(joints)*48);
        }
        out.close();if(!out)throw std::runtime_error("Output write failed");FreeLibrary(mapped);
        std::cout<<"{\"joints\":"<<joints<<",\"frames\":"<<frames<<",\"fps\":"<<fps<<",\"animation_sha256\":\""<<sha(a)<<"\",\"skeleton_sha256\":\""<<sha(s)<<"\",\"method\":\"offline native Edge evaluator; no entrypoint or events\"}\n";
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
