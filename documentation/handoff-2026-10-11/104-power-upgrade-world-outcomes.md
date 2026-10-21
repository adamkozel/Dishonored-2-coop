# 104. Power upgrades with world outcomes (#117)

Some power upgrades change the world rather than the caster's body. The movement checklist already covers what the
partner sees of the caster (`power_upgrade_variants`, chapter 99). This chapter decides, for each such outcome, whether
it is shared between the two worlds or confined to the caster's world, and why. The general rule:
- Corvo is the host, so his outcomes on NPCs happen in the authoritative world, and Emily's copies follow through NPC
  authority.
- Emily's outcomes happen in her world, on host-streamed copies, and need a path back to the host.

The live candidate is 1214fp.

## Time factor (shared for NPCs; players keep real time)

- **Stop Time:** coverage-v1637a and v1638a (Record-Coverage 'stoptime'; confirmed runtime):
  - Corvo casts Bend Time with Stop Time (`bendtime_stoptime`) in the lab. Native Bend Time runs in his world:
    desaturated view, hourglass, "End Bend Time".
  - The tutorial guard's head joint is exactly constant in both worlds while time is stopped, and the two worlds agree
    within 0.1 mm. The host's NPCs freeze, and Emily's copies follow the host's NPC stream.
  - Emily's world has no time effect, and she keeps walking (1.23 m in 2 s, 1637a). Corvo sees her move.
- **Decision:**
  - Corvo's time outcomes (`bendtime_stoptime`, `bendtime_relativity`, `bendtime_extended_duration`, `blink_bendtime`,
    and BriskEye's Snap Reaction when he has it) are shared for NPCs through NPC authority.
  - Players are never time-scaled: Emily, like a caster, keeps real time in both worlds.
- **Emily's own world-time effects:** `grapple_greater_far_reach`'s slow time while aiming, and Snap Reaction when she
  has it.
  - These stay in her world, because NPC time belongs to the host: her copies keep the host's pace.
  - This is a hypothesis, not taken.

## NPC outcomes

- **Shadow Kill** (`shadow_kill`; `shadow_kill_greater` adds aware NPCs):
  - **Without a fix:** the victim turned to ash only in the killer's world (carry-body-v1640a: Corvo's guard vanished
    from his world and stayed dead and visible in Emily's; 1640b the reverse).
  - **Native ash (confirmed static):** the NPC's `arkCpntAIPhysicsReaction` plays
    `midnight/gameplay/human/common/powers/shadow_killed.midnightscene` and fifteen joint effects
    (`models/effects/gameplay/shadow_kill/sk_*.fx`), then removes the body. The docks photographer, a civilian, did not
    turn to ash (1639a/b); the lab guard did.
  - **1214fk/fl** ([npc_authority_stream.h](../../outputs/d2-coop-lab/src/npc_authority_stream.h),
    [npc_authority_protocol.h](../../outputs/d2-coop-lab/src/npc_authority_protocol.h)):
    - A dead NPC that vanishes within 8 s of its death sends Dead marked ASH1 (`kNpcDeadAshed`).
    - The peer hides its copy (native hide `0x8784b0`) and plays four of the native ash effects at the body (native FX
      kinds 31-34).
    - 1214fk's message never left: Dead accepted no reserved value and was host-only (1642a/b). 1214fl accepts ASH1 on
      Dead from either role, and the unit tests cover both directions.
  - **Result (confirmed runtime):**
    - 1643a: Corvo's Shadow Kill sends ASH1, and Emily's copy is hidden with four ash effects.
    - 1643b: the reverse.
    - The films do not frame the guard (dark lab room), so the visual is still to review.
- **Corvo's NPC outcomes** (`windblast_greater` kills, Shadow Kill, Bloody Swarm): host-world outcomes. Deaths reach Emily
  through shared combat authority (chapter 95) and the ash through ASH1.
  - Bloody Swarm (`shadow_kill_bloody_swarm`) also spawns a bloodfly swarm (`bloodflyswarm_shadowkill.def`), which
    exists only in the killer's world (hypothesis).
