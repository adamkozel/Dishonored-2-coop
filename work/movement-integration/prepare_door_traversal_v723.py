from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
t=(N/'Test-DoorTraversal-v722.ps1').read_text(encoding='utf-8-sig').replace('v722','v723').replace('DoorTraversal722','DoorTraversal723')
assert t.count('-47.05 -163.0')==3
t=t.replace('-47.05 -163.0','-46.5 -163.0')
t=t.replace('locks/scripts/collision/campaign-wide doors remain unverified','collision traversal is measured here; locks/scripts/campaign-wide doors remain unverified')
(N/'Test-DoorTraversal-v723.ps1').write_text(t,encoding='utf-8')
print('723 centers receiver path inside the observed doorway; same crossing, lateral and floor assertions retained;722 preserved')
