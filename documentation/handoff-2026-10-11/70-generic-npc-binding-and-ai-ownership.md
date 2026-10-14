# Generic NPC binding and AI ownership, 836–838

This chapter records candidate837. The newer bounded network presentation result is documented in [chapter71](71-generic-npc-steam-root-and-body-stream.md).

## Result and boundary

The five active authored NPCs in the paired Dunwall world can now be selected by
one general native descriptor and held using one reusable decision/reflex lease
implementation. The client acquired ten native references, kept all five actors
paused, rejected duplicate acquisition, and returned all ten references on both
automatic expiry and explicit cancellation. The host AI continued running and
the sampled script-held actors retained their original native flags/scheduler
membership. This is a verified local ownership prerequisite, **not a general
networked NPC implementation**. It does not move actors, replay animations,
change health, deliver damage, or synchronize corpses.

The current DLL is candidate837; candidate831 remains preserved. No game asset,
save, settings, native first-person behavior, or Steam DLL replacement was needed.
The test ends with the experimental hold disabled. No new animation video or
visual acceptance claim is attached to this nonvisual ownership test.

## Why this work follows grouped pickup coverage

Consumables use shared native families; individual handlers are not the plan.
The preceding 51-member pickup catalog and four-window family have independent
receipts. The next world blocker is broader: the old guard relay selects one
authored guard, assumes maximum health 40, and its melee path handles one
40-to-30 sword event. It cannot be presented as general campaign AI authority.
The new binding removes those target-name/health assumptions from ownership.

## Read-only census836

[Inspector](../../work/natural-moveset/Inspect-NpcAuthority-v836.ps1),
[result](../../work/natural-moveset/npc-authority-census-v836/result.json), and
[all profiles](../../work/natural-moveset/npc-authority-census-v836/profiles.json)
retain the original per-process identity, health, AI, pose and animation reads.
Both roles had 48 unique reciprocal authored NPCs, 14 living actors, and five
actors whose decision and reflex components were started, unpaused, and present
in their native schedulers' active partitions:

| Authored spawner | Health | Definition family |
|---|---:|---|
| `ld_spawner_guard_tutorial_2` | 40 | Dunwall guard |
| `ld_spawner_guard_tutorial_1` | 40 | Dunwall guard |
| `ld_spawner_ramsey_escape` | 50 | Mortimer Ramsey |
| `ld_spawner_guard_escape_1` | 40 | Generic guard |
| `ld_spawner_guard_escape_2` | 40 | Generic guard |

Other living actors include Alexi and eight vista actors. Started flags alone
do not grant ownership: the native script/scheduler state excludes them. Vista3
has no usable decision snapshot in this scope. Dead cinematic remnants and the
unbound partner avatar also must not become ordinary AI targets.

## Implementation837

- [native_npc_authority.h](../../outputs/d2-coop-lab/src/native_npc_authority.h):
  read-only binding, eligibility and paused-state predicates, and catalog.
  Local incarnation combines the exact game, entity handle, reciprocal sole
  spawner child, authored name/definition, component pointers, component +18
  incarnation words, AI registry/pool index/tree, owned reference frame, and
  health declaration. A second snapshot rejects changed identity, health or AI
  state. Catalog matching includes unused spawners when rejecting duplicate
  names. No cross-process pointer or handle is a global identity.
- [npc_authority_hold.h](../../outputs/d2-coop-lab/src/npc_authority_hold.h):
  per-actor leases and the bounded client-side batch experiment. Acquisition
  revalidates each binding immediately before requesting its native references.
  It rejects active attacks, previous pause owners and script-inactive AI.
  Release checks component incarnation before using the established native
  release routine; stopped/dead retirement is recorded separately from release.
  Reused or discarded worlds never receive writes through saved pointers.
- [engine_bridge.cpp](../../outputs/d2-coop-lab/src/engine_bridge.cpp):
  engine-thread request/service/status integration. The default is off.
  `coop_npc_hold 12000` requests a nonrenewing 12-second client-only hold;
  `coop_npc_hold off` releases it. Existing one-guard, AI-hold and observer
  fixtures reject overlapping acquisition. The hold checks world/peer scope
  and verifies its per-actor ownership every 100 ms.
- [runtime_probe.cpp](../../outputs/d2-coop-lab/src/runtime_probe.cpp):
  `--npc-authority` is a compact read-only catalog of currently eligible actors,
  with authored identities, component incarnation words and native health.
  The external reader found all five before deployment in 63/47 ms; the
  in-process test catalog took 16 ms. These are local measurements, not a
  production frame-time guarantee.
