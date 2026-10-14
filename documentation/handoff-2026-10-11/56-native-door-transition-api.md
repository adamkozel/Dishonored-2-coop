# Native door transition API: bounded isolated experiment

This continues [ordinary door observations](47-native-door-interactions.md). Shared door networking is not implemented by this experiment. It preserves the working702 milestone and the unchanged protocol69, bank67 and357 clips. Only the physical isolated game copy is changed; all test scripts restore both checkpoint inventories/health and verify14 protected files.

## Identity and local native API

The exact executable SHA256 is `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Source [native_openable.h](../../outputs/d2-coop-lab/src/native_openable.h) validates the registry generation, exact ArkDoor/Openable types, reciprocal component/state-machine ownership, native visual-child registration and cached animation ownership. Local handles are never proposed as cross-process identities.

The isolated door is `ld_door_350cm_int_rich_wood_01_3` on the existing Dunwall tower map. The fixture requires exactly one matching validated door, stable native state, no lock/block/breakage/linked door, a valid authored animation, both gameplay-ready peers and an active NPC-isolation lease. Every call is revalidated on the engine thread. Duplicate desired states are no-ops. The experiment never writes a copied door transform or replaces native UI.

The initial707 reader mistakenly required character AnimBase VT RVA1b80408. [708](../../work/natural-moveset/native-door-api-corvo-v708/result.json) safely refused all four requests with zero native calls. [709 read-only diagnosis](../../work/movement-integration/door-native-api-v709/summary.json) confirmed the door uses base AnimBase VT **1b7f408**. Candidate710 corrects this; tests explicitly reject the character type.

## Why the first candidate failed

[711](../../work/natural-moveset/native-door-api-corvo-v711/result.json) made three calls and one no-op rather than the expected two calls/two no-ops. The first request traversed0→1→0; the second traversed0→1→2, then close returned0. Both checkpoint inventories, health and14 protected hashes were restored. This is a failed API proof, not accepted synchronization evidence.

The [source contact sheet](../../work/natural-moveset/native-door-api-corvo-v711/source-sheet.jpg) and [observer sheet](../../work/natural-moveset/native-door-api-corvo-v711/observer-sheet.jpg) were inspected. The source leaf moves, with a transient bright/unrendered doorway in the first opening. The observer door stays closed. Static sheets do not validate sounds, collision, full animation quality or pacing; the cause of the transient rendering appearance remains unconfirmed.

The first API candidate was `14074d7a0`, an interrupt/reversal helper. Static706 analysis missed its unconditional completion of the previous Moving state before starting a new one. The saved initial Moving target at Openable+0xb0 is **0xffffffff** in both711 and the earlier successful native-use641 baseline. `14074adf0` treats any nonzero saved target as Open. `1406df740` commits that state before the new Moving entry. The entry therefore treats the attempted opening as motion from Open, and finishes Closed. This static explanation matches711's observed first-call failure. Do not work around it by issuing the opening twice.

## Corrected candidate713 (verified in714/715 below)

[712 transcripts](../../work/movement-integration/door-native-api-v712) record the transition queue, completion helper, native auto-close and resume wrapper. `14074a400(machine, parameters)` queues Moving1 through the native state guard without finalizing an inactive prior transition. Four parameter bytes are `[opening, reverse, side, clip]`. Ordinary interaction `14074b6e0` inlines the same operation; native auto-close `14074ac50` calls it directly and resumes the animation and Openable via `1407e79a0`.

For opening, select entries+1e0/+1e1 by side. Closing preserves the recorded opening side but uses the opposite clip: +1e3 forside0, +1e2 forside1. The interrupt helper's side/clip convention is not the ordinary close convention. Tests now verify all four combinations.

Candidate713 uses this native queue and component resume with exact code signatures and the existing strict identity guard. It has not yet earned a runtime pass at this initial documentation point. The intended714/715 tests record source and observer for both characters and check stable0→1→2→1→0 with duplicate no-ops. Later results must be appended before claiming success.

## Remaining work

After local native API validation: reliable native Steam door requests and host ordering; authored identity matching across processes; duplicate/stale epoch rejection; suppression of reflected changes; convergence and conflict tests. Locks/keys, linked doors, breaking, script events, area visibility, save/load and NPC-door interactions still need coverage. Native calls alone do not prove all script, sound, navigation or collision semantics.

## Local API results714/715

[Corvo714](../../work/natural-moveset/native-door-api-corvo-v714/result.json) and [Emily715](../../work/natural-moveset/native-door-api-emily-v715/result.json) both pass. Each uses precisely two queue calls and two duplicate no-ops, with no refusals. Both native source cycles are0→1→2→1→0; the peer remainsClosed0 as required by the local-only experiment. Both full inventories, player/partner health and14 protected files were restored.

All four714/715 source/observer contact sheets were inspected. The source leaf opens onto the furnished closet and closes; the peer stays closed. The transient bright/unrendered opening seen in711 is absent in these sampled source sheets. The doorway and camera angle provide limited character visibility, so these are door API evidence, not acceptance of body animation. Audio was not recorded, collision traversal was not exercised, and script payloads were not audited. The explicit queue retains the native stored interactor rather than synthesizing a new player interaction event.

Verified local candidate713 DLL: `F474471D41F50917DD57726C3756523825094DD4AD85A1F6E6D7A2CE17BF48B3`. The original702 and failed707/710 candidates remain archived.

## Native Steam experiment716 (bounded718/719 verification below)

[door_protocol.h](../../outputs/d2-coop-lab/src/door_protocol.h) defines a496-byte desired-state packet. Only the host publishes revisions; only the client sends requests/acknowledgments. Canonical authored names/definitions, both sessions, both world epochs, the map and both explicit arm tokens identify the transaction scope. No native pointers or local handles cross the wire. Pure tests cover malformed scope, wrong roles, stale revisions, canonical strings and duplicate desired states.

[door_sync.h](../../outputs/d2-coop-lab/src/door_sync.h) is disabled by default, restricted to the single unlinked unlocked Dunwall door and the explicit isolated movement fixture. Each local native door lifetime is pinned and revalidated. Ordinary source motion creates an ordered desired state; the receiver invokes the validated native queue only once, waits for the native stable result, and suppresses reflection. Reliable Steam packets are retried until acknowledged. A request acknowledgment keeps the client from applying an older host state over its own action before the host orders that request. Loading, rearming or loss of the required native context disables this experiment.

`coop_door_sync on/off` is deferred to the native engine mutation phase. `coop_door_sync duplicate` resends the last exact request/state packet for duplicate testing. The planned717 harness uses real X interaction input, observes both native state machines, counts remote native calls, records both views, disables the experiment and restores the protected baseline. Runtime success must be established separately; packet tests alone do not verify shared doors.

## Bidirectional Steam results718/719 and retained717

[Audit720](../../work/movement-integration/native-door-audit-v720.json) summarizes four passing local/network cases. [Emily718](../../work/natural-moveset/native-door-sync-emily-v718/result.json) and [Corvo719](../../work/natural-moveset/native-door-sync-corvo-v719/result.json) use ordinary native X interaction on the source. Both processes traverse0→1→2→1→0. Each receiver executes exactly two native queue calls, while the source makes zero API calls. Both end applied/acknowledged at host revision3. Emily's case sends exactly two client requests, received and ordered by the host. Two explicit replays of the same reliable packets cause no extra native call or revision. The host direction also exercises repeated state delivery while awaiting native completion.

[717](../../work/natural-moveset/native-door-sync-corvo-v717/result.json) is retained as a failed harness run. Its state and call measurements already showed the correct network cycle, but it required zero rejected packets since arming. Its first sample, before input, already contains one client rejection. No rejection count changes during the recorded actions. The precise initial rejection reason is not instrumented; do not claim it has been diagnosed.718 records each initial counter and checks that none increase during measured actions. Both718 and719 pass this narrower, explicit assertion. No runtime guard or packet validation was relaxed.

All six717–719 source/observer contact sheets were inspected. The door visibly opens and closes in both game views, exposing the closet. Character visibility remains limited by the doorway and viewing angle. These recordings validate door presentation, not continuous remote-body visibility, foot contact, combat or the full movement library. Captures have no audio, and no traversal test was run.

All takes restore both full checkpoint inventories and player/partner health, then verify14 protected save/settings hashes. The experiment is disabled after testing; the test environment remains on the restored Dunwall baseline. The original Steam executable/DLL hashes and the corresponding isolated hashes are checked separately by the18-file integrity refresh.

Live716 DLL: `4A7F99EE4FB143EB4892BAAA13C0122EF3B0E5C1DE7345B2C45C9C7F4D81531D`. Probe remains `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Archives retain713 as the validated local API foundation and702 as the prior diagnostic-scheduling milestone. All bank hashes, protocol69 and357 clips remain unchanged.

Next: collision/traversal after a remote opening; mixed-player use and conflicting requests; interruption by checkpoint load and stale-scope refusal; then additional authored doors and lock/key/script semantics. The native queue preserves the stored interactor, so story events requiring a specific acting player require additional investigation. Keep the feature opt-in and scoped until those cases are tested. Complete movement, shared-world authority and usable campaign co-op are still unfinished.
