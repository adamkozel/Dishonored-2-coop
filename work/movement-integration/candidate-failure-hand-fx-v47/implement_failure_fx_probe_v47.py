"""Finite manual native hand-effect experiment; no network activation yet."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,pairs):
 p=S/name;s=p.read_text()
 for a,b in pairs:assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s)
edit('native_power_fx.h',[
 ('std::uint64_t epoch=0,streamToken=0,peerToken=0;ULONGLONG expires=0;', 'std::uint64_t epoch=0,streamToken=0,peerToken=0;ULONGLONG expires=0;std::array<float,3> handPoint{};'),
 ('const char* powerFxResult="idle";', 'std::uint64_t powerFxHandUpdates=0;\nconst char* powerFxResult="idle";'),
 ('kind==10?"models/effects/gameplay/powers/bend_time/bend_time_01.fx":', 'kind==10?"models/effects/gameplay/powers/bend_time/bend_time_01.fx":\n        kind==11?"models/effects/gameplay/player_generic/tatoo/out_of_mana/out_of_mana_01.fx":'),
 ('bool startNativePowerFx(unsigned kind,', '''bool nativeFailureFxHand(float* point,float* axis){
    NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s)||!s.stable||s.logic!=1||
       s.game!=observedGame||s.actor!=probeActor||!bodyShadowTarget(s)||s.parents.size()!=(localRole?215u:219u)||
       s.parents[9]!=8||s.parents[10]!=9||s.globalPose.size()!=s.parents.size())return false;
    const auto& hand=s.globalPose[9];
    for(unsigned r=0;r<3;++r){point[r]=s.root[r];for(unsigned k=0;k<3;++k)point[r]+=s.root[3+k*3+r]*hand[12+k];
        for(unsigned c=0;c<3;++c){axis[c*3+r]=0;for(unsigned k=0;k<3;++k)axis[c*3+r]+=s.root[3+k*3+r]*hand[c*4+k];}}
    for(unsigned i=0;i<3;++i)if(!std::isfinite(point[i])||std::fabs(point[i])>100000)return false;
    for(unsigned i=0;i<9;++i)if(!std::isfinite(axis[i])||std::fabs(axis[i])>1.001f)return false;
    return true;
}
bool startNativePowerFx(unsigned kind,'''),
 ('if(network&&(kind==6||kind==7||kind==10))','if(network&&(kind==6||kind==7||kind==10||kind==11))'),
 ('if(kind==8&&!origin)point[2]+=1.2f;', 'float handAxis[9]{};if(kind==11){if(!nativeFailureFxHand(point,handAxis))return reject("native_failure_hand_unavailable");axis=handAxis;}\n    if(kind==8&&!origin)point[2]+=1.2f;'),
 ('e.expires=tick+(kind==9?2500:1600);e.record=', 'for(unsigned k=0;k<3;++k)e.handPoint[k]=point[k];\n    e.expires=tick+(kind==9?2500:1600);e.record='),
 ('++powerFxStopped;e={};powerFxResult="owned_effect_stopped";\n        }', '''++powerFxStopped;e={};powerFxResult="owned_effect_stopped";
        }else if(e.kind==11){
            float point[3]{},axis[9]{};const auto record=nativePowerFxRecord(e);std::uintptr_t callback=1;
            if(e.actor!=probeActor||e.owner!=probeHandle||!nativeFailureFxHand(point,axis)||!record||
               !memoryRead(record+0x128,&callback,8)||callback){
                if(record)reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e.id);
                ++powerFxStopped;e={};powerFxResult="failure_hand_owner_retired";
            }else{
                // Exact native start writes world origin+38 and axis+44.
                // Only our current owned record is updated; no FP callbacks.
                std::memcpy(reinterpret_cast<void*>(record+0x38),point,12);std::memcpy(reinterpret_cast<void*>(record+0x44),axis,36);
                for(unsigned k=0;k<3;++k)e.handPoint[k]=point[k];++powerFxHandUpdates;
            }
        }'''),
 ('request==8||request==10){','request==8||request==10||request==11){'),
 ('fprintf(f,"{\\"result\\":', 'fprintf(f,"{\\"handUpdates\\":%llu,\\"result\\":'),
 ('powerFxResult,active,powerFxStarted,','powerFxHandUpdates,powerFxResult,active,powerFxStarted,'),
 ('\\"declaration\\":\\"0x%llx\\"}",first?', '\\"declaration\\":\\"0x%llx\\",\\"handPoint\\":[%.6f,%.6f,%.6f]}",first?'),
 ('(unsigned long long)e.declaration);first=false;', '(unsigned long long)e.declaration,e.handPoint[0],e.handPoint[1],e.handPoint[2]);first=false;')])
edit('engine_bridge.cpp',[
 ('if(!std::strcmp(text,"coop_power_fx bendtime_viewer_probe"))return true;', 'if(!std::strcmp(text,"coop_power_fx bendtime_viewer_probe"))return true;\n    if(!std::strcmp(text,"coop_power_fx outofmana_hand_probe"))return true;'),
 ('if(!std::strcmp(request.command,"coop_power_fx bendtime_viewer_probe")){powerFxRequest=10;return;}', 'if(!std::strcmp(request.command,"coop_power_fx bendtime_viewer_probe")){powerFxRequest=10;return;}\n    if(!std::strcmp(request.command,"coop_power_fx outofmana_hand_probe")){powerFxRequest=11;return;}')])
print('MANUAL_FAILURE_FX_V47')
