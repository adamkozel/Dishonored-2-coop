# Native enemy perception of the remote player

Updated14 October2026. Exact executable SHA256 B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180. The verified combat fallback is candidate966, DLL70D6F3F7BBB49082467308C7471D429876A131CE3135A25DABA430C1C8F8971D. This investigation concerns real host AI recognizing the network-controlled Doppelganger. Enemy damage to the actual remote player is a separate unfinished feature.

## Relationship workaround rejected as unnecessary

Candidate973 generalized the historical fixed-story-avatar relationship override to bound host NPCs. It was built, archived and tested, then removed from active source and binaries by [recovery975](../../work/movement-integration/npc-perception-recovery-v975.json). Its complete source remains in `work/movement-integration/candidate-npc-combat-v973`.

The actual974 baseline showed that both current Doppelganger avatars already use native faction Player9. All five inspected enemies had native base/effective hostility2 toward them without overrides; both partners regarded each other as friendly. The old experiment in `partner_ai.h` required faction Loyalists and applied to earlier story-character avatars. Generalizing it would add no useful behavior here.

Test974 remains failed: its acceptance mistakenly required acquisition of unnecessary overrides. It acquired zero, and automatic restoration passed. Do not call it a successful lifecycle/cleanup test. No original Steam asset was changed. Restored966 is independently recorded by975.

## Controlled native detection failures

Takes976 and978 retain native host AI, pause only client AI through the existing NPC stream, and place the remote body in front of the escape guard. No faction overrides or combat relay are active. Neither meets its requested sustained remote-target condition; both restore the lab.

Take978 first proves ordinary local-player detection in both games. The watched guard then remembers/chases host player1 while the remote clone never enters its attention list. This establishes that AI was awake, but root heading is not guaranteed to be visual facing. Take981 removes that assumption by placing the remote player at the same location used by the positive control, approximately(-42.9563,-122,61.00368). It also fails selection and restores successfully.

Do not generalize one watched guard to every enemy. This becomes important in982 below. Raw take directories and copied run scripts are under `work/natural-moveset/npc-native-perception-v{976,978,981,982}`.

## Confirmed component and visibility findings979–980

Void reader exports are read-only under `outputs/d2-coop-lab/research/asset-exports/npc-native-perception-v979` and `npc-vision-settings-v980b`. Relevant native definitions:

- Clone Observable: `components/characters/player/base/powers/decoy/observable_decoy.cpntobservable`; typeNPC, visibilityNPC, alive/asleep/hacked/possessed contexts.
- Clone attention source: `components/characters/player/base/powers/deadlyshadow/aiattentionsource_deadlyshadow.cpntaiattentionsource`; seeing-enemy-NPC evidence with high continuous yield.
- Native human vision settings distinguish player and NPC visibility distances. No visibility setting has been changed during this investigation.

Both live local player and clone contain active `arkCpntObservable` (vtableRVA1b98078) and `arkCpntAIAttentionSource` (1b94a68). Observable+40 is1 in both, so component absence or zero registration count is not supported by the read. Read-only scripts validate the isolated executable and actor/component ownership before recording data. Files: `work/movement-integration/observable-readonly-v979`, `observable-readonly-v980`, and `observable-readonly-v981`.

| Exact native address | Confirmed static behavior |
| --- | --- |
|1406002e0|Observable attachment chooses required physics component from declaration+68 and retains a generation reference at+20/+28.|
|140600420 /1405ff2d0|Activation increments+40 and on0→1 queues area event1BE with the Observable pointer.|
|1405ff1f0 /140600490|Reference end/deactivation queues1BF when membership ends.|
|140600210|Checks that Observable+40 is positive.|
|1405ff520|Declaration+6c selects visibility-point generation. Player2 and NPC3 have different required layouts.|
|1405ff810|Ordinary NPC visibility point uses world-bounds centerXY and maxZ minus declaration+364/+368 offset, selected by physics state+238.|
|1405ff9f0|Player path uses PlayerPhysics and stance/camera information to construct seven visibility points. Do not invoke it on an NPC component.|
|1408780d0|Returns world bounds from render-frame+1840/+1850 unless actor flags select an alternate transformed-bounds path.|
|140600220|Queues1BF then1BE for an already registered Observable, without changing+40.|
|14087d200|Native SetTransform calls600220 for same-area changes and uses875fc0 for an area change.|

The clone uses declaration+68=1/+6c=3; the player uses0/2. Both baseline980 and in-fixture981 show clone root, render bounds and17 physics-body positions in the expected location. The stale-bounds hypothesis is unsupported by those reads; they do not independently certify line of sight.

