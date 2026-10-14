# State record: CheckpointResearch

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "DllSha256": "CD1F0479923F520C490B0860AEF133927B5F683BF685FDE2A53F45B1B614F412",
  "NativeRoleRestorationPassed": true,
  "AutomaticRoleRestorationBeforeGameplay": true,
  "ExplicitLocalArmingRequired": true,
  "SourceCheckpointSha256": "9D79B4115AF4E13D8341C3CDC46F2B56B05A1E1D07CB0E086D5A7C988809EA09",
  "SourceRole": "Corvo",
  "RestoredRole": "Emily",
  "RoleRestorationRecord": "native-loading-character-verification.json",
  "LoadingHoldRecord": "native-loading-hold-verification.json",
  "NativeCharacterRoutineRva": "0x8a7650",
  "NativeMaintenanceRoutineRva": "0x8ab380",
  "NativeLoadingSwitchRva": "0x7885b0",
  "NativeLogicDuringRestoration": 15,
  "NativeWorldPhaseDuringRestoration": 3,
  "ReplacementQueueEmptyBeforeRelease": true,
  "ReplacementDisplacement": 0,
  "PreservedNpcIdentities": 64,
  "MovementDistance": 0.8758343964471809,
  "BoundedLocalLoadingHoldMilliseconds": 2000,
  "SteamTrafficDuringHoldVerified": true,
  "BinarySaveOrMetadataModifiedByConversion": false,
  "BodyDefinitionVerifiedBeforeGameplay": true,
  "CheckpointFilesAfterExperiments": [
    {
      "File": "checkpoint.dat",
      "Hash": "9D79B4115AF4E13D8341C3CDC46F2B56B05A1E1D07CB0E086D5A7C988809EA09",
      "Expected": "9D79B4115AF4E13D8341C3CDC46F2B56B05A1E1D07CB0E086D5A7C988809EA09"
    },
    {
      "File": "game.details",
      "Hash": "FD8E89DC59CBE8503150BD933A28C6F88095385CB60A1087FEDF466C92453194",
      "Expected": "FD8E89DC59CBE8503150BD933A28C6F88095385CB60A1087FEDF466C92453194"
    },
    {
      "File": "screenshot.png",
      "Hash": "7D5B1787E505CFDC18481F609DED2576C1AFC342014F3CA2E803BABDB7C18D6D",
      "Expected": "7D5B1787E505CFDC18481F609DED2576C1AFC342014F3CA2E803BABDB7C18D6D"
    }
  ],
  "NativeCommandObservers": [
    "LoadGame",
    "RestartFromCheckpoint",
    "RestartFromMemoryCheckpoint"
  ],
  "EarlierPostGameplayConversionRecord": "native-character-restore-verification.json",
  "EarlierNativeUiRecord": "native-character-ui-guards-verification.json",
  "EarlierNativeUiRecordDll": "DC5D01BF30EBD254CF0197625F911ED0A08078DFA9A144C9CD25102390975E70",
  "F9Verified": false,
  "GameOverDispatchVerified": false,
  "CanonicalSaveTransferImplemented": false,
  "SharedResetTransactionImplemented": false,
  "BothReadyLoadingBarrierImplemented": false,
  "Scope": "Explicitly armed client-only Dunwall load experiment. An unchanged Corvo checkpoint becomes Emily while the new world remains in native loading state 15, then a two-second local hold precedes gameplay. Position and 64 authored NPC identities preserved. No network reset, save transfer, general quest/inventory/power compatibility or both-ready transaction."
}
````
