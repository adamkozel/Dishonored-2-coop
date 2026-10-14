from pathlib import Path
p=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src/engine_bridge.cpp'
s=p.read_text();a='serviceAgilityFixture(tick);serviceNativePowerFx(tick);'
assert s.count(a)==1;s=s.replace(a,'serviceAgilityFixture(tick);serviceCarryAttachmentFixture(tick);serviceNativePowerFx(tick);');p.write_text(s)
