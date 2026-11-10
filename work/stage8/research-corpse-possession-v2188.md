# Corvo corpse possession: static research (v2188)

Static only: d2-ghidra MCP (image base 0x140000000) plus the read-only v324 decl exports in
`outputs/d2-coop-lab/research/asset-exports/possession-upgrades-v324` and `possession-target-types-v324`.
Nothing was launched, built or deployed. Labels: **CS** = confirmed static, **H** = hypothesis.

## 1. Which upgrade unlocks corpses, and is a coop_grant_named grant enough

- **CS (decls).** The five `attributes/possession_*.inventoryitem` decls are all `TYPE_POWER_UPGRADE` (20), each with one modifier:
  - `possession_parasite`: `ARK_ATTRIBUTE_POSSESSION_ALLOW_HUMANS` +1;
  - `possession_decayed_harbor`: `ARK_ATTRIBUTE_POSSESSION_ALLOW_CORPSES` +1 (name string `Upgrade_power_possession_corpsepossession_name`, journal image `j_pwr_Possession_corpse_is.png`);
  - `possession_alchemist`: `ALLOW_POTIONS`; `possession_fast_swap`: `ALLOW_HOST_FAST_SWAP`; `possession_ext_duration`: `POSSESSION_DURATION` +5.

  **decayed_harbor is the right item.**
- **CS.** The grant path works for attributes. `coop_grant_named` calls the native quantity setter `0x140618de0(inventory, decl, 1)`. That calls `0x1406170b0`, which applies the decl's `m_modifiers` list (decl+0x90, count at +0x98) through `0x140616940` whenever the quantity changes. The same path enabled human possession with `possession_parasite` (runtime, v324/v326). The v1833a take logged `granted_named` for both upgrades and `named_power_equip_requested` for the power.
- **CS.** This is how the power checks the attribute:
  - `arkCpntPowerPossession` has size 0x110, ctor `0x140447a20` and vtable `0x141b531c8`.
  - Its init, vtable[3] `0x1404480c0`, resolves five attribute indices by name into power+0xf8, +0xfc, +0x100, +0x104 and +0x108, using `0x14060ce50` on the attribute component at power+0x30.
  - The selection update `0x140448200` reads the attribute value as `float [[power+0x30]+0x48 + idx*0x28 + 0x24] != 0`. It passes:
    - +0x104 as `allowHumans`;
    - +0x108 as `allowCorpses`;

    to the pick function `0x1403c22f0(sensor, ranked, allowHumans, allowCorpses)`. That is the function the partner filter already hooks.
  - Mapping +0x108 to ALLOW_CORPSES is **CS by role**: that flag gates only the ragdoll list (below). The attribute name strings are not in the exe.
  - The pick has three lists. Each list sits at sensor+0x110+cat*0x58 and holds 0x30-byte records with the score at +0x14 and the entity handle at +0x18:
    - cat0 NPC: the entity's `+0xd1` must have neither 0x20 nor 0x08 set. The entity is accepted if allowHumans is set, or if `arkCpntAICharacterDescription` (type `0x1439c4ef0`) has `+0x34 > 1`.
    - **cat1 RAGDOLL (corpses):** the entity's `+0xd1 & 0x20` (killed flag, set by `0x140879410`) or `& 0x08` must be set. The entity is accepted if **allowCorpses** is set, or if the AICharacterDescription has `+0x34 > 1` (H: such non-human corpses need no upgrade).
    - cat2 swarm agents.
  - `ranked = (power+0xf0 > 0)`. The window is reset from decl+0x1ac when the power is armed (vtable[35] `0x140447ba0`). Ranked mode takes the first hit by priority, and a ragdoll comes **last**. After the window it takes the best score of NPC, ragdoll and swarm. **H:** a live civilian or rat in view would win over the corpse while the ranked window runs.

## 2. Native target-validity checks for a corpse

All entries below are CS unless marked.