- **Domino** (Emily; base linked outcomes, chapter 30): shared in both worlds through Linked requests (1214fm-fp).
  - **Fixture:** Test-CarryBody v1644-v1651 `-DominoLink -HoldAi`.
    - 1645a cast while the movement fixture still held Emily's input, so no mark was made.
    - From v1644 the casts run after arming, from behind each guard, with every step recorded and filmed.
    - The tutorial guards wander and noticed Emily in 1647a-1651b (fixture failures, kept). From v1649, `-HoldAi`
      holds the host NPCs' AI (lab-only `coop_npc_combat fixture_ai40000`).
  - **Native link (confirmed runtime):** each cast enters `UsePhantasmalLink`. Corvo's world presents both marks, and
    Emily's film shows the native link beam between the guards (1646a).
  - **Without a fix** (1648a, 1214fl): Emily's assassination of guard 2 was shared, but linked guard 1 lived at full
    health in both worlds.
    - About 1 s after guard 2, the native network repeated the hit on guard 1's copy with bit 40 of the damage flags
      set (`0x10000000000`).
    - No replay matched it, so the client refused it as `hit_damage_parameters` and suppressed it. 1647a's fatality
      shows the same.
  - **1214fm:** the client sends a player-owned hit carrying the link flag as a `Linked` combat request. The request
    carries the declaration, the native amount and the native flags without the link bit. The host applies it
    natively with the partner avatar as attacker, up to 96 m away, and HostWorld and Dead share the outcome.
  - **1214fn:** Domino's linked knockout is sleep damage, which passed through the client natively.
    - Emily's copy was knocked out locally, its channel ended (`ai_ownership_changed`), and only the Life roster carried
      the knockout to the host. The bodies ended up to 4.5 m apart (1649b, 1653b, 1655a).
    - A linked sleep hit on the client is now captured and sent as a Linked request.
  - **1214fo:** a peer's Request or Commit under matching nonces marks the combat channel ready.
    - After a re-arm, the client's Ready could reach the host before its Hello and be refused. The client was then
      ready and the host was not.
    - In 1654a the host dropped the linked knockout request as `receive_not_ready`, 30 ms before the next Hello round.
  - **1214fp:** a knockout sets the owner suppression flag, and the native maximum drops from 40 to the suppressed 10.
    - The host then committed a HostWorld loss from a baseline above the new maximum. The commit failed validation and
      stopped the host's whole combat session after every choke (`commit_queue_or_validation`; 1652b, 1654a, 1655a).
    - The host now rebases instead (`hostWorldRebased`). This is a general fix: any choke used to re-arm combat.
  - **Result (confirmed runtime):**
    - Lethal: guard 1 dies in both worlds with the same body position (1652a, 1653a, 1654b, 1655b, 1657b).
    - Choke (1657a, 1214fp): guard 1 is unconscious in both worlds at the same position.
      - The host's combat session stays up: 2 rebases and no re-arm.
      - The Linked request is applied, the channel is kept, and the unconscious body streams to Emily (389 poses).
  - **Not covered:** `domino_death_link` and links of three or four NPCs (hypothesis: the same Linked path). The death
    of a linked NPC caused by Corvo is applied to Emily's copy through Dead, outside the native network, so in her world
    it is not repeated on the other linked NPCs (hypothesis, not taken).
- **Rat Shadow** (`shadow_walk_rat_shadow`; takedown-variants-v1656a, Test-TakedownVariants-v1655 `-GrantNamed`):
  - Emily's Shadow Walk kill (`assassination_sw_back_short`) is shared. The guard is dead in both worlds, 0 m apart.
  - The corpse stays visible in both worlds, and no rats appear in either.
  - The host orders the kill: the client's own native kill is suppressed, and the death is applied by the host's
    commit. So the upgrade's native corpse-to-rats outcome never runs.
  - **Decision:** known limitation for the release. The body stays consistent in both worlds and can be hidden by hand.
    Making the rats would mean triggering the native rat outcome on the host for the partner's kill (#131).
- **Mesmerize in combat** (`mesmerize_incombat`; carry-body-v1658a, Test-CarryBody-v1652 `-CastPower mesmerize`):
  - The native power takes a target in Emily's world: guard 1's handle in the target list. The aim came from the
    floor below and missed guard 2.
  - Guard 2 kept walking in step in both worlds: 14.5 m in 10 s, 0.06 m apart.
  - NPC AI runs on the host, and the host has only the cosmetic Mesmerize model (chapter 25), so no NPC can be
    mesmerized in either world (confirmed static).
