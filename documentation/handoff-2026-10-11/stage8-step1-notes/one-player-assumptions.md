# Stage 8 step 1: where the game assumes one player

Read-only research for [#234](https://github.com/adamkozel/Dishonored-2-coop/issues/234), done on 2026-10-29 with d2-ghidra and `d2_resource_index.py` on `work/asset-game/base`. Node counts cover every `maps/campaign/**/*.entities`; the game-derived extracts stay local. Labels: **CS** confirmed static, **HANDLED** already handled by the mod, **H** hypothesis. The main session re-checked `0x1408a8ba0`: it returns entity-table slot 0 (`game+0x3130b0`, index 0).

## A. The engine's notion of "the player"

- **A1. The local-player getter `0x1408a8ba0(game)`** returns entity-table slot 0, handle 1. The game singleton is `0x1439a5290`. There are about 145 direct callers, plus wrappers:
  - `0x1405bef90`, the player;
  - `0x1405befa0`, its component `0x1439aed60`;
  - `0x1405bef00`/`0x1405beea0`, the player's +0x390 slot entity;
  - `0x1405befe0`, equipment slot 9.

  There is no player list, so every one-player question resolves to handle 1. CS.
- **A2. `IsLocalPlayer(handle)` `0x1405bf220`** checks `handle == player+0xa8`, with 20 callers. Variants:
  - `0x1405bf2c0`;
  - `0x1407806e0`, which also accepts a player-owned entity (parent +0xac == player, flags 0x9000).

  CS.
- **A3. Kiscule events carry the local player as instigator** (`0x1407a07b0`, `0x1407a45e0`, `0x1407ac360`, `0x1407ad220`, `0x1407ad570`). The script "Player" is `arkKisculeNodePlayer`: 1385 nodes in 13 map folders. A script cannot address a second player. CS for the handler pattern.

## B. Script commands that mean "the player" (area 4.1 unless noted)

- **B1. Player-class trigger filters:** 1953 volumes filter on `arkPlayer` and 771 on `arkNPC`.
  - HANDLED: the trigger mirror replays crossings for the other world's own player (chapters 89 and 110).
  - H: the partner rig, being an NPC, may fire `arkNPC` volumes in the host's world.
- **B2. "Player sees":** 780 `PlayerLookatNotification` nodes in 11 missions, for example the Wall of Light and Jindosh's `found_electro_therapy`. The partner's gaze never reaches the host's scripts. Not mirrored. CS.
- **B3. Player-scoped actions** act on the player of the world that runs them. CS.
  - The actions: `Cinematic` 534, `ActionTeleport` 714, `ActionPlayerInventory` 205, `ActionPlayerLoadout` 22, `ActionPlayerHands` 28, `PlayerOutsiderMarkControl` 12, `ActionMoney` 8, `AddItem` 20, `ActionPlayerStatisticsControl` 49, `ActionChaosDeedControl` 98, `ActionAchievement` 97.
  - When the partner's scripts stop (4.1), each of these must still reach her own player. Run by the host, they would hit the host.
  - HANDLED for 5 classes (`kiscule_action_mirror.h`). Not covered: cinematics, teleports, hands, money, statistics, achievements, chaos deeds.
- **B4. Inventory gates read the local player only:** `GetInventoryItemQuantity` 127, `CompareInventoryItem` 28, `PlayerInventoryNotification` 9, `GetPlayerMoney` 21. They check keys, `imperial_ring`, the Heart and notes. CS.
  - A partner holding the item does not satisfy the host's script.
  - With per-player items, story items must reach both players (4.1, 4.2).
- **B5. Other player-state listeners:** `PlayerState` 37, `PlayerPower` 10, `PlayerStance`, `PlayerHands` 7, `PlayerMoney`, `EntitiesToDistance` 49, `LocationsToDistance` 4. The classes are CS; which of them are wired to the Player node is H.
- **B6. AI relations to the player:** `ActionNPCRelation` 321, `NpcPsychicAwareness` 21, `ActionNpcKnowledgeControl` 17, `AIAttentionStateChanged` 210.
  - The tutorial trainer has an ENEMY relation with `Player` only (chapter 106, CS).
  - H: other scripted hostility, such as the end-of-mission ambushes, ignores the partner (4.1, 4.5).
- **B7. Use replay posts `_user` = handle 1** (HANDLED, #187). H: a partner's scripted use that opens a `Cinematic` would lock the host's player when the host runs it (4.3).

## C. Story state stored on the player (4.1)

- **C1.** Gameflow state changes write the local player's objectives component (`0x1439c5ac0`, through `0x1404ea090` and `0x1404e9db0`) and post a player event (`0x1404ea190`).
  - Mission completion writes the local profile (`HasFinishedCampaign_%d`, `HasFinishedNewGamePlus`) and stat 0x22. CS.
  - The partner's journal, HUD and profile update only when her own world's native set-state runs (HANDLED by the gameflow and mission-end mirrors).
  - Step 4.1 must keep this native apply, even with zero partner scripts.

## D. Kills, chaos, stats and summaries (4.1, 4.5)

- **D1.** Chaos (`arkCpntPlayerChaos +0x24`) accrues in `0x1404c14b0` only when the killer passes `0x1407806e0` or `0x14071b690(victim)` holds.
  - The host replays partner damage with `attacker = avatar.handle` (`npc_combat.h`).
  - So the host's chaos, which the partner copies (D2CH), probably leaves out the partner's kills. Both code paths are CS; the gap has not been measured at runtime.
- **D2.** Mission statistics (`game+0x1f77c8`): kill and detection stats 0xb, 0xd, 0xe and 0xf count only kills by the local player or player-owned entities. `D2_Health_ApplyLethal` posts message 0x73 "killed by player" only for handle 1. CS. Each result screen shows its own player's stats.
- **D3.** Achievements (97 nodes, plus campaign completion): under 4.1 the partner gets none unless they are dispatched to her (H).

## E. Interactions the host would run "as the partner" (4.3, 4.6)

- **E1.** Door use `0x14074b6e0` branches on `IsLocalPlayer(interactor)`. CS.
  - Otherwise it takes the NPC path `0x14074dd50`, which reads NPC component `0x1439e8010 +0x2bb4` with no null check (H: a crash if the rig lacks that component).
  - `0x140749ce0` (door AI and auto behaviour) also reads the local player.
- **E2.** Pickups run in the player's own loot state (`coin_native_api.h`). `0x14076e2b0` checks the local player's hand slot 0x14. There is no "pick up as partner", which fits the per-player design. CS.
- **E3.** Takedown awareness `TargetIsUnaware` → `0x1405bf560` is judged against the local player (chapter 100). H: partner takedowns run on the host are judged against the host (4.6).

## F. AI that only considers the local player (4.5)

- **F1.** Behaviour-tree conditions and events compare the AI target with handle 1: `0x1405cf2c0`, `0x1405b1020` (event 0x4f), `0x1405a1de0`, `0x1405aecf0`. CS. Player-specific reactions, probably alerts and search escalation (H), won't fire for the partner's avatar.
- **F2.** HANDLED: native perception of the avatar, targeting and pursuit, and enemy contacts routed to the real remote player (chapters 77 and 78), plus interaction-sensor exclusion (chapter 34).

## G. Per-world randomness (4.2, 4.3, 4.4)

- **G1.** One game RNG (an LCG at `game+0x600f8`), drawn by about 40 functions: AI random bools, parry chances, FX and loot spawns. Any difference in draw order makes the worlds diverge, so the host must roll rather than match seeds. CS. H: Kiscule `RandomInt`/`RandomFloat`/`RandomTime`/`ExtendedRandomInt` (301/175/161/12 nodes) use it too.
- **G2.** Safe combinations come from `script_kiscule_eotw_script`: Level Loaded → Randomize Combination → Format Combination into Note (`digits_to_combination.kiscule`, `SafeMain.ResetCombination`). They differ per load (chapter 106, confirmed runtime). The use mirror excludes `safe`/`digit`/`combinaison`. Jindosh's `EnigmaExpectedStage*` also differs (chapter 110).
- **G3.** Random bonecharms: `whale_bone_charm_01.def` inherits `pickablebonecharm_random`. CS; the roll is clock-seeded per world (see [state writers](state-writers.md)).
- **G4.** NPC belt loot: `loot_base` and 357 `m_basicLootOverride`. The pickup census varies between loads and by character (chapter 83, confirmed runtime). The cause is H.
- **G5.** Load scripts run again per world (LoadingEnd and Level Loaded graphs; 8 script spawners respawn with new handles, chapter 106) (4.4).

## Top 10 by risk

1. B3: player-scoped script actions hit the wrong player once the partner's scripts stop, or when the host runs them (4.1).
2. A1/A2: no second-player concept; handle 1 is hard-coded at about 165 sites. Running as the partner needs per-feature substitution, not a global swap.
3. D1: shared chaos leaves out the partner's kills (4.1, 4.5).
4. B4: story gates check only the host's inventory (4.1, 4.2).
5. E1: native door use with a non-player user takes the NPC branch, with a possible null dereference (4.3).
6. G1/G2: one shared RNG; safe codes and riddles roll at load (4.3, 4.4).
7. B2: look-at nodes; the partner's gaze never advances the host's story (4.1).
8. B6/F1: scripted hostility and AI escalation keyed to the local player (4.1, 4.5).
9. D2/D3: result stats and achievements count only the local player (4.1).
10. G3/G4: random bonecharms and NPC loot differ per world (4.2).
