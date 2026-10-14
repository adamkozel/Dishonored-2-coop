#pragma once
#include "native_swarm_trail_attachment.h"
struct SwarmTrailSource {
    struct Entry {NativeWindblastFxRecord fx{};NativeSwarmTrailAttachment binding{};double born=0;bool finished=false;};
    std::array<Entry,7> entries{};unsigned count=0,handle=0,scans=0;
    std::uint64_t token=0,peerToken=0;bool baseline=false,armed=false;
    double born=0,nextScan=0;
} swarmTrailSource;
std::uint64_t swarmTrailScans=0,swarmTrailReadFailures=0,swarmTrailSourceSamples=0,swarmTrailDiscovered=0;
double swarmTrailReadMaxMs=0;
void sampleSwarmTrails(const NativeSwarmWorld& world,CoopSwarmWorldPacket& packet){
    auto& s=swarmTrailSource;const double now=presentationClock();
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    const auto handle=world.present?world.handle:0;
    if(!s.baseline){s.baseline=true;s.handle=handle;return;} // Never replay an already-existing swarm on join.
    if(handle!=s.handle){s.handle=handle;s.entries={};s.count=0;s.born=now;s.nextScan=now;s.scans=0;s.armed=handle!=0;}
    if(!s.armed||!world.present||now<s.born||now-s.born>2500)return;
    const double began=presentationClock();const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(now>=s.nextScan&&now-s.born<=750&&s.scans<16&&s.count<s.entries.size()){
        s.nextScan=now+50;++s.scans;++swarmTrailScans;std::vector<NativeWindblastFxRecord> records;
        if(readNativeFilteredFxRecords(memoryRead,base,world.equipment.player.sword.game,records,[](const std::string& name){return name=="models/effects/gameplay/powers/rat_swarm/rat/rat_spawn_trail_01.fx";})){
            for(const auto& fx:records){
                if(s.count>=s.entries.size())break;
                if(std::any_of(s.entries.begin(),s.entries.begin()+s.count,[&](const auto& e){return e.fx.id==fx.id;}))continue;
                NativeSwarmTrailAttachment bind{};if(!readNativeSwarmTrailAttachment(memoryRead,base,world,fx,bind))continue;
                // One native initial spawn trail per rat incarnation; do not invent more on a reused slot.
                if(std::any_of(s.entries.begin(),s.entries.begin()+s.count,[&](const auto& e){return e.binding.index==bind.index;}))continue;
                s.entries[s.count++]={fx,bind,now,false};++swarmTrailDiscovered;
            }
        }else ++swarmTrailReadFailures;
    }
    for(unsigned i=0;i<s.count;++i){auto& e=s.entries[i];if(e.finished)continue;
        NativeSwarmTrailAttachment binding{};std::array<std::uintptr_t,2> identity{},again{};
        std::array<float,12> pose{};const auto age=now-e.born;
        if(age<0||age>2500||!memoryRead(e.fx.record,identity.data(),16)||unsigned(identity[0])!=e.fx.owner||unsigned(identity[0]>>32)!=e.fx.id||identity[1]!=e.fx.declaration||
           !readNativeSwarmTrailAttachment(memoryRead,base,world,e.fx,binding)||binding.index!=e.binding.index||binding.incarnation!=e.binding.incarnation||binding.parentHandle!=e.binding.parentHandle||
           !memoryRead(e.fx.record+0x38,pose.data(),48)||!validSwarmTransform(pose)||!memoryRead(e.fx.record,again.data(),16)||identity!=again){e.finished=true;++swarmTrailReadFailures;continue;}
        for(unsigned j=0;j<packet.count;++j){auto& rat=packet.agents[j];if(rat.index!=binding.index||rat.handle!=binding.parentHandle||rat.incarnation!=binding.incarnation)continue;
            float distance=0;for(unsigned k=0;k<3;++k){const float d=pose[k]-rat.transform[k];distance+=d*d;}
            if(distance>4){e.finished=true;++swarmTrailReadFailures;break;}
            rat.trailId=e.fx.id;rat.trailAgeMs=static_cast<unsigned>(age);rat.trailTransform=pose;++swarmTrailSourceSamples;break;
        }
    }
    swarmTrailReadMaxMs=(std::max)(swarmTrailReadMaxMs,presentationClock()-began);
}
