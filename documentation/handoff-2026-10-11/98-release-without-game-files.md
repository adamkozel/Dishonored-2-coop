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

## Release 0.7.0 (builder 1313)

[`build_release_v1313.py`](../../work/release/build_release_v1313.py) builds `d2coop-v0.7.0.zip` (37 MB). It contains:
- `dinput8.dll` (candidate 1214cf);
- `bin\d2coop_setup.exe`, pinned in [setup-tool-v1313](../../work/release/setup-tool-v1313/identity.json) with its source commit;
- the decl recipes, both bank patches and the tables recipe;
- `Install.cmd`, `Uninstall.cmd` and the README ([release notes](../../work/release/RELEASE-NOTES.md)).

The builder checks two things:
- It builds the packages from the original install (read only) and pins them to the lab's own files.
- No package file equals a game file.

**Installer** ([`Install-D2Coop.ps1`](../../work/release/Install-D2Coop.ps1), Windows PowerShell 5.1 or 7):
- It finds Dishonored 2 through Steam's library folders (app 403640) when no folder is given.
- It checks the executable, `steam_api64.dll`, the original `master.index` and the package hashes.
- It builds every game-derived file into a staging folder and checks them.
- It then backs up what it replaces and writes the receipt *before* copying. Any failure while copying rolls back by itself.
- It refuses while that folder's game runs. It hashes through .NET, and the `.cmd` wrappers clear `PSModulePath`: Windows PowerShell started from PowerShell 7 otherwise loads 7's modules and has no `Get-FileHash`.

**Install test ([`Test-ReleaseInstall-v1313.ps1`](../../work/release/Test-ReleaseInstall-v1313.ps1), scratch copies only):**

| Take | Result |
|---|---|
| 1313a | Failed: no `Get-FileHash` in Windows PowerShell started from PowerShell 7. |
| 1313b | Failed: the receipt write ("Argument types do not match", generic lists in `ConvertTo-Json` under 5.1) came after the copy and left a partial install without a receipt. This led to the receipt-first, self-rolling-back install. |
| [1313c](../../work/release/release-install-v1313c/result.json) | All 8 cases pass. The install builds everything from a full copy of the original game in 51 s (builder peak 1.24 GB). Packages are at their pinned hashes, the rebuilt banks pass `d2_movement_bank_tests`, and the 17 tables parse. Reinstall is refused, the rollback is byte-identical, a wrong exe and a modified `master.index` are refused with nothing changed, the Steam install is untouched and the protected saves are unchanged. |

**Clean-PC path in the VM ([`Test-ProductTwoAccount-v1314.ps1`](../../work/release/Test-ProductTwoAccount-v1314.ps1)).** Second Steam account, Defender on, no exclusion over the Steam folder. The zip is extracted into Downloads and `Install.cmd` runs with no arguments.

| Take | Result |
|---|---|
| 1314a (1214ce) | The install found the game and built everything (77 s), with no Defender detection. Product boot, F7 hosting and the Steam join connected both ways, and the uninstall rolled back. **But the VM stayed in its main menu:** join-follow only covered gameplay and pause menus (logic 1–3), not the front end (`campaign/menu/menu_p`, logic 6). Candidate 1214cf adds it. |
| 1314b (1214cf) | The VM loaded the host's mission from the main menu (`loads 1`), then waited at the native "Press any key" mission card (logic 15). The harness now presses a key, as a player does. |
| 1314c | `same_mission`, then the native character choice of *A Long Day in Dunwall* (logic 14). The harness now confirms Emily (Right, Enter), and the README tells the partner to choose Emily. |
| 1314d | The coup's "Play as Emily Kaldwin?" confirmation is a menu (logic 3). The harness now answers it as well (Right, Enter). |
| 1314e, 1314f | The host product instance on this PC never got past its title screen: real keystrokes to its window stopped reaching the game once the PC was in use (a game-streaming session was running). |

