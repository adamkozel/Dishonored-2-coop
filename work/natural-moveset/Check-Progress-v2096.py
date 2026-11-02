"""2096 (#265): after a progress-eotw take, compare the partner's carried entries before her game closed (step 7) with
those after it was relaunched and joined again (step 11), against the fresh mission (step 3).

Run: python -I Check-Progress-v2096.py <take> [--twin]
PASS: each granted entry (extended_health, bolt_tranquilizer) is above its fresh value before the close and is kept
after the rejoin. The twin (nothing saved) must lose them. Writes <take>/progress-check.json.
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
    here = os.path.dirname(os.path.abspath(__file__))
    out = os.path.join(here, take)
    names = {"fresh": "step03-probe-player-inventory-client.json", "earned": "step07-probe-player-inventory-client.json",
             "rejoined": "step11-probe-player-inventory-client.json"}
    r = {"take": take, "twin": twin}
    try:
        q = {k: quantities(os.path.join(out, v)) for k, v in names.items()}
    except (OSError, ValueError, KeyError) as e:
        r.update(status="ERROR", error=str(e))
    else:
        rows, kept = [], 0
        for n in WATCH:
            fresh, earned, rejoined = (q[k][0].get(n, 0) for k in ("fresh", "earned", "rejoined"))
            ok_earned = earned > fresh
            ok_kept = rejoined >= earned
            kept += ok_earned and ok_kept
            rows.append({"name": n, "fresh": fresh, "earned": earned, "rejoined": rejoined, "earnedAbove": ok_earned, "kept": ok_kept})
        r["characters"] = {k: q[k][1] for k in q}
        r["rows"] = rows
        if twin:
            r["status"] = "RED" if all(x["earnedAbove"] and not x["kept"] for x in rows) else "NOT_RED"
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
