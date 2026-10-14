# State record: SharedCheckpointFixture

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "ObservedAt": "2026-10-08T23:16:45.4355607+02:00",
  "DllSha256": "05EECCD0D21B4544C6A7A4D000A7E347694B9559313A6548B80A40B354A4E4FE",
  "Passed": true,
  "Reports": [
    {
      "Report": "shared-checkpoint-client-delay-client-verification.json",
      "Passed": true
    },
    {
      "Report": "shared-checkpoint-disconnect-client-verification.json",
      "Passed": true
    },
    {
      "Report": "shared-checkpoint-disconnect-host-verification.json",
      "Passed": true
    },
    {
      "Report": "shared-checkpoint-duplicates-verification.json",
      "Passed": true
    },
    {
      "Report": "shared-checkpoint-host-delay-host-verification.json",
      "Passed": true
    },
    {
      "Report": "shared-checkpoint-mismatch-verification.json",
      "Passed": true
    }
  ],
  "Scope": "Fixed, byte-identical pre-staged Dunwall SLOT82. Native reload request from either side, exactly one load each, restored Corvo/Emily, both-ready gate. Client and host delay, simultaneous/repeated requests, either peer disconnect to native pause, mismatched digest refusal and rearm recovery verified.",
  "NativeUiVisuallyVerified": false,
  "UiCaptureFailure": "foreground window did not report a process id",
  "SaveTransferImplemented": false,
  "DeathPropagationImplemented": false,
  "FullCoop": false,
  "Backup": "work/verified-shared-checkpoint-05ee"
}
````
