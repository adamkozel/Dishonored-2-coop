from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,pairs):
 p=S/name;s=p.read_text()
 for a,b in pairs:
  assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s)
edit('native_player_power_scene.h',[
 ('if(bendTime&&(states.player.current!=0||!s.power.player.sword.equipped))return false; // Standing armed source proven in v39.','if(bendTime&&states.player.current>1)return false; // Armed standing and holstered crouched sources proven in v39/v40.'),
 ('if((darkVision||windblast)&&a.id==6','if((darkVision||windblast||bendTime)&&a.id==6'),
 ('!((darkVision||windblast)&&states.player.current==1)','!((darkVision||windblast||bendTime)&&states.player.current==1)'),
 ('if(!darkVision&&!windblast&&s.power.player.character==1','if(!darkVision&&!windblast&&!bendTime&&s.power.player.character==1'),
 ('if(!darkVision&&!windblast&&s.power.player.character==2','if(!darkVision&&!windblast&&!bendTime&&s.power.player.character==2')])
edit('shadow_action_protocol.h',[('version=32,size','version=33,size'),('p.version==32','p.version==33'),
 ('((p.powerVariant==10||p.powerVariant==11)&&p.swordState==1)','((p.powerVariant>=10)&&p.swordState==1)')])
edit('shadow_stream.h',[('power.variant!=10&&power.variant!=11','power.variant<10'),
 ('Verified native holstered Dark Vision / Windblast scenes.','Verified native holstered Dark Vision / Windblast / Bend Time scenes.')])
edit('movement_bank.h',[('32+238*','32+247*'),('    bool hasBendTime()','    bool hasBendTimeContexts()const{return version>=39;}\n    Joint sampleBendTime(unsigned variant,unsigned joint,float phase,float crouched,float armed)const{\n        const unsigned v=variant-12;\n        if(!hasBendTimeContexts())return sampleParameter(235+v,joint,phase);\n        const auto stand=blend(sampleParameter(238+v*3,joint,phase),sampleParameter(235+v,joint,phase),armed);\n        const auto low=blend(sampleParameter(240+v*3,joint,phase),sampleParameter(239+v*3,joint,phase),armed);\n        return blend(stand,low,crouched);\n    }\n    bool hasBendTime()'),
 ('235,238};return v<=38?','235,238,247};return v<=39?'),('v<=38;++v','v<=39;++v')])
edit('movement_bank_tests.cpp',[('argc==1?1:38);format<=38','argc==1?1:39);format<=39'),
 ('        check(bank.hasBendTime()==','        check(bank.hasBendTimeContexts()==(bank.version>=39),"Bend Time contexts require the extended bank");\n        if(bank.hasBendTimeContexts())for(unsigned variant:{12u,14u})for(float low:{0.f,1.f})for(float armed:{0.f,1.f}){\n            const unsigned v=variant-12,clip=low?(armed?239:240)+v*3:armed?235+v:238+v*3;\n            const auto actual=bank.sampleBendTime(variant,16,.5f,low,armed),expected=bank.sampleParameter(clip,16,.5f);\n            for(unsigned k=0;k<7;++k)check(std::fabs(actual[k]-expected[k])<1e-6f,"Bend Time context selects matching hand and weapon pose");\n        }\n        check(bank.hasBendTime()==')])
edit('movement_playback.h',[
 ('power.variant<=14&&p.bank.hasBendTime()&&ground&&p.presentedDrawn','power.variant<=14&&p.bank.hasBendTime()&&((p.bank.hasBendTimeContexts()&&(ground||crouched))||(ground&&p.presentedDrawn))'),
 ('p.bank.sampleParameter(235+p.powerVariant-12,j,p.powerAmount)','p.bank.sampleBendTime(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix)')])
edit('native_power_fx.h',[
 ('kind==8?"models/effects/d2coop/windblast_remote.fx":','kind==8?"models/effects/d2coop/windblast_remote.fx":\n        kind==10?"models/effects/gameplay/powers/bend_time/bend_time_01.fx":'),
 ('network&&(kind==6||kind==7)','network&&(kind==6||kind==7||kind==10)'),
 ('request==7||request==8){','request==7||request==8||request==10){')])
