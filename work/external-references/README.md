# External references (local only)

Saved 2026-10-15 at the user's request, as reference text. Nothing here is executed. These files stay out of git: the repo `.gitignore` allowlist excludes this folder, and the LordRadai, fakuivan and disrev sources carry no license. Chapter 96 records what was verified from each source.

| File | Source | Pin | SHA-256 |
|---|---|---|---|
| `../Dishonored2-Debug/cvars.md` (full clone) | [LordRadai/Dishonored2-Debug](https://github.com/LordRadai/Dishonored2-Debug), no license | commit `ed1a72be2cec4ae5542524b72da8680b7280c09e` (upstream head on 2026-10-15) | `01953094941c2458f95934c19066e5ddd2f3a63aee3696dce484740b082a78fe` (CRLF checkout) |
| `fakuivan-cvars-1.77.9.0/dishonored2_cvars.txt` | [fakuivan gist](https://gist.github.com/fakuivan/8910a208fc88f831f06b047d0955e4fe): every cvar for 1.77.9.0, with its struct address, default and int-value address | gist version `86278f0f765e7bf8e33bfd545980e6d37576360e` | `e5bfaad6312987d5f69b239db0f8db276b216270ef27398cfbe4a8369e94a256` |
| `drtchops-asl/dishonored2.asl` | [drtchops/asl](https://github.com/drtchops/asl/blob/master/dishonored2.asl), MIT | commit `a5b2db7bbcf65f4b92d8ca00f369ee2e34549553` | `0882648045968b5c67208245f6f95d3c9e766c8ef3dd8affa2ff2271c9603718` |
| `disrev/` (full clone) | [chipolux/disrev](https://github.com/chipolux/disrev), no license: resource and `.entities` notes, Python readers and the voidtweak C++ editor (Kiscule, BWM, decls) | commit `e9cf30eec5fa3d35eeef9c448f12f8c711a5914c` | git commit pin |

## How to use them

- **cvar lookups:** grep the fakuivan dump for a name and address (`Dishonored2.exe+<struct> | name = default | +<value>`), and LordRadai's `cvars.md` for descriptions. Then check the value address's readers in Ghidra: a registered cvar can be stripped (god mode is; see the runbook's "Lab-only commands and dead cvars").
- **Campaign map list:** `disrev/docs/levels.md` lists every mission's maps (a Dreadful Wale `boat_<mission>_p` map, then one or more mission maps, split by chaos). It plans stage 6's map and transition coverage; the `.entities` readers there overlap `tools/diag/d2_resource_index.py`. Never build or run disrev or voidtweak.
- **Autosplitter state for 1.77.9.0:** `isLoading` bool at +0x288D2E8 (alternative +0x3BDA0A8) and `levelName` string128 at +0x4025C50. These are leads for stage 6 load and mission detection, not yet read in the lab.
