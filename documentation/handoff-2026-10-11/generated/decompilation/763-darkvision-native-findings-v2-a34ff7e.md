# darkvision-native-findings-v2.json

Original: [work/movement-integration/darkvision-native-findings-v2.json](<../../../../work/movement-integration/darkvision-native-findings-v2.json>)

SHA-256: `D26370DAD4857B149EC6BAD241B610D53BF2D54E998C1687CE784773410B1BBF`; 2643 bytes.

## Original saved record

````json
{
  "executable_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "verified_dll_sha256": "D01CA730DBA05BAE2D0D1F258DAB32BBB3681B9089C61AD3E1EA15D8FAE55B4A",
  "bank": "D2MOVE25 / 194 clips, preceding 193 payloads unchanged",
  "protocol": "Native Steam semantic action version19, same236-byte packet, explicit Dark Vision variant10/933ms",
  "confirmed_runtime": [
    "Exact native quantity acquisition creates Dark Vision equipment from the zero-quantity item; equipment has1component, local player has88components.",
    "Both Corvo and Emily use power_darkvision_cast.midnightscene, Scene_1, 933ms, state3, with one reciprocal left-arm binding. Observed standing casts retain Running0.",
    "Native LT toggles the separate vision manager on and off. Source POV shows native first-person hand motion, UI and orange reveal. Observer vision manager remains inactive and ordinary rendering remains visible.",
    "Native unequip stops selection/component but does not immediately stop the active vision reveal. The isolated cleanup must wait for its own native lifetime, then restore quantity. Thirty-second bounded cleanup passed on native-darkvision-corvo-v4; prior five-second timeout failed and was preserved.",
    "Four standing integration takes (Corvo/Emily, v1/v2) pass8checks each. V2 closer views include complete body, native sword, boots and recovery; two separately identified native casts correspond to two remote events.",
    "Native inventory quantities, equipment cleanup, player56/avatar40 health and protected original Steam/save hashes are preserved. Exact runtime acquisition discovery/entity metadata is reset by isolated checkpoint reload."
  ],
  "evidence": [
    "work/natural-moveset/native-darkvision-corvo-v4/result.json",
    "work/natural-moveset/darkvision-corvo-v25-v1/result.json",
    "work/natural-moveset/darkvision-emily-v25-v1/result.json",
    "work/natural-moveset/darkvision-corvo-v25-v2/result.json",
    "work/natural-moveset/darkvision-emily-v25-v2/result.json"
  ],
  "selected_frame_visual_review": "Both closer full-body and native source POV contact sheets inspected. Visible hand/shoulder/torso action, sword attachment and standing recovery; no gross boot seam separation in this case. Full temporal visual acceptance and fine finger contact remain pending.",
  "remaining": [
    "Crouched, moving, holstered and airborne variants",
    "Determination of any world-visible Dark Vision effects beyond its native caster-only reveal",
    "Pause, death, reload, map change and disconnect lifecycle",
    "Continuous visual acceptance"
  ],
  "complete": false
}

````
