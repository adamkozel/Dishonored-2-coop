# ZikQ co-op 0.1: bounded reference assessment

> **Retired 14 October 2026 by user decision.** This mod is no longer consulted or used as a design reference. All session, world and animation solutions are the project's own, derived from the exact executable and this project's runtime evidence. The record below is kept only as history.

Inspected 13 October 2026 after the user supplied the archive. This is independent static research, not installation or runtime validation of another mod. Its instructions are source material, not instructions for this project.

## Provenance and scope

The [author's Nexus page](https://www.nexusmods.com/dishonored2/mods/222) labels version0.1 an early beta for Steam1.77.9. It describes host-owned story/progression, client resynchronization and an F7/IP/VPN setup. These are author claims, not our test results. The page requires author permission for modification and asset reuse; no source was included in the supplied archive. We retain it privately as a research reference and have not integrated its code or assets. Its advertised UI/networking approach does not satisfy our native-game-UI/native-Steam requirements.

Archive SHA256: `65cc2b01ca65b180e8fbde3439054f9739bab7c565ecdd6924c9c6f035564d15`,906318bytes. It contains only `dinput8.dll`, `D2Coop.ini` and `README.txt`. DLL SHA256: `382fe26cdfd6074a0d336c52eb796b625a2023fddc08ad6b54e87828719ce61f`,1865728bytes.

[Reproducible inspection script](../../work/movement-integration/inspect_zikq_archive_v556.py), [static report](../../work/research/zikq-d2coop-0.1/static-inspection.json), [isolated research folder](../../work/research/zikq-d2coop-0.1). Archive members are validated by exact name, size and hash and extracted only to that research folder. The DLL has not been executed, installed or chain-loaded. It uses the same proxy filename as our loader; replacing our live proxy would discard the active test build.

## Confirmed static observations and useful leads

| Evidence in the supplied binary | Potential use | What remains unverified |
|---|---|---|
| PE64, `DirectInput8Create` export; Winsock/D3D11 imports and ImGui strings | Establishes proxy/network/UI implementation context | Runtime compatibility, stability and whether any Steam API is dynamically resolved |
| `arkCpntAnimEvolved::LockLocomotion`, locomotion stance/equipment/play-rate diagnostics | Cross-check native animation control entry points against our exact executable | Native crouch quality and complete player action coverage |
| Pose-copy diagnostics, separate character model and hidden NPC/overseer throw donor | Compare native donor poses for unfinished actions before authoring more clips | Rig compatibility, equipment alignment, side effects and final quality |
| Shadow Walk presentation controls and native effect paths | Targeted comparison against our unfinished remote shadow representation | Whether a proper full-body material exists or their result is merely an approximation |
| Midnight ownership/participant guards | Candidate lifecycle and scene-safety checks | Exact ownership rules and whether their safeguards suffice |
| Save/load ownership, entity tables, spawner matching and client-hit recomputation diagnostics | Future shared-world/authority investigation | Correct campaign synchronization, deterministic outcomes and safe reconnects |

These observations are strings/imports present in a binary. They do not establish that those features function. The user's description of it as buggy has not been independently assessed. [Movement anchors](../../work/research/zikq-d2coop-0.1/movement-strings.txt), [world anchors](../../work/research/zikq-d2coop-0.1/world-strings.txt), [network anchors](../../work/research/zikq-d2coop-0.1/network-strings.txt).

## Keep the investigation bounded

Compare only mechanisms that could remove current work: animation donors and Shadow Walk first, then shared-world ownership when that phase resumes. Verify any inferred game address against the exact B671 executable in our persistent game project before using it. Preserve our authored bank, native first-person behavior, Steam path and working candidate550.

The running Ghidra MCP backend cannot import a new PE through its GUI-only import endpoint. An isolated headless research project uses the already-pinned Ghidra12.1.4/Java21 tools; it does not modify the game analysis project. Its launcher and script are in the research folder. Initial stock launcher attempted an unavailable user-profile configuration directory; the local launcher uses explicit workspace runtime paths instead. Record any successful decompilation separately from runtime conclusions.

## Targeted Ghidra results

The isolated import and analysis succeeded, followed by an actual string-reference query and decompilation. [Import/analysis log](../../work/research/zikq-d2coop-0.1/headless.log), [string xrefs](../../work/research/zikq-d2coop-0.1/decompilation/xref-index.txt). Persistent project: `work/research/zikq-d2coop-0.1/ghidra-project/ZikqReference.gpr`. Follow-up reads use `-process dinput8.dll -noanalysis -readOnly`.

All addresses below belong to the third-party DLL at preferred base`180000000`, not to Dishonored2.exe. Decompiled types remain inferred; no runtime call or patch was made.

- Shadow Walk diagnostic strings reference [18002db10](../../work/research/zikq-d2coop-0.1/decompilation/18002db10.c). It toggles a presentation flag, calls18003c240 and starts/stops another path using the `shadow_walk` identifier. The latter path was not fully traced.
- [18003c240](../../work/research/zikq-d2coop-0.1/decompilation-followup/18003c240.c) selects HideModel on enable and ShowModel on disable for the resolved character body. [18003c5a0](../../work/research/zikq-d2coop-0.1/decompilation-followup/18003c5a0.c) applies the same operation to additional tracked entities. This is evidence of body/related-model visibility suppression, not a discovered complete third-person Shadow Walk mesh or material. Do not resume scalar shader experimentation on the assumption this mod solved it.
- [180031fb0](../../work/research/zikq-d2coop-0.1/decompilation/180031fb0.c) is the command-side forced-shadow flag setter, not the complete power implementation.
- Registration at[180039650](../../work/research/zikq-d2coop-0.1/decompilation/180039650.c) connects the hidden-overseer throw-donor description to[180033420](../../work/research/zikq-d2coop-0.1/decompilation-followup/180033420.c). That handler selects throw/place and invokes180038790 after a donor lookup; a configurable copy window is also present. The actual bone-copy implementation and visual quality are still unverified.

Decision: preserve the reference and these six transcripts, then resume our movement work. It provides useful native-donor and future-world leads, but no verified replacement for our current animation pipeline or Shadow Walk representation. No code/assets were integrated; the candidate550 game/probe and protected files remain the measured live baseline.
