# coop-anything: data, research and template slice

Reader slice: `data/SCHEMA.md`, `data/directory.yaml` (all 8,892 lines read), `research/reference-repos.md`, `templates/game.md` and `DIRECTORY.md` (skimmed; its header and sections match the YAML). Clone: `work/external-references/coop-anything` (MIT). Nothing from the clone was run. I computed the statistics with my own line parser in the scratchpad (PyYAML is not installed) and checked them against the header of `DIRECTORY.md`.

Labels: **[static]** means read in the files. **[derived]** means counted or inferred by me. Everything in the directory is the repo's own claim, sourced to external pages that I did not open.

---

## 1. Schema summary (data/SCHEMA.md) [static]

Each entry is one co-op project for one game. A game can have several entries. `tools/build_directory.py` renders `DIRECTORY.md` from the YAML, so edits go to the YAML.

| Field | Allowed values / meaning |
|---|---|
| `game` | Display name of the base game (`(multiple)` for tools) |
| `year` | Base game release year |
| `engine` | Engine family (free text; see playbook/engine-families.md) |
| `project` | Mod or project name; `(none found)` for gaps |
| `url` | Main page; empty for gaps |
| `other_urls` | Mirrors, forks, Discord pages |
| `kind` | `mod`, `fork`, `decomp-port`, `official`, `emulator-netplay`, `framework` (plus `gap`) |
| `status` | `playable`, `partial`, `alpha`, `prototype`, `abandoned`, `official` (gaps use `none`) |
| `coop_scope.players_visible` | Can the players see each other at all |
| `coop_scope.story` / `enemies` / `world_state` | `yes`, `partial`, `no`, `unknown` (only what a source confirms) |
| `coop_scope.items` | Same values, plus `per-player` and `shared` |
| `players` | Player count as the project states it (free text) |
| `networking` | `direct-ip`, `vpn-needed`, `steam-relay`, `dedicated-server`, `hosted-relay`, `lan`, `unknown` |
| `open_source` | `yes`, `no`, `partial` (licence goes in notes) |
| `last_activity` | YYYY-MM of the last confirmed release or commit |
| `qa` | See the QA levels below |
| `notes` | Free text |
| `sources` | URLs. Every claim must be backed by one |
| `checked` | Date the entry was last verified |
| `up_for_adoption` | Optional; the author stopped and wants a new owner. Gets its own DIRECTORY section |
| `made_with_coop_anything` | Optional; the project was built or revived with this hub |
| `searched` | Gaps only: where and when someone looked (e.g. "GitHub, Nexus...; Reddit not checked"). `sources` may then be empty |

**QA levels ("the flag that matters most"):**
- `none`: no reported testing, or only the author's claims. Rendered as "untested".
- `community-reports`: players report playing it (forums, Steam, Discord, Reddit), with no structured testing. Rendered as "player reports".
- `automated`: a scripted harness drives real game clients and checks shared state (playbook/automated-qa.md). AGENTS.md says it requires a test you actually ran, with the suite summary pasted into the write-up. Rendered as "automated tests".
- `verified-playthrough`: a full campaign was finished in co-op and written up with known issues.

Rules: don't guess, write `unknown`, and every entry needs at least one source URL and a `checked` date.

---

## 2. Directory statistics and patterns [derived]

**Size:** 340 entries: 302 projects and 38 gaps across about 218 games. The DIRECTORY header and my parse agree.

| Field | Counts (302 projects) |
|---|---|
| status | prototype 69, alpha 65, playable 64, partial 57, abandoned 35, official 12 |
| kind | mod 230, fork 25, framework 22, decomp-port 11, official 8, emulator-netplay 6 |
| networking | unknown 89, dedicated-server 50, steam-relay 46, direct-ip 41, hosted-relay 38, lan 20, vpn-needed 10, blank 8 |
| qa | none 180, community-reports 85, automated 37, **verified-playthrough 0** |
| open_source | yes 152, unknown 86, no 39, partial 25 |
| story | unknown 118, partial 67, no 56, **yes 53** |
| enemies | unknown 168, yes 71, partial 29, no 28 |
| items | unknown 191, per-player 45, shared 26, partial 16, no 14 |
| flags | made_with_coop_anything 31; up_for_adoption 1 (Crimson Desert cdmp) |

