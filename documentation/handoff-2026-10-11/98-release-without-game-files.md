# Release without game files (#30, stage 7)

Updated 2026-10-17. The release ships no Dishonored 2 file and no data copied from one. The installer's builder, `d2coop_setup.exe`, makes every game-derived file from the player's own install. Payload classification: [work/release/payload-classification-v1312](../../work/release/payload-classification-v1312/README.md).

## What the installer builds

| Output | Built from | Check |
|---|---|---|
| `base/master.index`, `base/game1voidmod.*` to `game4voidmod.*` | the player's resource containers and `decl-recipes.bin` (D2RCP004: our text plus line copies from the player's decls, [decl_recipe.h](../../outputs/d2-coop-lab/src/decl_recipe.h)) | byte-identical; SHA-256 pinned in the manifest |
| `d2coop/corvo-choke-bank.bin`, `d2coop/emily-choke-bank.bin` | the player's animations and `<role>-bank.patch` (D2BNK001, [bank_rebuild.h](../../outputs/d2-coop-lab/src/bank_rebuild.h)) | per-clip sums with a tolerance |
| `d2coop/native-tables.bin` | the player's prop and first-person animations and `native-tables.recipe` (D2NTR001, [table_rebuild.h](../../outputs/d2-coop-lab/src/table_rebuild.h)) | per-table sums with a tolerance |

Native samples come from the player's own `Dishonored2.exe`: the builder maps it without running it (`DONT_RESOLVE_DLL_REFERENCES`, no entry point, imports or game state) and calls only its Edge animation evaluator, as the lab's `sample_edge.cpp` does. The executable must be the supported build (SHA-256 checked first).

## Movement banks

Each bank clip's base is chosen by a fit ([fit_bank_bases.py](../../tools/release/fit_bank_bases.py), numpy mirror [bank_math.py](../../tools/release/bank_math.py)). Per frame and body part (14 parts), the base is one of:
- the bind pose;
- one source at a fractional time;
- a blend of two sources.

The sources are the clip's native assets from the classification:
- copied by joint name, or retargeted the way `retarget_native.py` does it (additive clips composed with their reference first);
- either one optionally mirrored, as the builders mirror;
- plus the bank clips the clip was built on.

Helper joints are then recomputed. The patch stores these choices and a residual quantized at 2^-17, which carries our edits, Mixamo and authored motion and the runtime captures.

**Motion explained by the player's own animations (confirmed static, fit of v1212):**

| Clips (Corvo) | Count | Motion energy explained |
|---|---|---|
| purely native | 201 | 95.2% |
| native and Mixamo | 117 | 85.2% |
| all clips | 374 | 86.0% |

The 14 purely native clips explained below 80% are runtime captures (carry, contact traces), IK-adjusted clips and composites the generic model cannot express.

**Rebuild (confirmed runtime, original Steam install read only):** the Corvo patch is 19.1 MB. It rebuilds all 374 clips of bank v1266 within half a step (3.8e-6) of our bank, and the decoder's result equals the encoder's ([report](../../work/release/bank-patches-v1/build-corvo-a.json)).

## Native tables

Until #30 the DLL compiled in about 445 KB of game-derived tables. They now come from `native-tables.bin`:
- **crossbow and grenade mechanism clips:** exact Edge samples of the prop animations. Their skeletons' parents and bind poses come from the player's `.edgeskl`.
- **ranged hand tracks:** the prop attachment relative to the wrist in the first-person samples, rotated by the hand calibration between the first-person and body skeletons (`HandReference.invcal`, computed from both skeletons' palms). Fitted times explain every track within 0.01–0.04°, and equip within 1.9°.

The 15 KB recipe rebuilds all 17 tables within 4.8e-7 of the old values ([report](../../work/release/native-tables-v1/build-a.json)). The reference values stay local (`work/release/native-tables-v1/reference-tables.bin`).

**DLL (candidates 1214cc–1214ce):**
- The DLL reads the tables in product mode from `<data dir>\native-tables.bin`, and in the lab from `<status path>.tables.bin`.
- Without the file, every user of a table refuses rather than guessing a pose.
- `coop_tables_probe` lists every table with the shape it is used at. Both lab roles load 17/17 (confirmed runtime, [host](../../work/movement-integration/candidate-npc-combat-v1214ce/native-tables-host.json)).
- 1214ce adds Windows version resources to the DLL and the builder (#25).

## Cross-CPU behaviour

The Edge evaluator (`FUN_1412e2c30` and 13 callees) uses 24 `RCPPS` and 10 `RSQRTPS` instructions (confirmed static), whose results differ between CPU vendors. A rebuilt bank or table can therefore differ from the lab's in its last bits on another machine (hypothesis for the magnitude: an ULP or two). So the installer checks the sums of every clip and table against the release build, with a tolerance of 1e-6 per value, instead of their hashes. Every input is still pinned by SHA-256: the executable, each skeleton and each animation.

## What is still derived data in the release

- The bank patches' residuals: about 5% of the native motion energy of the purely native clips, and the motion the model cannot place in the rest. This is a difference signal, not usable animation on its own. Publishing it is the user's decision (#30 Needs you).
- Two tiny curves compiled into the DLL: the vault pelvis lift (240 floats) and the equipment contact phases (8 floats).
- The takedown scene table holds names and durations only.
