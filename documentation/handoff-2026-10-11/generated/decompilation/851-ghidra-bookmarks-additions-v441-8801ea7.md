# ghidra-bookmarks-additions-v441.json

Original: [work/movement-integration/ghidra-bookmarks-additions-v441.json](<../../../../work/movement-integration/ghidra-bookmarks-additions-v441.json>)

SHA-256: `A9360ED68485325AA04FAF9292C637FBC84326F2E9DC22E1DFCAC751693F3460`; 1044 bytes.

## Original saved record

````json
[
  {
    "address": "1407db090",
    "category": "D2Coop_Doppelganger",
    "comment": "Runtime437/439 exact EXE: sceneinstance+10/+18 contains 0x20-byte entries {groupInst,actor,groupName,groupId}. GroupInst RTTI1c6ca98 +20/+28 is pointer list of tracks; +c8 reciprocal sceneinstance; +d0 bound clone. Runtime439 source clone handle210244124 has spawn scene221 duration1000 and native spelling doppelganger_dispawn scene223 duration466; SAME scene/group/track addresses reused, so pointer alone is not incarnation identity. FX track RTTI1c7f5a8 +18/+20 active key records24bytes, +c0/+c8 helper records12bytes {packedKeyId,helperHandle,stopMode}. All5 keys matched prop RTTI1afdfd0 +8 FXdecl +10 socket, and helper idEntity.439 harness count failed due four nulls from other tracks, not reader;440 corrected test passed all15 checks, including independent FX-map helper/declaration matches and complete cleanup. Never infer ownership from proximity or parent; preserve source clock/id/clone generation. No remote FX playback verified."
  }
]

````