- **Deadly Shade and Baffling Shade** (carry-body-v1658b, `-CastPower doppelganger`):
  - The host's NPCs do not perceive Emily's shade. Guard 2 is unaffected and in step in both worlds (0.001 m apart).
  - In Emily's world, one `kill_damage` event that was not player-owned was suppressed as a replica-world event, so
    it changed nothing.
- **Decision for Mesmerize, the shades and Rat Shadow:** confined to Emily's world as presentation (the spirit, the
  shade, the target effects); the outcome on NPCs is missing but never diverges.
  - The reason: these powers act through native world objects (`arkCpntMesmerize` sends `arkMsgTryMesmerizeNpc` and
    `arkMsgUnmesmerizeNpc`; the shade is a perceived entity). Those objects exist only in her world, while the AI
    that would react runs on the host.
  - Sharing them means creating the native object on the host for the partner's cast: #131 (stage 4, Todo). Until
    then they are known limitations for the release notes.
  - The takedowns in 1658a/b failed because the guard had walked off. Kept as evidence.

## Partner world-object powers on the host (#131)

- **Mesmerize: a native spirit in the host world** (lab, Test-MesmerizeHost v1716-v1721, candidates 1214gh-gm):
  - The host spawns the original object (`models/powers/mesmerize/mesmerize_object.def`), finds its
    `arkCpntMesmerize` on the following frames (1716a: not attached on the spawn frame) and configures it as
    `FUN_14072bf30` does.
  - The anchor must resolve. The world handle `0x3fff0010` does not (1718a), so a static spawner is used (1719a).
  - State 1 waits on its appear scene, which never ends on the host (1719a, 1720a, 1722a). State 3 is requested
    directly (1214gl).
  - State 3 crashed in the NPC AI (`FUN_14040ccf0`, 1721a). The native code finds the spirit through the local
    player's Mesmerize slot (`player+0x390`, then `+0xd4`; confirmed static), and Corvo has none. The host sets a
    phantom slot while its object runs (1214gm).
  - mesmerize-host-v1722b (confirmed runtime): state 3 with no crash, the timer counts down, the two tutorial guards
    are native targets, and the stop goes to state 2.
- **Following Emily's own cast** (1214gn; partner-mesmerize-v1723a, Test-PartnerMesmerize-v1723; confirmed runtime):
  - The host runs its object for her cast. Her world stream places it at her spirit's transform and anchors it to the
    spawner of the nearest NPC. The host requests state 1, then state 3 once hers is active, and stops it at her
    retire.
  - Both spirits took the same NPC (190252915, on the lower floor) in 43 of 44 active host samples. Her stand 5 m
    behind guard 2 was on the floor below, so guard 2 was taken in neither world.
  - **Failure:** after the stop, the host object stayed in state 2 for 25 s with the phantom slot set.
    - The state 2 exit scene never ends on the host either (confirmed runtime).
    - Natively, its end posts a message (type `1439a6f30`, also posted by entity-removal paths), so the native power
      spawns a new object per cast (hypothesis).
    - 1214go returned the cached object to the idle state 0, 1.5 s after it entered state 2. State 0's enter only sets
      bit 0x10 of the owner's `+0xd4` (confirmed static).
- **partner-mesmerize-v1724a** (1214go, Test-PartnerMesmerize-v1724; fail, confirmed runtime):
  - The host camera was held at her spirit. Same target set in both worlds in 46 of 51 samples. The taken NPC
    (190252915, `ld_spawner_guard_tutorial_1` on the lower floor) ends 0 m apart in the two worlds.
  - **Host crash** 1.5 s after the stop, in state 0's enter (`7290b4`, writing through the component's owner `+0x10`,
    already null). The object's component list read empty afterward.
  - **Cause:** the object is removed natively in state 2 (confirmed runtime). State 2's enter `FUN_140728f50` posts
    the message at once when its exit scene does not start (scene id 0), and its update `FUN_14072f370` posts it when
    the scene ends. 1723a's 25 s in state 2 were stale memory.
  - 1214gp: the host drops its cached object once the component's owner or the handle no longer match it, clears the
    phantom slot and spawns a new object per cast, as the native power does.