**Engine families (my bucketing):** Unreal 1/2/3/4/5 (36), N64 decomp or recomp ports (32), Unity (26), Creation/Gamebryo (19), REDengine (16), emulators (14), RAGE/RenderWare (13), CryEngine/Dunia (12), id Tech/Quake/Doom (11), Source/GoldSrc (7), plus single-digit families and about 80 one-off in-house engines. **Void Engine appears exactly once (the Dishonored 2 ZikQ entry).**

**QA crossed with status:** 30 of the 35 abandoned entries have `qa: none`. Of the 37 automated entries, 10 are playable, 18 alpha, 7 prototype and 2 partial. About half of the automated projects (19 of 37) use steam-relay. Most of them are `made_with_coop_anything` and date from 2026-10.

### What distinguishes the projects that reached full story co-op
Nobody has a `verified-playthrough`. The strongest claims are:
- **G3 Together (Gothic 3):** built from zero on the game SDK with ENet. 187 bot runs drive two real clients, and the bots played the story to one ending. People have not played it yet. This is the closest thing to a verified playthrough.
- **Jericho Co-op+:** automated tests cover the whole campaign over LAN between two PCs, with "every story split with a real host and client". A human session covered missions 1–8.
- **mdk2-coop:** an automated ladder of 31 gates, including all 96 checkpoints with two real clients and a host/client state diff.
- **mw2_coop:** a bot tour of all 19 level start points. Humans played up to No Russian.
- **Crime Scene Cleaner Co-op:** two humans finished chapters 1–6, and bot suites cover the rest.
- Older playable story mods with community reports only: Sven Co-op, Synergy, Seamless Co-op (Elden Ring, Dark Souls), F.E.A.R. Cooperative, Crysis Co-op and Shadow of COOP.

Common traits of the successful projects [derived]:
1. **Host-authoritative world.** Joiners mirror the host, and their inputs or hits are validated or replayed on the host (Stacklands, Autonauts, re2coop, BioShock Together, Gedonia, Shipwright-Online).
2. **Story replicated as events or effects, not by re-running scripts on each client.** fnv-coop captures "the effects of a quest script as one transaction" and clients apply them. RedSync relays host quest facts through a whitelist and sends late-join fact snapshots. MiSide relays story events (presses, triggers, dialogue choices) rather than state. Gedonia runs story calls on every machine in the host's order.
3. **Ownership with epochs.** Gedonia gives enemies, followers and world events "one tester with epochs". The OpenOL fork fixes duplicated map enemies with a rule that the lower player id wins.
4. **Reusing the game's own netcode or multiplayer remnants when it exists.** NewDark, SS2, the MW2 Spec Ops lockstep, Code Vein's sessions, Obscure's local co-op and World of Goo / Little Inferno's leftover multi-cursor code.
5. **A proxy DLL as the injection vector:** dinput8, winmm, xinput1_3, d3d9, AnselSDK64, SDL2 or version.dll.
6. **Steam lobbies plus P2P with no port forwarding.** Usually this means Spacewar AppID 480. Some projects use the game's own Steamworks instead: MiSide ("Steam P2P through the game's own steam_api64"), Baldi, Spooky, Gedonia and Graveyard Keeper. That matches our rule.
7. **Bot suites with twin or mutation checks:** every check has a twin flag that must turn red (DRG Survivor, Spooky).
8. **Separate co-op saves:** Elden Ring and Dark Souls Seamless, plus mdk2 co-op saves.