- **Sensor config** (`sensors/possession.cpntplayersensorpossession`):
  - `CATEGORY_KIND_RAGDOLL`: entityKind `idEntity`, collision layer **`NPC_BODY_RAGDOLL` only**, `m_maximumDistance` 15, sorted, `m_stopRayAtClosestOccluder=false`;
  - occlusion layers RIGID_BODY, MOVER and TRIGGER;
  - target joint `possession_target`;
  - `m_enabledAtStart=false`. The power enables sensor slot 0x19 on arm.

  The selection returns early unless `[sensor 0x19]+0x2d8 >= 2`.
- **Sensor scoring** `0x1403c73f0`. Per category config:
  - the distance must lie in [cfg+0x18, cfg+0x1c];
  - the angle term is `1 - angle/cfg+0x20`;
  - the weights are cfg+0x28 (distance) and cfg+0x2c (angle);
  - the record score must be > 0.

  The possession decl overrides only the maximum distance (15 m). The cone default lives in the class defaults and was not read (**H:** a wide cone). The ragdoll category scores against the record's bounds or joint (cfg+0x15 path), not the entity origin.
- **Sensor candidate filter** `0x1403c26f0`, run per ray hit:
  - The target component is `0x140877720(entity, &arkCpntPossession type 0x1439cba70, 2)`. That is GetComponent. It requires the component to be started (`0x1407e7390` table 2) and prefers the component whose decl+0x60 context mask contains `1 << entity+0x288`.
  - The `cpntCorpsePossession` decl has `m_entityContext = ASLEEP|DEAD`, and `possession_human` has `ALIVE|HACKED|POSSESSED`. **So the corpse component is returned only when the entity's current context (entity+0x288) is DEAD or ASLEEP.**
  - Then `vtable+0xd0 CanBePossessed(comp, 0)` must return true, and occluder hits are filtered.
- **`arkCpntCorpsePossession`.** Name `0x141cef020`; size 0x88; parent `arkCpntPossession` (base ctor `0x1404c2960`, vtable `0x141b63ea8`); factory `0x140934a20`; ctor `0x1404c28c0`; **vtable `0x141b640a8`**; typeinfo `0x1439edcb0` (type-index range at `0x1439edd10`/`0x1439edd14`).
  - `arkCpntNPCPossession`: vtable `0x141b641a8`, size 0xb0.
  - `arkCpntSwarmAgentPossession`: vtable `0x141b63fa8`.
  - **CanBePossessed** `0x1404c3580` (vtable[26], +0xd0):
    - with arg 0 (sensor): it returns false if `entity+0xd3 & 1` or `comp+0x7c & 2`, and otherwise returns `comp+0x7c & 0x20`;
    - with arg 1 (the controller travel and arrival states): it returns `comp+0x7c & 0x20`.
    - Bit 0x20 is set by the base ctor (enabled). Bit 0x02 marks the component as possessed now (vtable[25] `0x1404c4bc0` re-Begins when set).
    - `entity+0xd3 bit0` is set by `0x1405916c0` / `0x14038b1a0`. **H:** this is the body marked for ash or disintegration (Shadow-Kill-style), and such a body can never be possessed.
  - vtable[3] `0x1404c4350`: Begin. It registers event handlers 0x133–0x135 and, for some types, 0x10e, 0x10f, 0x144 and 0x145 (**H:** these toggle bit 0x20).
  - vtable[5] `0x1404c4a30`: activation. If decl+0x130 bit0 is set, `0x1404c41f0` tests an area or volume index against a player component bitset (type `0x1439dc9d0`) and may stop the component. **H:** a level-area restriction.
  - No separate "corpse age", "ragdoll settled" or "in water" test was found in this path. Settled and water are implicit at most: the body must be on the NPC_BODY_RAGDOLL layer to be sensed, and `m_usableWhenSwimming=true` is set on the power.
- **Cast** `0x140447db0` (power vtable[36]):
  - It re-resolves power+0xb8 (generation) and +0xc0 (component) with flag 2.
  - If power+0xc8 is non-null, that weak reference must still be valid.
  - There is **no range or LOS re-check at cast**.

## 3. The start function and calling it from a lab command

