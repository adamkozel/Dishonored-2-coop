"""Apply bounded phase-only Bend Time support to the v39 source baseline."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,pairs):
 p=S/name;s=p.read_text()
 for a,b in pairs:
  assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s)
edit('native_player_power_scene.h',[
 ('#include "native_player_windblast_item.h"','#include "native_player_windblast_item.h"\n#include "native_player_bendtime_item.h"'),
 ('    if(character==1&&sub=="Scene_1"&&duration==333){','    if(character==1&&sub=="Scene_1"){\n        if(asset==prefix+"power_bendtime_cast_in_test.midnightscene"&&duration==533)return 12;\n        if(asset==prefix+"power_bendtime_cast_loop_test.midnightscene"&&duration==300)return 13;\n        if(asset==prefix+"power_bendtime_cast_out_test.midnightscene"&&duration==433)return 14;\n    }\n    if(character==1&&sub=="Scene_1"&&duration==333){'),
 ('NativeDarkVisionItem dark{};NativeWindblastItem wind{};','NativeDarkVisionItem dark{};NativeWindblastItem wind{};NativeBendTimeItem bend{};'),
 ('    if(darkVision)s.power=dark;','    const bool bendTime=!darkVision&&!windblast&&readNativeBendTimeItem(read,base,bend,true);\n    if(bendTime)s.power=bend;\n    else if(darkVision)s.power=dark;'),
 ('    bool blinking=false,grappling=false;','    if(bendTime&&(states.player.current!=0||!s.power.player.sword.equipped))return false; // Standing armed source proven in v39.\n    bool blinking=false,grappling=false;'),
 ('if(!variant||(variant==10)!=darkVision||(variant==11)!=windblast)continue;','if(!variant||(variant==10)!=darkVision||(variant==11)!=windblast||(variant>=12)!=bendTime)continue;'),
 ('const unsigned expectedBindings=windblast?2:','const unsigned expectedBindings=bendTime?(s.variant==12?2:1):windblast?2:'),
 ('playerCount!=(windblast||expectedBindings==3?1u:0u)','playerCount!=(windblast||(bendTime&&s.variant==12)||expectedBindings==3?1u:0u)'),
 ('    if(darkVision){NativeDarkVisionItem end{};','    if(bendTime){NativeBendTimeItem end{};if(!readNativeBendTimeItem(read,base,end,true))return false;finalPower=end;}\n    else if(darkVision){NativeDarkVisionItem end{};')])
edit('power_presentation.h',[
 ('0,333,333,333,200,200,133,266,100,366,933,1000};return v<=11?d[v]:0;','0,333,333,333,200,200,133,266,100,366,933,1000,533,300,433};return v<=14?d[v]:0;'),
 ('return v==2||v==5||v==8;','return v==2||v==5||v==8||v==13;'),('s.variant<=11','s.variant<=14')])
edit('shadow_action_protocol.h',[
 ('version=31,size','version=32,size'),('p.version==31','p.version==32'),
 ('p.powerVariant<=3||p.powerVariant==11','p.powerVariant<=3||p.powerVariant>=11')])
edit('movement_bank.h',[
 ('32+235*','32+238*'),
 ('    bool hasCarryFetalRelease()','    bool hasBendTime()const{return version>=38;}\n    bool hasCarryFetalRelease()'),
 ('233,234,235};return v<=37?','233,234,235,238};return v<=38?'),('v<=37;++v','v<=38;++v')])
edit('movement_bank_tests.cpp',[
 ('argc==1?1:37);format<=37','argc==1?1:38);format<=38'),
 ('        check(bank.hasCarryFetalRelease()','        check(bank.hasBendTime()==(bank.version>=38),"Bend Time phases require their own bank version");\n        if(bank.hasBendTime())for(unsigned clip=235;clip<=237;++clip){\n            const auto a=bank.sampleParameter(clip,9,0),b=bank.sampleParameter(clip,9,1);\n            check(std::isfinite(a[0])&&std::isfinite(b[0]),"Bend Time samples full entry/hold/recovery without overrun");\n        }\n        check(bank.hasCarryFetalRelease()')])
edit('movement_playback.h',[
 ('||(ground&&p.presentedDrawn))));','||(ground&&p.presentedDrawn)))||\n            (power.variant>=12&&power.variant<=14&&p.bank.hasBendTime()&&ground&&p.presentedDrawn));'),
 ('||(p.powerVariant==11&&p.bank.hasWindblast()))){','||(p.powerVariant==11&&p.bank.hasWindblast())||(p.powerVariant>=12&&p.powerVariant<=14&&p.bank.hasBendTime()))){'),
 ('auto power=p.powerVariant==10?','auto power=p.powerVariant>=12?p.bank.sampleParameter(235+p.powerVariant-12,j,p.powerAmount):p.powerVariant==10?'),
 ('||(p.powerVariant==11&&p.bank.hasWindblastContexts()))&&MovementBank','||(p.powerVariant==11&&p.bank.hasWindblastContexts())||(p.powerVariant>=12&&p.bank.hasBendTime()))&&MovementBank')])
print('BENDTIME phase-only source and bank support integrated')
