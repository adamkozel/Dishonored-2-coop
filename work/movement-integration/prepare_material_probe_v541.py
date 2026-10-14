from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration'
p=R/'outputs/d2-coop-lab/src/runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
a='asset.find("midnight/d2coop/doppelganger_fx_")==0))'
b='asset.find("midnight/d2coop/doppelganger_fx_")==0||asset.find("midnight/gameplay/player/base/powers/power_shadow_walk_")==0))'
assert s.count(a)==1;s=s.replace(a,b);p.write_text(s,encoding='utf-8')
p=W/'Build-MaterialProbe-v539.ps1';s=p.read_text().replace('539','541');(W/'Build-MaterialProbe-v541.ps1').write_text(s,encoding='utf-8')
print('Probe541 observes native Shadow Walk scene bindings; original539 was limited to clone scenes.')
