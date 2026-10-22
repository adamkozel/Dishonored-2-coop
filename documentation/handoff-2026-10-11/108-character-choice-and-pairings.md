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

## Progress (runtime)

- **1214hx, the character on the wire and the partner's choice (protocol 81, pose version 12):**
  - Each game samples its player's character from the native definition and sends it in the pose flags.
  - Take character-choice-v1779a (two accounts; the host plays Edge of the World as Corvo) passes all 12 checks.
    The VM partner's box opens in its main menu, drawn by the game: "Character", with "Play as Corvo Attano" and
    "Play as Emily Kaldwin" (confirmed runtime). Result 0 picked Corvo and result 1 Emily.
  - The VM played the chosen character each time (`corvo/body.def`, then `emily/body.def`), and both games reported
    it. The box opened again on the second join.
  - The first mission is excluded: its native choice screen decides there. A native New Game whose intro was
    skipped once passed that screen and defaulted to Emily (coop-hostsave-v1775a); in v1776a the screen appeared.
- **1214hy, the partner rendered by character:** in take character-render-v1780a, a Corvo partner on a Corvo host
  got `d2coop_probe_corvo` with its movement bank active on the Corvo rig (215 joints). This was the first Corvo-Corvo
  session (confirmed runtime).
  - On the second join (as Emily) the host kept the first avatar, and the Emily bank could not bind to it. 1214hz
    removes a stale avatar with the engine's own remove event, and the arm rebuilds it.
- **Pairing Emily-Corvo (take pairing-v1781a, two accounts):** each side renders the other as the other's character.
  The host draws `d2coop_probe_corvo` (215 joints) and the VM draws `d2coop_probe_emily` (219). In the host's frame
  the partner walks along the canal as masked Corvo.
- **1214ia, the power presentation streams by character:**
  - Each stream is sent by whichever game plays the power's character and presented by the other. The validators
    accept a packet from the peer in either role.
  - The world effects of an Emily partner's powers stay with the host: the shade puppet and the Mesmerize object
    still need `localRole==0`.
  - Lab notes: the named grant equips a power only once, and a hand already holding a power refused the next one
    (`named_slot_occupied`); 1214ib switches it. A lab item grant (mana) also takes the hand.
- **Take host-powers-v1786a (1214ib, local lab; an Emily host in Edge of the World and a Corvo partner)** passes.
  The partner presents each of the host's casts:
  - the Doppelganger clone (visible);
  - the Mesmerize spirit (its native visual scene, state 3);
  - a Far Reach beam and impact;
  - the Domino stream.
- **Lab fixtures:** `work/release/Prepare-PairingFixtures-v1784.ps1` copies the other character's Dunwall checkpoint
  into each lab instance's SLOT80 (the client Corvo, the host Emily). `LoadGame 80` then gives any pairing at the
  tutorial guards; SLOT81 stays the restore fixture.
- **The shadow stream of a swapped-character lab fixture (diag-pairing-stream-v1788a/b, diag-partner-sword-v1789a):**
  - At the Dunwall fixture a Corvo partner (client `LoadGame 80`) never armed its shadow action stream
    (`unavailable_or_busy`). Its pair was ready (`pairBits` 0).
  - 1214ic records the refusal: the source failed only its sword term (`sourceValidBits` 128: the native `ply_sword`
    quantity is not 1).
  - The checkpoint holds no sword: the restore fixture gets it from the native Mayhew scene (PrepareShadowSword). A
    bare load skips that scene, so the sword quantity stays 0.
  - `work/release/Enter-PairingFixture-v1790.ps1` loads the pairing slot and plays the same scene. This is a lab
    fixture matter, not a mod one: a real mission start has its sword.

## #160: Corvo's powers used by the partner

A Corvo partner's own world holds replicas of the host's NPCs: their AI is paused, and they follow the host's NPC stream.
So her powers find nothing to act on there unless the host acts too. Each power needs its own path to the host's world.
All takes are on the local lab at the Dunwall tutorial guard 2, with the partner loading SLOT80 (Corvo) under the
product controller (`work/natural-moveset/Test-PartnerCorvoPower-*.ps1`, `Test-PartnerBendTime-*.ps1`).

