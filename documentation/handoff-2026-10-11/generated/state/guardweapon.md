# State record: GuardWeapon

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "Passed": true,
  "ObservedAt": "2026-10-09T04:08:55.2678844+02:00",
  "DllSha256": "DE225C601D7B3A8124C9DB46266C639EE065C6B3E13D5AA90ABD2553F0F76947",
  "Reports": [
    {
      "Report": "baseline-integrity-verification.json",
      "Sha256": "D61E6EA538062563000C07F4E1486E18C18DC91428AC6C33DF6EC02AD50F998E"
    },
    {
      "Report": "guard-body-weapon-verification.json",
      "Sha256": "11E057F3D63202E262282ABFCCF99BA118BBE695FD9CBE88AF6D12EAE47897B3"
    },
    {
      "Report": "guard-weapon-guards-verification.json",
      "Sha256": "35314FE3F669DEB3D52652568EA72C037225DBFB2457D29B7F275444B45A1FA6"
    },
    {
      "Report": "guard-weapon-verification.json",
      "Sha256": "7212DA20E0803A500CE534BAAEB4B8CDE6D48689F93EF450E3A2EF1A0683D5A4"
    },
    {
      "Report": "lab-restoration-verification.json",
      "Sha256": "41638E5ACCD88B8DB9C49053250D7A4372CEFBA89214443164F85BD5DE50AD63"
    },
    {
      "Report": "sword-death-combat-guard-weapon-verification.json",
      "Sha256": "92DDC8BADA40607179021ADFB0C2D2A0FA617350861EA56E13F2A66A31E66483"
    }
  ],
  "Scope": "Fixed guard sword state over native Steam; seven stream checks, five lifetime scenarios, five native fatal hits/shared retry and combined animation observations.",
  "WeaponCombatSamples": 61,
  "AnimatedDrawnSamples": 7,
  "CoreJointObservations": 270,
  "VisualAlignmentVerified": false,
  "OtherNpcs": "Five independent local NPC hits killed the visible observer; no controlled-guard local damage.",
  "PlayableCoop": false,
  "OnlineTwoAccounts": false,
  "Pending": "Sword attachment tag/joint correct but live weapon/root alignment requires investigation; client sword root height remained fixed during changing arm poses."
}
````