**Take series 1315 ([`Test-ProductTwoAccount-v1315.ps1`](../../work/release/Test-ProductTwoAccount-v1315.ps1)).** The VM side is unchanged: the release, `Install.cmd` with no arguments, and only the VM's keyboard. The host is the lab host instance, driven only through its command file and simulated controller, so nothing types on this PC's desktop. It runs `coop_session host_quiet` (no invite overlay), `coop_follow_host on` and `coop_product_auto on`. F7 hosting itself is covered by take 1225.

| Take | Result |
|---|---|
| [1315a](../../work/release/product-two-account-v1315a/result.json) | **Everything up to leaving passed (confirmed runtime):** <ul><li>The VM's install built every file from the VM's game in 65 s, with Defender on and no detection.</li><li>The VM's game followed the host from its main menu into *A Long Day in Dunwall* and took Emily.</li><li>Both partner banks were active. The VM's rebuilt Corvo bank has the same checksum as this PC's rebuild.</li><li>Each side saw the other walk (6.8 m and 7.8 m).</li><li>The uninstall rolled back, and the lab loopback reconnected.</li></ul> **Failed only on leaving:** the harness tapped F8 instantly, and the product hotkeys poll the key state. It now holds the key. |
| [1315b](../../work/release/product-two-account-v1315b/result.json) | **Pass (confirmed runtime), the clean-PC proof:** <ul><li>The VM downloaded the published [v0.7.0 release](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.7.0) itself (7 s; SHA-256 matches the zip).</li><li>It extracted it into Downloads, and `Install.cmd` alone found the game and built every file in 59 s. Defender was on, with no exclusion over the Steam folder and no detection.</li><li>The VM joined the host's lobby through Steam, followed it from the main menu into *A Long Day in Dunwall* as Emily, and both partner banks were active.</li><li>Each side saw the other walk (3.8 m and 7.8 m), and F8 left the session (`left_by_player`).</li><li>`Uninstall.cmd` restored the originals, and the lab loopback reconnected.</li></ul> |

## Release 0.7.0 on GitHub

