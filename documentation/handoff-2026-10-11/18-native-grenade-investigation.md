# Native grenade investigation and read-only baseline

Native equip/unequip has now been observed and validated on both roles in the isolated Dunwall fixture. The live gameplay DLL is the preserved v52 equip candidate. Cooking, throwing and remote body/prop binding remain unvalidated.

## Exact installation resources

[Void export manifest](../../outputs/d2-coop-lab/research/asset-exports/native-grenade-v52/manifest.json) records the exact executable digest, unchanged source indices, declarations and SHA-256 hashes. It contains the regular grenade inventory entry, weapon component, equipment/throwable definitions, effects references and player Midnight scenes.

The regular grenade inventory declaration uses `TYPE_AMMO`, while referencing `regular_grenade_equipment.def` and `arkCpntWeaponThrowable`. It is not a scalar bullet/bolt entry. The equipped object inherits `grenade_base.def`, uses the native grenade MD6 model and has grenade, weapon, animation, audio and Midnight components. The weapon component selects native equip/unequip, cooking entry/idle, high/low throw, reload, out-of-ammo, slide and slash scenes. Native hand tags are `weapon_attach_tag` and `gun_attach_tag`.

Player scenes list the native left arm, player camera/body and left equipment groups. These provide timing and equipment references; they do not supply a complete third-person body pose by themselves. Existing authored NPC grenade candidates are separate material, not verified player bindings.

## Ghidra evidence

[Saved function and cross-reference transcript](../../work/movement-integration/grenade-research/handle-fire-ghidra-v52.json) records the actual Ghidra queries against B671F824. String `arkCpntWeaponThrowable::HandleFire` at `141b9da40` has a code reference at `1406334ff`, in function `140632f70`. The function is now named `D2_WeaponThrowable_HandleFire`, with a persistent `D2Coop/Grenade-v52` bookmark.

Decompilation shows projectile resolution/creation through `14063b5f0`, linear/angular velocity calls at `14087c2a0`/`14087c280`, and another launch-related call at `140639970`. This is a gameplay path and must not be invoked again for cosmetic remote movement. Function signatures and the precise responsibilities of those callees remain provisional.

The branch on component byte `+0x17c` chooses declaration float pairs `+0x298/+0x29c` or `+0x2b8/+0x2bc`. Runtime read-only observations on both characters show those pairs as 6.4/17.2 and -1/8, matching the exact high/low throw angle/speed fields in the asset. Interpreting `+0x17c` as the low-throw flag remains a hypothesis until a genuine native throw changes it.

## Read-only runtime identities

Both characters' baseline observations confirm:

| Object/field | Verified value in this fixture |
|---|---|
| Inventory declaration vtable RVA | `0x1b9ea48` |
| Regular grenade item type / quantity / maximum | 4 / 0 / 4 |
| Weapon component RTTI / vtable RVA | `arkCpntWeaponThrowable` / `0x1b9d8b8` |
| Equipment declaration RTTI / vtable RVA | `arkDeclCpntWeaponThrowable` / `0x1b9d778` |
| Native equipment slot | 1 |
| Entity vtable RVA / parent handle | `0x1c87d58` / 1 |
| Stable baseline | Unequipped, not started, exact component membership |

An initial reader incorrectly reused the gun/crossbow entity vtable and refused at identity stage 3. Direct entity reads on both characters identified the different grenade vtable; the guard was corrected to that exact value, not removed.

[native_player_grenade.h](../../outputs/d2-coop-lab/src/native_player_grenade.h) now provides a read-only, repeated-snapshot reader with exact declaration/type/owner/component/list guards. The new `--player-grenade` probe command succeeds on [Corvo](../../work/movement-integration/grenade-research/host-reader-baseline-v52.json) and [Emily](../../work/movement-integration/grenade-research/client-reader-baseline-v52.json). The new executable is currently in `work/build/Release/d2_runtime_probe.exe`; the deployed lab probe now uses the v52 pin. No grenade game mutation was needed for these observations.

## Next work

Preserve this evidence, then add a bounded isolated native equip-only fixture with inventory/selection rollback and actual first-person scene observations. Validate genuine scene ownership/timing before binding the existing full-body candidates. Cooking, release, projectile ownership, effects, friendly-fire behavior, interruptions and rollback need separate runtime evidence. Cosmetic animation must not duplicate a grenade, detonation or damage.


