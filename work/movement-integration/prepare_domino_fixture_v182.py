"""Finite native inventory fixture; actual Domino use is controller-driven."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
s=(S/'mesmerize_fixture.h').read_text(encoding='utf-8-sig').replace('Mesmerize','Domino').replace('mesmerize','domino')
(S/'domino_fixture.h').write_text(s,encoding='utf-8')
f=S/'engine_bridge.cpp';s=f.read_text(encoding='utf-8-sig')
for a,b in [
 ('bool movementMesmerizeFixtureBusy();','bool movementMesmerizeFixtureBusy();\nbool movementDominoFixtureBusy();'),
 ('#include "mesmerize_fixture.h"','#include "mesmerize_fixture.h"\n#include "domino_fixture.h"'),
 ('writeWindblastFixture(tick);writeMesmerizeFixture(tick);','writeWindblastFixture(tick);writeMesmerizeFixture(tick);writeDominoFixture(tick);'),
 ('servicePotionFixture(tick);serviceMesmerizeFixture(tick);','servicePotionFixture(tick);serviceMesmerizeFixture(tick);serviceDominoFixture(tick);')]:
 assert s.count(a)==1,a;s=s.replace(a,b)
a='    if(!std::strcmp(text,"coop_mesmerize_fixture 20000")||!std::strcmp(text,"coop_mesmerize_fixture 40000")||!std::strcmp(text,"coop_mesmerize_fixture off"))return true;'
assert s.count(a)==1;s=s.replace(a,a+'\n'+a.replace('mesmerize','domino'))
for ending in ['20000")){mesmerizeFixtureRequest=1;return;}','40000")){mesmerizeFixtureRequest=3;return;}','off")){mesmerizeFixtureRequest=2;return;}']:
 a='    if(!std::strcmp(request.command,"coop_mesmerize_fixture '+ending
 assert s.count(a)==1,a;s=s.replace(a,a+'\n'+a.replace('mesmerize','domino'))
f.write_text(s,encoding='utf-8')
f=S/'potion_fixture.h';s=f.read_text(encoding='utf-8-sig');a='return potionFixture.until!=0||movementMesmerizeFixtureBusy();';assert s.count(a)==1
f.write_text(s.replace(a,'return potionFixture.until!=0||movementMesmerizeFixtureBusy()||movementDominoFixtureBusy();'),encoding='utf-8')
