#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include "native_skeleton.h"
#include "native_player_sword.h"
// Bounded, read-only capture for authoring. Never writes to or calls into D2.
int main(int argc,char** argv){
 if(argc!=6&&argc!=7){std::cerr<<"PID HANDLE SECONDS OUT actor|arm [publicationAddress]\n";return 2;}
 const auto pid=std::stoul(argv[1]),handle=std::stoul(argv[2]);const unsigned seconds=std::stoul(argv[3]);
 if(!seconds||seconds>20)return 2;
 const bool arm=std::string(argv[5])=="arm";
 const auto publication=argc==7?std::stoull(argv[6],nullptr,0):0;
 HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);
 MODULEENTRY32W m{};m.dwSize=sizeof(m);
 if(!Module32FirstW(snap,&m)||_wcsicmp(m.szModule,L"Dishonored2.exe")){CloseHandle(snap);return 3;}
 CloseHandle(snap);HANDLE proc=OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid);if(!proc)return 4;
 const auto read=[&](std::uintptr_t at,void* data,std::size_t n){SIZE_T done=0;return ReadProcessMemory(proc,reinterpret_cast<void*>(at),data,n,&done)&&done==n;};
 auto base=reinterpret_cast<std::uintptr_t>(m.modBaseAddr);IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};
 if(!read(base,&dos,sizeof(dos))||!read(base+dos.e_lfanew,&nt,sizeof(nt))||dos.e_magic!=IMAGE_DOS_SIGNATURE||nt.Signature!=IMAGE_NT_SIGNATURE||nt.FileHeader.TimeDateStamp!=0x67a50cde||nt.OptionalHeader.SizeOfImage!=68988928)return 5;
 std::ofstream out(argv[4]);if(!out)return 6;out<<std::setprecision(9);
 auto floats=[&](const float* a,unsigned n){out<<'[';for(unsigned i=0;i<n;i++)out<<(i?",":"")<<a[i];out<<']';};
 auto start=GetTickCount64(),next=start;unsigned accepted=0,rejected=0;std::uintptr_t world=0,actor=0;
 out<<"{\"startTick\":"<<start<<",\"source\":\"native read-only\",\"samples\":[";
 while(GetTickCount64()-start<seconds*1000ull){
  unsigned actual=handle;
  if(arm){NativePlayerSword sword{};if(!readNativePlayerSwordFrame(read,base,sword)||!sword.sword.equipped){rejected++;Sleep(5);continue;}actual=sword.arm.handle;}
  NativeSkeleton s{};std::uint64_t before=0,after=0;std::uintptr_t tracked=0;
  const bool first=!publication||(read(publication,&before,8)&&before&&!(before&1)&&read(publication+8,&tracked,8)&&tracked);
  if(first&&readNativeSkeleton(read,base,actual,s,arm?NativeSkeletonScope::PlayerArm:NativeSkeletonScope::Actor)&&s.stable&&
     (!publication||(tracked==s.animation&&read(publication,&after,8)&&before==after))){
   if(world&&(world!=s.game||actor!=s.actor))break;world=s.game;actor=s.actor;
   out<<(accepted?",":"")<<"{\"tick\":"<<GetTickCount64()<<",\"root\":";floats(s.root,12);out<<",\"local\":[";
   for(unsigned j=0;j<s.localPose.size();j++){out<<(j?",":"");floats(s.localPose[j].data(),16);}out<<"],\"preIk\":[";
   if(!s.phases.empty())for(unsigned j=0;j<s.phases[0].matrices.size();j++){out<<(j?",":"");floats(s.phases[0].matrices[j].data(),16);}out<<"]}";accepted++;
  }else rejected++;
  next+=20;auto now=GetTickCount64();if(next>now)Sleep(static_cast<DWORD>(next-now));else next=now;
 }
 out<<"],\"accepted\":"<<accepted<<",\"rejected\":"<<rejected<<"}\n";out.close();CloseHandle(proc);
 std::cout<<"accepted="<<accepted<<" rejected="<<rejected<<'\n';return accepted>=10?0:7;
}
