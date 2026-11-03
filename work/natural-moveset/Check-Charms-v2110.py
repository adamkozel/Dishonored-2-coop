"""2110 (#265): after a progress-eotw-charm take, check the partner's bonecharm granted by the route (coop_grant_charm)
in the --bonecharms probes: fresh (step 4), before her game closed (step 10) and after the rejoin (step 15).

Run: python -I Check-Charms-v2110.py <take> [--twin] [--charm 13858082]
PASS: the charm is not owned fresh, owned and equipped before the close, and owned and equipped after the rejoin.
The twin (progression off) must lose it after the rejoin (RED). Writes <take>/charm-check.json.
"""
import json
import os
import sys


def state(path, charm):
    d = json.load(open(path, encoding="utf-8-sig"))
    owned = any(e["kind"] == charm and e["flags"] & 2 for e in d["entries"])
    equipped = any(k["kind"] == charm for k in d["equipped"])
    return {"owned": owned, "equipped": equipped, "ownedCount": d["owned"]}


def main():
    take = sys.argv[1]
    twin = "--twin" in sys.argv
    charm = int(sys.argv[sys.argv.index("--charm") + 1]) if "--charm" in sys.argv else 13858082
    if charm >= 2 ** 31:
        charm -= 2 ** 32
    here = os.path.dirname(os.path.abspath(__file__))
    out = os.path.join(here, take)
    steps = {"fresh": 4, "earned": 10, "rejoined": 15}
    r = {"take": take, "twin": twin, "charm": charm}
    try:
        s = {k: state(os.path.join(out, "step%02d-probe-bonecharms-client.json" % n), charm) for k, n in steps.items()}
    except (OSError, ValueError, KeyError) as e:
        r.update(status="ERROR", error=str(e))
    else:
        r["states"] = s
        earned = not s["fresh"]["owned"] and s["earned"]["owned"] and s["earned"]["equipped"]
        kept = s["rejoined"]["owned"] and s["rejoined"]["equipped"]
        if twin:
            r["status"] = "RED" if earned and not s["rejoined"]["owned"] else "NOT_RED"
        else:
            r["status"] = "PASS" if earned and kept else "FAIL"
    if os.path.exists(os.path.join(out, "charm-check.json")):
        print("already checked", file=sys.stderr)
        sys.exit(2)
    with open(os.path.join(out, "charm-check.json"), "w", encoding="utf-8") as f:
        json.dump(r, f, indent=1)
    print(json.dumps(r))


if __name__ == "__main__":
    main()
