# 102. Scripted civilians and posted NPCs in NPC authority (#113)

Takedowns of scripted civilians, and of NPCs at scripted posts, did not present on the partner. Examples:
- the Edge of the World docks photographer kept working her camera in Corvo's window while Emily choked her
  (1424a, 1426a);
- the Royal Conservatory servant stayed seated in the partner's window during a seated choke (1577a, 1578e, 1579);
- the square overseer's client copy never bound at its post (1548b, 1557b, 1564c-f). Stage 3 worked around it with the
  `-PlaceBoth` and `-ClientPrebind` lab fixtures.

Candidates 1214ex and 1214ey fix both causes.

## Causes (confirmed runtime, both worlds)

- **Civilians were unreadable.**
  - With the probe-only candidate 1214ev (`--npc-authority-all`), right after loading Edge of the World (state-trigger-v1594a)
    and the Royal Conservatory (v1594b), every civilian failed the authority read outright. This included the
    photographer and the servant.
  - The probe-only candidate 1214ew (`--npc-ai-registries`, ai-registry-research-v1595a) showed why: civilians carry
    `arkCpntAIDecisionCivilian` (VT `1ccb1f8`, 0x48 bytes, AI-manager registry +0x458) in place of
    `arkCpntAIDecision` (VT `1cc4108`, 0x40, registry +0x2e0).
  - Their reflex, attack and idle-break components are the guards'. Both decision classes are built on the same base
    constructor `5880a0` (confirmed static: factories `8ff470` and `8f3d50`).
- **Posted NPCs are paused by the game.**
  - The overseers at their posts have decision and reflex AI in mode 2, with one pause reference and an inactive
    scheduler.
  - The host streams them, but the client's acquire rule wanted no pause reference and an active scheduler. It refused
    their Hellos as busy: about 4,300 refusals and 2,600 re-binds per take (1576b, 1578e).

## Changes

- **1214ex:**
  - `NativeAiKind::DecisionCivilian` ([native_ai_state.h](../../outputs/d2-coop-lab/src/native_ai_state.h)): the authority
    reader takes a civilian decision as the decision AI.
  - `npcAuthorityGamePaused` and `npcAuthorityLeaseEligible`
    ([native_npc_authority.h](../../outputs/d2-coop-lab/src/native_npc_authority.h)): an NPC that the game paused for a
    scripted post can be leased.
  - A lease adds exactly one pause reference on top of the game's and releases only its own
    ([npc_authority_hold.h](../../outputs/d2-coop-lab/src/npc_authority_hold.h)).
  - The client catalog, its Hello binds and every lease, in both roles, use the lease rule. The mod's other AI pause owners
    cannot run beside the NPC stream.
- **1214ey:** with civilians readable (49 on Edge of the World), a crowd could take the 24 channels from the guards
  whose fights and deaths are shared. Civilians now:
  - compete only within 15 m of a player;
  - are dropped beyond 20 m unless paired;
  - rank 8 places behind combat NPCs ([npc_authority_protocol.h](../../outputs/d2-coop-lab/src/npc_authority_protocol.h));
  - are marked `civilian` in the NPC stream status.

## Results (confirmed runtime; visual review of the partner films)

- **Readability:** on 1214ex, 107 of 108 living NPCs on Edge of the World are readable in both worlds; only Meagan is not
  (1596a). The client no longer refuses any Hello as busy, and binds 48-190 times per take.
- **Docks photographer, on 1214ey:**
  - Emily's choke (mission-coverage-v1598a): the host followed her victim pose (75 reverse poses). In Corvo's window the
    photographer is taken from her camera into the choke and slumps on the deck.
  - Corvo's choke (1598b): Emily's copy followed the host's stream (122 poses).
- **Conservatory servant, seated choke:**
  - Emily (1596c on 1214ex, 1598c on 1214ey) and Corvo (1597e, 1598e): she is bound on both sides. In the partner's
    window she is pulled up from the chair into the choke and lies unconscious beside it; she is unconscious in both
    worlds.
  - Only she was bound among the civilians on 1214ey (1 of 22 channels).
- **Posted square overseer:** without the client prebind (1597f), Emily's world shows him at the window where the host
  placed him, so the client bound its own game-paused copy and followed the host's root. That take's window choke did not
  run natively, as in 1576a/b (a stage 3 lab fixture issue).

## Remaining

- Meagan (a companion) is still unreadable; she is never a takedown target.
- The `-PlaceBoth` and `-ClientPrebind` fixture options are no longer needed for posted NPCs.
