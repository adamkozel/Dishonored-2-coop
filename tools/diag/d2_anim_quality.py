"""Partner movement quality in numbers (stage 8 step 5, #243).

Reads d2_runtime_probe --anim-sample output (one JSON line per sample of the observed partner rig: ms, root, yaw and,
per foot, the world points toe, ankle, knee, hip) and measures:
- foot skate: while a toe is planted (within 3 cm of its lowest height in the take and moving vertically under
  0.05 m/s), its horizontal drift over each contact; the worst contact and the p95 are reported;
- jitter: animcheck's reversal rule scaled to metres (5 reversals of the root's travel direction within 8 samples,
  each leg at least 0.036 m);
- snaps: a root step between two samples larger than 0.6 m (11 m/s at 55 ms, beyond a sprint);
- rubber-banding: the root leaves a point by 2.1 m or more and comes back within 0.5 m of it within 600 ms.

Run: python -I tools/diag/d2_anim_quality.py <samples.jsonl> [--out result.json]
Exit 0 when every limit holds, 1 when one fails, 2 when there are too few samples.
Limits are proposals until a clean baseline calibrates them (partner-quality-plan.md).
"""
import argparse
import json
import math
import os
import sys

MUT = os.environ.get("COOP_MUT", "")

LIMITS = {
    "footSkateP95": 0.05,      # metres per contact
    "footSkateMax": 0.12,
    "jitterEpisodes": 0,
    "snaps": 0,
    "rubberBands": 0,
}
PLANTED_HEIGHT = 0.03
PLANTED_VZ = 0.05
JITTER_AMP = 0.036
SNAP_STEP = 0.6
RUBBER_OUT = 2.1
RUBBER_BACK = 0.5
RUBBER_MS = 600


def load(path):
    rows = []
    with open(path, encoding="utf-8-sig") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                r = json.loads(line)
            except ValueError:
                continue
            if "root" in r and "ms" in r:
                rows.append(r)
    return rows


def dist2(a, b):
    return math.hypot(a[0] - b[0], a[1] - b[1])


def percentile(values, q):
    if not values:
        return 0.0
    s = sorted(values)
    k = min(len(s) - 1, max(0, int(math.ceil(q * len(s))) - 1))
    return s[k]


def foot_skate(rows):
    contacts = []
    for f in range(2):
        pts = []
        for r in rows:
            feet = r.get("feet") or []
            if len(feet) > f and feet[f].get("points"):
                pts.append((r["ms"], feet[f]["points"][0]["at"]))
        if len(pts) < 3:
            continue
        floor = min(p[1][2] for p in pts)
        drift, run = 0.0, 0
        for i in range(1, len(pts)):
            (t0, a), (t1, b) = pts[i - 1], pts[i]
            dt = max((t1 - t0) / 1000.0, 1e-3)
            vz = abs(b[2] - a[2]) / dt
            planted = (b[2] - floor) <= PLANTED_HEIGHT and vz < PLANTED_VZ
            if MUT == "skate_ignores_planted":
                planted = False
            if planted:
                drift += dist2(a, b)
                run += 1
            elif run:
                contacts.append(drift)
                drift, run = 0.0, 0
        if run:
            contacts.append(drift)
    return contacts


def jitter(rows):
    episodes, legs = 0, []
    for i in range(1, len(rows)):
        a, b = rows[i - 1]["root"], rows[i]["root"]
        legs.append((b[0] - a[0], b[1] - a[1]))
    i = 0
    while i + 8 <= len(legs):
        window = legs[i:i + 8]
        reversals = 0
        for k in range(1, len(window)):
            (x0, y0), (x1, y1) = window[k - 1], window[k]
            if x0 * x1 + y0 * y1 < 0 and math.hypot(x0, y0) >= JITTER_AMP and math.hypot(x1, y1) >= JITTER_AMP:
                reversals += 1
        if reversals >= 5 and MUT != "no_jitter":
            episodes += 1
            i += 8
        else:
            i += 1
    return episodes


def snaps(rows):
    limit = SNAP_STEP * (100 if MUT == "loose_snap" else 1)
    return sum(1 for i in range(1, len(rows)) if dist2(rows[i - 1]["root"], rows[i]["root"]) > limit)


def rubber_bands(rows):
    count, i = 0, 0
    while i < len(rows):
        start = rows[i]
        out_at = None
        for j in range(i + 1, len(rows)):
            if rows[j]["ms"] - start["ms"] > RUBBER_MS:
                break
            d = dist2(start["root"], rows[j]["root"])
            if out_at is None and d >= RUBBER_OUT:
                out_at = j
            elif out_at is not None and d <= RUBBER_BACK and MUT != "no_rubber":
                count += 1
                i = j
                break
        i += 1
    return count


def analyse(rows):
    if len(rows) < 10:
        return {"status": "SKIP", "reason": "fewer than 10 samples", "samples": len(rows)}
    contacts = foot_skate(rows)
    duration = (rows[-1]["ms"] - rows[0]["ms"]) / 1000.0
    travel = sum(dist2(rows[i - 1]["root"], rows[i]["root"]) for i in range(1, len(rows)))
    m = {
        "samples": len(rows), "seconds": round(duration, 3), "hz": round(len(rows) / duration, 2) if duration else 0,
        "travel": round(travel, 3), "contacts": len(contacts),
        "footSkateP95": round(percentile(contacts, 0.95), 4), "footSkateMax": round(max(contacts) if contacts else 0.0, 4),
        "jitterEpisodes": jitter(rows), "snaps": snaps(rows), "rubberBands": rubber_bands(rows),
    }
    failures = [k for k, v in LIMITS.items() if m[k] > v]
    m["limits"] = LIMITS
    m["failures"] = failures
    m["status"] = "PASS" if not failures else "FAIL"
    return m


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("samples")
    ap.add_argument("--out")
    a = ap.parse_args()
    r = analyse(load(a.samples))
    if a.out:
        with open(a.out, "w", encoding="utf-8") as f:
            json.dump(r, f, indent=1)
    print(json.dumps(r))
    sys.exit({"PASS": 0, "FAIL": 1}.get(r["status"], 2))


if __name__ == "__main__":
    main()
