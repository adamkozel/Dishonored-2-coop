# Stage 8 step 1: where the game writes shared state

A read-only Ghidra survey (through d2-ghidra) plus a source survey, run on 2026-10-29 for [#234](https://github.com/adamkozel/Dishonored-2-coop/issues/234). Labels:
- **CS:** confirmed static;
- **MOD:** already used by the mod;
- **H:** hypothesis.

Source paths are under `outputs/d2-coop-lab/src`. The main session re-checked one finding: `0x14077a880` (the usable apply), whose fourth argument skips messages 0x98/0x99 and the script events `0x1439acf70`/`0x1439acfc0`.

## 1. Gameflow, objectives, story flags and Kiscule

| Writer | What it writes | Mod hook today | Quiet apply or suppression point | Label |
|---|---|---|---|---|
| `0x1404e9bd0` set-state (mgr `game+0x1f7780`, id, state) → `0x1404ea190` | `node+0xc` = state. Before the write it posts event `0x1439ab800` on the worldspawn (`0x1408a8ba0`) through `0x1408848a0`, then the HUD notice `0x1404e9a30` and, for missions, `0x1404ea020` (stats `0x1404e9db0`; state 3 → logic 11 Results) | `mission_end_mirror.h` (D2GF/D2ME) calls it raw on the partner | A raw `node+0xc` write (the same lookup) plus, optionally, `0x1404e9a30` alone for the HUD. Today's path fires the change event, so the partner's own listeners run | CS / MOD |
| Callers of `0x1404e9bd0`: `0x14040eba0`/`0x14040f060`/`0x14040f760` | Each drains a request queue (`0x1403e3d30`/`0x1403e4400`/`0x1403e4440`) against a 7×7 transition table | none | Partner suppression: don't drain (or don't push) these queues | CS (queue) / H (who pushes) |
| Kiscule signal dispatch `0x1407c0e30` | Runs every node input handler (`signal+0xc` node, `+0x10` input, `+0x18` instance) | `kiscule_action_mirror.h` (5 classes mirrored) | **The choke point for "the partner runs zero story scripts":** an allow-list by node class. Presentation stays local; nodes that write the world are blocked | CS / MOD |
| Story-flag bools at `var+0x19` (vtable `0x141cc4388`; set slots `0x1408f3010`/`0x1408f33a0`/`0x1408fb8c0` write it directly, with no notify). Ints work the same way (vtable `0x141c9f998`) | Decisions: alive flags, non-lethal outcomes, robberies | `story_flag_mirror.h` (raw write, D2SF) | **The raw write is already quiet** | CS / MOD |
| Trigger enter/exit `0x140779e00` | Posts the script event and marks once-triggers | `trigger_mirror.h` | The partner's call becomes an intent; the host calls it with the partner as the actor | CS / MOD |
| Event posts `0x140883f80`, `0x140884920`, `0x1408848a0` | All entity script events | `kiscule_action_mirror.h` hooks the first two. `0x1408848a0` can't be hooked (the coin API needs its bytes, `coin_native_api.h`) | Suppress gameflow at `0x1404ea190`'s caller, not at the post | CS / MOD |
| Level transition `0x1407a1340` → `0x1404f1d40` | MapTransition | `mission_end_mirror.h` | The host decides; the partner applies `0x1404f1d40` | CS / MOD |

## 2. Mission decisions, kills, stats and chaos

| Writer | What it writes | Mod hook | Quiet apply or suppression | Label |
|---|---|---|---|---|
| `D2_Health_ApplyLethal 0x14071a9e0`, `D2_Health_ApplyDamage 0x140717950` | NPC death and health; calls stats | NPC outcome relay (`npc_knockout.h`) | It runs the real death, so events, flags, chaos and stats all fire. On the partner a death must arrive as a result: set health and pose quietly, and suppress chaos and stats | CS / MOD |
| Stats `0x14067aea0(game+0x1f77c8, id, delta)` | Statistics arrays | read only | Callers: ApplyLethal, inventory add, spawner (id 0x14), mission `0x1404e9db0`. Whether stats are per player is a policy decision | CS |
| Chaos `0x1404c14b0`: `+0x24 += outcome`, through the arkRule handler for message 0xa5 | Chaos points | `mission_end_mirror.h` (host publishes D2CH every second; the partner raw-writes `+0x24`) | Suppress the partner's own `0x1404c14b0` add. A partner kill must reach the host as an outcome. Script deeds: `arkKisculeNodeActionChaosDeedControl` (vtable `0x141ce82c8`), `arkMsgAddChaos` | CS / MOD |
| Mission thresholds `0x1404ea390` | The chaos level at mission end | `mission_end_mirror.h` | Follows the host | CS / MOD |

## 3. Items and inventory (per player)

| Writer | What it writes | Mod hook | Per-player handling | Label |
|---|---|---|---|---|
| LootObject reward `0x140841030` | Grants the pickup | `coin_sync.h` (observer) | Run locally only. Stop sending the removal, except for story items | CS / MOD |
| Claim `0x14076e2b0`, finish `0x14076ec20` | `+0xd0` flags; finish calls the owner's vtable `+0x80` (hide/remove) | `coin_native_api.h` | Removing a pickup from your own world is finish; don't mirror it | CS / MOD (vt+0x80 = hide: H) |
| Inventory add `0x1406156e0` (money: `0x140615620`) → `D2_InventoryEntry_AddClamped 0x14063c9f0` | Stacks; event `0x1439ab8f0`/`0x1439ab850`, message 0x9b, stats | catch-up, capacity `0x140618de0` | Draws the game RNG (`0x1408ac040`) for random ±1 ammo; fine per player | CS |
| Story-item grants: Kiscule `Inventory` node (vtable `0x141b7ddf8`), accessor `0x140534540` | Keys, the Heart, the Mark, loadouts | `kiscule_action_mirror.h` (host counts replayed) | Already host → partner; keep it | MOD |

