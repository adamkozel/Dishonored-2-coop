# Co-op session mode: design (own solution)

Started 14 October 2026. This chapter describes how the verified lab subsystems become a playable session that players start with native Steam and native game flows, without lab consoles or scripts. It is the project's own design; no third-party co-op mod is consulted.

## Current state (confirmed static)

- **Transport.** `initializeSteam` (`engine_bridge.cpp`) obtains `SteamNetworkingSockets012` through the running Steam client: `SteamClient017` → `GetISteamGenericInterface` with the game's own user/pipe.
  - `pumpConnections` uses **loopback IP 127.0.0.1:31426**. Role 0 listens; role 1 connects.
  - Role, status/command files, input file and the checkpoint directory come from `D2COOP_*` environment variables set by `Start-D2Lab.ps1`.
- **Subsystem activation.** Most gameplay subsystems (NPC stream, NPC attack/combat, door/coin/usable sync, avatar perception, shadow stream, movement bank, …) start through allowlisted lab console commands. The inventory in the next section lists them.
- **Native Steam surface available.** The game's own `steam_api64.dll` (SteamClient017, SteamUser019, SteamUtils008) exports the flat `ISteamFriends` (rich presence, `ActivateGameOverlayInviteDialog`), `ISteamMatchmaking` and `ISteamUtils` entry points plus callback registration. No Steam DLL needs replacing.

## Target design

1. **Transport selection.**
   - The lab keeps loopback IP for single-PC testing.
   - A session uses `CreateListenSocketP2P` / `ConnectP2P` with `SteamNetworkingIdentity` (SteamID) through Valve's relay. Both run on the same `ISteamNetworkingSockets` interface and the same existing packet protocols.
2. **Session and invite through native Steam UI.**
   - The host creates a friends-only Steam lobby (2 members) and sets rich presence `connect` to the lobby.
   - The partner joins through the Steam friends list ("Join game") or an invite sent with the native overlay (`ActivateGameOverlayInviteDialog`).
   - The join request arrives as a native Steam callback. If the game was not running, it arrives as a `+connect_lobby` launch argument. The joiner reads the host SteamID from the lobby owner.
3. **Characters.**
   - The host's campaign uses the game's native Corvo/Emily choice. The joiner presents as the other character.
   - Roles follow lobby ownership: host = authoritative world (as role 0 in the lab), joiner = role 1.
4. **Play mode.**
   - When the connection is established and both peers report matching session/map/world epoch, the mod enables the verified gameplay subsystems in dependency order. Their existing negotiation, epochs and nonces stay authoritative.
   - Test fixtures (time-limited `*_fixture`, duplicate/probe commands) are never part of play mode.
5. **Native status.** Connection, partner-ready and partner-lost states use native in-game notification UI. No custom overlay.
6. **Recovery.**
   - Peer loss, map change and checkpoint reload reuse the existing retire/renew paths.
   - Play mode re-enables each subsystem only through its own renewal logic after both peers are ready.

## Verification constraints

- Real two-account P2P and invites need two Steam accounts on two machines; the single-PC lab cannot exercise them.
- Lab verification covers:
  - transport selection;
  - lobby creation and rich presence on one account;
  - play-mode orchestration over loopback;
  - recovery.
- Online two-account validation stays an explicit outstanding item until performed.

## Subsystem inventory (static, 14 October 2026)

- **Starts automatically, and only when the lab environment is present** (`D2COOP_LAB_ROLE`, `D2COOP_LAB_POLICY`, `D2COOP_ENGINE_BRIDGE`):
  - proxy, mutex scoping, policy and bridge;
  - native hooks (damage observer, partner filters, perception, body pose, feet, cinematic skip, melee defense and observer camera);
  - Steam networking acquisition;
  - the loopback connection;
  - player pose exchange every 33 ms.
- **Needs a command:** every gameplay subsystem.
  - Avatar spawn and follow: `coop_spawn_probe shadow_*`, `coop_probe_follow exact`.
  - Shared checkpoint and death: `coop_checkpoint_sync on`. It is limited to the Dunwall tower map and SLOT82.
  - Cinematic sync and barrier.
  - Avatar perception, NPC stream, NPC combat, NPC attack.
  - Door, usable and coin session sync.
  - Shadow stream, movement bank and resume.
  - Locomotion presentation.
- **Session-scoped renewal** already exists for the door, usable, coin, NPC stream, combat and attack subsystems (`doorSessionScope`).
- **Bring-up sequences** live only in test harnesses (for example `Test-NpcParryObserved-v1028.ps1`) and `Restore-D2Lab.ps1`. There is no profile mechanism in the DLL.
- **Peer identity:** session = `(tick<<32)|pid`, validated with world epochs. There are no Steam IDs or P2P.

**Decision (roadmap):** session mode is stage 7. It is parked as this design while stage 3 (movement completion) continues, following [the roadmap](02-roadmap.md).
