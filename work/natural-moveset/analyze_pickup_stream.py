"""Check a stream armed before pickup, not one started after it."""
import hashlib,json,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
for take in sys.argv[1:]:
    p=P/take;rows=read(p/'observations.json');identity=read(p/'identity.json')
    result=read(p/'result.json')
    prep=[r for r in rows if any(a.get('carryResearch',{}).get('phase110')==1 for a in (r.get('native')or{}).get('active',[]))]
    def variant(r):return (r.get('native')or{}).get('carryScene',{}).get('variant',0)
    lift=[r for r in rows if variant(r)==1];held=[r for r in rows if variant(r)==2]
    streams=[r.get('observerStream')or{} for r in rows]
    source=[r.get('sourceStream')or{} for r in rows]
    received={s.get('carryRemote',0) for s in streams}
    checks=dict(harness_finished=result['failure']is None,
        preparation_observed=bool(prep),native_lift_observed=bool(lift),native_loop_observed=bool(held),
        receiver_continuously_ready=all(s.get('armed') and s.get('ready') for s in streams),
        sender_continuously_ready=all(s.get('armed') and s.get('ready') for s in source) if all(source) else None,
        no_receiver_restart=len({s.get('token')for s in streams})==1 and len({s.get('started')for s in streams})==1,
        received_pickup_and_loop={1,2}<=received,
        mapped_authored_target=all(s.get('carryLocalHandle') and s.get('carrySpawner') and s.get('carryDefinition') for s in streams if s.get('carryRemote')) and bool(received-{0}),
        native_health_preserved=read(p/'health-after.json')['current']==identity['healthBefore']['current'])
    transitions=[];previous=None
    for r in rows:
        s=r.get('observerStream')or{}
        current=(s.get('ready'),s.get('result'),s.get('remoteState'),s.get('carryRemote'))
        if current!=previous:
            transitions.append(dict(ms=r['beginMs'],ready=current[0],result=current[1],state=current[2],carry=current[3]))
            previous=current
    out=dict(identity=identity,checks=checks,passed=all(v is True for v in checks.values()),transitions=transitions,
        sample_count=len(rows),video_sha256=hashlib.sha256((p/'native.mp4').read_bytes()).hexdigest().upper(),
        scope='Semantic continuity through observed native pickup. First-person source video; no complete third-person pickup claim.',complete=False)
    (p/'stream-validation.json').write_text(json.dumps(out,indent=2))
    print(json.dumps(dict(take=take,checks=checks,transitions=transitions)))
