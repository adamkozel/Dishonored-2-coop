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
- Its matched control is 1664b below.

**2-hour control soak (1664, candidate 1214fq, [take](../../work/natural-moveset/mission-soak-control-v1664b/result.json), pass, confirmed runtime).** The 1198 control for 120 minutes on the course of 1316 (same mission, route, 30 s moves and a death every 6 minutes), with every co-op system off: only the bridge transport and the status writers run.
- 19 shared deaths, alternating host and client: both death screens appeared and both games restarted natively every time. 692 samples.
- Handle counts stayed flat (host 1375→1373, client 1378→1380).
- Private memory grew steadily at 18.3 MB/min (host) and 18.0 MB/min (client), 2.1 GB over two hours. The co-op soak 1316 grew 7.6–7.8 MB/min, so the co-op systems add no growth beyond the game's own rate. The game's own rate varies between runs (about 5 MB/min over 20 minutes in 1198).
- A run without the proxy cannot follow this course: the lab moves the players through the DLL's command file and its simulated controller (`Send-D2Input` publishes a packet the DLL consumes; confirmed static), so the bridge-only run is the closest matched control.
- 1664a, the first attempt, hung while a six-worker bank fit ran beside it on this PC; it is kept as evidence.
- Next: a 2-hour co-op soak on the 0.8.0 candidate, so co-op and control are measured on the same build.

