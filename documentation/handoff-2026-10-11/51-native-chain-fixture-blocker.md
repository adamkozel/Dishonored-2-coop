# Native chain fixture: bounded runtime result659–661

The original chain definition resolves in both games, but spawning it in the retained Dunwall tower does **not** provide a working climbing fixture. No climbing animation or remote climbing binding was accepted. Continue functional movement elsewhere rather than repeating this spawn or forcing the player into state29.

## Preserved builds and procedure

- [Validated prior651 archive](../../work/movement-integration/candidate-auto-crouch-v651/identity.json) remains intact with its357-clip banks and36 scoped automatic-crouch checks.
- [Query659](../../work/movement-integration/candidate-climb-query-v659/identity.json) adds a fixed, read-only declaration lookup to `coop_decl_probe`. Both [host](../../work/movement-integration/candidate-climb-query-v659/query-host.json) and [client](../../work/movement-integration/candidate-climb-query-v659/query-client.json) return the exact original chain declaration and spawnable flags48. Lookup does not reparse or modify an asset.
- [Fixture660](../../work/movement-integration/candidate-climb-fixture-v660/identity.json) adds the opt-in command `coop_climb_fixture spawn`, restricted to the physical isolated executable, exact tower map, active finite movement fixture and grounded player. One creation attempt is allowed per world. Native API prologues are pinned. Creation never replaces the co-op partner identity or forces a player state.
- [Reproducible test661](../../work/natural-moveset/Test-NativeClimb-v661.ps1) creates at(-33,-151.8,70), records ordinary X, forward/backward and B inputs, then resets the checkpoint. It records the loaded definition, entity, component lists,136 source-state samples and an actual source-game video.

660 DLL SHA256: `F88ED4CC3D897CE1CF62F4AE6AA54014DF5B338075264DD8FCFAB88D8501492B`. Probe unchanged: `438912670E027B5078386F554CB581B255CC5845A20628EC399215E326A8FEE9`. Protocol69 and bank67/357 unchanged. The creation API itself was exercised successfully; that does not establish usable climbing.

## Confirmed runtime observations

[Fixture receipt](../../work/natural-moveset/native-climb-corvo-v661/fixture.json) reaches native initialization, handle210178569 in that world. RTTI is `arkClimbable`. [Components immediately after creation](../../work/natural-moveset/native-climb-corvo-v661/chain-components.json) and [after the input course](../../work/natural-moveset/native-climb-corvo-v661/chain-components-after.json) contain only `arkCpntAudio` (VT RVA1c4e588) and `arkCpntAnimBase` (1b7f408). The entity has a skinned render model but no reference-frame pointer at+358. Neither required physics-system nor climbable component appears in the live component list.

[Source observations](../../work/natural-moveset/native-climb-corvo-v661/observations.json) remain in the default mode:101 samples Running0,2 Running0+Undraw81 and33 Sneaking6 after B. No Climb29, climb-mode entry or climb attachment was observed. [Actual source video](../../work/natural-moveset/native-climb-corvo-v661/source.mp4) and [inspected contact sheet](../../work/natural-moveset/native-climb-corvo-v661/source-sheet.jpg) do not show a usable chain or climbing prompt. The peer avatar can obstruct part of the view; component absence and state observations provide the stronger result. No complete-body visual acceptance is claimed.

[Result receipt](../../work/natural-moveset/native-climb-corvo-v661/result.json) passes three **execution/restoration** checks, not a climbing behavior test. The checkpoint removed `d2coop_climb_fixture`; both complete inventories, player health56 and partner health40 were restored, and all14 protected save/settings hashes matched. No original game asset or Steam DLL was changed. Both test games remain running in their normal restored fixture.

## Exact-image findings and unresolved dependency

Ghidra function1405be480 returns `entityDef+58`. [Native entity creation1408a78f0](../../work/movement-integration/climb-fixture-v657/decompile-1408a78f0.txt) checks bit20 at the returned descriptor+98, hence declaration+f0. It fatally rejects unspawnable classes. The query checks this flag before any create call. The targeted decompilation and bookmark were saved to the persistent exact-executable project.

The [original declaration export](../../outputs/d2-coop-lab/research/asset-exports/climb-fixture-v657/manifest.json) references `components/characters/base/physicssystem_chain.cpntphysicssystemevolved` and `components/interactive/chainclimbable.cpntclimbable`. Earlier all-catalog searches did not export either. The runtime result proves this creation route did not instantiate them. It does **not** prove why: a deprecated source definition, map-supplied overrides or missing compiled dependencies remain hypotheses. Obtain an actual campaign climbable with its effective components before another climb binding experiment. Do not invent those structures, equate a decorative chain with a climbable, or bypass native physics.

Fine animation polish remains deferred at the user's request. Next priority is a batched native pickup/food/readable observation using existing tower objects and the current movement/Steam pipeline. Swimming still requires a real native fixture, and the complete movement/co-op phase remains unfinished.
