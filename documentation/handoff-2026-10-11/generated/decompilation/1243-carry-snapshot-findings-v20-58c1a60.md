# carry-snapshot-findings-v20.json

Original: [work/natural-moveset/carry-snapshot-findings-v20.json](<../../../../work/natural-moveset/carry-snapshot-findings-v20.json>)

SHA-256: `1FDE67DBFE61B0CC9618D467B96B9D183EE0588C349B0B45D97A81CCA7009469`; 2223 bytes.

## Original saved record

````json
{
  "build": "82E7775D2AE3752887750E2A48AEFCD22D6E978368AAF2B1DD9B0C85240DE99D",
  "cases": {
    "corvo": {
      "video": "carry-snapshot-corvo-v20/native.mp4",
      "evidence": "carry-snapshot-corvo-v20/snapshot-validation.json",
      "root_follow": true,
      "transition_stamps": [
        {
          "restored_ms": 812,
          "attached_ms": 859
        }
      ],
      "checks": 20
    },
    "emily": {
      "video": "carry-snapshot-emily-v20/native.mp4",
      "evidence": "carry-snapshot-emily-v20/snapshot-validation.json",
      "root_follow": true,
      "transition_stamps": [
        {
          "restored_ms": 813,
          "attached_ms": 859
        }
      ],
      "checks": 20
    },
    "cancel": {
      "video": "carry-snapshot-cancel-corvo-v20/native.mp4",
      "evidence": "carry-snapshot-cancel-corvo-v20/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 593,
      "visual_failure": "Body launches upward before settling; mechanical cleanup passes but pose-to-physics handoff is not accepted."
    }
  },
  "confirmed": [
    "Native recovery snapshot inputs can blend a dead Mayhew body without invoking AI recovery or changing health.",
    "Separate pose blending and retarget activation removes the v18 floor dive in both roles.",
    "Engine-thread transition timestamps resolve the coarse status/probe sampling ambiguity.",
    "Early cancellation restores physics and bindings without creating an attachment."
  ],
  "hypotheses": [
    "The native retarget solver reads an unblended clip anchor while final globalPose includes recovery blending."
  ],
  "rejected": "carry-snapshot-corvo-v18/native.mp4",
  "interim": [
    "carry-snapshot-corvo-v19",
    "carry-snapshot-emily-v19"
  ],
  "remaining": [
    "Fix upward body launch on early snapshot cancellation.",
    "Source-timed full-body carrier lift and matching hands/body contact.",
    "Lower peak victim height and improve shoulder settling.",
    "Automatic native pickup and general body identity/authority.",
    "Throws, other releases and complete movement coverage."
  ],
  "complete": false,
  "visual_accepted": false
}
````
