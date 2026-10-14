from pathlib import Path
R=Path(__file__).resolve().parents[2];f=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=f.read_text(encoding='utf-8-sig')+'''
## Partner picker candidate, v211

[Native picker decompilation](../../work/ghidra-domino-picker-v210.json) identifies447250 as the candidate-return function.4478c0 calls it before comparing/replacing equipment+c8 and creating native aim FX. Returning null for the exact current co-op partner therefore uses the native no-target path, rather than the later insufficient-mana path447460. Other returned actors remain untouched. Native ranking and ray queries still run once. This does not choose enemies behind the excluded partner or modify the native candidate list.

Ghidra decompilation and raw bytes succeeded, but its disassembly/context endpoints returned empty/no instruction for these functions, and its xref index omitted the direct caller. [Read-only original-disk Capstone check](../../work/domino-picker-disk-disassembly-v211.json) independently confirms the exact executable hash,18 complete non-relative bytes at447250 and direct call4478e4. Capstone5.0.6 comes from the already bundled Ghidra wheel, extracted only to `work/tools/capstone-5.0.6-local`, with its wheel hash recorded. The Ghidra limitations are preserved rather than treating absent xrefs as absence of callers.

[Identity helper](../../outputs/d2-coop-lab/src/domino_partner_identity.h) checks current local Emily equipment, selected inventory slot, quantity, started flags, reciprocal component/entity registration, and the current registered partner generation before/after reading. [Synthetic tests](../../outputs/d2-coop-lab/src/domino_partner_identity_tests.h) cover ordinary guards, other components, inactive/deselected equipment, pending world changes and recycled/racing partner handles. The [native wrapper](../../outputs/d2-coop-lab/src/domino_partner_filter.h) calls the original picker exactly once, then returns null only for that proven identity. It leaves unrelated actors unchanged and records failures/costs. It does not invoke link or damage methods.

[Archived candidate211](../../work/movement-integration/candidate-domino-partner-v211/identity.json) passes identity tests and proxy smoke, with body44/bank47/world1 unchanged. The first isolated native partner capture is v212; runtime acceptance requires its final receipt and a separate normal-guard regression. Candidate creation alone is not proof.
'''
f.write_text(s,encoding='utf-8')