- `0x140447db0(arkCpntPowerPossession* power)`:
  1. `0x14061bea0(power)`: the base power apply (mana spend, use count).
  2. The duration: attribute[power+0xf8], plus player decl+0x1a4 if `0x14061c8f0`, times attribute[power+0xfc] if the target is a hound (`0x1406dd490`).
  3. It sets kind bits at power+0xf4.
  4. It calls **`0x140822a70(ctrl = [[power+0xe8]+0x2f8]+0x410, arkCpntPossession* target, weakref* power+0xc8, float xmm3 = powerDecl+0x198 (moveToHead 0.4) or +0x19c (chain 0.5), float [rsp+20] = duration, u32 [rsp+28] = powerDecl+0x1a8)`**.

  If there is no target it calls `0x14061bcb0` (cancel flag +0xa6).
- `0x140822a70`:
  - It stores the target handle at ctrl+0x22c (internal+0x63c) and the target reference at ctrl+0x230 (internal+0x640 generation, +0x648 component).
  - It stores the kind from `0x140823300` at ctrl+0x260 (**internal+0x670**).
  - It enters state 0, or state 0xb then 0xc when chaining.
- **Lab command (H, design).** On the engine thread:
  1. Resolve the entity with `0x1407b5e00(game+0x3130b0, &handle)`.
  2. Get `comp = 0x140877720(entity, base+0x39cba70, 2)`. Require `[comp] == base+0x1b640a8`, `(comp+0x1c & 0xc) >= 8` and `CanBePossessed(comp, 1)`.
  3. Find the player's power (vtable base+0x1b531c8; it must be equipped and armed).
  4. Write `power+0xb0 = [entity+0xa8]`, call `0x1403dab80(power+0xb8, comp)` and `0x1406b26f0(power+0xc8, 0)`.
  5. Call `0x140447db0(power)` **in the same tick**: `0x140448200` rewrites +0xb8 on every update.

  This mirrors the native cast, including mana. The lower-level alternative is to call `0x140822a70` directly with the decl floats above. That skips mana and the power bookkeeping.

## 4. Telling a corpse possession apart

- **CS.** Kind at internal+0x670 (`0x140823300`):
  - **2 = corpse** (`arkCpntCorpsePossession`);
  - 1 = NPC human;
  - 4 = rat, 8 = bloodfly, 0x10 = fish (swarm).
- **CS.** Controller state table (ctor `0x1408216d0`, state = internal+0x5d0, 14 = inactive). The arrival update `0x1408255c0` (state 1) chooses the occupied state:
  - swarm → **2** (leaving 3);
  - **corpse → 4** (enter `0x1408226a0`, exit `0x1408236b0`, update `0x140825520`; leaving **5**);
  - NPC with `comp+0xac & 1` → 6 (leaving 7);
  - other NPC → 8 (leaving 9).

  Travel is 0 and 1 (chain: 12 and 13), exit is 10, abort is `0x140823860`.
- **The corpse signature is:** state ∈ {4,5}, `[internal+0x648]` vtable == base+0x1b640a8, and `[internal+0x670] == 2`. The camera constraints are 25/15/30/30. The corpse decl's `m_playerMode` is `PLAYER_MODE_POSSESS_CORPSE`.

## Why v1833a probably failed (H) and next steps

The v1833a cast was done by the **partner (client)**. In her world the guard was `client_corpse` / `client_paired_corpse` (variant 103), with its pose driven by the host's reverse poses. A native possession needs four things, and the stream may have broken any of them:

- the body is a physics ragdoll on `NPC_BODY_RAGDOLL`;
- `entity+0xd1` has 0x20 set;
- the entity context (+0x288) is DEAD, so GetComponent returns the corpse component;
- the corpse component is started with bit 0x20 set.

The test also aimed at the entity origin + 0.3 m, not at the ragdoll's `possession_target` joint.

**Next:**
1. Add a read-only probe of the items above and of the selection state:
   - entity +0xd1, +0xd3, +0x288;
   - the corpse component's +0x1c and +0x7c;
   - attribute[power+0x108];
   - `[sensor0x19]+0x2d8`;
   - the ragdoll list count at sensor+0x110+0x58;
   - power+0xb0.
2. Run a host-world corpse take first, to isolate native gating from the co-op presentation.
