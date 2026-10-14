# carry-pickup-findings-v23.json

Original: [work/natural-moveset/carry-pickup-findings-v23.json](<../../../../work/natural-moveset/carry-pickup-findings-v23.json>)

SHA-256: `33404288842728834267CBD28799618D25541FDC9EA2B6D6E2DC4456B3F9EDF4`; 3314 bytes.

## Original saved record

````json
{
  "build": "96F05FF39420E7EB8982867C66484A6A1F7BCC792F97A42C4297E6D39FA65FA0",
  "cases": {
    "native-carrier-pickup-corvo-v23": {
      "evidence": "native-carrier-pickup-corvo-v23/validation.json",
      "video": "native-carrier-pickup-corvo-v23/native.mp4",
      "checks": 19,
      "video_sha256": "0EBB3448B5D91D291A0DB360DC250CB2E50ECF47722B26EF00B1021B8D48A47B",
      "visual_review": "native-carrier-pickup-corvo-v23/visual-review.json",
      "scope": "Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim."
    },
    "native-carrier-pickup-emily-v23": {
      "evidence": "native-carrier-pickup-emily-v23/validation.json",
      "video": "native-carrier-pickup-emily-v23/native.mp4",
      "checks": 19,
      "video_sha256": "3DE25B406FFB0B345A91B310106E5640006C37EB50245563B6AB2B3775592946",
      "visual_review": "native-carrier-pickup-emily-v23/visual-review.json",
      "scope": "Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim."
    },
    "native-carrier-pickup-crouched-corvo-v23": {
      "evidence": "native-carrier-pickup-crouched-corvo-v23/validation.json",
      "video": "native-carrier-pickup-crouched-corvo-v23/native.mp4",
      "checks": 14,
      "video_sha256": "61765D3BCDBDF2EFEAE5AFEA6F452E6E72933C9DC5DD76998CB40C26359C0DBE",
      "visual_review": "native-carrier-pickup-crouched-corvo-v23/visual-review.json",
      "scope": "Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim."
    }
  },
  "confirmed": [
    "Exact native Carry45+Equip53 preparation now transmits as variant7 with stable corpse identity; standing and crouched base states remain distinguished.",
    "Preparation uses a bounded local observation clock (300ms authored gather), not a fabricated native scene duration. In keeps the native 1000ms clock.",
    "v33 appends one standing carrier clip to each v32 bank; previous payload is byte-identical.",
    "Both standing roles execute gather/hoist/loaded carrier poses from ordinary player input and native Steam semantics.",
    "Corvo crouched pickup keeps stream and bank active and refuses the standing hoist.",
    "Source/observer/avatar health unchanged; native equipment lease restores after stream stop in captured Emily and crouched Corvo evidence."
  ],
  "limitations": [
    "Carrier gather remains visually deep.",
    "Native flags and holster tag are verified, but extended holstered blade appearance still needs work.",
    "The remote body remains on the floor in these carrier-only recordings.",
    "Crouched gather, no-preparation pickup, interruption cases, matched live victim path and arbitrary corpse authority remain incomplete."
  ],
  "complete": false,
  "visual_accepted": false
}
````
