# Evidence, provenance and interpretation

The project deliberately preserves narrow successes, failed experiments and historical implementations. [Milestones](generated/milestones.md), [recordings](generated/runtime-recordings.md), [research evidence](generated/evidence-index.md), [source transcripts](generated/historical-notes.md) and [file manifest](generated/source-manifest.json) are the audit entry points.

## Evidence chain

The game executable hash identifies the reverse-engineering target. The proxy hash identifies compiled runtime behavior. Probe hash identifies independent reader behavior. Bank/source/asset hashes identify authored material. Harness hash and controller events identify the stimulus. Raw observations identify what was measured. Video hash and measured capture rate identify visual evidence. Validator version/source identifies the criteria. A report without this chain may still be useful history, but should not be attributed to a newer build.

An archive can contain a source snapshot, binary, installed resource hashes, isolated profiles and reports. These have different lifetimes. Never infer that a new source edit is present in an old DLL, or that changing an authoring bank requires a different DLL. Preserve separate version identities.

## What checks prove

- A stream-ready or lifecycle check proves only the recorded protocol/ownership condition.
- A transferred matrix matching a formula proves transfer accuracy, not a natural pose.
- A stable sword socket does not prove closed fingers, correct hilt fit or absence of clipping.
- A native first-person source action is not proof that the remote actor performed it correctly.
- A rendered preview is authoring evidence, not D2 integration.
- A D2 preview clip is not automatically game-state-bound or authoritative gameplay.
- A thresholded shoulder/wrist target is a diagnostic, not accepted mesh contact.
- A passing native effect allocation/lifetime test is not complete power outcome synchronization.
- A short test at one canonical checkpoint does not prove general campaign recovery.

## Sampling and validator corrections

Native processes update asynchronously. Status JSON is a sampled publication; packet state, interpolated animation state and separately read skeletons need not refer to the same instant. Bracket reads, use final publication evidence where available and explicitly report non-atomic pairs. Reject inconsistent reads rather than silently widening tolerances.

Two recent examples are preserved:

1. In the v29 Emily carry test, delayed presentation still played gather while the latest received packet had already advanced to hoist. Checking those in the same row was invalid. The validator now separately checks native preparation transmission and gather-before-hoist playback ordering. The prior failed validation is retained.
2. The first Corvo v30 capture missed the received preparation interval during slow native reads/attachment setup. That take remains failed. A new harness captures received state earlier in each iteration, and a fresh recording passes. This did not retroactively create missing evidence or relax the runtime implementation.

## Capture rates and visual review

Use measured frame timestamps/rate. Earlier authoring proof videos at around 12 Hz encoded in 30 FPS containers must retain that distinction. Recent native captures request 30 Hz and measure roughly 26–27 Hz. Game FPS counters, transform update cadence and capture FPS are three different measurements.

Temporal contact sheets support inspection at selected moments but do not alone establish fluidity of an entire sequence. Full-speed video and useful camera views remain necessary before final acceptance. Keep `accepted:false` when hands, posture, transition quality or context coverage is unresolved.

## Snapshot consistency

The handoff generator hashes sources as it reads them and records generation time. Active development continues, so this folder is a dated snapshot, not a permanently live database. Regenerate the generated sections after meaningful work and update the authored current-state/current-session chapters. Preserve per-take immutable evidence and avoid rewriting historical claims.
