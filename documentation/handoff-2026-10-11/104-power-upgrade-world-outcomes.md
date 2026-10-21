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
  - Next: a control cast with the host object off (1214gq lab switch `coop_mesmerize_native follow_off`), to show the
    stop and the walk come from the host object; then visual review.

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