- **partner-mesmerize-v1725a** (1214gp, Test-PartnerMesmerize-v1725; pass, confirmed runtime):
  - The host followed her cast: begun, state 3, then stopped. Its object was in state 3 for 47 samples, with a host
    target in 46 (up to 2).
  - Guard 2 (189269860) was taken in 33 samples. It walked 13.3 m from the upper floor down to her spirit, in step in
    both worlds (13.34 and 13.38 m), and ends 0.07 m apart. The lower-floor guard 1 (190252915) ends 0 m apart.
  - Same target set in both worlds in 42 of 47 samples.
  - After her spirit, the object was removed natively (`object_removed`) and the phantom slot cleared. Fixture
    restored.
- **Control and repeat** (1214gq: the lab-only switch `coop_mesmerize_native follow_off|follow_on`; a product build
  always follows; Test-PartnerMesmerize-v1726; both pass, confirmed runtime):
  - **partner-mesmerize-v1726a** (control, host object off): her spirit took guard 1 (52 samples) and guard 2 (25).
    In the host world they kept walking, 16.2 m and 7.1 m, and her copies followed (0 m apart at the end). The host
    took no NPC.
  - **partner-mesmerize-v1726b** (followed): her spirit and the host object took guard 1, the same set in 51 of 52
    samples. Guard 1 moved 0.55 m while taken, and 0 m apart at the end. The object was removed natively, the
    phantom slot cleared and the fixture restored.
  - So the stop and the walk to the spirit come from the host object: the host's AI mesmerizes its NPCs natively, and
    NPC authority carries the result to Emily.
  - **Preliminary visual review** (1726b frames at 10, 16 and 22 s): in Corvo's view, the guard stands facing the
    spirit with the native link effect on its head, as in single player. Visual acceptance is recorded separately.
- **Mesmerize status:** done for #131 in the lab (both roles: Emily casts, Corvo's world reacts). Corvo has no
  Mesmerize. Next: the shades and Rat Shadow, then a product two-account check.
- **Doppelganger shade: a native puppet in the host world** (1214gr, `native_shade_host.h`, Test-PartnerShade-v1727):
  - Her shade is the native clone `models/powers/doppelganger/decoy_entity.def`: an `arkNPC` in the player faction,
    with the decoy AI, attention source and health.
  - The native cast `FUN_14043c950` creates it through `CreateClone` (`FUN_14043cdc0`: `FUN_1408ae9b0(game,
    definition, aim+0x94, the caster's orientation turned by pi)`). `OnCloneSpawned` (`FUN_14043e0c0`) then posts an AI
    stimulus (type 0x73, kind 5, the clone position) (confirmed static).
  - The host spawns the same definition at her streamed shade in the same way and posts the same stimulus. It keeps
    the puppet at her shade (a native transform when it drifts more than 1 m) and removes it with the native removal
    message when hers ends.
  - **partner-shade-v1727a** (fail, confirmed runtime): the host crashed right after the puppet appeared, at `3a9fd8`
    reading `0xb8`.
    - The cause: a player component update (`FUN_1403a9f90`; its owner cast to the player by `FUN_1408f2e90`) lists
      the player's clones through `*(player+0x398)` (the Doppelganger power), its handle array `+0xb8` and count
      `+0xc0`, with no null check (confirmed static). Corvo has no Doppelganger.
    - partner-shade-v1727b (the control) found the host already gone. It is kept as an invalid take.
  - **Next (1214gs):** while puppets exist, the host player gets a phantom power block in that slot, as the Mesmerize
    phantom slot does. Its `+0xb8` is the live puppets' handle array, `+0xc0` their count and `+0xd4` the last one. It is
    kept 5 s after the last puppet.
  - **partner-shade-v1727c** (1214gs; recorded as a pass, but its perception sign is weak; confirmed runtime):
    - No crash. The puppet spawned at her shade, followed it and was removed natively when hers ended.
    - Her shade runs: the native decoy fled 25 m in 7 s and was gone after about 7 s, while the puppet stood. The 1 m
      pin teleported it 62 times. Each native teleport refreshes observable membership.
    - Neither shade's health fell (1 of 1). Host guard 1 left its post and walked to where the shade had passed (an
      investigation, hypothesis), and came within 3 m of the puppet in one sample.
  - **partner-shade-v1727d** (control, puppet off): no puppet in any sample. Guard 1 kept its patrol and never went
    toward the shade's path. The harness failed it on the cumulative spawn counter (1 from 1727c), so it is recorded as
    a fail and read as a valid control.
  - **Next (1214gt, Test-PartnerShade-v1728):**
    - The puppet follows her shade every frame, inside the partner avatar's continuous-transform scope (the observable
      refresh keeps its membership). Jumps over 3 m are teleports.
    - The shade is cast 4 m in front of guard 1.
    - Both guards' native attention (`--ai-targets`) is sampled, and the pass needs the puppet there.
    - The counters are taken as deltas.
  - **partner-shade-v1728a** (1214gt; pass, confirmed runtime):
    - The puppet followed her running shade with 326 continuous moves and no teleport, a maximum drift of 0.42 m,
      and was removed natively when hers ended.
    - Both host guards perceived the puppet 1.5 s after it appeared (it was in their native attention), and from
      4.8 s both had it as their combat target. Then they also turned on Emily's avatar (209916411): the shade alerted
      them, as in single player.
    - Neither shade was hit: hers fled and was gone after about 10 s.
  - **partner-shade-v1728b** (control, puppet off; pass): no puppet, and the host guards' attention stayed empty for
    15 s. Guard 1 then noticed Emily herself.
  - So the host's NPCs now perceive and fight her base shade natively.
