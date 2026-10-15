# Build, test, capture and recovery runbooks

All commands below assume PowerShell at `C:\Users\adako\Documents\Codex\2026-10-08\i-x20`. Read the current script before execution. Native process access may require the task's approved elevated execution path; the user has authorized isolated autonomous development, not destructive changes to unrelated processes or original Steam data.

## Resume safely

1. Read current-state, latest continuation and exact candidate identity. Do not infer a live DLL from a historical `LoaderSha256` field.
2. Inspect each `work/lab-instances/<role>/launch.json`. Check PID, executable path and process start time; PID alone can be reused.
3. Confirm the executable is the physical `work/asset-game/Dishonored2.exe`. Never install assets into `work/lab-game/base` or the original Steam folder.
4. Check the current protected-file manifest [protected-save-hashes-before-v4.json](../../work/movement-integration/protected-save-hashes-before-v4.json). It records 14 original save/config hashes; do not overwrite them to make a test pass.
5. Allow any active recording/fixture/deployment to finish before starting another. Finalize recordings before validation, frame extraction or dashboard indexing.
6. Keep the two game windows windowed and side by side. Automated input is role-specific; concurrent physical input invalidates controlled comparisons.
7. Check `git status` and the current branch. Record any uncommitted result before starting the next card. Then take the next card from the [roadmap board](https://github.com/users/adamkozel/projects/3/views/2) (see [CLAUDE.md](../../CLAUDE.md)).

## Build

The maintained wrapper is [Build-D2Lab.ps1](../../outputs/d2-coop-lab/Build-D2Lab.ps1). Current CMake build tree: `work/build`; release proxy: `work/build/Release/dinput8.dll`. Use target `d2_lab_proxy`, not `dinput8`.

The established clean-environment invocation is:

```powershell
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vsRoot = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$cmake = Join-Path $vsRoot 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
& node outputs/d2-coop-lab/tooling/run-clean.cjs $cmake --build work/build --config Release --target d2_lab_proxy
if ($LASTEXITCODE) { throw 'Build failed' }
& work/build/Release/d2_proxy_smoke.exe work/build/Release/dinput8.dll
if ($LASTEXITCODE) { throw 'Proxy smoke failed' }
```

Relevant additional targets include `d2_movement_bank_tests` and `d2_shadow_action_tests`. Carry semantic tests belong to the latter, not the proxy smoke executable. Bank format 1–35 compatibility and both real candidate banks have recorded passing checks. Run the tests appropriate to changed behavior; do not replace failed evidence with a successful unrelated test.

## Archive and deploy

Before deploying, preserve the source, DLL, relevant banks/assets, manifests and exact hashes in a new uniquely named candidate directory. Keep the previous compatible binary/source/bank set. Since 14 October 2026 the workspace is also a Git repository (private GitHub repo `adamkozel/Dishonored-2-coop`). Git does not replace these archives: DLLs, banks and game-derived assets stay local, and candidate folders are append-only evidence. After each result, record it through the GitHub loop in [CLAUDE.md](../../CLAUDE.md): commit and push to the card's draft PR (about 15 s, never waiting on CI).

[Deploy-SourceClock.ps1](../../work/movement-integration/Deploy-SourceClock.ps1) checks the physical target and protected saves, stops only validated isolated processes, launches both roles with `-AssetExperiment`, restores the fixture and rechecks hashes. It is an active mutation script, not a read-only check. The launcher stages the built proxy; use the archived compatible set for deliberate rollback and verify it afterward. Do not copy a DLL over a running process or blindly rerun obsolete deploy/index scripts.

## Established Dunwall fixture

[PrepareShadowSword.ps1](../../work/PrepareShadowSword.ps1) restores the native sword-ready setup and takes roughly 90 seconds. It is a state reset and must not run during another capture. The fixture uses the native Mayhew progression; bypassing that can leave equipment or actions unavailable.

Current continuous carry test example:

```powershell
& work/PrepareShadowSword.ps1
& work/natural-moveset/Record-CarrySettledStance-v30.ps1 `
  -Role host -Take unique-corvo-carry-label -CrouchedStart -Paired `
  -BankManifest crouched-pickup-bank-manifest-v3.json
```

`host` tests Corvo as source; `client` tests Emily. `-CrouchedStart` controls pickup stance; `-CrouchedDrop` controls the later stance/drop, and the harness toggles only when they differ. Take names must be new; the harness refuses overwrite. Version v30 adds receive-state sampling before expensive native reads. The current capture lasts 15 seconds, observes through 12.5 seconds, samples settled contacts at about 8.05 seconds and requests native drop at 8.5 seconds.

After the harness process exits, run the five validators/analyses with the take **name**, not a full path:

```powershell
$py = 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
& $py work/natural-moveset/validate_native_carrier_pickup.py unique-corvo-carry-label
& $py work/natural-moveset/validate_pickup_drop.py unique-corvo-carry-label
& $py work/natural-moveset/validate_carry_orientation.py unique-corvo-carry-label
& $py work/natural-moveset/analyze_crouched_pickup_contact.py unique-corvo-carry-label
& $py work/natural-moveset/validate_carry_load_contact.py unique-corvo-carry-label
```

Inspect every exit code/result; PowerShell's final command exit status can hide an earlier failed command. Generate a temporal sheet with `make_contact_sheet.py`, open it using image inspection, and write an honest scoped `visual-review.json`. `index_pickup_drop_sequences.py` requires successful carrier evidence but can retain rejected physics/orientation/contact comparisons. An unindexed failed take still remains in its original folder and the documentation inventory.

## Capture and dashboard

Use the native window handle and the existing `gfxcapture` harness. Requested capture rate is not measured capture rate. Preserve `identity.json`, `observations.json`, result/validator files, `capture.log`, the actual `native.mp4` and visual review. Source, bank, executable, probe, harness and video hashes identify the evidence.

`build_review_catalog.py` rebuilds the local dashboard catalog. The primary data is in `work/natural-moveset`; the developer server is at `http://127.0.0.1:8769/review.html`. Do not mark the library complete because an index build succeeded. Avoid rerunning old per-version indexers that overwrite `LatestContinuation` with historical live builds.

## Ghidra and native investigation

Start the preserved backend with the project launcher; specify `program: Dishonored2.exe` for MCP calls. Verify the exact image and hash before interpreting addresses. Use targeted function lookup, xrefs, disassembly and decompilation, then independent native runtime observations. Save dated confidence-labelled bookmarks and the program after meaningful findings. Do not blindly rerun the original seed over newer annotations.

Standalone short-lived MCP clients should connect to an independently started backend: their Windows process cleanup can otherwise terminate a child Ghidra process. The fallback client detects errors embedded inside apparently successful MCP responses. A decompiled placeholder may still have a one-byte function body; inspect actual entry/unwind ranges before fixing the body.

**Static asset lookup (no lab time):** [`tools/diag/d2_resource_index.py`](../../tools/diag/d2_resource_index.py) reads the game's `.index`/`.resources` containers read-only.
- Point it at the game copy (`work/asset-game/base`), never at the Steam installation.
- `list` prints entries; `grep --type <T> --match <source regex> --text <line regex>` searches inside them. It uses the newest index of each family (`gameNvoidmod.index`).
- Mission entity files are type `File`, sources `maps/campaign/**/*.entities`; usable interaction names (`Usable_lever`, `Usable_tap` and so on) are in the `entityDef` decls.
- **Mission flow:** each mission's objective tree is the `arkTree` `models/gameflow/gameflow_<mission>.arktree`. It names the mission, its objectives and tasks, `m_nextMissionMap` and `m_showResultScreen`. The mission's scripts live in its `.entities` file as `script_kiscule_*` entities; grep them for `arkKisculeNode*` names, `m_eventName` and `m_mapName`.
- Extract into the scratchpad: the output is game-derived and stays local.

## Lab-only commands and dead cvars

Use these lab commands when a take needs them. All are refused outside the lab allowlist (`engine_bridge.cpp`, `Send-D2Command.ps1`) and never ship.

| Need | Use | Not this |
|---|---|---|
| Keep a player alive through a fight | `coop_player_health_hold on\|off` (1214ap) | `ark_god_player`, `god`, `ark_buddha_player`, `g_damageScale`, `g_playerIncomingDamageScale`, `pm_disableFatalFall`: stripped, nothing reads them (confirmed static, [chapter 96](96-external-research-references.md)) |
| Give a native item in product mode | `coop_grant_item grenade\|health\|mana` (1214au; grenades are equipped too) | Editing the inventory by hand |
| Drink a potion through the native wheel routine | `coop_use_item health\|mana` (1214av). A mana potion is only drunk after a native power has spent the pool | Writing the mana pool (1214aw: the drink still refuses) |
| Film from any point, with the observer's arms hidden | Turn the observer (`setviewpos` at its own position with a yaw), set the pitch, then `coop_observer_camera hold_at x y z`; `off` in cleanup ([chapter 92](92-native-use-interactions.md)) | IGCS: its camera uses Ansel, which pauses the game |

Before relying on any other cvar from a public list, check in Ghidra that game code reads it: a registered cvar can be stripped. Full cvar lists for this build are saved locally ([provenance](../../work/external-references/README.md)): `work/external-references/fakuivan-cvars-1.77.9.0/dishonored2_cvars.txt` gives each cvar's struct and value address, and `work/Dishonored2-Debug/cvars.md` gives descriptions. Live leads: `gp_manaEnable` (read by the mana component) and `ark_inventory_mana_potion_autouse`. Unverified leads (autosplitter loading flag and level name, save format) are tracked in [chapter 96](96-external-research-references.md).

## Cleanup after an experiment

Send neutral input, disable only the experiment's owned leases/streams/fixtures, restore native equipment and physics through their cleanup paths, verify no dangling ownership and preserve evidence. Pause/load/peer-loss tests must retire references before any new world is used. Keep saves and native assets protected; do not recursively delete paths discovered through junctions.
