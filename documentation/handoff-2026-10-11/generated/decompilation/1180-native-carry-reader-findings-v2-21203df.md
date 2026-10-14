# native-carry-reader-findings-v2.json

Original: [work/movement-integration/verified-native-carry-reader-v2/native-carry-reader-findings-v2.json](<../../../../work/movement-integration/verified-native-carry-reader-v2/native-carry-reader-findings-v2.json>)

SHA-256: `D58B73469D6CAC210BCE337E786348563ABF974CF86BE15CB3A679210CA27D89`; 3203 bytes.

## Original saved record

````json
{
  "complete": false,
  "remote_gameplay_bound": false,
  "visual_accepted": false,
  "cases": {
    "native-carry-corvo-reader-v2": {
      "result": "native-carry-corvo-reader-v2/result.json",
      "source_review": "native-carry-corvo-reader-v2/source-review.json",
      "samples": 279,
      "accepted_samples": 219,
      "checks": 11,
      "variants": {
        "1": 12,
        "2": 165,
        "3": 28,
        "4": 14
      },
      "video": "native-carry-corvo-reader-v2/native.mp4",
      "reader_sha256": "2707A9390884AAE72E466D768228C9536C57261C1DC89EAFA0DB767D9A017C79"
    },
    "native-carry-emily-reader-v2": {
      "result": "native-carry-emily-reader-v2/result.json",
      "source_review": "native-carry-emily-reader-v2/source-review.json",
      "samples": 280,
      "accepted_samples": 219,
      "checks": 11,
      "variants": {
        "1": 13,
        "2": 162,
        "3": 32,
        "6": 12
      },
      "video": "native-carry-emily-reader-v2/native.mp4",
      "reader_sha256": "2707A9390884AAE72E466D768228C9536C57261C1DC89EAFA0DB767D9A017C79"
    }
  },
  "confirmed": [
    "Exact native CarryCorpse45 and DropCorpse46 ownership chains decoded and checked against live scenes for both characters.",
    "Carry45+F8 is the source corpse generation; +110 phases1/2/3 are equipment preparation/pickup/carry; +24 owns the current scene.",
    "Native carry walking changes when horizontal velocity squared exceeds1; its1600ms scene is distinct from the3000ms idle.",
    "Corvo loop/walk uses Scene_2; Emily uses Scene_1. Both native standing and crouched carry walking were recorded.",
    "Drop46 copies the corpse to+38 and owns a separate scene. Open-space Corvo low_stand(kind0) and Emily low_sneak(kind1) passed reader replay.",
    "Near-wall Emily observation selected fetal_pose(kind4); this variant is decoded and unit-tested but not included in the final two replay cases.",
    "One Corvo idle scene retired between serial source and independent scene probes; the following sample owns walk. This boundary is recorded and excluded from matching coverage."
  ],
  "remaining": [
    "Remote body/corpse identity mapping, native third-person attachment and authoritative physical handoff.",
    "Whole-body pickup/contact alignment, crouched carrying clips, directional carrying and equipment transitions.",
    "High/soft releases and throwing require native observation and dedicated runtime bindings.",
    "Shared-target contention, interruption, death, reload, map transition and peer-loss cleanup.",
    "Full-body visual comparison and final acceptance for both characters."
  ],
  "ghidra": {
    "program": "Dishonored2.exe",
    "carry_vtable": "141c77748",
    "drop_vtable": "141c790c8",
    "carry_enter": "140829030",
    "carry_update": "14082a110",
    "carry_speed_test": "1408297b0",
    "drop_enter": "140831fe0",
    "drop_select": "140832770"
  },
  "source_reader": "outputs/d2-coop-lab/src/native_player_carry_scene.h",
  "tests": "work/movement-integration/world-tests-carry-reader-v2.log",
  "archive": "work/movement-integration/verified-native-carry-reader-v2"
}
````
