from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,pairs):
 f=S/name;s=f.read_text(encoding='utf-8-sig')
 for a,b in pairs:
  assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 f.write_text(s,encoding='utf-8')
edit('movement_bank.h',[
 ('32+285*','32+289*'),
 ('    bool hasMesmerize()const','    bool hasDomino()const{return version>=47;}\n    Joint sampleDomino(unsigned variant,unsigned joint,float phase)const{return sampleParameter(285+variant-19,joint,phase);}\n    bool hasMesmerize()const'),
 ('276,285};return v<=46?','276,285,289};return v<=47?'),
 ('for(unsigned v=1;v<=46;++v)','for(unsigned v=1;v<=47;++v)')])
edit('power_presentation.h',[
 ('466,1200,666};return v<=18?','466,1200,666,266,1333,400,266};return v<=22?'),
 ('v==13||v==17;','v==13||v==17||v==20;'),
 ('s.variant<=18&&','s.variant<=22&&')])
edit('native_player_states.h',[
 ('    if(nativePlayerStunStates(s,base)){motion=0;return true;}', '''    // Native v187/v191: dedicated Domino72 overlays Running0. Keep source
    // root motion valid; exact equipment and owned scene authorize gestures.
    if(s.player.logic==1&&s.defaultMode&&s.player.current==0&&s.active.size()==2){
        unsigned ground=0,domino=0;for(const auto& a:s.active){
            ground+=a.id==0&&a.vtable==base+0x1c85628;domino+=a.id==72&&a.vtable==base+0x1c3eeb8;
        }if(ground==1&&domino==1){motion=0;return true;}
    }
    if(nativePlayerStunStates(s,base)){motion=0;return true;}''')])
edit('shadow_stream.h',[
 ('#include "native_player_power_scene.h"','#include "native_player_power_scene.h"\n#include "native_player_domino_scene.h"'),
 ('nativeStateValid&&!stunValid&&readNativePlayerPowerScene(memoryRead,base,states,power,true);','nativeStateValid&&!stunValid&&(readNativePlayerPowerScene(memoryRead,base,states,power,true)||readNativePlayerDominoScene(memoryRead,base,states,power));')])
edit('shadow_action_protocol.h',[
 ('version=43,','version=44,'),('p.version==43&&','p.version==44&&'),
 ('((p.powerVariant>=4&&p.powerVariant<=9)||(p.powerVariant>=16&&p.powerVariant<=18))','((p.powerVariant>=4&&p.powerVariant<=9)||(p.powerVariant>=16&&p.powerVariant<=22))')])
edit('movement_playback.h',[
 ('const bool powerActive=powerValid&&power.variant&&((','const bool powerActive=powerValid&&power.variant&&((power.variant>=19&&power.variant<=22&&p.bank.hasDomino()&&ground&&p.presentedDrawn)||('),
 ('||(p.powerVariant>=16&&p.powerVariant<=18&&p.bank.hasMesmerize()))){','||(p.powerVariant>=16&&p.powerVariant<=18&&p.bank.hasMesmerize())||(p.powerVariant>=19&&p.powerVariant<=22&&p.bank.hasDomino()))){'),
 ('auto power=p.powerVariant>=16?p.bank.sampleMesmerize','auto power=p.powerVariant>=19?p.bank.sampleDomino(p.powerVariant,j,p.powerAmount):p.powerVariant>=16?p.bank.sampleMesmerize')])
edit('movement_bank_tests.cpp',[
 ('for(unsigned format=(argc==1?1:46);format<=46;++format)','for(unsigned format=(argc==1?1:47);format<=47;++format)'),
 ('        check(bank.hasMesmerize()==','        check(bank.hasDomino()==(bank.version>=47),"Domino appended clips cannot read older banks");\n        if(bank.hasDomino())for(unsigned v=19;v<=22;++v)for(unsigned j:{2u,9u,16u,20u,24u})for(float t:{0.f,.5f,1.f})for(float x:bank.sampleDomino(v,j,t))check(std::isfinite(x),"Domino whole-body samples finite");\n        check(bank.hasMesmerize()==')])
print('Domino body44 / bank47 integration source prepared')
