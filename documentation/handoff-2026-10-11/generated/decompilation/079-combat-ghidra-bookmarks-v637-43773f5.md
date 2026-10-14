# combat-ghidra-bookmarks-v637.json

Original: [work/combat-ghidra-bookmarks-v637.json](<../../../../work/combat-ghidra-bookmarks-v637.json>)

SHA-256: `5179EFBB309179749036D39CC85EED83CCF5DCEAA89634711E561276FEC8C066`; 4892 bytes.

## Original saved record

````json
[
  {
    "address": "141c85d28",
    "category": "D2Coop targeted sword scene ownership",
    "comment": "Exact B671F824 runtime607 both roles: standing0 Running0+Slash39; sword_slash_right/rightb, Scene_2 Corvo / Scene_1 Emily,1000ms, player+owned right arm+reciprocal guard. Old two-binding cap dropped these actions. Candidate610 fixes read-only optional target validation and excludes partner; unchanged protocol65/bank65/353. Corvo611 and Emily614 pass10 checks each (receipt615): owned scene, native sword, Steam remote body/root/recovery, receiver victim HP unchanged, full checkpoint/14file restoration. Emily body visible; Corvo611 view occluded. Retained failed Emily611 instead observed crouched throwhit/kick800 with right arm+player+player (not target-bearing). Emily612 one source sample failed minimum2; faster614 sampling removed unused sensor reads, retained assertions. Corvo614 could not stand near obstruction; no action and no visual proof. No victim events/damage replay or shared NPC outcomes. Native close-range/crouched613 observations pending.",
    "type": "Note"
  },
  {
    "address": "141c85d28",
    "category": "D2Coop native kick scene",
    "comment": "ExactB671F824 Emily611 native throwhit/kick800: Slash39+Sneaking6 physical1; owned rightarm+player+sameplayer. Void624 PlayerRightArm/PlayerCamera/PlayerBody corroborates duplicate references; closecombat activeframes163-667. Slash rulehandler selects throwhit if isThrowHitAvailable plus previous left/right; upstream availability cause unknown. 14086b900 obtains handler at state+A0.625 reader accepts exact kick exception only, no NPC/partner target; protocol67,bank66/354 existing kick donor1.5s normalized800ms, prior353 unchanged, no root/events. Build/native/wire/bank/smoke pass. Emily626 FAILED native kick selection: source instead crouched right933 then right hit866. Kick remains runtime-unverified; do not random-repeat fixture.625 regressions627 CorvoPASS10/EmilyPASS13 include existing crouched strikes, Emily right recovery15, monotonic followthrough, receiver victimhealth unchanged, complete inventories/playerpartnerhealth/14hash restoration. Left16 remains unverified. Receipt628 and actual observer sheets, no visual acceptance. Next sourceheavy629/630, existing pipeline only.",
    "type": "Note"
  },
  {
    "address": "141c85d28",
    "category": "D2Coop crouched hit recovery",
    "comment": "ExactB671F824 native613 both roles: Sneaking6+Slash39 physical1 scene handle state+24. Crouched right/left Scene_1/933 bind owned rightarm+player+uniqueguard. Optional Corvo left_hit Scene_2/666 arm+player becomesRunning0/physical0; Emily hit Scene_1/866 arm+player crouched. Selection cause unknown.619/621 show same-scene rise with no separate hit;620 accepted observed ground stance.620/622 both rolesPASS10; neither621 selected15/16. Later625/628 regressions: Corvo627PASS10, Emily627PASS13 including owned right recovery15 through Steam, monotonic authored followthrough, native stance return, unchanged receiver victimhealth, checkpoint inventories/playerpartnerhealth14protected hashes. Right15 now verified; left16 remains runtime-unverified.626 failed required kick selection but contains right15 and narrower independent audit628; failed result unchanged. Existing bank attack48/49 reused. Late entry .5 authored progress fallback is heuristic, not source fact. Observer sheets show body strike/recovery; fine contact/grip deferred, no visual acceptance. Shared enemy outcomes unfinished.",
    "type": "Note"
  },
  {
    "address": "141c85d28",
    "category": "D2Coop crouched heavy phases",
    "comment": "Exact B671F824: native focus In266/Loop2000, crouched release sword_attack_focus_sneak_out; owned player+right arm, physical1, Sneaking6+Slash39, state+24 handle. CORRECTION635: subscene is NOT character-specific. Corvo634 selected Scene_1/833; earlier629 Scene_2/1000. Void635 hash070B199FBD070824C7B0A3B7A4DEFA69F9E65F716CBA578D32D7BC6FBFC03B06 confirms common Corvo/Emily arm/body groups and out_a Scene_1/833 versus out_b Scene_2/1000.633 failed release due wrong role guard; original634 failure retained/restoration PASS. Corrected635 protocol69/bank67/357: both636 roles PASS10, receipt637. Ordinary stationary crouched charge/hold/release18 reaches whole remote body through Steam, native sword, independent root and stance return; inventories/health/14protected hashes restored. Both636 selected Scene_1/833; corrected remote Scene_2/1000 not exercised. Existing complete-body heavy50/51/52 crouch adaptation into354/355/356; prior354 unchanged. Native first-person/root/events/damage untouched. Actual observer sheets reviewed: coarse transitions remain, no final visual acceptance. Both release subscenes share one donor; distinct directional acting, moving/cancel/interrupt contexts and shared victim outcomes remain open.",
    "type": "Note"
  }
]

````