## 4. RNG

- **Game RNG:** seed at `game+0x600f8`, an LCG ×0x19660d +0x3c6ef35f. Wrappers `0x1408ac000`/`040`/`090`/`0d0`/`110`/`150`; about 99 inline sites. One stream is shared by AI, physics, combat and loot, so it diverges between the worlds no matter what. Host-only rolls must be sent as results; syncing the seed is not enough. CS.
- **Random bonecharm roll `0x14060fad0`:** it saves the seed, reseeds from `GetSystemTimeAsFileTime`, shuffles, then restores the seed.
  - It is called from pickup Start `FUN_140725d30`, which stores the result at component `+0x108`/`+0x110`/`+0x138`. It skips this when restored (`+0x1c` bit 30).
  - The roll is clock-seeded, so it always differs per world.
  - Quiet apply: write the host's roll into the partner's component.
  - Label: CS; the meaning of the fields is H. Under the per-player item rule the host still rolls once, so both players see the same charm.
- **Spawner spawn `0x14075b770`:** random point through `0x14075a8a0` (game RNG). CS.
- **Kiscule `RandomInt`/`ExtendedRandomInt`/`RandomBool`/`RandomFloat`:** vtables `0x141cbe168`/`0x141ca13a8`/`0x141cb6ea8`, factory `0x1408b9110`. The bool evaluator `0x1407ccbd0` mode 3 is `0x1408ac0d0(2)`. Classes CS; handlers H.

## 5. Doors, levers, usables and lifts

| Writer | What it writes | Mod hook | Quiet apply or suppression | Label |
|---|---|---|---|---|
| Door transition queue `0x14074a400` + resume `0x1407e79a0`; moving `0x14074c5c0`/`0x14074e7a0`/`0x14074cd10`; entry `0x14074ca10`/`0x14074cba0` | Openable state machine | `door_native_queue.h`, `door_fixture.h` | **Restore path `0x14074adf0`** sets the state machine's `+0xc` to 0/2 directly, with no Moving transition. It is the candidate quiet apply; entry notifications may still fire | CS / MOD (quiet: H) |
| Lock `0x14074e4c0(c, lock, recurse)` | Lock byte `+0x1e8`, the unlock event, owner dirty | `door_native_queue.h` | No quiet flag; a raw `+0x1e8` write plus dirty | CS / MOD (raw: H) |
| **Usable apply `0x14077a880(c, state, transition, quiet)`** | `+0x28` state, animation. Script events `0x1439acf70`/`0x1439acfc0` and messages 0x98/0x99 only when quiet == 0 | `usable_native_api.h` | **quiet = 1 is a native path without side effects**; load restore `0x14077b3b0` uses it. Re-checked by the main session | CS / MOD |
| `0x14077b870` (by state name), `0x14077ad80` (reset) | Script set-state requests; callers at `0x140496`–`0x140498` (Kiscule ActionUsable, H) | none | Partner suppression point for script-driven usables | CS / H |

## 6. Safes and combination dials

- The dials (`script_combinaison_numbers_*`, 10-state usables) are rescrambled per world on every load (chapter 106, confirmed runtime).
  - Usable Start `0x14077af40` and restore `0x14077b3b0` contain no randomness (CS).
  - So the scramble comes from a script: Kiscule Random* → ActionUsable set-state (H).
- The Jindosh `EnigmaExpectedStage*` values are Kiscule non-bool variables on the worldspawn (chapter 110). They can be raw-written like story flags (H).
- **Fix:** the host sends the dial states (usable apply, quiet = 1) and the expected values (raw write), and the partner's random nodes don't run.
- The Dunwall safe doors are plain 2-state usables.

## 7. NPC spawn and enable

- **Spawner `0x14075b770`:** creates through `0x14075b250` → `D2_Spawner_ApplyComponentOverrides 0x140759e70`, with a random transform. It posts spawned `0x1439ad100` / failed `0x1439ad150`, message 0x17d and stats 0x14. CS.
  - Partner suppression point.
  - Apply: the host's intent plus the host's transform.
- **`D2_Game_SpawnActor 0x1408a78f0`:** `climb_fixture.h`. CS / MOD.
- **Script nodes:** `arkKisculeNodeEntityStateControl` (`0x141ccc978`), `ActionSpawner` (`0x141cdaf08`), `EntitySpawnerControl` (ctor `0x14079da60`), `ActionTeleport` (factory `0x140900040`). Classes CS; handlers H.

## Open runtime checks

1. The Kiscule allow-list: which node classes can be blocked on the partner without breaking dialogue, cinematics or the HUD. Measure the run count per class per map on the partner.
2. Gameflow quiet apply: does a raw `node+0xc` write plus `0x1404e9a30` keep the partner's objectives and markers right?
3. The source of the dial and Enigma scramble: a hardware breakpoint on a dial's `+0x28` at load.
4. The bonecharm roll: where it lands, whether it survives a save, and whether a pre-write stops the reroll.
5. Door snap `0x14074adf0` outside a load; the raw lock write.
6. Usable quiet apply in the event trace.
7. Spawner divergence: compare spawn transforms between the two worlds.
8. Chaos: hook `0x1404c14b0` on the partner; a partner kill reaches the host.
9. The name of event `0x1439ab800`.
