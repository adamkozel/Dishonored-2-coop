from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; S=R/'outputs/d2-coop-lab/src'
B=W/'shadowwalk-body-pre-v524'; B.mkdir(exist_ok=False)
def edit(name, pairs):
    p=S/name; shutil.copy2(p,B/name); s=p.read_text(encoding='utf-8-sig')
    for a,b in pairs:
        assert s.count(a)==1,(name,a,s.count(a))
        s=s.replace(a,b)
    p.write_text(s,encoding='utf-8')
edit('power_presentation.h',[
 ('400,466,1200,666};return v<=31?d[v]:0;', '400,466,1200,666,1000,1000,1133};return v<=34?d[v]:0;'),
 ('||v==30;', '||v==30||v==33;'), ('s.variant<=31', 's.variant<=34')])
edit('shadow_action_protocol.h',[
 ('version=54,','version=55,'),('p.version==54','p.version==55'),
 ('if(variant==28)return swordState==1;', 'if(variant==28||(variant>=32&&variant<=34))return swordState==1;'),
 ('||(p.powerVariant>=29&&p.powerVariant<=31))))):', '||(p.powerVariant>=29&&p.powerVariant<=34))))):')])
edit('shadow_stream.h',[
 ('#include "native_player_doppelganger_scene.h"', '#include "native_player_doppelganger_scene.h"\n#include "native_player_shadowwalk_scene.h"'),
 ('readNativePlayerPossessionScene(memoryRead,base,states,power,true));', 'readNativePlayerPossessionScene(memoryRead,base,states,power,true)||readNativePlayerShadowWalkScene(memoryRead,base,states,power));')])
edit('movement_bank.h',[
 ('maximumBytes=32+331*', 'maximumBytes=32+335*'),
 ('bool hasDoppelganger()const', '''bool hasShadowWalk()const{return version>=58;}
    Joint sampleShadowWalk(unsigned variant,unsigned joint,float amount,float travelPhase,float moving)const{
        if(variant==32)return sampleParameter(331,joint,amount);
        if(variant==34)return sampleParameter(334,joint,amount);
        return blend(sample(332,joint,amount),sample(333,joint,travelPhase),moving);
    }
    float shadowWalkDistancePhase(float phase,float dx,float dy,float forwardX,float forwardY)const{
        const float distance=std::sqrt(dx*dx+dy*dy);
        // One complete authored cycle covers the recorded contact distance.
        // Backward travel reverses this first forward-only study. Dedicated
        // sideways/backward clips and their quality remain separate work.
        phase+=(dx*forwardX+dy*forwardY<0?-distance:distance)/clips[333].stride;
        return phase-std::floor(phase);
    }
    bool hasDoppelganger()const'''),
 ('322,325,331};return v<=57?', '322,325,331,335};return v<=58?'),('v<=57;++v','v<=58;++v'),
 ('terrain?(f[0]<.2f||f[0]>6.f):idle?', '(terrain||c==333)?(f[0]<.2f||f[0]>6.f):idle?')])
edit('movement_playback.h',[
 ('unsigned powerVariant=0;', 'float shadowWalkPhase=0;\n    unsigned powerVariant=0;'),
 ('const bool powerActive=powerValid&&power.variant&&(doppelgangerActive', '''// Alternate native mode has no default Running/Crouch motion flag.
        // Its exact owned power reader supplies presentation eligibility;
        // replicated root placement and gameplay remain independent.
        const bool shadowWalkActive=power.variant>=32&&power.variant<=34&&p.bank.hasShadowWalk()&&equipmentValid&&equipment.sword==1;
        const bool powerActive=powerValid&&power.variant&&(shadowWalkActive||doppelgangerActive'''),
 ('p.powerVariant=power.variant;p.powerAmount=powerAmount;', '''if(power.variant==32&&p.powerVariant!=32)p.shadowWalkPhase=0;
                p.powerVariant=power.variant;p.powerAmount=power.variant==33?std::fmod(float(pose.time*.001)/p.bank.clips[332].angle,1.f):powerAmount;'''),
 ('const unsigned stance=transportPower?0u:', 'const unsigned stance=(transportPower||shadowWalkActive)?0u:'),
 ('if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible,stance,', '''const float shadowDx=position[0]-p.clock.position[0],shadowDy=position[1]-p.clock.position[1];
        const bool shadowStep=powerActive&&power.variant==33&&eligible&&p.clock.positioned&&!pose.teleport;
        if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible,stance,'''),
 ('        equipmentRequest=eligible&&equipmentValid;', '''        else if(shadowStep)p.shadowWalkPhase=p.bank.shadowWalkDistancePhase(p.shadowWalkPhase,shadowDx,shadowDy,pose.orientation[0],pose.orientation[1]);
        equipmentRequest=eligible&&equipmentValid;'''),
 ('||(p.powerVariant>=29&&p.powerVariant<=31&&p.bank.hasDoppelganger()))){', '||(p.powerVariant>=29&&p.powerVariant<=31&&p.bank.hasDoppelganger())||(p.powerVariant>=32&&p.powerVariant<=34&&p.bank.hasShadowWalk()))){'),
 ('auto power=p.powerVariant>=29?', 'auto power=p.powerVariant>=32?p.bank.sampleShadowWalk(p.powerVariant,j,p.powerAmount,p.shadowWalkPhase,p.powerMoveMix):p.powerVariant>=29?'),
 ('||(p.powerVariant>=16&&p.bank.hasMesmerize()))', '||(p.powerVariant>=16&&p.powerVariant<=31&&p.bank.hasMesmerize()))'),
 ('\"powerEvent\\\":%llu,', '\"powerEvent\\\":%llu,\\\"shadowWalkPhase\\\":%.6f,'),
 ('p.powerWeight,p.powerEvent,p.combatAirMix', 'p.powerWeight,p.powerEvent,p.shadowWalkPhase,p.combatAirMix')])
