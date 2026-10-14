"""Extend the existing choke path for exact native591 combat-lose evidence."""
from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';N=R/'work/natural-moveset'
B=W/'choke-breakout-pre-v592';B.mkdir(exist_ok=False)
def edit(name, pairs):
 p=S/name;shutil.copy2(p,B/name);s=p.read_text()
 for old,new in pairs:
  assert old in s,(name,old);s=s.replace(old,new)
 p.write_text(s)
edit('choke_presentation.h',[
 ('d[]={0,600,1400,1800};return v<=3?', 'd[]={0,600,1400,1800,1500};return v<=4?'),
 ('s.variant<=3','s.variant<=4')])
edit('native_player_choke_scene.h',[
 ('    return 0;\n}', '    // Native591: releasing input early eventually lets the guard break free.\n    // This is not the separate, still unverified cancel400 scene.\n    if(duration==1500&&asset==prefix+"combat_lose.midnightscene")return 4;\n    return 0;\n}')])
edit('shadow_action_protocol.h',[('version=63','version=64'),('p.version==63','p.version==64')])
edit('movement_bank.h',[
 ('32+351*','32+352*'),
 ('bool hasChoke()const{return version>=63;}','bool hasChoke(unsigned variant=0)const{return version>=63&&(variant<=3||version>=64);}'),
 ('347,348,351};return v<=63?','347,348,351,352};return v<=64?'),
 ('v<=63;++v','v<=64;++v')])
edit('movement_playback.h',[
 ('chokeValid&&choke.variant&&semantic.state==21','chokeValid&&choke.variant&&p.bank.hasChoke(choke.variant)&&semantic.state==21'),
 ('p.bank.hasChoke()&&p.chokeWeight>.001f&&p.chokeVariant','p.bank.hasChoke(p.chokeVariant)&&p.chokeWeight>.001f&&p.chokeVariant')])
edit('shadow_action_protocol_tests.cpp',[
 ('for(unsigned v=1;v<=3;++v){choke.', 'for(unsigned v=1;v<=4;++v){choke.'),
 ('bad.version=62;check(!validShadowAction(bad,c,h),"pre-choke wire refused")','bad.version=63;check(!validShadowAction(bad,c,h),"pre-breakout choke wire refused")'),
 ('    ChokePresentation t;', '''    check(nativeChokeVariant("midnight/gameplay/player/base/choke/choke_combat_lose.midnightscene","Npc",1500)==4&&
          !nativeChokeVariant("midnight/gameplay/player/base/choke/choke_cancel.midnightscene","Npc",400)&&
          !nativeChokeVariant("midnight/gameplay/player/base/choke/choke_combat_lose.midnightscene","Npc",400),"native combat lose distinguished from cancel");
    check(ChokePresentation::valid({0,1,4,750,1500,31,41})&&!ChokePresentation::valid({0,1,4,1501,1500,31,41}),"breakout owns exactly1500ms");
    ChokePresentation t;''')])
for old,new in [('Build-Choke-v584.ps1','Build-Choke-v592.ps1'),('Deploy-Choke-v584.ps1','Deploy-Choke-v592.ps1')]:
 s=(W/old).read_text().replace('584','592').replace('choke-banks-v581','choke-breakout-banks-v592').replace('351 clip','352 clip');(W/new).write_text(s)
s=(W/'stage_choke_v584.py').read_text().replace('candidate-choke-network-v584','candidate-choke-breakout-v592').replace("P=W/'candidate-choke-network-v581'","P=W/'candidate-choke-network-v584'")
s=s.replace('v584','v592').replace('choke-banks-v581','choke-breakout-banks-v592').replace('build_choke_bank_v581.py','build_choke_breakout_v592.py')
s=s.replace('protocol=63,bankVersion=63,clips=351','protocol=64,bankVersion=64,clips=352')
start=s.index("scope='");end=s.index("')\n",start)
s=s[:start]+"scope='Native591 combat-lose1500 binding, using existing release/front-hit/idle body data. All previous351 clip payloads unchanged. Actor presentation only: no remote victim events, attachment, health or shared outcome. Exact state/target/arm ownership preserved; native cancellation400 still unverified. Runtime validation pending.'"+s[end+1:]
s=s.replace("W/'implement_choke_v581.py'","W/'implement_choke_breakout_v592.py'")
(W/'stage_choke_v592.py').write_text(s)
print('Breakout592 source, version gates and build/stage scripts prepared; prior source backed up.')
