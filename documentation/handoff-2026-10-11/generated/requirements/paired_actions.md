# paired_actions: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-18T17:47:48.827917+02:00. All fields below retain the catalog wording.

## lethal_takedown_front_back_sides

Visual sweep 1260: accepted (corvo accept, emily accept). The rear fast-back lethal body action reads for both characters. Only the back direction is recorded, and victim reaction and alignment are not part of these takes. Scope: rear presentation accepted (sweep); drop and slide accepted in audit 1257. Audit 1257: every census-1255 lethal scene is admitted by exact asset/subscene/duration; runtime: fast-back and assassination_back (Corvo), drop and slide in both roles, host-sourced four-binding sword finishers. Side and front lethals were not reached by the fixture (the guard notices a standing attacker). 604/605: Corvo and Emily each pass10 native/Steam fast-back lethal actor checks. Exact state13/standing0 and owned Npc1333 scene drive existing native third-person donor; native sword equipped/started confirmed, root and idle recovery preserved, receiver victim health40 unchanged. Body sequence observed for both roles. Shared victim alignment/animation/death/corpse and world outcomes remain unimplemented; visual polish deferred. Exact fast-back ordinary-guard actor presentation only; front/side/air/slide, different victim sizes, interruptions and campaign coverage remain incomplete. Receiver target health remained40 before/after each605 test, so cosmetic body playback did not duplicate damage. Shared victim death/corpse/animation and world outcomes are still separate unfinished work. Both native sword equipped and started were independently observed during state13; source first-person and inputs unchanged. Actual observer body sheets viewed for both roles; contact/grip/footwork polish deferred, no visual acceptance. Existing352 clip payloads unchanged; earlier592 breakout/finish receipts retain their own build attribution.