- **Windblast (no new code):** her copy's `windblast_npc` hit is forwarded and applied natively on the host (the existing
  PeerNative path). partner-windblast-v1791a: the guard loses the same health (40 -> 26) and is pushed about 3.3 m in both
  worlds (confirmed runtime).
- **Devouring Swarm (1214id, `swarm_host.h`):** her rats found no prey in her replica world (partner-corvo-power-v1792a).
  - While her swarm world stream is present, the host spawns a native swarm at her swarm's origin, as the power does
    (0x8ae9b0 with `devouring_swarm.def`, the host player as parent, its team index, the AI stimulus 0x73 kind 0xb). The
    host's cosmetic copy of her rats is hidden meanwhile.
  - It ends with hers: the swarm's own flee request (0x1406a6650), and the native removal 8 s later.
  - partner-corvo-power-v1794a: the host's rats kill the host's guard (40 -> 25 -> 10 -> 0), and her copy dies in step.
    v1794c covers the whole lifecycle: the swarm flees about 3 s after hers ends and is removed 8 s later (confirmed
    runtime).
  - Gap: the corpse lies about 1.6 m from where her copy fell.
- **Possession:** a human needs the upgrade `possession_parasite` (v1794b, without it, never possessed).
  - Baseline partner-corvo-power-v1795a: her cast reaches the host as effects only (16 possession-fx packets presented),
    and the host's guard keeps its own AI: her possessed copy walks 5.6 m, and the host's guard ends 1.3 m away.
  - 1214if sends her Possession of a bound replica as a paired scene (variant 1000): PairedHold every 200 ms and her
    possessed body as PairedPose, which the host follows as a victim (the AI lease, or without it when the guard is
    alert; start range 60 m).
  - 1214ig keeps her channel alive: the native possession takes the replica's AI, and the ownership watchdog stopped
    the channel at once (partner-corvo-power-v1798a: ai_ownership_changed, 1 reverse pose sent).
  - partner-corvo-power-v1800a (1214ih): her channel survives (client_possessed), and the host accepts 191 reverse
    poses in follow mode, but never presents one (presentedRoot 0); its guard walks by its own AI 0.4-2.4 m from her
    copy. 1214ii counts each refused host victim frame: all 621 were refused by the render identity
    (`npcRenderCurrent`), which for a live NPC requires our lease's AI pause. The guard was alert (her avatar 4 m in
    front), so it failed the calm-AI lease eligibility.
  - 1214ij: her Possession forces the host's AI lease (the game suspends a possessed victim's AI whatever its state),
    and a possessed render identity passes like a corpse or a knocked-out body.
  - **Result (1214ij, confirmed runtime):** partner-corvo-power-v1802a passes. The host takes the possession hold
    (`host_possession_hold`), presents all 659 victim frames, and its guard moves 7.78 m with her copy's 7.61 m, within
    0.55 m of it (0.0 m once the walk settles).
- **Bend Time and Stop Time (1214ie-1214ig, `time_bend.h`):** her NPC copies follow the host's stream, so her own time
  request slows nothing that matters.
  - Each game samples its own running Bend Time: the player's power component at +0x378 (+0xd0 active, +0xc0 its
    request id), and that request's factor in the time manager (game+0x1f52e8: clock-3 requests at +0, 12 bytes
    `{id, factor, persist}`, the smallest last). The pose flags carry it: bit 10, the percent in bits 16-22 (pose
    version 13).
  - The host adds the same factor as its own native request, as the game's non-power users do (FUN_14080eb40, persist 0,
    never saved), with her native blends (0.5 s in, 1.0 s out). It removes the request when hers ends (FUN_14080ede0),
    and keeps it through 300 ms without her bit, because her Stop Time re-issues its request on a second press.
  - Both players keep real time through the manager's own exemption (FUN_14080d8c0, 1e30 s). The manager's update
    rebends every exempted entity while byte +0x1a0 of the engine time object (game+0x2e7428) is 0 (0x14080f18e,
    confirmed static).
  - partner-bendtime-v1796a/v1797a (1214ie): the host held her factor 0 for her whole Stop Time and released it about
    0.5 s after; the host's guard head moved 0 mm (404 mm before), and her copy followed. But the host player froze too
    (0 m walks).
  - partner-bendtime-v1798a (1214if) found why: the exemption was dropped in the first frames, while the flag was still
    0 (exemption seconds 0, clock 0); the flag was 1 by the next sample. The host's own cast keeps 1e30 on the unbent
    clock. Base Bend Time runs the world at 0.25 and the caster's clock at 0.7 (his walks 1.9/1.4 m against 2.8/2.0).
  - 1214ig exempts both players again every 100 ms while the request is held. 1214ih passes the exemption seconds as
    raw bits in R8D: FUN_14080d8c0 spills R8D and stores it (the power passes 0x7149f2ca, 1e30), so a float argument in
    XMM2 left whatever R8 held (1214ig read 1.97e15).
  - **Result (1214ih, confirmed runtime):** partner-bendtime-v1800a (Stop Time) and v1800b (base Bend Time, her factor
    25 percent) pass. The host holds her factor for her whole Bend Time (12/12, 11/11 samples) and releases it after.
    The host's guard head moves 0 mm in her Stop Time (72.8 mm before) and 126 mm in her Bend Time (282 mm before),
    and her copy follows. The host player stays on the unbent clock: his walks in her Stop Time cover 2.84/1.92 m,
    against 2.82/1.96 m before.
  - The host sees no Bend Time screen effect: that belongs to the caster (the effect mode needs the local player's own
    component active).
  - Gap (to film): after her Stop Time ends, her copy's head joint stays still while the host's guard idles (after
    base Bend Time it moves). Her camera faced the ceiling in these takes.
