# Stage 7: release packaging, native Steam sessions and product mode (candidates 1171–1213b)

Updated 2026-10-14. Audit: [stage7-release-session-audit-v1180](../../work/movement-integration/stage7-release-session-audit-v1180.json).

## Release payload (read-only comparison, 1177)

The lab's game copy (`work/asset-game`) was compared with the original Steam install by size and SHA-256. The Steam install was **only read**.

The co-op payload is exactly these files:
- `dinput8.dll`, the mod proxy;
- `base/game1voidmod.index` and `.resources`, through `game4voidmod`, the partner asset packages;
- a modified `base/master.index`, which registers those packages.

The 147 small shared files, including `Dishonored2.exe` and `steam_api64.dll`, are byte-identical. Native Steam is never replaced.

## Package, install and rollback

[`build_release_v1177.py`](../../work/release/build_release_v1177.py) builds `outputs/release/d2coop-v1171-r2` from the live candidate. It contains the payload, a manifest of hashes, `Install-D2Coop.ps1` and `Uninstall-D2Coop.ps1`.

**Install** (the user points it at a folder):
- Refuses unless `Dishonored2.exe` and `steam_api64.dll` are the exact supported builds and `master.index` is the original.
- Verifies the package hashes.
- Backs up everything it replaces into `d2coop-backup/<time>/`.
- Installs the payload, verifies every hash and writes a receipt.

**Uninstall** uses the receipt:
- Removes the added files and restores the originals byte-identically.
- Verifies them, then removes the receipt and the backup.

**Test ([`Test-ReleaseInstall-v1177.ps1`](../../work/release/Test-ReleaseInstall-v1177.ps1), scratch targets only):**
- The first package, `d2coop-v1171`, failed: its hash helper `H` was shadowed by PowerShell's alias `h` (`Get-History`). It was never installed anywhere and is marked superseded.
- r2 passed all six cases:
  - install;
  - a second install refused;
  - rollback byte-identical;
  - a wrong executable refused with nothing changed;
  - a modified `master.index` refused with nothing changed;
  - the Steam install verified untouched.

## Native Steam session flow (candidate 1178)

[`session_online.h`](../../outputs/d2-coop-lab/src/session_online.h) uses only the game's own `steam_api64` interfaces (`SteamFriends015`, `SteamMatchMaking009`, `SteamUser019`, `SteamUtils008`) plus SteamNetworkingSockets.

**Host (`coop_session host`):**
- Creates a friends-only Steam lobby for two, tagged with the build version.
- Sets rich presence `connect=+connect_lobby <id>`, so Steam's own friends list offers *Join Game*.
- Opens a P2P listen socket and Steam's native invite overlay.
- Accepts exactly one partner who is not itself.

**Joiner:**
- Joins through a Steam *Join Game* or accepted invite (`GameLobbyJoinRequested_t`, registered with `SteamAPI_RegisterCallback`), the `+connect_lobby` launch argument, or `coop_session join <id>`.
- Joins the lobby, checks the version tag, and connects P2P to the lobby owner.

**Other details:**
- Roles: the host is Corvo (0) and the joiner Emily (1).
- Async results are polled with `ISteamUtils`.
- The lab's loopback transport is unchanged unless a session command is used.

**Runtime ([`Test-OnlineSession-v1179.ps1`](../../work/release/Test-OnlineSession-v1179.ps1), pass):**
- The host created a real Steam lobby, set its data and rich presence, opened the P2P listen socket and showed Steam's "Choose Friend to Invite" overlay in-game. No invite was sent.
- The second lab instance, on the **same Steam account**, entered that lobby and read the version tag. It then refused to connect to itself (`lobby_owner_is_self`). That is exactly where a second account is required.
- After the sessions were turned off, the lab loopback transport recovered.

**Notes:**
- The invite capture shows the user's Steam friends list. It stays local evidence and is not published.
- The invite dialog stayed open on the host lab window: closing it through desktop control was declined, so the user can close it with ×. Automated lab hosting should not open the dialog.

