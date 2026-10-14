from pathlib import Path
R=Path(__file__).resolve().parents[2];p=R/'outputs/d2-coop-lab/src/engine_bridge.cpp'
s=p.read_text(encoding='utf-8-sig')
old='presentDominoWorld(tick);serviceDominoMarks(tick);'
assert s.count(old)==1
s=s.replace(old,old+'serviceDominoBeams(tick);')
old='writeMesmerizeWorldStream();writeDominoWorld();'
assert s.count(old)==1
s=s.replace(old,old+'writeDominoBeams();')
p.write_text(s,encoding='utf-8')
