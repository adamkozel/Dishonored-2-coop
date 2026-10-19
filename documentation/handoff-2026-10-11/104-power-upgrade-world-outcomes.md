# 104. Power upgrades with world outcomes (#117)

Some power upgrades change the world rather than the caster's body. The movement checklist already covers what the
partner sees of the caster (`power_upgrade_variants`, chapter 99). This chapter decides, for each such outcome, whether
it is shared between the two worlds or confined to the caster's world, and why. The general rule:
- Corvo is the host, so his outcomes on NPCs happen in the authoritative world, and Emily's copies follow through NPC
  authority.
- Emily's outcomes happen in her world, on host-streamed copies, and need a path back to the host.

The live candidate is 1214fl.

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
- **Open, Emily's NPC outcomes:**
  - **Domino** (`domino_death_link`, and Domino's base multi-target outcomes, which chapter 30 left unfinished):
    - carry-body-v1645a tried to link the two tutorial guards and then assassinate one.
    - No mark was made (mark count 0), and the guard noticed Emily, so there was no takedown.
  - **Deadly Shade and Baffling Shade, Rat Shadow, Mesmerize in combat:** still to take.

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
