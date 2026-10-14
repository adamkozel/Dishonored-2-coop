# State record: CombatGuard

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "Passed": true,
  "ObservedAt": "2026-10-09T03:12:50.2829256+02:00",
  "DllSha256": "2935909D62DDE8C21A70FD25528D89F9C69B994F53550378C7A1E1F87F46B519",
  "ProbeSha256": "FFA370885475F8AB4AA52D9D4A5B932D1594274338BC8E8EE43AD087F4D25C93",
  "Reports": [
    {
      "Report": "combat-guard-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:01:27.7383683+02:00",
      "Sha256": "055A9E9B8F863DCB50D59A5ED35CE10F4B90A260CB738BC87B66B52FF850C863"
    },
    {
      "Report": "guard-link-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:03:11.1931538+02:00",
      "Sha256": "CDF529FBC79BF869B81011B6EF7D0FBF8CA42A21F91E7877AC078A5F793A8A53"
    },
    {
      "Report": "guard-health-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:05:39.5091685+02:00",
      "Sha256": "959C4A2911773F751C95CF6C80570128B6F155BDD9936BBFB4CAEC61C93F00DF"
    },
    {
      "Report": "baseline-integrity-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:10:13.8322994+02:00",
      "Sha256": "B852BBEE4BE34F36B1F90F6B50B6EC7211EBA48F483C80B78CC7F6C4519EB31A"
    },
    {
      "Report": "lab-restoration-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T03:10:13.7625804+02:00",
      "Sha256": "ABDA692BF64A61CB6C92881D39371E74BF9857DB1C5F8AD0B6BBE1ADE12C131C"
    }
  ],
  "Scope": "One native host escape guard pursues the remote counterpart, while client decision/reflex AI are natively paused. Root movement, a real sword hit and dependent lease cleanup verified. Existing inactive guard and nonfatal health regressions passed.",
  "HostMovement": 2.182132514528505,
  "ClientMovement": 2.1850465391754694,
  "NativeDamage": 15,
  "NativeAttackSamples": 15,
  "SourceBackup": "work/combat-guard-2935",
  "FullWorldAuthority": false,
  "PlayableCoop": false,
  "OnlineTwoAccounts": false,
  "IntegratedFatalCombatPassed": false,
  "Pending": "Fatal combined test exposed recovery-arm sequencing and native upward health changes; fixes are in newer unbuilt source."
}
````