### Common failure modes named in the notes [derived]
- **Duplicated world entities when every client runs its own copy.** OpenOL's map enemies are the clearest case. RAGECOOP warns that only one player may start a mission or entities duplicate.
- **Triggers fire for everyone wherever they are** (OpenOL). Checkpoint loads are not shared (OpenOL). Events need a room transition before they apply (Windwaker-coop).
- **Cutscenes are missing or unsupported for clients:** GTA IV Mission Loader, BioShock Together, Far Cry Assault Coop, FarCryCoop and Spider-Man.
- **Per-player objectives in some levels and individual boss fights** (BioShock Together). Clients crash in scripted sequences.
- **Remote avatars lag, warp or stick** (stock NewDark: sends only when 4+ units off, 12-unit snap, uninitialised steering). T2MPFix cut path deviation from 7.65 to 0.13.
- **Version pinning and binary hash coupling.** Hooks at hardcoded addresses break on remasters (w3m, Witcher Online on 5.01). Many mods require identical versions.
- **Quest state left per-player** (NV:MP, Witcher Online, KCD2). These read as presence co-op, not story co-op.
- **Abandonment, usually with `qa: none`,** and no source, so revival is impossible (Starfield Together, F4MP). Legal takedowns also happen: Sony stopped the TLOU2 MP mod.
- **"Tested on one PC, two copies; two PCs, internet and 3+ players untested"** is the most common caveat. Several projects say the two-real-Steam-accounts path has never run (W3Coop, re2coop, Baldi, MiSide).
- **Claims without in-game runs:** Arx co-op was never tested over the internet, and ZoneCoop "has not been run in the game yet".

---

## 3. Entries most relevant to us