Persistent Ghidra bookmark/save receipt: [v979](../../work/handoff-perception-bookmark-v979.json). Actual lookup/xref/decompilation responses are in `work/movement-integration/npc-perception-research-v979/ghidra-queries.json` and `ghidra-followups-v980.json`. Findings are analyst labels on the exact game executable, not official symbols.

## Repeated transform resets: diagnostic982

Existing remote interpolation calls native87d200 every engine callback, including while standing still. That repeatedly queues Observable removal/addition. Hypothesis: membership churn prevents AI from retaining the remote body normally.

Take982 repeats981 but stops only host-side remote follow after arrival. Doppelganger AI remains held; releasing that hold is unsafe because prior109 reproduced a native null-context crash. The specifically watched guard still does not select the clone, so the full982 test remains failed. However, Ramsey and escape guard2 deliver two genuine sword contacts to the owned clone40→20→0. Native damage records identify the attackers and weapon parents. Client player health is unchanged: this is not an enemy-to-client damage relay.

Six inspected client frames at0.2/1/2/3/4/7seconds show native enemies approach and swing, followed by withdrawal after the clone dies. This is visible diagnostic evidence, not complete paired combat acceptance. The comparison supports investigating repeated transform registration; it does not isolate every difference caused by stopping follow. Automatic restoration passes.

## Candidate983: bounded continuous-movement experiment

`src/avatar_perception.h` adds an opt-in `coop_avatar_perception on/off` diagnostic. Default is off. Only the already identity-validated owned Doppelganger transform receives a thread-local owner scope. Its same-area600220 refresh is suppressed for finite position changes up to3m. Every other actor/callback is forwarded unchanged, as are larger teleports. Native cross-area handling is outside the intercepted path. No faction, registration count, health or global vision setting is written.

The exact19-byte600220 prologue is checked before installation. Its conditional branch is relocated explicitly in a43-byte trampoline: copied13-byte prologue, inverse short branch over an absolute jump to the native exit, then absolute continuation at600233. No PC-relative instruction is copied without relocation. The experiment leaves local first-person controls and native AI hold ownership unchanged.

Build983 passes existing world/ownership, action, checkpoint, movement-resume, proxy smoke and actual357-clip bank checks. Candidate DLLDA0FCE9F4B7F78A08E92F82461E93AF9B1163D386DB9F27473FAC063A2313A59, unchanged probeBAFEB687A25126DDFCF385E971AB0F58A5A57F5E23E2D0A40976882085F0C6AA. Archive `work/movement-integration/candidate-npc-combat-v983` contains source, binaries, build/deploy scripts and identity. Deployment and runtime984 are not accepted until their actual receipts are checked.

Next test984 observes all bound relevant enemies rather than only one guard, requires remote follow and retained clone AI ownership throughout, records actual selection/hits, and restores the option and lab. Moving pursuit, repeated teleports/area boundaries, menu/checkpoint lifetime, enemy-to-player damage, sustained stability and campaign coverage remain open.

## Accepted scoped targeting and pursuit987

[Audit987](../../work/movement-integration/npc-perception-audit-v987.json) retains all three983 runtime cases. Test984 fails because its PowerShell `Array.Contains` comparison used different integer representations for the bound and requested handles, yielding an empty watched list. One native guard hit40→20 is independently recorded. This is not target-selection acceptance. Corrected985 uses PowerShell numeric membership and rejects fewer than three active enemies before testing; recording begins before placement. It passes10 samples in which native Ramsey/escape guard2 select the remote avatar while following remains on. Clone AI remains held.

Test986 adds one finite ordinary controller input, backward with sideways movement. The source travels approximately7m;15 samples after moving at least1m retain a native enemy target on the avatar. Both enemies pursue. No global hostility or visibility setting is changed. The original watched escape guard1 can continue remembering host1; multiple enemies are observed rather than forcing all AI to choose the same player.

Six inspected client frames show the approach, diagonal retreat, continued pursuit and close swings. FPS overlay decreases during active combat, which needs performance investigation. The source health UI remains unchanged because enemy hits still damage only its host-side clone; clone death eventually causes withdrawal. The existing enemy-to-player prototype requires an earlier1HP story avatar and a single guard, so it cannot simply be enabled here. General enemy contact routing is the next implementation task.

All three takes restore both local players to56 health, disable experiments and retain protected files. Complete movement, general AI/combat, arbitrary area transitions, menus/checkpoint continuity with this new option and separate-account Steam remain unaccepted. The983 option stays off by default. Runtime manifest now1397 includes the six new recordings with explicit acceptance limits.
