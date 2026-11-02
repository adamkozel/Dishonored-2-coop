# First mission inventory (step 5b, #264)

*A Long Day in Dunwall* (`campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo`) was the main test map through development, and in real co-op play it broke far more often than other missions (user, 2026-11-02). This note collects every known problem, then the measured ones, one row each. Fixes follow host-decides first, each with a mutation twin.

## 1. Known from earlier work

| # | Problem | Source | State |
|---|---|---|---|
| K1 | The route's loot of Ramsey's body did not register | `hoststory-lead-dunwall-v2017d` to `v2023a`, `continue-decision-vm-v2047g` (ch. 122) | Fixed for the partner's ring in hoststory-v2024 (ch. 119: ActionSpecialLoot runs in both worlds). Recheck in the measure takes |
| K2 | The end-of-mission talk to Meagan did not register | `route-dunwall-lead-v1753e`, `v1755a`, `hoststory-lead-dunwall-v2024a`, `v2026b` | Passed from `hoststory-lead-dunwall-v2027a` on. Recheck |
| K3 | The tower safe-room doors (`env_interiors_safe_door`, `_6`, `_9`) open on the host and stay closed on the partner | ch. 119 (still open after hoststory-v2027) | Open |
| K4 | The partner's game had to answer the native choice screen in the first mission | ch. 126 | Fixed in nativechoice-v2088 (the co-op Character box asks; the native screen is answered) |
| K5 | Co-op saves switch stalls in `profile_saving` | #263 | Open, seen in some lab takes |
| K6 | The partner's presentation is not live in the scripted opening | ch. 95 (`1829a`) | Known; recheck what the partner sees at the start |
| K7 | Ramsey and Alexi alive in a partner's fresh world after a mid-mission join | ch. 106 | Fixed by the join snapshot (#240, ch. 123). Recheck |
| K8 | The user's and partner's own list | asked 2026-11-02 | Not received yet |

## 2. Measured (Run-FirstMission-v2095.sh)

The full first-mission route (casts, deaths, the bedroom, Mayhew, Ramsey and his loot, the ring locker, the safe room, the balcony, the streets and Meagan) runs on two Steam accounts in all four pairings, on candidate notice-v2092. A world diff is recorded after each stage: start, deaths, mayhew, ramsey-loot, ring-locker, saferoom, balcony, streets, end.

| Take | Pairing (host / partner) | Result |
|---|---|---|
| `firstmission-dunwall-v2095a` | Corvo / Emily | queued |
| `firstmission-dunwall-v2095b` | Emily / Corvo | queued |
| `firstmission-dunwall-v2095c` | Corvo / Corvo | queued |
| `firstmission-dunwall-v2095d` | Emily / Emily | queued |

## 3. Done when

The whole first mission plays through cleanly in all four pairings on two accounts, every diff equal, twice in a row on one candidate, and the user's own list is resolved or written down as known.