Published as [v0.7.0](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.7.0) from `main` (squash merge 652e8e81 of #97). The asset is `d2coop-v0.7.0.zip`, SHA-256 `484FF854DA210A611AC62DCCFCCDB1C8508B3F570A9CCF2B98CA2CBF10946EFF`. The user made the repository public on 2026-10-17. Still open with the user:
- the history decisions in #98: game-derived headers before #30, Mixamo, the personal email;
- the Microsoft false-positive submission (#25);
- the human playtest (#99).

## Release 0.8.0 (builder 1668, candidate 1214fx)

What changed since 0.7.0: shared NPC combat authority (stage 4), partner-led story steps and the mid-mission join (#134, #135), the real autosave pair (#136), breakables (#132), power outcomes (#117), and the Ramsey campaign blocker fix (1214fw, 1214fx; chapter 106).

- **Data:**
  - decl recipes v2: 48 decls, adding the Timepiece prop (#122) and the two remote Windblast effects;
  - bank patches v2 for bank 75/382 (payload classification v1667);
  - native tables v1, unchanged.
- **Setup tool:** setup-tool-v1669. `d2coop_setup.exe` 0.8.0 from the 1214fr build; only its version resource differs from 0.7.0.
- **Build** ([log](../../work/release/build-release-v1668.log)):
  - `d2coop-v0.8.0.zip`, 37,205,349 bytes, SHA-256 `F485423567B0C51CC2C60DE73A2D11CEE8C3BCA36D8F2A9023C420B350A71DE0`;
  - the live DLL is 1214fx's, the generated packages match the lab's, and no package file equals a game file.

| Take | Result |
|---|---|
| [release-install-v1669a](../../work/release/release-install-v1669a/result.json) | **Pass, 8/8,** on scratch copies of the original install: <ul><li>install in 51 s, with a 1.3 GB setup peak;</li><li>the built banks load and the tables parse;</li><li>a reinstall is refused, and rollback is byte-identical;</li><li>a wrong executable and a modified `master.index` are refused unchanged;</li><li>the Steam install is untouched.</li></ul> |
| [product-two-account-v1670a](../../work/release/product-two-account-v1670a/result.json) | **Pass (confirmed runtime), the clean-PC proof for 0.8.0:** <ul><li>The VM downloaded the published [v0.8.0 release](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.0) itself (2.5 s; SHA-256 matches the zip).</li><li>It extracted it into Downloads, and `Install.cmd` alone found the game and built every file (14 files) in 90 s, with Defender on.</li><li>The lab host on this PC was in *A Long Day in Dunwall* and hosted a friends-only lobby.</li><li>The VM joined through Steam on the second account and followed the host's mission (`loads` 1, the same map, both partner banks active).</li><li>Each side saw the other walk (6.7 m and 7.8 m), and F8 left the session (`left_by_player`).</li><li>`Uninstall.cmd` removed 13 added files and restored the original `master.index`, and the lab loopback reconnected.</li></ul> |

Published as [v0.8.0](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.0) (Latest) from `main` (squash merge 4db178fc1 of #141), with the release notes and the zip's SHA-256.

## Release 0.8.1 (builder 1695, candidate 1214fz)

What changed since 0.8.0: the partner's mission end and level transition no longer expire on arrival (1214fy, `tickAge`; chapter 106). A packet stamped after the receiver's frame tick had a wrapped age and was dropped at once, so the partner could miss a mission end. 1214fz is 1214fy with the version resources set to 0.8.1.

- **Data:** unchanged from 0.8.0 (decl recipes v2, bank patches v2, native tables v1).
- **Setup tool:** setup-tool-v1695. `d2coop_setup.exe` 0.8.1 from the 1214fz build; only its version resource differs from 0.8.0.
- **Build** ([log](../../work/release/build-release-v1695.log)): `d2coop-v0.8.1.zip`, 37,205,535 bytes, 13 files, SHA-256 `E219DF72D311CC8F7AED24DF307E74FD465667456B2DE97A94A0436B64C397D9`.

| Take | Result |
|---|---|
| [release-install-v1695a](../../work/release/release-install-v1695a/result.json) | **Pass, 8/8,** on scratch copies of the original install: <ul><li>install in 56 s, with a 1.3 GB setup peak;</li><li>the built banks load and the tables parse;</li><li>a reinstall is refused, and rollback is byte-identical;</li><li>a wrong executable and a modified `master.index` are refused unchanged;</li><li>the Steam install is untouched.</li></ul> |
| [product-two-account-v1696a](../../work/release/product-two-account-v1696a/result.json) | **Failed only at the story step; everything before it passed (confirmed runtime):** <ul><li>The VM downloaded the published [v0.8.1 release](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.1) itself (2.3 s; SHA-256 matches the zip).</li><li>`Install.cmd` alone built every file (14 files) in 73 s, with Defender on and no detection.</li><li>The VM joined the host's friends-only lobby through Steam, followed it into *A Long Day in Dunwall* (`loads` 1), and both partner banks were active.</li><li>Each side saw the other walk (6.8 m and 7.8 m).</li><li>`Uninstall.cmd` removed 13 added files and restored the original `master.index`, and the lab loopback reconnected.</li></ul> **The story step:** the lab host's fixture was already past all five route triggers (gameflow 239,2,5,2, *Find Ramsey*), so no trigger changed anything in either world. Both worlds stayed equal, so this is a harness gap, not a mirror failure. Test-ProductTwoAccount-v1699 has the host kill Ramsey when no trigger advances the story. |
| [product-two-account-v1699a](../../work/release/product-two-account-v1699a/result.json) | **Failed at the story step again; everything before it passed (confirmed runtime):** download (2.0 s, SHA-256 matches), install (80 s, Defender on, no detection), the Steam join, the same mission, both banks active, each side saw the other walk, and the rollback. <ul><li>The Ramsey fallback attacked 60 times without a story change.</li><li>The VM game showed "No connected user. You won't be able to save." (`vm-story.png`, and the same in 1696a). It had followed the host's mission from its title screen, where nobody had pressed a key, so it had no signed-in user ([#146](https://github.com/adamkozel/Dishonored-2-coop/issues/146)).</li></ul> Test-ProductTwoAccount-v1705 presses a key at the title screen as a player does. Its Ramsey fallback picks the Ramsey nearest his room (the route's rule), traces his health and records the VM's state. The release notes now tell the partner to press a key at the title screen. |

Published as [v0.8.1](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.1) (Latest) from `main` (squash merge a5abe3106 of #142). The campaign re-validation on the 0.8.1 binary is in [chapter 97](97-campaign-map-sequence.md).

## Release 0.8.2 (builder 1712, candidate 1214gf)

What changed since 0.8.1:
- **#148:** deaths are shared on the first map of the campaign. The guard map is played as a mission in product and online sessions (1214gb).
- **#140:** the partner's key-target finishers are paired scenes, so her kill of a story target happens in both worlds (census 1707, 1214gc).
- **#135:** a rejoining partner is placed beside the host (1214ga).
- **#145:** the product controller counts worlds (1214ga).
- **Diagnostics (read-only):** the partner scene probe and the corpse trace (1214ga, 1214ge).

1214gf is 1214ge with the version resources set to 0.8.2.

- **Data:** unchanged from 0.8.0 (decl recipes v2, bank patches v2, native tables v1).
- **Setup tool:** setup-tool-v1712. `d2coop_setup.exe` 0.8.2 from the 1214gf build; only its version resource differs.
- **Build** ([log](../../work/release/build-release-v1712.log)): `d2coop-v0.8.2.zip`, 37,210,194 bytes, 13 files, SHA-256 `FACF00B04F39ACB5439B25114A71556FDDC2662743EFACE743AB4D31DE644EE5`.

| Take | Result |
|---|---|
| [release-install-v1712a](../../work/release/release-install-v1712a/result.json) | **Pass, 8/8,** on scratch copies of the original install: <ul><li>install in 51 s, with a 1.3 GB setup peak;</li><li>the built banks load and the tables parse;</li><li>a reinstall is refused, and rollback is byte-identical;</li><li>a wrong executable and a modified `master.index` are refused unchanged;</li><li>the Steam install is untouched.</li></ul> |

The whole campaign passes on two Steam accounts on this binary: [chapter 97](97-campaign-map-sequence.md), campaign-*-vm-v1713.

| Take | Result |
|---|---|
| [product-two-account-v1709a](../../work/release/product-two-account-v1709a/result.json) | **Pass (confirmed runtime), the clean-PC proof for 0.8.2:** <ul><li>The VM downloaded the published [v0.8.2 release](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.2) itself (2.0 s; SHA-256 matches the zip).</li><li>`Install.cmd` alone built every file in 78 s, with Defender on and no detection.</li><li>The host started a fresh campaign as Corvo, as a real host does (Test-ProductTwoAccount-v1709).</li><li>The VM pressed a key at its title screen, joined the friends-only lobby through Steam on the second account, and followed the host into *A Long Day in Dunwall*. Both partner banks were active.</li><li>Each side saw the other walk (8.0 m and 8.1 m).</li><li>The bedroom trigger advanced both worlds equally (241,4,3,0 to 240,4,3,1).</li><li>On the first map, the host fell: both games showed GameOver and both restarted together (#148 in product mode).</li><li>F8 left the session (`left_by_player`), and `Uninstall.cmd` restored the originals.</li></ul> |

Published as [v0.8.2](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.2) (Latest) from `main` (squash merge 97b8d317a of #147).
