# External references (local only)

Saved 2026-10-15 at the user's request, as reference text. Nothing here is executed. These files stay out of git: the repo `.gitignore` allowlist excludes this folder, and the LordRadai, fakuivan and disrev sources carry no license. Chapter 96 records what was verified from each source.

| File | Source | Pin | SHA-256 |
|---|---|---|---|
| `../Dishonored2-Debug/cvars.md` (full clone) | [LordRadai/Dishonored2-Debug](https://github.com/LordRadai/Dishonored2-Debug), no license | commit `ed1a72be2cec4ae5542524b72da8680b7280c09e` (upstream head on 2026-10-15) | `01953094941c2458f95934c19066e5ddd2f3a63aee3696dce484740b082a78fe` (CRLF checkout) |
| `fakuivan-cvars-1.77.9.0/dishonored2_cvars.txt` | [fakuivan gist](https://gist.github.com/fakuivan/8910a208fc88f831f06b047d0955e4fe): every cvar for 1.77.9.0, with its struct address, default and int-value address | gist version `86278f0f765e7bf8e33bfd545980e6d37576360e` | `e5bfaad6312987d5f69b239db0f8db276b216270ef27398cfbe4a8369e94a256` |
| `drtchops-asl/dishonored2.asl` | [drtchops/asl](https://github.com/drtchops/asl/blob/master/dishonored2.asl), MIT | commit `a5b2db7bbcf65f4b92d8ca00f369ee2e34549553` | `0882648045968b5c67208245f6f95d3c9e766c8ef3dd8affa2ff2271c9603718` |
| `disrev/` (full clone) | [chipolux/disrev](https://github.com/chipolux/disrev), no license: resource and `.entities` notes, Python readers and the voidtweak C++ editor (Kiscule, BWM, decls) | commit `e9cf30eec5fa3d35eeef9c448f12f8c711a5914c` | git commit pin |
| `coop-anything/` (full clone, 2026-10-29) | [cranebeaker/coop-anything](https://github.com/cranebeaker/coop-anything), MIT: co-op modding playbook, sync test tools (flight, chaos, animcheck, mutants and more) and a read-only knowledge MCP server | commit `3dbf6dda2d6bf8183fc21e9764e309d7fe2ef22b` | git commit pin |

## How to use them

- **cvar lookups:** grep the fakuivan dump for a name and address (`Dishonored2.exe+<struct> | name = default | +<value>`), and LordRadai's `cvars.md` for descriptions. Then check the value address's readers in Ghidra: a registered cvar can be stripped (god mode is; see the runbook's "Lab-only commands and dead cvars").
- **Campaign map list:** `disrev/docs/levels.md` lists every mission's maps (a Dreadful Wale `boat_<mission>_p` map, then one or more mission maps, split by chaos). It plans stage 6's map and transition coverage; the `.entities` readers there overlap `tools/diag/d2_resource_index.py`. Never build or run disrev or voidtweak.
- **Autosplitter state for 1.77.9.0:** `isLoading` bool at +0x288D2E8 (alternative +0x3BDA0A8) and `levelName` string128 at +0x4025C50. These are leads for stage 6 load and mission detection, not yet read in the lab.
- **coop-anything (the one exception to "nothing here is executed", at the user's request on 2026-10-29):** its read-only knowledge MCP server `tools/mcp/coop_mcp.py` runs from the venv `work/tools/coop-anything-venv` (PyYAML 6.0.3; its 13 self-tests pass), registered with `claude mcp add --scope local coop-anything`. Its `coop-*` skills are linked into the user skills folder as junctions. Its sync tools (flight, chaos, animcheck, mutants) are vendored into the mod only with the MIT notice, under `third_party/coop-anything/`. The REA MCP (`rea-agents` 6.8.0, MIT, from npm) is installed in `work/tools/rea` and runs on the portable Node `work/tools/node-v22.23.4-win-x64` (SHA-256 checked against nodejs.org) with the lab's Ghidra 12.1.4 and JDK 21.
