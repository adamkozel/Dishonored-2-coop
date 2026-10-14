# NPC menu and checkpoint lifecycle, 847–849

## Verified scope

Candidate847 adds continuing session behavior to the842 NPC presentation stream. Ordinary native menus retain actor bindings and owned AI references while stopping source pose sending and client root/body writes. Resume clears pre-menu interpolation, preserves the source sequence replay floor and negotiates readiness again on the same arms. Shared checkpoint reloads release old references before native teardown and rebuild the eligible NPC catalog only after both games complete loading and restore their player roles.

[Audit849](../../work/movement-integration/npc-lifecycle-audit-v849.json), [lifecycle take848b](../../work/natural-moveset/npc-recovery-v848b/result.json) and [visible take849](../../work/natural-moveset/npc-visible-menu-v849/result.json) are the acceptance evidence. This is continuing presentation/AI ownership in the isolated Dunwall lab, **not complete world or combat authority**.

| Test | Observed result |
|---|---|
| Host pause, then client pause | Each held over6 seconds. Five initial channels retained their arms; no source poses or client root writes during stable suspension. Both resumed fresh traffic. Total recorded menu suspension about14 seconds per role. |
| Coordinated reload | One added load and completion per role, changed epochs, Corvo1/Emily2 preserved, fresh larger catalog negotiated. |
| Loaded catalog | Seven eligible actors: the prior five plus `ld_spawner_vista_5` and `ld_spawner_alexi_escape`. Five223-joint rigs and two224-joint rigs. Their eligibility was read anew; no added per-NPC implementation. |
| Native AI ownership | Initial10 references released before old-world teardown;14 acquired for the seven loaded actors. Final session totals24 acquired,24 released,0 retired,0 lost. |
| Visible pause positive control | Two visible223-joint guards had341 current-arm body applications each before pause. Lifetime body count held at5967 throughout the sampled pause, then increased to6211 after resume. Ten pause references remained owned during the menu, then released at test end. |
| Restoration | Both tests restored complete isolated inventory and56 player health, gameplay and protected baseline hashes. Streams end disabled. |

[Host menu recording](../../work/natural-moveset/npc-visible-menu-v849/host.mp4), [client recording](../../work/natural-moveset/npc-visible-menu-v849/client.mp4), and [contact sheet](../../work/natural-moveset/npc-visible-menu-v849/contact.png) show the native Pause screen, the client's held guard posture, and animation resuming. These recordings are indexed in the local review dashboard as **NPC menu suspension and resume**. They do not visually validate the224-joint actors, combat, death or a whole campaign transition.

## Implementation and exact pins

- [Candidate847](../../work/movement-integration/candidate-npc-stream-v847/identity.json): DLL `15CEB61FDAC3FF17117EE2C6E5411A77AA6F698FD9381D9BBF532E4C5747ACD4`; probe remains `EA5444F1ECEDED2937AEE80E21AA7F5609AA03536B797D82B92FF27C28944489`. Verified842 remains archived.
- [NPC stream](../../outputs/d2-coop-lab/src/npc_authority_stream.h): session scope, menu suspension, coordinated renewal, aggregate ownership counters and explicit pre-load cleanup.
- [NPC protocol/buffer](../../outputs/d2-coop-lab/src/npc_authority_protocol.h): `resume()` drops sample/clock state while retaining `replayFloor`; a stale accepted sequence cannot re-enter after a menu. Wire protocol2 is unchanged.
- [Shared scope policy](../../outputs/d2-coop-lab/src/door_session_scope.h): reused session/map/epoch, recognized-menu and both-new-world checks. Although named for doors, it grants no native entity access; NPC ownership remains separately validated.
- [Checkpoint trace](../../outputs/d2-coop-lab/src/checkpoint_trace.h): `beforeNpcNativeLoad()` runs before the original native load command. An uncoordinated native load disables the stream session; it does not silently inherit a different world.
- [Checkpoint coordinator](../../outputs/d2-coop-lab/src/checkpoint_sync.h): calls begin/finish NPC recovery alongside the existing world systems, preserving the both-complete and role/avatar restoration barrier. Shared death retires active presentation, but general enemy death synchronization is still absent.

Menus keep references because the native actor/component lifetimes have not changed. Gameplay mutation remains forbidden while either peer is in a recognized menu. Cutscenes, unknown native phases, disconnected sessions and uncoordinated world changes are not treated as ordinary menus. The loaded catalog never reuses old native pointers as mutation targets. Native generations may repeat across a reload, so the new world epochs and fresh per-channel arms are also required.

Existing build/test machinery is retained: [build847](../../work/movement-integration/Build-NpcStream-v847.ps1), [stage847](../../work/movement-integration/stage_npc_stream_v847.py), [deploy847](../../work/movement-integration/Deploy-NpcStream-v847.ps1), [lifecycle harness](../../work/natural-moveset/Test-NpcRecovery-v848.ps1), [visible harness](../../work/natural-moveset/Test-NpcVisibleMenu-v849.ps1), and [auditor/publisher](../../work/movement-integration/publish_npc_lifecycle_v849.py). World/NPC protocol, action, movement resume, checkpoint protocol, both actual357-clip banks and proxy smoke passed. New protocol cases exercise replay rejection after a menu and rejection of old-epoch pose envelopes after coordinated renewal.

All game tests validate the isolated executable path/start time and candidate hash and hold the named native-operation mutex. The original Steam executable/DLL and protected saves/settings remain unchanged. No new native UI or first-person animation is introduced.

## Failed848 and corrected848b

[Raw848 failure](../../work/natural-moveset/npc-recovery-v848/result.json) is retained. The harness assumed that every fresh checkpoint would contain five eligible actors. In fact both renewed channels had seven actors, including Alexi and a vista guard that were eligible in that loaded state. All seven were ready and streaming; cleanup released24/lost0 and restored the fixture.

Only the test was changed for848b: it reads the new native host catalog, verifies stream membership against that catalog, checks reciprocal peer handles/arms/rigs, and expects exactly two references per fresh actor. Candidate847 source/binary did not change between these takes. The corrected test passed. This demonstrates catalog reconstruction on a coordinated load; it does **not** implement ongoing actor creation/destruction within a running world.

## Persistent findings and next development

Ghidra bookmark `D2Coop_NpcLifecycle_849` at native LoadGame `0x140658d00` records the verified pre-teardown ownership release and the acceptance boundaries. [Tool receipt](../../work/handoff-npc-lifecycle-bookmark-v849.json) confirms the project was saved. This extends existing native load/AI API findings rather than claiming a new full decompilation.

Remaining NPC work includes mid-world actor birth/retirement, repeated client hit requests and authoritative health/death, enemy equipment, script and physics outcomes, AI perception of both players, interest/rate selection and the remaining visible rigs. Menu retention currently requires a fresh connected same-world peer; reconnection is a separate session lifecycle. Continue campaign/progression, movement/VFX gaps and native session UI/separate-account Steam validation toward the unchanged playable-co-op goal.
