"""Partner-presentation gap metrics from a take's observer movement samples (*-observer-movement.ndjson).

For each file: how often the receiving partner's body dropped to its native pose (movement clock weight
below 0.001 while the presentation is active), the stale-state (mask 16) and invalid-semantic (mask 1)
eligibility records, the largest source state age, and root-step rejections. Used by audit 1241 (1213m)."""
import json, sys
from pathlib import Path

def metrics(path):
    rows = []
    for line in Path(path).read_text(encoding='utf-8-sig').splitlines():
        try: rows.append(json.loads(line))
        except ValueError: pass
    rows = [r for r in rows if isinstance(r.get('status'), dict)]
    if not rows: return None
    active = [r for r in rows if r['status'].get('active')]
    dropped = [r for r in active if r['status'].get('weight', 0) < .001]
    # A drop is a transition from presented (weight >= 0.5) to native (< 0.001) between consecutive samples.
    drops = sum(1 for a, b in zip(active, active[1:]) if a['status'].get('weight', 0) >= .5 and b['status'].get('weight', 0) < .001)
    eligibility = rows[-1]['status'].get('eligibility') or {}
    records = eligibility.get('records') or []
    stale = [x for x in records if x.get('mask', 0) & 16]
    invalid = [x for x in records if x.get('mask', 0) & 1]
    first = rows[0]['status'].get('clockRejection') or {}; last = rows[-1]['status'].get('clockRejection') or {}
    return dict(file=str(path).replace('\\', '/'), seconds=round((rows[-1]['ms'] - rows[0]['ms']) / 1000, 2), samples=len(rows), activeSamples=len(active),
                nativePoseSamples=len(dropped), nativePoseFraction=round(len(dropped) / len(active), 3) if active else None, presentedToNativeDrops=drops,
                eligibilityFailures=eligibility.get('failures'), staleStateRecords=len(stale), invalidSemanticRecords=len(invalid),
                maxStaleStateAgeMs=max((x.get('stateAge', 0) for x in stale), default=0),
                rootStepRejections=(last.get('count') or 0) - (first.get('count') or 0))

if __name__ == '__main__':
    for p in sys.argv[1:]: print(json.dumps(metrics(p)))
