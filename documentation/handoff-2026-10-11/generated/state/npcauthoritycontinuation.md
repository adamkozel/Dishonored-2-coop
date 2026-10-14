# State record: NpcAuthorityContinuation

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "at": "2026-10-13T21:12:01.874724+02:00",
  "candidate": "candidate-npc-stream-v847",
  "receipt": "work/movement-integration/npc-lifecycle-audit-v849.json",
  "details": "documentation/handoff-2026-10-11/72-npc-menu-and-checkpoint-lifecycle.md",
  "runtimeVerified": true,
  "presentationStreamVerified": true,
  "checkpointRenewalVerified": true,
  "networkAuthorityVerified": false,
  "complete": false,
  "latest": "847–849: NPC stream sessions now retain bindings and pause native writes during ordinary menus, resume with fresh interpolation/replay floor, and release before shared checkpoint teardown. Test848b verifies both menus >6s then fresh catalog of seven actors, two player roles retained; client acquired/released24/lost0. Visible849 shows live body updates before and after host Pause, zero during suspension. Failed848 fixed only the harness five-actor assumption.",
  "next": "Generalize ongoing NPC lifecycle and shared combat outcomes: new/retired actor negotiation, repeated hit requests and authoritative health/death/equipment, AI perception of both players. Validate remaining224-joint visible rigs and sustained bandwidth. Continue campaign/progression, functional movement/VFX gaps and native session UI/separate-account Steam validation.",
  "scope": "Verified native host/client menu suspension and resume on retained NPC arms; same-map coordinated checkpoint retires old AI leases and rebinds seven fresh native actors after both-complete/role restoration. Visible menu proof has two223-joint body callbacks before/after, none during held pause. General combat, mid-world spawning/death and campaign transitions remain incomplete."
}
````