**Arkane / Void / id Tech lineage**
- **Dishonored 2 COOP mod (ZikQ), Nexus mods/222:** alpha, story partial, 2 players, vpn-needed, `qa: none`. Noted as existing only. Per project rules it was not researched, opened or used.
- **Dishonored COOP mod (J.P), D1 Definitive Edition:** a d3d9.dll mod with Steam lobbies, invites and voice. The second player drives a body that mirrors the host rather than a full Corvo. Partial, community reports, Sept 2026 mod of the month.
- **dismod (D1), GitHub ectrc:** a multiplayer prototype with a character spawning panel. Its notes say it "bypasses Steam DRM to connect to servers", which is the opposite of our native-Steam rule.
- **Arx Fatalis (Arkane's first game),** two forks of Arx Libertatis: kingzmanh (2 players, shared quests, proximity voice, never tested over the internet) and ArxCoop (2–4 players, LAN/VPN). Both are alpha and GPL-3.0.
- **Dark Messiah (Arkane, Source):** the wiltOS co-op mod, alpha, hosted-relay master server. An early test build lacked player models and cutscenes.
- **Prey (2017, Arkane, CryEngine), Prey Coop/CoopPrototype:** full-campaign alpha on Chairloader, new or existing saves, UDP 27015 direct IP, "expect desyncs and crashes". This is the closest Arkane immersive-sim analogue.
- **id Tech:** Doom 3 (LibreCoop on dhewm3, GPL; OpenCoop), Quake 4 OpenCoop (2007 tech demo), DOOM Eternal Co-operative Mode (built on the Battlemode lobby), and Wolfenstein TNO/II (id Tech 5, the ancestor of Void) as a **gap**.
- **Not in the directory at all:** Deathloop, Redfall and Dishonored: Death of the Outsider.

**Immersive sims**
- **Deus Ex:** HX (campaign co-op, dedicated server, last build 2019) and cranebeaker deus-ex-coop (HX plus fixes plus a Steam relay, automated suite with lag and packet-loss runs, playable).
- **System Shock 2:** the original built-in co-op unlocked by SS2Tool (3 players), plus Nightdive's 2025 remaster advertising 4-player co-op.
- **Thief II:** NewDark's built-in MP beta plus T2MPFix, automated A/B tests on avatar smoothing (a lesson for our Doppelganger rig movement). Thief Gold is a gap.
- **BioShock Together:** host-controlled enemies. Doors, switches, hacking and quest items are shared, while ADAM and choices are per-player. Cutscenes are not fully supported. This is the closest design analogue to our choice-based Chaos and story split.
- **Gothic 3 G3 Together:** the best-QA'd story co-op (see section 2).

**Stealth and first-person story**
- **Outlast:** OpenOL plus a G3RKA fork. A relay, a 67-pass two-client suite, duplicate-enemy and trigger-scope lessons.
- **Alien: Isolation:** a prototype aimed at Survivor mode.
- **Splinter Cell: Chaos Theory:** official co-op campaign, now needs virtual LAN.
- **Hitman: Blood Money:** gap.
- **Metro 2033 and Exodus:** gaps.
- **Jericho Co-op+:** dinput8 proxy, ENet or Spacewar, host-authoritative story, scenes, powers and doors. Whole-campaign automated tests.
- **Obscure:** an xinput proxy turns the host's local P2 into a network peer, and the host mirrors its game to the friend.
- **MDK2 coop:** cutscene skip vote, revive at partner, co-op saves, 96-checkpoint gate.
- **Crime Scene Cleaner:** a co-op screen in the main menu, the host picks the save slot, the client follows the host into jobs.
- **Clive Barker's Jericho, The Witness, Night in the Woods:** first-person or story prototypes.
- **Cyberpunk RedSync:** host-authoritative quest-fact relay plus late-join snapshot. **fnv-coop:** effect-capture story transactions. Both map directly onto our move of story to host authority.

---

## 4. research/reference-repos.md [static]

The file is the hub's own design research (2026-10-16). It is about agent-contribution mechanics, not co-op code.

| Repo / topic | What it is | Relevance to us |
|---|---|---|
| **wesbos/photo-frame-dashboard** | Agent-driven hardware rooting repo. README "For agents: start here", nine hard rules (backup before write, verify by readback, SHA-256 pinned downloads), identity gating by board and firmware, a phase map, a PR template with privacy and safety checklists, a maintainer `triage-contributions` skill | Process only. Its identity gating (exe SHA, build id) and private/public directory split mirror our protected files and evidence rules |
| **morluto/rea** | "One MCP for reverse engineering": wraps Ghidra, IDA and Hopper (bring your own); `npx rea-agents setup`; read-only analysis; AGENTS.md rules (separate observations, inferences and unknowns; report only checks that ran; PRs stay draft) | Tool. We already use Ghidra via our d2-ghidra MCP. Its labelling rules match our evidence labels. Optional |
| **LaurieWired/GhidraMCP** | Ghidra plugin HTTP server plus Python MCP bridge (decompile, rename, list) | Same class of tool as our d2-ghidra MCP. Useful only as a reference |
| **mrexodia/ida-pro-mcp** | IDA plugin JSON-RPC MCP plus a headless idalib supervisor; ships agent prompts writing to `RE/*.md` | Tool. Relevant only if we ever add IDA; lesson: thin installs plus a real toolset plus a prompt |
| Claude Code, Codex, Gemini, Cursor, Aider, OpenCode subagent config | Concurrency and model keys, e.g. `CLAUDE_CODE_MAX_CONCURRENT_SUBAGENTS` default 20, spawn depth 3 | Process tuning for multi-agent work |
| AGENTS.md convention | One shared file; `CLAUDE.md` imports `@AGENTS.md` | Process |
| Recommendations | Repo structure, 11 AGENTS.md rules, PR flow, index format with an identity column | Informs how we would contribute an entry |

None of these is a co-op mod, so none conflicts with our "no third-party co-op mod code" rule. Generic tools are fine. The directory itself names generic libraries and frameworks worth knowing:
- networking: ENet, Valve GameNetworkingSockets (RedSync), Mirror/KCP (QSB), Steamworks.NET;
- the Anchor relay;
- loaders: MinHook, Ultimate ASI Loader, BepInEx/Harmony, UE4SS, RED4ext, Chairloader (Prey);
- tools: Nucleus Co-op and Universal Split Screen (multi-instance), Archipelago (multiworld).

For our native C++ DLL on Void, the transferable pieces are **MinHook-style hooking, proxy-DLL loading and GameNetworkingSockets-style reliable channels over the game's own Steam P2P.**

---

## 5. templates/game.md and a draft for us

**Template [static]:** a header table (Engine, Versions tested, Projects, QA level, Last updated), then these sections:
- *Where things stand* (2–3 sentences);
- *Existing work*;
- *How the game works (for co-op)*: engine, scripting, netcode, saves, level loading, client vs server, tools;
- *What we changed*: bug, root cause, fix, the test that proves it;
- *QA*: command, assertions, last suite result;
- *How to play*;
- *Known issues*;
- *Next steps*.

### Draft directory entry (our project) [hypothesis where marked]

```yaml
- game: Dishonored 2
  year: 2016
  engine: Void Engine (id Tech 5 derived)
  project: Dishonored 2 Corvo/Emily co-op (adamkozel/Dishonored-2-coop)
  url: https://github.com/adamkozel/Dishonored-2-coop
  other_urls: []
  kind: mod
  status: alpha            # UNKNOWN: alpha vs partial; releases 0.9.x shipped, final audit reopened cards
  coop_scope:
    players_visible: 'yes' # native Doppelganger rigs, first person
    story: partial         # host authority for story in progress
    enemies: 'yes'         # host-authoritative combat
    world_state: partial   # pickups/doors/safes/far NPCs moving to host authority
    items: unknown         # per-player vs shared not confirmed here
  players: '2'
  networking: steam-relay  # Steam P2P / friends-only lobby via the game's own steam_api64 (own AppID, not Spacewar)
  open_source: 'yes'       # UNKNOWN licence; repo public since 2026-10-17
  last_activity: 2026-10
  qa: automated            # two-instance lab plus a second-account VM; confirm the suite qualifies
  notes: Native C++ DLL for the Steam build. Corvo and Emily in all four pairings, host picks character
    once per save, partner every join; co-op menu entry, isolated co-op saves, Leave co-op. Partners
    excluded from each other's direct interactions. Release 0.9.6 install-tested on a clean PC.
    Reverse engineered with Ghidra (MCP). Built independently, no third-party co-op mod code.
  sources:
  - https://github.com/adamkozel/Dishonored-2-coop
  checked: '2026-10-29'
  made_with_coop_anything: false
```

### Draft write-up header

| | |
|---|---|
| Engine | Void Engine (Arkane, id Tech 5 derived), x64 |
| Versions tested | Steam release (UNKNOWN: exact build id and exe SHA-256 to record) |
| Projects | ours, and Dishonored 2 COOP mod (ZikQ), listed only and not consulted |
| QA level | automated (two-instance lab plus a two-account VM over a friends-only Steam lobby) |
| Last updated | 2026-10-29 |

- **Where things stand:** Corvo and Emily can play the campaign together over Steam. Combat is host-authoritative. Story, pickups, doors, safes and far NPCs are being moved to host authority (stage 6). The final audit reopened cards in stages 2, 4, 5 and 7, so a full verified playthrough is not claimed.
- **How the game works:** Void Engine with no shipped netcode. Fill in from our docs: scripting layer, save system, level loading and Doppelganger rigs.
- **What we changed / QA / Known issues:** map from `documentation/handoff-2026-10-11` and the evidence catalog. Each fix gets bug, root cause, fix and take id.
- **How to play:** launch through Steam (otherwise the game shows Free Trial), host from the co-op menu, partner joins by invite.

**Gaps to fill before publishing:** licence, exact build and hash, the items scope, honest status (alpha or partial), and the suite summary needed to justify `qa: automated`. Per our privacy rules: no SteamIDs and no game-derived assets in any submission. Whether to submit at all is the user's decision.