## Product mode and session controller (candidate 1181, audit 1185)

[Audit 1185](../../work/movement-integration/product-controller-audit-v1185.json).

**Product activation.** With no lab environment, the proxy activates only when `<game>\d2coop\d2coop.cfg` exists.
- It installs only the engine bridge: no lab window policy, instance-mutex rename, simulated input or command file.
- Status files and both partner movement banks live in `d2coop\`. The bank for the partner being rendered is loaded (the host renders Emily).
- Partner assets are detected from `game1voidmod.index`.
- Mission checkpoints read the game's own save details; the `savegame=` path in the config is optional.
- **F7** hosts (Steam lobby and invite overlay) and **F8** leaves. Joins arrive through Steam's Join Game or invite, or `+connect_lobby`.

**Session controller.** For every new shared world, the DLL itself arms the standard feature set, using the same validated command dispatch the lab command file uses (`dispatchCoopCommand`):
- cinematic sync and barrier, and world observation;
- the Doppelganger partner, with follow, locomotion, movement bank and shadow stream;
- avatar perception;
- door, usable and coin sessions;
- NPC stream, combat and attack;
- mission checkpoints and the trigger mirror.

Every 5 s it also re-issues the idempotent session commands, so a system refused transiently is armed again. The lab-only `coop_product_auto on` runs the same controller in the isolated lab.

**Runtime (1183, pass; 1182 failed on a harness alias clash).**
- The harness sent only `coop_product_auto on`, and the controller armed all 17 steps on both peers.
- It kept everything armed through two shared mission restarts: doors, coins, usables and the NPC stream renewed, and the partner avatar was restored 2 of 2.
- At the post-restart sample the client had 0 bound NPC channels while the host had 9. The client binds on host Hellos, so convergence wasn't captured by that sample.

**Package r3** (`outputs/release/d2coop-v1181`, [`build_release_v1184.py`](../../work/release/build_release_v1184.py)) adds `d2coop/d2coop.cfg` and both partner banks: 13 files in all. The install/rollback test passes all six cases on scratch targets ([result](../../work/release/release-install-v1184/result.json)). Rollback also removes the `d2coop` folder.

## Stability soak (1186, audit 1190)

[Audit 1190](../../work/movement-integration/stability-soak-audit-v1190.json). Candidate 1181, Edge of the World, product controller only.
- 20 minutes with movement across the map every 30 s and three shared deaths/restarts (both roles).
- No crash, the transport never went stale, every session was armed at the end and handle counts stayed flat (confirmed runtime).
- Private memory grew about 6–7 MB/min on both peers. Not yet attributed: a control soak without co-op systems must separate the game's own streaming from the mod.
- The NPC transport queue reached about 1 s during teleports and restarts, with no stale state.

**Control soak (1198, audit [1200](../../work/movement-integration/soak-control-audit-v1200.json)).** This run used the same mission, route, 20 minutes and three death/restart cycles, with every co-op system off (bridge transport and status writers only). Both peers died together and restarted natively.
- Steady-state private memory still grew by about 5 MB/min (host 4.9, client 5.3), excluding the first minute after each restart (a transient load peak). The three native restarts added about 190 MB more.
- With the product controller (1186) the steady rate was 6.0 and 7.1 MB/min. The co-op systems add at most about 1–2 MB/min; most of the growth is present without them (confirmed runtime).
- Still open: a vanilla baseline without the proxy (needs a lab instance relaunched without `dinput8`), and a 2-hour soak to see whether the growth plateaus.

**2-hour soak (1316, candidate 1214cf, [take](../../work/natural-moveset/mission-soak-v1316a/result.json), pass, confirmed runtime).** Same course as 1186 for 120 minutes, armed only by the product controller.
- 19 shared deaths, alternating host and client: both death screens appeared and both worlds restarted every time.
- At the end the transport was fresh. Product and checkpoint were armed, and the door, usable and NPC sessions had renewed after the last shared checkpoint.
- Handle counts stayed flat (host 1413→1409, client 1394→1394).
- Private memory grew steadily at 7.75 MB/min (host) and 7.56 MB/min (client), restarts included. That matches the 20-minute run (6.0–7.1) against the game's own rate in control 1198 (about 5 MB/min plus about 60 MB per restart). No degradation over time was seen.
- Still open (#13): a matched control without the co-op systems or the proxy.

## Join-follow (candidate 1188, audit 1192)

[Audit 1192](../../work/movement-integration/join-follow-audit-v1192.json).
- The host publishes its current mission on `D2MF` packets: the `idDeclMapInfo` name from `idCommonLocal+0x2b8`, and chaos from the loaded map name.
  - That declaration is null during a mission started with the `campaign` command (confirmed runtime, take route-institute-v1303am), so nothing was published then. Since 1214bu the mapinfo is derived from the loaded map's name when it is absent ([chapter 97](97-campaign-map-sequence.md)).
- A joiner on another map loads the same mission natively with the game's own `campaign <mapinfo> <low|high> emily`. It does this once per host map, from gameplay or a menu, never while loading or in a cinematic.
- Product mode follows automatically; the lab uses `coop_follow_host on`.

**Runtime:**
- **1191, pass.** The host on Edge of the World published 15 packets. The client on Dunwall received 14, loaded the mission once and reached `same_mission`.
- **1189.** The same functional pass, but flagged *Recording failed*: ffmpeg's exit code across the map load. Both recordings are intact; the client video shows Dunwall, the loading screen, then Emily at the docks. The harness now checks recording duration instead.

## Product-mode boot (candidate 1193, audit 1196)

[Audit 1196](../../work/movement-integration/product-boot-audit-v1196.json). [`Test-ProductBoot`](../../work/release/Test-ProductBoot-v1194.ps1) starts a standalone process from the lab game copy:
- no lab environment;
- a temporary `d2coop\d2coop.cfg` and partner banks;
- an isolated `fs_savepath`.

The process is closed and the temporary folder removed afterwards.

**1187 (candidate 1188), failed.**
- Product mode activated by itself: status in `d2coop`, partner assets detected, controller `waiting_partner`.
- Steam interfaces were only acquired on host or join, so the `GameLobbyJoinRequested` callback wasn't registered: an in-game Steam *Join Game* or accepted invite before F7 would be missed.
- The one-PC lab loopback listener also opened.

**Candidate 1193 fixes both.**
- Product mode acquires the game's own Steam interfaces at boot, retrying each second.
- It never opens the lab loopback; it waits for a Steam session.

**1194 (candidate 1193), pass.**
- Steam ready at boot, with no UDP 31426 endpoint.
- No session started by itself.
- The game reached its native boot screens.
- Cleanup was complete, the protected files were unchanged and the lab stayed healthy.

**Package r4** (`outputs/release/d2coop-v1193`, [`build_release_v1195.py`](../../work/release/build_release_v1195.py)):
- Contains the same 13 files plus a player `README.txt` (install, F7 host, Steam join, F8 leave, the uninstall, and the fact that online play is not yet validated).
- Install/rollback passes all six scratch cases ([result](../../work/release/release-install-v1195/result.json)).
- r3 (`d2coop-v1181`) is marked superseded.

## Package r5 (candidate 1213b)

`outputs/release/d2coop-v1213b` ([`build_release_v1216.py`](../../work/release/build_release_v1216.py)) carries:
- protocol 74 and bank 71/374, with the native use interactions from [chapter 92](92-native-use-interactions.md);
- the product presentation recovery from [chapter 91](91-product-presentation-recovery.md).

Peers on different protocols refuse each other's action packets, so r4 (`d2coop-v1193`) is marked superseded.

Results:
- Install/rollback passes all six scratch cases ([result](../../work/release/release-install-v1216/result.json)).
- The product-mode boot passes again on 1213b ([1217](../../work/release/product-boot-v1217/result.json)): Steam ready at boot, no loopback, idle, cleaned up.

## Two-account connection through the lab VM (take 1221)

The second Steam account runs in the Hyper-V lab VM `D2CoopVM`. It has its own Dishonored 2 install, and its `Dishonored2.exe` is byte-identical to the lab copy. The two accounts are Steam friends. `tools/vm/Sync-D2LabVMWorkspace.ps1` mirrors the lab into the guest, and `tools/vm/D2LabVM.Lab.ps1` drives the guest instance from the host.

**Result** (confirmed runtime, [take](../../work/natural-moveset/two-account-session-v1221/result.json)):
- **Host:** the host lab instance (1213d, main account, Dunwall fixture) ran `coop_session host` and got a friends-only lobby.
- **Join:** the VM lab instance (same DLL, second account, same fixture) ran `coop_session join <lobby>`.
- **Link:** Steam P2P connected on both sides: phase 5, connection state 3, packets both ways, `remoteFresh`.
- **Movement:** the host saw the VM player walk 1.55 m, and the VM saw the host player walk 5.47 m.
- Visual acceptance was not assessed.

**Findings**
- **Lab mode keeps a stale loopback connection (confirmed static and runtime, #23).** `coop_session host` started from lab mode leaves the loopback connection handle in place, because `stopOnlineSession` resets the transport only when an online session is already active. While the host's local client exists, every lobby partner is refused. The take restarted the host instance alone. Product mode is unaffected.
- **The VM has no controller device (confirmed runtime, #24).** The simulated controller is ignored inside the VM. The Hyper-V keyboard drives the VM role meanwhile.
- **The release needs the VC++ runtime (#22).** The DLL and probe need the VC++ 2015–2022 runtime, which a clean Windows install such as the VM lacks. The lab copies it app-local; the release must solve it too.

**Not run yet:**
- F7 product hosting with the invite overlay / Steam Join Game, from the release package installed in the VM;
- disconnect and reconnect.

## Stage 2 completed: lab fix, reconnect and the product path (1213e/1213f, takes 1222–1225)

**1213e (#23):** `startOnlineHost`/`startOnlineJoin` now close the lab loopback link before the online session takes the transport.

**Take [1222](../../work/natural-moveset/two-account-session-v1222/result.json):** two machines and two accounts, with the host's local client connected.
- The VM joins and both sides connect.
- Movement syncs both ways: 6.39 m and 4.28 m.
- The VM leaves and the host releases the slot.
- The VM rejoins and connects with no stale ownership (4.32 m).
- The take is recorded as `passed=false` only because Defender quarantined the DLL during restoration (#25). Afterwards the project folder and `C:\D2Lab` were excluded by the user.

**1213f (#22):** the C runtime is statically linked. Take [1224](../../work/natural-moveset/vm-clean-crt-v1224/result.json): the VM, with no VC++ runtime anywhere, runs the mod fully.

**Release r6** (`outputs/release/d2coop-v1213f`, local; builder `work/release/build_release_v1225.py`): 1213f with the same payload layout as r5.

**Take [1225](../../work/release/product-two-account-v1225/result.json)** walked the product path the way players use it, with two accounts on two machines:
1. r6 was installed into the VM's own Steam game with its own installer.
2. The game launched through Steam, and product mode activated.
3. The host product instance (lab copy, isolated profile with the fixture save) reached gameplay from real keystrokes.
4. **F7** created the friends-only lobby and opened the invite overlay.
5. The VM joined through Steam's join route (`steam://joinlobby`, the mechanism behind Join Game and invite acceptance). Both sides reported connected (phase 5, state 3) with fresh data.
6. The **uninstaller rolled back** the VM install: 12 added files were removed and `master.index` restored. The exe is unchanged.

The joiner was still at its main menu when the link came up. Join-follow into the host's mission is part of the stage 6 and campaign work.

## What stage 7 still needs

- **Two-account online validation** (host, join, invite, disconnect and reconnect) needs a second Steam account and the user. It remains outstanding until it is actually performed.
- **Native in-game UI** for host/join and character choice (the host currently plays Corvo).
- **Soak attribution**: a vanilla baseline without the proxy and a 2-hour soak (the control soak shows the co-op systems are not the main source).
