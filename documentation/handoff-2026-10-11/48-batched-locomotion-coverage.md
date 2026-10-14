# Batched locomotion coverage646

The current pipeline is retained: live candidate635, protocol69, bank67/357. No new production DLL, clip or framework was introduced. [Native run645](../../work/natural-moveset/native-gait-batch-v645/result.json) records20 timed input stages on each character, both game views and one final checkpoint restoration. [Independent audit646](../../work/movement-integration/gait-batch-audit-v646.json) passes22 functional checks. [Test harness](../../work/natural-moveset/Test-GaitBatch-v643.ps1) and [audit code](../../work/movement-integration/audit_gait_batch_v646.py) are reproducible with a new take name.

The course covers armed idle, walking into running, stopping, immediate forward/back reversal, opposing diagonals, translation while turning, native holster, unarmed idle/walking/stopping, redraw and stationary armed recovery. Both source and receiver native sword states are independently checked. Steam stays connected, the bank stays active and settled body weight stays above0.9998. Source movement remains native; network pose owns remote root translation.

## Measurement limits

Corvo214 and Emily215 samples show mean contemporaneous root differences about0.039m and0.029m. Maxima are0.478m and0.830m. The one-metre test bound detects abandoned/wrong following; it does not certify ideal smoothness or latency. Different probe timestamps and deliberate interpolation contribute, but these measurements alone do not isolate causes.

This is one fixed Dunwall course. It does not finish every start/stop gait, sprint/crouched variant, terrain, arbitrary collision, combat interruption or complete movement. Both observer sheets were inspected: native bodies perform the sequence. Emily partly exits the left camera edge during some moving-turn/backward intervals, so those frames cannot establish complete visual acceptance. Fine feet, contact, grip and body acting remain polish for later animator work.

## Preserved and deferred

Full checkpoint inventories, both players/partners' health and14 protected save/settings hashes are restored. Editable bank construction scripts, source/donor clips and the current archive remain available. The user confirmed that functional integration comes first and a hired animator can polish the retained sources later.

[Failed setup643](../../work/natural-moveset/native-gait-batch-v643/result.json) is retained: the harness requested unsupported `coop_shadow_stream 60000`, so the ready check refused to start recordings. Supported continuous mode is `coop_shadow_stream on`; it renews bounded leases internally.645 corrects the harness, not production. The failure's inventory/health/protected-file restoration passed.
