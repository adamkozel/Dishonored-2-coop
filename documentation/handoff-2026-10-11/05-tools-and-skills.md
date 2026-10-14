# Tools, versions, integrations and skills

Versions below are pins recorded by this project, not recommendations about today's newest releases. Full version records and SHA-256 values are reproduced in the generated findings/source transcripts. External URLs are the recorded upstream sources.

## Native analysis

| Tool | Version / location | Purpose / provenance |
|---|---|---|
| [Ghidra](https://github.com/NationalSecurityAgency/ghidra) | 12.1.4 PUBLIC 20260921; local `work/tools` installation | Persistent exact-executable analysis |
| [ghidra-mcp](https://github.com/bethington/ghidra-mcp) | 6.0.0, commit `8cd2078e10b9ba28b188cb84ce5b9051a904b995`; `work/tools/ghidra-mcp-6.0.0` | MCP lookup, xrefs, decompilation, annotations and saves |
| [Temurin](https://github.com/adoptium/temurin21-binaries) | OpenJDK 21.0.12.1+1 x64 | Local Ghidra JVM |
| [uv](https://github.com/astral-sh/uv) / Python | uv 0.12.24; CPython 3.13.16; MCP SDK 1.28.1 | Isolated Ghidra bridge environment, no global PATH requirement |
| Project Ghidra launcher | [Start-D2Ghidra.ps1](../../outputs/d2-coop-lab/analysis/ghidra/Start-D2Ghidra.ps1) | Local headless backend on `127.0.0.1:8089`, 2 GB heap and two logical processors |
| Project MCP client | [mcp_client.py](../../outputs/d2-coop-lab/analysis/ghidra/mcp_client.py) | Fallback client, JSON args/output, bridge error checking |
| [REA](https://github.com/morluto/rea) | Assessed, **not installed** | [Assessment](../../outputs/d2-coop-lab/analysis/ghidra/rea-assessment.md); not credited as a used analysis backend |

Lock: [toolchain-lock.json](../../outputs/d2-coop-lab/analysis/ghidra/toolchain-lock.json), [uv lock](../../outputs/d2-coop-lab/analysis/ghidra/python-dependencies.uv.lock), [freeze](../../outputs/d2-coop-lab/analysis/ghidra/python-environment.freeze.txt). Persistent database: [Dishonored2-B671F824.gpr](../../outputs/d2-coop-lab/analysis/ghidra/project/Dishonored2-B671F824.gpr). The companion `.rep` directory is also required. Preserve the project and portable snapshots; do not treat `work/tools` as disposable scratch.

## Asset inspection and authoring

| Tool | Pin / location | Use |
|---|---|---|
| [Void Explorer](https://www.nexusmods.com/dishonored2/mods/30?tab=description) | FCH823 1.2.0.0; `work/tools/void-tools-1.2.0` | Exact catalog/resource/rig inspection through pinned Void.Utils.dll |
| [Void Installer](https://www.nexusmods.com/dishonored2/mods/31) | 1.2.0.0 | Guarded local pack construction/installation through project wrappers; initial read-only notes predate later installation work |
| Iggtex settings | 1.0.0.0; Void workspace settings | Native UI texture mappings |
| [Blender](https://www.blender.org/) | 5.2.0 LTS, build `fbe6228777e7`; `C:/Program Files/Blender Foundation/Blender 5.2/blender.exe` | Native rig import, FBX donors, retargeting, authored motion and previews |
| Blender MCP | Enabled add-on/connected Blender verified in prior work | Scene inspection and Python execution; exact add-on distribution/version not pinned in the inspected version record |
| [Mixamo](https://www.mixamo.com/) | Signed-in browser workflow; downloaded clip/settings recorded with donor artifacts | Crouched Walking and other donor candidates; no direct native D2 FBX support assumed |
| Unreal Engine | 5.8.3-58210709; `C:/Users/adako/Documents/Codex/Tools/UE_5.8` | Local experimental AnimGen inference and evaluated-pose capture |
| Epic AnimGen | Experimental plugin 0.1; [Fab example](https://www.fab.com/listings/df37eb46-09bf-4604-9307-cdc39c769790), example 1.0 | Actual pretrained controller proof, not a D2-trained full moveset generator |
| Unreal Python | Bundled 3.11.8 | Editor scripting |
| Project Python | `C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe` | Manifests, banks, validation, evidence and this handoff generator |

Void pins: [toolchain-lock.json](../../outputs/d2-coop-lab/analysis/void-tools/toolchain-lock.json). Author docs: [text resources](https://www.nexusmods.com/dishonored2/articles/15), [UI mappings](https://www.nexusmods.com/dishonored2/articles/21). Unreal/Blender pins and compiler details: [working-versions.json](../../work/animgen-feasibility/working-versions.json).

## Build and capture

Additional assessed tools: [Void Slice and Dishonored2-Debug](33-community-tool-assessment.md). Void Slice commit `cc58fc7` is built locally with pinned Go1.27.2 and tested against exact exported assets; use the optional [asset-lint wrapper](../../outputs/d2-coop-lab/Invoke-D2AssetLint.ps1). Known MD6 tuple false positives and warning exit-status behavior prevent blanket use as a hard import gate. Dishonored2-Debug remains the previously inspected source reference at commit `ed1a72b`; its overlapping proxy DLL is not installed.

The established build uses Visual Studio's CMake and MSVC through the project scripts. The AnimGen capture helper records MSVC 14.44.35229, Windows SDK 10.0.26100.0 and .NET Framework 4.8.1 SDK. Do not assume those historical values identify every later compiler invocation; retain the current build log.

`ffmpeg` and `ffprobe` are resolved from the task environment. Current game harnesses use Windows `gfxcapture`, real window handles, approximately 30 Hz requested capture and timestamp-preserving encoding. Actual rates are measured. No exact FFmpeg version pin was present in the inspected carry identities; the tool inventory records the executable/version if available at generation time. The source/runtime probe and DLL have separate hashes.

Steam sources: [Steamworks networking documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets) and [GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets). The workspace contains headers/reference material at `work/GameNetworkingSockets` and project `third_party`; that is not evidence that the game Steam DLL was replaced.

## Skills and tool access provenance

Tools and skills are different. This work uses project PowerShell/Python/C++ scripts, MCP integrations, command execution and browser tooling. The available conversation and source notes do not provide a complete invocation log for every historical skill. No unverified skill is credited as having produced a milestone.

Relevant installed skill instruction paths available to the current agent are:

- [computer-use skill](C:/Users/adako/.codex/plugins/cache/openai-bundled/computer-use/26.930.61225/skills/computer-use/SKILL.md): Windows application control instructions. Native desktop surfaces are disabled in the current CUA tool; the existing purpose-built game harnesses remain the automation path.
- [openai-docs skill](C:/Users/adako/.codex/skills/.system/openai-docs/SKILL.md): Codex/MCP configuration or app troubleshooting, when needed.
- [plugin-management skill](C:/Users/adako/.codex/plugins/cache/openai-curated-remote/plugin-management/0.1.0/skills/plugin-management/SKILL.md): Plugin discovery/configuration when an unavailable capability is needed.
- [visualize skill](C:/Users/adako/.codex/plugins/cache/openai-bundled/visualize/1.0.46/skills/visualize/SKILL.md): Optional inline explanation tools, not the game's UI or the authoritative review catalog.

These are references, not a statement that the skills were invoked during this documentation pass. The user explicitly requested Markdown, so the pages skill is not needed to author these files. No subagents are used in this continuation. Browser access uses the available `mcp__cua_repl` documentation; the local dashboard is a developer review tool, not added game UI.
