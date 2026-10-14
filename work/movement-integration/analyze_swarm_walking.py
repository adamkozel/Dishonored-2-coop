"""Require actual displacement and gait activity, independently of input values."""
from pathlib import Path
import argparse,json,math
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
a=argparse.ArgumentParser();a.add_argument('take');args=a.parse_args();t=P/args.take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
rows=read(t/'states.json'); result=read(t/'result.json'); phase=[x for x in rows if 900<=x['ms']<=6200]
distance=sum(math.dist(a['player']['position'][:2],b['player']['position'][:2]) for a,b in zip(phase,phase[1:]))
movingCast=[x for x in phase if x['movement']['powerWeight']>.5 and x['movement']['speed']>.1]
advance=sum((b['movement']['phase']-a['movement']['phase'])%1 for a,b in zip(phase,phase[1:]) if a['movement']['speed']>.1 and b['movement']['speed']>.1)
stop=[x for x in rows if 5300<x['ms']<6200]
motion=dict(take=args.take,requestedAxis=result.get('move'),distance=distance,maximumRemoteSpeed=max(x['movement']['speed'] for x in phase),
    activeWalkingCastSamples=len(movingCast),gaitCyclesAdvancedWhileMoving=advance,
    stoppedPhaseRange=max(x['movement']['phase'] for x in stop)-min(x['movement']['phase'] for x in stop) if stop else None,
    walkingCoveragePassed=bool(result['passed'] and distance>.2 and len(movingCast)>=4 and advance>.1),
    scope='Observed source path length plus remote gait speed/phase during native casting. Numerical eligibility does not establish foot sliding or visual acceptance.')
(t/'walking-evidence.json').write_text(json.dumps(motion,indent=2),encoding='utf-8');print(json.dumps(motion))
