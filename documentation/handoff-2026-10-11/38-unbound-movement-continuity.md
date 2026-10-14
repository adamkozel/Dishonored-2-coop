# Movement continuity while an action binding is unfinished

The user explicitly ended further third-party co-op investigation and asked to accelerate the established pipeline, with cosmetic polish later. This change addresses a shared blocker: missing cosmetic action data used to retire an otherwise healthy movement stream after250ms. It does not count a fallback as a completed animation.

## Candidate558

Preserved previous working candidate550. [Candidate558 identity and complete source archive](../../work/movement-integration/candidate-unbound-presentation-v558/identity.json); [pre-change source backup](../../work/movement-integration/unbound-presentation-pre-v558). DLL `5E1E2EC5BC3961B463A1A66635DE23D04B9639028F542FE0A5BCAC8A23A4DCDC`. Probe remains `EFDD57EB2F79A2023BD18E63DDA8729E732C9D263D882639054D225B4CC0A54B`. Semantic wire59, unchanged336-byte layout. Existing bank59/336 clips and native assets are unchanged.

When a specific action reader fails, the source may emit explicit presentation state18. [The separate reader](../../outputs/d2-coop-lab/src/native_presentation_fallback.h) revalidates native default standing/crouching motion, actual ground class, game/player ownership and stable native sword equipment. It uses the existing native motion classifier; no additional native state classes are guessed. It reads both state/equipment snapshots again and rejects replacement or disagreement.

The source constructs a fresh packet, discarding any partially collected gesture, target, effect, throw or traversal data. State18 can carry only verified equipment state and ordinary ground presentation. The receiver keeps its existing root interpolation and full-body locomotion bank. It still rejects stale/wrong-world/invalid-owner data. Alternative power modes, airborne states and stunned states are not converted to ground. Timeouts, local first-person controls, gameplay outcomes and friendly-fire policies are unchanged.

The exact known action readers remain preferred. When a supported action resumes, its existing animation resumes on the normal event timeline. New `unboundSamples` and `unboundRefused` status counters expose fallback usage. This allows continued play and helps locate unfinished action bindings without mislabeling them complete.

## Verification

[Build script](../../work/movement-integration/Build-UnboundPresentation-v558.ps1) passed ownership tests, action/protocol tests, proxy smoke and both actual animation banks. New tests cover valid standing/crouching policy, rejected unknown classes, paused/alternate/air/stun modes, failed ownership reads, explicit state18 timeline handling, old-peer refusal, and rejection of attached action/effect/air payloads.

[Runtime test559](../../work/natural-moveset/Test-UnboundPresentation-v559.ps1) batches Corvo/Emily and standing/crouching. A bounded6000ms diagnostic command suppresses only the cosmetic equipment binding while normal native X input holsters the sword. It verifies the explicit Steam fallback, continuous body weight, native equipment settlement, four movement directions and recovery to the normal authored draw. The diagnostic is off by default and retires on stream stop. No synthetic native gameplay phase or damage is invoked.

The first559 attempt was stopped by the lab command whitelist before exercising the feature; the helper was updated for the bounded command. [559b](../../work/natural-moveset/unbound-presentation-v559b/result.json) exercised Corvo standing successfully, then hit a test assertion that mistook a retained animation ID at zero blend weight for an active gesture. It recorded31 accepted full-weight fallback samples; the completed checkpoint restoration and protected-file check remain in the failed receipt. The corrected assertion checks active blend weights. Neither failed attempt is relabeled as passing.

Runtime completion, video review and remaining limitations must be recorded from the final559c receipt. Full movement, visual acceptance and playable co-op remain unfinished.

## Completed runtime result559c

[559c](../../work/natural-moveset/unbound-presentation-v559c/result.json) passes all16 checks on candidate558. Both-role standing/crouching missing-binding cases preserve full body weight and Steam readiness; normal authored draws resume and four holstered directions remain functional. Both checkpoints and all14 protected save/settings hashes restore. Actual Corvo/Emily temporal sheets were viewed and their two recordings are indexed in the dashboard. The failed559b recording is also retained with a failed/unreviewed label. Continuous visual quality is not certified. [Consolidated receipt560](../../work/movement-integration/unbound-presentation-verification-v560.json).

Next: continue broad bindings with existing authored clips. Cosmetic polish remains deferred. No new pipeline, asset generation or third-party code was introduced by this change.