edit('engine_bridge.cpp',[
 ('    if(!std::strcmp(text,"coop_power_fx windblast_duplicate"))return true;','    if(!std::strcmp(text,"coop_power_fx windblast_duplicate"))return true;\n    if(!std::strcmp(text,"coop_power_fx bendtime_viewer_probe"))return true;'),
 ('    if(!std::strcmp(request.command,"coop_power_fx windblast_duplicate")){duplicateNativeWindblastFx();return;}','    if(!std::strcmp(request.command,"coop_power_fx windblast_duplicate")){duplicateNativeWindblastFx();return;}\n    if(!std::strcmp(request.command,"coop_power_fx bendtime_viewer_probe")){powerFxRequest=10;return;}')])
edit('shadow_action_protocol_tests.cpp',[
 ('bad=bend;bad.version=31;','bad=bend;bad.version=32;'),
 ('check(!validShadowAction(bad,c,h),"unobserved holstered Bend Time refused")','check(validShadowAction(bad,c,h),"native holstered Bend Time preserves equipment state")')])
edit('world_identity_tests.cpp',[
 ('                        auto bad=bend;bad.d(instance+0x18,1);','                        auto low=bend;low.d(physics+0x50,1);low.d(physics+0x54,1);low.d(active,6);low.q(mode+0x10+6*8,ground);low.q(ground,base+0x1c84258);low.d(ground+8,6);\n                        low.q(inv+0x68,0);low.d(cpnt+0x1c,0x20000002);low.q(frame+0x3c8,actor);low.q(frame+0x3c0,names+0x200);low.d(frame+0x3d0,2);\n                        require(inspectCast(low)&&observedCast.variant==12&&!observedCast.power.player.sword.equipped,"native crouched empty-hand Bend Time retains owned player/left-arm bindings");\n                        auto bad=bend;bad.d(instance+0x18,1);')])
P=R/'work/natural-moveset'
s=(P/'Test-BendTimeBody.ps1').read_text().replace("[string]$Take='bendtime-corvo-body-v40-v1')\n$Crouched=$false;$Holstered=$false;$Moving=$false","[string]$Take='bendtime-corvo-context-v41-v1',[switch]$Crouched,[switch]$Holstered,[switch]$Moving)")
assert '[switch]$Crouched'in s
s=s.replace('bendtime-banks-v40/$character-bendtime-bank.bin','bendtime-context-banks-v41/$character-bendtime-context-bank.bin').replace('version-eq38','version-eq39').replace('version-ne38','version-ne39').replace('v38 stance','v39 stance').replace('Standing armed Corvo BendTime body integration only.','Corvo Bend Time ground stance/equipment/travel context integration.')
(P/'Test-BendTimeContexts.ps1').write_text(s)
s=(P/'Test-WindblastRemoteFx.ps1').read_text().replace("[ValidateSet('windblast')][string]$Family='windblast'","[ValidateSet('bendtime')][string]$Family='bendtime'").replace('remote-windblast-fx-corvo-v1','bendtime-viewer-fx-v41-v1').replace("$first='windblast_remote_probe';$second='windblast_ground_probe';$kinds=@('8','7')","$first='bendtime_viewer_probe';$second='bendtime_viewer_probe';$kinds=@('10','10')")
s=s.replace('Both native $Family declarations','Native $Family screen declaration').replace('Native Windblast effects/lifecycle','Native Bend Time effects/lifecycle')
s=s.replace('Isolated world-space reuse of native Windblast particles and sound, plus native ground-dust feasibility. Dedicated effect declaration omits first-person fullscreen distortion/rumble. Manual finite native visual effects, without casting or gameplay outcomes. Screen distortion and visible world effects require visual review before any network integration.','Isolated manual native Bend Time viewer particle probe. Original screen-particle FX, 1600ms lease or explicit cancellation, owned clone record. No network FX replication, render-factor override, power activation or world time changes. Visual feasibility only; not complete native Bend Time appearance.')
(P/'Test-BendTimeViewerFx.ps1').write_text(s)
print('BENDTIME contexts and bounded viewer probe integrated')