- **The shade upgrades** (1214gt, Test-PartnerShade-v1729; confirmed runtime):
  - **partner-shade-v1729b** (Baffling Shade; pass):
    - Both guards perceived the puppet, and a host guard killed it (health 1 to 0; `puppet_lost`).
    - Her shade in her world kept its health and lived on, so the death diverges between worlds.
    - Baffling Shade has no code strings; it is data-driven (hypothesis: the decoy's death outcome reads the caster's
      attribute, which Corvo does not have).
  - **partner-shade-v1729a** (Deadly Shade; fail): her shade never reached the stream (595 unknown reads at reader
    stage 3).
    - The film shows the upgrade's shade: it appears, walks to the guard and engages it.
    - Its entity is `models/powers/doppelganger/deadlyshadow_entity.def`, with `arkCpntDeadlyShadow`, a combat NPC.
      The reader accepts only `decoy_entity.def` with the decoy component.
    - So with Deadly Shade, Corvo sees no shade and no host NPC is touched. Guard health stayed 40 in both worlds.
- **The deadly shadow streamed** (1214gu: the reader accepts `deadlyshadow_entity.def` by its actor; the Doppelganger
  world packet v6 carries a shade kind; the host puppet takes the same definition):
  - **partner-shade-v1729c** (Deadly Shade; fail, confirmed runtime): her deadly shadow was streamed (kind 2, health
    40). The host crashed in the host-spawned deadly shadow's AI (`4a404e`): `FUN_1404a3f90` reads a component
    (`1439e8250`) the free-standing spawn lacks.
  - Natively, `OnCloneSpawned` replaces the clone through `FUN_14043de90` (a lookup at the caster's position). It
    spawns that result's clone definition with the result as parent, and removes the decoy (confirmed static). The
    deadly shadow is therefore tied to a target NPC (hypothesis).
  - In her world, the replica guard 1 read 0 health by the end (the host had crashed, so this is unexplained).
  - The base-shade regression take was skipped (host down).
  - **Next (1214gv):** no host puppet for the deadly shadow; Corvo sees it through the presentation. Her deadly
    shadow's hits are to be forwarded.
- **The shade death and the deadly shadow** (1214gv, Test-PartnerShade-v1730; both pass, confirmed runtime):
  - 1214gv:
    - The host records hits on a puppet's health component (read-only, in the native damage observer).
    - When the puppet's health reaches 0, or it is lost within 2 s of a hit, the host sends the kill
      (`CoopShadeKillPacket`).
    - Her game applies the same native hit to her own clone, so its native death outcome runs.
    - There is no puppet for the deadly shadow.
  - **partner-shade-v1730a** (Baffling Shade): the host guards hit the puppet twice, but its health stayed 1, so no
    kill was sent; the death sync is still to be shown. The puppet was perceived and removed with her shade.
  - **partner-shade-v1730b** (Deadly Shade):
    - Her deadly shadow was streamed (kind 2) for the whole take, so Corvo sees it through the presentation.
    - No host puppet and no crash.
    - In her world it engaged the guard in 59 of 67 samples, but the guard's health stayed 40 in both worlds. Her hit
      (`kill_damage`) is suppressed as a replica-world event.
  - **Next (1214gw):** on her game, a hit whose source (or a parent of it) is her current shade counts as hers, so the
    deadly shadow's `kill_damage` goes to the host as a PeerNative request.
- **Her own shade's hits** (1214gw, Test-PartnerShade-v1731; confirmed runtime):
  - **partner-shade-v1731b** (Baffling Shade; pass): the shade death is shared.
    - A host guard killed the puppet at 3.9 s (one recorded hit, health 1 to 0).
    - The host sent the kill, and her game applied the same native hit to her own clone (`kill_applied`). Her shade
      ended by 4.4 s.
  - **partner-shade-v1731a** (Deadly Shade; fail):
    - Her hit now counts as hers: it was no longer suppressed as a replica-world event.
    - It was then refused as `hit_damage_parameters`, so no PeerNative request was sent.
    - Her copy of guard 1 read 0 health while the host guard kept 40: the deadly shadow's kill diverges.
  - **Next (1214gx):** for a hit by her own shade, the amount is clamped to the PeerNative limit and a world inflictor
    or attacker is allowed. Her game records these hits.
- **partner-shade-v1732a** (1214gx, Deadly Shade; pass, confirmed runtime):
  - Her deadly shadow's hit (attacker her deadly shadow, `kill_damage`) counted as hers and was forwarded as a
    PeerNative request.
  - The host applied it natively, and guard 1 died in both worlds (host 40 to 0, her copy 40 to 0). Her deadly shadow
    was streamed throughout (Corvo sees it), with no host puppet.
- **Baffling Shade's cloud** (partner-shade-v1731b film; preliminary visual review):
  - In her world, the shade's death releases a smoke cloud, and her copy of the guard stands in it.
  - The host has no cloud, and its guard keeps fighting. So the death is shared, but the baffling effect on the host's
    NPCs is missing.
  - Next: find what the death spawns (an entity snapshot of her world around the death) and spawn it at the host
    puppet's death.
- **Shades, still to do:**
  1. The reader and the stream accept the deadly shadow (a shade kind in the packet), and the host puppet uses the
     same definition as hers. A deadly shadow on the host fights the host's NPCs natively.
  2. A host kill of the puppet kills her shade natively, so its death outcome runs in her world.
  3. Her shade's hits on replicas reach the host.

## Possessed bodies (host world; consistent)

- carry-body-v1644b (confirmed runtime): Corvo, with Possession, Parasite and Decayed Harbor granted, possesses the
  guard's corpse after his lethal takedown.
  - Player state 26 from about 1.3 s to 7 s, with his position at the corpse.
  - The possessed corpse did not move under the walk input in his world, and Emily's copy stayed with it (0 m in both,
    end gap 0 m). He left at the corpse.