## Native equip experiment v52

The first publisher attempt rejected the new command before granting an item. The next take proved native selection and rollback, but selection alone did not start first-person presentation. The fixed harness uses normal X holster/draw input, as the previous ranged-weapon investigation did.

[Corvo v3](../../work/natural-moveset/native-grenade-equip-corvo-v52-v3/result.json) and [Emily](../../work/natural-moveset/native-grenade-equip-emily-v52/result.json) each pass seven checks: native equip/unequip scenes, crouched holding, no observed cooking/throw, exact original item rollback, full inventory/health preservation and fourteen save/settings hashes. Both local first-person sheets were visually inspected. The native grenade is visibly held after ordinary draw; this is not evidence of a remote grenade prop or full-body animation.

Exact `gadget_grenade_equip` lasts 500 ms and binds the owned native left arm plus grenade entity. `gadget_grenade_unequip` lasts 233 ms and binds only the owned left arm. Both use `Scene_1`. During combined sword/grenade equipment changes, states 53 and 64 share the EquipChange vtable; state 53 can refer to the sword scene while the grenade unequip scene still runs separately. Scene ownership must therefore be checked through exact arm/equipment bindings, not inferred from the state name or item-selected bit. Full scene/state observations and exact harness copies are retained beside each result.

The twelve-second fixture uses native quantity/equip/unequip calls, single-attempt transitions and bounded rollback. No cook/fire/projectile/effect call is made. No grenade source asset or original Steam file is modified. Next work is a guarded read-only scene reader, followed by isolated remote presentation. Projectile creation, detonation and damage must stay authoritative and must not be replayed from cosmetic animation events.


<!-- GRENADE-ACTIONS-V54 -->
## Native cooking, release and source references v54

Updated 2026-10-11T23:08:17.108605+02:00. Gameplay remains the preserved v52 equip-fixture DLL. The v53/v54 reader builds are separate read-only probe candidates; their exact hashes are recorded per take and are not the deployed lab-probe pin.

Corvo's high/low and Emily's high native source takes each pass six scoped checks. Normal controller LT is held for 1,300 ms, then released. The game creates and detonates the real grenade. Both isolated checkpoints are reloaded afterward and player/avatar health, inventory and fourteen protected files are checked. Source self-damage is recorded before reload; no world consistency, remote projectile authority or complete friendly-fire coverage follows from these observations.

| Native scene | Duration | Exact binding roles |
|---|---:|---|
| cooking_in | 500 ms | left arm, player, grenade |
| cooking_idle | 1,333 ms | left arm, grenade |
| throw_high | 400 ms | left arm, player, grenade |
| throw_high_ooa | 566 ms | left arm, player |
| throw_low | 400 ms | left arm, player, grenade |
| throw_low_ooa | 400 ms | left arm, player |

All use `Scene_1` and native `ThrowWeapon62`, RTTI `arkPlayerStateThrowWeapon`, vtable RVA `0x1c7f118`, alongside Running0 in these tests. The grenade quantity falls from one to zero at native release while the same equipment identity remains available. Under downward aim, component byte `+0x17c` becomes true during the low throw; it remains false during cooking and the high throw. This confirms the low-throw interpretation within these exact native cases, not a continuously updated aim flag. The authored high-throw scene has a native WEAPON_FIRE event at 293 ms: cosmetic playback must not invoke it.

The guarded reader checks exact scene names/durations, original player/arm/item ownership, stable native lists and supported active states. A later generic probe could miss the 233 ms unequip after the exact reader had seen it. The bounded read-only monitor resolves that sampling issue; failed takes are retained. Successful equip-reader tests pass nine checks for Corvo and Emily. Action-reader validation is recorded separately, per take.

[Thirty-five native first-person references](../../work/natural-moveset/fp-grenade-reference-manifest-v53.json) were sampled offline from the exact base left-arm animation set with the native Edge evaluator. They supply timing, finger articulation and equipment placement relative to a free wrist. They do not anchor a full body to the first-person camera. High/low source action sheets were viewed for Corvo and the high source sheet for Emily. Remote animation/prop attachment, locomotion, crouched actions, reload with additional grenades, cancellation, slide and VFX synchronization are still open.
<!-- /GRENADE-ACTIONS-V54 -->
