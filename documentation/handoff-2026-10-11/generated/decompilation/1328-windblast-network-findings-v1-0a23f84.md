# windblast-network-findings-v1.json

Original: [work/natural-moveset/windblast-network-findings-v1.json](<../../../../work/natural-moveset/windblast-network-findings-v1.json>)

SHA-256: `C6A96D2C9010E73A3ECCF9C1AC22317629A1C354C948CC4F407BA8815DDE23FB`; 1827 bytes.

## Original saved record

````json
{
  "exact_game_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "dll": "80E9E28879F84E427BF05BFEC45357430EE181376C9A1669CA28D4A5E9670F8A",
  "confirmed": [
    "Native FX ID map provides distinct basic Windblast effects owned by the source native windblast object.",
    "Native FX record addresses are reused; ID, owner and declaration must be checked.",
    "Original wblast_blast_01.fx produces unwanted observer fullscreen distortion when used remotely.",
    "Separate windblast_remote.fx reuses native world particles/sound and removes private screen effects.",
    "Both standing drawn and holstered native casts produce exactly one remote effect with independently matched native position and orientation.",
    "Replayed reliable packets are rejected; finite clone effects stop and tracked ownership clears."
  ],
  "tests": [
    "windblast-corvo-network-v28-v1/result.json",
    "windblast-corvo-empty-network-v28-v1/result.json"
  ],
  "asset_install": "outputs/d2-coop-lab/research/windblast-presentation-asset-install.json",
  "asset_source": "outputs/d2-coop-lab/assets/windblast_remote.fx",
  "remaining": [
    "Standing stationary basic Corvo Windblast only.",
    "Fine hand contact and continuous visual quality still require acceptance.",
    "Crouched, travelling, aerial, upgraded and impact variants remain incomplete.",
    "Remote physics/damage outcomes are outside this cosmetic effect proof.",
    "Recordings contain no audio; sound is not audibly verified."
  ],
  "hypotheses": [
    "The map reader can support upgraded Windblast effects after separately proving owner, phase and visual scope.",
    "Native contact-effect observation may support remote impacts; no office-carpet dust contact was observed."
  ],
  "complete": false
}
````