- **Decision:** Possession (`possession_decayed_harbor`, `possession_fast_swap`, `possession_alchemist`,
  `possession_ext_duration`) is Corvo's, so it is always a host-world outcome. Emily's copies follow NPC authority, and
  Corvo's own hide and show is the accepted possession presentation (audit v1341). Fast swap between hosts is not taken
  (hypothesis: Corvo stays hidden between hosts).

## Fixtures

- Record-Coverage-v1637/v1638 'stoptime'.
- Record-MissionCoverage-v1633 and Test-CarryBody-v1640/v1641 `-GrantNamed`. `coop_grant_named` enables power
  enhancements; Shadow Kill ran on the lab guard.
- Test-CarryBody-v1641 tolerates a guard turned to ash; v1642 `-PossessCorpse`; v1643 `-DominoLink`.
- Test-CarryBody v1644-v1651:
  - Domino casts after arming, from behind each guard, with each step recorded and filmed;
  - placement at the guard's floor height;
  - the guard 1 sample after any kill of guard 2;
  - the source's health held, the aim pitch clamped and a missed mark retried;
  - `-HoldAi` from arming, with no renewal.
- v1652 `-CastPower mesmerize|doppelganger`.
- Test-TakedownVariants-v1655 `-GrantNamed` and the guard's presence after the kill.