````json
{
  "id": "lethal_takedown_front_back_sides",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_census_lethals_back_drop_slide_finishers_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "lethal takedown front back sides",
  "motions": [
    "sword_doppelganger_assassination_fastback",
    "sword_doppelganger_assassination_fastfront",
    "paired_fast_front_tall",
    "paired_fast_front_female",
    "paired_fast_back_tall",
    "paired_fast_back_female"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The rear fast-back lethal body action reads for both characters. Only the back direction is recorded, and victim reaction and alignment are not part of these takes. Scope: rear presentation accepted (sweep); drop and slide accepted in audit 1257. Audit 1257: every census-1255 lethal scene is admitted by exact asset/subscene/duration; runtime: fast-back and assassination_back (Corvo), drop and slide in both roles, host-sourced four-binding sword finishers. Side and front lethals were not reached by the fixture (the guard notices a standing attacker). 604/605: Corvo and Emily each pass10 native/Steam fast-back lethal actor checks. Exact state13/standing0 and owned Npc1333 scene drive existing native third-person donor; native sword equipped/started confirmed, root and idle recovery preserved, receiver victim health40 unchanged. Body sequence observed for both roles. Shared victim alignment/animation/death/corpse and world outcomes remain unimplemented; visual polish deferred. Exact fast-back ordinary-guard actor presentation only; front/side/air/slide, different victim sizes, interruptions and campaign coverage remain incomplete. Receiver target health remained40 before/after each605 test, so cosmetic body playback did not duplicate damage. Shared victim death/corpse/animation and world outcomes are still separate unfinished work. Both native sword equipped and started were independently observed during state13; source first-person and inputs unchanged. Actual observer body sheets viewed for both roles; contact/grip/footwork polish deferred, no visual acceptance. Existing352 clip payloads unchanged; earlier592 breakout/finish receipts retain their own build attribution.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "partner-sword-contexts-host-v392/host-front-standing.mp4",
    "partner-sword-contexts-host-v398/host-front-standing.mp4",
    "partner-sword-contexts-host-v398/host-rear-standing.mp4",
    "partner-sword-contexts-host-v398/host-front-crouched.mp4",
    "partner-sword-contexts-host-v398/host-rear-crouched.mp4",
    "partner-sword-contexts-client-v400/client-front-standing.mp4",
    "partner-sword-contexts-client-v400/client-rear-standing.mp4",
    "partner-sword-contexts-client-v400/client-front-crouched.mp4",
    "partner-sword-contexts-client-v400/client-rear-crouched.mp4",
    "ordinary-guard-sword-control-v399/host-front-sword.mp4",
    "partner-sword-contexts-host-v397/host-front-standing.mp4",
    "native-lethal-corvo-v596/host-rear-lethal.mp4",
    "lethal-network-corvo-v605/host-rear-lethal.mp4",
    "lethal-network-corvo-v605/observer.mp4",
    "native-lethal-emily-v596/client-rear-lethal.mp4",
    "lethal-network-emily-v605/client-rear-lethal.mp4",
    "lethal-network-emily-v605/observer.mp4"
  ],
  "complete": true,
  "visual_accepted": false,
  "evidence": [
    "../movement-integration/takedown-variants-audit-v1257.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "takedown-variants-v1254i/result.json",
    "takedown-variants-v1254h/result.json",
    "takedown-variants-v1254w/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1369-lethal.json",
    "decision": "complete",
    "scope": "Lethal takedowns from behind, from either side and from the front, by either player, presented on the partner as the native assassination with a shared death.",
    "takes": [
      {
        "take": "takedown-variants-v1254i",
        "cases": [
          "host-back-lethal",
          "client-back-lethal"
        ]
      },
      {
        "take": "takedown-variants-v1254h",
        "cases": [
          "host-left-lethal",
          "client-left-lethal",
          "host-right-lethal",
          "client-right-lethal",
          "host-front-lethal"
        ]
      },
      {
        "take": "takedown-variants-v1254w",
        "cases": [
          "client-front-lethal"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254i/back-host-lethal-observer.mp4",
        "verdict": "Corvo seizes the guard from behind and stabs; the guard arches, drops to his knees and slumps at Corvo's feet."
      },
      {
        "video": "takedown-variants-v1254i/back-client-lethal-observer.mp4",
        "verdict": "Emily takes the guard from behind and drives the blade in; he crumples to the floor while she crouches over him with the sword raised."
      },
      {
        "video": "takedown-variants-v1254h/left-host-lethal-observer.mp4",
        "verdict": "Corvo comes in at the guard's left, grabs him and brings him down; the guard ends kneeling and slumped beside him."
      },
      {
        "video": "takedown-variants-v1254h/left-client-lethal-observer.mp4",
        "verdict": "Emily strikes at the guard's left side and he slumps to the floor beside her."
      },
      {
        "video": "takedown-variants-v1254h/right-host-lethal-observer.mp4",
        "verdict": "Corvo lunges at the guard's right side and hauls him over; the body falls away out of frame behind the railing while Corvo straightens."
      },
      {
        "video": "takedown-variants-v1254h/right-client-lethal-observer.mp4",
        "verdict": "Emily grabs the guard from his right; he folds and drops to the floor at her feet."
      },
      {
        "video": "takedown-variants-v1254h/front-host-lethal-observer.mp4",
        "verdict": "Corvo walks up to the guard face to face, grabs him and drives him down; the guard sinks at his feet and Corvo straightens with the sword raised."
      },
      {
        "video": "takedown-variants-v1254w/front-client-lethal-observer.mp4",
        "verdict": "Emily steps in to the guard standing and drives into him from the front; he goes down at her feet and she stands over the body."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: ../movement-integration/takedown-variants-audit-v1257.json](<../../../../work/movement-integration/takedown-variants-audit-v1257.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: takedown-variants-v1254i/result.json](<../../../../work/natural-moveset/takedown-variants-v1254i/result.json>)
- [evidence: takedown-variants-v1254h/result.json](<../../../../work/natural-moveset/takedown-variants-v1254h/result.json>)
- [evidence: takedown-variants-v1254w/result.json](<../../../../work/natural-moveset/takedown-variants-v1254w/result.json>)
- [in_game_recordings: partner-sword-contexts-host-v392/host-front-standing.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v392/host-front-standing.mp4>)
- [in_game_recordings: partner-sword-contexts-host-v398/host-front-standing.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v398/host-front-standing.mp4>)
- [in_game_recordings: partner-sword-contexts-host-v398/host-rear-standing.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v398/host-rear-standing.mp4>)
- [in_game_recordings: partner-sword-contexts-host-v398/host-front-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v398/host-front-crouched.mp4>)
- [in_game_recordings: partner-sword-contexts-host-v398/host-rear-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v398/host-rear-crouched.mp4>)
- [in_game_recordings: partner-sword-contexts-client-v400/client-front-standing.mp4](<../../../../work/natural-moveset/partner-sword-contexts-client-v400/client-front-standing.mp4>)
- [in_game_recordings: partner-sword-contexts-client-v400/client-rear-standing.mp4](<../../../../work/natural-moveset/partner-sword-contexts-client-v400/client-rear-standing.mp4>)
- [in_game_recordings: partner-sword-contexts-client-v400/client-front-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-client-v400/client-front-crouched.mp4>)
- [in_game_recordings: partner-sword-contexts-client-v400/client-rear-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-client-v400/client-rear-crouched.mp4>)
- [in_game_recordings: ordinary-guard-sword-control-v399/host-front-sword.mp4](<../../../../work/natural-moveset/ordinary-guard-sword-control-v399/host-front-sword.mp4>)
- [in_game_recordings: partner-sword-contexts-host-v397/host-front-standing.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v397/host-front-standing.mp4>)
- [in_game_recordings: native-lethal-corvo-v596/host-rear-lethal.mp4](<../../../../work/natural-moveset/native-lethal-corvo-v596/host-rear-lethal.mp4>)
- [in_game_recordings: lethal-network-corvo-v605/host-rear-lethal.mp4](<../../../../work/natural-moveset/lethal-network-corvo-v605/host-rear-lethal.mp4>)
- [in_game_recordings: lethal-network-corvo-v605/observer.mp4](<../../../../work/natural-moveset/lethal-network-corvo-v605/observer.mp4>)
- [in_game_recordings: native-lethal-emily-v596/client-rear-lethal.mp4](<../../../../work/natural-moveset/native-lethal-emily-v596/client-rear-lethal.mp4>)
- [in_game_recordings: lethal-network-emily-v605/client-rear-lethal.mp4](<../../../../work/natural-moveset/lethal-network-emily-v605/client-rear-lethal.mp4>)
- [in_game_recordings: lethal-network-emily-v605/observer.mp4](<../../../../work/natural-moveset/lethal-network-emily-v605/observer.mp4>)
## nonlethal_takedown_front_back_sides

Visual sweep 1260: accepted (corvo accept, emily accept). The rear choke's entry, hold and release read on both characters. Only rear chokes were judged, and the victim doesn't react in these older takes. Scope: rear presentation accepted (sweep); side/front runtime in audits 1257 and 1254c. Audit 1257: rear choke in both roles; side chokes choke_left/choke_right presented for Emily (ids 68/69, then loop and win, knockout shared); front choke choke_front for Corvo (62). Corvo's side and Emily's front chokes not yet reached by the fixture. Candidate592: distinct native combat-lose1500 now drives the remote full body through Steam, with unchanged source first-person and independent network root. Corvo593/597 and Emily595 pass12 checks each; Corvo/Emily598 finish regression each passes12. Breakout body observed on both roles; no visual acceptance. All previous351 clip payloads preserved in bank64/352. Receiver victim alignment/animation and shared outcomes remain unimplemented. Actor-body presentation only; receiver guard animation/placement, knockout/death, corpse and world outcomes remain unsynchronized by this change. Native cancel400 still unverified; combat_lose1500 is a separate branch. Corvo593 guard occludes most actor; Corvo597 and Emily595 show actual breakout body. Corvo finish598 camera clips geometry and is numerical regression only; unchanged prior clips retain earlier scoped recordings. Visual/contact/grip polish deferred; no visual acceptance or completed moveset claim. Earlier trap576 freshness fade and other archived intermittent cases remain unresolved. Audit 1246 (1213r): the choke hold (Choke41 at stance 12) sent root motion without a wire stance, so peers rejected every pose packet of the hold and the lab lease retired the partner; fixed and re-verified with a standing choke in both roles (1245d).

````json
{
  "id": "nonlethal_takedown_front_back_sides",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_census_chokes_back_both_roles_sides_emily_front_corvo",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "nonlethal takedown front back sides",
  "motions": [
    "paired_nonlethal_hold",
    "nonlethal_back_hold_sequence",
    "nonlethal_back_finish_sequence",
    "nonlethal_back_release_sequence",
    "nonlethal_front_hold_sequence",
    "nonlethal_front_finish_sequence",
    "nonlethal_front_release_sequence",
    "nonlethal_left_hold_sequence",
    "nonlethal_left_finish_sequence",
    "nonlethal_left_release_sequence",
    "nonlethal_right_hold_sequence",
    "nonlethal_right_finish_sequence",
    "nonlethal_right_release_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The rear choke's entry, hold and release read on both characters. Only rear chokes were judged, and the victim doesn't react in these older takes. Scope: rear presentation accepted (sweep); side/front runtime in audits 1257 and 1254c. Audit 1257: rear choke in both roles; side chokes choke_left/choke_right presented for Emily (ids 68/69, then loop and win, knockout shared); front choke choke_front for Corvo (62). Corvo's side and Emily's front chokes not yet reached by the fixture. Candidate592: distinct native combat-lose1500 now drives the remote full body through Steam, with unchanged source first-person and independent network root. Corvo593/597 and Emily595 pass12 checks each; Corvo/Emily598 finish regression each passes12. Breakout body observed on both roles; no visual acceptance. All previous351 clip payloads preserved in bank64/352. Receiver victim alignment/animation and shared outcomes remain unimplemented. Actor-body presentation only; receiver guard animation/placement, knockout/death, corpse and world outcomes remain unsynchronized by this change. Native cancel400 still unverified; combat_lose1500 is a separate branch. Corvo593 guard occludes most actor; Corvo597 and Emily595 show actual breakout body. Corvo finish598 camera clips geometry and is numerical regression only; unchanged prior clips retain earlier scoped recordings. Visual/contact/grip polish deferred; no visual acceptance or completed moveset claim. Earlier trap576 freshness fade and other archived intermittent cases remain unresolved. Audit 1246 (1213r): the choke hold (Choke41 at stance 12) sent root motion without a wire stance, so peers rejected every pose packet of the hold and the lab lease retired the partner; fixed and re-verified with a standing choke in both roles (1245d).",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "partner-takedown-exclusion-host-v388/host-rear-choke.mp4",
    "partner-takedown-exclusion-client-v389/client-rear-choke.mp4",
    "ordinary-guard-choke-control-v390/host-rear-choke.mp4",
    "partner-rear-choke-host-v386/host-rear-choke.mp4",
    "partner-context-targeting-v385/host-rear.mp4",
    "partner-context-targeting-v385/client-rear.mp4",
    "native-guard-choke-corvo-v579/host-rear-choke.mp4",
    "native-guard-choke-emily-v580/client-rear-choke.mp4",
    "choke-network-corvo-v583/host-rear-choke.mp4",
    "choke-network-corvo-v583/observer.mp4",
    "choke-network-corvo-v585/host-rear-choke.mp4",
    "choke-network-corvo-v585/observer.mp4",
    "choke-network-emily-v586/client-rear-choke.mp4",
    "choke-network-emily-v586/observer.mp4",
    "choke-network-emily-v589/client-rear-choke.mp4",
    "choke-network-emily-v589/observer.mp4",
    "native-choke-cancel-corvo-v590/host-rear-choke.mp4",
    "native-choke-cancel-corvo-v591/host-rear-choke.mp4",
    "choke-breakout-corvo-v593/host-rear-choke.mp4",
    "choke-breakout-corvo-v593/observer.mp4",
    "choke-breakout-emily-v595/client-rear-choke.mp4",
    "choke-breakout-emily-v595/observer.mp4",
    "choke-breakout-corvo-v597/host-rear-choke.mp4",
    "choke-breakout-corvo-v597/observer.mp4",
    "choke-finish-corvo-v598/host-rear-choke.mp4",
    "choke-finish-corvo-v598/observer.mp4",
    "choke-finish-emily-v598/client-rear-choke.mp4",
    "choke-finish-emily-v598/observer.mp4"
  ],
  "complete": true,
  "visual_accepted": false,
  "evidence": [
    "../movement-integration/crouched-takedown-audit-v1246.json",
    "../movement-integration/takedown-variants-audit-v1257.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "takedown-variants-v1254i/result.json",
    "takedown-variants-v1254h/result.json",
    "takedown-variants-v1254j/result.json",
    "takedown-variants-v1254l/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1354-chokes.json",
    "decision": "complete",
    "scope": "Chokes from behind (choke_back), the left and right sides (choke_left/choke_right) and the front (choke_front), each followed by choke_loop and choke_win, in both source roles: presented on the partner and the guard unconscious in both worlds at the same place.",
    "takes": [
      {
        "take": "takedown-variants-v1254i",
        "cases": [
          "client-back-choke",
          "host-back-choke"
        ]
      },
      {
        "take": "takedown-variants-v1254h",
        "cases": [
          "client-left-choke",
          "host-left-choke",
          "host-right-choke",
          "host-front-choke"
        ]
      },
      {
        "take": "takedown-variants-v1254j",
        "cases": [
          "client-right-choke"
        ]
      },
      {
        "take": "takedown-variants-v1254l",
        "cases": [
          "client-front-choke"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254i/back-client-choke-observer.mp4",
        "verdict": "Emily takes the guard from behind, holds him in the choke while he struggles and lowers him to the floor."
      },
      {
        "video": "takedown-variants-v1254h/left-client-choke-observer.mp4",
        "verdict": "Emily comes in at the guard's left side, wraps him in the choke and lowers him unconscious."
      },
      {
        "video": "takedown-variants-v1254h/left-host-choke-observer.mp4",
        "verdict": "Corvo seizes the guard from his left side, holds him through the struggle and lets him sag to the floor."
      },
      {
        "video": "takedown-variants-v1254h/right-host-choke-observer.mp4",
        "verdict": "Corvo grabs the guard from the right, holds the choke while the guard kicks, and lowers him."
      },
      {
        "video": "takedown-variants-v1254j/right-client-choke-observer.mp4",
        "verdict": "Emily steps in from the guard's right and chokes him; he folds to the floor beside her."
      },
      {
        "video": "takedown-variants-v1254h/front-host-choke-observer.mp4",
        "verdict": "Corvo seizes the guard face to face; the guard sinks to the floor in front of him (partly hidden behind Corvo)."
      },
      {
        "video": "takedown-variants-v1254l/front-client-choke-observer.mp4",
        "verdict": "Emily faces the guard, seizes him and holds the choke, then lowers him to the floor."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: ../movement-integration/crouched-takedown-audit-v1246.json](<../../../../work/movement-integration/crouched-takedown-audit-v1246.json>)
- [evidence: ../movement-integration/takedown-variants-audit-v1257.json](<../../../../work/movement-integration/takedown-variants-audit-v1257.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: takedown-variants-v1254i/result.json](<../../../../work/natural-moveset/takedown-variants-v1254i/result.json>)
- [evidence: takedown-variants-v1254h/result.json](<../../../../work/natural-moveset/takedown-variants-v1254h/result.json>)
- [evidence: takedown-variants-v1254j/result.json](<../../../../work/natural-moveset/takedown-variants-v1254j/result.json>)
- [evidence: takedown-variants-v1254l/result.json](<../../../../work/natural-moveset/takedown-variants-v1254l/result.json>)
- [in_game_recordings: partner-takedown-exclusion-host-v388/host-rear-choke.mp4](<../../../../work/natural-moveset/partner-takedown-exclusion-host-v388/host-rear-choke.mp4>)
- [in_game_recordings: partner-takedown-exclusion-client-v389/client-rear-choke.mp4](<../../../../work/natural-moveset/partner-takedown-exclusion-client-v389/client-rear-choke.mp4>)
- [in_game_recordings: ordinary-guard-choke-control-v390/host-rear-choke.mp4](<../../../../work/natural-moveset/ordinary-guard-choke-control-v390/host-rear-choke.mp4>)
- [in_game_recordings: partner-rear-choke-host-v386/host-rear-choke.mp4](<../../../../work/natural-moveset/partner-rear-choke-host-v386/host-rear-choke.mp4>)
- [in_game_recordings: partner-context-targeting-v385/host-rear.mp4](<../../../../work/natural-moveset/partner-context-targeting-v385/host-rear.mp4>)
- [in_game_recordings: partner-context-targeting-v385/client-rear.mp4](<../../../../work/natural-moveset/partner-context-targeting-v385/client-rear.mp4>)
- [in_game_recordings: native-guard-choke-corvo-v579/host-rear-choke.mp4](<../../../../work/natural-moveset/native-guard-choke-corvo-v579/host-rear-choke.mp4>)
- [in_game_recordings: native-guard-choke-emily-v580/client-rear-choke.mp4](<../../../../work/natural-moveset/native-guard-choke-emily-v580/client-rear-choke.mp4>)
- [in_game_recordings: choke-network-corvo-v583/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-network-corvo-v583/host-rear-choke.mp4>)
- [in_game_recordings: choke-network-corvo-v583/observer.mp4](<../../../../work/natural-moveset/choke-network-corvo-v583/observer.mp4>)
- [in_game_recordings: choke-network-corvo-v585/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-network-corvo-v585/host-rear-choke.mp4>)
- [in_game_recordings: choke-network-corvo-v585/observer.mp4](<../../../../work/natural-moveset/choke-network-corvo-v585/observer.mp4>)
- [in_game_recordings: choke-network-emily-v586/client-rear-choke.mp4](<../../../../work/natural-moveset/choke-network-emily-v586/client-rear-choke.mp4>)
- [in_game_recordings: choke-network-emily-v586/observer.mp4](<../../../../work/natural-moveset/choke-network-emily-v586/observer.mp4>)
- [in_game_recordings: choke-network-emily-v589/client-rear-choke.mp4](<../../../../work/natural-moveset/choke-network-emily-v589/client-rear-choke.mp4>)
- [in_game_recordings: choke-network-emily-v589/observer.mp4](<../../../../work/natural-moveset/choke-network-emily-v589/observer.mp4>)
- [in_game_recordings: native-choke-cancel-corvo-v590/host-rear-choke.mp4](<../../../../work/natural-moveset/native-choke-cancel-corvo-v590/host-rear-choke.mp4>)
- [in_game_recordings: native-choke-cancel-corvo-v591/host-rear-choke.mp4](<../../../../work/natural-moveset/native-choke-cancel-corvo-v591/host-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-corvo-v593/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v593/host-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-corvo-v593/observer.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v593/observer.mp4>)
- [in_game_recordings: choke-breakout-emily-v595/client-rear-choke.mp4](<../../../../work/natural-moveset/choke-breakout-emily-v595/client-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-emily-v595/observer.mp4](<../../../../work/natural-moveset/choke-breakout-emily-v595/observer.mp4>)
- [in_game_recordings: choke-breakout-corvo-v597/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v597/host-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-corvo-v597/observer.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v597/observer.mp4>)
- [in_game_recordings: choke-finish-corvo-v598/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-finish-corvo-v598/host-rear-choke.mp4>)
- [in_game_recordings: choke-finish-corvo-v598/observer.mp4](<../../../../work/natural-moveset/choke-finish-corvo-v598/observer.mp4>)
- [in_game_recordings: choke-finish-emily-v598/client-rear-choke.mp4](<../../../../work/natural-moveset/choke-finish-emily-v598/client-rear-choke.mp4>)
- [in_game_recordings: choke-finish-emily-v598/observer.mp4](<../../../../work/natural-moveset/choke-finish-emily-v598/observer.mp4>)
## crouched_takedown

Audit 1246 (1213r): a rear takedown started from crouch is bound in both roles. Native: the choke stands the player into Choke41 (same scenes as standing); the fast-back lethal keeps the player crouched (Assassination13 at stance 1, admitted by 1213p). The partner presents every native phase and returns to locomotion. The partner rises into the standing paired clips; a crouched-height lethal clip, victim alignment and shared outcomes remain open.

````json
{
  "id": "crouched_takedown",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_crouched_rear_choke_and_fast_back_lethal_presented_both_roles",
  "visual_validation": "accepted_client_choke_client_lethal_host_choke_host_lethal",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "crouched takedown",
  "motions": [
    "crouched_paired_fast_back_tall",
    "crouched_nonlethal_back_hold_sequence",
    "crouched_nonlethal_back_finish_sequence",
    "crouched_nonlethal_back_release_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1246 (1213r): a rear takedown started from crouch is bound in both roles. Native: the choke stands the player into Choke41 (same scenes as standing); the fast-back lethal keeps the player crouched (Assassination13 at stance 1, admitted by 1213p). The partner presents every native phase and returns to locomotion. The partner rises into the standing paired clips; a crouched-height lethal clip, victim alignment and shared outcomes remain open.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "partner-sword-contexts-host-v398/host-front-crouched.mp4",
    "partner-sword-contexts-host-v398/host-rear-crouched.mp4",
    "partner-sword-contexts-client-v400/client-front-crouched.mp4",
    "partner-sword-contexts-client-v400/client-rear-crouched.mp4",
    "crouched-takedown-observed-v1247b/host-choke-observer.mp4",
    "crouched-takedown-observed-v1247d/host-lethal-observer.mp4",
    "crouched-takedown-observed-v1247d/client-choke-observer.mp4",
    "crouched-takedown-observed-v1247d/client-lethal-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/crouched-takedown-audit-v1246.json",
    "takedown-variants-v1254g/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1345-paired.json",
    "decision": "complete",
    "scope": "Takedowns started from a crouch in both roles: the side lethals (approach crouched; native scenes 31 and 108) and Corvo's crouched front choke (62) are presented with the shared outcome; the crouched rear choke and fast-back lethal were bound in both roles in audit 1246.",
    "takes": [
      {
        "take": "takedown-variants-v1254g",
        "cases": [
          "host-left-lethal",
          "client-left-lethal",
          "host-front-choke"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254g/left-host-lethal-observer.mp4",
        "verdict": "Corvo comes in low from the guard's side, drives the blade in and the guard folds to the floor of the walkway."
      },
      {
        "video": "takedown-variants-v1254f/right-client-lethal-observer.mp4",
        "verdict": "Emily closes on the guard from the side on the balcony and stabs him; he drops and she straightens over the body."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/crouched-takedown-audit-v1246.json](<../../../../work/movement-integration/crouched-takedown-audit-v1246.json>)
- [evidence: takedown-variants-v1254g/result.json](<../../../../work/natural-moveset/takedown-variants-v1254g/result.json>)
- [in_game_recordings: partner-sword-contexts-host-v398/host-front-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v398/host-front-crouched.mp4>)
- [in_game_recordings: partner-sword-contexts-host-v398/host-rear-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-host-v398/host-rear-crouched.mp4>)
- [in_game_recordings: partner-sword-contexts-client-v400/client-front-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-client-v400/client-front-crouched.mp4>)
- [in_game_recordings: partner-sword-contexts-client-v400/client-rear-crouched.mp4](<../../../../work/natural-moveset/partner-sword-contexts-client-v400/client-rear-crouched.mp4>)
- [in_game_recordings: crouched-takedown-observed-v1247b/host-choke-observer.mp4](<../../../../work/natural-moveset/crouched-takedown-observed-v1247b/host-choke-observer.mp4>)
- [in_game_recordings: crouched-takedown-observed-v1247d/host-lethal-observer.mp4](<../../../../work/natural-moveset/crouched-takedown-observed-v1247d/host-lethal-observer.mp4>)
- [in_game_recordings: crouched-takedown-observed-v1247d/client-choke-observer.mp4](<../../../../work/natural-moveset/crouched-takedown-observed-v1247d/client-choke-observer.mp4>)
- [in_game_recordings: crouched-takedown-observed-v1247d/client-lethal-observer.mp4](<../../../../work/natural-moveset/crouched-takedown-observed-v1247d/client-lethal-observer.mp4>)
## aerial_drop_kill

Audit 1257 (1214e+, product controller, natural guard AI): assassination_drop_back (census id 19) is presented by the partner and the death is shared in both source roles (victims 0.0 m apart). Visual accepted from both windows as functional (fast-back stab donor normalized to the native clock). Revision required: actor/target overlap during impact, sword contact and recovery foot clearance need work. Native victim playback stops at the exact ragdoll event; subsequent physics is NOT simulated. No live paired outcome is implemented.

````json
{
  "id": "aerial_drop_kill",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_drop_lethal_presented_and_shared_both_roles_product",
  "visual_validation": "accepted_functional_both_roles_fast_back_donor",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "aerial drop kill",
  "motions": [
    "aerial_lethal_back_study"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1257 (1214e+, product controller, natural guard AI): assassination_drop_back (census id 19) is presented by the partner and the death is shared in both source roles (victims 0.0 m apart). Visual accepted from both windows as functional (fast-back stab donor normalized to the native clock). Revision required: actor/target overlap during impact, sword contact and recovery foot clearance need work. Native victim playback stops at the exact ragdoll event; subsequent physics is NOT simulated. No live paired outcome is implemented.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "takedown-variants-v1254b/drop-client-lethal-observer.mp4",
    "takedown-variants-v1254b/drop-host-lethal-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/takedown-variants-audit-v1257.json",
    "takedown-variants-v1254k/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1353-aerial.json",
    "decision": "complete",
    "scope": "The drop assassination (assassination_drop_back, 1800-2000 ms) in both source roles: presented on the partner from the native scene, the guard dead in both worlds at the same place.",
    "takes": [
      {
        "take": "takedown-variants-v1254k",
        "cases": [
          "client-drop-lethal",
          "host-drop-lethal"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254k/drop-host-lethal-observer.mp4",
        "verdict": "Corvo falls from above onto the guard, drives him down and rises with the sword raised over the body."
      },
      {
        "video": "takedown-variants-v1254k/drop-client-lethal-observer.mp4",
        "verdict": "Emily lands behind the guard from above and he crumples forward onto the floor; she straightens over the body."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/takedown-variants-audit-v1257.json](<../../../../work/movement-integration/takedown-variants-audit-v1257.json>)
- [evidence: takedown-variants-v1254k/result.json](<../../../../work/natural-moveset/takedown-variants-v1254k/result.json>)
- [in_game_recordings: takedown-variants-v1254b/drop-client-lethal-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/drop-client-lethal-observer.mp4>)
- [in_game_recordings: takedown-variants-v1254b/drop-host-lethal-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/drop-host-lethal-observer.mp4>)
## aerial_nonlethal_takedown

Audit 1257: choke_drop_back (census id 58) is presented and the knockout shared in both source roles (the client asks the host for the knockout when the scene begins; 0.0-0.01 m). Visual accepted from both windows as functional (choke donor). Revision required: initial body collision and weight transfer need work. Native victim playback stops at the exact ragdoll event; subsequent physics is NOT simulated. No live paired outcome is implemented.

````json
{
  "id": "aerial_nonlethal_takedown",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_drop_choke_presented_and_knockout_shared_both_roles_product",
  "visual_validation": "accepted_functional_both_roles_choke_donor",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "aerial nonlethal takedown",
  "motions": [
    "aerial_nonlethal_back_study"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1257: choke_drop_back (census id 58) is presented and the knockout shared in both source roles (the client asks the host for the knockout when the scene begins; 0.0-0.01 m). Visual accepted from both windows as functional (choke donor). Revision required: initial body collision and weight transfer need work. Native victim playback stops at the exact ragdoll event; subsequent physics is NOT simulated. No live paired outcome is implemented.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "takedown-variants-v1254b/drop-client-choke-observer.mp4",
    "takedown-variants-v1254b/drop-host-choke-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/takedown-variants-audit-v1257.json",
    "takedown-variants-v1254k/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1353-aerial.json",
    "decision": "complete",
    "scope": "The drop choke (choke_drop_ready then choke_drop_back, 2166-2633 ms) in both source roles: presented on the partner, the guard unconscious in both worlds at the same place.",
    "takes": [
      {
        "take": "takedown-variants-v1254k",
        "cases": [
          "client-drop-choke",
          "host-drop-choke"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254k/drop-host-choke-observer.mp4",
        "verdict": "Corvo drops onto the guard, forces him to his knees and holds him in the choke until he slumps to the floor."
      },
      {
        "video": "takedown-variants-v1254k/drop-client-choke-observer.mp4",
        "verdict": "Emily lands on the guard from above and takes him down; he lies unconscious at her feet."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/takedown-variants-audit-v1257.json](<../../../../work/movement-integration/takedown-variants-audit-v1257.json>)
- [evidence: takedown-variants-v1254k/result.json](<../../../../work/natural-moveset/takedown-variants-v1254k/result.json>)
- [in_game_recordings: takedown-variants-v1254b/drop-client-choke-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/drop-client-choke-observer.mp4>)
- [in_game_recordings: takedown-variants-v1254b/drop-host-choke-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/drop-host-choke-observer.mp4>)
## sliding_takedown

Audit 1257: choke_slide_back (73) and assassination_slide_back (33) are presented and shared in both source roles; slide takedowns keep physical stance 5 inside the paired state (0.0-0.29 m). Visual accepted from both windows as functional. Rear nonlethal prototype only. Revision required: initial sweep/body overlap, recovery and Corvo hand contact (up to 4cm residual) need work. Emily wrist targets fit within 0.3mm, which does not establish acceptable hand/mesh contact. Targets hold their native ragdoll-time pose; physics and live pairing are absent.

````json
{
  "id": "sliding_takedown",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_slide_choke_and_lethal_presented_and_shared_both_roles_product",
  "visual_validation": "accepted_functional_both_roles_choke_and_lethal",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "sliding takedown",
  "motions": [
    "sliding_nonlethal_back_study"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1257: choke_slide_back (73) and assassination_slide_back (33) are presented and shared in both source roles; slide takedowns keep physical stance 5 inside the paired state (0.0-0.29 m). Visual accepted from both windows as functional. Rear nonlethal prototype only. Revision required: initial sweep/body overlap, recovery and Corvo hand contact (up to 4cm residual) need work. Emily wrist targets fit within 0.3mm, which does not establish acceptable hand/mesh contact. Targets hold their native ragdoll-time pose; physics and live pairing are absent.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "takedown-variants-v1254b/slide-client-lethal-observer.mp4",
    "takedown-variants-v1254b/slide-host-lethal-observer.mp4",
    "takedown-variants-v1254b/slide-client-choke-observer.mp4",
    "takedown-variants-v1254b/slide-host-choke-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/takedown-variants-audit-v1257.json",
    "takedown-variants-v1254k/result.json",
    "takedown-variants-v1254h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1355-slide.json",
    "decision": "complete",
    "scope": "The slide assassination (assassination_slide_back, 2000-2333 ms) in both source roles in two takes: presented on the partner, the guard dead in both worlds at the same place; the slide choke (census 73) from audit 1257.",
    "takes": [
      {
        "take": "takedown-variants-v1254k",
        "cases": [
          "client-slide-lethal",
          "host-slide-lethal"
        ]
      },
      {
        "take": "takedown-variants-v1254h",
        "cases": [
          "client-slide-lethal",
          "host-slide-lethal"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254h/slide-host-lethal-observer.mp4",
        "verdict": "Corvo slides in behind the guard and strikes as he rises; the guard falls and Corvo stands over the body."
      },
      {
        "video": "takedown-variants-v1254k/slide-client-lethal-observer.mp4",
        "verdict": "Emily slides in low behind the guard and strikes; he pitches forward onto the floor and she straightens behind him."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/takedown-variants-audit-v1257.json](<../../../../work/movement-integration/takedown-variants-audit-v1257.json>)
- [evidence: takedown-variants-v1254k/result.json](<../../../../work/natural-moveset/takedown-variants-v1254k/result.json>)
- [evidence: takedown-variants-v1254h/result.json](<../../../../work/natural-moveset/takedown-variants-v1254h/result.json>)
- [in_game_recordings: takedown-variants-v1254b/slide-client-lethal-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/slide-client-lethal-observer.mp4>)
- [in_game_recordings: takedown-variants-v1254b/slide-host-lethal-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/slide-host-lethal-observer.mp4>)
- [in_game_recordings: takedown-variants-v1254b/slide-client-choke-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/slide-client-choke-observer.mp4>)
- [in_game_recordings: takedown-variants-v1254b/slide-host-choke-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/slide-host-choke-observer.mp4>)
## wall_and_window_variants

Rear window lethal and nonlethal entry/hold/finish/release studies. Native victim timing accompanies authored full-body stepping and pulling. Neutral sill clearance was checked visually. Fingers, blade contact, native window geometry, ragdoll continuation and live paired control remain unverified. Wall and other window directions remain missing.

````json
{
  "id": "wall_and_window_variants",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "wall and window variants",
  "motions": [
    "window_rear_lethal_study",
    "window_nonlethal_hold_sequence",
    "window_nonlethal_finish_sequence",
    "window_nonlethal_release_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Rear window lethal and nonlethal entry/hold/finish/release studies. Native victim timing accompanies authored full-body stepping and pulling. Neutral sill clearance was checked visually. Fingers, blade contact, native window geometry, ragdoll continuation and live paired control remain unverified. Wall and other window directions remain missing.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": []
}
````

## enemy_body_size_variants

Tall and female native-rig stand-ins only. These are neutral-grey review targets, not the co-op partner. Small, armored, nonhuman and contextual variants remain missing.

````json
{
  "id": "enemy_body_size_variants",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "enemy body size variants",
  "motions": [
    "paired_fast_front_tall",
    "paired_fast_front_female",
    "paired_fast_back_tall",
    "paired_fast_back_female"
  ],
  "deliverable_kind": "animation",
  "notes": "Tall and female native-rig stand-ins only. These are neutral-grey review targets, not the co-op partner. Small, armored, nonhuman and contextual variants remain missing.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": []
}
````

## paired_victim_alignment

Audit 1252 (1214a, product controller, natural guard AI): for a rear choke or fast-back lethal by either player on a host-owned NPC, the victim is animated and ends in the same place and state in both worlds (host-sourced: the NPC stream carries the host victim; client-sourced: the host holds its NPC, follows the client victim pose and knocks it out at choke_win; deaths ride the combat relay). Visuals accepted for the host- and client-sourced choke and the client-sourced lethal. Other takedown scenes (assassination_back, front, air, slide) and body sizes remain open.

````json
{
  "id": "paired_victim_alignment",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "shared_victim_and_outcome_both_roles_choke_and_lethal_product",
  "visual_validation": "accepted_host_and_client_sourced_rear_choke_and_lethal_victims",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "paired victim alignment",
  "motions": [
    "paired_fast_front_tall",
    "paired_fast_front_female",
    "paired_fast_back_tall",
    "paired_fast_back_female"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1252 (1214a, product controller, natural guard AI): for a rear choke or fast-back lethal by either player on a host-owned NPC, the victim is animated and ends in the same place and state in both worlds (host-sourced: the NPC stream carries the host victim; client-sourced: the host holds its NPC, follows the client victim pose and knocks it out at choke_win; deaths ride the combat relay). Visuals accepted for the host- and client-sourced choke and the client-sourced lethal. Other takedown scenes (assassination_back, front, air, slide) and body sizes remain open.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "paired-victim-product-v1248/host-choke-observer.mp4",
    "paired-victim-product-v1248j/client-choke-observer.mp4",
    "paired-victim-product-v1248j/client-lethal-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/paired-victim-audit-v1249.json",
    "../movement-integration/paired-takedowns-audit-v1252.json",
    "takedown-variants-v1254g/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1345-paired.json",
    "decision": "complete",
    "scope": "Every passing takedown in both roles (back, side, front, drop and slide; chokes and lethals; 15 cases in 1254g alone) ends with the same outcome in both worlds and the victim within 0-0.03 m of the same place, the partner presenting the source side of the paired scene.",
    "takes": [
      {
        "take": "takedown-variants-v1254g",
        "cases": [
          "host-back-choke",
          "client-back-choke",
          "host-back-lethal",
          "client-back-lethal",
          "host-left-lethal",
          "client-left-lethal",
          "host-drop-lethal",
          "client-drop-lethal"
        ]
      }
    ],
    "visual": [
      {
        "video": "takedown-variants-v1254g/back-host-choke-observer.mp4",
        "verdict": "Corvo grabs the guard from behind and holds the choke while the guard struggles; the guard sags and collapses at his feet in the same place."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/paired-victim-audit-v1249.json](<../../../../work/movement-integration/paired-victim-audit-v1249.json>)
- [evidence: ../movement-integration/paired-takedowns-audit-v1252.json](<../../../../work/movement-integration/paired-takedowns-audit-v1252.json>)
- [evidence: takedown-variants-v1254g/result.json](<../../../../work/natural-moveset/takedown-variants-v1254g/result.json>)
- [in_game_recordings: paired-victim-product-v1248/host-choke-observer.mp4](<../../../../work/natural-moveset/paired-victim-product-v1248/host-choke-observer.mp4>)
- [in_game_recordings: paired-victim-product-v1248j/client-choke-observer.mp4](<../../../../work/natural-moveset/paired-victim-product-v1248j/client-choke-observer.mp4>)
- [in_game_recordings: paired-victim-product-v1248j/client-lethal-observer.mp4](<../../../../work/natural-moveset/paired-victim-product-v1248j/client-lethal-observer.mp4>)
## struggle_and_escape

Visual sweep 1260: accepted (corvo accept, emily accept). The choke breakout (being thrown off and recovering) reads clearly for both characters in the newer takes. The victim's side is not synchronized. Candidate592: distinct native combat-lose1500 now drives the remote full body through Steam, with unchanged source first-person and independent network root. Corvo593/597 and Emily595 pass12 checks each; Corvo/Emily598 finish regression each passes12. Breakout body observed on both roles; no visual acceptance. All previous351 clip payloads preserved in bank64/352. Receiver victim alignment/animation and shared outcomes remain unimplemented. Actor-body presentation only; receiver guard animation/placement, knockout/death, corpse and world outcomes remain unsynchronized by this change. Native cancel400 still unverified; combat_lose1500 is a separate branch. Corvo593 guard occludes most actor; Corvo597 and Emily595 show actual breakout body. Corvo finish598 camera clips geometry and is numerical regression only; unchanged prior clips retain earlier scoped recordings. Visual/contact/grip polish deferred; no visual acceptance or completed moveset claim. Earlier trap576 freshness fade and other archived intermittent cases remain unresolved. Audit 1246 (1213r): the choke hold (Choke41 at stance 12) sent root motion without a wire stance, so peers rejected every pose packet of the hold and the lab lease retired the partner; fixed and re-verified with a standing choke in both roles (1245d).

````json
{
  "id": "struggle_and_escape",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_rear_choke_actor_and_breakout",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "struggle and escape",
  "motions": [
    "nonlethal_back_hold_sequence",
    "nonlethal_back_release_sequence",
    "nonlethal_front_hold_sequence",
    "nonlethal_front_release_sequence",
    "nonlethal_left_hold_sequence",
    "nonlethal_left_release_sequence",
    "nonlethal_right_hold_sequence",
    "nonlethal_right_release_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The choke breakout (being thrown off and recovering) reads clearly for both characters in the newer takes. The victim's side is not synchronized. Candidate592: distinct native combat-lose1500 now drives the remote full body through Steam, with unchanged source first-person and independent network root. Corvo593/597 and Emily595 pass12 checks each; Corvo/Emily598 finish regression each passes12. Breakout body observed on both roles; no visual acceptance. All previous351 clip payloads preserved in bank64/352. Receiver victim alignment/animation and shared outcomes remain unimplemented. Actor-body presentation only; receiver guard animation/placement, knockout/death, corpse and world outcomes remain unsynchronized by this change. Native cancel400 still unverified; combat_lose1500 is a separate branch. Corvo593 guard occludes most actor; Corvo597 and Emily595 show actual breakout body. Corvo finish598 camera clips geometry and is numerical regression only; unchanged prior clips retain earlier scoped recordings. Visual/contact/grip polish deferred; no visual acceptance or completed moveset claim. Earlier trap576 freshness fade and other archived intermittent cases remain unresolved. Audit 1246 (1213r): the choke hold (Choke41 at stance 12) sent root motion without a wire stance, so peers rejected every pose packet of the hold and the lab lease retired the partner; fixed and re-verified with a standing choke in both roles (1245d).",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-choke-cancel-corvo-v590/host-rear-choke.mp4",
    "native-choke-cancel-corvo-v591/host-rear-choke.mp4",
    "choke-breakout-corvo-v593/host-rear-choke.mp4",
    "choke-breakout-corvo-v593/observer.mp4",
    "choke-breakout-emily-v595/client-rear-choke.mp4",
    "choke-breakout-emily-v595/observer.mp4",
    "choke-breakout-corvo-v597/host-rear-choke.mp4",
    "choke-breakout-corvo-v597/observer.mp4"
  ],
  "complete": false,
  "visual_accepted": false,
  "evidence": [
    "../movement-integration/crouched-takedown-audit-v1246.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ]
}
````

- [evidence: ../movement-integration/crouched-takedown-audit-v1246.json](<../../../../work/movement-integration/crouched-takedown-audit-v1246.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: native-choke-cancel-corvo-v590/host-rear-choke.mp4](<../../../../work/natural-moveset/native-choke-cancel-corvo-v590/host-rear-choke.mp4>)
- [in_game_recordings: native-choke-cancel-corvo-v591/host-rear-choke.mp4](<../../../../work/natural-moveset/native-choke-cancel-corvo-v591/host-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-corvo-v593/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v593/host-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-corvo-v593/observer.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v593/observer.mp4>)
- [in_game_recordings: choke-breakout-emily-v595/client-rear-choke.mp4](<../../../../work/natural-moveset/choke-breakout-emily-v595/client-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-emily-v595/observer.mp4](<../../../../work/natural-moveset/choke-breakout-emily-v595/observer.mp4>)
- [in_game_recordings: choke-breakout-corvo-v597/host-rear-choke.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v597/host-rear-choke.mp4>)
- [in_game_recordings: choke-breakout-corvo-v597/observer.mp4](<../../../../work/natural-moveset/choke-breakout-corvo-v597/observer.mp4>)
## carry_body_lift_idle_move_drop_throw

Audit 1267: lift, carry and throw accepted for both characters from the v38 recordings (the sweep had judged v13). Visual sweep 1260: reject (corvo reject, emily reject). Carrying and dropping work, but in both roles the pickup is a one-frame shoulder attachment pop with no lift, and no throw is recorded. Scoped native pickup, stance-change, low-drop, high-throw and compact-release experiments are preserved by build. v38 removes the compact-release upright pop in both roles; named high throws and low drops regress successfully. Final hand contact, recovery, arbitrary bodies and interrupted lifecycle remain unfinished. Historical failed comparisons remain explicitly labelled.

````json
{
  "id": "carry_body_lift_idle_move_drop_throw",
  "family": "paired_actions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "bounded_native_source_paired_pickup_both_roles",
  "visual_validation": "accepted_functional_both_roles_v38",
  "native_reference_family_count": 927,
  "priority": 3,
  "title": "carry body lift idle move drop throw",
  "motions": [
    "carry_body_idle",
    "carry_body_walk",
    "carry_body_drop_low_sequence",
    "carry_body_drop_high_sequence",
    "carry_body_drop_soft_sequence",
    "carry_body_floor_lift_study",
    "carry_body_throw_study",
    "carry_body_short_lift_study",
    "carry_pickup_contact_pilot",
    "carry_pickup_supported_settle"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1267: lift, carry and throw accepted for both characters from the v38 recordings (the sweep had judged v13). Visual sweep 1260: reject (corvo reject, emily reject). Carrying and dropping work, but in both roles the pickup is a one-frame shoulder attachment pop with no lift, and no throw is recorded. Scoped native pickup, stance-change, low-drop, high-throw and compact-release experiments are preserved by build. v38 removes the compact-release upright pop in both roles; named high throws and low drops regress successfully. Final hand contact, recovery, arbitrary bodies and interrupted lifecycle remain unfinished. Historical failed comparisons remain explicitly labelled.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-carry-corvo-reader-v2/native.mp4",
    "native-carry-emily-reader-v2/native.mp4",
    "native-carry-carrier-corvo-v3/native.mp4",
    "native-carry-carrier-emily-v3/native.mp4",
    "carry-victim-native-corvo-v8/native.mp4",
    "carry-victim-native-emily-v8/native.mp4",
    "carry-victim-native-corvo-v13/native.mp4",
    "carry-victim-native-corvo-crouched-v13/native.mp4",
    "carry-victim-native-emily-v13/native.mp4",
    "carry-victim-native-emily-crouched-v13/native.mp4",
    "carry-contact-corvo-crouched-v2/native.mp4",
    "carry-contact-emily-crouched-v2/native.mp4",
    "carry-cleanup-stream-corvo-v16/native.mp4",
    "native-pickup-geometry-corvo-v1/native.mp4",
    "carry-cleanup-stream-emily-v16/native.mp4",
    "native-pickup-geometry-emily-v1/native.mp4",
    "native-pickup-stream-corvo-v17/native.mp4",
    "native-pickup-stream-emily-v17/native.mp4",
    "carry-snapshot-corvo-v20/native.mp4",
    "carry-snapshot-emily-v20/native.mp4",
    "carry-snapshot-cancel-corvo-v20/native.mp4",
    "carry-snapshot-corvo-v18/native.mp4",
    "native-pickup-crouched-corvo-v21b/native.mp4",
    "carry-snapshot-cancel-corvo-v21/native.mp4",
    "native-pickup-crouched-emily-v21/native.mp4",
    "carry-snapshot-cancel-emily-v21/native.mp4",
    "carry-snapshot-cancel-emily-v22/native.mp4",
    "carry-snapshot-cancel-corvo-v22/native.mp4",
    "carry-snapshot-cancel-emily-v22b/native.mp4",
    "carry-snapshot-stream-emily-v22/native.mp4",
    "short-lift-corvo-v4/native.mp4",
    "short-lift-emily-v4/native.mp4",
    "short-lift-corvo-v6/native.mp4",
    "short-lift-emily-v6/native.mp4",
    "native-carrier-pickup-corvo-v23/native.mp4",
    "native-carrier-pickup-emily-v23/native.mp4",
    "native-carrier-pickup-crouched-corvo-v23/native.mp4",
    "native-paired-pickup-corvo-v24/native.mp4",
    "native-paired-pickup-emily-v24/native.mp4",
    "native-paired-pickup-corvo-v24-trace/native.mp4",
    "native-paired-pickup-corvo-v24-fast/native.mp4",
    "native-paired-pickup-emily-v24-fast/native.mp4",
    "native-paired-pickup-corvo-contact-v3/native.mp4",
    "native-paired-pickup-emily-cancel-v24c/native.mp4",
    "native-paired-pickup-emily-contact-v4/native.mp4",
    "native-paired-pickup-corvo-cancel-v24/native.mp4",
    "native-paired-pickup-corvo-contact-v3-final/native.mp4",
    "native-paired-pickup-corvo-settle-v25/native.mp4",
    "native-paired-pickup-emily-settle-v25/native.mp4",
    "native-paired-pickup-corvo-visibility-v26/native.mp4",
    "native-paired-pickup-emily-visibility-v26/native.mp4",
    "native-pickup-walk-drop-corvo-v26/native.mp4",
    "native-pickup-walk-drop-emily-v26/native.mp4",
    "native-pickup-walk-drop-corvo-v27/native.mp4",
    "native-pickup-walk-drop-emily-v27/native.mp4",
    "native-pickup-walk-crouch-drop-corvo-v27/native.mp4",
    "native-pickup-walk-crouch-drop-emily-v27/native.mp4",
    "native-crouched-pickup-drop-corvo-v28/native.mp4",
    "native-crouched-pickup-drop-emily-v28/native.mp4",
    "native-crouched-pickup-drop-corvo-v28-contact-v2/native.mp4",
    "native-crouched-pickup-drop-emily-v28-contact-v2/native.mp4",
    "native-stand-crouch-carry-corvo-v28-contact-v2/native.mp4",
    "native-crouched-pickup-corvo-orientation-v29/native.mp4",
    "native-crouched-pickup-emily-orientation-v29/native.mp4",
    "native-crouched-pickup-emily-v29-contact-v3/native.mp4",
    "native-crouched-to-standing-carry-corvo-v29-contact-v3/native.mp4",
    "native-standing-to-crouched-carry-emily-v29-contact-v3/native.mp4",
    "native-crouched-to-standing-carry-corvo-v30-sampled/native.mp4",
    "native-standing-to-crouched-carry-emily-v30/native.mp4",
    "native-body-throw-corvo-v1/native.mp4",
    "native-body-throw-emily-v1/native.mp4",
    "native-high-throw-emily-v31/native.mp4",
    "native-high-throw-emily-v32/native.mp4",
    "native-high-throw-corvo-v32/native.mp4",
    "native-high-throw-corvo-v33/native.mp4",
    "native-high-throw-emily-v33/native.mp4",
    "native-high-throw-corvo-open-v33-b/native.mp4",
    "native-high-throw-emily-contact-v34/native.mp4",
    "native-high-throw-corvo-v34/native.mp4",
    "native-high-throw-emily-v34/native.mp4",
    "native-low-drop-corvo-regression-v34/native.mp4",
    "native-low-drop-emily-regression-v34/native.mp4",
    "native-fetal-release-corvo-reader-v35/native.mp4",
    "native-fetal-release-corvo-v35/native.mp4",
    "native-fetal-release-corvo-turn-v35/native.mp4",
    "native-fetal-release-emily-turn-v35/native.mp4",
    "native-fetal-release-corvo-framed-v35/native.mp4",
    "native-fetal-release-emily-framed-v35/native.mp4",
    "native-fetal-release-corvo-blend-v36/native.mp4",
    "native-fetal-release-corvo-order-v37/native.mp4",
    "native-fetal-release-corvo-hold-v38/native.mp4",
    "native-fetal-release-emily-hold-v38/native.mp4",
    "native-high-throw-corvo-regression-v38/native.mp4",
    "native-high-throw-emily-regression-v38/native.mp4",
    "native-low-drop-corvo-regression-v38/native.mp4",
    "native-low-drop-emily-regression-v38/native.mp4"
  ],
  "native_source_evidence": {
    "findings": "native-carry-reader-findings-v2.json",
    "cases": [
      "native-carry-corvo-reader-v2",
      "native-carry-emily-reader-v2"
    ],
    "remote_gameplay_bound": false
  },
  "carrier_runtime_evidence": {
    "cases": {
      "corvo": {
        "video": "native-carry-carrier-corvo-v3/native.mp4",
        "validation": "native-carry-carrier-corvo-v3/validation.json",
        "visual_review": "native-carry-carrier-corvo-v3/visual-review.json",
        "checks": 14,
        "samples": 182,
        "held_samples": 139,
        "actual_capture_hz": 27.0
      },
      "emily": {
        "video": "native-carry-carrier-emily-v3/native.mp4",
        "validation": "native-carry-carrier-emily-v3/validation.json",
        "visual_review": "native-carry-carrier-emily-v3/visual-review.json",
        "checks": 14,
        "samples": 183,
        "held_samples": 140,
        "actual_capture_hz": 27.789473684210527
      }
    },
    "corpse_gameplay_bound": false,
    "complete": false,
    "visual_accepted": false
  },
  "attachment_runtime_evidence": {
    "cases": {
      "corvo": {
        "video": "carry-victim-native-corvo-v8/native.mp4",
        "validation": "carry-victim-native-corvo-v8/validation.json",
        "visual_review": "carry-victim-native-corvo-v8/visual-review.json",
        "checks": 11,
        "samples": 125,
        "held_samples": 55
      },
      "emily": {
        "video": "carry-victim-native-emily-v8/native.mp4",
        "validation": "carry-victim-native-emily-v8/validation.json",
        "visual_review": "carry-victim-native-emily-v8/visual-review.json",
        "checks": 11,
        "samples": 127,
        "held_samples": 57
      }
    },
    "complete": false,
    "visual_accepted": false
  },
  "release_runtime_evidence": {
    "cases": {
      "corvo_standing": {
        "video": "carry-victim-native-corvo-v13/native.mp4",
        "validation": "carry-victim-native-corvo-v13/validation.json",
        "visual_review": "carry-victim-native-corvo-v13/visual-review.json",
        "checks": 19,
        "samples": 146,
        "held_samples": 93,
        "post_release_samples": 30
      },
      "corvo_crouched": {
        "video": "carry-victim-native-corvo-crouched-v13/native.mp4",
        "validation": "carry-victim-native-corvo-crouched-v13/validation.json",
        "visual_review": "carry-victim-native-corvo-crouched-v13/visual-review.json",
        "checks": 19,
        "samples": 143,
        "held_samples": 92,
        "post_release_samples": 32
      },
      "emily_standing": {
        "video": "carry-victim-native-emily-v13/native.mp4",
        "validation": "carry-victim-native-emily-v13/validation.json",
        "visual_review": "carry-victim-native-emily-v13/visual-review.json",
        "checks": 19,
        "samples": 140,
        "held_samples": 89,
        "post_release_samples": 29
      },
      "emily_crouched": {
        "video": "carry-victim-native-emily-crouched-v13/native.mp4",
        "validation": "carry-victim-native-emily-crouched-v13/validation.json",
        "visual_review": "carry-victim-native-emily-crouched-v13/visual-review.json",
        "checks": 19,
        "samples": 145,
        "held_samples": 93,
        "post_release_samples": 31
      }
    },
    "complete": false,
    "visual_accepted": false,
    "source_declarations": "carry-release-scenes-v1.json",
    "carrier_banks": "carry-release-bank-manifest-v1.json"
  },
  "contact_runtime_evidence": {
    "cases": {
      "corvo": {
        "video": "carry-contact-corvo-crouched-v2/native.mp4",
        "validation": "carry-contact-corvo-crouched-v2/validation.json",
        "visual_review": "carry-contact-corvo-crouched-v2/visual-review.json",
        "checks": 19,
        "samples": 144,
        "bank_sha256": "BC6CB2E66FDAB2F1BCCE42A2C668581A4EBC165DCBE44A4090E7005CF692FA60"
      },
      "emily": {
        "video": "carry-contact-emily-crouched-v2/native.mp4",
        "validation": "carry-contact-emily-crouched-v2/validation.json",
        "visual_review": "carry-contact-emily-crouched-v2/visual-review.json",
        "checks": 19,
        "samples": 148,
        "bank_sha256": "CBC612FCFED34A83340CC0896A51CB8F92FA18ECE53BB88F7FE467AB6F6CA1B0"
      }
    },
    "complete": false,
    "visual_accepted": false,
    "manifest": "carry-contact-bank-manifest-v2.json"
  },
  "recovery_runtime_evidence": {
    "cases": {
      "corvo": {
        "video": "carry-cleanup-stream-corvo-v16/native.mp4",
        "evidence": "carry-cleanup-stream-corvo-v16/validation.json",
        "checks": 18,
        "samples": 141,
        "avatar_health_after": 40,
        "impact_blocks": 1,
        "root_follow_passed": false
      },
      "emily": {
        "video": "carry-cleanup-stream-emily-v16/native.mp4",
        "evidence": "carry-cleanup-stream-emily-v16/validation.json",
        "checks": 18,
        "samples": 144,
        "avatar_health_after": 40,
        "impact_blocks": 1,
        "root_follow_passed": false
      }
    },
    "regressions": {
      "friendly-fire-native": "carry-impact-v16-regression\\friendly-fire-native.json",
      "friendly-fire-strikes": "carry-impact-v16-regression\\friendly-fire-strikes.json",
      "native-sword-damage-verification": "carry-impact-v16-regression\\native-sword-damage-verification.json"
    },
    "complete": false
  },
  "pickup_timing_references": {
    "corvo": {
      "video": "native-pickup-geometry-corvo-v1/native.mp4",
      "evidence": "native-pickup-geometry-corvo-v1/geometry-analysis.json",
      "checks": {
        "harness_complete": true,
        "confirmed_preparation": true,
        "confirmed_native_lift": true,
        "confirmed_loop": true,
        "one_corpse_generation": true,
        "first_person_health_preserved": true,
        "capture_at_least_24hz": true
      }
    },
    "emily": {
      "video": "native-pickup-geometry-emily-v1/native.mp4",
      "evidence": "native-pickup-geometry-emily-v1/geometry-analysis.json",
      "checks": {
        "harness_complete": true,
        "confirmed_preparation": true,
        "confirmed_native_lift": true,
        "confirmed_loop": true,
        "one_corpse_generation": true,
        "first_person_health_preserved": true,
        "capture_at_least_24hz": true
      }
    }
  },
  "pickup_stream_continuity": {
    "build": "287E50874C2CCB796844BAE2C1360091EA11E4F1D6AB174FF1B7666D7DC350F4",
    "baseline_failure": "native-pickup-stream-corvo-v16/stream-validation.json",
    "cases": {
      "corvo": {
        "evidence": "native-pickup-stream-corvo-v17/stream-validation.json",
        "video": "native-pickup-stream-corvo-v17/native.mp4",
        "checks": 10,
        "transitions": [
          {
            "ms": 1,
            "ready": true,
            "result": "ready",
            "state": 0,
            "carry": 0
          },
          {
            "ms": 1878,
            "ready": true,
            "result": "ready",
            "state": 14,
            "carry": 1
          },
          {
            "ms": 2813,
            "ready": true,
            "result": "ready",
            "state": 14,
            "carry": 2
          }
        ],
        "actual_capture_hz": 28.25
      },
      "emily": {
        "evidence": "native-pickup-stream-emily-v17/stream-validation.json",
        "video": "native-pickup-stream-emily-v17/native.mp4",
        "checks": 10,
        "transitions": [
          {
            "ms": 1,
            "ready": true,
            "result": "ready",
            "state": 0,
            "carry": 0
          },
          {
            "ms": 1781,
            "ready": true,
            "result": "ready",
            "state": 14,
            "carry": 1
          },
          {
            "ms": 2829,
            "ready": true,
            "result": "ready",
            "state": 14,
            "carry": 2
          }
        ],
        "actual_capture_hz": 27.25
      }
    },
    "confirmed": "Native phase1 Carry45 owns its target before a scene exists. Running0+Carry45+Equip53 is a valid temporary grounded state. The previous reader rejected it and the peer stopped as stale.",
    "fix": "Strict read-only preparation reader plus existing unique authored target resolution. Send grounded presentation until a validated native carry scene begins. Existing 250ms timeout stays unchanged.",
    "remaining": [
      "No authored remote lifting clip connected during native In.",
      "Remote victim pickup and snapshot blending remain separate experiments.",
      "Crouched preparation has not been observed and remains refused."
    ],
    "complete": false
  },
  "snapshot_runtime_evidence": {
    "build": "82E7775D2AE3752887750E2A48AEFCD22D6E978368AAF2B1DD9B0C85240DE99D",
    "cases": {
      "corvo": {
        "video": "carry-snapshot-corvo-v20/native.mp4",
        "evidence": "carry-snapshot-corvo-v20/snapshot-validation.json",
        "root_follow": true,
        "transition_stamps": [
          {
            "restored_ms": 812,
            "attached_ms": 859
          }
        ],
        "checks": 20
      },
      "emily": {
        "video": "carry-snapshot-emily-v20/native.mp4",
        "evidence": "carry-snapshot-emily-v20/snapshot-validation.json",
        "root_follow": true,
        "transition_stamps": [
          {
            "restored_ms": 813,
            "attached_ms": 859
          }
        ],
        "checks": 20
      },
      "cancel": {
        "video": "carry-snapshot-cancel-corvo-v20/native.mp4",
        "evidence": "carry-snapshot-cancel-corvo-v20/snapshot-cancel-validation.json",
        "checks": 13,
        "cleanup_delay_ms": 593,
        "visual_failure": "Body launches upward before settling; mechanical cleanup passes but pose-to-physics handoff is not accepted."
      }
    },
    "confirmed": [
      "Native recovery snapshot inputs can blend a dead Mayhew body without invoking AI recovery or changing health.",
      "Separate pose blending and retarget activation removes the v18 floor dive in both roles.",
      "Engine-thread transition timestamps resolve the coarse status/probe sampling ambiguity.",
      "Early cancellation restores physics and bindings without creating an attachment."
    ],
    "hypotheses": [
      "The native retarget solver reads an unblended clip anchor while final globalPose includes recovery blending."
    ],
    "rejected": "carry-snapshot-corvo-v18/native.mp4",
    "interim": [
      "carry-snapshot-corvo-v19",
      "carry-snapshot-emily-v19"
    ],
    "remaining": [
      "Fix upward body launch on early snapshot cancellation.",
      "Source-timed full-body carrier lift and matching hands/body contact.",
      "Lower peak victim height and improve shoulder settling.",
      "Automatic native pickup and general body identity/authority.",
      "Throws, other releases and complete movement coverage."
    ],
    "complete": false,
    "visual_accepted": false
  },
  "crouched_pickup_stream_continuity": {
    "corvo": {
      "video": "native-pickup-crouched-corvo-v21b/native.mp4",
      "evidence": "native-pickup-crouched-corvo-v21b/stream-validation.json",
      "checks": 10,
      "transitions": [
        {
          "ms": 1,
          "ready": true,
          "result": "ready",
          "state": 0,
          "carry": 0
        },
        {
          "ms": 1856,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 1
        },
        {
          "ms": 2877,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 2
        }
      ]
    },
    "emily": {
      "video": "native-pickup-crouched-emily-v21/native.mp4",
      "evidence": "native-pickup-crouched-emily-v21/stream-validation.json",
      "checks": 10,
      "transitions": [
        {
          "ms": 1,
          "ready": true,
          "result": "ready",
          "state": 0,
          "carry": 0
        },
        {
          "ms": 1783,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 1
        },
        {
          "ms": 2880,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 2
        }
      ]
    }
  },
  "snapshot_cancel_recovery": {
    "corvo": {
      "video": "carry-snapshot-cancel-corvo-v21/native.mp4",
      "evidence": "carry-snapshot-cancel-corvo-v21/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 279,
      "visual_observations": "Corvo falls and settles without the v20 large upward launch; a small pose discontinuity remains.",
      "visual_accepted": false
    },
    "emily": {
      "video": "carry-snapshot-cancel-emily-v21/native.mp4",
      "evidence": "carry-snapshot-cancel-emily-v21/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 293,
      "visual_observations": "Emily still launches upward and sideways at 2.6-2.9 seconds. Mechanical cleanup passes but the visual handoff fails.",
      "visual_accepted": false
    }
  },
  "snapshot_pose_handoff": {
    "carry-snapshot-cancel-emily-v22": {
      "role": "emily",
      "video": "carry-snapshot-cancel-emily-v22/native.mp4",
      "evidence": "carry-snapshot-cancel-emily-v22/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 577,
      "video_sha256": "81AF55C39A5AF26483381C95CCBD6A0E99528EF1D88187E624F0FE78340564A0",
      "actual_capture_hz": 27.272727272727273,
      "visual_observations": "Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result."
    },
    "carry-snapshot-cancel-corvo-v22": {
      "role": "corvo",
      "video": "carry-snapshot-cancel-corvo-v22/native.mp4",
      "evidence": "carry-snapshot-cancel-corvo-v22/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 293,
      "video_sha256": "D13B7902B8ACF1F6561CBE41C19EF57C7F1FD4A503EB594B837C95CEFB3BBFD8",
      "actual_capture_hz": 26.863636363636363,
      "visual_observations": "Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result."
    },
    "carry-snapshot-cancel-emily-v22b": {
      "role": "emily",
      "video": "carry-snapshot-cancel-emily-v22b/native.mp4",
      "evidence": "carry-snapshot-cancel-emily-v22b/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 560,
      "video_sha256": "22F52523007CAC21C32E3E87B538CF12FB567D64CD0FCE7B3C48ECD8E02A1F78",
      "actual_capture_hz": 27.0,
      "visual_observations": "Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result."
    },
    "carry-snapshot-stream-emily-v22": {
      "role": "emily",
      "video": "carry-snapshot-stream-emily-v22/native.mp4",
      "evidence": "carry-snapshot-stream-emily-v22/snapshot-cancel-validation.json",
      "checks": 15,
      "cleanup_delay_ms": 732,
      "video_sha256": "223208D4ADCCE1073B8FAA4F2E9759E4DCAF9865341ED039AC810D0D10E0D7BC",
      "actual_capture_hz": 26.727272727272727,
      "visual_observations": "Source-stream loss occurs later in the blend than direct cancellation, so the body is already raised farther. It returns to ragdoll and settles; restarting both action streams does not recreate the retired attachment."
    }
  },
  "short_lift_screening": {
    "short-lift-corvo-v4": {
      "passed": true,
      "checks": {
        "harness_passed": true,
        "both_views": true,
        "no_review_refusals": true,
        "native_holster_during_motion": true,
        "health_preserved": true,
        "capture_at_least_24hz": true
      },
      "active_samples": 57,
      "actual_capture_hz": 27.625,
      "video_sha256": "B2C0EF61429702C7497E8A9D28F038BB2E1C833E1769FC8D90678886051552A4",
      "clip_sha256": "f1f99756a9b982ce3742a1bd1bea326a665d10fcf1b616fdd09e2a8c3e72109e",
      "visual_review": "short-lift-corvo-v4/visual-review.json"
    },
    "short-lift-emily-v4": {
      "passed": true,
      "checks": {
        "harness_passed": true,
        "both_views": true,
        "no_review_refusals": true,
        "native_holster_during_motion": true,
        "health_preserved": true,
        "capture_at_least_24hz": true
      },
      "active_samples": 54,
      "actual_capture_hz": 27.745302713987474,
      "video_sha256": "1210F37BF0B0A219B5563BD62CAC9C8EA81ACBB1511A12930655687AE4F25CDE",
      "clip_sha256": "0962f247a5b05cabe3dd840e5e76529b3fa565fb0db4f4ce4df405f742d086ad",
      "visual_review": "short-lift-emily-v4/visual-review.json"
    },
    "short-lift-corvo-v6": {
      "passed": true,
      "checks": {
        "harness_passed": true,
        "both_views": true,
        "no_review_refusals": true,
        "native_holster_during_motion": true,
        "health_preserved": true,
        "capture_at_least_24hz": true
      },
      "active_samples": 54,
      "actual_capture_hz": 27.625,
      "video_sha256": "287C5ABEE6783E676412185D09578CA0116D0B64620DA31394D7F6A7A1B7937A",
      "clip_sha256": "c1bae72da8bb9cced442ee347cd3ac3101492bbea0eb010fbd46765d190697ad",
      "visual_review": "short-lift-corvo-v6/visual-review.json"
    },
    "short-lift-emily-v6": {
      "passed": true,
      "checks": {
        "harness_passed": true,
        "both_views": true,
        "no_review_refusals": true,
        "native_holster_during_motion": true,
        "health_preserved": true,
        "capture_at_least_24hz": true
      },
      "active_samples": 57,
      "actual_capture_hz": 27.6875,
      "video_sha256": "5CEEAFE674D2B14F34643A1AC4D6368319E5FA4A21BF5B6D2AE8B12A162A0589",
      "clip_sha256": "3eec122e45793d31a59069f03e2888a04b262f88789f1c487ea410cf09397ab3",
      "visual_review": "short-lift-emily-v6/visual-review.json"
    }
  },
  "native_carrier_pickup": {
    "native-carrier-pickup-corvo-v23": {
      "evidence": "native-carrier-pickup-corvo-v23/validation.json",
      "video": "native-carrier-pickup-corvo-v23/native.mp4",
      "checks": 19,
      "video_sha256": "0EBB3448B5D91D291A0DB360DC250CB2E50ECF47722B26EF00B1021B8D48A47B",
      "visual_review": "native-carrier-pickup-corvo-v23/visual-review.json",
      "scope": "Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim."
    },
    "native-carrier-pickup-emily-v23": {
      "evidence": "native-carrier-pickup-emily-v23/validation.json",
      "video": "native-carrier-pickup-emily-v23/native.mp4",
      "checks": 19,
      "video_sha256": "3DE25B406FFB0B345A91B310106E5640006C37EB50245563B6AB2B3775592946",
      "visual_review": "native-carrier-pickup-emily-v23/visual-review.json",
      "scope": "Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim."
    },
    "native-carrier-pickup-crouched-corvo-v23": {
      "evidence": "native-carrier-pickup-crouched-corvo-v23/validation.json",
      "video": "native-carrier-pickup-crouched-corvo-v23/native.mp4",
      "checks": 14,
      "video_sha256": "61765D3BCDBDF2EFEAE5AFEA6F452E6E72933C9DC5DD76998CB40C26359C0DBE",
      "visual_review": "native-carrier-pickup-crouched-corvo-v23/visual-review.json",
      "scope": "Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim."
    }
  },
  "source_timed_paired_pickup": {
    "native-paired-pickup-corvo-v24": {
      "video": "native-paired-pickup-corvo-v24/native.mp4",
      "evidence": "native-paired-pickup-corvo-v24/paired-validation.json",
      "visual_review": "native-paired-pickup-corvo-v24/visual-review.json",
      "character": "corvo",
      "checks": 31,
      "video_sha256": "6591FF7AEDBA8B1B4B59665CF45F7297E7E6A9A918001DCA1861B5DEA3172836",
      "bank_sha256": "0717D428314DE88CC3768CCFDED386720F28167F6A11B28C6845FFF5B90072F0",
      "contact_pilot": false,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-v24": {
      "video": "native-paired-pickup-emily-v24/native.mp4",
      "evidence": "native-paired-pickup-emily-v24/paired-validation.json",
      "visual_review": "native-paired-pickup-emily-v24/visual-review.json",
      "character": "emily",
      "checks": 31,
      "video_sha256": "E7DA7A1155033AC59283C31342C280EFCA9066671449F9259312CB79BE224409",
      "bank_sha256": "AFB5ED64BD8CE30B4A8C36C201972CE4D49F0C7BE16EE6464BF2D94D964B9668",
      "contact_pilot": false,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-corvo-v24-trace": {
      "video": "native-paired-pickup-corvo-v24-trace/native.mp4",
      "evidence": "native-paired-pickup-corvo-v24-trace/paired-validation.json",
      "visual_review": "native-paired-pickup-corvo-v24-trace/visual-review.json",
      "character": "corvo",
      "checks": 31,
      "video_sha256": "7C64237A4D4404B07AE6A45593C2406B5C6F516A9F50BA59C0E44D488D3B850C",
      "bank_sha256": "0717D428314DE88CC3768CCFDED386720F28167F6A11B28C6845FFF5B90072F0",
      "contact_pilot": false,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-corvo-v24-fast": {
      "video": "native-paired-pickup-corvo-v24-fast/native.mp4",
      "evidence": "native-paired-pickup-corvo-v24-fast/paired-validation.json",
      "visual_review": "native-paired-pickup-corvo-v24-fast/visual-review.json",
      "character": "corvo",
      "checks": 31,
      "video_sha256": "6EAC4AD20EE819BED32343069557392D8EAB63E9EB1EAC0F38EC4638F58EE0BA",
      "bank_sha256": "0717D428314DE88CC3768CCFDED386720F28167F6A11B28C6845FFF5B90072F0",
      "contact_pilot": false,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-v24-fast": {
      "video": "native-paired-pickup-emily-v24-fast/native.mp4",
      "evidence": "native-paired-pickup-emily-v24-fast/paired-validation.json",
      "visual_review": "native-paired-pickup-emily-v24-fast/visual-review.json",
      "character": "emily",
      "checks": 31,
      "video_sha256": "53A1064F93FE5AF3A9855D44D0E9400601A295052B6F6FEAEF64A2A15576566A",
      "bank_sha256": "AFB5ED64BD8CE30B4A8C36C201972CE4D49F0C7BE16EE6464BF2D94D964B9668",
      "contact_pilot": false,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-corvo-contact-v3": {
      "video": "native-paired-pickup-corvo-contact-v3/native.mp4",
      "evidence": "native-paired-pickup-corvo-contact-v3/paired-validation.json",
      "visual_review": "native-paired-pickup-corvo-contact-v3/visual-review.json",
      "character": "corvo",
      "checks": 31,
      "video_sha256": "49CE7B33F6FF08C41591A0CAC1949C6BD80E9CBE0E36F5E235798027F2EFA2B5",
      "bank_sha256": "32FFAA27C56F37BF0948346C36A6441D8BEB4CF60A7A39E6311675995587F2C9",
      "contact_pilot": true,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-cancel-v24c": {
      "video": "native-paired-pickup-emily-cancel-v24c/native.mp4",
      "evidence": "native-paired-pickup-emily-cancel-v24c/cancel-validation.json",
      "visual_review": "native-paired-pickup-emily-cancel-v24c/visual-review.json",
      "character": "emily",
      "checks": 29,
      "video_sha256": "A63D53E9217FEC58F8E33A3C99D140A94E10EF85382BEBFFF5C14320A0E6B71B",
      "bank_sha256": "FD314005ABCC8BA2AD25351575FCD2A942D55E5B62407AECA9F00AC26843EEE9",
      "contact_pilot": true,
      "cancel_before_attachment": true,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-contact-v4": {
      "video": "native-paired-pickup-emily-contact-v4/native.mp4",
      "evidence": "native-paired-pickup-emily-contact-v4/paired-validation.json",
      "visual_review": "native-paired-pickup-emily-contact-v4/visual-review.json",
      "character": "emily",
      "checks": 31,
      "video_sha256": "0F8294633538011351DFFC21E9547E7FC203BA5428142B2187D1FBFCD8CE2C20",
      "bank_sha256": "FD314005ABCC8BA2AD25351575FCD2A942D55E5B62407AECA9F00AC26843EEE9",
      "contact_pilot": true,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-corvo-cancel-v24": {
      "video": "native-paired-pickup-corvo-cancel-v24/native.mp4",
      "evidence": "native-paired-pickup-corvo-cancel-v24/cancel-validation.json",
      "visual_review": "native-paired-pickup-corvo-cancel-v24/visual-review.json",
      "character": "corvo",
      "checks": 29,
      "video_sha256": "263605D7E970DBBCA29CA233DD398BE76FCFFD0940500BBE6EC34FE06F55F546",
      "bank_sha256": "32FFAA27C56F37BF0948346C36A6441D8BEB4CF60A7A39E6311675995587F2C9",
      "contact_pilot": true,
      "cancel_before_attachment": true,
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-corvo-contact-v3-final": {
      "video": "native-paired-pickup-corvo-contact-v3-final/native.mp4",
      "evidence": "native-paired-pickup-corvo-contact-v3-final/paired-validation.json",
      "visual_review": "native-paired-pickup-corvo-contact-v3-final/visual-review.json",
      "character": "corvo",
      "checks": 31,
      "video_sha256": "64730AE2F5328A046A78FC806780EC6BFFEB7985D4A6528BB493F04BDC82AE82",
      "bank_sha256": "32FFAA27C56F37BF0948346C36A6441D8BEB4CF60A7A39E6311675995587F2C9",
      "contact_pilot": true,
      "cancel_before_attachment": false,
      "complete": false,
      "visual_accepted": false
    }
  },
  "supported_pickup_settle": {
    "native-paired-pickup-corvo-settle-v25": {
      "video": "native-paired-pickup-corvo-settle-v25/native.mp4",
      "evidence": "native-paired-pickup-corvo-settle-v25/validation.json",
      "paired_evidence": "native-paired-pickup-corvo-settle-v25/paired-validation.json",
      "visual_review": "native-paired-pickup-corvo-settle-v25/visual-review.json",
      "checks": 34,
      "bank_sha256": "CA5F15FC627DF5E04493F6E6B393E5FDE3BD60DFE18E40FF53F3B6B3DCA73147",
      "video_sha256": "36020167E82FBB6E484FBD4EA2504B87378DC6AADB7C5FD0E6DB58E9BF99564E",
      "role": "corvo",
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-settle-v25": {
      "video": "native-paired-pickup-emily-settle-v25/native.mp4",
      "evidence": "native-paired-pickup-emily-settle-v25/validation.json",
      "paired_evidence": "native-paired-pickup-emily-settle-v25/paired-validation.json",
      "visual_review": "native-paired-pickup-emily-settle-v25/visual-review.json",
      "checks": 34,
      "bank_sha256": "E73A7AB744CF6836CE74B600A3E754375BC0C17A4D24B5F3DA94B6E9ED712FF3",
      "video_sha256": "9FE31159559887D876ECC3021C055A8AB6D5EC4D38162B33FC99332A98D77EF5",
      "role": "emily",
      "complete": false,
      "visual_accepted": false
    }
  },
  "native_sword_visibility": {
    "native-paired-pickup-corvo-visibility-v26": {
      "video": "native-paired-pickup-corvo-visibility-v26/native.mp4",
      "evidence": "native-paired-pickup-corvo-visibility-v26/validation.json",
      "paired_evidence": "native-paired-pickup-corvo-visibility-v26/paired-validation.json",
      "sword_evidence": "native-paired-pickup-corvo-visibility-v26/sword-visibility-validation.json",
      "visual_review": "native-paired-pickup-corvo-visibility-v26/visual-review.json",
      "checks": 44,
      "video_sha256": "5D083A9A68D6E9C70834432C349CF0A5F7D9429F3060CCA04AB65B86D3A20240",
      "role": "corvo",
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-visibility-v26": {
      "video": "native-paired-pickup-emily-visibility-v26/native.mp4",
      "evidence": "native-paired-pickup-emily-visibility-v26/validation.json",
      "paired_evidence": "native-paired-pickup-emily-visibility-v26/paired-validation.json",
      "sword_evidence": "native-paired-pickup-emily-visibility-v26/sword-visibility-validation.json",
      "visual_review": "native-paired-pickup-emily-visibility-v26/visual-review.json",
      "checks": 44,
      "video_sha256": "50D60A38EAC36841F9E712E0881B6DBC9F47CFF2448073F1E5F80F67E56267E3",
      "role": "emily",
      "complete": false,
      "visual_accepted": false
    }
  },
  "continuous_pickup_drop": {
    "native-pickup-walk-drop-corvo-v26": {
      "video": "native-pickup-walk-drop-corvo-v26/native.mp4",
      "character": "corvo",
      "label": "Corvo · Pickup, walk, standing drop · REJECTED physics comparison",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-pickup-walk-drop-corvo-v26/pickup-drop-validation.json",
      "visual_review": "native-pickup-walk-drop-corvo-v26/visual-review.json",
      "proxy_sha256": "A3A1542B9245DBCE5D33FC89952FAD0CC3C90656F8F727C715A6150145130D86",
      "bank_sha256": "CA5F15FC627DF5E04493F6E6B393E5FDE3BD60DFE18E40FF53F3B6B3DCA73147",
      "actual_capture_hz": 26.615384615384617,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": false,
      "checks": 36,
      "video_sha256": "26BAB24567BAE323E4616050582E5F208A871331A15E7446CDC06FC498086A75",
      "final_corpse_distance_from_source_m": 5.350299007326346
    },
    "native-pickup-walk-drop-emily-v26": {
      "video": "native-pickup-walk-drop-emily-v26/native.mp4",
      "character": "emily",
      "label": "Emily · Pickup, walk, standing drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-pickup-walk-drop-emily-v26/pickup-drop-validation.json",
      "visual_review": "native-pickup-walk-drop-emily-v26/visual-review.json",
      "proxy_sha256": "A3A1542B9245DBCE5D33FC89952FAD0CC3C90656F8F727C715A6150145130D86",
      "bank_sha256": "E73A7AB744CF6836CE74B600A3E754375BC0C17A4D24B5F3DA94B6E9ED712FF3",
      "actual_capture_hz": 26.76923076923077,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 36,
      "video_sha256": "61112E2C38960035846790486AC248787D5B5C8FA1A64BC933AC98D66915A8FC",
      "final_corpse_distance_from_source_m": 1.1101840060888357
    },
    "native-pickup-walk-drop-corvo-v27": {
      "video": "native-pickup-walk-drop-corvo-v27/native.mp4",
      "character": "corvo",
      "label": "Corvo · Pickup, walk, standing drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-pickup-walk-drop-corvo-v27/pickup-drop-validation.json",
      "visual_review": "native-pickup-walk-drop-corvo-v27/visual-review.json",
      "proxy_sha256": "80DCD988745C5DA1863D13CE1F8FA67B12A4E15FEA6EDE85E17179B50E8197F0",
      "bank_sha256": "CA5F15FC627DF5E04493F6E6B393E5FDE3BD60DFE18E40FF53F3B6B3DCA73147",
      "actual_capture_hz": 27.37789203084833,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 36,
      "video_sha256": "21EC5A473735803416D1165491E53A78043A925745DEC98FEF52B6784E20C671",
      "final_corpse_distance_from_source_m": 0.4423104409464181
    },
    "native-pickup-walk-drop-emily-v27": {
      "video": "native-pickup-walk-drop-emily-v27/native.mp4",
      "character": "emily",
      "label": "Emily · Pickup, walk, standing drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-pickup-walk-drop-emily-v27/pickup-drop-validation.json",
      "visual_review": "native-pickup-walk-drop-emily-v27/visual-review.json",
      "proxy_sha256": "80DCD988745C5DA1863D13CE1F8FA67B12A4E15FEA6EDE85E17179B50E8197F0",
      "bank_sha256": "E73A7AB744CF6836CE74B600A3E754375BC0C17A4D24B5F3DA94B6E9ED712FF3",
      "actual_capture_hz": 26.692307692307693,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 36,
      "video_sha256": "6334513CE13066DB71C518D1C833094E37B86DF12543C0727DB553D1FA5ADE1E",
      "final_corpse_distance_from_source_m": 0.383828695946562
    },
    "native-pickup-walk-crouch-drop-corvo-v27": {
      "video": "native-pickup-walk-crouch-drop-corvo-v27/native.mp4",
      "character": "corvo",
      "label": "Corvo · Pickup, walk, crouched drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-pickup-walk-crouch-drop-corvo-v27/pickup-drop-validation.json",
      "visual_review": "native-pickup-walk-crouch-drop-corvo-v27/visual-review.json",
      "proxy_sha256": "80DCD988745C5DA1863D13CE1F8FA67B12A4E15FEA6EDE85E17179B50E8197F0",
      "bank_sha256": "CA5F15FC627DF5E04493F6E6B393E5FDE3BD60DFE18E40FF53F3B6B3DCA73147",
      "actual_capture_hz": 26.23076923076923,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 36,
      "video_sha256": "0872B8607536949486F525DAE2368F0FA24ACF6E9907033AF8D5260218336AA3",
      "final_corpse_distance_from_source_m": 1.1574890601868912
    },
    "native-pickup-walk-crouch-drop-emily-v27": {
      "video": "native-pickup-walk-crouch-drop-emily-v27/native.mp4",
      "character": "emily",
      "label": "Emily · Pickup, walk, crouched drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-pickup-walk-crouch-drop-emily-v27/pickup-drop-validation.json",
      "visual_review": "native-pickup-walk-crouch-drop-emily-v27/visual-review.json",
      "proxy_sha256": "80DCD988745C5DA1863D13CE1F8FA67B12A4E15FEA6EDE85E17179B50E8197F0",
      "bank_sha256": "E73A7AB744CF6836CE74B600A3E754375BC0C17A4D24B5F3DA94B6E9ED712FF3",
      "actual_capture_hz": 27.153846153846153,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 36,
      "video_sha256": "A67B5953634954C4D747BA161328BD4177C2A1A9579D62E4245647CD6175510A",
      "final_corpse_distance_from_source_m": 1.1472188652167525
    },
    "native-crouched-pickup-drop-corvo-v28": {
      "video": "native-crouched-pickup-drop-corvo-v28/native.mp4",
      "character": "corvo",
      "label": "Corvo · Crouched pickup, walk, crouched drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-drop-corvo-v28/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-drop-corvo-v28/visual-review.json",
      "proxy_sha256": "84112870B0B30DD979F124F91F3075D420F2D140197FAD678AF55302609F80F9",
      "bank_sha256": "8C9244AF6B2227FB339D2D228F68E8B267A9166E85202B97D7E41ADD9333E251",
      "actual_capture_hz": 27.076923076923077,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 39,
      "video_sha256": "AC2D8B7131CD5CBA2A4D0D0AE995844BD0B20BE4368B92A9F06CD274D86356D7",
      "final_corpse_distance_from_source_m": 1.3611247668022481
    },
    "native-crouched-pickup-drop-emily-v28": {
      "video": "native-crouched-pickup-drop-emily-v28/native.mp4",
      "character": "emily",
      "label": "Emily · Crouched pickup, walk, crouched drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-drop-emily-v28/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-drop-emily-v28/visual-review.json",
      "proxy_sha256": "84112870B0B30DD979F124F91F3075D420F2D140197FAD678AF55302609F80F9",
      "bank_sha256": "FCACB3AB0B1C1B3F3F22669B65DCBC0E79C882BB3ED346C3031E1AA07786FFF8",
      "actual_capture_hz": 26.692307692307693,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 39,
      "video_sha256": "BBCBF15151858EDF55B913ADEC30B6CD487594F05C8E270BBEB9992E974D08FD",
      "final_corpse_distance_from_source_m": 1.1935867954214219
    },
    "native-crouched-pickup-drop-corvo-v28-contact-v2": {
      "video": "native-crouched-pickup-drop-corvo-v28-contact-v2/native.mp4",
      "character": "corvo",
      "label": "Corvo · Crouched pickup, walk, crouched drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-drop-corvo-v28-contact-v2/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-drop-corvo-v28-contact-v2/visual-review.json",
      "proxy_sha256": "84112870B0B30DD979F124F91F3075D420F2D140197FAD678AF55302609F80F9",
      "bank_sha256": "44C366C774D59CB7CE5C36D0A2E91F90989E524EB5212C9378B716558389A576",
      "actual_capture_hz": 27.23076923076923,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": null,
      "checks": 39,
      "video_sha256": "08B6E4879F2BAC494C1F496B92FF27FA1345CBE4325AF82753B89F8A9D63961F",
      "final_corpse_distance_from_source_m": 1.2949476255652097
    },
    "native-crouched-pickup-drop-emily-v28-contact-v2": {
      "video": "native-crouched-pickup-drop-emily-v28-contact-v2/native.mp4",
      "character": "emily",
      "label": "Emily · Crouched pickup, walk, crouched drop · REJECTED rotation comparison",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-drop-emily-v28-contact-v2/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-drop-emily-v28-contact-v2/visual-review.json",
      "proxy_sha256": "84112870B0B30DD979F124F91F3075D420F2D140197FAD678AF55302609F80F9",
      "bank_sha256": "B375095065972EAFF70729D98A93196A5A1E9C29A7B099506D9CD95F5C94525B",
      "actual_capture_hz": 26.91516709511568,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": false,
      "orientation_evidence": "native-crouched-pickup-drop-emily-v28-contact-v2/orientation-validation.json",
      "checks": 43,
      "video_sha256": "74C3EE7E922E6ECA03EA0569920C0C366212DA7F29B157EA5446B3124DAEE691",
      "final_corpse_distance_from_source_m": 1.3461638171083872
    },
    "native-stand-crouch-carry-corvo-v28-contact-v2": {
      "video": "native-stand-crouch-carry-corvo-v28-contact-v2/native.mp4",
      "character": "corvo",
      "label": "Corvo · Pickup, walk, crouched drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-stand-crouch-carry-corvo-v28-contact-v2/pickup-drop-validation.json",
      "visual_review": "native-stand-crouch-carry-corvo-v28-contact-v2/visual-review.json",
      "proxy_sha256": "84112870B0B30DD979F124F91F3075D420F2D140197FAD678AF55302609F80F9",
      "bank_sha256": "44C366C774D59CB7CE5C36D0A2E91F90989E524EB5212C9378B716558389A576",
      "actual_capture_hz": 26.39386189258312,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": "native-stand-crouch-carry-corvo-v28-contact-v2/orientation-validation.json",
      "checks": 40,
      "video_sha256": "BCC026BDA0F54EABEB99184B945DA56643158112E13842DCE22D1141E13AB19E",
      "final_corpse_distance_from_source_m": 1.1476617694808433
    },
    "native-crouched-pickup-corvo-orientation-v29": {
      "video": "native-crouched-pickup-corvo-orientation-v29/native.mp4",
      "character": "corvo",
      "label": "Corvo · Crouched pickup, walk, crouched drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-corvo-orientation-v29/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-corvo-orientation-v29/visual-review.json",
      "proxy_sha256": "B700CFBF950E994C77C60A4BBD5B46A99F6C6C4311FC282AECE7E1D0B6F7F100",
      "bank_sha256": "44C366C774D59CB7CE5C36D0A2E91F90989E524EB5212C9378B716558389A576",
      "actual_capture_hz": 27.23076923076923,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": "native-crouched-pickup-corvo-orientation-v29/orientation-validation.json",
      "checks": 43,
      "video_sha256": "1A5A545009E7613169E6427BBC9D8563F060CD89C6FE4D3B8752C5649615781F",
      "final_corpse_distance_from_source_m": 0.7739893558008235
    },
    "native-crouched-pickup-emily-orientation-v29": {
      "video": "native-crouched-pickup-emily-orientation-v29/native.mp4",
      "character": "emily",
      "label": "Emily · Crouched pickup, walk, crouched drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-emily-orientation-v29/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-emily-orientation-v29/visual-review.json",
      "proxy_sha256": "B700CFBF950E994C77C60A4BBD5B46A99F6C6C4311FC282AECE7E1D0B6F7F100",
      "bank_sha256": "B375095065972EAFF70729D98A93196A5A1E9C29A7B099506D9CD95F5C94525B",
      "actual_capture_hz": 27.307692307692307,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": "native-crouched-pickup-emily-orientation-v29/orientation-validation.json",
      "checks": 43,
      "video_sha256": "713E166BF33F5EF1B8E08C7D03129143AC7954AC39907EF2CD9C7F1EDD5ADEB6",
      "final_corpse_distance_from_source_m": 0.7832023782544886
    },
    "native-crouched-pickup-emily-v29-contact-v3": {
      "video": "native-crouched-pickup-emily-v29-contact-v3/native.mp4",
      "character": "emily",
      "label": "Emily · Crouched pickup, walk, crouched drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-pickup-emily-v29-contact-v3/pickup-drop-validation.json",
      "visual_review": "native-crouched-pickup-emily-v29-contact-v3/visual-review.json",
      "proxy_sha256": "B700CFBF950E994C77C60A4BBD5B46A99F6C6C4311FC282AECE7E1D0B6F7F100",
      "bank_sha256": "31035CEC92EA7678082D3D085F71F81C84C5D7FD1BBA64F3B012867EAAEB7DEB",
      "actual_capture_hz": 27.333333333333332,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": "native-crouched-pickup-emily-v29-contact-v3/orientation-validation.json",
      "contact_evidence": "native-crouched-pickup-emily-v29-contact-v3/load-contact-validation.json",
      "checks": 50,
      "video_sha256": "92EA2EFBC2F1D98D91043CBBFADF527BEED2382919FE5133C7E932B8578FE763",
      "final_corpse_distance_from_source_m": 0.9270412019646423
    },
    "native-crouched-to-standing-carry-corvo-v29-contact-v3": {
      "video": "native-crouched-to-standing-carry-corvo-v29-contact-v3/native.mp4",
      "character": "corvo",
      "label": "Corvo · Crouched pickup, walk, standing drop · REJECTED load/contact comparison",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-to-standing-carry-corvo-v29-contact-v3/pickup-drop-validation.json",
      "visual_review": "native-crouched-to-standing-carry-corvo-v29-contact-v3/visual-review.json",
      "proxy_sha256": "B700CFBF950E994C77C60A4BBD5B46A99F6C6C4311FC282AECE7E1D0B6F7F100",
      "bank_sha256": "752B441F3BB5C137E3506ED82B11D6AB9738196DF9FD151E9A73F0818908A677",
      "actual_capture_hz": 26.993318485523385,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": false,
      "orientation_evidence": "native-crouched-to-standing-carry-corvo-v29-contact-v3/orientation-validation.json",
      "contact_evidence": "native-crouched-to-standing-carry-corvo-v29-contact-v3/load-contact-validation.json",
      "checks": 50,
      "video_sha256": "A1A316F69ED07E86EEC426930BFB115ADF46F5F85F1ADABCBEE2921A6AEF6512",
      "final_corpse_distance_from_source_m": 0.9505026125194902
    },
    "native-standing-to-crouched-carry-emily-v29-contact-v3": {
      "video": "native-standing-to-crouched-carry-emily-v29-contact-v3/native.mp4",
      "character": "emily",
      "label": "Emily · Pickup, walk, crouched drop · REJECTED load/contact comparison",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-standing-to-crouched-carry-emily-v29-contact-v3/pickup-drop-validation.json",
      "visual_review": "native-standing-to-crouched-carry-emily-v29-contact-v3/visual-review.json",
      "proxy_sha256": "B700CFBF950E994C77C60A4BBD5B46A99F6C6C4311FC282AECE7E1D0B6F7F100",
      "bank_sha256": "31035CEC92EA7678082D3D085F71F81C84C5D7FD1BBA64F3B012867EAAEB7DEB",
      "actual_capture_hz": 27.32739420935412,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": false,
      "orientation_evidence": "native-standing-to-crouched-carry-emily-v29-contact-v3/orientation-validation.json",
      "contact_evidence": "native-standing-to-crouched-carry-emily-v29-contact-v3/load-contact-validation.json",
      "checks": 48,
      "video_sha256": "0FBFCE55897A91F748DA54C2C9FFB042E091244DD641EABF2AB588BBECBCF66B",
      "final_corpse_distance_from_source_m": 1.238906497235612
    },
    "native-crouched-to-standing-carry-corvo-v30-sampled": {
      "video": "native-crouched-to-standing-carry-corvo-v30-sampled/native.mp4",
      "character": "corvo",
      "label": "Corvo · Crouched pickup, walk, standing drop",
      "alias": "crouched_carry_pickup",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-crouched-to-standing-carry-corvo-v30-sampled/pickup-drop-validation.json",
      "visual_review": "native-crouched-to-standing-carry-corvo-v30-sampled/visual-review.json",
      "proxy_sha256": "891A60B35E162DEE695B7D9C0E7ED67EC5F1CE5C9A05DA4BCD932D4989BD7727",
      "bank_sha256": "752B441F3BB5C137E3506ED82B11D6AB9738196DF9FD151E9A73F0818908A677",
      "actual_capture_hz": 26.407982261640797,
      "scope": "One continuous native-input crouched pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": "native-crouched-to-standing-carry-corvo-v30-sampled/orientation-validation.json",
      "contact_evidence": "native-crouched-to-standing-carry-corvo-v30-sampled/load-contact-validation.json",
      "checks": 51,
      "video_sha256": "700084FBB03A124B9BACF842F9453494E48156354170A03501D62DDCA417D484",
      "final_corpse_distance_from_source_m": 0.46820169312712395
    },
    "native-standing-to-crouched-carry-emily-v30": {
      "video": "native-standing-to-crouched-carry-emily-v30/native.mp4",
      "character": "emily",
      "label": "Emily · Pickup, walk, crouched drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-standing-to-crouched-carry-emily-v30/pickup-drop-validation.json",
      "visual_review": "native-standing-to-crouched-carry-emily-v30/visual-review.json",
      "proxy_sha256": "891A60B35E162DEE695B7D9C0E7ED67EC5F1CE5C9A05DA4BCD932D4989BD7727",
      "bank_sha256": "31035CEC92EA7678082D3D085F71F81C84C5D7FD1BBA64F3B012867EAAEB7DEB",
      "actual_capture_hz": 27.066666666666666,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": "native-standing-to-crouched-carry-emily-v30/orientation-validation.json",
      "contact_evidence": "native-standing-to-crouched-carry-emily-v30/load-contact-validation.json",
      "checks": 48,
      "video_sha256": "42681D9A247508D90B01C7B37D63B450191B2FF4AFA2FD3EFED40D92BDC09031",
      "final_corpse_distance_from_source_m": 0.7964335283803816
    },
    "native-low-drop-corvo-regression-v38": {
      "video": "native-low-drop-corvo-regression-v38/native.mp4",
      "character": "corvo",
      "label": "Corvo · Pickup, walk, standing drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-low-drop-corvo-regression-v38/pickup-drop-validation.json",
      "visual_review": "native-low-drop-corvo-regression-v38/visual-review.json",
      "proxy_sha256": "A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 27.76923076923077,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": null,
      "contact_evidence": null,
      "checks": 37,
      "video_sha256": "0C6CAC62AC9B8FC249159A1FC8948F8E2D4888897837A3EE602383C6E310C5D9",
      "final_corpse_distance_from_source_m": 1.1101503908040864
    },
    "native-low-drop-emily-regression-v38": {
      "video": "native-low-drop-emily-regression-v38/native.mp4",
      "character": "emily",
      "label": "Emily · Pickup, walk, crouched drop",
      "alias": "carry_pickup_supported_settle",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-low-drop-emily-regression-v38/pickup-drop-validation.json",
      "visual_review": "native-low-drop-emily-regression-v38/visual-review.json",
      "proxy_sha256": "A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D",
      "bank_sha256": "D134A0CBD680E084076FF95A7232C5294A1401B0D2CB6CF779FE3AEE80F4DAFA",
      "actual_capture_hz": 27.307692307692307,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "orientation_evidence": null,
      "contact_evidence": null,
      "checks": 37,
      "video_sha256": "87E65F19DAB1E0C6A86C12D24BEC30B86C64FDBB47085B8DB1F98D84846A1D0D",
      "final_corpse_distance_from_source_m": 0.7982109064450928
    }
  },
  "native_high_throw": {
    "native-high-throw-emily-v31": {
      "video": "native-high-throw-emily-v31/native.mp4",
      "character": "emily",
      "label": "Emily · Native pickup, walk and high throw · REJECTED throw comparison",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-emily-v31/high-throw-validation.json",
      "visual_review": "native-high-throw-emily-v31/visual-review.json",
      "proxy_sha256": "59BB03FE23AB8BDE5B363A0CF58658749EBA6DD928BE582C98B7131743963426",
      "bank_sha256": "6612CAEA0DD5D545B3D2ADFF18716DBF3EE691DEB0BC7397A6254D88C561C0BA",
      "actual_capture_hz": 27.2,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": false,
      "checks": 21,
      "passed_checks": 18,
      "video_sha256": "B10A101F8812BD662E53C4956A0E1B32B58372BED5833E2749F3F26BEC895CB3"
    },
    "native-high-throw-emily-v32": {
      "video": "native-high-throw-emily-v32/native.mp4",
      "character": "emily",
      "label": "Emily · Native pickup, walk and high throw · REJECTED throw comparison",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-emily-v32/high-throw-validation.json",
      "visual_review": "native-high-throw-emily-v32/visual-review.json",
      "proxy_sha256": "46DCE62AB9B8FEDA9457C6760F3096E494307ED9DBBAD5FD398D4EDB1FFF3217",
      "bank_sha256": "6612CAEA0DD5D545B3D2ADFF18716DBF3EE691DEB0BC7397A6254D88C561C0BA",
      "actual_capture_hz": 27.4,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": false,
      "checks": 21,
      "passed_checks": 18,
      "video_sha256": "E8BA5F8A6A2966056227A2ACBE4BEE034A46E503948026CA7224B0FDFEDAD7BD"
    },
    "native-high-throw-corvo-v32": {
      "video": "native-high-throw-corvo-v32/native.mp4",
      "character": "corvo",
      "label": "Corvo · Native pickup, walk and high throw · REJECTED throw comparison",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-corvo-v32/high-throw-validation.json",
      "visual_review": "native-high-throw-corvo-v32/visual-review.json",
      "proxy_sha256": "46DCE62AB9B8FEDA9457C6760F3096E494307ED9DBBAD5FD398D4EDB1FFF3217",
      "bank_sha256": "9097431EF49B0EB70001BF8B33F02A59E0FB438F228710CD03ECC86DA5E1BBCD",
      "actual_capture_hz": 27.266666666666666,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": false,
      "checks": 21,
      "passed_checks": 18,
      "video_sha256": "8714B737EB0D972278BDD431B5F67C5AC334A3DEA2C90CE087BF3D5DC6222C8B"
    },
    "native-high-throw-corvo-v33": {
      "video": "native-high-throw-corvo-v33/native.mp4",
      "character": "corvo",
      "label": "Corvo · Native pickup, walk and high throw · scoped checks passed; visual review pending",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-corvo-v33/high-throw-validation.json",
      "visual_review": "native-high-throw-corvo-v33/visual-review.json",
      "proxy_sha256": "48A17FA29C9BF0DE418C06F1F5A9BCEBF73E9B30D3F000101C3D4C98A114C520",
      "bank_sha256": "9097431EF49B0EB70001BF8B33F02A59E0FB438F228710CD03ECC86DA5E1BBCD",
      "actual_capture_hz": 26.933333333333334,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": true,
      "checks": 21,
      "passed_checks": 21,
      "video_sha256": "E2D12711227A53EB9D43B680CF14E5EAF4899656A041D4D59C3DAA27E89C2197"
    },
    "native-high-throw-emily-v33": {
      "video": "native-high-throw-emily-v33/native.mp4",
      "character": "emily",
      "label": "Emily · Native pickup, walk and high throw · scoped checks passed; visual review pending",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-emily-v33/high-throw-validation.json",
      "visual_review": "native-high-throw-emily-v33/visual-review.json",
      "proxy_sha256": "48A17FA29C9BF0DE418C06F1F5A9BCEBF73E9B30D3F000101C3D4C98A114C520",
      "bank_sha256": "6612CAEA0DD5D545B3D2ADFF18716DBF3EE691DEB0BC7397A6254D88C561C0BA",
      "actual_capture_hz": 27.333333333333332,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": true,
      "checks": 21,
      "passed_checks": 21,
      "video_sha256": "71D5EFF4D1AF934C6C7F5A214408DA781A7A900E3385040E96CA8FAFE57761F0"
    },
    "native-high-throw-corvo-open-v33-b": {
      "video": "native-high-throw-corvo-open-v33-b/native.mp4",
      "character": "corvo",
      "label": "Corvo · Native pickup, walk and high throw · REJECTED throw comparison",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-corvo-open-v33-b/high-throw-validation.json",
      "visual_review": "native-high-throw-corvo-open-v33-b/visual-review.json",
      "proxy_sha256": "48A17FA29C9BF0DE418C06F1F5A9BCEBF73E9B30D3F000101C3D4C98A114C520",
      "bank_sha256": "9097431EF49B0EB70001BF8B33F02A59E0FB438F228710CD03ECC86DA5E1BBCD",
      "actual_capture_hz": 27.466666666666665,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": false,
      "checks": 21,
      "passed_checks": 13,
      "video_sha256": "D591D5C4F2F098ED7C575A4888AFCFA6246C1358B46A44FFD296F4F41F688972"
    },
    "native-high-throw-emily-contact-v34": {
      "video": "native-high-throw-emily-contact-v34/native.mp4",
      "character": "emily",
      "label": "Emily · Native pickup, walk and high throw · REJECTED throw comparison",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-emily-contact-v34/high-throw-validation.json",
      "visual_review": "native-high-throw-emily-contact-v34/visual-review.json",
      "proxy_sha256": "48A17FA29C9BF0DE418C06F1F5A9BCEBF73E9B30D3F000101C3D4C98A114C520",
      "bank_sha256": "6612CAEA0DD5D545B3D2ADFF18716DBF3EE691DEB0BC7397A6254D88C561C0BA",
      "actual_capture_hz": 27.466666666666665,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": false,
      "checks": 21,
      "passed_checks": 20,
      "video_sha256": "C22099D53F69F968F5E555321DD59C36AEBFF8205402E413AC5576FEE3171047"
    },
    "native-high-throw-corvo-v34": {
      "video": "native-high-throw-corvo-v34/native.mp4",
      "character": "corvo",
      "label": "Corvo · Native pickup, walk and high throw · scoped checks passed; visual review pending",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-corvo-v34/high-throw-validation.json",
      "visual_review": "native-high-throw-corvo-v34/visual-review.json",
      "proxy_sha256": "67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85",
      "bank_sha256": "9097431EF49B0EB70001BF8B33F02A59E0FB438F228710CD03ECC86DA5E1BBCD",
      "actual_capture_hz": 26.8,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": true,
      "checks": 21,
      "passed_checks": 21,
      "video_sha256": "93A7B124059D28A51400D7535E19FCD90E5D5202F2D96A61AFB4208E05FBD372"
    },
    "native-high-throw-emily-v34": {
      "video": "native-high-throw-emily-v34/native.mp4",
      "character": "emily",
      "label": "Emily · Native pickup, walk and high throw · scoped checks passed; visual review pending",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-emily-v34/high-throw-validation.json",
      "visual_review": "native-high-throw-emily-v34/visual-review.json",
      "proxy_sha256": "67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85",
      "bank_sha256": "6612CAEA0DD5D545B3D2ADFF18716DBF3EE691DEB0BC7397A6254D88C561C0BA",
      "actual_capture_hz": 27.266666666666666,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": true,
      "checks": 21,
      "passed_checks": 21,
      "video_sha256": "F3F49304BF863BA5F46A82EE0623D8FF3A85E12AE3A088E5E82226260F46B54F"
    },
    "native-high-throw-corvo-regression-v38": {
      "video": "native-high-throw-corvo-regression-v38/native.mp4",
      "character": "corvo",
      "label": "Corvo · Native pickup, walk and high throw · scoped checks passed; visual review pending",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-corvo-regression-v38/high-throw-validation.json",
      "visual_review": "native-high-throw-corvo-regression-v38/visual-review.json",
      "proxy_sha256": "A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 26.14190687361419,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": true,
      "checks": 21,
      "passed_checks": 21,
      "video_sha256": "8C36E940E93F36FE01F2A4007FDCE76151E06C8E96FB3D8DE863057EF82B559B"
    },
    "native-high-throw-emily-regression-v38": {
      "video": "native-high-throw-emily-regression-v38/native.mp4",
      "character": "emily",
      "label": "Emily · Native pickup, walk and high throw · scoped checks passed; visual review pending",
      "alias": "carry_body_drop_high_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-high-throw-emily-regression-v38/high-throw-validation.json",
      "visual_review": "native-high-throw-emily-regression-v38/visual-review.json",
      "proxy_sha256": "A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D",
      "bank_sha256": "D134A0CBD680E084076FF95A7232C5294A1401B0D2CB6CF779FE3AEE80F4DAFA",
      "actual_capture_hz": 27.193763919821826,
      "scope": "Standing native X pickup, short carry walk, native RT high throw. Bounded same-PC Mayhew fixture; source-driven carrier clip, victim-only scene, one queued native impulse. Wall proximity, arbitrary corpses, pitched throws, world authority and mesh contacts are not accepted by these numerical checks.",
      "checks_passed": true,
      "checks": 21,
      "passed_checks": 21,
      "video_sha256": "2368524CE463865A9568D3306BEE21B84B8EA23F235EEBE0AD7AE100095D9CB6"
    }
  },
  "high_throw_build_low_drop_regression": {
    "native-low-drop-corvo-regression-v34": {
      "video": "native-low-drop-corvo-regression-v34/native.mp4",
      "character": "corvo",
      "label": "Corvo · Ordinary low-drop regression on high-throw build",
      "alias": "carry_body_drop_low_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-low-drop-corvo-regression-v34/pickup-drop-validation.json",
      "visual_review": "native-low-drop-corvo-regression-v34/visual-review.json",
      "proxy_sha256": "67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85",
      "bank_sha256": "9097431EF49B0EB70001BF8B33F02A59E0FB438F228710CD03ECC86DA5E1BBCD",
      "actual_capture_hz": 26.792873051224944,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 37,
      "video_sha256": "1B1A35FC565209D8A46BD46BD291F68E9E1DAC707596C2338F9840F4BBF1C603",
      "final_corpse_distance_from_source_m": 1.0831953472591411
    },
    "native-low-drop-emily-regression-v34": {
      "video": "native-low-drop-emily-regression-v34/native.mp4",
      "character": "emily",
      "label": "Emily · Ordinary low-drop regression on high-throw build",
      "alias": "carry_body_drop_low_sequence",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-low-drop-emily-regression-v34/pickup-drop-validation.json",
      "visual_review": "native-low-drop-emily-regression-v34/visual-review.json",
      "proxy_sha256": "67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85",
      "bank_sha256": "6612CAEA0DD5D545B3D2ADFF18716DBF3EE691DEB0BC7397A6254D88C561C0BA",
      "actual_capture_hz": 27.466666666666665,
      "scope": "One continuous native-input standing pickup, brief carry walk, optional crouch, native low drop, and physics handoff for the unique Mayhew fixture. No arbitrary-corpse or throw coverage.",
      "checks_passed": true,
      "checks": 37,
      "video_sha256": "CCCACEB22FC9B321D8111C9B1BE59D0764C26E26BD30083A55F2DD289193A35F",
      "final_corpse_distance_from_source_m": 0.44207920346025387
    }
  },
  "native_compact_release": {
    "native-fetal-release-corvo-reader-v35": {
      "video": "native-fetal-release-corvo-reader-v35/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · REJECTED comparison",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-reader-v35/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-reader-v35/visual-review.json",
      "proxy_sha256": "67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85",
      "bank_sha256": "9097431EF49B0EB70001BF8B33F02A59E0FB438F228710CD03ECC86DA5E1BBCD",
      "actual_capture_hz": 27.266666666666666,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": false,
      "checks": 19,
      "passed_checks": 16,
      "video_sha256": "3D6B34D982E701A9E7334D08724DCCE74A4D336B194585020FD64D4FB1C4DF6D"
    },
    "native-fetal-release-corvo-v35": {
      "video": "native-fetal-release-corvo-v35/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · REJECTED comparison",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-v35/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-v35/visual-review.json",
      "proxy_sha256": "DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 27.339246119733925,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": false,
      "checks": 19,
      "passed_checks": 15,
      "video_sha256": "A6AFA51D32C6C1AE034C395EF5515FA20B56E9F04BB43FD3659AC0432C6E009D"
    },
    "native-fetal-release-corvo-turn-v35": {
      "video": "native-fetal-release-corvo-turn-v35/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-turn-v35/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-turn-v35/visual-review.json",
      "proxy_sha256": "DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 27.527839643652563,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "A7EAD3B2C48E2FA117153CBCC9B15B376501C02AE201EB629C546E92001DAF79"
    },
    "native-fetal-release-emily-turn-v35": {
      "video": "native-fetal-release-emily-turn-v35/native.mp4",
      "character": "emily",
      "label": "Emily · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-emily-turn-v35/fetal-release-validation.json",
      "visual_review": "native-fetal-release-emily-turn-v35/visual-review.json",
      "proxy_sha256": "DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589",
      "bank_sha256": "D134A0CBD680E084076FF95A7232C5294A1401B0D2CB6CF779FE3AEE80F4DAFA",
      "actual_capture_hz": 27.0,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "630197E8984EB7C6610A89896CD536693E8F6ECAC25DA12A3CCCE65ACE3E8591"
    },
    "native-fetal-release-corvo-framed-v35": {
      "video": "native-fetal-release-corvo-framed-v35/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-framed-v35/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-framed-v35/visual-review.json",
      "proxy_sha256": "DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 26.0,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "431DECC09B3EC6B0CD88DBD7A954645CE3323C0032A89E19339359882C490B13"
    },
    "native-fetal-release-emily-framed-v35": {
      "video": "native-fetal-release-emily-framed-v35/native.mp4",
      "character": "emily",
      "label": "Emily · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-emily-framed-v35/fetal-release-validation.json",
      "visual_review": "native-fetal-release-emily-framed-v35/visual-review.json",
      "proxy_sha256": "DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589",
      "bank_sha256": "D134A0CBD680E084076FF95A7232C5294A1401B0D2CB6CF779FE3AEE80F4DAFA",
      "actual_capture_hz": 27.06013363028953,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "2CFD4765D6E9862347B4757CEA6EF17B0B5B7DFDD950879694A05043ADBDAD88"
    },
    "native-fetal-release-corvo-blend-v36": {
      "video": "native-fetal-release-corvo-blend-v36/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · REJECTED comparison",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-blend-v36/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-blend-v36/visual-review.json",
      "proxy_sha256": "A726151D2416C6A0B83F0206C07FC6FDEC925544DF20425856BC229081591177",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 26.92650334075724,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": false,
      "checks": 19,
      "passed_checks": 17,
      "video_sha256": "EEBF9B945A5856C7606BFAC369BAEE1C513A31C79C5939BE1C5CF166B68389CD"
    },
    "native-fetal-release-corvo-order-v37": {
      "video": "native-fetal-release-corvo-order-v37/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-order-v37/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-order-v37/visual-review.json",
      "proxy_sha256": "0D16B29C6757132107320EF10706E0592992B6726394245CAA39A9144399190F",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 27.4,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "9A1A706D828BEA2AADF4878A6290D06FF98549C2A0B6DA967C7ED7A6A75D97F9"
    },
    "native-fetal-release-corvo-hold-v38": {
      "video": "native-fetal-release-corvo-hold-v38/native.mp4",
      "character": "corvo",
      "label": "Corvo · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-corvo-hold-v38/fetal-release-validation.json",
      "visual_review": "native-fetal-release-corvo-hold-v38/visual-review.json",
      "proxy_sha256": "A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D",
      "bank_sha256": "5EBB1A4B6785E286FDED9D60C3DE226C294792C63B5B24BA9D025DD159BF5CC0",
      "actual_capture_hz": 27.260579064587972,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "3C3C6FDE2BBC4BDA27CF10DF2B93EBECB50C32F638DA97924AC28DFB743E9EC7"
    },
    "native-fetal-release-emily-hold-v38": {
      "video": "native-fetal-release-emily-hold-v38/native.mp4",
      "character": "emily",
      "label": "Emily · Contextual compact release · scoped checks pass; visual issues remain",
      "alias": "carry_fetal_release_runtime",
      "requirements": [
        "carry_body_lift_idle_move_drop_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-fetal-release-emily-hold-v38/fetal-release-validation.json",
      "visual_review": "native-fetal-release-emily-hold-v38/visual-review.json",
      "proxy_sha256": "A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D",
      "bank_sha256": "D134A0CBD680E084076FF95A7232C5294A1401B0D2CB6CF779FE3AEE80F4DAFA",
      "actual_capture_hz": 27.466666666666665,
      "scope": "Standing variant5 compact fallback, no high-throw impulse. Mayhew-only isolated presentation. Handoff166ms is a candidate from victim clip duration, not an explicit native ragdoll event. Contact, arbitrary bodies and world authority remain unaccepted.",
      "checks_passed": true,
      "checks": 19,
      "passed_checks": 19,
      "video_sha256": "9F6650B55A1EEE00347CB1075EDCC88883425CF13DA173E80D58F88821C4E1D5"
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [in_game_recordings: native-carry-corvo-reader-v2/native.mp4](<../../../../work/natural-moveset/native-carry-corvo-reader-v2/native.mp4>)
- [in_game_recordings: native-carry-emily-reader-v2/native.mp4](<../../../../work/natural-moveset/native-carry-emily-reader-v2/native.mp4>)
- [in_game_recordings: native-carry-carrier-corvo-v3/native.mp4](<../../../../work/natural-moveset/native-carry-carrier-corvo-v3/native.mp4>)
- [in_game_recordings: native-carry-carrier-emily-v3/native.mp4](<../../../../work/natural-moveset/native-carry-carrier-emily-v3/native.mp4>)
- [in_game_recordings: carry-victim-native-corvo-v8/native.mp4](<../../../../work/natural-moveset/carry-victim-native-corvo-v8/native.mp4>)
- [in_game_recordings: carry-victim-native-emily-v8/native.mp4](<../../../../work/natural-moveset/carry-victim-native-emily-v8/native.mp4>)
- [in_game_recordings: carry-victim-native-corvo-v13/native.mp4](<../../../../work/natural-moveset/carry-victim-native-corvo-v13/native.mp4>)
- [in_game_recordings: carry-victim-native-corvo-crouched-v13/native.mp4](<../../../../work/natural-moveset/carry-victim-native-corvo-crouched-v13/native.mp4>)
- [in_game_recordings: carry-victim-native-emily-v13/native.mp4](<../../../../work/natural-moveset/carry-victim-native-emily-v13/native.mp4>)
- [in_game_recordings: carry-victim-native-emily-crouched-v13/native.mp4](<../../../../work/natural-moveset/carry-victim-native-emily-crouched-v13/native.mp4>)
- [in_game_recordings: carry-contact-corvo-crouched-v2/native.mp4](<../../../../work/natural-moveset/carry-contact-corvo-crouched-v2/native.mp4>)
- [in_game_recordings: carry-contact-emily-crouched-v2/native.mp4](<../../../../work/natural-moveset/carry-contact-emily-crouched-v2/native.mp4>)
- [in_game_recordings: carry-cleanup-stream-corvo-v16/native.mp4](<../../../../work/natural-moveset/carry-cleanup-stream-corvo-v16/native.mp4>)
- [in_game_recordings: native-pickup-geometry-corvo-v1/native.mp4](<../../../../work/natural-moveset/native-pickup-geometry-corvo-v1/native.mp4>)
- [in_game_recordings: carry-cleanup-stream-emily-v16/native.mp4](<../../../../work/natural-moveset/carry-cleanup-stream-emily-v16/native.mp4>)
- [in_game_recordings: native-pickup-geometry-emily-v1/native.mp4](<../../../../work/natural-moveset/native-pickup-geometry-emily-v1/native.mp4>)
- [in_game_recordings: native-pickup-stream-corvo-v17/native.mp4](<../../../../work/natural-moveset/native-pickup-stream-corvo-v17/native.mp4>)
- [in_game_recordings: native-pickup-stream-emily-v17/native.mp4](<../../../../work/natural-moveset/native-pickup-stream-emily-v17/native.mp4>)
- [in_game_recordings: carry-snapshot-corvo-v20/native.mp4](<../../../../work/natural-moveset/carry-snapshot-corvo-v20/native.mp4>)
- [in_game_recordings: carry-snapshot-emily-v20/native.mp4](<../../../../work/natural-moveset/carry-snapshot-emily-v20/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-corvo-v20/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-corvo-v20/native.mp4>)
- [in_game_recordings: carry-snapshot-corvo-v18/native.mp4](<../../../../work/natural-moveset/carry-snapshot-corvo-v18/native.mp4>)
- [in_game_recordings: native-pickup-crouched-corvo-v21b/native.mp4](<../../../../work/natural-moveset/native-pickup-crouched-corvo-v21b/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-corvo-v21/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-corvo-v21/native.mp4>)
- [in_game_recordings: native-pickup-crouched-emily-v21/native.mp4](<../../../../work/natural-moveset/native-pickup-crouched-emily-v21/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-emily-v21/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-emily-v21/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-emily-v22/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-emily-v22/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-corvo-v22/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-corvo-v22/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-emily-v22b/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-emily-v22b/native.mp4>)
- [in_game_recordings: carry-snapshot-stream-emily-v22/native.mp4](<../../../../work/natural-moveset/carry-snapshot-stream-emily-v22/native.mp4>)
- [in_game_recordings: short-lift-corvo-v4/native.mp4](<../../../../work/natural-moveset/short-lift-corvo-v4/native.mp4>)
- [in_game_recordings: short-lift-emily-v4/native.mp4](<../../../../work/natural-moveset/short-lift-emily-v4/native.mp4>)
- [in_game_recordings: short-lift-corvo-v6/native.mp4](<../../../../work/natural-moveset/short-lift-corvo-v6/native.mp4>)
- [in_game_recordings: short-lift-emily-v6/native.mp4](<../../../../work/natural-moveset/short-lift-emily-v6/native.mp4>)
- [in_game_recordings: native-carrier-pickup-corvo-v23/native.mp4](<../../../../work/natural-moveset/native-carrier-pickup-corvo-v23/native.mp4>)
- [in_game_recordings: native-carrier-pickup-emily-v23/native.mp4](<../../../../work/natural-moveset/native-carrier-pickup-emily-v23/native.mp4>)
- [in_game_recordings: native-carrier-pickup-crouched-corvo-v23/native.mp4](<../../../../work/natural-moveset/native-carrier-pickup-crouched-corvo-v23/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-v24/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-v24/native.mp4>)
- [in_game_recordings: native-paired-pickup-emily-v24/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-emily-v24/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-v24-trace/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-v24-trace/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-v24-fast/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-v24-fast/native.mp4>)
- [in_game_recordings: native-paired-pickup-emily-v24-fast/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-emily-v24-fast/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-contact-v3/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-contact-v3/native.mp4>)
- [in_game_recordings: native-paired-pickup-emily-cancel-v24c/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-emily-cancel-v24c/native.mp4>)
- [in_game_recordings: native-paired-pickup-emily-contact-v4/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-emily-contact-v4/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-cancel-v24/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-cancel-v24/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-contact-v3-final/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-contact-v3-final/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-settle-v25/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-settle-v25/native.mp4>)
- [in_game_recordings: native-paired-pickup-emily-settle-v25/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-emily-settle-v25/native.mp4>)
- [in_game_recordings: native-paired-pickup-corvo-visibility-v26/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-corvo-visibility-v26/native.mp4>)
- [in_game_recordings: native-paired-pickup-emily-visibility-v26/native.mp4](<../../../../work/natural-moveset/native-paired-pickup-emily-visibility-v26/native.mp4>)
- [in_game_recordings: native-pickup-walk-drop-corvo-v26/native.mp4](<../../../../work/natural-moveset/native-pickup-walk-drop-corvo-v26/native.mp4>)
- [in_game_recordings: native-pickup-walk-drop-emily-v26/native.mp4](<../../../../work/natural-moveset/native-pickup-walk-drop-emily-v26/native.mp4>)
- [in_game_recordings: native-pickup-walk-drop-corvo-v27/native.mp4](<../../../../work/natural-moveset/native-pickup-walk-drop-corvo-v27/native.mp4>)
- [in_game_recordings: native-pickup-walk-drop-emily-v27/native.mp4](<../../../../work/natural-moveset/native-pickup-walk-drop-emily-v27/native.mp4>)
- [in_game_recordings: native-pickup-walk-crouch-drop-corvo-v27/native.mp4](<../../../../work/natural-moveset/native-pickup-walk-crouch-drop-corvo-v27/native.mp4>)
- [in_game_recordings: native-pickup-walk-crouch-drop-emily-v27/native.mp4](<../../../../work/natural-moveset/native-pickup-walk-crouch-drop-emily-v27/native.mp4>)
- [in_game_recordings: native-crouched-pickup-drop-corvo-v28/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-drop-corvo-v28/native.mp4>)
- [in_game_recordings: native-crouched-pickup-drop-emily-v28/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-drop-emily-v28/native.mp4>)
- [in_game_recordings: native-crouched-pickup-drop-corvo-v28-contact-v2/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-drop-corvo-v28-contact-v2/native.mp4>)
- [in_game_recordings: native-crouched-pickup-drop-emily-v28-contact-v2/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-drop-emily-v28-contact-v2/native.mp4>)
- [in_game_recordings: native-stand-crouch-carry-corvo-v28-contact-v2/native.mp4](<../../../../work/natural-moveset/native-stand-crouch-carry-corvo-v28-contact-v2/native.mp4>)
- [in_game_recordings: native-crouched-pickup-corvo-orientation-v29/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-corvo-orientation-v29/native.mp4>)
- [in_game_recordings: native-crouched-pickup-emily-orientation-v29/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-emily-orientation-v29/native.mp4>)
- [in_game_recordings: native-crouched-pickup-emily-v29-contact-v3/native.mp4](<../../../../work/natural-moveset/native-crouched-pickup-emily-v29-contact-v3/native.mp4>)
- [in_game_recordings: native-crouched-to-standing-carry-corvo-v29-contact-v3/native.mp4](<../../../../work/natural-moveset/native-crouched-to-standing-carry-corvo-v29-contact-v3/native.mp4>)
- [in_game_recordings: native-standing-to-crouched-carry-emily-v29-contact-v3/native.mp4](<../../../../work/natural-moveset/native-standing-to-crouched-carry-emily-v29-contact-v3/native.mp4>)
- [in_game_recordings: native-crouched-to-standing-carry-corvo-v30-sampled/native.mp4](<../../../../work/natural-moveset/native-crouched-to-standing-carry-corvo-v30-sampled/native.mp4>)
- [in_game_recordings: native-standing-to-crouched-carry-emily-v30/native.mp4](<../../../../work/natural-moveset/native-standing-to-crouched-carry-emily-v30/native.mp4>)
- [in_game_recordings: native-body-throw-corvo-v1/native.mp4](<../../../../work/natural-moveset/native-body-throw-corvo-v1/native.mp4>)
- [in_game_recordings: native-body-throw-emily-v1/native.mp4](<../../../../work/natural-moveset/native-body-throw-emily-v1/native.mp4>)
- [in_game_recordings: native-high-throw-emily-v31/native.mp4](<../../../../work/natural-moveset/native-high-throw-emily-v31/native.mp4>)
- [in_game_recordings: native-high-throw-emily-v32/native.mp4](<../../../../work/natural-moveset/native-high-throw-emily-v32/native.mp4>)
- [in_game_recordings: native-high-throw-corvo-v32/native.mp4](<../../../../work/natural-moveset/native-high-throw-corvo-v32/native.mp4>)
- [in_game_recordings: native-high-throw-corvo-v33/native.mp4](<../../../../work/natural-moveset/native-high-throw-corvo-v33/native.mp4>)
- [in_game_recordings: native-high-throw-emily-v33/native.mp4](<../../../../work/natural-moveset/native-high-throw-emily-v33/native.mp4>)
- [in_game_recordings: native-high-throw-corvo-open-v33-b/native.mp4](<../../../../work/natural-moveset/native-high-throw-corvo-open-v33-b/native.mp4>)
- [in_game_recordings: native-high-throw-emily-contact-v34/native.mp4](<../../../../work/natural-moveset/native-high-throw-emily-contact-v34/native.mp4>)
- [in_game_recordings: native-high-throw-corvo-v34/native.mp4](<../../../../work/natural-moveset/native-high-throw-corvo-v34/native.mp4>)
- [in_game_recordings: native-high-throw-emily-v34/native.mp4](<../../../../work/natural-moveset/native-high-throw-emily-v34/native.mp4>)
- [in_game_recordings: native-low-drop-corvo-regression-v34/native.mp4](<../../../../work/natural-moveset/native-low-drop-corvo-regression-v34/native.mp4>)
- [in_game_recordings: native-low-drop-emily-regression-v34/native.mp4](<../../../../work/natural-moveset/native-low-drop-emily-regression-v34/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-reader-v35/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-reader-v35/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-v35/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-v35/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-turn-v35/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-turn-v35/native.mp4>)
- [in_game_recordings: native-fetal-release-emily-turn-v35/native.mp4](<../../../../work/natural-moveset/native-fetal-release-emily-turn-v35/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-framed-v35/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-framed-v35/native.mp4>)
- [in_game_recordings: native-fetal-release-emily-framed-v35/native.mp4](<../../../../work/natural-moveset/native-fetal-release-emily-framed-v35/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-blend-v36/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-blend-v36/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-order-v37/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-order-v37/native.mp4>)
- [in_game_recordings: native-fetal-release-corvo-hold-v38/native.mp4](<../../../../work/natural-moveset/native-fetal-release-corvo-hold-v38/native.mp4>)
- [in_game_recordings: native-fetal-release-emily-hold-v38/native.mp4](<../../../../work/natural-moveset/native-fetal-release-emily-hold-v38/native.mp4>)
- [in_game_recordings: native-high-throw-corvo-regression-v38/native.mp4](<../../../../work/natural-moveset/native-high-throw-corvo-regression-v38/native.mp4>)
- [in_game_recordings: native-high-throw-emily-regression-v38/native.mp4](<../../../../work/natural-moveset/native-high-throw-emily-regression-v38/native.mp4>)
- [in_game_recordings: native-low-drop-corvo-regression-v38/native.mp4](<../../../../work/natural-moveset/native-low-drop-corvo-regression-v38/native.mp4>)
- [in_game_recordings: native-low-drop-emily-regression-v38/native.mp4](<../../../../work/natural-moveset/native-low-drop-emily-regression-v38/native.mp4>)

