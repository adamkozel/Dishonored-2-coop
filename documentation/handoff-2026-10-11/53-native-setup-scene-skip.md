# Native setup scene skipping

The user noticed routine resets repeatedly played a skippable cutscene. `work/PrepareShadowSword.ps1` restored the two isolated Dunwall checkpoints, approached Mayhew, and waited up to45 seconds for the scene. It never sent the native skip input. This was an avoidable harness delay, not a requirement of the movement tests.

## Change and exact scope

[The setup helper](../../work/PrepareShadowSword.ps1) now waits for both native games to advertise the same skippable scene and sequence boundary. It requires fresh connected Steam peers, enabled skip synchronization, valid sequence metadata, active Cinematic5 and no pending transition. It sends ordinary controller X for3000ms to the host once. The existing native skip callback and reliable Steam relay apply the skip to the client. Client input is not changed. The host input is released before repositioning.

The helper verifies one native source callback, one send, one peer application, no peer callback/send echo and no added dropped request. It writes a receipt to `outputs/d2-coop-lab/research/native-sword-setup-latest.json`, or a caller-supplied `-EvidencePath`. A scene that never meets the guard retains normal playback. `-PlaySetupScene` explicitly retains full playback for cutscene-specific tests. This is not a timescale change and does not bypass native scene completion or grant inventory directly.

## Runtime evidence680

- [Preserved original helper](../../work/movement-integration/native-setup-skip-v680/PrepareShadowSword-before.ps1)
- [Reproducible native test](../../work/movement-integration/Test-NativeSetupSkip-v680.ps1)
- [Result](../../work/movement-integration/native-setup-skip-v680/result.json)
- [Skip receipt](../../work/movement-integration/native-setup-skip-v680/skip-receipt.json)
- [Full-playback baseline](../../work/movement-integration/native-setup-skip-v680/before-full-playback-baseline.json)
- [After native skip](../../work/movement-integration/native-setup-skip-v680/after-native-skip.json)

Candidate676 passed the actual two-instance comparison. Scene handling took4619ms; the complete reset call took20857ms. The prior45-second timeout is not a measured duration and must not be used to claim a40-second saving. Both players returned to native Running0 with their original roles, complete inventory names/quantities, one native sword each, player health56 and partner health40. Starting positions matched exactly: host `[-33,-153,66.403648]`, client `[-33,-148,66.978355]`. Both peers were fresh with follow enabled and changed checkpoint epochs. All14 protected save/settings hashes matched. Original Steam assets were not changed.

The first preflight tried an unsupported player `--sword-equipment 1` query and stopped before resetting. Its failure is preserved separately. The corrected test uses established inventory and player-state queries. The later successful receipt does not erase that failed preflight.

This verifies the existing Mayhew fixture reset. It does not establish that skipping every campaign scene is equivalent for all scripts or that campaign synchronization is complete. The helper is used by ordinary isolated movement cleanup; dedicated cutscene tests retain their intended playback.
