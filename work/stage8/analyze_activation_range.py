"""Stage 8.1 (#234): analyse an activation-range take (Test-ActivationRange-v2001.ps1).

Reads activity-<phase>-<role>.jsonl (d2_runtime_probe --npc-activity samples) and writes analysis.json:
per phase and world, the AI NPCs by entity LOD, how many are paused (pause refs > 0) or out of the scheduler,
the decl auto-pause levels seen, and for NPCs near the partner (within NEAR m of the client's player) the
same NPC in both worlds: LOD, paused share and path length over the phase.

Usage: python -I analyze_activation_range.py <take-folder-name>
"""
import json
import math
import os
import sys
from collections import Counter, defaultdict

NEAR = 40.0


def load(path):
    rows = []
    if not os.path.exists(path):
        return rows
    with open(path, encoding="utf-8-sig") as f:
        for line in f:
            line = line.strip()
            if line:
                rows.append(json.loads(line))
    return rows


def dist(a, b):
    return math.sqrt(sum((x - y) ** 2 for x, y in zip(a, b)))


def ai_entries(sample):
    return [e for e in sample["entries"] if e.get("authority") and e.get("health", 0) > 0 and "position" in e]


def key(e):
    return e["name"] + "|" + e["definition"]


def unique_keys(samples):
    counts = Counter()
    for s in samples[:1]:
        counts.update(key(e) for e in ai_entries(s))
    return {k for k, n in counts.items() if n == 1}


def summarise_world(samples):
    lods = Counter()
    paused = scheduled = total = 0
    auto = Counter()
    for s in samples:
        for e in ai_entries(s):
            total += 1
            lods[e["lod"]] += 1
            d = e["ai"][0]
            paused += d["pauseRefs"] > 0
            scheduled += bool(d["scheduler"])
            auto[d["autoPauseLod"]] += 1
    return {
        "npcSamples": total,
        "lod": dict(sorted(lods.items())),
        "pausedShare": round(paused / total, 3) if total else None,
        "schedulerShare": round(scheduled / total, 3) if total else None,
        "autoPauseLevels": dict(sorted(auto.items())),
    }


def tracks(samples, keys):
    t = defaultdict(list)
    for s in samples:
        for e in ai_entries(s):
            if key(e) in keys:
                t[key(e)].append(e)
    return t


def path_length(track):
    return sum(dist(a["position"], b["position"]) for a, b in zip(track, track[1:]))


def near_partner(phase, host, client):
    partner = [s["sample"].get("player") for s in client if s["sample"].get("player")]
    hview = [s["sample"].get("view") for s in host if s["sample"].get("view")]
    if not partner or not hview:
        return None
    p0, v0 = partner[len(partner) // 2], hview[len(hview) // 2]
    hs, cs = [s["sample"] for s in host], [s["sample"] for s in client]
    keys = unique_keys(hs) & unique_keys(cs)
    th, tc = tracks(hs, keys), tracks(cs, keys)
    rows = []
    for k in sorted(keys):
        if k not in th or k not in tc:
            continue
        first = tc[k][0]["position"]
        if dist(first, p0) > NEAR:
            continue
        h, c = th[k], tc[k]
        rows.append({
            "npc": k.split("|")[0],
            "toPartner": round(dist(first, p0), 1),
            "toHostView": round(dist(h[0]["position"], v0), 1),
            "hostLod": dict(Counter(e["lod"] for e in h)),
            "clientLod": dict(Counter(e["lod"] for e in c)),
            "hostPausedShare": round(sum(e["ai"][0]["pauseRefs"] > 0 for e in h) / len(h), 2),
            "clientPausedShare": round(sum(e["ai"][0]["pauseRefs"] > 0 for e in c) / len(c), 2),
            "hostScheduledShare": round(sum(bool(e["ai"][0]["scheduler"]) for e in h) / len(h), 2),
            "hostPath": round(path_length(h), 2),
            "clientPath": round(path_length(c), 2),
            "endGap": round(dist(h[-1]["position"], c[-1]["position"]), 2),
        })
    return {"phase": phase, "partner": p0, "hostView": v0, "near": NEAR, "npcs": rows}


def main(take):
    folder = take if os.path.isabs(take) else os.path.join(os.path.dirname(os.path.abspath(__file__)), take)
    out = {"take": os.path.basename(folder), "phases": {}}
    for phase in ("spawn", "far", "far-noauth"):
        host = load(os.path.join(folder, "activity-%s-host.jsonl" % phase))
        client = load(os.path.join(folder, "activity-%s-client.jsonl" % phase))
        if not host or not client:
            out["phases"][phase] = {"status": "SKIP", "reason": "no samples"}
            continue
        out["phases"][phase] = {
            "samples": {"host": len(host), "client": len(client)},
            "host": summarise_world([s["sample"] for s in host]),
            "client": summarise_world([s["sample"] for s in client]),
            "nearPartner": near_partner(phase, host, client),
        }
    near = (out["phases"].get("far") or {}).get("nearPartner") or {}
    rows = near.get("npcs") or []
    if not rows:
        out["decision"] = {"status": "SKIP", "reason": "no NPC near the far partner was matched in both worlds"}
    else:
        paused = [r for r in rows if r["hostPausedShare"] >= 0.5]
        out["decision"] = {
            "status": "MEASURED",
            "npcsNearPartner": len(rows),
            "pausedInHostWorld": len(paused),
            "hostCanRunFarGuardsAsIs": not paused,
            "note": "Paused = the host world's decision AI holds a pause ref for at least half the far phase.",
        }
    with open(os.path.join(folder, "analysis.json"), "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1)
    print(json.dumps(out["decision"]))
    for phase, p in out["phases"].items():
        if "host" in p:
            print(phase, "host", p["host"]["lod"], "paused", p["host"]["pausedShare"], "| client", p["client"]["lod"], "paused", p["client"]["pausedShare"])


if __name__ == "__main__":
    main(sys.argv[1])
