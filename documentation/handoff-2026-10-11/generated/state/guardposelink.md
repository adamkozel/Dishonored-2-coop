# State record: GuardPoseLink

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "Passed": true,
  "DllSha256": "CD1F0479923F520C490B0860AEF133927B5F683BF685FDE2A53F45B1B614F412",
  "Record": "guard-link-verification.json",
  "Checks": [
    "No binding or mutation when only client arms",
    "Fresh incarnation handshake, both native AI holds, host movement, cached/rendered client pose and animated body alignment",
    "Fixed lease expiry and duplicate-arm refusal",
    "Guard poses expire while ordinary Steam traffic stays connected",
    "client native pause releases both references and stops mutations",
    "host native pause releases both references and stops mutations",
    "Checkpoint reload releases controls and requires new binding, nonce and world epoch"
  ],
  "Scope": "Eight-second, explicitly armed, host-to-client pose transfer for one authored guard with native decision/reflex pause. Host movement stimulus is a bounded native transform nudge. Combat must stay inactive. Does not establish combat/damage/quest authority, arbitrary NPC support, or complete remote animation.",
  "MaxPoseError": 1.363813194366535e-07,
  "MaxAnimatedBodyError": 1.1400000005323818e-05,
  "BodyObservations": 68,
  "ReloadClient": true,
  "DisconnectHost": false,
  "HistoricalFullRecord": "guard-link-full-verification.json",
  "HistoricalFullRecordDll": "31D5221EC8FC5360249FC49F2B28C5D3027973CB44B25B0BC13F55B5C73C3916",
  "CombatAuthorityImplemented": false,
  "NaturalAiNavigationVerified": false,
  "SharedCheckpointReloadImplemented": false
}
````
