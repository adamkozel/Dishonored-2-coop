# sword-visibility-findings-v26.json

Original: [work/natural-moveset/sword-visibility-findings-v26.json](<../../../../work/natural-moveset/sword-visibility-findings-v26.json>)

SHA-256: `4A8D043C1A37BD1E276017B95AA31EB61CF00974C8314B219310EE7E470E1684`; 2175 bytes.

## Original saved record

````json
{
  "build": "A3A1542B9245DBCE5D33FC89952FAD0CC3C90656F8F727C715A6150145130D86",
  "cases": {
    "native-paired-pickup-corvo-visibility-v26": {
      "video": "native-paired-pickup-corvo-visibility-v26/native.mp4",
      "evidence": "native-paired-pickup-corvo-visibility-v26/validation.json",
      "paired_evidence": "native-paired-pickup-corvo-visibility-v26/paired-validation.json",
      "sword_evidence": "native-paired-pickup-corvo-visibility-v26/sword-visibility-validation.json",
      "visual_review": "native-paired-pickup-corvo-visibility-v26/visual-review.json",
      "checks": 44,
      "video_sha256": "5D083A9A68D6E9C70834432C349CF0A5F7D9429F3060CCA04AB65B86D3A20240",
      "role": "corvo",
      "complete": false,
      "visual_accepted": false
    },
    "native-paired-pickup-emily-visibility-v26": {
      "video": "native-paired-pickup-emily-visibility-v26/native.mp4",
      "evidence": "native-paired-pickup-emily-visibility-v26/validation.json",
      "paired_evidence": "native-paired-pickup-emily-visibility-v26/paired-validation.json",
      "sword_evidence": "native-paired-pickup-emily-visibility-v26/sword-visibility-validation.json",
      "visual_review": "native-paired-pickup-emily-visibility-v26/visual-review.json",
      "checks": 44,
      "video_sha256": "50D60A38EAC36841F9E712E0881B6DBC9F47CFF2448073F1E5F80F67E56267E3",
      "role": "emily",
      "complete": false,
      "visual_accepted": false
    }
  },
  "tests": [
    "equipment-visibility-v26/result.json",
    "equipment-visibility-pause-v26/result.json"
  ],
  "confirmed": [
    "Exact clone inventory child retained; no replacement weapon or inventory mutation for visibility.",
    "Native entity hide/show follows confirmed holster/draw and matching native attachment tags.",
    "Both roles: standing/crouched draw-holster and unarmed directional movement; pause retires ownership and resume restores original sword."
  ],
  "limitations": [
    "Sword folding is not animated; the static clone blade is hidden while holstered.",
    "Full carry and whole movement delivery remain incomplete."
  ],
  "complete": false
}
````
