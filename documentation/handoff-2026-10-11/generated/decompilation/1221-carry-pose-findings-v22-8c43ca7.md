# carry-pose-findings-v22.json

Original: [work/natural-moveset/carry-pose-findings-v22.json](<../../../../work/natural-moveset/carry-pose-findings-v22.json>)

SHA-256: `B21C15EBDFA53CE15F53180FE33DA6761F26EE8CD768B38E4DCE94D1EF408172`; 3847 bytes.

## Original saved record

````json
{
  "build": "F687BAA10340D23F4AFF68961EAB45C0E2CF941876346E7F770AB7324ECA759E",
  "cases": {
    "carry-snapshot-cancel-emily-v22": {
      "role": "emily",
      "video": "carry-snapshot-cancel-emily-v22/native.mp4",
      "evidence": "carry-snapshot-cancel-emily-v22/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 577,
      "video_sha256": "81AF55C39A5AF26483381C95CCBD6A0E99528EF1D88187E624F0FE78340564A0",
      "actual_capture_hz": 27.272727272727273,
      "visual_observations": "Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result."
    },
    "carry-snapshot-cancel-corvo-v22": {
      "role": "corvo",
      "video": "carry-snapshot-cancel-corvo-v22/native.mp4",
      "evidence": "carry-snapshot-cancel-corvo-v22/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 293,
      "video_sha256": "D13B7902B8ACF1F6561CBE41C19EF57C7F1FD4A503EB594B837C95CEFB3BBFD8",
      "actual_capture_hz": 26.863636363636363,
      "visual_observations": "Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result."
    },
    "carry-snapshot-cancel-emily-v22b": {
      "role": "emily",
      "video": "carry-snapshot-cancel-emily-v22b/native.mp4",
      "evidence": "carry-snapshot-cancel-emily-v22b/snapshot-cancel-validation.json",
      "checks": 13,
      "cleanup_delay_ms": 560,
      "video_sha256": "22F52523007CAC21C32E3E87B538CF12FB567D64CD0FCE7B3C48ECD8E02A1F78",
      "actual_capture_hz": 27.0,
      "visual_observations": "Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result."
    },
    "carry-snapshot-stream-emily-v22": {
      "role": "emily",
      "video": "carry-snapshot-stream-emily-v22/native.mp4",
      "evidence": "carry-snapshot-stream-emily-v22/snapshot-cancel-validation.json",
      "checks": 15,
      "cleanup_delay_ms": 732,
      "video_sha256": "223208D4ADCCE1073B8FAA4F2E9759E4DCAF9865341ED039AC810D0D10E0D7BC",
      "actual_capture_hz": 26.727272727272727,
      "visual_observations": "Source-stream loss occurs later in the blend than direct cancellation, so the body is already raised farther. It returns to ragdoll and settles; restarting both action streams does not recreate the retired attachment."
    }
  },
  "confirmed": [
    "734d90 copies current native joint transforms into rigid bodies and clears linear/angular velocity.",
    "Calling this exact native routine before requesting ragdoll, with the pose blend still bound, removes the observed v21 Emily launch in two cancellation recordings.",
    "Corvo also settles correctly in the corresponding case; mechanical cleanup/health/Steam transport checks pass.",
    "Emily action-stream loss restores physics and scene ownership; fresh two-sided handshake does not replay the retired attachment."
  ],
  "hypotheses": [
    "Residual animated rigid-body velocity and/or unsynchronized body transforms contributed to the previous impulse. This experiment changes both; the individual cause is not isolated."
  ],
  "remaining": [
    "Full-body carrier gather/hoist acting, synchronized with the actual native pickup clock.",
    "Matching hand contact and victim path without floating, head overlap or foot sliding.",
    "Automatic native pickup, arbitrary corpse poses/types and authoritative world identity.",
    "Throws, remaining releases, other movement bindings, powers and native VFX."
  ],
  "complete": false,
  "visual_accepted": false
}
````
