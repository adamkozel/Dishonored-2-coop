# Dishonored 2 co-op: audit and continuation handoff

This folder documents the project through the ongoing 14 October 2026 development session. It is an evidence-backed development handoff, not a release claim. The mod and the complete movement phase remain unfinished. Historical reports retain the build, fixture and limitations under which they were obtained.

Latest stage-6 work: [shared death, checkpoint restart and paired autosaves on mission maps, candidate 1153](88-mission-shared-death-and-checkpoint.md). Latest stage-5 work: [shared usables on mission maps, candidate 1123e](87-mission-shared-usables.md). Latest stage-4 work: [shared NPC authority on mission maps, candidate 1091](86-mission-npc-authority.md). Latest presentation work: [partner equipment on missions and item-wheel robustness, candidate 1074j](85-mission-equipment-and-item-wheel.md). Latest world-authority work: [shared pickups](83-mission-shared-pickups.md) and [shared doors](84-mission-shared-doors.md) on mission maps, candidate 1059c. Latest movement work: [water exit and action-stream hold, candidate 1056](82-water-exit-and-action-stream-hold.md); swimming/chain-climbing role parity in [chapter 50](50-swimming-climbing-mode-discovery.md). Latest combat work: [native timed parry for relayed enemy hits](80-native-timed-parry.md); before that [native held block for relayed enemy hits](79-enemy-combat-recovery-and-defense.md#native-pre-damage-block-resolution-and-accepted-held-block-1013c). Earlier ranged work: [native pistol integration](75-native-ranged-contact-development.md) and [shared projectile-contact profiles](76-shared-ranged-profiles.md).

Workspace: `C:\Users\adako\Documents\Codex\2026-10-08\i-x20`.

Start with [current state](01-current-state.md), [roadmap](02-roadmap.md), [continuation runbook](06-runbooks.md) and [audit checklist](10-audit-and-known-gaps.md). The [generated inventory](generated/README.md) supplies the detailed evidence, full movement requirement list, historical state sections, native findings, source addresses, test scripts, milestones and recordings.

## Contents

1. [Current implementation and exact pins](01-current-state.md)
2. [Complete project roadmap and movement acceptance](02-roadmap.md)
3. [History, decisions and rejected approaches](03-history.md)
4. [Architecture and ownership rules](04-architecture.md)
5. [Tools, versions, skills and links](05-tools-and-skills.md)
6. [Build, test, capture, recovery and continuation](06-runbooks.md)
7. [Animation authoring and porting pipeline](07-animation-pipeline.md)
8. [Reverse engineering and decompilation guide](08-decompilation-guide.md)
9. [Evidence interpretation and provenance](09-evidence-and-validation.md)
10. [Independent audit and remaining risks](10-audit-and-known-gaps.md)
11. [Work completed while documenting](11-current-session.md)
12. [All generated inventories and source transcripts](generated/README.md)
13. [Documentation audit and export limitations](12-documentation-audit.md)
14. [Native body throw: source, impulse, ownership and experiments](13-native-body-throw.md)
15. [Compact contextual release: exact source, rejected transitions and continuation](14-contextual-body-release.md)
16. [Bend Time: native investigation and movement integration](15-bend-time-integration.md)
17. [Native failed casts and interrupted movement recovery](16-power-failure-and-resume.md)
18. [Equipment recordings and crouched cast sword clearance](17-equipment-and-cast-clearance.md)
19. [Native grenade investigation and read-only identities](18-native-grenade-investigation.md)
20. [Grenade authoring, native prop and integration work](19-grenade-authoring-and-prop.md)
21. [Grenade Steam presentation, failed runs and serialization](20-grenade-stream-development.md)
22. [Native stun presentation](21-native-stun-development.md)
23. [Crouched grenade contexts](22-crouched-grenade-development.md)
24. [Airborne grenade contexts](23-airborne-grenade-development.md)
25. [Native elixirs](24-native-elixir-investigation.md)
26. [Native Mesmerize body and world](25-native-mesmerize-investigation.md)
27. [Mesmerize targets, partner exclusion and beam effects](26-mesmerize-targets-and-partner-exclusion.md)
28. [Presentation AI lifetime and menu-crash regression](27-presentation-ai-lifetime.md)
29. [Mesmerize cancellation, natural lifetime and crouch](28-mesmerize-cancel-lifetime-and-crouch.md)
30. [Airborne root interpolation and body continuity](29-airborne-root-continuity.md)
31. [Native Domino / PhantasmalLink investigation](30-native-domino-investigation.md)
32. [Native Devouring Swarm investigation](31-native-devouring-swarm.md)

## Reading rules

- **Confirmed static** means code or exact asset evidence supports the finding. It does not prove a runtime mutation is safe.
- **Confirmed runtime** means a particular test passed on its recorded executable, mod build, bank and scene. It does not establish campaign coverage.
- **Hypothesis / candidate** means an interpretation or authoring choice remains to be tested.
- **Visual acceptance** is distinct from numeric transfer, lifecycle and physics checks. Neither candidate counts nor video counts measure finished work.
- Some long-lived state fields and older READMEs are historical and contradict newer milestones. Read the per-build report and the current-state page before treating any field as live.

## Portability and coverage

The Markdown includes original saved research notes and JSON findings rendered as readable, source-labelled records. Local links point into this workspace. The companion inventory records source sizes and SHA-256 digests. Videos, game resources, save files, tool installations and the live Ghidra database are referenced rather than duplicated into this folder. A receiving agent needs the workspace or the referenced materials as well as these documents.

This is a reconstruction from saved project artifacts and the available conversation. It cannot recover unsaved historical tool output or claim that every transient investigation was persisted. Saved Ghidra project notes are exported separately to cover findings that were not added to older JSON journals. No credentials or global account configuration are included.

Regeneration: [build_handoff.py](../../work/build_handoff.py). It refreshes generated documentation; the authored chapters above are maintained separately. The generation audit reports missing local links and unavailable source exports rather than hiding them.

- [Native Possession: exact-image static findings and runtime plan](32-native-possession-investigation.md)
- [Community tools: tested Void Slice CLI and existing Dishonored2-Debug reference](33-community-tool-assessment.md)
- [Mutual partner exclusion: requirement, coverage, Possession investigation and verification](34-partner-interaction-exclusion.md)
- [Native Shadow Walk: exact-image identities and integration plan](36-native-shadow-walk-investigation.md)
- [Emily Doppelganger casting, clone presentation and native effects](35-doppelganger-cast-and-clone-lifecycle.md)
- [Existing ZikQ co-op mod: bounded static reference assessment](37-existing-coop-mod-assessment.md)
- [Movement continuity through unfinished action bindings](38-unbound-movement-continuity.md)
- [Native Heart body and held-prop binding](39-native-heart-binding.md)
- [Native trap body, held props and retained failed tests](40-native-trap-binding.md)
- [Native rear-choke phases and existing authored body binding](41-native-rear-choke-binding.md)
- [Native fast rear assassination and existing paired actor donor](42-native-fast-back-assassination.md)
- [Target-bearing sword playback through the existing combat pipeline](43-target-bearing-sword-combat.md)
- [Crouched combat and native hit recovery](44-crouched-combat-hit-recovery.md)
- [Native close-range kick through the existing pipeline](45-native-close-range-kick.md)
- [Native crouched heavy attack and exact next binding](46-native-crouched-heavy-attack.md)

48. [Native ordinary door observation and state trace](47-native-door-interactions.md)

49. [Batched locomotion coverage and later animator polish](48-batched-locomotion-coverage.md)

- [Automatic crouch, confined low mantle and retained continuity limits](49-automatic-crouch-and-confined-mantle.md)

- [Native swimming/climbing mode and fixture discovery](50-swimming-climbing-mode-discovery.md)

- [Native chain fixture and concrete dependency blocker](51-native-chain-fixture-blocker.md)

- [Native pickups and readable-screen recovery](52-native-pickups-and-readable-recovery.md)

- [Native setup scene skipping](53-native-setup-scene-skip.md)

- [Crouched loot and native food continuity](54-crouched-loot-and-native-food.md)

- [Native diagnostic hitches and bounded scheduling](55-native-diagnostic-hitches.md)

- [Native door transition API and bounded Steam replication](56-native-door-transition-api.md)

- [Native door catalogue, traversal and mixed-player use](57-native-door-catalog-and-traversal.md)

- [Native checkpoint retry correction and door interruption](58-native-checkpoint-retry-and-door-interruption.md)

- [Native door conflicts and opening direction](59-native-door-conflicts-and-opening-side.md)

- [Door-session renewal after shared checkpoints](60-door-session-checkpoint-renewal.md)

- [Ordinary doors survive native pause and resume](61-door-native-menu-resume.md)

- [Continuous shared checkpoints and native death/retry](62-continuous-checkpoint-death-recovery.md)

- [Native coin lifecycle, currency and guarded reward experiment](63-native-coin-pickup-and-reward.md)

- [Native Steam shared coin, callback threads and checkpoint receipts](64-native-steam-shared-coin.md)

- [Currency catalog, native batch validation and pickup families](65-currency-catalog-and-pickup-families.md)

- [Shared inventory stack families and native capacity](66-shared-inventory-stack-families.md)

- [Food family and ordinary pickup validation](67-food-and-grouped-pickup-validation.md)

- [Native interaction families research](68-native-interaction-families-research.md)

- [Native window state-list synchronization](69-native-window-state-list-sync.md)

- [Generic NPC binding and native AI ownership](70-generic-npc-binding-and-ai-ownership.md)

- [Generic NPC Steam root and body stream](71-generic-npc-steam-root-and-body-stream.md)

- [NPC menu and checkpoint lifecycle](72-npc-menu-and-checkpoint-lifecycle.md)

- [Generic NPC combat development and current limitations](73-npc-combat-development.md)

- [Native damage families, final damage boundary and compound contact ordering](74-native-damage-families-and-ordering.md)

- [Native ranged contacts, projectile identity and replay](75-native-ranged-contact-development.md)

- [Shared ranged profiles and two-player native contacts](76-shared-ranged-profiles.md)

- [Native remote-player perception investigation](77-native-remote-perception.md)

- [Native enemy contacts to the actual remote player](78-native-enemy-to-player-contacts.md)

- [Enemy combat recovery and native defense](79-enemy-combat-recovery-and-defense.md)

- [Native timed parry and attacker blowback](80-native-timed-parry.md)

- [Co-op session mode design (stage 7, parked)](81-session-mode-design.md)
- [Water exit and action-stream hold (candidate 1056)](82-water-exit-and-action-stream-hold.md)
- [Shared pickups on mission maps (candidates 1059–1059c)](83-mission-shared-pickups.md)
- [Shared doors on mission maps (candidate 1059c)](84-mission-shared-doors.md)
- [Partner equipment on missions and item-wheel robustness (candidates 1071–1074j)](85-mission-equipment-and-item-wheel.md)
- [Shared NPC authority on mission maps (candidate 1091)](86-mission-npc-authority.md)
- [Shared usables on mission maps (candidates 1123–1123e)](87-mission-shared-usables.md)
- [Shared death, checkpoint restart and paired autosaves on mission maps (candidates 1147–1153)](88-mission-shared-death-and-checkpoint.md)
- [Mission progression: discovery and trigger mirroring (candidates 1163–1171)](89-mission-progression-discovery.md)
- [Stage 7: release packaging, native Steam sessions, product mode, join-follow and soak (candidates 1171–1193)](90-release-packaging-and-steam-sessions.md)
- [Product partner-presentation recovery: pause, modal, load, restart; bank stall, stream handshake, congestion, usable hellos (candidates 1203–1203f)](91-product-presentation-recovery.md)
- [Stage 3: native use interactions (state 75 -> authored reach), window/counter/knockdown investigations (candidates 1213–1213b)](92-native-use-interactions.md)
- [Stage 3: takedown census, drop/slide/side variants and sword finishers (candidates 1214b–1214g)](93-takedown-census-variants-and-finishers.md)
- [Stage 3: visual-acceptance sweep, counterattack, sword locks and item gestures (candidates 1214h–1214j)](94-visual-sweep-counterattack-and-gestures.md)
- [Stage 4: shared combat authority on missions — unshared deaths and host-world damage (candidates 1214n–1214o)](95-shared-combat-authority.md)
- [96. External research references](96-external-research-references.md): public Dishonored 2 resources (camera, cvars, autosplitter, saves) and what was verified against the lab build.
- [97. Campaign map sequence](97-campaign-map-sequence.md): every mission's maps in play order, how each one ends, and the map-change boundaries stage 6 must carry.
- [98. Release without game files](98-release-without-game-files.md): how the installer builds the voidmod packages, movement banks and prop tables from the player's own install, release 0.7.0, its installer and the clean-PC path in the VM.
- [99. Movement completion coverage](99-movement-completion-coverage.md): the product-controller coverage harnesses (1318–1326), lab geometry and native input facts, the hold and grace receiver fixes (1214cg, 1214ch) and the per-item completion audits.
- [100. Native takedown trigger research](100-native-takedown-triggers.md): the native conditions behind the window, Shadow Walk wall, seated and knocked-down takedowns (static, Ghidra MCP), and the takedown and choke sensors.
- [101. Native player states outside the movement checklist](101-native-player-states.md): the decision for every unlisted native player state (#102), and the Timepiece presented on the partner like the Heart (1214eu).
- [102. Scripted civilians and posted NPCs in NPC authority](102-scripted-npcs-in-npc-authority.md): civilians' own decision AI class, NPCs paused by the game at scripted posts, civilian relevance limits, and their takedowns presented on the partner (#113, 1214ex-ey).
- [103. Far Reach pulls in both worlds](103-far-reach-pulls.md): the pulled object and the pulled body follow Emily's pull in Corvo's world through the held-object path (1214fc-fe), the "refused" body pulls that were aim failures, and the standing host corpse after her lethal takedowns (#127, fixed in 1214fj by freezing her last victim pose).
- [104. Power upgrades with world outcomes](104-power-upgrade-world-outcomes.md): Stop Time freezes the host's NPCs in both worlds, Shadow Kill's ash is shared through the ASH1 Dead (1214fk-fl), Corvo's possession of a corpse stays consistent, Emily's Domino linked deaths and knockouts are shared through Linked requests (1214fm-fp, with the implied Ready and the knockout rebase as general combat fixes), and Emily's Mesmerize, shades and Rat Shadow rats are confined to her world (#117; sharing them is #131).
