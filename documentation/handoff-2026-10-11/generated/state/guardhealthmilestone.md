# State record: GuardHealthMilestone

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "ObservedAt": "2026-10-09T01:58:56.7018876+02:00",
  "DllSha256": "42BEAD3DF5FDCAE248E332F84F3CBAA82C63587FF604424B3A761DA345F629FA",
  "ProbeSha256": "55BB4F54012E0B5A59C7289598C585C130EA85E2799E3FD848EDC07559BEC93B",
  "Passed": true,
  "Reports": [
    {
      "Report": "guard-health-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:46:47.7404668+02:00",
      "Sha256": "EF3A6E90E7696B76EBC81F6622BC93C3728F92FF511D0475349289D21BD2713B"
    },
    {
      "Report": "guard-link-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:47:53.3955509+02:00",
      "Sha256": "E44F431F61FADEF62FB2819DFD92391B9973DC3FCB988D2983D10ACC6778C0FF"
    },
    {
      "Report": "world-observation-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:48:19.3143623+02:00",
      "Sha256": "73712E951B36076F446D480FB27EF8CED0A597D6DEE865E814EC0CFFBB87B809"
    },
    {
      "Report": "shared-death-host-health-damage-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:48:39.7254189+02:00",
      "Sha256": "BA59B6ECEBEE7D70B574644B7939D304A8F5275F58A0B327342BFF627F359120"
    },
    {
      "Report": "shared-death-client-health-damage-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:48:59.1258294+02:00",
      "Sha256": "6B9DC5AF434C3E34223B6453A645D9DDA78520ED758ABD32B9C256586675C727"
    },
    {
      "Report": "body-stream-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:51:42.6447989+02:00",
      "Sha256": "A2795137A839FE4A4964A01D6B89591F314F9A02C4B03BF3D8832DD672EEEB58"
    },
    {
      "Report": "baseline-integrity-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:49:26.6527652+02:00",
      "Sha256": "DEA6C9BD9D95D2237D4C8AFB01B2BEA45977C264C2517C0E8D7FB7047DC4031D"
    },
    {
      "Report": "lab-restoration-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T01:55:06.2039103+02:00",
      "Sha256": "76BC9A4C5FD54180EDA0C46A601D3CB97626C57F248A80CA62AB37749DFDA0A8"
    }
  ],
  "Scope": "One authored inactive guard: explicitly armed native health 40 to 35, host/client isolation and lifetime guards; read-only world health; both natural fall deaths and coordinated retries; body stream and baseline integrity.",
  "BodyFixtureCorrection": "Four transition captures precede stationary crouch sampling. First pre-fix sample crossed the native transition. Rotation tolerance unchanged; failed raw data preserved.",
  "PlayableCoop": false,
  "GeneralCombatReplication": false,
  "OnlineTwoAccounts": false,
  "SourceBackup": "work/verified-guard-health-42BE"
}
````