- [native_npc_authority_tests.h](../../outputs/d2-coop-lab/src/native_npc_authority_tests.h):
  generic names and 50-health acceptance, script-held and active-attack exclusion,
  multi-child/foreign-frame rejection, loading/retired component rejection,
  same-address reincarnation and concurrent-generation changes, and preservation
  of extra native pause references.

There is no new network packet in this candidate. The existing native Steam
transport, movement protocol69, bank67 and 357 clips remain in place.

## Exact-executable analysis

Game SHA-256:
`B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.
Ghidra program `Dishonored2.exe` was queried through the existing MCP connection.
[Full decompilation receipt](../../work/natural-moveset/ghidra-npc-ai-ownership-v837.json)
retains both function results. Bookmark `D2Coop_NpcAuthority_837` is saved in the
persistent project.

| Native function | Confirmed behavior |
|---|---|
| VA `1407e7800`, analyst label `D2_Component_Pause` | Locks manager +C08; requires a started component; increments the 21-bit reference count at +1C; queues the first pause through `1407eb390`. |
| VA `1407e79c0`, analyst label `D2_Component_Resume` | Uses the same lock; decrements one reference; queues native resume through `1407eb3e0` only when the last reference is removed. |

The state count and scheduler transition are distinct. The implementation checks
both pause bits and inactive scheduler membership, and never directly overwrites
either. The 32-byte API prefixes are checked before calls. Actor AI vtables and
registry offsets remain those already recorded in
[native_ai_state.h](../../outputs/d2-coop-lab/src/native_ai_state.h): decision
RVA1cc4108/manager+2E0, reflex1b8a588/+580, attack1b93a88/+600. General combat,
script handoff, new spawns, body playback and death are still separate work.

## Runtime evidence and failed take

[Harness](../../work/natural-moveset/Test-NpcAuthority-v838.ps1) and
[successful take838b](../../work/natural-moveset/npc-authority-hold-v838b/result.json)
retain their exact run scripts, catalogs, all observed AI states, inventory,
health, world epochs and cleanup receipts.

1. Both catalogs contained the same five authored identities, including Ramsey
   at maximum50. Native pointers/incarnation values remained process-local.
2. Client Decision/Reflex for each actor reached one owned pause reference,
   pause state2, and inactive scheduler membership. All attack components stayed
   inactive. The host Decision/Reflex components remained active and unpaused.
3. Vista reflex/attack samples kept their original flags/scheduler state.
4. Repeating the arm command did not add references or renew the lease.
5. Natural expiry released10, retired0, lost0; all five native AI pairs became
   active again with unchanged incarnation values.
6. Fresh acquisition followed by explicit cancellation also released10 with
   lost0. Player inventory/health, world epochs and protected files were retained.

[Final readback](../../work/natural-moveset/npc-authority-hold-v838b/final-readback.json)
independently found all five eligible active actors on both sides after explicit
cancellation. One earlier external snapshot returned unavailable; stable retries
passed without a native mutation or game restart.

[Take838](../../work/natural-moveset/npc-authority-hold-v838/result.json) remains
failed. A harness function named `H` collided with PowerShell's `h` alias for
`Get-History`, so the readiness poll never read the status file. The native
experiment still expired and released ten references. The function was renamed
`GetNpcHoldState`; the successful take uses the same837 DLL and probe. Do not
rewrite the raw failed result or attribute its incomplete checks to success.

## Pins and continuation

[Candidate identity](../../work/movement-integration/candidate-npc-authority-v837/identity.json)
records full source, binaries, build/deploy scripts and immutable bank hashes.

- DLL: `71D0E0D8292DE6E076E4509A0C6E994F01169B304C4BB4B9B5F2F235E584D188`.
- Probe: `EA5444F1ECEDED2937AEE80E21AA7F5609AA03536B797D82B92FF27C28944489`.
- Previous stable scope: [candidate831](../../work/movement-integration/candidate-usable-sync-v831/identity.json).
- [Build script](../../work/movement-integration/Build-NpcAuthority-v837.ps1),
  [deployment script](../../work/movement-integration/Deploy-NpcAuthority-v837.ps1),
  [staging script](../../work/movement-integration/stage_npc_authority_v837.py).

Next, use these bindings and owned leases in a separate Steam authority channel
with per-actor negotiation and world epochs. The host must own simulation and
gameplay outcomes; the client must present roots/bodies without autonomous AI or
duplicate damage. Generalize frame leases and rig-aware body playback beyond the
single guard, then repeated combat, deaths, bodies, spawns, script ownership,
pause/checkpoint/reconnect renewal and sustained play. Native partner perception,
shared progression, campaign coverage, native session UI and separate-account
online validation remain on the full roadmap. Functional batch coverage remains
the priority; this ownership receipt does not complete those requirements.
