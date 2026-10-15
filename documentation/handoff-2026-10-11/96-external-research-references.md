# External research references

On 15 October 2026 the user researched public resources for reverse-engineering Dishonored 2. No third-party table, script, DLL or game tool was run; co-op mods stay excluded. This chapter records the leads with their verification state. Claude's checks against the lab build are marked.

**Local copies (2026-10-15):** the LordRadai `cvars.md` (full clone at `work/Dishonored2-Debug`), the fakuivan 1.77.9.0 cvar dump, the drtchops autosplitter and a disrev clone are saved, pinned and hashed under `work/external-references` ([provenance](../../work/external-references/README.md)). They stay local: the LordRadai, fakuivan and disrev sources carry no license.

## Verified against the lab build

- **Build:** the lab executable is Dishonored 2 **1.77.9.0** (confirmed static, PE version resource). That is the build of the cvar dump and the autosplitter's main state.
- **Camera (IGCS):** [InjectableGenericCameraSystem, Dishonored 2](https://github.com/FransBouma/InjectableGenericCameraSystem/tree/master/Cameras/Dishonored2) (BSD 2-Clause), read as a reference only.
  - The lab already hooks the native render-view setter its notes describe (+0x890c50).
  - #61 builds the free observer camera on it (`coop_observer_camera hold_at`, [chapter 92](92-native-use-interactions.md)).
  - Its current camera uses the native Ansel mode, which pauses the game, so it is not usable for motion takes.
- **God mode is stripped (confirmed static):**
  - `ark_god_player` and `ark_god_npc` are registered at +0x138a6c0 / +0x138a670.
    - The `ark_god_player` object is at +0x398d500, value at +0x398d528, default "0", flags 9.
  - The only code reading the value is the `god` console toggle (+0x718f93); damage code never reads it.
  - **Confirmed runtime (take counterattack-v1258ai):** the source's health went 56 → 0 with the cvar set.
  - The lab therefore uses its own health hold instead (`coop_player_health_hold`, 1214ap, lab only; confirmed runtime in take counterattack-v1258ap).
- **AI cvars:** the build has 61 `ark_ai_*` cvars, but no notarget or perception switch among them (confirmed static).
- **Damage cvars from [LordRadai cvars.md](https://github.com/LordRadai/Dishonored2-Debug/blob/main/cvars.md) (confirmed static by a reference scan):**
  - Stripped like `ark_god_player`: `ark_buddha_player` (read only by its console toggle), and `g_damageScale`, `g_playerIncomingDamageScale` and `pm_disableFatalFall`, which have no readers beyond their registration.
  - Live: `gp_manaEnable` is read five times in the mana component code (+0x745ab9…+0x746d0d). It is a candidate for free power use in lab power takes.
  - `ark_inventory_mana_potion_autouse` (default 1) auto-drinks a mana potion when a power is cast without enough mana.
- **Mana potion drink (confirmed runtime, takes coverage-v1287a/b):**
  - The lab can grant a mana potion in product mode (`coop_grant_item mana`), but the native drink routine (+0x745b20) refused both on a full pool and after the lab wrote the pool to 0. Only the value changed; the component flags still read as before.
  - In take potion-mana-corvo-v67 the pool was spent by a native power (flags 0x2800006a → 0x2800000a), and the wheel drink then consumed the potion.
  - The mana elixir view therefore waits for a map with powers.
- **disrev** ([chipolux/disrev](https://github.com/chipolux/disrev), saved locally at `e9cf30e`): resource containers and `.entities` maps (world setup, objects, encounters). It is a lead for finding mission contexts for the remaining checklist items: windows, chains, shops.
  - Its `docs/levels.md` lists all 36 campaign maps by mission. Each mission starts on a Dreadful Wale map (`boat_<mission>_p`). Clockwork Mansion, Dust District, A Crack in the Slab, the Grand Palace and Death to the Empress span several maps. So stage 6 needs in-mission map transitions as well as mission-to-mission ones (confirmed static against the containers for Edge of the World: `boat_eotw_p`, `eotw_p_lowchaos`, `eotw_p_highchaos`).

## Leads not yet verified

| Need | Source | State |
|---|---|---|
| Loading state and level name (stage 6 automation) | [drtchops dishonored2.asl](https://github.com/drtchops/asl/blob/master/dishonored2.asl) (also the Som1Lse fork) | v1.77.9.0 offsets as declared: loading flag +0x288D2E8 (bool), level name +0x4025C50 (string128). Not yet read in the lab |
| Console commands and cvars | [fakuivan cvar dump v1.77.9.0](https://gist.github.com/fakuivan/8910a208fc88f831f06b047d0955e4fe); [LordRadai Dishonored2-Debug](https://github.com/LordRadai/Dishonored2-Debug) (`listCmds`, `listCvars`, `SaveGame`, `LoadGame`, `RestartFromCheckpoint`, `nextAI`) | Build matches. A listed command or cvar may be stripped (as `god` is) |
| Health, mana and ammo hooks | Cheat Engine tables (Fearless Revolution t=194 and t=27219, Nexus mod 187, Guided Hacking 231, FRAMED archive) | Pages blocked or not inspected; reference only, never run |
| Save decompression (stage 6) | ZenHAX PS4 save thread (t=14452): 512-byte reads, 0x20000 window, 4-byte size header; compared with [DOOM 3 BFG Compressor.cpp](https://github.com/id-Software/DOOM-3-BFG/blob/master/neo/framework/Compressor.cpp) | Hypothesis for PC saves. Decompression alone leaves serialization and checkpoint consistency open |
| Save location | Bethesda migration help: `%USERPROFILE%\Saved Games\Arkane Studios\Dishonored2\base\savegame` | The protected-file rules still apply: user saves are data and are never installed into the Steam saves |
| Resource formats | [chipolux/disrev](https://github.com/chipolux/disrev), Void Explorer (Nexus 30) | Asset containers and `.entities`; not runtime layouts |
