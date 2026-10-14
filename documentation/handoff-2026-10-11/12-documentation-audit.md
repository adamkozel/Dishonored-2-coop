# Handoff audit notes

The generated link checker verifies local Markdown links outside fenced source transcripts. See the latest [generation audit](generated/generation-audit.md) for exact counts. The source manifest hashes the files actually read by the generator; the much larger evidence index lists paths and sizes without claiming that all large telemetry was rehashed.

## Function metadata export limitations

All 69 unique addresses from the initial bookmarked-function export were queried. 64 returned function documentation. Five preserved lookups returned no function:

- `141c85948`, `141c79cf8`, `141c86448` are documented native state **vtables**, so a function-documentation request is inapplicable. Their complete research notes remain in the bookmark export.
- `140828845` and `14082888f` are native code-site bookmarks for carry research. The current Ghidra function map does not resolve a containing function at those locations. The direct lookup also returned no function. Their notes and related native code evidence remain preserved; the handoff does not invent a function prototype or silently repair analysis boundaries.

These are explicit export limitations, not missing bookmark findings. Later carry/throw findings are also included in the final bookmark export. The exact-function lookup/xref/decompilation verification at `140734d90` succeeded separately.

The category-listing endpoint returned no types for `/` and `/D2Coop/ConfirmedPartial`, but a direct `D2_` name search confirmed all seven initial partial structures plus the 24-byte `D2_AnimBasicInputData` and their pointer types. Both the empty category result and successful search are preserved as source transcripts. An empty category response must not be interpreted as lost project types.

## Historical program options

The Ghidra `D2Coop Research` options were exported in full. Their early hypotheses predate the native authored-equipment and Doppelganger milestones; do not interpret “no equipment creation validated” there as the current state of every later inventory experiment. The generator preserves the original wording for audit, while current-state and history chapters explain chronology.

## Preservation check

[Fresh integrity verification](../../work/handoff-integrity-verification.json) records the current14 protected original save/config comparisons, original and isolated executable hashes, and deployed candidate/probe identities. Read its timestamp and actual pins; earlier v30 checks are historical. This check is read-only and is distinct from historical baseline reports; it does not rerun the entire mod regression suite.

The handoff generator writes only its documentation subtree and removes only orphaned generated Markdown pages inside explicitly resolved managed subdirectories. Original sources, saves, tool installations, Ghidra database and large evidence files are retained.
