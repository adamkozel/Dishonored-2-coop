"""2158 (#265): after a progress-eotw-perchar take, check that the partner's progression is kept per character.

The route (progress-eotw-perchar-v2158) gives Emily extended_health and charm C1, rejoins as Corvo, gives Corvo
tranquilizer bolts and charm C2, rejoins as Emily, then as Corvo again. The probes read at steps 3 (Emily fresh),
9 (Emily earned), 14 (Corvo after the switch), 20 (Corvo earned), 25 (Emily again) and 29 (Corvo again).

Run: python -I Check-PerChar-v2158.py <take> [--twin]
PASS: Corvo after the switch has neither Emily's extended_health nor C1; Emily again has her extended_health, her bolts
and C1 (owned and equipped) and not C2; Corvo again has his bolts and C2 (owned and equipped) and not C1.
The twin (progression off) must lose both sets (RED). Writes <take>/perchar-check.json.
"""
import json
import os
import sys

E = "components/characters/player/base/attributes/extended_health.inventoryitem"
T = "components/equipment/ammo/bolt_tranquilizer.inventoryitem"
C1 = 13858082
C2 = 3997576305 - 2 ** 32
STEPS = {"emilyFresh": 3, "emilyEarned": 9, "corvoSwitch": 14, "corvoEarned": 20, "emilyAgain": 25, "corvoAgain": 29}


def read(out, step):
    inv = json.load(open(os.path.join(out, "step%02d-probe-player-inventory-client.json" % step), encoding="utf-8-sig"))
    ch = json.load(open(os.path.join(out, "step%02d-probe-bonecharms-client.json" % (step + 1)), encoding="utf-8-sig"))
    q = {it["name"]: it["quantity"] for it in inv["items"] if it["name"] in (E, T)}

    def charm(c):
        return {"owned": any(e["kind"] == c and e["flags"] & 2 for e in ch["entries"]),
                "equipped": any(k["kind"] == c for k in ch["equipped"])}
    return {"character": inv.get("character"), "health": q.get(E, 0), "bolts": q.get(T, 0), "c1": charm(C1), "c2": charm(C2)}


def main():
    take = sys.argv[1]
    twin = "--twin" in sys.argv
    out = os.path.join(os.path.dirname(os.path.abspath(__file__)), take)
    r = {"take": take, "twin": twin}
    try:
        s = {k: read(out, n) for k, n in STEPS.items()}
    except (OSError, ValueError, KeyError) as e:
        r.update(status="ERROR", error=str(e))
    else:
        r["states"] = s
        held = lambda c: c["owned"] and c["equipped"]
        checks = {
            # The probe's character is a number (Emily 2 in progress-eotw-v2117a); Corvo's steps must read another one.
            "characters": all(s[k]["character"] == s["emilyFresh"]["character"] for k in ("emilyEarned", "emilyAgain"))
            and all(s[k]["character"] == s["corvoSwitch"]["character"] != s["emilyFresh"]["character"] for k in ("corvoEarned", "corvoAgain")),
            "emilyEarned": s["emilyEarned"]["health"] != s["emilyFresh"]["health"] and held(s["emilyEarned"]["c1"]),
            "corvoStartsOwn": s["corvoSwitch"]["health"] == s["emilyFresh"]["health"] and not s["corvoSwitch"]["c1"]["owned"],
            "corvoEarned": s["corvoEarned"]["bolts"] != s["corvoSwitch"]["bolts"] and held(s["corvoEarned"]["c2"]),
        }
        kept = {
            "emilyKept": s["emilyAgain"]["health"] == s["emilyEarned"]["health"] and s["emilyAgain"]["bolts"] == s["emilyEarned"]["bolts"]
            and held(s["emilyAgain"]["c1"]) and not s["emilyAgain"]["c2"]["owned"],
            "corvoKept": s["corvoAgain"]["bolts"] == s["corvoEarned"]["bolts"] and s["corvoAgain"]["health"] == s["corvoEarned"]["health"]
            and held(s["corvoAgain"]["c2"]) and not s["corvoAgain"]["c1"]["owned"],
        }
        lost = {
            "emilyLost": s["emilyAgain"]["health"] == s["emilyFresh"]["health"] and not s["emilyAgain"]["c1"]["owned"],
            "corvoLost": not s["corvoAgain"]["c2"]["owned"],
        }
        r["checks"], r["kept"], r["lost"] = checks, kept, lost
        if twin:
            r["status"] = "RED" if all(checks.values()) and all(lost.values()) else "NOT_RED"
        else:
            r["status"] = "PASS" if all(checks.values()) and all(kept.values()) else "FAIL"
    if os.path.exists(os.path.join(out, "perchar-check.json")):
        print("already checked", file=sys.stderr)
        sys.exit(2)
    with open(os.path.join(out, "perchar-check.json"), "w", encoding="utf-8") as f:
        json.dump(r, f, indent=1)
    print(json.dumps({k: r[k] for k in r if k != "states"}))


if __name__ == "__main__":
    main()
