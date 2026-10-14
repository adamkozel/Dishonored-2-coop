# crouch-balance-findings-v1.json

Original: [work/movement-integration/candidate-balanced-crouch-80E9-v3/crouch-balance-findings-v1.json](<../../../../work/movement-integration/candidate-balanced-crouch-80E9-v3/crouch-balance-findings-v1.json>)

SHA-256: `3F868E47B86D73CB3943034634E467CBDD832C25A75CAA598B956B94D6540F6D`; 1788 bytes.

## Original saved record

````json
{
  "complete": false,
  "confirmed": [
    "Both character comparisons favor the complete-body Posture-100 mean idle over the low asymmetric previous rest pose.",
    "Standing close-up views show closed native grips on both characters.",
    "Crouched Windblast revision 3 passes two native casts, matching native world effect placement and cleanup, with a forward casting gesture.",
    "Holstered forward/backward travelling Windblast preserves body ownership, gait advance and source-only private distortion."
  ],
  "rejected": [
    {
      "candidate": "balanced-crouch-power-banks-v1",
      "reason": "Native FX numerical checks passed, but the crouched local shoulder offsets pointed the gesture downwards in game."
    },
    {
      "candidate": "balanced-crouch-power-banks-v2",
      "reason": "Offline positional arm targets needed 0.312m/0.261m reach clamps; rejected before runtime testing."
    }
  ],
  "candidate": "balanced-crouch-power-banks-v3",
  "recordings": [
    "crouch-balance-corvo-baseline-v1.mp4",
    "crouch-balance-corvo-candidate-v1.mp4",
    "crouch-balance-emily-baseline-v1.mp4",
    "crouch-balance-emily-candidate-v1.mp4",
    "grip-closeup-corvo-v2.mp4",
    "grip-closeup-emily-v1.mp4",
    "windblast-corvo-balanced-network-v28-v3.mp4",
    "windblast-corvo-balanced-network-v28-v3-source.mp4",
    "windblast-corvo-balanced-empty-moving-v28-v3.mp4",
    "windblast-corvo-balanced-empty-moving-v28-v3-source.mp4"
  ],
  "remaining": [
    "Crouched sword orientation needs broader visual comparison across actions.",
    "Other crouched contexts still need rebaking around the balanced rest pose.",
    "Air, lateral travel, interruption and lifecycle cases are not established by these comparisons."
  ]
}
````
