from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'Record-CarryStanceRegression-v28.ps1').read_text()
t=t.replace("C $peer 'setviewpos -35 -162 68.1 180 0'", "C $peer 'setviewpos -36.7 -161.8 68.1 173 0'")
t=t.replace('-Role $peer -Pitch 12','-Role $peer -Pitch 20')
(P/'Record-CarryCloseView-v28.ps1').write_text(t)
print('CLOSE_VIEW_STANCE_HARNESS_READY')
