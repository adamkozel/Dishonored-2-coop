#include <chrono>
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
#include "midnight_fx_probe.h"
#include "native_ai_state.h"
#include "native_character.h"
#include "native_gameover.h"
#include "native_player_stance.h"
#include "native_potion_statistics.h"
#include "native_player_states.h"
#include "native_player_landing_scene.h"
#include "native_player_keyhole_scene.h"
#include "native_player_grenade.h"
#include "native_player_potion.h"
#include "native_player_grenade_scene.h"
#include "native_player_stun_scene.h"
#include "native_player_combat_scene.h"
#include "native_player_equipment_scene.h"
#include "native_player_traversal_scene.h"
#include "native_player_vault.h"
#include "native_skeleton.h"
#include "coop_protocol.h"
#include "doppelganger_rig.h"
#include "native_health.h"
#include "native_sword_equipment.h"
#include "native_player_sword.h"
#include "native_player_power_item.h"
#include "native_player_darkvision_item.h"
#include "native_player_windblast_item.h"
#include "native_player_mesmerize_item.h"
#include "native_player_doppelganger_item.h"
#include "native_player_shadowwalk_item.h"
#include "native_player_shadowwalk_state.h"
#include "native_player_shadowwalk_scene.h"
#include "native_player_domino_item.h"
#include "native_player_swarm_item.h"
#include "native_player_possession_item.h"
#include "native_player_possession_target.h"
#include "native_possession_target_fx.h"
#include "native_swarm_world.h"
#include "native_swarm_trail_attachment.h"
#include "native_domino_targets.h"
#include "native_domino_links.h"
#include "native_player_domino_scene.h"
#include "native_player_swarm_scene.h"
#include "native_player_doppelganger_scene.h"
#include "native_doppelganger_world.h"
#include "native_doppelganger_fx_scene.h"
#include "native_player_possession_scene.h"
#include "native_mesmerize_world.h"
#include "native_mesmerize_eye.h"
#include "native_mesmerize_targets.h"
#include "native_player_bendtime_item.h"
#include "native_player_ranged_item.h"
#include "native_crossbow_presentation.h"
#include "native_windblast_fx_records.h"
#include "native_player_agility.h"
#include "native_player_power_scene.h"
#include "native_player_ranged_scene.h"
#include "native_player_carry_scene.h"
#include "native_retargeting.h"
#include "native_player_grapple.h"
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
    if (argc < 2 || argc > 6 || (argc==6&&std::strcmp(argv[2],"--grapple-attachment")&&std::strcmp(argv[2],"--contact-pair-final")) || (argc==5&&std::strcmp(argv[2],"--watch-potion-states")&&std::strcmp(argv[2],"--doppelganger-skeleton-final")&&std::strcmp(argv[2],"--skeleton-final")&&std::strcmp(argv[2],"--contact-pair")) || (argc==4&&std::strcmp(argv[2],"--material-params")&&std::strcmp(argv[2],"--watch-shadowwalk-state")&&std::strcmp(argv[2],"--watch-doppelganger-effects")&&std::strcmp(argv[2],"--watch-possession-effects")&&std::strcmp(argv[2],"--watch-possession-state")&&std::strcmp(argv[2],"--watch-possession-scenes")&&std::strcmp(argv[2],"--watch-swarm-effects")&&std::strcmp(argv[2],"--watch-swarm-world")&&std::strcmp(argv[2],"--watch-doppelganger-world")&&std::strcmp(argv[2],"--watch-doppelganger-scenes")&&std::strcmp(argv[2],"--watch-swarm-scenes")&&std::strcmp(argv[2],"--watch-domino-links")&&std::strcmp(argv[2],"--watch-domino-targets")&&std::strcmp(argv[2],"--watch-domino-scenes")&&std::strcmp(argv[2],"--watch-mesmerize-targets")&&std::strcmp(argv[2],"--watch-mesmerize-eyes")&&std::strcmp(argv[2],"--watch-mesmerize-world")&&std::strcmp(argv[2],"--watch-power-scenes")&&std::strcmp(argv[2],"--watch-midnight")&&std::strcmp(argv[2],"--player-potion")&&std::strcmp(argv[2],"--watch-grenade-scenes")&&std::strcmp(argv[2],"--retargeting")&&std::strcmp(argv[2],"--loading-world")&&std::strcmp(argv[2],"--components")&&std::strcmp(argv[2],"--object")&&std::strcmp(argv[2],"--bones")&&std::strcmp(argv[2],"--skeleton")&&std::strcmp(argv[2],"--doppelganger-skeleton")&&std::strcmp(argv[2],"--rat-skeleton")&&std::strcmp(argv[2],"--arm-skeleton")&&std::strcmp(argv[2],"--prop-skeleton")&&std::strcmp(argv[2],"--weapon-skeleton")&&std::strcmp(argv[2],"--health")&&std::strcmp(argv[2],"--avatar-sword")&&std::strcmp(argv[2],"--avatar-sword-attachment")&&std::strcmp(argv[2],"--sword-equipment")&&std::strcmp(argv[2],"--sword-attachment")&&std::strcmp(argv[2],"--faction")&&std::strcmp(argv[2],"--ai-targets")&&std::strcmp(argv[2],"--animation")&&std::strcmp(argv[2],"--ai-decision")&&std::strcmp(argv[2],"--ai-reflex")&&std::strcmp(argv[2],"--ai-attack")&&std::strcmp(argv[2],"--ai-idle")&&std::strcmp(argv[2],"--npc-pose"))) { std::cerr << "Usage: d2_runtime_probe PID [--detail|--commands|--player|--player-stance|--player-states|--gameover|--character|--input|--entities|--world|--world-npcs|--logic|--cinematic|--midnight|--kiscule|--loading-world ADDRESS|--components HANDLE|--object ADDRESS|--bones HANDLE|--skeleton HANDLE|--arm-skeleton HANDLE|--prop-skeleton HANDLE|--weapon-skeleton HANDLE|--health HANDLE|--faction HANDLE|--sword-equipment HANDLE|--sword-attachment HANDLE|--avatar-sword HANDLE|--avatar-sword-attachment HANDLE|--ai-targets HANDLE|--animation HANDLE|--ai-decision HANDLE|--ai-reflex HANDLE|--ai-attack HANDLE|--ai-idle HANDLE|--npc-pose HANDLE]\n"; return 2; }
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
    if((argc==5&&!std::strcmp(argv[2],"--contact-pair"))||(argc==6&&!std::strcmp(argv[2],"--contact-pair-final"))){
        // Compact read-only observation of two known entity generations.
        // Sequential reads remain explicitly non-atomic. Do not suspend the game.
        NativeSkeleton pair[2]{};const auto begin=GetTickCount64();
        auto reader=[&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);};
        std::uintptr_t publication=0;std::uint64_t before[2]{},after[2]{};
        if(argc==6){
            char* end=nullptr;publication=static_cast<std::uintptr_t>(std::strtoull(argv[5],&end,0));
            if(!end||*end||!publication||(publication&7))return 64;
            bool inMod=false;MODULEENTRY32W m{};m.dwSize=sizeof(m);
            if(Module32FirstW(snapshot.value,&m))do{const auto start=reinterpret_cast<std::uintptr_t>(m.modBaseAddr);
                if(!_wcsicmp(m.szModule,L"dinput8.dll")&&publication>=start&&publication-start+16<=m.modBaseSize)inMod=true;
            }while(Module32NextW(snapshot.value,&m));
            if(!inMod||!reader(publication,before,16))return 64;
            if(!before[0]||(before[0]&1))return 53;
        }
        for(unsigned i=0;i<2;++i){
            char* end=nullptr;const auto value=std::strtoull(argv[i+3],&end,0);
            if(!end||*end||value>0xffffffffull||!value||!readNativeSkeleton(reader,base,unsigned(value),pair[i])||!pair[i].stable)return 53;
        }
        const auto finish=GetTickCount64();
        if(pair[0].game!=pair[1].game||pair[0].actor==pair[1].actor||finish-begin>50)return 53;
        for(const auto& s:pair)if(worldIdentityEntity(reader,s.game,s.handle)!=s.actor)return 53;
        if(publication&&(!reader(publication,after,16)||std::memcmp(before,after,16)||before[1]!=pair[0].animation))return 53;
        std::cout.precision(9);std::cout<<"{\"readBeginTick\":"<<begin<<",\"readEndTick\":"<<finish<<",\"atomic\":false"
            <<",\"carrierFinalPublicationComplete\":"<<(publication?"true":"false")<<",\"publicationSequence\":"<<before[0];
        for(unsigned k=0;k<2;++k){const auto& s=pair[k];
            const auto count=(std::min)(size_t(27),s.jointIds.size());
            std::vector<std::uint16_t> ids(s.jointIds.begin(),s.jointIds.begin()+count);std::vector<std::string> names;
            if(!jointNames(process.value,base,ids,names))return 53;
            std::cout<<(k?",\"victim\":{":",\"carrier\":{")<<"\"handle\":"<<s.handle<<",\"stable\":true,\"root\":[";
            for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<s.root[i];std::cout<<"],\"joints\":[";
            for(unsigned j=0;j<count;++j){std::cout<<(j?",":"")<<"{\"index\":"<<j<<",\"nativeName\":"<<jsonString(names[j])<<",\"globalPose\":[";
                for(unsigned i=0;i<16;++i)std::cout<<(i?",":"")<<s.globalPose[j][i];std::cout<<"]}";
            }std::cout<<"]}";
        }
        std::cout<<"}\n";return 0;
    }
    if(argc==3&&(!std::strcmp(argv[2],"--windblast-effects")||!std::strcmp(argv[2],"--ranged-effects")||!std::strcmp(argv[2],"--mesmerize-effects")||!std::strcmp(argv[2],"--domino-effects")||!std::strcmp(argv[2],"--doppelganger-effects")||!std::strcmp(argv[2],"--shadowwalk-effects"))){
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        std::uintptr_t game=0;std::vector<NativeWindblastFxRecord> records;unsigned failureLine=0;NativeWindblastFxRecord diagnostic{};
        if(!reader(base+0x39a5290,&game,8)||!(std::strcmp(argv[2],"--ranged-effects")==0?
           readNativeRangedFxRecords(reader,base,game,records,&failureLine,&diagnostic):std::strcmp(argv[2],"--mesmerize-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/mesmerize_v2/")==0;},&failureLine,&diagnostic):std::strcmp(argv[2],"--domino-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/phantasmal_link/")==0;},&failureLine,&diagnostic):std::strcmp(argv[2],"--doppelganger-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/doppelganger/")==0;},&failureLine,&diagnostic):std::strcmp(argv[2],"--shadowwalk-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/shadow_walk/")==0;},&failureLine,&diagnostic):readNativeWindblastFxRecords(reader,base,game,records,&failureLine))){std::cout<<"{\"readOnly\":true,\"stable\":false,\"failureLine\":"<<failureLine<<",\"owner\":"<<diagnostic.owner<<",\"name\":"<<jsonString(diagnostic.name)<<",\"ownerActor\":\"0x"<<std::hex<<diagnostic.ownerActor<<"\"}\n";return 69;}
        std::cout.precision(9);std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"effects\":[";bool first=true;
        for(const auto& r:records){
            std::cout<<(first?"":",")<<"{\"id\":"<<r.id<<",\"owner\":"<<r.owner<<",\"parent\":"<<r.parent
                <<",\"name\":"<<jsonString(r.name)<<",\"ownerDefinition\":"<<jsonString(r.ownerDefinition)
                <<",\"record\":\"0x"<<std::hex<<r.record<<"\",\"declaration\":\"0x"<<r.declaration<<std::dec<<"\",\"origin\":[";
            for(unsigned i=0;i<3;++i)std::cout<<(i?",":"")<<r.origin[i];std::cout<<"],\"axis\":[";
            for(unsigned i=0;i<9;++i)std::cout<<(i?",":"")<<r.axis[i];std::cout<<"]}";first=false;
        }
        std::cout<<"]}\n";return 0;
    }
    // Read-only source FX research: native clone ownership is rechecked around
    // each FX map snapshot. Parent matches are evidence to investigate, not an
    // authorization to recreate effects or infer a bone attachment.
    if(argc==4 && std::strcmp(argv[2],"--watch-doppelganger-effects")==0){
        char* end=nullptr;const auto duration=std::strtoul(argv[3],&end,10);
        if(!end||*end||duration<1000||duration>32000)return 80;
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        NativeDoppelgangerItem known{};bool discovered=false;const auto started=GetTickCount64();std::cout.precision(9);
        do{
            const auto begin=std::chrono::steady_clock::now();const auto tick=GetTickCount64();
            if(!discovered){NativeDoppelgangerItem candidate{};if(readNativeDoppelgangerItem(reader,base,candidate,true)&&candidate.instantiated){known=candidate;discovered=true;}}
            NativeDoppelgangerWorld world{},after{};unsigned worldStage=0,failureLine=0;
            const bool worldRead=discovered&&readNativeDoppelgangerWorld(reader,base,world,&worldStage,&known);
            if(worldRead)known=world.equipment;
            NativeDoppelgangerFxScene cloneScene{};unsigned cloneSceneLine=0;
            const auto sceneStart=std::chrono::steady_clock::now();
            const bool sceneRead=worldRead&&world.present&&readNativeDoppelgangerFxScene(reader,base,world,cloneScene,&cloneSceneLine);
            const double sceneReadMs=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-sceneStart).count();
            std::uintptr_t game=0;std::vector<NativeWindblastFxRecord> records;
            const bool fxRead=reader(base+0x39a5290,&game,8)&&readNativeFilteredFxRecords(reader,base,game,records,
                [](const std::string& name){return name.rfind("models/effects/gameplay/powers/doppelganger/",0)==0||name=="models/effects/d2coop/doppelganger_spawn_remote.fx"||name=="models/effects/d2coop/doppelganger_despawn_remote.fx";},&failureLine);
            const bool worldStable=worldRead&&readNativeDoppelgangerWorld(reader,base,after,nullptr,&known)&&
                world.equipment.player.sword.game==game&&after.equipment.player.sword.game==game&&world.equipment.component==after.equipment.component&&
                world.handle==after.handle&&world.actor==after.actor&&world.generation==after.generation&&world.present==after.present&&world.retired==after.retired;
            const double elapsed=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-begin).count();
            std::cout<<"{\"readOnly\":true,\"valid\":"<<(fxRead?"true":"false")<<",\"tick\":"<<tick<<",\"readMs\":"<<elapsed<<",\"failureLine\":"<<failureLine
                <<",\"worldStable\":"<<(worldStable?"true":"false")<<",\"worldStage\":"<<worldStage<<",\"worldPresent\":"<<(world.present?"true":"false")
                <<",\"worldRetired\":"<<(world.retired?"true":"false")<<",\"cloneHandle\":"<<world.handle<<",\"cloneGeneration\":"<<world.generation
                <<",\"equipmentHandle\":"<<world.equipment.handle<<",\"previewHandle\":"<<world.preview<<",\"effects\":[";
            bool first=true;if(fxRead)for(const auto& r:records){
                std::uintptr_t frame=0;if(r.ownerActor)reader(r.ownerActor+0x358,&frame,8);
                std::cout<<(first?"":",")<<"{\"id\":"<<r.id<<",\"owner\":"<<r.owner<<",\"parent\":"<<r.parent<<",\"name\":"<<jsonString(r.name)
                    <<",\"ownerDefinition\":"<<jsonString(r.ownerDefinition)<<",\"ownerType\":"<<jsonString(objectType(process.value,base,r.ownerActor))
                    <<",\"frameType\":"<<jsonString(objectType(process.value,base,frame))<<",\"ownerActor\":\"0x"<<std::hex<<r.ownerActor
                    <<"\",\"frame\":\"0x"<<frame<<"\",\"record\":\"0x"<<r.record<<"\",\"declaration\":\"0x"<<r.declaration<<std::dec
                    <<"\",\"cloneOwnerMatches\":"<<(worldStable&&world.present&&r.owner==world.handle&&r.ownerActor==world.actor?"true":"false")
                    <<",\"cloneParentMatches\":"<<(worldStable&&world.present&&r.parent==world.handle?"true":"false")<<",\"origin\":[";
                for(unsigned i=0;i<3;++i)std::cout<<(i?",":"")<<r.origin[i];std::cout<<"],\"axis\":[";for(unsigned i=0;i<9;++i)std::cout<<(i?",":"")<<r.axis[i];std::cout<<"]}";first=false;
            }
            std::cout<<"],\"cloneScene\":{\"valid\":"<<(sceneRead&&worldStable?"true":"false")<<",\"id\":"<<cloneScene.id<<",\"variant\":"<<cloneScene.variant
                <<",\"timeMs\":"<<cloneScene.timeMs<<",\"durationMs\":"<<cloneScene.durationMs<<",\"failureLine\":"<<cloneSceneLine<<",\"readMs\":"<<sceneReadMs<<",\"helpers\":[";
            for(unsigned i=0;i<5;++i)std::cout<<(i?",":"")<<cloneScene.helpers[i];std::cout<<"]}}\n"<<std::flush;Sleep(50);
        }while(GetTickCount64()-started<duration);return 0;
    }
    // Read-only research inventory: deliberately outside the game hook.
    // Controller slot offsets are static candidates until correlated below.
    if(argc==4 && std::strcmp(argv[2],"--watch-possession-effects")==0){
        char* end=nullptr;const auto duration=std::strtoul(argv[3],&end,10);
        if(!end||*end||duration<1000||duration>20000)return 80;
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        const auto started=GetTickCount64();std::cout.precision(9);
        do{
            const auto sampleStart=GetTickCount64();std::uintptr_t game=0;
            NativePossessionTarget target{},after{};std::vector<NativeWindblastFxRecord> records;unsigned failureLine=0;
            NativePossessionTargetFx scoped{};unsigned scopedStage=0;
            const auto scopedStart=std::chrono::steady_clock::now();
            const bool scopedValid=readNativePossessionTargetFx(reader,base,scoped,&scopedStage);
            const double scopedMs=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-scopedStart).count();
            const bool targetRead=readNativePossessionTarget(reader,base,target);
            const bool valid=reader(base+0x39a5290,&game,8)&&readNativeFilteredFxRecords(reader,base,game,records,
                [](const std::string& name){return name.rfind("models/effects/gameplay/powers/possession/",0)==0||name=="models/effects/d2coop/possession_target_remote.fx";},&failureLine);
            struct Slot {std::uintptr_t declaration=0;unsigned id=0;bool valid=false;std::string name;};std::array<Slot,5> slots{};
            if(targetRead)for(unsigned i=0;i<slots.size();++i){auto& x=slots[i];std::uintptr_t vt=0,name=0,declAgain=0;unsigned idAgain=0;
                const auto at=target.internal+0x410+0x2b8+i*0x10;
                x.valid=reader(at,&x.declaration,8)&&reader(at+8,&x.id,4);
                if(x.valid&&x.declaration)x.valid=reader(x.declaration,&vt,8)&&vt==base+0x1ab3138&&reader(x.declaration+8,&name,8)&&worldIdentityString(reader,name,x.name);
                x.valid=x.valid&&reader(at,&declAgain,8)&&declAgain==x.declaration&&reader(at+8,&idAgain,4)&&idAgain==x.id;
            }
            const bool targetStable=targetRead&&readNativePossessionTarget(reader,base,after)&&target.player.player.game==game&&
                after.player.player.game==game&&target.internal==after.internal&&target.state==after.state&&target.phase==after.phase&&
                target.target==after.target&&target.entity==after.entity&&target.handle==after.handle&&target.generation==after.generation&&target.hasTarget==after.hasTarget;
            std::cout<<"{\"scoped\":{\"valid\":"<<(scopedValid?"true":"false")<<",\"stage\":"<<scopedStage
                <<",\"ms\":"<<scopedMs<<",\"selected\":"<<(scopedValid&&scoped.selected?"true":"false")
                <<",\"present\":"<<(scopedValid&&scoped.present?"true":"false")<<",\"target\":"<<scoped.handle
                <<",\"id\":"<<scoped.id<<",\"owner\":"<<scoped.effect.owner<<"},\"readOnly\":true,\"valid\":"<<(valid?"true":"false")<<",\"tick\":"<<sampleStart<<",\"readMs\":"<<(GetTickCount64()-sampleStart)
                <<",\"failureLine\":"<<failureLine<<",\"possessionValid\":"<<(targetStable?"true":"false")<<",\"internalState\":"<<target.state
                <<",\"targetHandle\":"<<target.handle<<",\"targetPresent\":"<<(target.hasTarget?"true":"false")<<",\"controllerSlotsResearch\":[";
            for(unsigned i=0;i<slots.size();++i){const auto& x=slots[i];std::cout<<(i?",":"")<<"{\"index\":"<<i<<",\"valid\":"<<(targetStable&&x.valid?"true":"false")
                <<",\"id\":"<<x.id<<",\"declaration\":\"0x"<<std::hex<<x.declaration<<std::dec<<"\",\"name\":"<<jsonString(x.name)<<"}";}
            std::cout<<"],\"effects\":[";bool first=true;
            if(valid)for(const auto& r:records){std::cout<<(first?"":",")<<"{\"id\":"<<r.id<<",\"owner\":"<<r.owner<<",\"parent\":"<<r.parent
                <<",\"name\":"<<jsonString(r.name)<<",\"ownerDefinition\":"<<jsonString(r.ownerDefinition)<<",\"ownerActor\":\"0x"<<std::hex<<r.ownerActor
                <<"\",\"record\":\"0x"<<r.record<<"\",\"declaration\":\"0x"<<r.declaration<<std::dec<<"\",\"origin\":[";
                for(unsigned i=0;i<3;++i)std::cout<<(i?",":"")<<r.origin[i];std::cout<<"],\"axis\":[";
                for(unsigned i=0;i<9;++i)std::cout<<(i?",":"")<<r.axis[i];std::cout<<"]}";first=false;}
            std::cout<<"]}\n"<<std::flush;Sleep(50);
        }while(GetTickCount64()-started<duration);return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--watch-swarm-effects")==0){
        char* end=nullptr;const auto duration=std::strtoul(argv[3],&end,10);
        if(!end||*end||duration<1000||duration>20000)return 80;
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        NativeSwarmItem known{};bool discovered=false;const auto started=GetTickCount64();std::cout.precision(9);
        do{
            std::uintptr_t game=0;std::vector<NativeWindblastFxRecord> records;unsigned failureLine=0;
            const bool valid=reader(base+0x39a5290,&game,8)&&readNativeFilteredFxRecords(reader,base,game,records,
                [](const std::string& name){return name.rfind("models/effects/gameplay/powers/rat_swarm/",0)==0;},&failureLine);
            NativeSwarmItem equipment{};unsigned worldHandle=0,ownerAgain=0;
            const bool equipmentValid=discovered?readCurrentNativeSwarmEquipment(reader,base,known,equipment):readNativeSwarmItem(reader,base,equipment);
            if(equipmentValid&&equipment.instantiated){known=equipment;discovered=true;reader(equipment.component+0xd4,&worldHandle,4);}
            if(!equipmentValid)discovered=false;
            const bool ownershipStable=equipmentValid&&equipment.instantiated&&reader(equipment.component+0xd4,&ownerAgain,4)&&worldHandle==ownerAgain;
            NativeSwarmWorld trailWorld{};unsigned trailStage=0;
            const bool hasTrails=valid&&std::any_of(records.begin(),records.end(),[](const auto& r){return r.name=="models/effects/gameplay/powers/rat_swarm/rat/rat_spawn_trail_01.fx";});
            const bool trailWorldValid=hasTrails&&ownershipStable&&readNativeSwarmWorld(reader,base,trailWorld,&trailStage,&equipment)&&trailWorld.present&&trailWorld.handle==worldHandle;
            std::cout<<"{\"equipmentValid\":"<<(ownershipStable?"true":"false")<<",\"equipmentHandle\":"<<equipment.handle<<",\"worldHandle\":"<<worldHandle<<",\"readOnly\":true,\"valid\":"<<(valid?"true":"false")<<",\"tick\":"<<GetTickCount64()<<",\"failureLine\":"<<failureLine<<",\"effects\":[";
            bool first=true;
            if(valid)for(const auto& r:records){
                std::cout<<(first?"":",")<<"{\"id\":"<<r.id<<",\"owner\":"<<r.owner<<",\"parent\":"<<r.parent<<",\"name\":"<<jsonString(r.name)<<",\"ownerDefinition\":"<<jsonString(r.ownerDefinition)
                    <<",\"ownerActor\":\"0x"<<std::hex<<r.ownerActor<<"\",\"record\":\"0x"<<r.record<<"\""<<std::dec<<",\"origin\":[";
                for(unsigned i=0;i<3;++i)std::cout<<(i?",":"")<<r.origin[i];std::cout<<"],\"axis\":[";
                for(unsigned i=0;i<9;++i)std::cout<<(i?",":"")<<r.axis[i];
                NativeSwarmTrailAttachment attachment{};
                const bool attached=trailWorldValid&&readNativeSwarmTrailAttachment(reader,base,trailWorld,r,attachment);
                std::cout<<"],\"trail\":{\"worldValid\":"<<(trailWorldValid?"true":"false")<<",\"worldStage\":"<<trailStage<<",\"attached\":"<<(attached?"true":"false")
                    <<",\"parentHandle\":"<<attachment.parentHandle<<",\"ratIndex\":"<<attachment.index<<",\"incarnation\":"<<attachment.incarnation<<",\"joint\":"<<attachment.joint
                    <<",\"binding\":"<<jsonString(attachment.bindingName)<<",\"frame\":\"0x"<<std::hex<<attachment.frame<<"\",\"parentActor\":\"0x"<<attachment.parent<<"\""<<std::dec<<"}}";first=false;
            }
            std::cout<<"]}\n";std::cout.flush();Sleep(100);
        }while(GetTickCount64()-started<duration);
        return 0;
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
    if(argc==3 && std::strcmp(argv[2],"--player-state-table")==0) {
        const auto reader=[&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);};
        NativePlayerStates s{};
        if(!readNativePlayerStates(reader,base,s)||!s.defaultMode)return 52;
        std::array<std::uintptr_t,90> slots{},again{};
        if(!reader(s.mode+0x10,slots.data(),sizeof(slots)))return 52;
        struct Row {unsigned id;std::uintptr_t pointer,table;std::string type;};
        std::vector<Row> rows;
        for(unsigned id=0;id<slots.size();++id){
            if(!slots[id])continue;
            std::uintptr_t vt=0,owner=0;unsigned actual=0;
            if(!reader(slots[id],&vt,8)||vt<base+0x1a7d000||vt>=base+68988928||
               !reader(slots[id]+8,&actual,4)||actual!=id||!reader(slots[id]+0x10,&owner,8)||owner!=s.component)return 52;
            rows.push_back({id,slots[id],vt,objectType(process.value,base,slots[id])});
        }
        NativePlayerStates last{};
        if(!readNativePlayerStates(reader,base,last)||last.mode!=s.mode||last.component!=s.component||last.player.game!=s.player.game||
           !reader(s.mode+0x10,again.data(),sizeof(again))||slots!=again)return 52;
        for(const auto& row:rows){std::uintptr_t vt=0,owner=0;unsigned id=0;
            if(!reader(row.pointer,&vt,8)||vt!=row.table||!reader(row.pointer+8,&id,4)||id!=row.id||
               !reader(row.pointer+0x10,&owner,8)||owner!=s.component)return 52;}
        std::cout<<"{\"scope\":\"Read-only default-mode state slots; presence is not activation or action-phase evidence\",\"states\":[";
        bool first=true;for(const auto& row:rows){std::cout<<(first?"":",")<<"{\"id\":"<<row.id<<",\"vtableRva\":\"0x"<<std::hex<<row.table-base<<std::dec<<"\",\"type\":"<<jsonString(row.type)<<'}';first=false;}
        std::cout<<"]}\n";return 0;
    }
    if(argc==4 && std::strcmp(argv[2],"--retargeting")==0) {
        NativeRetargeting s{};const auto handle=static_cast<unsigned>(std::strtoul(argv[3],nullptr,0));
        const bool valid=readNativeRetargeting([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,handle,s);
        std::cout<<"{\"valid\":"<<(valid?"true":"false")<<",\"handle\":"<<handle;
        if(valid){
            std::cout<<",\"actor\":\"0x"<<std::hex<<s.actor<<"\",\"component\":\"0x"<<s.component<<"\",\"declaration\":\"0x"<<s.declaration
                <<"\",\"records\":\"0x"<<s.records<<"\",\"target\":\"0x"<<s.target<<"\",\"targetFrame\":\"0x"<<s.targetFrame
                <<"\",\"targetPhysics\":\"0x"<<s.targetPhysics<<"\""<<std::dec<<",\"count\":"<<s.count<<",\"mode\":"<<s.mode
                <<",\"flags\":"<<s.flags<<",\"targetHandle\":"<<s.targetHandle<<",\"ownerJoint\":"<<s.ownerJoint<<",\"targetJoint\":"<<s.targetJoint;
            std::cout<<",\"ownerOffset\":[";for(unsigned i=0;i<7;++i)std::cout<<(i?",":"")<<s.ownerOffset[i];
            std::cout<<"],\"targetOffset\":[";for(unsigned i=0;i<7;++i)std::cout<<(i?",":"")<<s.targetOffset[i];std::cout<<']';
        }
        std::cout<<"}\n";return 0;
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
                <<"\",\"type\":"<<jsonString(objectType(process.value,base,state.pointer));
            if(state.id==45&&state.vtable==base+0x1c77748){
                // Read-only carry candidates from native 140829030 / 140829ff0.
                // Phase and scene correlation remains diagnostic until observed.
                unsigned scene=0,corpse=0,phase=0;unsigned char flags=0;std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x24,&scene,4)||!read(process.value,state.pointer+0xf8,&corpse,4)||
                   !read(process.value,state.pointer+0x110,&phase,4)||!read(process.value,state.pointer+0xd0,&flags,1)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                const auto actor=worldIdentityEntity([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},s.player.game,corpse);
                std::cout<<",\"carryResearch\":{\"scene24\":"<<scene<<",\"corpseF8\":"<<corpse<<",\"phase110\":"<<phase<<",\"flagsD0\":"<<unsigned(flags)
                    <<",\"actor\":\"0x"<<std::hex<<actor<<std::dec<<"\"}";
            }
            if(state.id==46&&state.vtable==base+0x1c790c8){
                unsigned scene=0,corpse=0,kind=0;unsigned char flags[3]{};std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x24,&scene,4)||!read(process.value,state.pointer+0x38,&corpse,4)||
                   !read(process.value,state.pointer+0x54,&kind,4)||!read(process.value,state.pointer+0x50,flags,3)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                std::cout<<",\"dropResearch\":{\"scene24\":"<<scene<<",\"corpse38\":"<<corpse<<",\"kind54\":"<<kind<<",\"flags50to52\":["
                    <<unsigned(flags[0])<<','<<unsigned(flags[1])<<','<<unsigned(flags[2])<<"]}";
            }
            if(state.id==18&&state.vtable==base+0x1c82b28){
                float side=0,vertical=0,scale=0;std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x40,&side,4)||!read(process.value,state.pointer+0x44,&vertical,4)||
                   !read(process.value,state.pointer+0x50,&scale,4)||!std::isfinite(side)||!std::isfinite(vertical)||!std::isfinite(scale)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                // Exact update function 14085dcc0 writes these fields before
                // calling native camera lean. Direction semantics need live proof.
                std::cout<<",\"leanResearch\":{\"side40\":"<<side<<",\"vertical44\":"<<vertical<<",\"scale50\":"<<scale<<'}';
            }
            if(state.id==12&&state.vtable==base+0x1c815d8){
                float values[9]{};unsigned char flags[2]{};std::uintptr_t component=0;float target[6]{};
                if(!read(process.value,state.pointer+0x54,values,sizeof(values))||!read(process.value,state.pointer+0xa4,flags,2)||
                   !read(process.value,s.player.player+0x380,&component,8)||!component||!read(process.value,component+0x1f4,target,12)||!read(process.value,component+0x208,target+3,12))return 52;
                for(float v:values)if(!std::isfinite(v))return 52;for(float v:target)if(!std::isfinite(v))return 52;
                std::cout<<",\"blinkResearch\":{\"state54to74\":[";for(unsigned i=0;i<9;++i)std::cout<<(i?",":"")<<values[i];
                std::cout<<"],\"componentTargets\":[";for(unsigned i=0;i<6;++i)std::cout<<(i?",":"")<<target[i];
                std::cout<<"],\"flagsA4\":"<<unsigned(flags[0])<<",\"flagsA5\":"<<unsigned(flags[1])<<'}';
            }
            if(state.id==2&&state.vtable==base+0x1c85028){
                float elapsed=0,startZ=0,targetZ=0,height=0;int remaining=0;std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x9c,&elapsed,4)||!read(process.value,state.pointer+0xa0,&startZ,4)||
                   !read(process.value,state.pointer+0xa4,&targetZ,4)||!read(process.value,state.pointer+0x90,&height,4)||
                   !read(process.value,state.pointer+0x94,&remaining,4)||!std::isfinite(elapsed)||!std::isfinite(startZ)||!std::isfinite(targetZ)||!std::isfinite(height)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                std::cout<<",\"jumpResearch\":{\"elapsed9c\":"<<elapsed<<",\"startZa0\":"<<startZ<<",\"targetZa4\":"<<targetZ<<",\"height90\":"<<height<<",\"remaining94\":"<<remaining<<'}';
            }
            if(state.id==3&&state.vtable==base+0x1c845c8){
                float distance=0,thresholds[5]{};std::uintptr_t decl=0,vt=0,owner=0;
                if(!read(process.value,state.pointer+0x18,&decl,8)||!decl||!read(process.value,state.pointer+0x88,&distance,4)||
                   !read(process.value,decl+0x158,thresholds,sizeof(thresholds))||!std::isfinite(distance)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                for(float value:thresholds)if(!std::isfinite(value))return 52;
                std::cout<<",\"fallResearch\":{\"distance88\":"<<distance<<",\"thresholds158\":[";
                for(unsigned i=0;i<5;++i)std::cout<<(i?",":"")<<thresholds[i];std::cout<<"]}";
            }
            if(state.id==24&&state.vtable==base+0x1c7c4a8){
                unsigned scene=0,door=0,phase=0;unsigned char flags[5]{};std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x24,&scene,4)||!read(process.value,state.pointer+0x1f0,&door,4)||
                   !read(process.value,state.pointer+0xec,&phase,4)||!read(process.value,state.pointer+0x220,flags,5)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                std::cout<<",\"peekingResearch\":{\"scene24\":"<<scene<<",\"door1f0\":"<<door<<",\"phaseEc\":"<<phase<<",\"flags220\":[";
                for(unsigned i=0;i<5;++i)std::cout<<(i?",":"")<<unsigned(flags[i]);std::cout<<"]}";
            }
            if(state.id==4&&state.vtable==base+0x1c82158){
                unsigned scene=0;float damage=0,threshold=0;std::uintptr_t vt=0,owner=0,declaration=0,again=0;
                if(!read(process.value,state.pointer+0x24,&scene,4)||!read(process.value,state.pointer+0x50,&damage,4)||!std::isfinite(damage)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component||
                   !read(process.value,state.pointer+0x18,&declaration,8)||!declaration||!read(process.value,declaration+0xa0,&threshold,4)||!std::isfinite(threshold)||
                   !read(process.value,state.pointer+0x18,&again,8)||again!=declaration)return 52;
                std::cout<<",\"catFallResearch\":{\"scene24\":"<<scene<<",\"damage50\":"<<damage<<",\"strongThresholdA0\":"<<threshold<<'}';
            }
            if(state.id==8&&state.vtable==base+0x1c82f68){
                // Static-only field candidates from 14085eaf0/14085fd40.
                // Report observations, never infer traversal phase yet.
                unsigned scene=0,transition=0,next=0,kind=0,target=0;float goal[3]{},angles[3]{},start[3]{},duration=0;
                std::uintptr_t vt=0,owner=0,dummy=0;
                if(!read(process.value,state.pointer+0x24,&scene,4)||!read(process.value,state.pointer+0xb4,&transition,4)||
                   !read(process.value,state.pointer+0xb8,&next,4)||!read(process.value,state.pointer+0xbc,goal,12)||
                   !read(process.value,state.pointer+0xc8,angles,12)||!read(process.value,state.pointer+0xd4,&target,4)||
                   !read(process.value,state.pointer+0xd8,&kind,4)||!read(process.value,state.pointer+0xe0,&dummy,8)||
                   !read(process.value,state.pointer+0xe8,start,12)||!read(process.value,state.pointer+0x114,&duration,4)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                for(auto x:goal)if(!std::isfinite(x))return 52;for(auto x:angles)if(!std::isfinite(x))return 52;for(auto x:start)if(!std::isfinite(x))return 52;if(!std::isfinite(duration))return 52;
                std::cout<<",\"mantleResearch\":{\"scene24\":"<<scene<<",\"transitionb4\":"<<transition<<",\"nextb8\":"<<next<<",\"kindd8\":"<<kind<<",\"targetd4\":"<<target
                    <<",\"dummyE0\":\"0x"<<std::hex<<dummy<<std::dec<<"\",\"duration114\":"<<duration<<",\"goalBc\":["<<goal[0]<<','<<goal[1]<<','<<goal[2]
                    <<"],\"anglesC8\":["<<angles[0]<<','<<angles[1]<<','<<angles[2]<<"],\"startE8\":["<<start[0]<<','<<start[1]<<','<<start[2]<<"]}";
            }
            if(state.id==9&&state.vtable==base+0x1c86e18){
                // Static references 1408715a0/140872380. Semantics still need
                // native activation and independent trajectory correlation.
                unsigned phase=0,sceneHandle=0;float values[16]{};std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x38,&phase,4)||!read(process.value,state.pointer+0x24,&sceneHandle,4)||!read(process.value,state.pointer+0x40,values,sizeof(values))||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                for(auto x:values)if(!std::isfinite(x))return 52;
                std::cout<<",\"vaultResearch\":{\"phase38\":"<<phase<<",\"scene24\":"<<sceneHandle<<",\"floats40to7c\":[";
                for(unsigned k=0;k<16;++k)std::cout<<(k?",":"")<<values[k];std::cout<<"]}";
            }
            if(state.id==7&&state.vtable==base+0x1c86108){
                float extent=0,distance=0,speed=0;unsigned next=0;unsigned char exiting=0;std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0xcc,&extent,4)||!read(process.value,state.pointer+0xd0,&distance,4)||
                   !read(process.value,state.pointer+0xe0,&speed,4)||!read(process.value,state.pointer+0xf0,&next,4)||
                   !read(process.value,state.pointer+0xf4,&exiting,1)||!std::isfinite(extent)||!std::isfinite(distance)||!std::isfinite(speed)||
                   !read(process.value,state.pointer,&vt,8)||vt!=state.vtable||!read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                std::cout<<",\"slideResearch\":{\"extentcc\":"<<extent<<",\"distanced0\":"<<distance<<",\"speede0\":"<<speed<<",\"nextf0\":"<<next<<",\"exitingf4\":"<<unsigned(exiting)<<'}';
            }
            if((state.id==39&&state.vtable==base+0x1c85d28)||(state.id==44&&state.vtable==base+0x1c81a68)||
               ((state.id==54||state.id==65)&&state.vtable==base+0x1c78a68)||
               ((state.id==53||state.id==64||state.id==80)&&state.vtable==base+0x1c79cf8)||
               (state.id==81&&state.vtable==base+0x1c7df88)){
                // Native player animation-state helper 8549c0 stores its
                // Midnight handle at +24. Correlate it before relying on it.
                unsigned sequence=0;std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0x24,&sequence,4)||!read(process.value,state.pointer,&vt,8)||vt!=state.vtable||
                   !read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                std::cout<<",\"sequence24Research\":"<<sequence;
            }
            if(state.id==39&&state.vtable==base+0x1c85d28){
                // Static references: 14086aa80 entry, 14086cf10 release,
                // 14086d060 update. Raw research fields, not interpreted states.
                unsigned b0=0,handles[3]{},f0=0;unsigned char flags[12]{};double e8=0;std::uintptr_t vt=0,owner=0;
                if(!read(process.value,state.pointer+0xb0,&b0,4)||!read(process.value,state.pointer+0xc0,handles,12)||
                   !read(process.value,state.pointer+0xd0,flags,12)||!read(process.value,state.pointer+0xe8,&e8,8)||!std::isfinite(e8)||
                   !read(process.value,state.pointer+0xf0,&f0,4)||!read(process.value,state.pointer,&vt,8)||vt!=state.vtable||
                   !read(process.value,state.pointer+0x10,&owner,8)||owner!=s.component)return 52;
                std::cout<<",\"slashResearch\":{\"b0\":"<<b0<<",\"c0_c4_c8\":["<<handles[0]<<','<<handles[1]<<','<<handles[2]<<"],\"flags_d0_db\":[";
                for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<unsigned(flags[i]);
                std::cout<<"],\"e8\":"<<e8<<",\"f0\":"<<f0<<'}';
            }
            std::cout<<'}';first=false;
        }
        std::cout<<']';
        NativePlayerLandingScene landing{};
        const bool landingValid=readNativePlayerLandingScene([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,s,landing);
        NativePlayerKeyholeScene keyhole{};unsigned keyholeStage=0;const bool keyholeValid=readNativePlayerKeyholeScene([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,s,keyhole,&keyholeStage);
        std::cout<<",\"keyholeSceneValid\":"<<(keyholeValid?"true":"false")<<",\"keyholeReaderStage\":"<<keyholeStage;
        if(keyholeValid)std::cout<<",\"keyholeScene\":{\"handle\":"<<keyhole.handle<<",\"variant\":"<<keyhole.variant<<",\"timeMs\":"<<keyhole.timeMs<<",\"durationMs\":"<<keyhole.durationMs<<",\"endStance\":"<<keyhole.endStance<<",\"doorHandle\":"<<keyhole.doorHandle<<",\"character\":"<<keyhole.character<<'}';
        std::cout<<",\"landingSceneValid\":"<<(landingValid?"true":"false");
        if(landingValid)std::cout<<",\"landingScene\":{\"handle\":"<<landing.handle<<",\"character\":"<<landing.character<<",\"timeMs\":"<<landing.timeMs<<",\"durationMs\":"<<landing.durationMs
            <<",\"asset\":"<<jsonString(landing.asset)<<",\"subscene\":"<<jsonString(landing.subscene)<<'}';
        if(landingValid){
            std::uintptr_t input=0,again=0;std::uint64_t bits=0,last=0;unsigned index=0,endIndex=0;
            const bool valid=read(process.value,s.component+0x2f8,&input,8)&&input&&read(process.value,input+0x6c,&index,4)&&index<64&&
                read(process.value,input+0xa0,&bits,8)&&read(process.value,s.component+0x2f8,&again,8)&&again==input&&
                read(process.value,input+0x6c,&endIndex,4)&&endIndex==index&&read(process.value,input+0xa0,&last,8)&&last==bits;
            if(valid)std::cout<<",\"landingExitResearch\":{\"index\":"<<index<<",\"crouched\":"<<(((bits>>index)&1)?"true":"false")<<'}';
        }
        NativePlayerCombatScene combat{};
        const bool combatValid=readNativePlayerCombatScene([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s,combat);
        std::cout<<",\"combatSceneValid\":"<<(combatValid?"true":"false");
        if(combatValid)std::cout<<",\"combatScene\":{\"handle\":"<<combat.handle<<",\"action\":"<<combat.action<<",\"variant\":"<<combat.variant
            <<",\"character\":"<<combat.character<<",\"timeMs\":"<<combat.timeMs<<",\"durationMs\":"<<combat.durationMs
            <<",\"asset\":"<<jsonString(combat.asset)<<",\"subscene\":"<<jsonString(combat.subscene)<<'}';
        NativePlayerEquipmentScene equipment{};
        NativePlayerVault vault{};const bool vaultValid=readNativePlayerVault([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s,vault);
        std::cout<<",\"vaultSceneValid\":"<<(vaultValid?"true":"false");
        if(vaultValid)std::cout<<",\"vaultScene\":{\"phase\":"<<vault.phase<<",\"handle\":"<<vault.handle<<",\"character\":"<<vault.character<<",\"variant\":"<<vault.sceneVariant<<",\"clockMs\":"<<vault.clockMs<<",\"durationMs\":"<<vault.durationMs<<",\"height\":"<<vault.height<<",\"progress\":"<<vault.progress<<'}';
        NativePlayerTraversalScene traversal{};unsigned traversalStage=0;
        const bool traversalValid=readNativePlayerTraversalScene([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s,traversal,&traversalStage);
        std::cout<<",\"traversalSceneValid\":"<<(traversalValid?"true":"false")<<",\"traversalReaderStage\":"<<traversalStage;
        if(traversalValid)std::cout<<",\"traversalScene\":{\"handle\":"<<traversal.handle<<",\"variant\":"<<traversal.variant<<",\"timeMs\":"<<traversal.timeMs<<",\"durationMs\":"<<traversal.durationMs
            <<",\"asset\":"<<jsonString(traversal.asset)<<",\"subscene\":"<<jsonString(traversal.subscene)<<'}';
        const bool equipmentValid=readNativePlayerEquipmentScene([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,s,equipment);
        std::cout<<",\"equipmentSceneValid\":"<<(equipmentValid?"true":"false");
        if(equipmentValid)std::cout<<",\"equipmentScene\":{\"handle\":"<<equipment.handle<<",\"variant\":"<<equipment.variant<<",\"character\":"<<equipment.character
            <<",\"timeMs\":"<<equipment.timeMs<<",\"durationMs\":"<<equipment.durationMs<<",\"equipped\":"<<(equipment.sword.sword.equipped?"true":"false")
            <<",\"asset\":"<<jsonString(equipment.asset)<<",\"subscene\":"<<jsonString(equipment.subscene)<<'}';
        NativePlayerPowerScene power{};const bool powerValid=readNativePlayerPowerScene([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,s,power,true);
        std::cout<<",\"powerSceneValid\":"<<(powerValid?"true":"false");
        if(powerValid)std::cout<<",\"powerScene\":{\"handle\":"<<power.handle<<",\"variant\":"<<power.variant<<",\"character\":"<<power.power.player.character
            <<",\"timeMs\":"<<power.timeMs<<",\"durationMs\":"<<power.durationMs<<",\"armHandle\":"<<power.arm.handle
            <<",\"asset\":"<<jsonString(power.asset)<<",\"subscene\":"<<jsonString(power.subscene)<<'}';
        // Read-only eligibility diagnostics from 140872380 and 1408715a0.
        NativePlayerRangedScene ranged{};const bool rangedValid=readNativePlayerRangedScene([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,s,ranged);
        std::cout<<",\"rangedSceneValid\":"<<(rangedValid?"true":"false");
        if(rangedValid)std::cout<<",\"rangedScene\":{\"handle\":"<<ranged.handle<<",\"variant\":"<<ranged.variant<<",\"kind\":"<<ranged.weapon.kind<<",\"character\":"<<ranged.weapon.player.character
            <<",\"timeMs\":"<<ranged.timeMs<<",\"durationMs\":"<<ranged.durationMs<<",\"armHandle\":"<<ranged.arm.handle<<",\"weaponHandle\":"<<ranged.weapon.handle
            <<",\"asset\":"<<jsonString(ranged.asset)<<",\"subscene\":"<<jsonString(ranged.subscene)<<'}';
        NativePlayerCarryScene carry{};const bool carryValid=readNativePlayerCarryScene([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,s,carry);
        std::cout<<",\"carrySceneValid\":"<<(carryValid?"true":"false");
        if(carryValid)std::cout<<",\"carryScene\":{\"handle\":"<<carry.handle<<",\"variant\":"<<carry.variant<<",\"stateId\":"<<carry.stateId
            <<",\"phase\":"<<carry.phase<<",\"character\":"<<carry.character<<",\"stance\":"<<carry.stance<<",\"corpseHandle\":"<<carry.corpseHandle
            <<",\"timeMs\":"<<carry.timeMs<<",\"durationMs\":"<<carry.durationMs<<",\"asset\":"<<jsonString(carry.asset)<<",\"subscene\":"<<jsonString(carry.subscene)<<'}';
        // Separate opt-in high-throw diagnostics. Default readers still refuse
        // it; the isolated source stream explicitly opts in for v36 playback.
        NativePlayerCarryScene highThrow{};
        const bool highThrowValid=!carryValid&&readNativePlayerCarryScene([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,s,highThrow,true)&&highThrow.variant==8;
        std::cout<<",\"carryThrowResearchValid\":"<<(highThrowValid?"true":"false");
        if(highThrowValid)std::cout<<",\"carryThrowResearch\":{\"handle\":"<<highThrow.handle<<",\"variant\":"<<highThrow.variant
            <<",\"stateId\":"<<highThrow.stateId<<",\"phase\":"<<highThrow.phase<<",\"character\":"<<highThrow.character
            <<",\"stance\":"<<highThrow.stance<<",\"corpseHandle\":"<<highThrow.corpseHandle<<",\"timeMs\":"<<highThrow.timeMs
            <<",\"durationMs\":"<<highThrow.durationMs<<",\"flags50to52\":["<<unsigned(highThrow.releaseFlags[0])<<','<<unsigned(highThrow.releaseFlags[1])<<','<<unsigned(highThrow.releaseFlags[2])
            <<"],\"throwVelocity\":["<<highThrow.throwVelocity[0]<<','<<highThrow.throwVelocity[1]<<','<<highThrow.throwVelocity[2]
            <<"],\"asset\":"<<jsonString(highThrow.asset)<<",\"subscene\":"<<jsonString(highThrow.subscene)<<'}';
        // The sensor flags are observations, not permission to force state9.
        std::uintptr_t vaultState=0,vaultSensor=0,vaultDecl=0,vaultVt=0,vaultOwner=0,again=0;unsigned vaultId=0;unsigned char front=0,clearance=0;
        float sensorValues[7]{},velocity[3]{},minSpeed=0;
        bool sensorOk=s.defaultMode&&read(process.value,s.mode+0x10+9*8,&vaultState,8)&&vaultState&&
            read(process.value,vaultState,&vaultVt,8)&&vaultVt==base+0x1c86e18&&read(process.value,vaultState+8,&vaultId,4)&&vaultId==9&&
            read(process.value,vaultState+0x10,&vaultOwner,8)&&vaultOwner==s.component&&read(process.value,vaultState+0x18,&vaultDecl,8)&&vaultDecl&&
            read(process.value,vaultState+0x30,&vaultSensor,8)&&vaultSensor&&read(process.value,vaultSensor+0x38,&front,1)&&read(process.value,vaultSensor+0xf0,&clearance,1)&&
            read(process.value,vaultSensor+0xf4,sensorValues,sizeof(sensorValues))&&read(process.value,vaultDecl+0x70,&minSpeed,4)&&
            read(process.value,s.player.player+0x33c,velocity,sizeof(velocity))&&
            read(process.value,vaultState+0x30,&again,8)&&again==vaultSensor&&read(process.value,s.mode+0x10+9*8,&again,8)&&again==vaultState;
        for(auto f:sensorValues)if(!std::isfinite(f))sensorOk=false;for(auto f:velocity)if(!std::isfinite(f))sensorOk=false;if(!std::isfinite(minSpeed))sensorOk=false;
        std::cout<<",\"vaultSensorResearch\":";
        if(sensorOk){std::cout<<"{\"front38\":"<<unsigned(front)<<",\"clearF0\":"<<unsigned(clearance)<<",\"minSpeed70\":"<<minSpeed<<",\"valuesF4to10c\":[";
            for(unsigned k=0;k<7;++k)std::cout<<(k?",":"")<<sensorValues[k];std::cout<<"],\"velocityCandidate33c\":["<<velocity[0]<<','<<velocity[1]<<','<<velocity[2]<<"]}";
        }else std::cout<<"null";
        std::cout<<"}\n";return 0;
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
    if(argc==6&&!std::strcmp(argv[2],"--grapple-attachment")){
        const auto handle=unsigned(std::strtoul(argv[3],nullptr,0));const auto record=std::uintptr_t(std::strtoull(argv[4],nullptr,0));
        const auto publication=std::uintptr_t(std::strtoull(argv[5],nullptr,0));
        auto liveRecord=[&](std::uintptr_t game,unsigned id){
            // Read-only reconstruction of exact native FX lookup 1a04a0.
            const auto manager=game+0x1f8430;std::uintptr_t pool=0,table=0,node=0;unsigned mask=0,lock=1;
            if(!read(process.value,manager+0x1dde9*8,&lock,4)||lock||!read(process.value,manager,&pool,8)||!pool||
               !read(process.value,manager+0x1ddec*8,&table,8)||!table||!read(process.value,manager+0x1ddee*8,&mask,4)||mask>65535||
               !read(process.value,table+(id&mask)*8,&node,8))return false;
            const auto head=node;std::uintptr_t found=0;
            for(unsigned i=0;node&&i<2048;++i){unsigned key=0;unsigned short index=0;std::uintptr_t next=0;
                if(!read(process.value,node,&key,4)||!read(process.value,node+4,&index,2)||!read(process.value,node+8,&next,8))return false;
                if(key>=id){if(key==id)found=pool+std::uintptr_t(index)*0x140;break;}if(next==node)return false;node=next;
            }
            std::uintptr_t again=0;return found==record&&read(process.value,table+(id&mask)*8,&again,8)&&again==head&&read(process.value,manager+0x1dde9*8,&lock,4)&&!lock;
        };
        NativeSkeleton s{};std::array<unsigned char,0x140> first{},last{};bool stable=false;
        for(unsigned attempt=0;attempt<32&&!stable;++attempt){
            std::uint64_t before[2]{},after[2]{};
            if(!read(process.value,publication,before,16)||!before[0]||(before[0]&1)||!read(process.value,record,first.data(),first.size())||!readNativeSkeleton([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,handle,s)||
               !read(process.value,record,last.data(),last.size())||!read(process.value,publication,after,16))continue;
            unsigned id=0;std::memcpy(&id,first.data()+4,4);
            stable=s.stable&&before[1]==s.animation&&!std::memcmp(before,after,16)&&!std::memcmp(first.data(),last.data(),24)&&!std::memcmp(first.data()+0x38,last.data()+0x38,12)&&!std::memcmp(first.data()+0x80,last.data()+0x80,12)&&liveRecord(s.game,id);
        }
        unsigned owner=0,id=0;std::uintptr_t declaration=0,callback=0,name=0,vt=0;
        std::memcpy(&owner,first.data(),4);std::memcpy(&id,first.data()+4,4);std::memcpy(&declaration,first.data()+8,8);std::memcpy(&callback,first.data()+0x128,8);
        if(!stable||owner!=handle||s.parents.size()!=219||s.parents[9]!=8||callback||s.phases.size()<4||s.phases[3].mapping.size()<10||s.phases[3].mapping[9]<0||
           !read(process.value,declaration,&vt,8)||vt!=base+0x1ab3138||!read(process.value,declaration+8,&name,8)||readString(process.value,name)!="models/effects/gameplay/powers/shadow_grapple/grapple/sgrapple_body_01.fx")return 65;
        float origin[3]{},end[3]{},hand[3]{},cacheHand[3]{};std::memcpy(origin,first.data()+0x38,12);std::memcpy(end,first.data()+0x80,12);
        for(unsigned k=0;k<3;++k){hand[k]=cacheHand[k]=s.root[k];for(unsigned c=0;c<3;++c){hand[k]+=s.root[3+c*3+k]*s.globalPose[9][12+c];cacheHand[k]+=s.root[3+c*3+k]*s.phases[3].matrices[9][12+c];}}
        float error=0,cacheError=0;for(unsigned k=0;k<3;++k){error+=(origin[k]-hand[k])*(origin[k]-hand[k]);cacheError+=(origin[k]-cacheHand[k])*(origin[k]-cacheHand[k]);}
        std::cout.precision(9);std::cout<<"{\"stable\":true,\"finalPublicationComplete\":true,\"activeRecord\":true,\"tick\":"<<GetTickCount64()<<",\"owner\":"<<owner<<",\"id\":"<<id<<",\"callback\":0,\"handError\":"<<std::sqrt(error)<<",\"cacheHandError\":"<<std::sqrt(cacheError);
        auto point=[&](const char* key,const float* p){std::cout<<",\""<<key<<"\":["<<p[0]<<','<<p[1]<<','<<p[2]<<']';};point("origin",origin);point("end",end);point("hand",hand);point("cacheHand",cacheHand);std::cout<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-grapple")){
        NativePlayerGrapple g{};const bool ok=readNativePlayerGrapple([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,g);
        std::cout.precision(9);std::cout<<"{\"valid\":"<<(ok?"true":"false")<<",\"present\":"<<(g.present?"true":"false")<<",\"pulling\":"<<(g.pulling?"true":"false")<<",\"id\":"<<g.id<<",\"flags\":"<<g.flags;
        auto point=[&](const char* name,const auto& p){std::cout<<",\""<<name<<"\":["<<p[0]<<','<<p[1]<<','<<p[2]<<']';};
        point("hand",g.hand);point("end",g.end);point("goal",g.goal);point("normal",g.normal);std::cout<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-agility")){
        NativePlayerAgility p{};if(!readNativePlayerAgility([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"entry\":\"0x"<<std::hex<<p.entry<<"\",\"declaration\":\"0x"<<p.declaration<<"\"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-power")){
        NativePlayerPowerItem p{};if(!readNativePlayerPowerItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        const auto& s=p.player.sword;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"name\":"<<jsonString(p.name)<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<s.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<",\"slot\":"<<p.slot<<",\"slots\":[";
        for(unsigned i=0;i<16;++i)std::cout<<(i?",":"")<<(s.slots[i]==p.entry?"true":"false");std::cout<<"]}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-darkvision")){
        NativeDarkVisionItem p{};if(!readNativeDarkVisionItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"manager\":\"0x"<<p.manager<<"\",\"componentFlags\":"<<std::dec<<p.flags<<",\"managerFlags\":"<<p.managerFlags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<",\"managerStarted\":"<<(p.managerStarted?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-windblast")){
        NativeWindblastItem p{};if(!readNativeWindblastItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-targets"))||(argc==4&&!std::strcmp(argv[2],"--watch-mesmerize-targets"))){
        char* end=nullptr;const auto milliseconds=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||milliseconds<100||milliseconds>45000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        NativeMesmerizeWorld discovered{};ULONGLONG discoveredAt=0;
        do{
            // Full equipment discovery spans many remote reads. Repeating it
            // inside both sides of every FX sample can cross several frames
            // and consistently miss the short native trail. The discovered
            // world is only a hint: the existing current-owner reader checks
            // all handles, component memberships and native state on every
            // attempt, then target/effect poses are still revalidated exactly.
            const auto sampleStarted=GetTickCount64();
            bool discoveredNow=false;
            if(!discovered.present||sampleStarted-discoveredAt>=1000){
                NativeMesmerizeWorld fresh{};
                if(readNativeMesmerizeWorld(reader,base,fresh)){
                    discovered=std::move(fresh);discoveredAt=sampleStarted;discoveredNow=true;
                }else discovered={};
            }
            NativeMesmerizeTargets s{};unsigned stage=1;
            bool valid=false;
            if(discovered.present)valid=readNativeMesmerizeTargets(reader,base,s,&stage,&discovered);
            else if(discoveredNow){s.world=discovered;stage=0;valid=true;}
            if(!valid&&stage==1)discovered={};
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"sampleMs\":"<<(GetTickCount64()-sampleStarted)<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage;
            if(valid){std::cout<<",\"worldPresent\":"<<(s.world.present?"true":"false")<<",\"worldHandle\":"<<s.world.handle<<",\"worldState\":"<<s.world.state<<",\"targets\":[";
                unsigned n=0;for(const auto& t:s.targets){std::cout<<(n++?",":"")<<"{\"handle\":"<<t.key.handle<<",\"definition\":"<<jsonString(t.name)<<",\"vtable\":\"0x"<<std::hex<<t.vtable-base<<std::dec<<"\",\"anchor\":"<<t.key.anchor<<",\"active\":"<<unsigned(t.key.active)<<",\"removing\":"<<unsigned(t.key.removing)<<",\"effects\":[";
                    unsigned j=0;for(const auto& e:t.effects){std::cout<<(j++?",":"")<<"{\"kind\":"<<e.kind<<",\"id\":"<<e.fx.id<<",\"owner\":"<<e.fx.owner<<",\"name\":"<<jsonString(e.fx.name)<<",\"origin\":[";
                        for(unsigned k=0;k<3;++k)std::cout<<(k?",":"")<<e.fx.origin[k];std::cout<<"],\"axis\":[";for(unsigned k=0;k<9;++k)std::cout<<(k?",":"")<<e.fx.axis[k];std::cout<<"],\"other\":[";for(unsigned k=0;k<12;++k)std::cout<<(k?",":"")<<e.other[k];std::cout<<"]}";}std::cout<<"]}";}std::cout<<']';}
            std::cout<<"}\n";if(!milliseconds)return valid?0:70;Sleep(45);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-eyes"))||(argc==4&&!std::strcmp(argv[2],"--watch-mesmerize-eyes"))){
        char* end=nullptr;const auto milliseconds=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||milliseconds<100||milliseconds>45000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeMesmerizeEyes s{};unsigned stage=0;const bool valid=readNativeMesmerizeEyes(reader,base,s,&stage);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage;
            if(valid){std::cout<<",\"worldPresent\":"<<(s.world.present?"true":"false")<<",\"worldHandle\":"<<s.world.handle<<",\"worldState\":"<<s.world.state<<",\"slots\":[";
                for(unsigned i=0;i<5;++i)std::cout<<(i?",":"")<<'['<<s.slots.entries[i][0]<<','<<s.slots.entries[i][1]<<']';
                std::cout<<"],\"eyes\":[";unsigned n=0;for(const auto& e:s.effects){std::cout<<(n++?",":"")<<"{\"id\":"<<e.id<<",\"owner\":"<<e.owner<<",\"record\":\"0x"<<std::hex<<e.record<<std::dec<<"\",\"origin\":[";
                    for(unsigned k=0;k<3;++k)std::cout<<(k?",":"")<<e.origin[k];std::cout<<"],\"axis\":[";for(unsigned k=0;k<9;++k)std::cout<<(k?",":"")<<e.axis[k];std::cout<<"]}";}std::cout<<']';}
            std::cout<<"}\n";if(!milliseconds)return valid?0:69;Sleep(35);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-world"))||(argc==4&&!std::strcmp(argv[2],"--watch-mesmerize-world"))){
        char* end=nullptr;const auto milliseconds=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||milliseconds<100||milliseconds>45000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeMesmerizeWorld w{};const bool valid=readNativeMesmerizeWorld(reader,base,w);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid){std::cout<<",\"present\":"<<(w.present?"true":"false")<<",\"handle\":"<<w.handle<<",\"state\":"<<w.state<<",\"component\":\"0x"<<std::hex<<w.component<<std::dec
                <<"\",\"equipmentHandle\":"<<w.equipment.handle<<",\"equipmentSelected\":"<<(w.equipment.equipped?"true":"false")<<",\"quantity\":"<<w.equipment.quantity<<",\"transform\":[";
                for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<w.transform[i];std::cout<<']';
                // Observational countdown: native 1407290c0 initializes +1e8;
                // 14072f3d0 decrements it. Revalidate the exact owned world
                // after reading. Never influence the native timer or lifecycle.
                float lifetime=0;NativeMesmerizeWorld after{};
                const bool timer=w.present&&reader(w.component+0x1e8,&lifetime,4)&&std::isfinite(lifetime)&&
                    lifetime>=-1.f&&lifetime<=300.f&&readNativeMesmerizeWorld(reader,base,after)&&after.present&&
                    after.actor==w.actor&&after.handle==w.handle&&after.component==w.component&&
                    after.declaration==w.declaration&&after.state==w.state&&
                    after.equipment.component==w.equipment.component&&after.equipment.handle==w.equipment.handle;
                std::cout<<",\"lifetimeObserved\":"<<(timer?"true":"false");
                if(timer)std::cout<<",\"remainingSeconds\":"<<lifetime;}
            std::cout<<"}\n";if(!milliseconds)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--domino-links"))||(argc==4&&!std::strcmp(argv[2],"--watch-domino-links"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;}
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};const auto started=GetTickCount64();NativeDominoItem known{};bool discovered=false;
        do{NativeDominoLinks s{};unsigned stage=0;LARGE_INTEGER begin{},end{},frequency{};QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&begin);
            const bool cached=discovered;const bool valid=readNativeDominoLinks(reader,base,s,&stage,cached?&known:nullptr);QueryPerformanceCounter(&end);
            if(valid&&s.network.equipment.instantiated){known=s.network.equipment;discovered=true;}else if(!valid&&stage==1)discovered=false;
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage
                <<",\"cached\":"<<(cached?"true":"false")<<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/frequency.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(s.network.present?"true":"false")<<",\"networkHandle\":"<<s.network.handle<<",\"targets\":"<<s.network.targets.size()<<",\"connections\":[";
                bool first=true;for(const auto& x:s.connections){if(!first)std::cout<<',';first=false;
                    std::cout<<"{\"from\":"<<x.from<<",\"to\":"<<x.to<<",\"state\":"<<x.state<<",\"progress\":"<<x.progress<<",\"hasBeam\":"<<(x.hasBeam?"true":"false")
                        <<",\"id\":"<<x.fxId<<",\"owner\":"<<x.owner<<",\"pointer\":\"0x"<<std::hex<<x.pointer<<"\",\"record\":\"0x"<<x.beam.record<<"\""<<std::dec;
                    if(x.hasBeam){std::cout<<",\"start\":[";for(unsigned i=0;i<12;++i){if(i)std::cout<<',';std::cout<<x.start[i];}
                        std::cout<<"],\"end\":[";for(unsigned i=0;i<12;++i){if(i)std::cout<<',';std::cout<<x.end[i];}std::cout<<']';}
                    std::cout<<'}';}std::cout<<']';}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--domino-targets"))||(argc==4&&!std::strcmp(argv[2],"--watch-domino-targets"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;}
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};const auto started=GetTickCount64();NativeDominoItem known{};bool discovered=false;
        do{NativeDominoTargets s{};unsigned stage=0;LARGE_INTEGER begin{},end{},frequency{};QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&begin);
            const bool cached=discovered;const bool valid=readNativeDominoTargets(reader,base,s,&stage,cached?&known:nullptr);QueryPerformanceCounter(&end);
            if(valid&&s.equipment.instantiated){known=s.equipment;discovered=true;}else if(!valid&&stage==1)discovered=false;
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage
                <<",\"cached\":"<<(cached?"true":"false")<<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/frequency.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(s.present?"true":"false")<<",\"networkHandle\":"<<s.handle<<",\"component\":\"0x"<<std::hex<<s.component<<"\",\"targets\":["<<std::dec;
                bool first=true;for(const auto& t:s.targets){if(!first)std::cout<<',';first=false;
                    std::cout<<"{\"handle\":"<<t.key.handle<<",\"definition\":"<<jsonString(t.name)<<",\"generation\":"<<t.key.generation<<",\"hasMark\":"<<(t.hasMark?"true":"false")
                        <<",\"mark\":"<<t.key.mark<<",\"markOwner\":"<<t.key.markOwner<<",\"node\":\"0x"<<std::hex<<t.key.node<<"\",\"actor\":\"0x"<<t.actor<<"\",\"animation\":\"0x"<<t.key.animation<<"\",\"position\":["<<std::dec;
                    for(unsigned i=0;i<3;++i){if(i)std::cout<<',';std::cout<<t.transform[i];}std::cout<<"]";
                    if(t.hasMark){std::cout<<",\"effectName\":"<<jsonString(t.mark.name)<<",\"effectPosition\":[";for(unsigned i=0;i<3;++i){if(i)std::cout<<',';std::cout<<t.mark.origin[i];}std::cout<<"]";}
                    std::cout<<'}';}std::cout<<']';}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if(argc==4&&!std::strcmp(argv[2],"--watch-domino-scenes")){
        char* end=nullptr;const auto ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePlayerStates states{};NativePlayerPowerScene s{};
            const bool stateValid=readNativePlayerStates(reader,base,states);
            const bool valid=stateValid&&readNativePlayerDominoScene(reader,base,states,s);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")
                <<",\"playerStatesValid\":"<<(stateValid?"true":"false");
            if(stateValid){std::cout<<",\"stance\":"<<states.player.current<<",\"activeStates\":[";bool first=true;
                for(const auto& a:states.active){if(!first)std::cout<<",";first=false;std::cout<<a.id;}std::cout<<"]";}
            if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
                <<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle;
            std::cout<<"}\n";Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-domino-scene")){
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};NativePlayerStates states{};NativePlayerPowerScene s{};
        if(!readNativePlayerStates(reader,base,states))return 59;
        const bool valid=readNativePlayerDominoScene(reader,base,states,s);
        std::cout<<"{\"readOnly\":true,\"valid\":"<<(valid?"true":"false");
        if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
            <<",\"asset\":"<<jsonString(s.asset)<<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle;
        std::cout<<"}\n";return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--swarm-world"))||(argc==4&&!std::strcmp(argv[2],"--watch-swarm-world"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>45000)return 2;}
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};const auto started=GetTickCount64();
        std::cout.precision(9);NativeSwarmItem known{};bool discovered=false;
        do{NativeSwarmWorld w{};unsigned stage=0;LARGE_INTEGER begin{},end{},frequency{};QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&begin);
            const bool cached=discovered;const bool valid=readNativeSwarmWorld(reader,base,w,&stage,cached?&known:nullptr);
            if(valid&&w.equipment.instantiated){known=w.equipment;discovered=true;}else if(!valid&&stage==1)discovered=false;QueryPerformanceCounter(&end);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage
                <<",\"cached\":"<<(cached?"true":"false")<<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/frequency.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(w.present?"true":"false")<<",\"handle\":"<<w.handle<<",\"capacity\":"<<w.capacity
                <<",\"component\":\"0x"<<std::hex<<w.component<<"\",\"pool\":\"0x"<<w.pool<<"\",\"pipeline\":\"0x"<<w.pipeline<<"\""<<std::dec<<",\"agents\":[";
                bool first=true;for(const auto& a:w.agents){if(!first)std::cout<<',';first=false;
                    std::cout<<"{\"index\":"<<a.index<<",\"handle\":"<<a.handle<<",\"incarnation\":"<<a.incarnation<<",\"state\":"<<a.state
                        <<",\"directFrameDriver\":"<<(a.drivingFrame?"true":"false")<<",\"frameGeneration\":"<<a.frameGeneration<<",\"animationGeneration\":"<<a.animationGeneration<<",\"scale\":"<<a.scale<<",\"transform\":[";
                    for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<a.transform[i];std::cout<<"]}";
                }std::cout<<']';}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if((argc==4&&!std::strcmp(argv[2],"--watch-possession-state"))||(argc==3&&!std::strcmp(argv[2],"--player-possession-state"))){
        unsigned long ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;}
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePossessionTarget p{};const bool valid=readNativePossessionTarget(reader,base,p);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\"internalState\":"<<p.state<<",\"phaseWord\":"<<p.phase<<",\"physicsStance\":"<<p.player.player.current
                <<",\"defaultMode\":"<<(p.player.defaultMode?"true":"false")<<",\"hasTarget\":"<<(p.hasTarget?"true":"false")
                <<",\"handle\":"<<p.handle<<",\"generation\":"<<p.generation<<",\"flags\":"<<p.flags
                <<",\"machine\":\"0x"<<std::hex<<p.player.component<<"\",\"internal\":\"0x"<<p.internal<<"\",\"target\":\"0x"<<p.target<<"\",\"entity\":\"0x"<<p.entity<<"\""<<std::dec;
            std::cout<<"}\n";if(ms)Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if(argc==4&&!std::strcmp(argv[2],"--watch-possession-scenes")){
        char* end=nullptr;const auto ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{unsigned readerStage=0;NativePlayerStates states{};NativePlayerPowerScene s{};
            const bool stateValid=readNativePlayerStates(reader,base,states);
            const bool valid=stateValid&&readNativePlayerPossessionScene(reader,base,states,s,true,&readerStage);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")
                <<",\"readerStage\":"<<readerStage<<",\"playerStatesValid\":"<<(stateValid?"true":"false");
            if(stateValid){std::cout<<",\"stance\":"<<states.player.current<<",\"activeStates\":[";bool first=true;
                for(const auto& a:states.active){if(!first)std::cout<<",";first=false;std::cout<<a.id;}std::cout<<"]";}
            if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
                <<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle
                <<",\"swordDrawVariant\":"<<s.swordDraw.variant<<",\"swordDrawHandle\":"<<s.swordDraw.handle
                <<",\"swordDrawTimeMs\":"<<s.swordDraw.timeMs<<",\"swordDrawDurationMs\":"<<s.swordDraw.durationMs;
            std::cout<<"}\n";Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-possession-scene")){
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};NativePlayerStates states{};NativePlayerPowerScene s{};
        if(!readNativePlayerStates(reader,base,states))return 59;
        const bool valid=readNativePlayerPossessionScene(reader,base,states,s,true);
        std::cout<<"{\"readOnly\":true,\"valid\":"<<(valid?"true":"false");
        if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
            <<",\"asset\":"<<jsonString(s.asset)<<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle;
        std::cout<<"}\n";return 0;
    }
    if(argc==4&&!std::strcmp(argv[2],"--watch-swarm-scenes")){
        char* end=nullptr;const auto ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePlayerStates states{};NativePlayerPowerScene s{};
            const bool stateValid=readNativePlayerStates(reader,base,states);
            const bool valid=stateValid&&readNativePlayerSwarmScene(reader,base,states,s);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")
                <<",\"playerStatesValid\":"<<(stateValid?"true":"false");
            if(stateValid){std::cout<<",\"stance\":"<<states.player.current<<",\"activeStates\":[";bool first=true;
                for(const auto& a:states.active){if(!first)std::cout<<",";first=false;std::cout<<a.id;}std::cout<<"]";}
            if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
                <<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle;
            std::cout<<"}\n";Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }

    if((argc==3&&!std::strcmp(argv[2],"--doppelganger-world"))||(argc==4&&!std::strcmp(argv[2],"--watch-doppelganger-world"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>45000)return 2;}
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        NativeDoppelgangerItem known{};bool haveKnown=false;
        do{NativeDoppelgangerWorld s{};unsigned stage=0;LARGE_INTEGER begin{},end{},freq{};QueryPerformanceFrequency(&freq);QueryPerformanceCounter(&begin);
            const bool cached=haveKnown;
            const bool valid=readNativeDoppelgangerWorld(reader,base,s,&stage,haveKnown?&known:nullptr);QueryPerformanceCounter(&end);
            if(valid&&s.equipment.instantiated){known=s.equipment;haveKnown=true;}else if(!valid&&stage==1){haveKnown=false;}
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage<<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/freq.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(s.present?"true":"false")<<",\"count\":"<<(s.arrayFlags&0xffffff)<<",\"preview\":"<<s.preview<<",\"last\":"<<s.last<<",\"equipmentHandle\":"<<s.equipment.handle<<",\"cached\":"<<(cached?"true":"false")<<",\"retired\":"<<(s.retired?"true":"false")<<",\"registryActor\":\"0x"<<std::hex<<s.registryActor<<"\",\"registryHandle\":"<<std::dec<<s.registryHandle;
                if(s.present){std::cout<<",\"handle\":"<<s.handle<<",\"generation\":"<<s.generation<<",\"rawA0\":"<<s.rawA0<<",\"actor\":\"0x"<<std::hex<<s.actor<<"\",\"component\":\"0x"<<s.component<<"\",\"transform\":["<<std::dec;for(unsigned i=0;i<12;++i){if(i)std::cout<<',';std::cout<<s.transform[i];}std::cout<<']';}}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(50);
        }while(GetTickCount64()-started<ms);return 0;
    }

    if((argc==4&&!std::strcmp(argv[2],"--watch-shadowwalk-state"))||(argc==3&&!std::strcmp(argv[2],"--player-shadowwalk-state"))){
        char* end=nullptr;const auto ms=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||ms<100||ms>30000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeShadowWalkState s{};const bool valid=readNativeShadowWalkState(reader,base,s);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\"phase\":"<<s.phase<<",\"form\":"<<(s.form?"true":"false")<<",\"stance\":"<<s.states.player.current
                <<",\"swordState\":"<<(s.item.player.sword.equipped?2:1)<<",\"equipmentHandle\":"<<s.item.handle
                <<",\"mode\":\"0x"<<std::hex<<s.states.mode<<"\",\"component\":\"0x"<<s.item.component<<"\""<<std::dec;
            NativePlayerPowerScene scene{};const bool sceneValid=valid&&readNativePlayerShadowWalkScene(reader,base,s.states,scene);
            std::cout<<",\"sceneValid\":"<<(sceneValid?"true":"false");
            if(sceneValid)std::cout<<",\"variant\":"<<scene.variant<<",\"scene\":"<<scene.handle<<",\"timeMs\":"<<scene.timeMs<<",\"durationMs\":"<<scene.durationMs;
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if(argc==4&&!std::strcmp(argv[2],"--watch-doppelganger-scenes")){
        char* end=nullptr;const auto ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePlayerStates states{};NativePlayerPowerScene s{};
            const bool stateValid=readNativePlayerStates(reader,base,states);
            const bool valid=stateValid&&readNativePlayerDoppelgangerScene(reader,base,states,s);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")
                <<",\"playerStatesValid\":"<<(stateValid?"true":"false");
            if(stateValid){std::cout<<",\"stance\":"<<states.player.current<<",\"activeStates\":[";bool first=true;
                for(const auto& a:states.active){if(!first)std::cout<<",";first=false;std::cout<<a.id;}std::cout<<"]";}
            if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
                <<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle;
            std::cout<<"}\n";Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-swarm-scene")){
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};NativePlayerStates states{};NativePlayerPowerScene s{};
        if(!readNativePlayerStates(reader,base,states))return 59;
        const bool valid=readNativePlayerSwarmScene(reader,base,states,s);
        std::cout<<"{\"readOnly\":true,\"valid\":"<<(valid?"true":"false");
        if(valid)std::cout<<",\"variant\":"<<s.variant<<",\"handle\":"<<s.handle<<",\"timeMs\":"<<s.timeMs<<",\"durationMs\":"<<s.durationMs
            <<",\"asset\":"<<jsonString(s.asset)<<",\"subscene\":"<<jsonString(s.subscene)<<",\"armHandle\":"<<s.arm.handle<<",\"equipmentHandle\":"<<s.power.handle;
        std::cout<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-possession")){
        NativePossessionItem p{};if(!readNativePossessionItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-swarm")){
        NativeSwarmItem p{};if(!readNativeSwarmItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-domino")){
        NativeDominoItem p{};if(!readNativeDominoItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-mesmerize")){
        NativeMesmerizeItem p{};if(!readNativeMesmerizeItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-doppelganger")){
        NativeDoppelgangerItem p{};if(!readNativeDoppelgangerItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-shadowwalk")){
        NativeShadowWalkItem p{};if(!readNativeShadowWalkItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-bendtime")){
        NativeBendTimeItem p{};if(!readNativeBendTimeItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p))return 59;
        std::cout<<"{\"readOnly\":true,\"character\":"<<p.player.character<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags
          <<",\"powerActive\":"<<(p.powerActive?"true":"false")<<",\"instantiated\":"<<(p.instantiated?"true":"false")<<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if(argc==3&&(!std::strcmp(argv[2],"--player-gun")||!std::strcmp(argv[2],"--player-crossbow"))){
        NativePlayerRangedItem p{};const unsigned kind=!std::strcmp(argv[2],"--player-gun")?1u:2u;
        if(!readNativePlayerRangedItem([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,kind,p))return 59;
        NativeRangedAmmo ammo{};NativeRangedDefaultUpgrade upgrade{};
        NativeCrossbowPresentation mechanical{};const bool mechanismValid=readNativeCrossbowPresentation([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p,mechanical);
        const bool ammoValid=readNativeRangedAmmo([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p,ammo);
        const bool upgradeValid=readNativeRangedDefaultUpgrade([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p,upgrade);
        std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"character\":"<<p.player.character<<",\"kind\":"<<p.kind<<",\"name\":"<<jsonString(p.name)<<",\"equipment\":"<<jsonString(p.equipmentName)<<",\"quantity\":"<<p.quantity<<",\"handle\":"<<p.handle
          <<",\"component\":\"0x"<<std::hex<<p.component<<"\",\"entity\":\"0x"<<p.entity<<"\",\"inventory\":\"0x"<<p.player.sword.inventory<<"\",\"declaration\":\"0x"<<p.declaration
          <<"\",\"entry\":\"0x"<<p.entry<<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"componentFlags\":"<<std::dec<<p.flags<<",\"slot\":"<<p.slot
          <<",\"mechanismValid\":"<<(mechanismValid?"true":"false")<<",\"mechanism\":"<<mechanical.state<<",\"ammoValid\":"<<(ammoValid?"true":"false")<<",\"ammo\":"<<ammo.quantity<<",\"defaultUpgradeValid\":"<<(upgradeValid?"true":"false")<<",\"defaultUpgradeQuantity\":"<<upgrade.quantity
          <<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")<<"}\n";return 0;
    }
    if((argc==3&&(!std::strcmp(argv[2],"--player-grenade-scene")||!std::strcmp(argv[2],"--player-stun-scene")))||(argc==4&&!std::strcmp(argv[2],"--watch-grenade-scenes"))){
        const unsigned watchMs=argc==4?static_cast<unsigned>(std::strtoul(argv[3],nullptr,10)):0;
        if(argc==4&&(watchMs<100||watchMs>12000))return 2;
        const auto start=GetTickCount64();
        do{
        const auto reader=[&](auto a,auto d,auto n){return read(process.value,a,d,n);};
        NativePlayerStates states{};NativePlayerGrenadeScene p{};
        const bool stateValid=readNativePlayerStates(reader,base,states);
        const bool valid=stateValid&&readNativePlayerGrenadeScene(reader,base,states,p);
        std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")
          <<",\"character\":"<<p.grenade.player.character<<",\"variant\":"<<p.variant<<",\"handle\":"<<p.handle
          <<",\"timeMs\":"<<p.timeMs<<",\"durationMs\":"<<p.durationMs<<",\"armHandle\":"<<p.arm.handle
          <<",\"itemHandle\":"<<p.grenade.handle<<",\"equipped\":"<<(p.grenade.equipped?"true":"false")
          <<",\"asset\":"<<jsonString(p.asset)<<",\"subscene\":"<<jsonString(p.subscene);
        // Retain exact active native IDs even outside grenade scenes, so a
        // later hurt/unknown state can be correlated with stream retirement.
        // These are observations, never inferred animation/action ownership.
        unsigned motion=0;
        const bool motionValid=stateValid&&nativePlayerMotion(states,base,motion);
        std::cout<<",\"stateValid\":"<<(stateValid?"true":"false")<<",\"defaultMode\":"<<(states.defaultMode?"true":"false")
            <<",\"stance\":"<<states.player.current<<",\"motionValid\":"<<(motionValid?"true":"false")<<",\"motion\":"<<motion<<",\"active\":[";
        bool firstState=true;
        if(stateValid)for(const auto& a:states.active){std::cout<<(firstState?"":",")<<"{\"id\":"<<a.id<<",\"vtableRva\":\"0x"<<std::hex<<a.vtable-base<<std::dec<<"\"}";firstState=false;}
        NativePlayerStunScene stun{};const bool stunValid=stateValid&&readNativePlayerStunScene(reader,base,states,stun);
        std::cout<<"],\"stun\":{\"valid\":"<<(stunValid?"true":"false")<<",\"variant\":"<<stun.variant<<",\"timeMs\":"<<stun.timeMs<<",\"durationMs\":"<<stun.durationMs<<",\"handle\":"<<stun.handle<<",\"asset\":"<<jsonString(stun.asset)<<"}}\n";
        if(!watchMs)break;
        std::cout.flush();Sleep(15);
        }while(GetTickCount64()-start<watchMs);
        return 0;
    }
    if((argc==4&&!std::strcmp(argv[2],"--player-potion"))||(argc==5&&!std::strcmp(argv[2],"--watch-potion-states"))){
        const unsigned kind=static_cast<unsigned>(std::strtoul(argv[3],nullptr,10));
        const unsigned watchMs=argc==5?static_cast<unsigned>(std::strtoul(argv[4],nullptr,10)):0;
        if((kind!=1&&kind!=2)||(argc==5&&(watchMs<100||watchMs>12000)))return 2;
        const auto start=GetTickCount64();const auto reader=[&](auto a,auto d,auto n){return read(process.value,a,d,n);};
        do{NativePlayerPotion p{};NativePlayerStates states{};NativeHealth health{};NativePotionStatistics stats{};
            const bool valid=readNativePlayerPotion(reader,base,kind,p),stateValid=readNativePlayerStates(reader,base,states),healthValid=readNativeHealth(reader,base,1,health);
            const bool statisticsValid=readNativePotionStatistics(reader,base,stats);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"character\":"<<p.player.character<<",\"kind\":"<<kind<<",\"quantity\":"<<p.quantity<<",\"maximum\":"<<p.maximum
              <<",\"entry\":\"0x"<<std::hex<<p.entry<<"\",\"declaration\":\"0x"<<p.declaration<<std::dec<<"\",\"stateValid\":"<<(stateValid?"true":"false")<<",\"stance\":"<<states.player.current<<",\"healthValid\":"<<(healthValid?"true":"false")<<",\"health\":"<<health.current<<",\"active\":[";
            bool first=true;if(stateValid)for(const auto& a:states.active){std::cout<<(first?"":",")<<"{\"id\":"<<a.id<<",\"vtableRva\":\"0x"<<std::hex<<a.vtable-base<<std::dec<<"\"}";first=false;}
            std::cout<<"],\"statisticsValid\":"<<(statisticsValid?"true":"false")<<",\"statisticsA\":["<<stats.a[0]<<','<<stats.a[1]<<"],\"statisticsB\":["<<stats.b[0]<<','<<stats.b[1]<<"]}\n";if(!watchMs)break;std::cout.flush();Sleep(15);
        }while(GetTickCount64()-start<watchMs);return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-grenade")){
        NativePlayerGrenade p{};unsigned stage=0;
        if(!readNativePlayerGrenade([&](auto a,auto d,auto n){return read(process.value,a,d,n);},base,p,&stage)){std::cerr<<"Grenade reader refused stage="<<stage<<'\n';return 59;}
        std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"character\":"<<p.player.character
            <<",\"quantity\":"<<p.quantity<<",\"maximum\":"<<p.maximum<<",\"handle\":"<<p.handle<<",\"slot\":"<<p.slot
            <<",\"equipped\":"<<(p.equipped?"true":"false")<<",\"started\":"<<(p.started?"true":"false")
            <<",\"lowThrowCandidate\":"<<(p.lowThrowCandidate?"true":"false")<<",\"flags\":"<<p.flags
            <<",\"entry\":\"0x"<<std::hex<<p.entry<<"\",\"component\":\"0x"<<p.component
            <<"\",\"equipmentDeclaration\":\"0x"<<p.equipmentDeclaration<<"\",\"entity\":\"0x"<<p.entity<<"\"}\n";return 0;
    }
    if(argc==3&&!std::strcmp(argv[2],"--player-inventory")){
        const auto reader=[&](auto p,auto b,auto size){return read(process.value,p,b,size);};NativePlayerSword first{},last{};
        if(!readNativePlayerSword(reader,base,first))return 59;
        const auto& inv=first.sword;
        std::vector<std::array<unsigned char,56>> raw(inv.itemCount),again(inv.itemCount);
        if(!reader(inv.items,raw.data(),raw.size()*56))return 59;
        struct Entry{std::uintptr_t decl=0,declVt=0,component=0,componentVt=0;unsigned handle=0,type=0xffffffffu;int quantity=0,maximum=0;std::string name;};
        std::vector<Entry> entries(inv.itemCount);
        for(unsigned i=0;i<inv.itemCount;++i){auto& e=entries[i];const auto* p=raw[i].data();std::uintptr_t vt=0,name=0;
            std::memcpy(&e.decl,p,8);std::memcpy(&e.handle,p+8,4);std::memcpy(&e.component,p+16,8);std::memcpy(&e.quantity,p+32,4);std::memcpy(&e.maximum,p+36,4);
            // Exact RTTI confirms inventory items and equipment upgrades coexist
            // in this native list. The upgrade's +60 is NOT an inventory type.
            if(!e.decl||!reader(e.decl,&vt,8)||(vt!=base+0x1b9ea48&&vt!=base+0x1b555a8)||!reader(e.decl+8,&name,8)||!worldIdentityString(reader,name,e.name)||
               (vt==base+0x1b9ea48&&!reader(e.decl+0x60,&e.type,4))||(e.component&&!reader(e.component,&e.componentVt,8))){std::cerr<<"Inventory entry refused index="<<i<<" decl="<<std::hex<<e.decl<<" vtRva="<<(vt?vt-base:0)<<" name="<<e.name<<" component="<<e.component<<'\n';return 59;}e.declVt=vt;
        }
        if(!reader(inv.items,again.data(),again.size()*56)||again!=raw||!readNativePlayerSword(reader,base,last)||
           !sameSwordEquipmentIdentity(first.sword,last.sword)||first.character!=last.character)return 59;
        std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"character\":"<<first.character<<",\"inventory\":\"0x"<<std::hex<<inv.inventory<<"\",\"items\":[";
        for(unsigned i=0;i<entries.size();++i){const auto& e=entries[i];std::cout<<(i?",":"")<<"{\"name\":"<<jsonString(e.name)<<",\"declaration\":\"0x"<<std::hex<<e.decl
            <<"\",\"declarationVtableRva\":\"0x"<<(e.declVt-base)<<"\",\"component\":\"0x"<<e.component<<"\",\"componentVtableRva\":\"0x"<<(e.componentVt?e.componentVt-base:0)<<"\",\"handle\":"<<std::dec<<e.handle<<",\"type\":"<<e.type
            <<",\"quantity\":"<<e.quantity<<",\"maximum\":"<<e.maximum<<"}";}
        std::cout<<"]}\n";return 0;
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
        unsigned char visibility=0;if(!read(process.value,s.entity+0xd0,&visibility,1))return 57;
        std::cout<<"{\"tick\":"<<GetTickCount64()<<",\"handle\":"<<s.handle<<",\"logic\":"<<s.logic
            <<",\"inventory\":\"0x"<<std::hex<<s.inventory<<"\",\"entry\":\"0x"<<s.entry<<"\",\"component\":\"0x"<<s.component
            <<"\",\"entity\":\"0x"<<s.entity<<"\",\"frame\":\"0x"<<s.frame<<"\",\"parent\":\"0x"<<s.parent<<"\""<<std::dec
            <<",\"weaponHandle\":"<<s.weaponHandle<<",\"slot\":"<<s.slot<<",\"quantity\":"<<s.quantity<<",\"item\":"<<jsonString(s.itemName)
            <<",\"equipment\":"<<jsonString(s.equipmentName)<<",\"equipped\":"<<(s.equipped?"true":"false")
            <<",\"started\":"<<(s.started?"true":"false")<<",\"flags\":"<<s.componentFlags<<",\"joint\":"<<s.joint
            <<",\"explicitHidden\":"<<((visibility&1)?"true":"false")<<",\"attachedTag\":"<<jsonString(s.attachedTag)<<",\"root\":[";
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
    if((argc==5&&(!std::strcmp(argv[2],"--doppelganger-skeleton-final")||!std::strcmp(argv[2],"--skeleton-final")))||(argc==4 && (std::strcmp(argv[2],"--skeleton")==0||std::strcmp(argv[2],"--doppelganger-skeleton")==0||std::strcmp(argv[2],"--rat-skeleton")==0||(std::strcmp(argv[2],"--arm-skeleton")==0||std::strcmp(argv[2],"--prop-skeleton")==0||std::strcmp(argv[2],"--weapon-skeleton")==0)))) {
        std::uintptr_t publication=0;std::uint64_t before[2]{},after[2]{};
        if(argc==5){
            char* end=nullptr;publication=static_cast<std::uintptr_t>(std::strtoull(argv[4],&end,0));
            if(!end||*end||(publication&7))return 64;
            bool inMod=false;MODULEENTRY32W m{};m.dwSize=sizeof(m);
            if(Module32FirstW(snapshot.value,&m))do{const auto start=reinterpret_cast<std::uintptr_t>(m.modBaseAddr);
                if(!_wcsicmp(m.szModule,L"dinput8.dll")&&publication>=start&&publication-start+(!std::strcmp(argv[2],"--doppelganger-skeleton-final")?24+219*48:16)<=m.modBaseSize)inMod=true;
            }while(Module32NextW(snapshot.value,&m));
            if(!inMod||!read(process.value,publication,before,16))return 64;
        }
        // Cosmetic scope includes only dedicated ranged and grenade props;
        // the target reader rejects gameplay-owned weapons and other entities.
        NativeSkeleton s{};
        if(!readNativeSkeleton([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},base,
           static_cast<std::uint32_t>(std::strtoul(argv[3],nullptr,0)),s,(std::strcmp(argv[2],"--doppelganger-skeleton")==0||std::strcmp(argv[2],"--doppelganger-skeleton-final")==0)?NativeSkeletonScope::CosmeticDoppelganger:std::strcmp(argv[2],"--rat-skeleton")==0?NativeSkeletonScope::CosmeticRat:std::strcmp(argv[2],"--weapon-skeleton")==0?NativeSkeletonScope::PlayerRanged:std::strcmp(argv[2],"--prop-skeleton")==0?NativeSkeletonScope::CosmeticRanged:std::strcmp(argv[2],"--arm-skeleton")==0?NativeSkeletonScope::PlayerArm:NativeSkeletonScope::Actor)){
            std::cout<<"{\"stable\":false,\"failureLine\":"<<s.failureLine<<",\"model\":"<<jsonString(s.modelName)<<",\"skeleton\":"<<jsonString(s.skeletonName)<<"}\n";return 53;
        }
        const bool cloneFinal=argc==5&&!std::strcmp(argv[2],"--doppelganger-skeleton-final");
        std::array<std::array<float,12>,219> expectedClone{};std::uint64_t expectedSequence=0;double poseError=0;
        if(cloneFinal){
            if(s.localPose.size()!=219||!read(process.value,publication+16,&expectedSequence,8)||!read(process.value,publication+24,expectedClone.data(),sizeof(expectedClone)))return 64;
            for(unsigned j=0;j<219;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r){const double expected=expectedClone[j][c*3+r];if(!std::isfinite(expected))return 64;poseError=(std::max)(poseError,std::fabs(double(s.localPose[j][c*4+r])-expected));}
        }
        bool finalComplete=false;
        if(publication){if(!read(process.value,publication,after,16))return 64;
            finalComplete=before[0]&&!(before[0]&1)&&!std::memcmp(before,after,16)&&before[1]==s.animation;s.stable=s.stable&&finalComplete;}
        std::vector<std::string> names;const bool namesValid=jointNames(process.value,base,s.jointIds,names);
        std::cout.precision(9);
        auto floats=[](const float* values,unsigned n){std::cout<<'[';for(unsigned i=0;i<n;++i)std::cout<<(i?",":"")<<values[i];std::cout<<']';};
        std::cout<<"{\"handle\":"<<s.handle<<",\"finalPublicationComplete\":"<<(finalComplete?"true":"false")<<",\"publicationBefore\":"<<before[0]<<",\"publicationAfter\":"<<after[0]<<",\"armCharacter\":"<<s.playerArm.character<<",\"armSide\":"<<s.playerArm.side<<",\"stable\":"<<(s.stable?"true":"false")<<",\"model\":"<<jsonString(s.modelName)
            <<",\"skeleton\":"<<jsonString(s.skeletonName)<<",\"root\":";floats(s.root,12);
        std::cout<<",\"cloneRig\":\""<<std::hex<<doppelgangerRig(s)<<"\",\"cloneRigRaw\":\""<<doppelgangerRig(s,false)<<"\""<<std::dec;
        std::cout<<",\"cloneExpectedSequence\":"<<expectedSequence<<",\"cloneExpectedPoseError\":"<<poseError<<",\"nativeNamesValid\":"<<(namesValid?"true":"false")<<",\"joints\":[";
        for(unsigned j=0;j<s.jointIds.size();++j){
            std::cout<<(j?",":"")<<"{\"index\":"<<j<<",\"nativeNameId\":"<<s.jointIds[j]<<",\"nativeName\":"<<jsonString(namesValid?names[j]:"")<<",\"parent\":"<<s.parents[j]<<",\"bindLocalQTS\":";
            floats(s.bindLocal[j].data(),12);std::cout<<",\"localPose\":";floats(s.localPose[j].data(),16);
            std::cout<<",\"globalPose\":";floats(s.globalPose[j].data(),16);std::cout<<'}';
        }
        std::uintptr_t treePointer=0;unsigned char treeActive=0,treeHasOutput=0;
        read(process.value,s.animation+0x350,&treePointer,8);read(process.value,s.animation+0x3ec,&treeActive,1);read(process.value,s.animation+0x3ed,&treeHasOutput,1);
        std::cout<<"],\"treePresent\":"<<(treePointer?"true":"false")<<",\"treeActive\":"<<unsigned(treeActive)<<",\"treeHasOutput\":"<<unsigned(treeHasOutput)<<",\"phases\":[";
        for(unsigned p=0;p<s.phases.size();++p) {
            std::cout<<(p?",":"")<<"{\"phase\":"<<p<<",\"matrices\":[";bool first=true;
            for(unsigned j=0;j<s.jointIds.size()&&!s.phases[p].mapping.empty();++j) {
                if(s.phases[p].mapping[j]<0)continue;
                std::cout<<(first?"":",")<<"{\"joint\":"<<j<<",\"cacheIndex\":"<<s.phases[p].mapping[j]<<",\"matrix\":";first=false;
                floats(s.phases[p].matrices[j].data(),16);std::cout<<'}';
            }
            std::cout<<"]}";
        }
        std::cout<<"],\"weaponTreeBindings\":[";
        if(std::strcmp(argv[2],"--weapon-skeleton")==0){
            // Read-only diagnostic of the native equipment graph, whose
            // declaration uses the same bounded input table as NPC graphs.
            std::uintptr_t tree=0,data=0,decl=0,buckets=0;unsigned mask=0;bool first=true;
            if(read(process.value,s.animation+0x338,&tree,8)&&tree&&read(process.value,tree,&data,8)&&data&&
               read(process.value,s.animation+0x340,&decl,8)&&decl&&read(process.value,decl+0x70,&buckets,8)&&buckets&&
               read(process.value,decl+0x80,&mask,4)&&mask<=4095){
                std::unordered_set<std::uintptr_t> seen;
                for(unsigned b=0;b<=mask;++b){std::uintptr_t entry=0;read(process.value,buckets+b*8,&entry,8);
                    for(unsigned depth=0;entry&&depth<128;++depth){
                        if(!seen.insert(entry).second)break;
                        std::uintptr_t name=0,next=0;unsigned type=0;int offset=-1;unsigned words[6]{};
                        if(!read(process.value,entry,&name,8)||!read(process.value,entry+8,&type,4)||
                           !read(process.value,entry+12,&offset,4)||offset<0||offset>0x100000||!read(process.value,entry+0x970,&next,8))break;
                        const auto label=readString(process.value,name);
                        if((label=="defaultAnim"||label=="midnightWeight"||label=="midnightMainAnimPtr"||label=="midnightMainAnimFrame")&&read(process.value,data+offset,words,sizeof(words))){
                            std::cout<<(first?"":",")<<"{\"name\":"<<jsonString(label)<<",\"type\":"<<type<<",\"offset\":"<<offset<<",\"words\":[";first=false;
                            for(unsigned j=0;j<6;++j)std::cout<<(j?",":"")<<words[j];std::cout<<"]}";
                        }
                        entry=next;
                    }
                }
            }
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
    if(argc==4&&!std::strcmp(argv[2],"--watch-power-scenes")){
        char* end=nullptr;const auto milliseconds=std::strtoul(argv[3],&end,10);
        if(!end||*end||milliseconds<100||milliseconds>18000)return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativePlayerStates states{};NativePlayerPowerScene scene{};
            const bool valid=readNativePlayerStates(reader,base,states)&&readNativePlayerPowerScene(reader,base,states,scene,true);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\"variant\":"<<scene.variant<<",\"handle\":"<<scene.handle<<",\"timeMs\":"<<scene.timeMs<<",\"durationMs\":"<<scene.durationMs
                <<",\"armHandle\":"<<scene.arm.handle<<",\"armSide\":"<<scene.arm.side<<",\"character\":"<<scene.arm.character<<",\"asset\":"<<jsonString(scene.asset);
            std::cout<<"}\n";Sleep(15);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
    if((argc==3&&!std::strcmp(argv[2],"--midnight"))||(argc==4&&!std::strcmp(argv[2],"--watch-midnight"))) {
        const unsigned watchMs=argc==4?static_cast<unsigned>(std::strtoul(argv[3],nullptr,10)):0;
        if(argc==4&&(watchMs<100||watchMs>32000))return 2;
        const auto started=GetTickCount64();
        do {
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
        std::cout << "{\"readOnly\":true,\"atomic\":false,\"tick\":" << GetTickCount64() << ",\"pid\":" << pid << ",\"manager\":\"0x" << std::hex << manager << "\",\"proxy\":\"0x" << proxy << "\",\"channels\":[" << std::dec << channels[0] << ',' << channels[1] << "],\"instances\":[";
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
                std::cout << "]";
                // Native 7ce7e0 constructs a bounded 0x20-byte binding
                // array at +10/+18. +8 is the resolved native actor.
                std::uintptr_t bindingItems=0;unsigned bindingFlags=0;
                if(read(process.value,instance+0x10,&bindingItems,8)&&read(process.value,instance+0x18,&bindingFlags,4)&&
                   (bindingFlags&0xffffff)<=64&&(!(bindingFlags&0xffffff)||bindingItems)){
                    std::cout<<",\"bindingsResearch\":[";
                    for(unsigned k=0;k<(bindingFlags&0xffffff);++k){
                        std::uintptr_t actor=0;unsigned actorHandle=0;
                        const bool resolved=read(process.value,bindingItems+k*0x20+8,&actor,8)&&actor&&
                            read(process.value,actor+0xa8,&actorHandle,4)&&worldIdentityEntity([&](std::uintptr_t p,void* b,std::size_t n){return read(process.value,p,b,n);},game,actorHandle)==actor;
                        std::cout<<(k?",":"")<<"{\"actor\":\"0x"<<std::hex<<actor<<std::dec<<"\",\"handle\":"<<actorHandle<<",\"reciprocal\":"<<(resolved?"true":"false");
                        const auto asset=readString(process.value,name);
                        if(resolved&&state==3&&(asset.find("midnight/gameplay/human/common/doppelganger/")==0||asset.find("midnight/d2coop/doppelganger_fx_")==0))
                            std::cout<<",\"fxResearch\":"<<midnightFxResearch(process.value,base,game,instance,bindingItems+k*0x20,actor);
                        std::cout<<'}';
                    }
                    std::cout<<']';
                }
                std::cout<<'}';
            }
        }
        std::cout << "]}\n";if(!watchMs)break;std::cout.flush();Sleep(15);
        }while(GetTickCount64()-started<watchMs);return 0;
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
    if(argc==4 && std::strcmp(argv[2],"--material-params")==0){
        const auto handle=static_cast<unsigned>(std::strtoul(argv[3],nullptr,0));std::uintptr_t game=0,vt=0;
        if(!read(process.value,base+0x39a5290,&game,8)||!game||!read(process.value,game,&vt,8)||vt!=base+0x1c940a8)return 68;
        std::cout<<materialParmResearch(process.value,base,game,handle)<<'\n';return 0;
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


