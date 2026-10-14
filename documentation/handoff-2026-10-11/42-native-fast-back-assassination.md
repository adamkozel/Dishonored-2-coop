# Native fast rear assassination,604 development

## Existing pipeline and source proof

The user prioritizes functional coverage through the established authoring, native-state and Steam pipeline, deferring fine polish and further third-party co-op research. Verified breakout592 is preserved before this extension; see [chapter41](41-native-rear-choke-binding.md).

[Corvo596](../../work/natural-moveset/native-lethal-corvo-v596/result.json) and [Emily596](../../work/natural-moveset/native-lethal-emily-v596/result.json) each pass four source-observation/restoration checks. Ordinary RightTrigger250ms behind unique authored guard2 triggers `midnight/gameplay/player/base/assassination/assassination_fast_back.midnightscene`, subscene `Npc`,1333ms. Both traces show exclusive state13, `arkPlayerStateAssassination`, vtable RVA1c83468, default mode, physical stance0 and logic1. Each scene has four reciprocal bindings: the ordinary NPC, player1 and both owned player arms. Clock total+15c/local+160/start+164 has start0. Native source plays the lethal action and owns its outcomes. No state forcing or direct victim mutation is introduced.

The exact image hash remains `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Both tests restore checkpoint inventory/health and14 protected files. Source-only observations do not establish a remote animation or shared victim outcome.

## Candidate implementation

[Candidate604](../../work/movement-integration/candidate-lethal-v604/identity.json) extends the existing paired actor channel. The legacy `choke` field names remain for continuity: variants1–4 are rear-choke phases; variant5 is exact fast-back lethal. Protocol65 remains400 bytes. Variant5 requires its own exclusive native state13 rather than admitting it through the choke-state41 guard. Scene resource, duration, subscene, target identity and arm ownership are independently validated. Other assassination variants are refused.

[Builder604](../../work/natural-moveset/build_lethal_bank_v604.py) uses `paired-native-v1/paired_fast_back_tall-{corvo,emily}.bin`, the existing native Doppelganger third-person actor donors. Their original1.333-second duration matches the observed player scene. It resamples the existing whole-body transforms to120 samples plus the standard guard frame, discards source root tracks/events and appends one clip to bank65/353. All previous352 payloads remain byte-identical. Existing native skeleton layouts, helpers, sword attachment and network root remain in use. This requires no new authoring tool or retargeting pass.

[Source changes](../../work/movement-integration/implement_lethal_v604.py) are backed up in `work/movement-integration/lethal-pre-v604`. Native ownership, protocol, proxy smoke and actual353-clip bank tests pass. [Harness605](../../work/natural-moveset/Test-LethalNetwork-v605.ps1) independently checks native scene and sword state, Steam body playback, source-root following, recovery and unchanged receiver victim health; it restores the isolated checkpoints. Runtime verification remains pending until completed receipts are recorded. In particular, sword-equipped behavior is an explicit tested premise, not assumed proof.

No receiver victim animation, target attachment, damage, death/corpse or shared world outcome is implemented by this actor-only binding. No native animation gameplay events are replayed. The local first-person sequence and controls stay native. Visual quality and paired contact are separate from the numerical checks.

## Completed runtime605

[Receipt606](../../work/movement-integration/lethal-verification-v606.json) records ten passing checks on each role. Both native sword `equipped` and `started` are independently true during the owned native state13 scene; the explicit implementation premise is confirmed. Source-owned phase1333 drives the remote actor through Steam, the independent root follows native source movement, and ordinary standing playback returns afterwards. Receiver target health stays40 before/after each test, so cosmetic actor playback does not introduce a duplicate hit. Both checkpoints, inventories, player/partner health and14 protected hashes are restored.

The actual Corvo and Emily observer sheets show the whole-body sword action and recovery. The receiver's guard stays standing and independent. These are useful actor-only recordings, **not** completed paired kills, synchronized deaths or visual acceptance. Fine contact and pose polish remain deferred. The dashboard retains both native596 source recordings and both605 source/observer pairs, alongside the prior breakout tests; total1012 indexed videos after606 is a provenance count, not a completion percentage.

[Frontal observation607](../../work/natural-moveset/Observe-NativeFront-v607.ps1) completed on both roles and produced ordinary sword combat, not a frontal assassination. Both research assertions fail and both restoration checks pass. The traces expose a useful target-bearing combat reader gap addressed in [chapter43](43-target-bearing-sword-combat.md); no front-assassination binding is added.

## Targeted Ghidra work

The Ghidra project is the exact `Dishonored2.exe` at `/Dishonored2.exe`. Searching `arkPlayerState.*Assass` finds the class string at141c83650; cross-references include140863799 and1408637b5. Interior-address function lookup/decompilation returned no function. The established exact-PE parser identified unwind boundary140863760–1408639d5, also vtable slot68. Creating a function at the boundary reported that `FUN_140863760` already exists; no new function or rename was applied. Decompilation at that entry succeeded.

[Raw pseudocode](../../work/natural-moveset/native-lethal-research-v602/ghidra-140863760.txt), [xrefs](../../work/natural-moveset/native-lethal-research-v602/string-xrefs.json) and [PE boundaries](../../work/natural-moveset/native-lethal-research-v602/pe-boundaries.json) are retained. Candidate target handle+c4, branch kind+100 and flags+13c are **static hypotheses**. Pseudocode includes cleanup/state transition paths and calls to8022e0 with0/6. None of these guessed fields or native calls is used by the new reader; it relies on the runtime-confirmed scene ownership instead. Findings are saved as Ghidra Note bookmarks at141c83468 and140863760, and the project was saved.
