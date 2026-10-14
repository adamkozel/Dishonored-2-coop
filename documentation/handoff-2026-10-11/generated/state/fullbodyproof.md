# State record: FullbodyProof

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "at": "2026-10-09T18:57:50.665187+02:00",
  "dllSha256": "F4F035D954BFC39F59E170E494C6ED1B0093E332C55167649D1C43F3C5A1817D",
  "clipSha256": "109956C449EC4AB9DA9D68F944C173F844D391074B3A5D3310264B9F180484FF",
  "layoutHashHex": "00db32d37719931f",
  "result": "One complete Corvo forward advance, native-FP-inspired guard and recovery authored in Blender and played on the owned native D2 Doppelganger",
  "pipeline": "Offline full-body Blender authoring to validated native-skeleton local-pose clip; existing Steam-driven root; native correctives and equipment",
  "productionIntegrated": false,
  "animGenUsed": false,
  "userVisualAcceptance": "Pending user review of recorded proof, not a milestone approval gate",
  "visualReview": {
    "evidence": "controller-video-4, authored-ready/step/settled/guard/recovery images, final-authoring-guard.png",
    "assessment": "Useful limited proof; skin proportions coherent, support contacts retained, guard hand on hilt, complete-body action survives transfer",
    "limitations": [
      "Restrained offhand and narrow stance still deserve artistic refinement",
      "Existing relaxed native idle and open hand return outside the authored clip",
      "Low capture cadence limits judder assessment",
      "No Emily, direction changes, crouch, terrain or complete moveset acceptance"
    ]
  },
  "runtimeMetrics": "runtime-verification.json",
  "runtimeSafety": "safety-verification.json",
  "additionalSafety": {
    "FinalStatus": {
      "experimental": true,
      "active": false,
      "start": 121907656,
      "end": 0,
      "seconds": 0.800003,
      "fixed": -1.0,
      "checksum": 11806995091830539036,
      "applied": 4216,
      "refused": 1
    },
    "NoMovementExpirySeconds": 10,
    "ObservedActiveAfterExpiry": false
  },
  "firstPersonRegression": "milestone-F4F/player-arm-observation-verification.json",
  "baselineIntegrity": "milestone-F4F/baseline-integrity-verification.json",
  "videos": [
    "d2-fullbody-sequence.mp4",
    "blender-sequence.mp4"
  ],
  "authoring": "corvo-fullbody-authored.blend",
  "instructions": "README.txt",
  "transport": "Existing native SteamNetworkingSockets012 on one PC; separate-account Internet hosting/joining not established by this proof",
  "preserved": [
    "work/friendly-fire-345C",
    "work/sword-grip-fit/789d-grip-milestone",
    "work/sword-grip-fit/2997-combined-research",
    "work/fullbody-proof/milestone-F4F"
  ],
  "blocker": {
    "component": "UE 5.8.2 / Epic AnimGen",
    "approvalReview": "Rejected uninstall/removal because confirmation deletes engine data; cancelled, not retried",
    "reason": "Registered engine directory missing; repair IS-0002-DP-01",
    "fallback": "Blender 5.2.0 LTS",
    "existingProjectsModified": false
  },
  "confirmedFindings": [
    "Native skinned 215-joint Corvo imported with preserved weights and rest axes",
    "Set edit-bone length before assigning native matrix; full rest-matrix comparison catches inverse-bind corruption",
    "Authoring clip must leave root/anchor identity and preserve anatomical local translations",
    "Native corrective helper routine edits local and global matrices; final native pass required",
    "Repeated native setviewpos is not a locomotion driver; controller-driven step removed observed vertical oscillation"
  ],
  "hypotheses": [
    "Dynamic view-height subtraction plus deferred position event likely explains repeated-setviewpos height oscillation; exact causal path not completely traced"
  ],
  "pending": [
    "Automatic selection by replicated movement/combat state and interruption arbitration",
    "Convincing start/stop/turn/stance transition set",
    "Emily adaptation and broad sword contact review",
    "Native UI/session setup, campaign and complete no-friendly-fire coverage"
  ]
}
````
