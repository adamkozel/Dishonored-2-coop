# ghidra-bookmarks-additions-v341.json

Original: [work/movement-integration/ghidra-bookmarks-additions-v341.json](<../../../../work/movement-integration/ghidra-bookmarks-additions-v341.json>)

SHA-256: `1D0FE000A13BB4A9CA4550743F3509EB6D5D4521013B65BFEFA1B0C5EC0CF20B`; 2671 bytes.

## Original saved record

````json
[
  {
    "address": "1408784b0",
    "category": "D2Coop/SwordVisibility-v26",
    "comment": "CONFIRMED v26 runtime A3A1542B: exact clone inventory child hide/show for both roles. Four stance/equipment transitions per role, unarmed directional movement, pause/resume restore pass (20 checks); paired-pickup movies both roles pass44 checks each and temporal sheets inspected. Native flags+d0 bit0 and same inventory child independently observed; blade absent at holster and returns on cleanup. These funcs update renderer/descendants via87e040/87e0a0. No physical sword folding or complete carry claim. Evidence work/natural-moveset/sword-visibility-findings-v26.json.",
    "type": "Note"
  },
  {
    "address": "1408784b0",
    "category": "D2Coop/Possession-visibility-v336",
    "comment": "CONFIRMED static exact Steam exe: native entity Hide8784b0 sets entity+D0 bit0 and calls87e040; Show87c590 clears bit0. DLL338 runtime339 PASS14: protocol50 Corvo-only semantic17, internal1 transfer/8 occupied, finite250ms generation/epoch/session/token-owned exact clone lease; 21 hidden samples,16 bracketed stable occupied samples,exactly1hide/1show,zero refused/lost. Peer video confirms disappearance; return flag restored but return camera occluded. Internal8 keeps unselected Possession component STARTED; internal1 stopped. No player/NPC/collision calls. Interruption/newworld/alternatecontexts pending. Remote NPC control/VFX/shared gameplay unimplemented. Evidence work/natural-moveset/possession-visibility-network-v339.",
    "type": "Note"
  },
  {
    "address": "140824e40",
    "category": "D2Coop/Possession-state-sentinel-v326",
    "comment": "CONFIRMED exact exe: Possession internal14 inactive sentinel, separate from player physics stance18. Native human target weak reference generation/entity/NPCPossession membership validated. Runtime335 passes12: successful release400ms phase28 under exact EnterPossessedHost26,vtable1c79878,internal0,quantity1/unselected/stopped; one scene233/event10 sent and accepted with6 remote bank52 release samples. Runtime339 DLL338/protocol50 passes14: transfer internal1 stopped/unselected, occupied internal8 STARTED/unselected; finite exact Corvo clone hide lease maintains21 hidden samples incl16 bracketed stable occupied snapshots; normal-input exit+native visibility restore. Transition reads unknown, not quantity0. Office-key0to1 native progression separately recorded, causal attribution unproven; full checkpoint inventory/health and protected14 restored. No NPC authority, gameplay outcome or VFX replication yet. Evidence work/natural-moveset/possession-visibility-network-v339.",
    "type": "Note"
  }
]

````
