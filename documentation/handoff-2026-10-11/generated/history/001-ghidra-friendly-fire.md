# Historical note: outputs/d2-coop-lab/analysis/ghidra/friendly-fire.md

Original: [outputs/d2-coop-lab/analysis/ghidra/friendly-fire.md](<../../../../outputs/d2-coop-lab/analysis/ghidra/friendly-fire.md>)

SHA-256: `7171ABCF288AA6CB2C74A88FF7A352FE48E632ADE7E06F7D749FFBC6EAD57B6B`; 4104 bytes.

````markdown
# Friendly-fire filter, build 345C

Verified on the exact B671F824 executable, 2026-10-09. The live isolated build is `345CD1D409BE275C6817C96C5E5C8F5CB64FDCFAF88715B2E4DFBAB5E82CE303`.

The health entry at RVA `0x717950` now rejects friendly damage before native health, reaction, death and queued-damage handling. Target identity must be the actual local player or the current, locally spawned custom Corvo/Emily Doppelganger. Source attribution uses the native attacker (`params+0x54`) or inflictor (`params+0x50`), following generation-validated parent handles up to eight entities. The native empty-result constructor at `0x780470` returns a normal zero-damage result. There are no health restoration writes, global invulnerability settings, faction changes or new network messages.

The current game/actor/handle publication is retired when the world or avatar identity is replaced. Protection persists when presentation follow is stopped; it does not rely on an animation lease. The older outgoing presentation-damage firewall remains in place to prevent duplicate animation-event damage.

Confirmed evidence:

- All build checks pass, including dedicated identity fixtures for stale handles, wrong definitions, foreign victims, source ownership cycles, environmental/self/enemy exclusions and concurrent world/victim replacement.
- Both roles passed six actual native-entry calls: direct actor and attached sword attribution in each direction, plus assassination declarations in each direction. All returned zero damage; player health remained 56 and partner health 40.
- Normal controller sword attacks produced three blocked contacts per character. Both instances retained player health 56 and avatar health 40. This is contact evidence, not merely a swing that missed.
- The same native-entry tests passed again after the checkpoint was reloaded (world epoch 11 to 21). Paused pairs and duplicate developer test requests were rejected.
- Positive controls still work: a native enemy sword hit causes 15 damage; a player-attributed assassination kills the selected ordinary guard. Protected original save/config files, Steam executable and Steam DLL passed integrity checks.
- The prior upright-crouch implementation is byte-identical in source and has been re-enabled in both instances. Native SteamNetworkingSockets012 remains active.

Reports: `research/friendly-fire-native-first-345C.json`, `friendly-fire-native.json`, `friendly-fire-strikes.json`, `native-sword-damage-verification.json`, `native-enemy-damage-verification.json`, `baseline-integrity-verification.json` and `doppelganger-crouch-live-enable.json`.

Limits: this is a type-independent health-entry filter for damage with verified player attribution, not proof that every attack reaches that entry with usable attribution. Unattributed indirect hazards, physics impacts, every projectile/power, non-health knockouts and pre-damage takedown choreography remain unverified. The full no-harm requirement remains open for those cases. The older blanket outgoing avatar firewall may also reject authoritative enemy-outcome adapters that nominate the presentation clone as attacker; those adapters need a separately validated application scope as combat replication expands.

Preserved milestones: `work/doppelganger-smoothing-424B`, `work/doppelganger-crouch-upright-303A-research`, `work/friendly-fire-wip-before-mixamo`, and the new `work/friendly-fire-345C`. Original Steam assets were not edited.

Later unattended-state observation: while setting up Blender/Mixamo, the client was found at `(-41.098145,-140.061142,60.965324)` with health 0/game-over, while the remote clone still had health 40 and valid protection. Damage observation was off, so the cause is not established. Evidence is retained in `research/friendly-fire-later-death-345C.json`; do not claim this proves friendly fire or any particular external input. The fixtures were subsequently restored, with both players at 56 and avatars at 40 before the 345C archival manifest was written (epoch 26). This incident limits sustained unattended stability claims.

````