- **Blink:** her Blink casts (her blink effect stream sends), and her attack after it is shared:
  - partner-corvo-power-v1804a: her post-blink sword kill killed the host's guard too (40 -> 0 in both worlds).
  - Blink Assault's kick has not landed in a take yet. In v1801a-v1803a the guard walked its patrol, or her channel
    for it had not formed. In v1804a she killed rather than kicked, and the host's corpse lay 6.9 m from hers: her
    replica was stale before the strike.
- **Moved to #168 (stage 4):** Blink Assault's kick, her replicas' skeleton after her Stop Time ends, the upgrades
  (Greater Windblast, Double Swarm, the possession chains) and a framed visual review of each power.

### #168: Blink Assault, the upgrades and her replicas after Stop Time

Harness `work/natural-moveset/Test-PartnerCorvoPower-v1807.ps1` (`-Upgrade` grants a native upgrade after the power):
for Blink Assault, Windblast and Swarm it waits for the guard's patrol pause, reads its position and facing from the host
(authoritative), requires her replica to be ready within 0.3 m of it, and frames the host's camera 4.5 m to its side.

- **Her replicas after her Stop Time (resolved, not a bug):** partner-bendtime-v1801a frames her copy of the guard
  (her camera aimed twice: the first aim after the fixture leaves her view on the ceiling). Her film shows the guard
  attacking her, frozen through her Stop Time, then moving and attacking again; her copy's head moves 704 mm after it
  (the host's 605 mm). The still head in partner-bendtime-v1800a was an idle guard read by the probe.
- **Greater Windblast and Shockwave (confirmed runtime):** her hit carries the upgrade's native damage to the host's
  guard. partner-corvo-power-v1807a (`windblast_greater`): 40 -> 22.3 in both worlds, pushed 6.1 m together; v1807c
  (`windblast_shockwave`): 40 -> 31, pushed 7.5 m.
- **Double Swarm (confirmed runtime):** partner-corvo-power-v1806b (`devouring_swarm_double`): the host's swarm kills
  the host's guard, 40 -> 0 in both worlds.
- **Known limitations:**
  - **Blink Assault's kick staggers rather than knocks down.** partner-corvo-power-v1805a: her kick lands
    (`blink_attack.damage`), is forwarded as a PeerNative request and applied natively on the host, and both worlds'
    guard staggers together at full health. A host's own kick knocks the guard down, because its kick scene binds the
    NPC (1214ct); the damage alone does not reproduce that.
  - **The host's swarm is always the base swarm.** The host spawns `devouring_swarm.def` for her swarm, so the size
    upgrades (`devouring_swarm_bigger`) act only in her world. The Bigger Swarm cast never fired at the lab fixture
    (v1806c, v1807b).
  - **Corpse and rat possession are not shared.** Her Possession of a human is (partner-corvo-power-v1802a). Rats
    and fish are not streamed NPCs, and `possession_decayed_harbor` (corpses) is untested.
