"""Stage 8 difference detector (#235): compare the shared world of the host's and the partner's games.

Input: `d2_runtime_probe PID --world-digest` snapshots, one JSON object per line, from each game:
    {"map": ..., "categories": {"<category>": {"<stable key>": <value>, ...}, ...}}
A stable-world gate comes first (coop-anything): a key counts only when its value is the same in every
snapshot of that game (pass three snapshots taken a few seconds apart). Unstable keys are reported, never compared.
Per category the tool builds coop-anything's 256-bucket digest (third_party/coop-anything/flight/cofr.py),
compares the digests, and names every differing key with both values.

Categories listed in PER_PLAYER are expected to differ (items are per player since the user's
2026-10-29 decision). They are reported but never fail the comparison.

Usage:
    python -I tools/diag/d2_world_diff.py HOST.jsonl CLIENT.jsonl [--out result.json] [--ignore CATEGORY ...]
Exit code: 0 = no differences, 1 = differences, 2 = SKIP (nothing comparable: different maps or no stable keys).
"""
import argparse
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "outputs", "d2-coop-lab", "third_party", "coop-anything", "flight"))
import cofr  # noqa: E402  (vendored, MIT)

PER_PLAYER = {"pickups"}
# Entities that exist only for one character (Corvo or Emily), such as ld_dun_audiograph_01_corvo: with different
# characters in the two games they are expected to differ.
# Mutation twins (coop-anything): COOP_MUT names a deliberately broken variant that test_d2_world_diff.py must catch.
MUT = os.environ.get("COOP_MUT", "")
PER_CHARACTER = re.compile(r"(^|_)(corvo|emily)(_|$)", re.I)


def load_snapshots(path):
    snaps = []
    with open(path, encoding="utf-8-sig") as f:
        for line in f:
            line = line.strip()
            if line:
                snaps.append(json.loads(line))
    if not snaps:
        raise SystemExit("no snapshots in %s" % path)
    return snaps


# Entities created at runtime get a generated number (outsider_rune_12363 in one game, outsider_rune_12364 in the
# other, take world-digest-v2002a). Keys ending in _<4+ digits> are grouped by their base name into "<base>#gen",
# whose value is the sorted list of the group's values, so the same objects compare equal whatever their numbers.
GENERATED = re.compile(r"^(.*)_\d{4,}$")


def normalise(snapshot):
    if MUT == "no_generated_grouping":
        return snapshot
    cats = {}
    for cat, entries in snapshot.get("categories", {}).items():
        out, groups = {}, {}
        for k, v in entries.items():
            m = GENERATED.match(k)
            if m:
                groups.setdefault(m.group(1) + "#gen", []).append(canonical(v))
            else:
                out[k] = v
        for k, vs in groups.items():
            out[k] = sorted(vs)
        cats[cat] = out
    return dict(snapshot, categories=cats)


def canonical(value):
    return json.dumps(value, sort_keys=True, separators=(",", ":"))


def stable_view(snaps):
    """Per category: {key: canonical value} for keys equal in every snapshot, plus the unstable keys."""
    stable, unstable = {}, {}
    categories = set()
    for s in snaps:
        categories.update(s.get("categories", {}))
    for cat in sorted(categories):
        values = [s.get("categories", {}).get(cat, {}) for s in snaps]
        keys = set().union(*values)
        stable[cat], unstable[cat] = {}, []
        for k in keys:
            seen = {canonical(v[k]) if k in v else None for v in values}
            if MUT == "no_stability_gate":
                seen = {canonical(values[-1][k]) if k in values[-1] else None}
            if len(seen) == 1:
                only = seen.pop()
                if only is not None:
                    stable[cat][k] = only
            else:
                unstable[cat].append(k)
    return stable, unstable


def key_hash(key):
    return cofr.hash64(key.encode("utf-8"))


def digest(entries):
    pairs = [(key_hash(k), cofr.hash64(v.encode("utf-8")) & 0xFFFFFFFF) for k, v in entries.items()]
    return cofr.bucketed_digest(pairs)


def compare(host_snaps, client_snaps, ignore=()):
    hmap = {s.get("map") for s in host_snaps}
    cmap = {s.get("map") for s in client_snaps}
    result = {"hostMap": sorted(m for m in hmap if m), "clientMap": sorted(m for m in cmap if m), "categories": {}}
    if len(hmap) != 1 or hmap != cmap:
        result["status"] = "SKIP"
        result["reason"] = "the two games are not on one map in every snapshot"
        return result
    hs, hu = stable_view([normalise(x) for x in host_snaps])
    cs, cu = stable_view([normalise(x) for x in client_snaps])
    compared = differing = 0
    for cat in sorted(set(hs) | set(cs)):
        if cat in ignore:
            continue
        h, c = hs.get(cat, {}), cs.get(cat, {})
        unstable = set(hu.get(cat, [])) | set(cu.get(cat, []))
        h = {k: v for k, v in h.items() if k not in unstable}
        c = {k: v for k, v in c.items() if k not in unstable}
        hd, cd = digest(h), digest(c)
        diffs = []
        for k in sorted(set(h) | set(c)):
            if (k in h) != (k in c) if MUT == "presence_only" else h.get(k) != c.get(k):
                diffs.append({"key": k, "perCharacter": bool(PER_CHARACTER.search(k)),
                              "host": json.loads(h[k]) if k in h else "<absent>",
                              "client": json.loads(c[k]) if k in c else "<absent>"})
        per_player = cat in PER_PLAYER
        result["categories"][cat] = {
            "perPlayer": per_player,
            "hostKeys": len(h), "clientKeys": len(c), "unstable": len(unstable),
            "hostDigest": "%016x" % hd[1], "clientDigest": "%016x" % cd[1],
            "differingBuckets": sum(a != b for a, b in zip(hd[0], cd[0])),
            "equal": hd[1] == cd[1] and not diffs,
            "differences": diffs,
        }
        compared += min(len(h), len(c))
        if not per_player:
            differing += sum(not d["perCharacter"] for d in diffs)
    result["comparedKeys"] = compared
    result["differences"] = differing
    if compared == 0 and MUT != "zero_compared_passes":
        result["status"] = "SKIP"
        result["reason"] = "no stable keys were compared"
    else:
        result["status"] = "EQUAL" if differing == 0 else "DIFFERENT"
    return result


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("host")
    ap.add_argument("client")
    ap.add_argument("--out")
    ap.add_argument("--ignore", nargs="*", default=[])
    a = ap.parse_args()
    r = compare(load_snapshots(a.host), load_snapshots(a.client), set(a.ignore))
    if a.out:
        with open(a.out, "w", encoding="utf-8") as f:
            json.dump(r, f, indent=1)
    print(r["status"], "compared", r.get("comparedKeys", 0), "differences", r.get("differences", 0))
    for cat, c in r["categories"].items():
        if c["differences"]:
            tag = " (per player, expected)" if c["perPlayer"] else ""
            print("  %s%s: %d" % (cat, tag, len(c["differences"])))
            for d in c["differences"][:10]:
                note = " (per character, expected)" if d["perCharacter"] else ""
                print("    %s host=%s client=%s%s" % (d["key"], d["host"], d["client"], note))
    sys.exit({"EQUAL": 0, "DIFFERENT": 1}.get(r["status"], 2))


if __name__ == "__main__":
    main()
