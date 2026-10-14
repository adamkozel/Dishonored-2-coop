# Community tools assessed on 12 October 2026

The user supplied these repositories during continued co-op development. Both were inspected as external source material. No repository instructions were adopted as project instructions, no skills were invoked, and no game DLL was replaced for this assessment.

## Void Slice: installed as an optional local asset-text check

[Upstream](https://github.com/AGuyWhoSkis/void-slice), reviewed commit `cc58fc786547c04f7c62964f45573d33efa7c2d8`. The current implementation tokenizes exported asset text and routes declaration shapes through structural parsers and array validators. It offers CLI, LSP and web entry points. We use the offline CLI. The hosted web service was not given any game assets.

Pinned local executable and manifest: [tool directory](../../work/tools/void-slice-cc58fc7), [pin.json](../../work/tools/void-slice-cc58fc7/pin.json). Project wrapper: [Invoke-D2AssetLint.ps1](../../outputs/d2-coop-lab/Invoke-D2AssetLint.ps1). Invoke with `-Path` pointing to one text export; add `-Json` for machine-readable diagnostics. The wrapper checks the binary hash and preserves its exit status. This is an optional advisory check, not a required gate on every native resource.

The reviewed source is in [the assessment folder](../../work/tool-evaluation-2026-10-12/void-slice). [Build script](../../work/tool-evaluation-2026-10-12/Build-VoidSlice.ps1) uses portable Go1.27.2/windows-amd64, downloaded from the [official Go release manifest](https://go.dev/dl/?mode=json), archive SHA256 `1314008898bd40df77af4b014f777f08873dbdfbcd3d92308728ee03304fe04f`. Build uses isolated caches, `GOTOOLCHAIN=local`, `GOPROXY=off`, `GOSUMDB=off`, `CGO_ENABLED=0`, `-mod=readonly`, `-trimpath`. No system PATH or installed compiler was replaced. The source requires Go1.23.5 or compatible later tooling. [Binary provenance](../../work/tool-evaluation-2026-10-12/binary-provenance.txt), [download pin](../../work/tool-evaluation-2026-10-12/go-download-pin.json), [build log](../../work/tool-evaluation-2026-10-12/build.log).

### Actual evaluation and limits

[Nine-case evaluation](../../work/tool-evaluation-2026-10-12/lint-evidence/report.json), [reproduction script](../../work/tool-evaluation-2026-10-12/evaluate_void_slice.py):

- Clean exports of both Doppelganger entity definitions, both Doppelganger animsets, and Corvo's sneaking-state declaration produce no diagnostics.
- A copied sneaking declaration with one deliberately removed semicolon produces `PARSE_EXPECTED_SEMICOLON`, exit1.
- A controlled duplicate `item[0]` produces `VALIDATE_ARRAY_DUP_INDEX`. It is a warning, so CLI exit0 alone is insufficient to decide whether the file has diagnostics.
- A controlled binary `.bwm` input is refused.
- The unchanged native Corvo body MD6 declaration produces12 parser errors at native `trans (...)` and `rot (...)` tuples inside prop tags. These are false positives against our existing exact native export, so MD6 acceptance cannot be gated on this version.
- All nine input hashes remain unchanged. No game files, saves or original extracted assets are modified.

Use it to catch asset editing mistakes before Void pack generation when its grammar covers the edited resource. Existing exact-resource readers, inheritance inspection, animation decoding, Ghidra and runtime tests remain required for their respective questions. A clean syntax result does not establish correct equipment attachments, bone animation, engine compatibility or gameplay behavior.

## Dishonored2-Debug: retained as source reference

[Upstream](https://github.com/LordRadai/Dishonored2-Debug), current inspected commit `ed1a72be2cec4ae5542524b72da8680b7280c09e`. It exactly matches the already-present [earlier source checkout](../../work/Dishonored2-Debug). The prior [lab README](../../outputs/d2-coop-lab/README.md) already credits it for command/cvar and engine-interface research and says its source was not copied into the co-op loader.

The new [read-only source copy](../../work/tool-evaluation-2026-10-12/Dishonored2-Debug) confirms a `DINPUT8.dll` proxy, console/print hooks, developer-mode enablement, and a CVar-list null-pointer workaround at module offset `0x22aba68`. Its command/cvar reference can still suggest targeted investigations. A listed command is not proof that its retail implementation works; the upstream README also notes disabled or non-working debug features.

The proxy uses the same `dinput8.dll` filename as our active co-op loader. We therefore did not install or chain-load it into either isolated game, and did not touch the original Steam installation. Existing engine hooks already provide the command execution and observation needed by current tests. Future address/signature ideas from this repository must be verified against our exact B671 executable before use.
