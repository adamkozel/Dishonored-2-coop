# State record: DoppelgangerSharedRecovery

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "ObservedAt": "2026-10-09T12:44:38.6790564+02:00",
  "Passed": true,
  "DllSha256": "424BE19EA9BF87FD134787D99D1637E2A249CF1A827E51D77D5B28AE333A9B7D",
  "Cases": [
    "ReloadHost",
    "ReloadClient",
    "DeathHost",
    "DeathClient"
  ],
  "Report": "research/doppelganger-recovery-424B.json",
  "Scope": "Fixed canonical Dunwall SLOT82. Either reload and either natural player death/native retry; both original death bindings, one shared load, both-ready role gate, exactly one owned native clone/model with AI/idle held and initialized sword after recovery. Does not verify arbitrary campaign checkpoints or cinematic rearming.",
  "Milestone": "work/doppelganger-smoothing-424B",
  "PreviousPostRecoveryActions": "research/doppelganger-post-recovery-actions-ADB4.json"
}
````
