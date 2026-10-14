# findings.json

Original: [work/movement-integration/high-mantle-research/findings.json](<../../../../work/movement-integration/high-mantle-research/findings.json>)

SHA-256: `A86443F9A9D7BF68E0C41700A80A31358A6E46691BC2D0B707ACE40A878FDB8D`; 2344 bytes.

## Original saved record

````json
{
  "executable_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "confirmed_static": [
    "Native state8 vtable RVA1C82F68 enters via85EAF0. +D8 kind selects low(0), mid(1), high(other); +B8 selects exit-state-specific declaration.",
    "Void base mantle declaration maps high standing to mantle_idle.midnightscene and high sneak to mantle_sneak.midnightscene. State8 kind1 currently verified by the port is natively the mid case.",
    "Native sensor3BCA70 compares targetZ(+2FC)-originZ(3BB790) against declaration+80. Exact asset m_highMantleHeight=1.7000000477.",
    "High mantle_idle contains PlayerBody, PlayerRightArm, PlayerLeftArm groups and833ms animation tracks. Native low_idle uses500ms.",
    "Existing native full-body donors cover2.0/2.5/3.0/3.5m original climbs. Their roots differ from player trajectory; root transforms must remain excluded."
  ],
  "runtime_confirmed": [
    "Canopy north approach [-30.7,-153.6,eye68.1,yaw270] triggers native kind2 without asset edits.16 cases: both roles, standing/crouched exit, armed/holstered, two repetitions;158 scene samples.",
    "High kind2 uses mantle_idle or mantle_sneak,833ms; Corvo Scene_2,Emily Scene_1. Subsequent exact identity read shows Corvo single support arm is LEFT, correcting earlier handle-only inference. Failed v20-v1 recording pinned and reader v2 corrected; reciprocal ownership remains required.",
    "Kind1 low_idle at canopy front can bind left arm/body/right arm. Reader must allow up to3 validated unique player bindings.",
    "Port v20-v2 DLL9393D00E: four lit actual D2 takes (both roles/exit stances) pass36 checks across16 native climbs including drawn/holstered states. Sampled frames show whole-body rise/tuck and stance recovery; close palm fit remains unresolved."
  ],
  "hypotheses_pending": [
    "Fine palm/sword contacts require closer camera evaluation. The current authored contact is calibrated to one Dunwall canopy.",
    "Other ledge geometries and native interrupted traversal are not yet accepted."
  ],
  "scope": "Targeted native kind2 binding verified in an isolated Dunwall fixture. General traversal and full visual acceptance remain incomplete.",
  "next_test": "Closer hand contact and geometry variants; preserve four indexed high-mantle-*-v20-lit-v1 recordings."
}

````
