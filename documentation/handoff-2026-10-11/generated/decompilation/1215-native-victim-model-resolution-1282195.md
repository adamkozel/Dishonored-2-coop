# native-victim-model-resolution.json

Original: [work/natural-moveset/carry-peer-research-v1/native-victim-model-resolution.json](<../../../../work/natural-moveset/carry-peer-research-v1/native-victim-model-resolution.json>)

SHA-256: `5976326A2127173616CB9CF758D0AEA223DE5D87F9C942BF301A3E1951EF09F0`; 3249 bytes.

## Original saved record

````json
{
  "executable_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "confirmed": [
    {
      "evidence": "carry-victim-native-corvo-v4/scene-groupInfo-held.json",
      "finding": "GroupInfo+68 has four native skeleton names, +18 resolved pointer list is empty."
    },
    {
      "evidence": "carry-victim-native-corvo-v4/scene-tracks-held.json",
      "finding": "Only native LookAt track remains. Animation input count is zero."
    },
    {
      "rva": "197a60",
      "finding": "Native Midnight declaration model resolver iterates groups and calls 193500, then resolves scene-specific models."
    },
    {
      "rva": "193500",
      "finding": "Clears GroupInfo+18 pointer list and resolves +68 name list. Boolean true calls native declaration lookup 1e030; false calls existing-resource lookup 1e020."
    },
    {
      "rva": "7db9a0",
      "finding": "Native group instance selects compatible skeleton from the resolved model list, stores it at +d8."
    },
    {
      "evidence": "carry-victim-native-corvo-v5",
      "finding": "Native resolver enables Anim track and one graph input; mechanics pass, placement still too high."
    },
    {
      "evidence": "carry-victim-native-corvo-v6/corpse-skeleton-held.json",
      "finding": "Native folded-body pose matches ordinary pickup: anchor local z1.34369, spine03 z1.83437. Actor root remains too high; initial placement change did not fix the retarget constraint."
    },
    {
      "evidence": "carry-victim-native-corvo-v8 and carry-victim-native-emily-v8",
      "finding": "Both roles connect native female generic_carrycorpse_idle, physics 0->3->0 and restore own carried flag. 22 mechanical checks pass; initial attachment pop and timer release remain visually unaccepted."
    },
    {
      "evidence": "carry-victim-native-corvo-v10",
      "finding": "16-second lease permits release driven by source Drop46 before deadline, with cleanup. Earlier v9/v9b request refusal did not reproduce after diagnostics; exact failed prerequisite not established."
    },
    {
      "evidence": "carry-victim-native-corvo-v11",
      "finding": "Aligning actual victim spine02 with carrier left_arm plus authored clearance lowers the victim from the prior origin-coincident placement. Visual acceptance remains pending."
    },
    {
      "evidence": "carry-victim-native-corvo-v12/corpse-native-clip-drop.json",
      "finding": "Actual native generic_carrycorpse_droplow.md6anim connected on Mayhew. Source time drives the drop. Capture has too few post-cleanup samples for the existing release check; next harness extended to22s."
    },
    {
      "rva": "7d2160",
      "finding": "Start wrapper 7d9270 argument3 is int* initial scene time; argument4 is behavior flag."
    }
  ],
  "rejected_hypotheses": [
    {
      "hypothesis": "Sparse group UID prevents animation",
      "evidence": "v4 uses UID1 consistently; animation remains absent."
    }
  ],
  "pending": "v13 D2MOVE32 adds authored carrier low-drop standing/crouched poses to the native victim drop experiment. Both actual banks passed decoder/root/event/distance checks. Both-role runtime and visuals still pending."
}
````
