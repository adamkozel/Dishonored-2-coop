# State record: GuardAnimation

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "Passed": true,
  "ObservedAt": "2026-10-09T03:37:06.9495991+02:00",
  "DllSha256": "4A1862BBD8A8574EF1986F430D71FB2A725D21883B89A1B2953885E1B17EED01",
  "ProbeSha256": "FFA370885475F8AB4AA52D9D4A5B932D1594274338BC8E8EE43AD087F4D25C93",
  "Reports": [
    {
      "Report": "sword-death-combat-guard-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:14:15.4645838+02:00",
      "Sha256": "FB5FF601F19E7EEB73848C55A6C45892E2D52F1C569E189E4BB0FD1BA1E76D1C"
    },
    {
      "Report": "guard-body-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:32:37.3492239+02:00",
      "Sha256": "D52308F04D4058634E028BF2A83478C33DB68A948C347DEABC59EB3DEC74098B"
    },
    {
      "Report": "guard-body-guards-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:35:43.7159491+02:00",
      "Sha256": "813EAB3DB875A29087E96086AC976E71D4BBAF04A19D72E88A9DFD8E2759D178"
    },
    {
      "Report": "lab-restoration-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:36:15.5438137+02:00",
      "Sha256": "61D3CF902C90DBAD58C8DC70AC21AD0E7392983F98A12A1F32A705A44065B663"
    },
    {
      "Report": "baseline-integrity-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:36:15.5809329+02:00",
      "Sha256": "DD21FEC456F2E03FC6D51EE79F0004B84A3F50F327D6C338348E524D9EB618FC"
    }
  ],
  "Scope": "One native combat guard: five relayed sword hits, both death screens and coordinated checkpoint recovery; visible native guard core pose comparison and eight animation lifetime checks.",
  "CoreJointObservations": 270,
  "NativeAttackSamples": 6,
  "OtherNpcDamage": "Six local hits from Ramsey and escape guard 2 killed the observer during the animation-only fixture; controlled guard caused none. Other NPC world authority remains absent.",
  "PlayableCoop": false,
  "OnlineTwoAccounts": false,
  "SourceBackup": "work/native-guard-body-4A18"
}
````
