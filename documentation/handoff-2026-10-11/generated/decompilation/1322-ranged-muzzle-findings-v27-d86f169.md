# ranged-muzzle-findings-v27.json

Original: [work/natural-moveset/ranged-muzzle-findings-v27.json](<../../../../work/natural-moveset/ranged-muzzle-findings-v27.json>)

SHA-256: `AA267DE020A1F94A4AE343F434F2D9390C381F1D2DED8ECF2ABB466B949184F9`; 1585 bytes.

## Original saved record

````json
{
  "complete": false,
  "visual_accepted": false,
  "dll": "829D43447DC206110E4884214EB3374969E4E011A1E2803C561AB308DDD738D4",
  "checks": 16,
  "tests": [
    "ranged-corvo-muzzle-v27-v2/result.json",
    "ranged-emily-muzzle-v27-v2/result.json"
  ],
  "recordings": [
    "ranged-corvo-muzzle-v27-v2.mp4",
    "ranged-corvo-muzzle-v27-v2-source.mp4",
    "ranged-emily-muzzle-v27-v2.mp4",
    "ranged-emily-muzzle-v27-v2-source.mp4"
  ],
  "superseded_comparison": [
    "ranged-corvo-muzzle-v27-v1/result.json",
    "ranged-emily-muzzle-v27-v1/result.json"
  ],
  "confirmed": [
    "Both roles pass eight checks.",
    "Native temporary world-effect owners may lack an entity definition; reader verifies owner generation and stable identity.",
    "Fresh exact role effect IDs are correlated with owned gun firing scenes and bounded source positions.",
    "Shot confirmation is discrete on the delayed Steam timeline; one finite visual start per semantic event.",
    "Cosmetic muzzle uses exact native model tag offset (.38,0,.055) on origin joint; no projectile or damage calls."
  ],
  "limitations": [
    "Standing base pistol, isolated two-round fixture only; no remote projectile or impact synchronization.",
    "No audio captured. The exact native character sound event is selected, but spatial audibility and mixing still need verification.",
    "Moving, crouched, airborne, upgrades and weapon switching remain unfinished.",
    "Final natural body motion, reload contact, grip and complete transition quality remain unaccepted."
  ]
}
````
