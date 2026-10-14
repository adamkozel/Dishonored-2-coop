# keyhole-native-findings-v1.json

Original: [work/movement-integration/keyhole-native-findings-v1.json](<../../../../work/movement-integration/keyhole-native-findings-v1.json>)

SHA-256: `46B3AC6BEC8B43ADB2E3130CDFE2870B218C66CA3E413E4207B825E25A2BD569`; 1596 bytes.

## Original saved record

````json
{
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "confirmed": [
    "Exact Peeking ID24, vtable RVA1c7c4a8, native X hold and B exit",
    "Standing/crouched entry and recovery, left/right scenes, both roles",
    "State+24 current scene, +1f0 logical door handle, +221 initial crouch, +223 side",
    "Native scene bindings: player body, owned right/left arms, door scene child",
    "Scene child parent+ac equals logical ArkDoor generation handle, ArkDoor VT1cb7c68",
    "Logical ArkDoor +b0 array is empty; it is not the FP arms child-list scheme",
    "Instance+15c cumulative scene clock; +160 current subscene time; +164 subscene base",
    "Logical hold loops every500ms. Full-body donor has3.1667s natural loop; source cumulative clock retained",
    "Ground+Interact75 precedes Peeking by approximately500ms; ground stance must remain valid"
  ],
  "observations": [
    "native-keyhole-corvo-v2",
    "native-keyhole-corvo-crouch-v3",
    "native-keyhole-emily-v3",
    "native-keyhole-corvo-reverse-v3",
    "native-keyhole-emily-reverse-crouch-v3",
    "native-keyhole-reader-corvo-v3",
    "native-keyhole-reader-emily-v3"
  ],
  "ghidra": [
    "1408426b0",
    "140843750",
    "140843540"
  ],
  "candidate": "D2MOVE24/193, append10clips per role; previous183payloads unchanged",
  "unverified": [
    "Remote visual door/hand/head alignment",
    "Door variants and arbitrary room geometry",
    "Early cancellation or death/load during keyhole",
    "Full movement phase remains incomplete"
  ]
}

````
