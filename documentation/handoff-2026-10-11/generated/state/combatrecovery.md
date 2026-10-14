# State record: CombatRecovery

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "Passed": true,
  "ObservedAt": "2026-10-09T02:56:02.6684234+02:00",
  "DllSha256": "ABC00D06C0A1E17CEEC1BD4740CEEE9A14AF99C61B4C26CA93E4FFC05647574E",
  "ProbeSha256": "FFA370885475F8AB4AA52D9D4A5B932D1594274338BC8E8EE43AD087F4D25C93",
  "Reports": [
    {
      "Report": "sword-death-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:49:26.5195681+02:00",
      "Sha256": "162CB0AD6663439E35FAD79324250287B114831D1075421D35645CEDDBE9AA2D"
    },
    {
      "Report": "sword-relay-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:53:36.9027703+02:00",
      "Sha256": "98D59EBC8FF443B9A7D13C4A114AE08F3F74B1C8BC0A2C7D7104F265E8691CBE"
    },
    {
      "Report": "sword-relay-guards-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:51:07.937762+02:00",
      "Sha256": "BE803E7E5A6C1F3C6E05179C3C91C1472B75B835E4C79BD0BFFFE0FA5FE37029"
    },
    {
      "Report": "sword-relay-guards-cumulative-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:52:55.2435234+02:00",
      "Sha256": "C0EA43E33457ACB2FD8D7BC4D8675901AC8CC7869DF3E3C567E5DE93312AB96D"
    },
    {
      "Report": "native-loading-character-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:54:24.5078622+02:00",
      "Sha256": "B078B4940067497A0EE8C2EFD94AF427326A49A8E73384C4D00E1B4EEC1C00D5"
    },
    {
      "Report": "shared-checkpoint-client-delay-client-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:54:51.4593904+02:00",
      "Sha256": "910E76D593F8EBB041F50095B23E3FBD54B49B525FF1D044CCDCB52CF2F5C4BE"
    },
    {
      "Report": "shared-checkpoint-duplicates-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:55:03.914909+02:00",
      "Sha256": "83494110165A02F3EE90D12498AC83CADAA998381CC1779B5F5267A0B805641F"
    },
    {
      "Report": "shared-death-host-health-damage-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:55:23.32213+02:00",
      "Sha256": "9F28F38E86BD82886E3594998DD462C5F0315A6857314A4A3D399E7DCC54DF58"
    },
    {
      "Report": "baseline-integrity-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:55:23.346443+02:00",
      "Sha256": "B7B6AF001240D32C83233996C9A809540DB242DAA864810C193E1E766C368557"
    },
    {
      "Report": "lab-restoration-verification.json",
      "Passed": true,
      "ObservedAt": "2026-10-09T02:55:38.1572059+02:00",
      "Sha256": "B491A692843B4CEB363427AD84F63C42D1E95558FCCD5F43C9D1FBD3FBD59BC6"
    }
  ],
  "Scope": "Five actual native guard hits relayed through Steam to fatal client health, both native death screens, one coordinated retry, role restoration, both-ready gate and checkpoint health preservation. Single and cumulative mode cleanup, mismatched modes, native pauses, expiry, reload, actual client loss. One known guard and isolated offset fixture.",
  "NativeSwordHits": 5,
  "CheckpointHealth": 56,
  "HealthBeforeNativeRoleCorrection": 70,
  "PlayableCoop": false,
  "OnlineTwoAccounts": false,
  "GeneralCombatAuthority": false,
  "SourceBackup": "work/combat-recovery-ABC0"
}
````