edit('movement_bank_tests.cpp',[
 ('format=(argc==1?1:57);format<=57;', 'format=(argc==1?1:58);format<=58;'),
 ('if(c==226||c==228)put', 'if(c==226||c==228||c==333)put'),
 ('check(bank.hasDoppelganger()==', '''check(bank.hasShadowWalk()==(bank.version>=58),"Shadow Walk requires complete body bank58");
        if(bank.hasShadowWalk()){
            const float stride=bank.clips[333].stride;
            check(std::fabs(bank.shadowWalkDistancePhase(.2f,0,0,1,0)-.2f)<1e-6f,"stationary shadow crawl cannot advance");
            check(std::fabs(bank.shadowWalkDistancePhase(.2f,stride*.25f,0,1,0)-.45f)<1e-6f,"crawl cadence follows measured quarter-stride");
            check(std::fabs(bank.shadowWalkDistancePhase(.2f,-stride*.25f,0,1,0)-.95f)<1e-6f,"backward crawl reverses and wraps");
            for(unsigned v:{32u,33u,34u})for(float t:{0.f,.5f,1.f})for(unsigned j=0;j<joints;++j){
                auto pose=bank.sampleShadowWalk(v,j,t,.4f,.7f);for(float x:pose)check(std::isfinite(x),"whole shadow body stays finite");
                if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"shadow body cannot move network root");
            }
            auto bad=bytes;put(bad,32+333*(24+size_t(MovementBank::frames)*joints*28)+16,0.f);MovementBank refused;
            check(!refused.decode(bad,layout,joints),"zero shadow stride rejected before cadence division");
        }
        check(bank.hasDoppelganger()==''')])
edit('shadow_action_protocol_tests.cpp',[
 ('      for(unsigned v=29;v<=31;++v){', '''      for(unsigned v=32;v<=34;++v){
        auto shadow=emily;shadow.powerVariant=v;shadow.powerDurationMs=PowerPresentation::duration(v);shadow.swordState=1;
        check(validShadowAction(shadow,ec,eh)&&shadowPowerSwordContext(v,1),"owned Emily shadow form phases accepted hands-free");
        auto bad=shadow;bad.swordState=2;check(!validShadowAction(bad,ec,eh),"shadow form cannot retain drawn sword");
        bad=shadow;bad.character=1;bad.role=0;check(!validShadowAction(bad,c,h),"Corvo cannot claim Emily shadow form");
        bad=shadow;bad.airStage=2;check(!validShadowAction(bad,ec,eh),"unverified shadow air metadata refused");
        bad=shadow;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"exact native shadow transition duration required");
        bad=shadow;bad.version=54;check(!validShadowAction(bad,ec,eh),"old peer cannot claim unsupported shadow body contract");
      }
      {PowerPresentation t;PowerPresentation::Sample s{};float a=0;
       check(t.push({100,1,32,200,1000})&&t.push({150,1,32,250,1000})&&t.sample(125,s,a)&&std::fabs(a-.225f)<1e-6f,"entry follows native clock");
       check(!t.push({180,1,33,0,1000})&&t.push({180,2,33,0,1000}),"sustained form requires new semantic event");
       check(t.push({240,3,34,600,1133})&&t.sample(240,s,a)&&std::fabs(a-600.f/1133)<1e-6f,"exit follows native clock");
       check(!t.sample(441,s,a),"stale shadow form presentation retires");}
      for(unsigned v=29;v<=31;++v){''')])
s=(W/'Build-ShadowWalkInventory-v515.ps1').read_text(encoding='utf-8-sig')
s=s.replace('shadowwalk-inventory','shadowwalk-body').replace('v515','v524').replace('Build515','Build524').replace('actual331','actual335').replace('doppelganger-holstered-banks-v495','shadowwalk-banks-v523').replace('-doppelganger-holstered-bank.bin','-shadowwalk-bank.bin')
(W/'Build-ShadowWalkBody-v524.ps1').write_text(s,encoding='utf-8')
print('Prepared shadow body524; prior sources retained, no live deployment')
