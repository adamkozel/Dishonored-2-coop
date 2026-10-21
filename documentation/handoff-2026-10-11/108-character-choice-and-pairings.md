# 108. Character choice and the four pairings (#158, #159, #160)

The host chooses Corvo or Emily once per co-op save; the partner chooses on every join. Co-op must work in all four
pairings (host first): Corvo-Emily (today's only pairing), Emily-Corvo, Corvo-Corvo and Emily-Emily. Labels follow
the handoff rules: confirmed static, confirmed runtime or hypothesis.

## How the game chooses the character (confirmed static)

**The developer `campaign` command** (registered at 0x140042b16, handler 0x140049ae0):
- It checks only the substrings "high" and "corvo".
- "corvo" sets the cvar `ark_player_override` (0x1439a5f60) to 1 and `DAT_142280a7c` to 1. Anything else, including
  no argument or "emily", sets them to 2 and 0, which is Emily.
- "high" sets `ark_chaos_override` and picks the map at mapinfo +0xb8; otherwise +0x90. The command then runs
  `wait 1` and `map %s`.
- The override is used once, at player spawn (0x1408ad9c0), and only when no save is loading.

**The native choice screen exists only in the first mission's map:**
- The map's script node `arkKisculeNodeActionPlayerCharacter` (pins Start, Corvo, Emily) puts the player in player
  state 37 (`arkPlayerStateCharacterChoice`), which switches the game logic to state 14
  (`arkGameLogicStateCharacterChoice`).
- State 14 shows two in-world flash surfaces, whose entities are named by the decl. Confirming opens the native
  message box ("Play as Corvo Attano?", logic 3).
- The script then calls `D2_Game_SwitchCharacter(game, 1 or 2)` (0x1408a7650). It writes world+0x4c0, the cvar
  `ark_speech_usecorvo` and `DAT_142280a7c`, and queues the player swap.
- State 14 is created per map from the world's decl list and needs those entities, so it cannot run on the front
  end or in other missions.

**Where the protagonist is stored:**
- In the save header (`ARKS` v0x36) at +0x18. Loading a save sets `ark_speech_usecorvo` and `DAT_142280a7c` from it.
- The slot details hold a `character` key for the save list.
- Entering the main menu resets `DAT_142280a7c` to 0, so a new game starts as Emily until the choice.

**Consequences:**
- The host's choice is already native and made once per save: a co-op New Game reaches the first mission's choice
  screen, and every co-op save records the protagonist.
- The partner has no native choice outside the first mission. Join-follow loads the host's mission with
  `campaign <map> <chaos> emily` (session_product.h).

## The partner's choice: a native message box (design)

The engine message box manager (`arkMessageBoxManagerLocal`, game+0x30f260, vtable 0x141b58018) is what the quit and
overwrite questions use (confirmed static):
- `Open` (vtable +0x18) supports only fixed button pairs.
- The AS3 request handler (0x1404630f0) builds a box with any buttons. It allocates 0x730 bytes, calls
  `FUN_140460f90(box, *(mgr+0x18), mgr, &info, callback, 0)` with info type 7 (no fixed buttons), then pushes, per
  button, a label string id (`FUN_140165c80(&id, key)` into box+0x2f8) and an action string (box+0x318).
  `FUN_140462b00(mgr, box, priority)` then queues the box and returns its handle.
- The question can be raw text: with info+4 = 0, the idStr at info+8 is shown. Button labels must be string ids.
- The callback is `{vtable, object, function}`. The manager calls `vtable[1](callback, handle, result)`. The result
  is the pressed button's index (hypothesis; the cancel value is checked at runtime).
- Opening a box switches the game logic to state 3, and the box draws in the current state's movie. That covers the
  main menu (logic 6) and gameplay.

**Plan:**
- Before join-follow loads the host's mission, the partner's game shows this box. The question is the game's own
  text if a fitting key exists; the two buttons are the localized character names.
- The answer becomes `campaign <map> <chaos> corvo|emily` and is sent to the host.
- String ids are the FNV-1a hash of the lowercased key. The table is at 0x1432c7e98: 0x58-byte entries with the id at
  +0, the key idStr at +8 and the text idStr at +0x30; the count is the low 24 bits at +8 (confirmed static).
- The keys for the names are not in the executable. A lab command lists the keys whose text contains a phrase.

## The pairings: role-keyed code (survey of outputs/d2-coop-lab/src, 2026-10-22)

Almost every avatar, bank, prop and power path picks Corvo or Emily from `localRole` (role 0 renders a remote Emily,
role 1 a remote Corvo). The readers of the local player's native state are already character-based. No packet carries
the real character: the shadow-action, arm and avatar-weapon packets carry `character`, but always as `role+1`, and
both sides check that.

| Area | Role-keyed sites |
|---|---|
| Avatar rig and spawn | `coop_spawn_probe shadow_corvo/shadow_emily` (session_product.h, checkpoint respawn); `localRole?DoppelgangerCorvo:DoppelgangerEmily` in about 15 files; `CorvoTutorial:EmilyPartner` scopes; `shadow_corvo/emily.md6` |
| Props | ranged, grenade, trap and heart props, the remote muzzle, joint limits 215/219 |
| Movement bank | product `<corvo/emily>-choke-bank.bin` by role, the lab `movement.bin`, layout hash and joints by role |
| Join-follow | `campaign %s %s emily`; `MissionFollowPacket` has no character; lobby data `role_host=corvo` |
| Character checks | `character==localRole+1` in shadow_stream.h (about 16 sites), checkpoint_sync.h (`postload_role_invalid`), avatar_weapon.h, arm_stream.h |
| Powers | Corvo's powers stream only host to partner (Blink, Possession, Swarm, Windblast, failed Bend Time); Emily's only partner to host (Far Reach, Doppelganger, Domino, Mesmerize, Shadow Walk); npc_combat.h `partnerBaffle` |

Safe already:
- the friendly-fire and damage checks accept either clone definition;
- the local player and the partner are told apart by handle, never by character;
- the story grants are character-aware.

## Plan for #159

1. **The character on the wire:** each side reads its own character from the native player (1 Corvo, 2 Emily). The
   pose packet carries it in its flags (the protocol becomes 81). `remoteCharacter` falls back to the role default
   until the first packet, which keeps today's pairing unchanged.
2. **Rendering the partner:** every `localRole?Corvo:Emily` that picks the remote avatar's rig, bank, props or effects
   uses `remoteCharacter` instead. The checks keyed on the local character use `localCharacter`.
3. **Powers:** streams are sent by the character that owns the power and presented by whichever side does not. A
   Corvo partner's powers acting on the host's characters are #160 (Possession through the host's AI, Bend Time,
   Swarm, Windblast, Blink).
4. **Same-character pairs:** the local player and partner are already told apart by handle. The partner avatar is
   the Doppelganger rig of its character, so two Corvos are still two different entity kinds.
5. **Lab fixtures for the new pairings:** the lab `campaign` command gives either character (with `corvo` or
   `emily`), so a host Emily and a partner Corvo are a fixture away. Each pairing gets its own lab checkpoints.
