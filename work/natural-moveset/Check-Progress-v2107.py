"""2107 (#265): after a progress-eotw take, compare the partner's carried entries before her game closed with those after
it was relaunched and joined again, against the fresh mission. Check-Progress-v2096 asked for earned values above the
fresh ones, but coop_grant_named sets an ammunition entry to 1 (progress-eotw-v2104a/b: tranquilizer bolts 3 -> 1), and
since progress-v2105 a join restores ammunition exactly, so the rule is now: changed from fresh, and equal after the rejoin.

Run: python -I Check-Progress-v2107.py <take> [--twin] [--steps 3,7,11]
PASS: each granted entry (extended_health, bolt_tranquilizer) differs from its fresh value before the close and has the
same value after the rejoin. The twin (nothing saved) must lose both. Writes <take>/progress-check.json.
"""
import json
import os
import sys

WATCH = ["components/characters/player/base/attributes/extended_health.inventoryitem",
         "components/equipment/ammo/bolt_tranquilizer.inventoryitem"]


def quantities(path):
    d = json.load(open(path, encoding="utf-8-sig"))
    return {it["name"]: it["quantity"] for it in d["items"] if it["name"] in WATCH}, d.get("character")


def main():
    take = sys.argv[1]
    twin = "--twin" in sys.argv
    steps = [3, 7, 11]
    if "--steps" in sys.argv:
        steps = [int(x) for x in sys.argv[sys.argv.index("--steps") + 1].split(",")]
    here = os.path.dirname(os.path.abspath(__file__))
    out = os.path.join(here, take)
    names = {k: "step%02d-probe-player-inventory-client.json" % s for k, s in zip(("fresh", "earned", "rejoined"), steps)}
    r = {"take": take, "twin": twin, "steps": steps}
    try:
        q = {k: quantities(os.path.join(out, v)) for k, v in names.items()}
    except (OSError, ValueError, KeyError) as e:
        r.update(status="ERROR", error=str(e))
    else:
        rows, kept = [], 0
        for n in WATCH:
            fresh, earned, rejoined = (q[k][0].get(n, 0) for k in ("fresh", "earned", "rejoined"))
            changed = earned != fresh
            ok_kept = rejoined == earned
            kept += changed and ok_kept
            rows.append({"name": n, "fresh": fresh, "earned": earned, "rejoined": rejoined, "changed": changed, "kept": ok_kept})
        r["characters"] = {k: q[k][1] for k in q}
        r["rows"] = rows
        if twin:
            r["status"] = "RED" if all(x["changed"] and not x["kept"] for x in rows) else "NOT_RED"
        else:
            r["status"] = "PASS" if kept == len(WATCH) else "FAIL"
    if os.path.exists(os.path.join(out, "progress-check.json")):
        print("already checked", file=sys.stderr)
        sys.exit(2)
    with open(os.path.join(out, "progress-check.json"), "w", encoding="utf-8") as f:
        json.dump(r, f, indent=1)
    print(json.dumps(r))


if __name__ == "__main__":
    main()
