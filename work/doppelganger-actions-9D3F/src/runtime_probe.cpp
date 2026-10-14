#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cmath>

// Read-only research tool: no injection, patches, hooks, or game function calls.
struct ProcessHandle {
    HANDLE value;
    ~ProcessHandle() { if (value && value != INVALID_HANDLE_VALUE) CloseHandle(value); }
};
bool read(HANDLE process, std::uintptr_t address, void* buffer, SIZE_T size) {
    SIZE_T done = 0;
    return ReadProcessMemory(process, reinterpret_cast<void*>(address), buffer, size, &done) && done == size;
}
std::string readString(HANDLE process, std::uintptr_t address) {
    std::string result;
    for (int i = 0; i < 512; ++i) {
        char ch = 0;
        if (!read(process, address + i, &ch, 1) || ch == 0) break;
        if (static_cast<unsigned char>(ch) < 32 || static_cast<unsigned char>(ch) > 126) return "";
        result += ch;
    }
    return result;
}
std::string jsonString(const std::string& value) {
    std::string result = "\"";
    for (char ch : value) {
        if (ch == '\\' || ch == '"') result += '\\';
        result += ch;
    }
    return result + '"';
}
std::string objectType(HANDLE process,std::uintptr_t base,std::uintptr_t pointer) {
    std::uintptr_t vt=0,locator=0;std::uint32_t type=0;
    if(!pointer||!read(process,pointer,&vt,8)||vt<base||vt>=base+68988928||
       !read(process,vt-8,&locator,8)||locator<base||locator>=base+68988928||
       !read(process,locator+12,&type,4)||type>=68988928)return "";
    return readString(process,base+type+16);
}
#include "bone_probe.h"
#include "animation_probe.h"
#include "world_identity.h"
#include "native_ai_state.h"
#include "native_character.h"
#include "native_gameover.h"
#include "native_player_stance.h"
#include "native_player_states.h"
#include "native_skeleton.h"
#include "native_health.h"
#include "native_sword_equipment.h"
#include "native_player_sword.h"
#include "native_faction.h"
#include "native_ai_targets.h"
// Read-only reconstruction of idAnimationLocal::GetJointName (23cb00).
// Interned IDs are process-local; these labels are diagnostic only.
bool jointNames(HANDLE process,std::uintptr_t base,const std::vector<std::uint16_t>& ids,std::vector<std::string>& out){
    std::uintptr_t manager=0,vt=0,entries=0,again=0;std::uint32_t count=0,countAgain=0;
    if(!read(process,base+0x22995c0,&manager,8)||!manager||!read(process,manager,&vt,8)||vt!=base+0x1b093c8||
       !read(process,manager+8,&entries,8)||!entries||!read(process,manager+0x10,&count,4)||!(count&0xffffff)||(count&0xffffff)>65535)return false;
    std::vector<std::string> names;std::vector<std::uintptr_t> pointers;
    for(auto id:ids){
        std::uintptr_t p=0;std::string name;
        if(id==0xffff||id>=(count&0xffffff)||!read(process,entries+static_cast<std::uintptr_t>(id)*72+16,&p,8)||
           !worldIdentityString([&](auto a,auto d,auto n){return read(process,a,d,n);},p,name)||name.empty()||name.size()>127)return false;
        names.push_back(std::move(name));pointers.push_back(p);
    }
    if(!read(process,base+0x22995c0,&again,8)||again!=manager||!read(process,manager+8,&again,8)||again!=entries||
       !read(process,manager+0x10,&countAgain,4)||countAgain!=count)return false;
    for(unsigned i=0;i<ids.size();++i)if(!read(process,entries+static_cast<std::uintptr_t>(ids[i])*72+16,&again,8)||again!=pointers[i])return false;
    out=std::move(names);return true;
}
int main(int argc, char** argv) {
    if (argc < 2 || argc > 4 || (argc==4&&std::strcmp(argv[2],"--loading-world")&&std::strcmp(argv[2],"--components")&&std::strcmp(argv[2],"--object")&&std::strcmp(argv[2],"--bones")&&std::strcmp(argv[2],"--skeleton")&&std::strcmp(argv[2],"--arm-skeleton")&&std::strcmp(argv[2],"--health")&&std::strcmp(argv[2],"--avatar-sword")&&std::strcmp(argv[2],"--avatar-sword-attachment")&&std::strcmp(argv[2],"--sword-equipment")&&std::strcmp(argv[2],"--sword-attachment")&&std::strcmp(argv[2],"--faction")&&std::strcmp(argv[2],"--ai-targets")&&std::strcmp(argv[2],"--animation")&&std::strcmp(argv[2],"--ai-decision")&&std::strcmp(argv[2],"--ai-reflex")&&std::strcmp(argv[2],"--ai-attack")&&std::strcmp(argv[2],"--ai-idle")&&std::strcmp(argv[2],"--npc-pose"))) { std::cerr << "Usage: d2_runtime_probe PID [--detail|--commands|--player|--player-stance|--player-states|--gameover|--character|--input|--entities|--world|--world-npcs|--logic|--cinematic|--midnight|--kiscule|--loading-world ADDRESS|--components HANDLE|--object ADDRESS|--bones HANDLE|--skeleton HANDLE|--arm-skeleton HANDLE|--health HANDLE|--faction HANDLE|--sword-equipment HANDLE|--sword-attachment HANDLE|--avatar-sword HANDLE|--avatar-sword-attachment HANDLE|--ai-targets HANDLE|--animation HANDLE|--ai-decision HANDLE|--ai-reflex HANDLE|--ai-attack HANDLE|--ai-idle HANDLE|--npc-pose HANDLE]\n"; return 2; }
    const bool detail = argc == 3 && std::strcmp(argv[2], "--detail") == 0;
    const DWORD pid = std::stoul(argv[1]);
    ProcessHandle snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid)};
    MODULEENTRY32W module{};
    module.dwSize = sizeof(module);
    if (!Module32FirstW(snapshot.value, &module)) { std::cerr << "Module snapshot failed: " << GetLastError() << '\n'; return 3; }
    if (_wcsicmp(module.szModule, L"Dishonored2.exe") != 0) { std::cerr << "Target is not Dishonored2.exe\n"; return 4; }
    ProcessHandle process{OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid)};
    if (!process.value) { std::cerr << "Read access failed: " << GetLastError() << '\n'; return 5; }
    const auto base = reinterpret_cast<std::uintptr_t>(module.modBaseAddr);
    IMAGE_DOS_HEADER dos{};
    IMAGE_NT_HEADERS64 nt{};
    if (!read(process.value, base, &dos, sizeof(dos)) || !read(process.value, base + dos.e_lfanew, &nt, sizeof(nt))) return 6;
    if (dos.e_magic != IMAGE_DOS_SIGNATURE || nt.Signature != IMAGE_NT_SIGNATURE ||
        nt.FileHeader.TimeDateStamp != 0x67a50cde || nt.OptionalHeader.SizeOfImage != 68988928) {
        std::cerr << "Unsupported game image fingerprint\n"; return 10;
    }
    if(argc==4 && std::strcmp(argv[2],"--loading-world")==0) {
        char* end=nullptr;const auto game=static_cast<std::uintptr_t>(std::strtoull(argv[3],&end,0));
        if(!game||!end||*end)return 48;
        const auto reader=[&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);};
        NativeCharacterState s{};std::uintptr_t owner=0,stack=0,sessionGame=0,active=0,again=0;
        unsigned count=0,logic=0,pending=0,phase=0;float position[3]{};
        if(!readNativeCharacter(reader,base,s,game)||!reader(game+0x1f6ed0,&owner,8)||owner!=game||
           !reader(game+0x1f6ec8+0x528,&stack,8)||!stack||!reader(game+0x1f6ec8+0x530,&count,4)||
           !(count&0xffffff)||(count&0xffffff)>32||!reader(stack+((count&0xffffff)-1)*4,&logic,4)||logic>16||
           !reader(game+0x1f6ec8+0x538,&pending,4)||pending>16||!reader(game+0x341058,&phase,4)||
           !reader(s.player+0x300,position,sizeof(position))||!reader(game+0x3130b0,&again,8)||again!=s.player)return 48;
        for(float p:position)if(!std::isfinite(p))return 48;
        reader(base+0x25af110+0x19a8,&sessionGame,8);reader(base+0x39a5290,&active,8);
        std::cout.precision(9);
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"game\":\"0x"<<std::hex<<game<<"\",\"sessionGame\":\"0x"<<sessionGame<<"\",\"activeGame\":\"0x"<<active
            <<"\",\"player\":\"0x"<<s.player<<std::dec<<"\",\"logic\":"<<logic<<",\"pending\":"<<pending<<",\"phase\":"<<phase
            <<",\"character\":"<<s.character<<",\"queued\":"<<s.queued<<",\"definition\":"<<jsonString(s.definitionName)
            <<",\"position\":["<<position[0]<<','<<position[1]<<','<<position[2]<<"]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--gameover")==0) {
        NativeGameOverState s{};
        if(!readNativeGameOver([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s))return 49;
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"game\":\"0x"<<std::hex<<s.game<<"\",\"state\":\"0x"<<s.state<<"\",\"flash\":\"0x"<<s.flash
            <<"\",\"showFunction\":[\"0x"<<s.showFunction[0]<<"\",\"0x"<<s.showFunction[1]<<"\"],\"logic\":"<<std::dec<<s.logic<<",\"pending\":"<<s.pending<<"}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--character")==0) {
        NativeCharacterState s{};
        if(!readNativeCharacter([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s))return 47;
        std::cout << "{\"character\":" << s.character << ",\"queued\":" << s.queued << ",\"game\":\"0x" << std::hex << s.game
            << "\",\"settings\":\"0x" << s.settings << "\",\"player\":\"0x" << s.player << "\",\"definition\":" << jsonString(s.definitionName)
            << ",\"corvoDefinition\":" << jsonString(s.variantNames[1]) << ",\"emilyDefinition\":" << jsonString(s.variantNames[2]) << "}\n";
        return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--player-states")==0) {
        NativePlayerStates s{};
        if(!readNativePlayerStates([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s))return 52;
        std::uint32_t motion=0;const bool motionValid=nativePlayerMotion(s,base,motion);
        std::cout.precision(9);
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"game\":\"0x"<<std::hex<<s.player.game<<"\",\"player\":\"0x"<<s.player.player
            <<"\",\"machine\":\"0x"<<s.component<<"\",\"mode\":\"0x"<<s.mode<<std::dec<<"\",\"stance\":"<<s.player.current
            <<",\"defaultMode\":"<<(s.defaultMode?"true":"false")<<",\"motionValid\":"<<(motionValid?"true":"false")<<",\"motion\":"<<motion
            <<",\"position\":["<<s.player.position[0]<<','<<s.player.position[1]<<','<<s.player.position[2]<<"],\"active\":[";
        bool first=true;for(const auto& state:s.active){
            std::cout<<(first?"":",")<<"{\"id\":"<<state.id<<",\"vtableRva\":\"0x"<<std::hex<<state.vtable-base<<std::dec
                <<"\",\"type\":"<<jsonString(objectType(process.value,base,state.pointer))<<'}';first=false;
        }
        std::cout<<"]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--player-stance")==0) {
        NativePlayerStance s{};
        if(!readNativePlayerStance([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s))return 50;
        std::cout.precision(9);
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"game\":\"0x"<<std::hex<<s.game<<"\",\"player\":\"0x"<<s.player
            <<"\",\"physics\":\"0x"<<s.physics<<std::dec<<"\",\"logic\":"<<s.logic<<",\"current\":"<<s.current
            <<",\"previous\":"<<s.previous<<",\"position\":["<<s.position[0]<<','<<s.position[1]<<','<<s.position[2]<<"]}\n";
        return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--bones")==0)return inspectBones(process.value,base,static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)));
    if(argc==4 && std::strcmp(argv[2],"--ai-targets")==0){
        NativeAiTargets s{};
        if(!readNativeAiTargets([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)),s))return 56;
        const auto emit=[](const NativeAiTargetRecord& r){std::cout<<"{\"handle\":"<<r.handle<<",\"resolved\":"<<(r.resolved?"true":"false")<<",\"flags\":"<<r.flags<<",\"perceived\":"<<r.perceived<<",\"cached\":"<<r.cached<<'}';};
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"handle\":"<<s.handle<<",\"logic\":"<<s.logic<<",\"component\":\"0x"<<std::hex<<s.component<<"\",\"actor\":\"0x"<<s.actor<<std::dec<<"\",\"combat\":";emit(s.combat);std::cout<<",\"attention\":[";
        bool first=true;for(const auto& r:s.attention){if(!first)std::cout<<',';first=false;emit(r);}std::cout<<"]}\n";return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--faction")==0){
        NativeFaction s{};
        if(!readNativeFaction([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)),s))return 55;
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"handle\":"<<s.handle<<",\"logic\":"<<s.logic<<",\"component\":\"0x"<<std::hex<<s.component<<"\",\"actor\":\"0x"<<s.actor<<"\",\"game\":\"0x"<<s.game<<std::dec
            <<"\",\"name\":"<<jsonString(s.name)<<",\"definition\":"<<jsonString(s.definition)<<",\"index\":"<<s.index<<",\"factionCount\":"<<(s.factionCount&0xffffff)<<",\"registered\":"<<s.nodeCount<<",\"relations\":[";
        bool first=true;for(const auto& r:s.relations){if(!first)std::cout<<',';first=false;
            std::cout<<"{\"handle\":"<<r.handle<<",\"index\":"<<r.index<<",\"base\":"<<r.base<<",\"effective\":"<<r.effective<<",\"overridden\":"<<(r.overridden?"true":"false")<<",\"overrideValue\":"<<r.overrideValue<<'}';}
        std::cout<<"]}\n";return 0;
    }
    if(argc==4 && (!std::strcmp(argv[2],"--sword-attachment")||!std::strcmp(argv[2],"--avatar-sword-attachment"))){
        auto swordScope=!std::strcmp(argv[2],"--avatar-sword-attachment")?NativeSwordScope::CorvoTutorial:NativeSwordScope::Guard;
        const auto reader=[&](auto a,auto d,auto n){return read(process.value,a,d,n);};
        const auto handle=static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0));
        NativeSwordEquipment s{},again{};NativeSkeleton skeleton{},skeletonAgain{};
        std::uintptr_t animation=0,render=0,vt=0,owner=0,renderAgain=0;
        float local[7]{},localAgain[7]{},weaponRender[12]{},parentRender[12]{},renderAgainPose[12]{},parentRenderAgain[12]{};unsigned char flags=0;
        if(swordScope==NativeSwordScope::CorvoTutorial&&!readNativeSwordEquipment(reader,base,handle,s,swordScope))swordScope=NativeSwordScope::EmilyPartner;
        if(swordScope==NativeSwordScope::EmilyPartner&&!readNativeSwordEquipment(reader,base,handle,s,swordScope))swordScope=NativeSwordScope::DoppelgangerEmily;
        if(swordScope==NativeSwordScope::DoppelgangerEmily&&!readNativeSwordEquipment(reader,base,handle,s,swordScope))swordScope=NativeSwordScope::DoppelgangerCorvo;
        if(!readNativeSwordEquipment(reader,base,handle,s,swordScope)||!readNativeSkeleton(reader,base,handle,skeleton)||skeleton.actor!=s.actor||
           skeleton.phases.size()!=4||s.joint>=skeleton.jointIds.size()||skeleton.phases[3].mapping.empty()||skeleton.phases[3].mapping[s.joint]<0||
           !reader(s.frame+0x400,&animation,8)||animation!=skeleton.animation||!reader(animation+0x10,&owner,8)||owner!=s.actor||
           !reader(s.frame+0x3d4,local,28)||!reader(s.frame+0x408,&flags,1)||!reader(s.entity+0xf0,&render,8)||!render||
           !reader(render,&vt,8)||vt!=base+0x1b0dd88||!reader(render+0x1860,weaponRender,48)||!reader(skeleton.render+0x1860,parentRender,48)||
           !reader(render+0x1860,renderAgainPose,48)||!reader(s.entity+0xf0,&renderAgain,8)||renderAgain!=render||
           !reader(s.frame+0x3d4,localAgain,28)||std::memcmp(local,localAgain,28)||!readNativeSwordEquipment(reader,base,handle,again,swordScope)||!sameSwordEquipmentIdentity(s,again)||
           !readNativeSkeleton(reader,base,handle,skeletonAgain)||skeletonAgain.game!=skeleton.game||skeletonAgain.actor!=skeleton.actor||
           skeletonAgain.animation!=skeleton.animation||skeletonAgain.render!=skeleton.render||skeletonAgain.skeleton!=skeleton.skeleton||skeletonAgain.jointIds!=skeleton.jointIds||
           skeletonAgain.phases.size()!=4||skeletonAgain.phases[3].mapping!=skeleton.phases[3].mapping||
           !reader(skeleton.render+0x1860,parentRenderAgain,48))return 58;
        for(float f:local)if(!std::isfinite(f))return 58;
        for(float f:weaponRender)if(!std::isfinite(f))return 58;
        for(float f:parentRender)if(!std::isfinite(f))return 58;
        // Equipment identity deliberately excludes movement. The attachment
        // snapshot must separately bracket native roots, render roots and the
        // selected joint so an in-flight frame is never labelled coherent.
        const bool stable=skeleton.stable&&skeletonAgain.stable&&!std::memcmp(weaponRender,renderAgainPose,48)&&
            s.root==again.root&&!std::memcmp(parentRender,parentRenderAgain,48)&&!std::memcmp(skeleton.root,skeletonAgain.root,48)&&
            skeleton.phases[3].matrices[s.joint]==skeletonAgain.phases[3].matrices[s.joint];
        std::cout.precision(9);
        auto floats=[](const float* a,unsigned count){std::cout<<'[';for(unsigned i=0;i<count;++i)std::cout<<(i?",":"")<<a[i];std::cout<<']';};
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"weaponHandle\":"<<s.weaponHandle<<",\"joint\":"<<s.joint<<",\"tag\":"<<jsonString(s.attachedTag)
            <<",\"stable\":"<<(stable?"true":"false")<<",\"frameFlags\":"<<static_cast<unsigned>(flags)<<",\"phase\":3,\"offset\":";
        floats(local,3);std::cout<<",\"quaternion\":";floats(local+3,4);std::cout<<",\"jointMatrix\":";floats(skeleton.phases[3].matrices[s.joint].data(),16);
        std::cout<<",\"weaponNativeRoot\":";floats(s.root.data(),12);std::cout<<",\"weaponRenderRoot\":";floats(weaponRender,12);
        std::cout<<",\"parentNativeRoot\":";floats(skeleton.root,12);std::cout<<",\"parentRenderRoot\":";floats(parentRender,12);std::cout<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-sword")){
        NativePlayerSword n{};
        if(!readNativePlayerSword([&](auto p,auto b,auto size){return read(process.value,p,b,size);},base,n))return 59;
        const auto& s=n.sword;
        float root[12]{},attachment[7]{};
        if(!read(process.value,s.entity+0x300,root,sizeof(root))||!read(process.value,s.frame+0x3d4,attachment,sizeof(attachment)))return 59;
        std::cout.precision(9);
        std::cout<<"{\"root\":[";
        for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<root[i];
        std::cout<<"],\"attachment\":[";
        for(unsigned i=0;i<7;++i)std::cout<<(i?",":"")<<attachment[i];
        std::cout<<"],\"frame\":\"0x"<<std::hex<<s.frame<<"\","<<std::dec;
        std::cout<<"\"tick\":"<<GetTickCount64()<<",\"character\":"<<n.character<<",\"logic\":"<<s.logic<<",\"quantity\":"<<s.quantity
            <<",\"weaponHandle\":"<<s.weaponHandle<<",\"equipped\":"<<(s.equipped?"true":"false")<<",\"started\":"<<(s.started?"true":"false")
            <<",\"joint\":"<<s.joint<<",\"tag\":"<<jsonString(s.attachedTag)<<",\"armHandle\":"<<n.arm.handle
            <<",\"actor\":\"0x"<<std::hex<<s.actor<<"\",\"inventory\":\"0x"<<s.inventory<<"\",\"entity\":\"0x"<<s.entity
            <<"\",\"parent\":\"0x"<<s.parent<<"\",\"component\":\"0x"<<s.component<<"\"}\n";return 0;
    }
    if(argc==4 && (!std::strcmp(argv[2],"--sword-equipment")||!std::strcmp(argv[2],"--avatar-sword"))){
        auto swordScope=!std::strcmp(argv[2],"--avatar-sword")?NativeSwordScope::CorvoTutorial:NativeSwordScope::Guard;
        NativeSwordEquipment s{};const auto handle=static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0));
        if(swordScope==NativeSwordScope::CorvoTutorial&&!readNativeSwordEquipment([&](auto a,auto d,auto n){return read(process.value,a,d,n);},base,handle,s,swordScope))swordScope=NativeSwordScope::EmilyPartner;
        if(swordScope==NativeSwordScope::EmilyPartner&&!readNativeSwordEquipment([&](auto a,auto d,auto n){return read(process.value,a,d,n);},base,handle,s,swordScope))swordScope=NativeSwordScope::DoppelgangerEmily;
        if(swordScope==NativeSwordScope::DoppelgangerEmily&&!readNativeSwordEquipment([&](auto a,auto d,auto n){return read(process.value,a,d,n);},base,handle,s,swordScope))swordScope=NativeSwordScope::DoppelgangerCorvo;
        if(!readNativeSwordEquipment([&](auto a,auto d,auto n){return read(process.value,a,d,n);},base,handle,s,swordScope))return 57;
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"handle\":"<<s.handle<<",\"logic\":"<<s.logic
            <<",\"inventory\":\"0x"<<std::hex<<s.inventory<<"\",\"entry\":\"0x"<<s.entry<<"\",\"component\":\"0x"<<s.component
            <<"\",\"entity\":\"0x"<<s.entity<<"\",\"frame\":\"0x"<<s.frame<<"\",\"parent\":\"0x"<<s.parent<<"\""<<std::dec
            <<",\"weaponHandle\":"<<s.weaponHandle<<",\"slot\":"<<s.slot<<",\"quantity\":"<<s.quantity<<",\"item\":"<<jsonString(s.itemName)
            <<",\"equipment\":"<<jsonString(s.equipmentName)<<",\"equipped\":"<<(s.equipped?"true":"false")
            <<",\"started\":"<<(s.started?"true":"false")<<",\"flags\":"<<s.componentFlags<<",\"joint\":"<<s.joint
            <<",\"attachedTag\":"<<jsonString(s.attachedTag)<<",\"root\":[";
        for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<s.root[i];
        std::cout<<"],\"cached\":[";for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<s.cached[i];
        std::cout<<"],\"slots\":[";for(unsigned i=0;i<16;++i)std::cout<<(i?",":"")<<"\"0x"<<std::hex<<s.slots[i]<<"\"";
        std::cout<<"]}\n";return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--health")==0){
        NativeHealth s{};
        if(!readNativeHealth([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,
            static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)),s))return 54;
        std::cout.precision(9);
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"handle\":"<<s.handle<<",\"logic\":"<<s.logic<<",\"component\":\"0x"<<std::hex<<s.component
            <<"\",\"actor\":\"0x"<<s.actor<<"\",\"attributes\":\"0x"<<s.attributes<<std::dec<<"\",\"current\":"<<s.current<<",\"maximum\":"<<s.maximum
            <<",\"baseMaximum\":"<<s.baseMaximum<<",\"suppressedMaximum\":"<<s.suppressedMaximum<<",\"ownerSuppressed\":"<<((s.ownerFlags&0x20)?"true":"false")
            <<",\"maximumIndex\":"<<s.maximumIndex<<",\"suppressedIndex\":"<<s.suppressedIndex<<",\"attributeCount\":"<<(s.attributeCount&0xffffff)
            <<",\"attributeValues\":["<<s.attributeValues[0]<<','<<s.attributeValues[1]<<','<<s.attributeValues[2]<<','<<s.attributeValues[3]<<"]}\n";return 0;
    }
    if(argc==4 && (std::strcmp(argv[2],"--skeleton")==0||std::strcmp(argv[2],"--arm-skeleton")==0)) {
        NativeSkeleton s{};
        if(!readNativeSkeleton([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,
           static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)),s,std::strcmp(argv[2],"--arm-skeleton")==0?NativeSkeletonScope::PlayerArm:NativeSkeletonScope::Actor))return 53;
        std::vector<std::string> names;const bool namesValid=jointNames(process.value,base,s.jointIds,names);
        std::cout.precision(9);
        auto floats=[](const float* values,unsigned n){std::cout<<'[';for(unsigned i=0;i<n;++i)std::cout<<(i?",":"")<<values[i];std::cout<<']';};
        std::cout<<"{\"handle\":"<<s.handle<<",\"armCharacter\":"<<s.playerArm.character<<",\"armSide\":"<<s.playerArm.side<<",\"stable\":"<<(s.stable?"true":"false")<<",\"model\":"<<jsonString(s.modelName)
            <<",\"skeleton\":"<<jsonString(s.skeletonName)<<",\"root\":";floats(s.root,12);
        std::cout<<",\"nativeNamesValid\":"<<(namesValid?"true":"false")<<",\"joints\":[";
        for(unsigned j=0;j<s.jointIds.size();++j){
            std::cout<<(j?",":"")<<"{\"index\":"<<j<<",\"nativeNameId\":"<<s.jointIds[j]<<",\"nativeName\":"<<jsonString(namesValid?names[j]:"")<<",\"parent\":"<<s.parents[j]<<",\"bindLocalQTS\":";
            floats(s.bindLocal[j].data(),12);std::cout<<",\"localPose\":";floats(s.localPose[j].data(),16);
            std::cout<<",\"globalPose\":";floats(s.globalPose[j].data(),16);std::cout<<'}';
        }
        std::cout<<"],\"phases\":[";
        for(unsigned p=0;p<s.phases.size();++p) {
            std::cout<<(p?",":"")<<"{\"phase\":"<<p<<",\"matrices\":[";bool first=true;
            for(unsigned j=0;j<s.jointIds.size()&&!s.phases[p].mapping.empty();++j) {
                if(s.phases[p].mapping[j]<0)continue;
                std::cout<<(first?"":",")<<"{\"joint\":"<<j<<",\"cacheIndex\":"<<s.phases[p].mapping[j]<<",\"matrix\":";first=false;
                floats(s.phases[p].matrices[j].data(),16);std::cout<<'}';
            }
            std::cout<<"]}";
        }
        std::cout<<"]}\n";return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--animation")==0)return inspectAnimation(process.value,base,static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)));
    if(argc==4 && std::strcmp(argv[2],"--npc-pose")==0) {
        const auto handle=static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0));
        const auto reader=[&](std::uintptr_t at,void* data,std::size_t size){return read(process.value,at,data,size);};
        std::uintptr_t game=0,actor=0,vt=0,render=0;float pose[12]{},renderPose[12]{},again[12]{};
        if(!reader(base+0x39a5290,&game,8)||!game||!reader(game,&vt,8)||vt!=base+0x1c940a8||
           !(actor=worldIdentityEntity(reader,game,handle))||!reader(actor,&vt,8)||vt!=base+0x1c5e108||
           !reader(actor+0xf0,&render,8)||!render||!reader(actor+0x300,pose,sizeof(pose))||
           !reader(render+0x1860,renderPose,sizeof(renderPose))||!reader(actor+0x300,again,sizeof(again)))return 47;
        for(float f:pose)if(!std::isfinite(f))return 47;
        for(float f:renderPose)if(!std::isfinite(f))return 47;
        const bool stable=!std::memcmp(pose,again,sizeof(pose))&&worldIdentityEntity(reader,game,handle)==actor;
        std::cout.precision(9);
        std::cout<<"{\"handle\":"<<handle<<",\"stable\":"<<(stable?"true":"false")<<",\"root\":[";
        for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<pose[i];std::cout<<"],\"render\":[";
        for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<renderPose[i];std::cout<<"]}\n";return 0;
    }
    if(argc==4 && (!std::strcmp(argv[2],"--ai-decision")||!std::strcmp(argv[2],"--ai-reflex")||!std::strcmp(argv[2],"--ai-attack")||!std::strcmp(argv[2],"--ai-idle"))) {
        std::uintptr_t game=0;NativeAiComponentState s{};
        const auto kind=!std::strcmp(argv[2],"--ai-idle")?NativeAiKind::IdleBreak:!std::strcmp(argv[2],"--ai-reflex")?NativeAiKind::Reflex:!std::strcmp(argv[2],"--ai-attack")?NativeAiKind::Attack:NativeAiKind::Decision;
        const auto reader=[&](std::uintptr_t at,void* data,std::size_t size){return read(process.value,at,data,size);};
        if(!reader(base+0x39a5290,&game,8)||!readNativeAiComponent(reader,base,game,static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)),kind,s)) {
            std::cerr << "Native AI decision snapshot unavailable or changing\n";return 46;
        }
        std::cout << "{\"pid\":" << pid << ",\"handle\":\"0x" << std::hex << s.handle << "\",\"actor\":\"0x" << s.actor
            << "\",\"component\":\"0x" << s.component << "\",\"manager\":\"0x" << s.manager << "\",\"registry\":\"0x" << s.registry
            << "\",\"tree\":\"0x" << s.tree << "\",\"flags\":\"0x" << s.flags << std::dec
            << "\",\"pauseReferences\":" << ((s.flags>>6)&0x1fffff) << ",\"pauseState\":" << ((s.flags>>4)&3)
            << ",\"started\":" << ((s.flags&0xc)==8?"true":"false") << ",\"schedulerActive\":" << (s.schedulerActive?"true":"false")
            << ",\"idleState\":" << s.idleState << ",\"idleAutoPlay\":" << s.idleAutoPlay << ",\"poolIndex\":" << s.poolIndex << ",\"liveCount\":" << s.liveCount << ",\"activeCount\":" << s.activeCount << ",\"activeIndices\":[";
        for(unsigned i=0;i<s.activeIndices.size();++i)std::cout << (i?",":"") << s.activeIndices[i];
        std::cout << "]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--world-npcs")==0) {
        std::uintptr_t game=0;unsigned unbound=0;std::vector<NativeWorldNpc> catalog;
        const auto reader=[&](std::uintptr_t at,void* data,std::size_t size){return read(process.value,at,data,size);};
        if(!reader(base+0x39a5290,&game,8)||!readWorldNpcCatalog(reader,base,game,catalog,unbound))return 45;
        std::cout << "{\"pid\":" << pid << ",\"unboundNpcs\":" << unbound << ",\"npcs\":[";
        bool first=true;
        for(const auto& n:catalog) {
            if(!first)std::cout << ',';first=false;
            std::cout << "{\"handle\":\"0x" << std::hex << n.handle << "\",\"spawnerHandle\":\"0x" << n.spawnerHandle << std::dec
                << "\",\"spawnerName\":" << jsonString(n.spawnerName) << ",\"definition\":" << jsonString(n.definition)
                << ",\"childCount\":" << n.childCount << ",\"reciprocal\":" << (n.reciprocal?"true":"false")
                << ",\"unique\":" << (n.unique?"true":"false") << ",\"position\":[" << n.position[0] << ',' << n.position[1] << ',' << n.position[2] << "],\"orientation\":[";
            for(unsigned i=0;i<9;++i)std::cout << (i?",":"") << n.orientation[i];std::cout << "]}";
        }
        std::cout << "]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--midnight")==0) {
        std::uintptr_t game=0,vt=0,player=0,components=0,proxy=0;std::uint32_t componentFlags=0,channels[2]{};
        if(!read(process.value,base+0x25af110+0x19a8,&game,8)||!game||!read(process.value,game,&vt,8)||vt!=base+0x1c940a8||
           !read(process.value,game+0x3130b0,&player,8)||!player||!read(process.value,player+0x268,&components,8)||!components||
           !read(process.value,player+0x270,&componentFlags,4)||(componentFlags&0xffffff)>256)return 25;
        for(unsigned i=0;i<(componentFlags&0xffffff);++i) {
            std::uintptr_t component=0,owner=0;
            if(read(process.value,components+i*8,&component,8)&&component&&read(process.value,component,&vt,8)&&vt==base+0x1c59ac8&&
               read(process.value,component+0x10,&owner,8)&&owner==player)proxy=component;
        }
        if(!proxy||!read(process.value,proxy+0x20,&channels[0],4)||!read(process.value,proxy+0x30,&channels[1],4))return 26;
        const auto manager=game+0x1f5338;
        std::cout << "{\"pid\":" << pid << ",\"manager\":\"0x" << std::hex << manager << "\",\"proxy\":\"0x" << proxy << "\",\"channels\":[" << std::dec << channels[0] << ',' << channels[1] << "],\"instances\":[";
        bool first=true;std::unordered_set<std::uintptr_t> seen;
        for(unsigned field:{0u,0x610u}) {
            std::uintptr_t items=0;std::uint32_t flags=0;
            if(!read(process.value,manager+field,&items,8)||!read(process.value,manager+field+8,&flags,4)||(flags&0xffffff)>512||((flags&0xffffff)&&!items))return 27;
            for(unsigned i=0;i<(flags&0xffffff);++i) {
                std::uintptr_t instance=0;std::uint32_t id=0,state=0;
                if(!read(process.value,items+i*8,&instance,8)||!instance||!seen.insert(instance).second)continue;
                if(!read(process.value,instance+0x158,&id,4)||!read(process.value,instance+0x27c,&state,4))return 28;
                if(!first)std::cout << ',';first=false;
                std::cout << "{\"pointer\":\"0x" << std::hex << instance << "\",\"listOffset\":\"0x" << field << std::dec << "\",\"id\":" << id << ",\"state\":" << state
                    << ",\"playerChannel\":" << (id&&id==channels[0]?0:id&&id==channels[1]?1:-1);
                std::uintptr_t decl=0,scene=0,name=0,sceneName=0;std::uint32_t sceneDurationMs=0,clockWords[6]{};
                if(read(process.value,instance,&decl,8)&&read(process.value,instance+8,&scene,8)&&
                   objectType(process.value,base,decl)==".?AVarkDeclMidnightScene@@"&&objectType(process.value,base,scene)==".?AVarkMidnightScene@@"&&
                   read(process.value,decl+8,&name,8)&&read(process.value,scene+0x30,&sceneName,8)&&read(process.value,scene+0x28,&sceneDurationMs,4)) {
                    std::cout << ",\"asset\":" << jsonString(readString(process.value,name)) << ",\"subscene\":" << jsonString(readString(process.value,sceneName)) << ",\"sceneDurationMs\":" << sceneDurationMs;
                }
                read(process.value,instance+0x15c,clockWords,sizeof(clockWords));std::cout << ",\"clockWords\":[";
                for(unsigned c=0;c<6;++c)std::cout << (c?",":"") << clockWords[c];
                std::cout << "]}";
            }
        }
        std::cout << "]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--kiscule")==0) {
        std::uintptr_t game=0,vt=0,owner=0;
        if(!read(process.value,base+0x25af110+0x19a8,&game,8)||!game||!read(process.value,game,&vt,8)||vt!=base+0x1c940a8||
           !read(process.value,game+0x1f6e48,&owner,8)||owner!=game)return 24;
        std::cout << "{\"manager\":\"0x" << std::hex << game+0x1f6e48 << "\",\"entries\":[";
        std::unordered_set<std::uintptr_t> seen;bool first=true;
        for(unsigned field:{0x18u,0x50u,0x68u}) {
            std::uintptr_t table=0;std::uint32_t count=0;
            if(!read(process.value,game+0x1f6e48+field,&table,8)||!table||!read(process.value,game+0x1f6e48+field+8,&count,4)||(count&0xffffff)>1024)continue;
            for(unsigned bucket=0;bucket<(count&0xffffff);++bucket) {
                std::uintptr_t node=0;read(process.value,table+bucket*8,&node,8);
                for(unsigned depth=0;node&&depth<64;++depth) {
                    if(!seen.insert(node).second)break;
                    std::uintptr_t value=0,next=0;std::uint32_t key=0;
                    if(!read(process.value,node,&key,4)||!read(process.value,node+8,&value,8)||!read(process.value,node+16,&next,8))break;
                    if(!first)std::cout << ',';first=false;
                    std::cout << "{\"field\":\"0x" << field << "\",\"key\":\"0x" << key << "\",\"value\":\"0x" << value << "\",\"type\":" << jsonString(objectType(process.value,base,value)) << '}';
                    node=next;
                }
            }
        }std::cout << "]}\n";return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--object")==0) {
        const auto pointer=static_cast<std::uintptr_t>(std::strtoull(argv[3],nullptr,0));
        std::uintptr_t first=0;if(!pointer||!read(process.value,pointer,&first,8))return 23;
        std::cout << "{\"pointer\":\"0x" << std::hex << pointer << "\",\"type\":" << jsonString(objectType(process.value,base,pointer)) << ",\"words\":[";
        for(unsigned offset=0;offset<0x400;offset+=8) {
            std::uintptr_t word=0;if(!read(process.value,pointer+offset,&word,8))break;
            if(offset)std::cout << ',';
            std::cout << "{\"offset\":\"0x" << offset << "\",\"word\":\"0x" << word << "\",\"pointerType\":" << jsonString(objectType(process.value,base,word))
                << ",\"pointerText\":" << jsonString(readString(process.value,word)) << '}';
        }std::cout << "]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--cinematic")==0) {
        std::uintptr_t game=0,vt=0,state=0,candidate=0;
        std::uint32_t phase=0,flags=0;
        if(!read(process.value,base+0x25af110+0x19a8,&game,8)||!game||!read(process.value,game,&vt,8)||vt!=base+0x1c940a8||
           !read(process.value,game+0x1f6ec8+0x18+5*8,&state,8)||!state||!read(process.value,state,&vt,8)||vt!=base+0x1c60128||
           !read(process.value,state+0x9cc,&phase,4)||!read(process.value,state+0x9c0,&flags,4)||
           !read(process.value,game+0x1f6368,&candidate,8))return 22;
        std::cout << "{\"pid\":" << pid << ",\"cinematicState\":\"0x" << std::hex << state << std::dec << "\",\"phase\":" << phase << ",\"flags\":" << flags
            << ",\"gameField1f6368\":\"0x" << std::hex << candidate << "\",\"candidateWords\":[";
        if(candidate)for(unsigned offset=0;offset<0x200;offset+=8) {
            std::uintptr_t word=0,wordVt=0,locator=0;std::uint32_t type=0;
            if(!read(process.value,candidate+offset,&word,8))break;
            if(offset)std::cout << ',';
            if(word&&read(process.value,word,&wordVt,8)&&wordVt>=base&&wordVt<base+nt.OptionalHeader.SizeOfImage&&
               read(process.value,wordVt-8,&locator,8)&&locator>=base&&locator<base+nt.OptionalHeader.SizeOfImage)read(process.value,locator+12,&type,4);
            std::cout << "{\"offset\":\"0x" << offset << "\",\"word\":\"0x" << word << "\",\"pointerType\":" << jsonString(type?readString(process.value,base+type+16):"") << '}';
        }
        std::uintptr_t manager=0,decl=0;read(process.value,game+0x1e8960,&manager,8);read(process.value,state+0x10,&decl,8);
        std::cout << "],\"gameField1e8960\":\"0x" << manager << "\",\"gameField1e8960Type\":" << jsonString(objectType(process.value,base,manager))
            << ",\"stateField10\":\"0x" << decl << "\",\"stateField10Type\":" << jsonString(objectType(process.value,base,decl)) << ",\"cinematicNodes\":[";
        std::uintptr_t player=0,observers=0,items=0;std::uint32_t count=0;bool first=true;
        if(read(process.value,game+0x3130b0,&player,8)&&player&&read(process.value,player+8,&observers,8)&&observers&&
           read(process.value,observers,&items,8)&&items&&read(process.value,observers+8,&count,4)&&(count&0xffffff)<=256) {
            for(unsigned i=0;i<(count&0xffffff);++i) {
                std::uintptr_t node=0,nodeVt=0,field20=0,field28=0;unsigned char field30=0;
                if(!read(process.value,items+i*8,&node,8)||!node||!read(process.value,node,&nodeVt,8)||nodeVt!=base+0x1c63e48)continue;
                read(process.value,node+0x20,&field20,8);read(process.value,node+0x28,&field28,8);read(process.value,node+0x30,&field30,1);
                if(!first)std::cout << ',';first=false;
                std::cout << "{\"pointer\":\"0x" << node << "\",\"field20\":\"0x" << field20 << "\",\"field20Type\":" << jsonString(objectType(process.value,base,field20))
                    << ",\"field28\":\"0x" << field28 << "\",\"field28Type\":" << jsonString(objectType(process.value,base,field28)) << ",\"field30\":" << std::dec << static_cast<unsigned>(field30);
                std::uintptr_t instanceVt=0,graphNodeVt=0,executable=0,actor=0,name=0;std::uint32_t ownerHandle=0,actualHandle=0,nodeId=0;
                if(read(process.value,field20,&instanceVt,8)&&instanceVt==base+0x1c69ad8&&
                   read(process.value,field28,&graphNodeVt,8)&&graphNodeVt==base+0x1c69828&&
                   read(process.value,field28+0x20,&executable,8)&&executable==node&&read(process.value,field28+8,&nodeId,4)&&
                   read(process.value,field20+0x60,&ownerHandle,4)) {
                    const auto slot=static_cast<std::int32_t>(ownerHandle)>>16;
                    if(slot>=0&&slot<16384&&read(process.value,game+0x3130b0+slot*8,&actor,8)&&actor&&
                       read(process.value,actor+0xa8,&actualHandle,4)&&actualHandle==ownerHandle&&read(process.value,actor+0x20,&name,8)) {
                        std::cout << ",\"nativeNodeId\":" << nodeId << ",\"ownerHandle\":\"0x" << std::hex << ownerHandle << "\",\"ownerName\":" << jsonString(readString(process.value,name));
                    }
                }
                std::cout << std::hex << '}';
            }
        }
        std::cout << "]}" << std::dec << '\n';return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--logic")==0) {
        std::uintptr_t game=0,gameVtable=0,owner=0,stack=0;std::uint32_t flags=0,pending=0;
        // The active gameplay global is null during the native intro. The
        // common session still owns the loaded GameLocal and its state stack.
        if(!read(process.value,base+0x25af110+0x19a8,&game,8)||!game||!read(process.value,game,&gameVtable,8)||gameVtable!=base+0x1c940a8)return 19;
        const auto manager=game+0x1f6ec8;
        if(!read(process.value,manager+8,&owner,8)||owner!=game||!read(process.value,manager+0x528,&stack,8)||
           !read(process.value,manager+0x530,&flags,4)||!read(process.value,manager+0x538,&pending,4)||(flags&0xffffff)>32||pending>=17)return 20;
        std::cout << "{\"pid\":" << pid << ",\"manager\":\"0x" << std::hex << manager << "\",\"owner\":\"0x" << owner << std::dec << "\",\"pending\":" << pending << ",\"stack\":[";
        for(unsigned i=0;i<(flags&0xffffff);++i) {
            std::uint32_t id=0,type=0;std::uintptr_t state=0,vt=0,locator=0;
            if(!stack||!read(process.value,stack+i*4,&id,4)||id>=17||!read(process.value,manager+0x18+id*8,&state,8)||!state||
               !read(process.value,state,&vt,8)||vt<base||vt>=base+nt.OptionalHeader.SizeOfImage)return 21;
            if(read(process.value,vt-8,&locator,8)&&locator>=base&&locator<base+nt.OptionalHeader.SizeOfImage)read(process.value,locator+12,&type,4);
            if(i)std::cout << ',';
            std::cout << "{\"id\":" << id << ",\"pointer\":\"0x" << std::hex << state << "\",\"vtableRva\":\"0x" << vt-base << std::dec << "\",\"type\":" << jsonString(type?readString(process.value,base+type+16):"") << '}';
        }
        std::cout << "]}\n";return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--components")==0) {
        const auto handle=static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0));
        const auto slot=static_cast<std::int32_t>(handle)>>16;
        std::uintptr_t game=0,actor=0,list=0;std::uint32_t actual=0,flags=0;
        if(slot<0||slot>=16384||!read(process.value,base+0x39a5290,&game,8)||!game||
           !read(process.value,game+0x3130b0+slot*8,&actor,8)||!actor||!read(process.value,actor+0xa8,&actual,4)||actual!=handle||
           !read(process.value,actor+0x268,&list,8)||!read(process.value,actor+0x270,&flags,4)||(flags&0xffffff)>256)return 17;
        std::cout << "{\"handle\":\"0x" << std::hex << handle << "\",\"components\":[";
        for(std::uint32_t i=0;i<(flags&0xffffff);++i) {
            std::uintptr_t cpnt=0,vtable=0,locator=0,owner=0;std::uint32_t type=0,componentState=0;
            if(i)std::cout << ',';
            read(process.value,list+i*8,&cpnt,8);read(process.value,cpnt,&vtable,8);
            read(process.value,cpnt+0x10,&owner,8);read(process.value,cpnt+0x1c,&componentState,4);
            if(vtable>base&&vtable<base+nt.OptionalHeader.SizeOfImage&&read(process.value,vtable-8,&locator,8)&&locator>base&&locator<base+nt.OptionalHeader.SizeOfImage)read(process.value,locator+12,&type,4);
            std::cout << "{\"pointer\":\"0x" << cpnt << "\",\"vtableRva\":\"0x" << vtable-base << "\",\"owner\":\"0x" << owner << "\",\"flags\":\"0x" << componentState << "\",\"type\":" << jsonString(type?readString(process.value,base+type+16):"") << '}';
        }
        std::cout << "]}\n";return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--entities")==0) {
        std::uintptr_t game=0,handles=0;std::uint32_t flags=0;
        if(!read(process.value,base+0x39a5290,&game,8)||!game||!read(process.value,game+0x601c0,&handles,8)||!read(process.value,game+0x601c8,&flags,4))return 15;
        const auto count=flags&0xffffff;if(count>16384)return 16;
        std::cout << "{\"pid\":" << pid << ",\"handleCount\":" << count << ",\"entities\":[";
        bool first=true;
        for(std::uint32_t i=0;i<count;++i){
            std::uint32_t handle=0,actual=0;std::uintptr_t actor=0,vtable=0,name=0,locator=0;std::uint32_t type=0;
            if(!read(process.value,handles+i*4,&handle,4))continue;
            const auto slot=static_cast<std::int32_t>(handle)>>16;
            if(slot<0||slot>=16384||!read(process.value,game+0x3130b0+slot*8,&actor,8)||!actor||
               !read(process.value,actor+0xa8,&actual,4)||actual!=handle)continue;
            read(process.value,actor,&vtable,8);read(process.value,actor+0x20,&name,8);
            if(vtable>base&&vtable<base+nt.OptionalHeader.SizeOfImage&&read(process.value,vtable-8,&locator,8)&&locator>base&&locator<base+nt.OptionalHeader.SizeOfImage)read(process.value,locator+12,&type,4);
            if(!first)std::cout << ',';first=false;
            std::uintptr_t definition=0,definitionName=0,render=0;float position[3]{},renderPosition[3]{};
            read(process.value,actor+0x2e8,&definition,8);
            if(definition)read(process.value,definition+8,&definitionName,8);
            read(process.value,actor+0x300,position,sizeof(position));
            read(process.value,actor+0xf0,&render,8);
            const bool renderRead=render&&read(process.value,render+0x1860,renderPosition,sizeof(renderPosition));
            std::cout << "{\"handle\":\"0x" << std::hex << handle << "\",\"pointer\":\"0x" << actor << "\",\"vtableRva\":\"0x" << vtable-base << "\",\"name\":" << jsonString(readString(process.value,name)) << ",\"type\":" << jsonString(type?readString(process.value,base+type+16):"")
                << ",\"definition\":\"0x" << definition << "\",\"definitionName\":" << jsonString(readString(process.value,definitionName)) << std::dec;
            if(std::isfinite(position[0])&&std::isfinite(position[1])&&std::isfinite(position[2]))std::cout << ",\"position\":[" << position[0] << ',' << position[1] << ',' << position[2] << ']';
            if(renderRead&&std::isfinite(renderPosition[0])&&std::isfinite(renderPosition[1])&&std::isfinite(renderPosition[2]))std::cout << ",\"renderPosition\":[" << renderPosition[0] << ',' << renderPosition[1] << ',' << renderPosition[2] << ']';
            if(readString(process.value,name).find("d2coop_probe_")==0) {
                float orientation[9]{},renderOrientation[9]{};
                if(read(process.value,actor+0x30c,orientation,sizeof(orientation))) {
                    std::cout << ",\"orientation\":[";
                    for(int j=0;j<9;++j){if(j)std::cout << ',';std::cout << orientation[j];}std::cout << ']';
                }
                if(render&&read(process.value,render+0x186c,renderOrientation,sizeof(renderOrientation))) {
                    std::cout << ",\"renderOrientation\":[";
                    for(int j=0;j<9;++j){if(j)std::cout << ',';std::cout << renderOrientation[j];}std::cout << ']';
                }
                std::uintptr_t list=0;std::uint32_t componentFlags=0;
                if(read(process.value,actor+0x268,&list,8)&&read(process.value,actor+0x270,&componentFlags,4)&&(componentFlags&0xffffff)<=256) {
                    for(std::uint32_t j=0;j<(componentFlags&0xffffff);++j) {
                        std::uintptr_t component=0,componentVtable=0;
                        if(read(process.value,list+j*8,&component,8)&&read(process.value,component,&componentVtable,8)&&componentVtable==base+0x1c54008) {
                            std::uintptr_t instance=0,world=0,ids=0,bodies=0;std::uint32_t bodyCount=0;
                            if(read(process.value,component+0x100,&instance,8)&&instance&&read(process.value,instance+0x20,&world,8)&&world&&
                               read(process.value,instance+0x28,&ids,8)&&read(process.value,instance+0x30,&bodyCount,4)&&bodyCount<=256&&
                               read(process.value,world+0x28,&bodies,8)) {
                                std::cout << ",\"physicsBodies\":[";bool firstBody=true;
                                for(std::uint32_t k=0;k<bodyCount;++k) {
                                    std::uint32_t bodyId=0;float bodyPosition[3]{};
                                    if(!read(process.value,ids+k*4,&bodyId,4)||(bodyId&0xffffff)==0xffffff||
                                       !read(process.value,bodies+(bodyId&0xffffff)*0xb0+0x30,bodyPosition,sizeof(bodyPosition)))continue;
                                    if(!firstBody)std::cout << ',';firstBody=false;
                                    std::cout << "{\"id\":" << bodyId << ",\"position\":[" << bodyPosition[0] << ',' << bodyPosition[1] << ',' << bodyPosition[2] << "]}";
                                }std::cout << ']';
                            }
                        }
                    }
                }
            }
            std::cout << '}';
        }
        std::cout << "]}" << std::dec << '\n';return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--world")==0) {
        std::uintptr_t commonVtable=0,sessionGetter=0,session=0,sessionVtable=0,mapGetter=0,mapName=0;std::uint32_t worldState=0;
        if(!read(process.value,base+0x25af110,&commonVtable,8)||!read(process.value,commonVtable+0x190,&sessionGetter,8))return 18;
        read(process.value,base+0x25af110+0x19a8,&session,8);
        if(session){read(process.value,session,&sessionVtable,8);read(process.value,sessionVtable+0x88,&mapGetter,8);read(process.value,session+0x337128+0x10,&mapName,8);read(process.value,session+0x341058,&worldState,4);}
        std::cout << "{\"commonVtableRva\":\"0x" << std::hex << commonVtable-base << "\",\"sessionGetterRva\":\"0x" << sessionGetter-base
            << "\",\"session\":\"0x" << session << "\",\"sessionVtableRva\":\"0x" << sessionVtable-base << "\",\"mapGetterRva\":\"0x" << mapGetter-base
            << "\",\"mapName\":" << jsonString(readString(process.value,mapName)) << ",\"state\":" << std::dec << worldState << "}\n";
        return 0;
    }
    if(argc==3 && std::strcmp(argv[2],"--input")==0) {
        std::uintptr_t system=0,vtable=0,hook=0;
        if(!read(process.value,base+0x2286cf8,&system,8) || !system || !read(process.value,system,&vtable,8)) return 13;
        std::cout << "{\"pid\":" << pid << ",\"system\":\"0x" << std::hex << system << "\",\"vtableRva\":\"0x" << vtable-base << "\",\"methods\":[";
        for(int i=0;i<26;++i) {std::uintptr_t method=0;read(process.value,vtable+i*8,&method,8);if(i)std::cout << ',';std::cout << "\"0x" << method-base << "\"";}
        read(process.value,base+0x1a7ecf0,&hook,8);
        std::cout << "],\"xinputImport\":\"0x" << hook << "\",\"controllerFields\":[";
        for(int offset:{0x5f8,0x5fc,0x600,0x610,0x614,0x740}) {
            std::uint32_t value=0;read(process.value,base+0x3134140+offset,&value,4);
            if(offset!=0x5f8) std::cout << ',';
            std::cout << "{\"offset\":\"0x" << offset << "\",\"value\":\"0x" << value << "\"}";
        }
        std::cout << "],\"inputModules\":[";
        bool first=true;
        Module32FirstW(snapshot.value,&module);
        do {
            if(!_wcsicmp(module.szModule,L"dinput8.dll") || !_wcsicmp(module.szModule,L"XINPUT9_1_0.dll")) {
                if(!first)std::cout << ',';first=false;
                const char* name = !_wcsicmp(module.szModule,L"dinput8.dll") ? "dinput8.dll" : "XINPUT9_1_0.dll";
                std::cout << "{\"name\":\"" << name << "\",\"base\":\"0x" << reinterpret_cast<std::uintptr_t>(module.modBaseAddr) << "\"}";
            }
        }while(Module32NextW(snapshot.value,&module));
        unsigned char acquired=0,active=0;
        if(!read(process.value,base+0x3134140+8,&acquired,1) ||
           !read(process.value,base+0x3137f51,&active,1)) return 14;
        std::cout << "],\"controllerAcquired\":" << std::dec << static_cast<int>(acquired)
            << ",\"active\":" << static_cast<int>(active) << "}\n";
        return 0;
    }
    if (argc == 3 && std::strcmp(argv[2], "--player") == 0) {
        // Reproduce the read-only portion of native getviewpos's fallback path.
        // These fields describe a candidate local player view; no actor writes.
        std::uintptr_t game=0, player=0, table=0,gameVtable=0,spawnMethod=0,body=0,bodyDefinition=0;
        std::uint32_t viewHandle=0;
        int localIndex=-2, alternate=0;
        unsigned char active=0;
        if (!read(process.value,base+0x39a5290,&game,8) || !game ||
            !read(process.value,base+0x39a6088,&localIndex,4) ||
            !read(process.value,base+0x25ae9b8,&alternate,4)) {
            std::cerr << "Game world is not initialized yet (menu/loading screen)\n"; return 11;
        }
        if (localIndex==0 || (localIndex==-1 && alternate==0)) read(process.value,game+0x3130b0,&player,8);
        read(process.value,game,&gameVtable,8);read(process.value,gameVtable+0x1c0,&spawnMethod,8);
        if(player){read(process.value,player+0x3e0,&body,8);read(process.value,player+0x3e8,&bodyDefinition,8);}
        read(process.value,game+0x1f52b8,&table,8);
        read(process.value,game+0x1f52c0,&viewHandle,4);
        read(process.value,base+0x3137f51,&active,1);
        std::cout << "{\"pid\":" << pid << ",\"active\":" << static_cast<int>(active)
            << ",\"localIndex\":" << localIndex << ",\"alternate\":" << alternate
            << ",\"player\":\"0x" << std::hex << player << "\",\"gameVtableRva\":\"0x" << gameVtable-base << "\",\"spawnMethodRva\":\"0x" << spawnMethod-base
            << "\",\"auxiliaryEntity\":\"0x" << body << "\",\"auxiliaryDefinition\":\"0x" << bodyDefinition << "\",\"viewListFlags\":\"0x" << viewHandle << std::dec << "\"";
        float transform[12]{};
        if (player && read(process.value,player+0x300,transform,sizeof(transform))) {
            for (float f : transform) if (!std::isfinite(f)) return 12;
            std::cout << ",\"candidatePosition\":[" << transform[0] << ',' << transform[1] << ',' << transform[2] << "],\"candidateOrientation\":[";
            for (int i=3;i<12;++i) { if(i!=3) std::cout << ','; std::cout << transform[i]; }
            std::cout << ']';
        }
        const auto viewCount = viewHandle & 0xffffff;
        std::uintptr_t viewEntry=0;
        float viewPosition[3]{};
        float viewOrientation[9]{};
        if (table && viewCount && viewCount < 4096 &&
            read(process.value,table+(viewCount-1)*8,&viewEntry,8) && viewEntry &&
            read(process.value,viewEntry+0x10+0xa8,viewPosition,sizeof(viewPosition)) &&
            read(process.value,viewEntry+0x10+0xcc,viewOrientation,sizeof(viewOrientation))) {
            for (float f:viewPosition) if(!std::isfinite(f)) return 12;
            for (float f:viewOrientation) if(!std::isfinite(f)) return 12;
            std::cout << ",\"nativeViewPosition\":[" << viewPosition[0] << ',' << viewPosition[1] << ',' << viewPosition[2] << "],\"nativeViewOrientation\":[";
            for(int i=0;i<9;++i){if(i)std::cout << ',';std::cout << viewOrientation[i];}
            std::cout << ']';
        }
        std::cout << "}\n";
        return 0;
    }
    if (argc == 3 && std::strcmp(argv[2], "--commands") == 0) {
        // Build-specific research address; validate the linked records before use.
        std::uintptr_t registry = 0, entry = 0;
        if (!read(process.value, base + 0x228bae0, &registry, 8) || !registry || !read(process.value, registry + 8, &entry, 8)) return 7;
        std::unordered_set<std::uintptr_t> visited;
        std::cout << "{\"pid\":" << pid << ",\"commands\":[";
        bool first = true;
        while (entry && visited.size() < 4096 && visited.insert(entry).second) {
            std::uintptr_t fields[5]{};
            if (!read(process.value, entry, fields, sizeof(fields))) return 8;
            const auto name = readString(process.value, fields[1]);
            if (name.empty() || name.size() > 128) return 9;
            if (!first) std::cout << ',';
            first = false;
            std::cout << "{\"name\":" << jsonString(name) << ",\"description\":" << jsonString(readString(process.value, fields[4]))
                << ",\"callbackRva\":\"0x" << std::hex << (fields[2] - base) << std::dec << "\"}";
            entry = fields[0];
        }
        std::cout << "]}\n";
        return 0;
    }
    std::vector<IMAGE_SECTION_HEADER> sections(nt.FileHeader.NumberOfSections);
    if (!read(process.value, base + dos.e_lfanew + 24 + nt.FileHeader.SizeOfOptionalHeader, sections.data(), sections.size() * sizeof(IMAGE_SECTION_HEADER))) return 6;
    const std::vector<std::string> names{"win_allowMultipleInstances", "fs_savepath", "r_fullscreen", "r_windowWidth", "r_windowHeight", "r_windowPosX", "r_windowPosY", "win_pauseOnFocusLoss", "in_controlInactiveWindow", "com_showLoadingScreen", "com_timeScaler", "r_maxDynamicDecalsPreset", "r_textureDetailsPreset", "r_viewDistPreset", "r_modelsQualityPreset", "r_oceanQualityPreset", "r_shadowQualityPreset", "r_AntiAliasingPreset", "r_skipCameraMotionBlur", "r_skipSSAO"};
    std::vector<std::pair<std::string, std::uintptr_t>> strings;
    for (auto& section : sections) {
        if (!(section.Characteristics & IMAGE_SCN_MEM_READ) || (section.Characteristics & IMAGE_SCN_MEM_EXECUTE)) continue;
        std::vector<unsigned char> buffer(section.Misc.VirtualSize);
        if (!read(process.value, base + section.VirtualAddress, buffer.data(), buffer.size())) continue;
        for (auto& name : names) {
            for (size_t i = 0; i + name.size() < buffer.size(); ++i) {
                if ((i == 0 || buffer[i - 1] == 0) && std::memcmp(buffer.data() + i, name.c_str(), name.size() + 1) == 0) {
                    strings.emplace_back(name, base + section.VirtualAddress + i);
                }
            }
        }
    }
    std::cout << "PID " << pid << " image_base 0x" << std::hex << base << std::dec << '\n';
    // Build-specific, read-only window reference from the inspected debug research.
    std::uintptr_t windowReference = 0;
    if (read(process.value, base + 0x3137f40, &windowReference, 8)) std::cout << "window_reference " << windowReference << '\n';
    for (auto& section : sections) {
        if (!(section.Characteristics & IMAGE_SCN_MEM_WRITE)) continue;
        std::vector<unsigned char> buffer(section.Misc.VirtualSize);
        if (!read(process.value, base + section.VirtualAddress, buffer.data(), buffer.size())) continue;
        for (auto& [name, pointer] : strings) {
            for (size_t i = 0; i + 144 <= buffer.size(); i += 8) {
                std::uintptr_t candidate = 0;
                std::memcpy(&candidate, buffer.data() + i, 8);
                if (candidate != pointer) continue;
                // Print surrounding fields rather than assuming the private cvar layout.
                std::cout << name << " name_field_rva 0x" << std::hex << section.VirtualAddress + i << std::dec << '\n';
                if (i < 48) continue;
                // This build stores the live value 48 bytes before its name.
                // The cached integer follows at name-8. The startup code and
                // window/config values were used to cross-check this layout.
                std::uintptr_t textPointer = 0;
                std::memcpy(&textPointer, buffer.data() + i - 48, 8);
                std::int32_t intValue = 0;
                std::memcpy(&intValue, buffer.data() + i - 8, 4);
                std::cout << "  current_text: " << readString(process.value, textPointer) << " integer: " << intValue << '\n';
                if (!detail) continue;
                for (int delta = -48; delta <= 128; delta += 8) {
                    if (static_cast<std::int64_t>(i) + delta < 0) continue;
                    std::uintptr_t value = 0;
                    std::memcpy(&value, buffer.data() + i + delta, 8);
                    std::cout << "  field " << delta << " 0x" << std::hex << value << std::dec;
                    const auto text = readString(process.value, value);
                    if (!text.empty()) std::cout << " string: " << text;
                    std::cout << '\n';
                }
                std::uintptr_t internal = 0;
                std::memcpy(&internal, buffer.data() + i + 72, 8);
                for (int delta = 0; delta <= 128; delta += 8) {
                    std::uintptr_t value = 0;
                    if (!read(process.value, internal + delta, &value, 8)) break;
                    std::cout << "  internal " << delta << " 0x" << std::hex << value << std::dec;
                    const auto text = readString(process.value, value);
                    if (!text.empty()) std::cout << " string: " << text;
                    std::cout << '\n';
                }
            }
        }
    }
}





