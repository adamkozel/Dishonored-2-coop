# State record: PoseSmoothing

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "Passed": true,
  "CurrentDll": "424BE19EA9BF87FD134787D99D1637E2A249CF1A827E51D77D5B28AE333A9B7D",
  "MeasurementDll": "80078F9F68B055D7F8A5D918ACB8D1F548252378A08CF53AD8A51FE834BF073A",
  "Report": "research/pose-smoothing-verification.json",
  "DelayMs": 80,
  "DefaultEnabled": true,
  "TraceScope": "Native/render transform callback cadence, not rendered FPS; six horizontal/diagonal/vertical A/B comparisons, both roles. Raw position updates about18-22Hz; smoothed153-547 callbacks/sec. No observer displacement; zero sampled native/render target error.",
  "Algorithm": "Receiver QPC timeline; linear XYZ and shortest-arc normalized quaternion rotation. No extrapolation; resets on session/world/map, >250ms gap or >3m discontinuity.",
  "Remaining": [
    "Internet jitter and two-account sessions",
    "Short teleports below3m",
    "Long-fall/high-speed discontinuity tuning"
  ],
  "Milestone": "work/doppelganger-smoothing-424B"
}
````