**2-hour co-op soak on the release binary (1698, candidate 1214fz, release 0.8.1, [take](../../work/natural-moveset/mission-soak-v1698a/result.json), pass, confirmed runtime).** The course of 1316 and 1664 (same mission, route, 30 s moves and a death every 6 minutes), armed only by the product controller.
- 19 shared deaths, alternating host and client: both death screens appeared and both worlds restarted every time. 690 samples.
- At the end the transport was fresh. Product and checkpoint were armed, and the door, usable and NPC sessions had renewed after the last shared checkpoint.
- Handle counts stayed flat or fell (host 1477→1380, client 1491→1400).
- Private memory grew steadily at 4.64 MB/min (host) and 4.79 MB/min (client), 550 MB over two hours, restarts included.
- **Result (#13):** the co-op systems add no memory growth beyond the game's own rate.
  - The bridge-only control 1664b grew 18.3 and 18.0 MB/min, and the earlier co-op soak 1316 grew 7.75 and 7.56 MB/min.
  - The game's own rate varies between runs (about 5 MB/min in control 1198).
  - Nothing degraded over two hours, so the release notes' advice stands: restart the game after several hours.

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

Updated 2026-10-20. Stage 7's exit criteria (roadmap and #17): native host, join, character-choice and recovery flows; packaging and rollback; sustained stability; online validation.

**Done (confirmed runtime unless noted):**
- **Online validation on two Steam accounts,** with the lab VM's second account (sections above, takes 1221–1225): host, invite, join, leave and reconnect. Clean-PC proofs from each published release: 0.7.0 (1315b), 0.8.0 (1670a) and 0.8.1 (1696a, up to the story step) ([chapter 98](98-release-without-game-files.md)).
- **Host and join through Steam's own UI:**
  - F7 creates a friends-only lobby and opens Steam's invite overlay.
  - The partner accepts the invite or uses *Join Game* in the friends list, from the main menu or a mission.
  - The partner then loads the host's mission natively (join-follow) and is placed beside the host when joining mid-mission (1214fr, [chapter 106](106-partner-led-story-and-mid-mission-join.md)).
  - F8 leaves.
- **Character choice:** the game's own choice at the start of *A Long Day in Dunwall*. The host takes Corvo and the partner Emily; the route harnesses steer it ([chapter 97](97-campaign-map-sequence.md)).
- **Recovery:** native shared death screens and restarts, checkpoint and autosave pairs, the mission end and level transition mirrors, and the presentation watchdog. The campaign plays through on two accounts ([chapter 97](97-campaign-map-sequence.md)).
- **Packaging and rollback:** releases 0.7.0, 0.8.0, 0.8.1, 0.8.2 and 0.8.3. Each installer builds the game-derived files from the player's own copy, and each rollback is byte-identical ([chapter 98](98-release-without-game-files.md)).
- **Sustained stability:** the 2-hour co-op soaks 1316 and 1698a (the release binary) and their bridge-only control 1664b (above). The co-op systems add no memory growth beyond the game's own rate (#13).

**Found and fixed since (release 0.8.2, candidates 1214ga-1214gf; chapter 106):**
- The clean-PC sessions 1696a and 1699a stopped at the story step because the VM joined from its title screen with no signed-in user (#146). The harness now presses a key at the title, as a player does, and the release notes tell partners to do the same. In 1705a the story then reached the VM.
- 1705a then found that on the campaign first map (the guard map) a product partner checkpoint sync could never arm, so deaths were not shared there (#148). 1214gb plays the guard map as a mission in product and online sessions. Two-account take campaign-dunwall-die-vm-v1707a: a host death and a partner death on that map each restarted both worlds.
- The two-account mid-mission join, leave and rejoin passed (campaign-join-vm-v1703a, #135). 1214ga places a rejoining partner beside the host again.
- The product controller counts every world and restarts its repair backoff there (1214ga, #145).

- The clean-PC session from the v0.8.2 download passed with the host starting a fresh campaign: story, a shared death on the first map, leave and rollback (product-two-account-v1709a, chapter 98).
- The two-account rejoin places the partner beside the host (campaign-join-vm-v1714b, #135).
- Release 0.8.3 (#152; partner Mesmerize and Doppelganger shades on the host's NPCs, #131):
  - install test release-install-v1740a: 8/8;
  - clean-PC proof from the v0.8.3 download: product-two-account-v1740a.
- A partner who joins from the title screen now waits there until a key is pressed (#146, 1214ha, chapter 106; campaign-titlejoin-vm-v1744a).
- Release 0.8.4 (#162; the partner's assassinated story target stays and can be searched, #140, chapter 106; the
  title-screen join, #146):
  - candidate 1214hm (1214hl with version resources at 0.8.4), setup tool v1754;
  - install test release-install-v1754b: 8/8;
  - clean-PC proof from the v0.8.4 download: product-two-account-v1754b.
- Release 0.9.0 (#171; the Co-op menu entry and co-op saves #156, Leave co-op #157, the character choice and the
  four pairings #158/#159, a Corvo partner's powers on the host's world #160/#168, validated in #161, chapters 107-109):
  - candidate 1214ik (1214ij with version resources at 0.9.0), setup tool v1755, builder build_release_v1755.py;
  - install test release-install-v1755b: 8/8, on the package rebuilt with the tutorial notes (release-install-v1755a:
    8/8 on the first build, kept locally as d2coop-v0.9.0-r1);
  - on two Steam accounts with the release binary: Leave co-op (coop-leave-v1755a) and a co-op save made and loaded
    from the menu (coop-hostsave-v1755a);
  - every mission in the release pairing, host Corvo and VM partner Emily, one take each: campaign-dunwall-vm-v1755a,
    -eotw-, -institute-, -madinventor-, -curator- (v1755b; v1755a: the audiograph use did not register), -dust-, -bunker-,
    -palace- and -empress-vm-v1755a;
  - Memories of Training (the tutorial; not one of the nine missions), host Emily and VM partner Emily: not yet
    passing on 0.9.0 (#174). The last full two-account passes are 1701b (1214fz) and 1713a (1214gf).
    - The key theft failed on 0.9.0 in training-vm-v1756a/b and the host-alone take campaign-training-solo-v1758a
      (0 of 29 tries) but passed on 0.8.4 at the seventh try (campaign-training-solo-v1759a). The cause was the route
      action, not the mod (confirmed runtime):
      - Pickpocketing works on 0.9.0. In coverage-v1760b both players stole the guard's key; that take fails only on
        Record-Coverage-v1638's loot-state check, which 1398 skipped for pickpocket.
      - The difficulty is the same: the active preset in the 0.9.0 tutorial reads `difficulty/preset/easy`
        (game+0x1f7b90, preset at +0x10).
      - The old action pressed X six times blind from 0.7 m behind the walking trainer. About one try in five worked
        on older builds. The new action `pickpocket_focus` walks after him reading the use focus and presses X while
        the focus is on him. Alone, the host stole the key on the second try (campaign-training-focus-v1760a, which
        ends without a join by design).
    - With the partner present, the stealth lessons restart again and again (campaign-training-vm-v1760a/b, confirmed
      runtime):
      - the trainer's attention list holds the partner's avatar just before a restart;
      - a failure in either game restarts both, and every restart puts both players back at the lesson start, in his
        sight;
      - the choke lesson restarted the same way in the passing 0.8.x takes 1701b and 1713a.
      Hiding the partner on the roof before each try (Hide-RoutePartner-v1760, routes/campaign-training-vm-v1760b)
      was not enough. In v1760c a restart began in her game, where the host's avatar waited at the lesson start. The
      theft passed at the eighth try in v1760c and failed all ten in v1760d.
    - v1760c then swung 22 times at the ceiling in the sword lesson: the crossbow lesson had left the view 33 degrees
      up, and `setviewpos` keeps the pitch. The route action `face` now levels a view more than 10 degrees off.
    - A partner joining mid-tutorial:
      - v1760e: Route-MissionVM's -JoinAfterStep counts step tags, so the join came right after the choke lesson. The
        host sat on the next lesson's tutorial page (logic 12) and publishes its mission only in gameplay, so the
        partner waited until the timeout.
      - v1760f stopped at its start: the host lab instance was not yet in gameplay after v1760e's restore.
      - v1760g: the partner joined after the crossbow use, loaded the tutorial in 14 s and was placed 1.2 m from the
        host with equal gameflow. The co-op systems never armed: both sides stayed `waiting_shared_gameplay`.
    - The release notes tell players to start the tutorial together, and to keep the other player out of the
      trainer's sight during the stealth lessons. Both findings are #174.
  - Clean-PC proof from the v0.9.0 download, product-two-account-v1755a: download, install, join, walk and story
    passed, but the host's death was not shared. The VM never showed GameOver: the host's checkpoint sync sent 157
    hellos and received none (`death_peer_unavailable`, peer arm 0). Release 0.9.1 fixes it.
- Release 0.9.1 (#171): shared deaths and checkpoints in a real co-op session (confirmed runtime and static).
  - Cause: the installed partner's checkpoint sync never armed, for two reasons.
    - The mission autosave was read beside the checkpoint directory, which names the player's own
      `base\savegame\SLOT82`. Since 0.9.0 a co-op session saves to `base\savegame_coop` (#156), so the details were
      missing or stale.
    - Only autosaves (slot type 3) were accepted. On the campaign's first map a new game first writes the native
      mission-start save (slot type 2, slot campaign x 30 + 21, FUN_140464e40, confirmed static), and a fresh co-op
      save folder holds nothing else yet. The 0.8.x proofs armed only because the VM's own saves still held an older
      tower autosave.
    - The lab never saw either: lab partners neither switch to co-op saves nor start from an empty save folder.
  - Fix:
    - 1214il reads the autosave under the game's current save root (`missionSaveFolder`, checkpoint_scope.h, with unit
      tests).
    - 1214im also accepts the mission-start save, and reports `nativeAutosaveSlot` and `saveRoot` in the checkpoint
      status.
    - Proof with 1214il's package: product-two-account-v1761a still failed (VM `enable_refused` 205 times, only
      SLOT21 of type 2 in savegame_coop).
    - With 1214im's package: product-two-account-v1761b passed. The VM armed on savegame_coop slot 21 and the host on
      slot 61. The host's death showed GameOver in both games, and both restarted on the same map.
  - Candidate 1214im (version resources 0.9.1), setup tool v1761 (byte-identical for 1214il and 1214im), builder
    build_release_v1761.py.
  - Install test release-install-v1761a: 8/8.
  - Proof harness Test-ProductTwoAccount-v1761 records both checkpoint syncs before the death and waits up to 30 s
    for both to arm.
  - Clean-PC proof from the v0.9.1 download, product-two-account-v1761d: pass.
    - The VM downloaded the release (SHA-256 591FA52A..., the published zip) and installed it with Install.cmd alone.
    - Two Steam accounts joined, walked and passed the story step.
    - Both checkpoint syncs armed (host slot 61 in savegame, VM slot 21 in savegame_coop). The host's death showed
      GameOver in both games, and both restarted on the same map.
    - The uninstall restored the game.
    - v1761c had stopped at its first check: the lab host was still on the pause menu after v1761b's restore.
  - Validating the missions on co-op saves, as installed players play them, is #175.

- Release 0.9.2 (#213): smooth frame pacing (#195), shared pickups again and the resumed-host join (#189), pickups
  both ways (#203) and the end of the tutorial (#174).
  - Candidate 1214ka (version resources 0.9.2), setup tool v1888, builder build_release_v1888.py. Package SHA-256
    634F52E1..., 13 files.
  - Install test release-install-v1888a: 8/8.
  - Clean-PC proof product-two-account-v1888a failed: "VM product mode did not report (no product status)". With
    1214jz's diagnostics off, an installed game wrote only its main status file, and the proof reads the product
    status.
- Release 0.9.3 (#213): the partner plays the host's difficulty (#177, chapter 109), and an installed game writes its
  product status set again.
  - Candidate 1214kb (version resources 0.9.3), setup tool v1889, builder build_release_v1889.py. Package
    `d2coop-v0.9.3.zip`, SHA-256 F60D70B7..., 13 files, no game files.
  - Install test: release-install-v1889b, 8/8. Release-install-v1889a failed before testing: the package argument was
    the zip, not the folder.
  - The nine missions on two accounts with this binary pass (chapter 109, "The campaign on 0.9.3").
  - Proof harness Test-ProductTwoAccount-v1891 stops the lab's idle local partner instance during the session and
    relaunches the lab pair in its restoration.
  - Clean-PC proof from the v0.9.3 download, product-two-account-v1891a: pass.
    - The VM downloaded the published zip (SHA-256 F60D70B7..., 6.7 s) and installed it with Install.cmd alone:
      receipt, DLL and the five built files. Defender's real-time protection was on, with no detections.
    - Product mode reported, and the VM passed its title screen.
    - The two accounts connected on the same mission, with the movement banks active on both.
    - Each player walked about 8 m and the other saw it, and the story trigger started native melee.
    - The host's death showed GameOver in both games, and both restarted on the same map.
    - F8 left the session, and the uninstall restored the game (the executable and master.index hashes).
    - The lab pair was relaunched and connected, and no SteamID was recorded.
- Release 0.9.4: the partner's combat choke on a host-owned enemy (#209), and story characters that agree between
  worlds (1214ke).
  - Candidate 1214kf (version resources 0.9.4), setup tool v1906, builder build_release_v1906.py. Package
    `d2coop-v0.9.4.zip`, SHA-256 5250E17E..., 13 files, no game files.
  - The builder now checks that the release notes name only this version's zip. The final audit found the download step
    still naming `d2coop-v0.9.1.zip` in 0.9.2 and 0.9.3.
  - Install test: release-install-v1906a, 8/8.
  - The nine missions on two accounts with this binary pass on the first attempt (chapter 109, "The campaign on
    0.9.4").
  - Clean-PC proof from the v0.9.4 download, product-two-account-v1906a: pass.
    - The VM downloaded the published zip (SHA-256 5250E17E..., 2 s) and installed it with Install.cmd alone in 64 s.
    - Product mode reported. The two accounts connected on the same mission, with the movement banks active on both.
    - Each player walked about 8 m and the other saw it. A story trigger advanced both worlds' objectives equally,
      with native melee active.
    - The host's death showed GameOver in both games, and both restarted on the same map with new world epochs.
    - F8 left the session, and the uninstall restored the game (13 added files removed, the original master.index
      restored).
    - The lab pair was relaunched and connected, and no SteamID was recorded.
- Release 0.9.5 (2026-10-25): hosting by itself ([#222](https://github.com/adamkozel/Dishonored-2-coop/issues/222)), a closed or crashed game ([#221](https://github.com/adamkozel/Dishonored-2-coop/issues/221)), every enemy
  attack on the partner ([#219](https://github.com/adamkozel/Dishonored-2-coop/issues/219)), the same decisions in both games ([#220](https://github.com/adamkozel/Dishonored-2-coop/issues/220)), and no lost autosaves
  ([#225](https://github.com/adamkozel/Dishonored-2-coop/issues/225)).
  - Candidate 1214km (version resources 0.9.5), setup tool v1916, builder build_release_v1916.py. Package
    `d2coop-v0.9.5.zip`, SHA-256 2898E07B..., 13 files, no game files.
  - Install test: [release-install-v1916a](../../work/release/release-install-v1916a), 8/8.
  - The nine missions on two accounts with this binary pass (chapter 109, "The campaign on 0.9.5").
  - Published: [v0.9.5](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.9.5) from commit abd75a5bf (the
    tree squash-merged to main as 8c9c02575).
  - Clean-PC proof from the v0.9.5 download, [product-two-account-v1916a](../../work/release/product-two-account-v1916a): pass.
    - The VM downloaded the published zip (SHA-256 2898E07B..., 2.3 s) and installed it with Install.cmd alone in 74 s.
    - Product mode reported, and the host hosted its lobby by itself. The two accounts connected on the same mission,
      with the movement banks active on both.
    - Each player walked about 8 m and the other saw it. A story trigger advanced both worlds' objectives equally,
      with native melee active.
    - The host's death showed GameOver in both games, and both restarted on the same map with new world epochs.
    - F8 left the session, and the uninstall restored the game (13 added files removed, the original master.index
      restored).
    - The lab pair was relaunched and connected, and no SteamID was recorded.

**Still open:**
- Needs you: the human playtest with a friend (#99), and the Defender false positive (#25).
