"""2117 (#265): after a progress-eotw-crafted take, check the partner's found charm (coop_grant_charm 13858082) and the
lab crafted charm (coop_lab_crafted_charm, name coop_lab_crafted) in the --bonecharms probes: fresh (step 4), before
her game closed (step 11) and after the rejoin (step 16). The lab command's result is read from the step 12 status.

Run: python -I Check-Crafted-v2117.py <take> [--twin]
PASS: neither is owned fresh; both owned and equipped before the close and after the rejoin.
The twin (progression off) must lose both after the rejoin (RED). Writes <take>/crafted-check.json.
"""
import json
import os
import sys

FOUND = 13858082
NAME = "coop_lab_crafted"


def state(path):
    d = json.load(open(path, encoding="utf-8-sig"))
    def has(rows, flags):
        return {
            "found": any(e["kind"] == FOUND and not e["name"] and (not flags or e["flags"] & 2) for e in rows),
            "crafted": any(e["name"] == NAME and (not flags or e["flags"] & 2) for e in rows),
        }
    owned, equipped = has(d["entries"], True), has(d["equipped"], False)
    return {"owned": owned, "equipped": equipped, "craftedList": d.get("crafted")}


def main():
    take = sys.argv[1]
    twin = "--twin" in sys.argv
    here = os.path.dirname(os.path.abspath(__file__))
    out = os.path.join(here, take)
    steps = {"fresh": 4, "earned": 11, "rejoined": 16}
    r = {"take": take, "twin": twin}
    try:
        s = {k: state(os.path.join(out, "step%02d-probe-bonecharms-client.json" % n)) for k, n in steps.items()}
        status = json.load(open(os.path.join(out, "step12-status-partner-progress.json-client.json"), encoding="utf-8-sig"))
    except (OSError, ValueError, KeyError) as e:
        r.update(status="ERROR", error=str(e))
    else:
        r["states"] = s
        r["lab"] = status.get("crafted", {}).get("lab")
        both = lambda x, k: x[k]["found"] and x[k]["crafted"]
        earned = not s["fresh"]["owned"]["found"] and not s["fresh"]["owned"]["crafted"] and both(s["earned"], "owned") and both(s["earned"], "equipped")
        kept = both(s["rejoined"], "owned") and both(s["rejoined"], "equipped")
        lost = not s["rejoined"]["owned"]["found"] and not s["rejoined"]["owned"]["crafted"]
        if twin:
            r["status"] = "RED" if earned and lost else "NOT_RED"
        else:
            r["status"] = "PASS" if earned and kept else "FAIL"
    if os.path.exists(os.path.join(out, "crafted-check.json")):
        print("already checked", file=sys.stderr)
        sys.exit(2)
    with open(os.path.join(out, "crafted-check.json"), "w", encoding="utf-8") as f:
        json.dump(r, f, indent=1)
    print(json.dumps(r))


if __name__ == "__main__":
    main()
