# State record: AiDecisionScope

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "DllSha256": "31D5221EC8FC5360249FC49F2B28C5D3027973CB44B25B0BC13F55B5C73C3916",
  "Scope": "Two-second reference-counted pause of AIDecision only. Independent native scheduler reads. Does not establish complete AI authority or host NPC pose application.",
  "Record": "ai-decision-hold-verification.json",
  "Target": "client ld_spawner_guard_tutorial_1",
  "Invalidated": 0,
  "PostLoadControl": "ai-postload-control.json",
  "Passed": true,
  "Released": 4,
  "Acquired": 4,
  "Checks": [
    "Host mutation refused",
    "Only one client decision component removed and restored; duplicate did not renew; host and Steam continued",
    "Explicit cancellation",
    "client native pause releases hold and refuses reacquisition",
    "host native pause releases hold and refuses reacquisition"
  ],
  "CurrentBuildReloadAndDisconnectTested": false,
  "HistoricalFullRecord": "ai-decision-hold-before-guard-verification.json"
}
````
