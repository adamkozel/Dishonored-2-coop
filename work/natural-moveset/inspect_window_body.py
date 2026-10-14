"""Inspect native FP body timing/root/legs without anchoring full-body wrists."""
import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import *
from edge_data import poses
sk=read(P/'fp-window-body-skeleton.json');rig=NativeRig(sk['joints']);report=[]
for m in read(P/'fp-window-body-manifest.json')['rows']:
 data=poses(P/m['sample_file']);frames=data['frames'];phases=[]
 for u in [0,.1,.25,.4,.5,.65,.75,.9,1]:
  raw=frames[round(u*(len(frames)-1))];g=rig.fk([qts(x)for x in raw]);phases.append(dict(phase=u,joints={j['nativeName']:[round(v,4)for v in g[i].translation]for i,j in enumerate(sk['joints'])}))
 report.append(dict(alias=m['aliases'][0],duration=(len(frames)-1)/data['fps'],joints=len(sk['joints']),phases=phases,asset_sha256=m['asset_sha256']))
(P/'pair-research/fp-window-body-phases.json').write_text(json.dumps(report,indent=2))
for r in report:
 print(r['alias'],r['duration'],r['joints']);print(json.dumps(r['phases'][::2],indent=1))
