# ghidra-bookmarks-additions-v345.json

Original: [work/movement-integration/ghidra-bookmarks-additions-v345.json](<../../../../work/movement-integration/ghidra-bookmarks-additions-v345.json>)

SHA-256: `5B5D6C2C8B4B2D4F9DBE7095DA0D3F21C4C16EC0059FCC70CC8683C31387ED1E`; 3765 bytes.

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
    "comment": "Exact B671 exe native entity Hide8784b0/Show87c590 sets/clears entity+D0 bit0 and calls87e040. DLL338339/342 PASS14 for finite Corvo-only generation/epoch/session/token-owned clone visibility lease, no local-player/NPC/collision writes. DLL343345 PASS15: normal source exit and remote movement continuous; Corvo visibly returns in close view but overlaps unsynchronized native guard.344 PASS14 receiver-stop:7 hidden occupied baseline samples,25 restored/disarmed rows incl3while source still possesses host,exactly1hide/1show,zero refused/lost. Source firstperson preserved.346 menu cleanup observed correct but test failed because script exited host immediately after resume, eliminating sampled occupied interval; retained failure. Native menu/newworld/alternatehost coverage not complete. Evidence work/natural-moveset/possession-receiver-stop-v344 and possession-exit-close-v345.",
    "type": "Note"
  },
  {
    "address": "140824e40",
    "category": "D2Coop/Possession-state-sentinel-v326",
    "comment": "CONFIRMED exact exe: Possession internal14 inactive sentinel, separate from player physics stance18. Native human target weak reference generation/entity/NPCPossession membership validated. Runtime335 passes12: successful release400ms phase28 under exact EnterPossessedHost26,vtable1c79878,internal0,quantity1/unselected/stopped; one scene233/event10 sent and accepted with6 remote bank52 release samples. Runtime339 DLL338/protocol50 passes14: transfer internal1 stopped/unselected, occupied internal8 STARTED/unselected; finite exact Corvo clone hide lease maintains21 hidden samples incl16 bracketed stable occupied snapshots; normal-input exit+native visibility restore. Transition reads unknown, not quantity0. Office-key0to1 native progression separately recorded, causal attribution unproven; full checkpoint inventory/health and protected14 restored. No NPC authority, gameplay outcome or VFX replication yet. Evidence work/natural-moveset/possession-visibility-network-v339.",
    "type": "Note"
  },
  {
    "address": "140824e40",
    "category": "D2Coop/Possession-exit-v343",
    "comment": "Runtime finding tied to native Possession exit (not a rename of this internal state sentinel):339/342 exit defaultMode Running0+EquipChange64+53 owns distinct power_equip Scene_1/333ms on left arm and sword_equip Scene_2/1066ms on Corvo right arm+sword. Old paired reader only accepted unequip ->state stale. DLL343 validates exact Corvo standing dual draw plus reciprocal arms, handle/duration/classes and double-read identity.343 PASS15:3owned dual samples,6remote draw samples,18ready post-exit rows,5walking samples,0.4907341m native backward walk without rearm.345 repeat PASS15 with closer camera confirms visible Corvo return but native NPC overlap. No firstperson change, damage or NPC control. Probe34398254B03...,DLL3437AF4A971...,protocol50,bank52/316. Complete NPC authority/VFX and co-op remain unfinished.",
    "type": "Note"
  }
]

````
