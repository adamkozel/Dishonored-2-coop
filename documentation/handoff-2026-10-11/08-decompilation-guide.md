# Decompilation findings and source map

All addresses in this project refer to the exact executable SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Ghidra static image base is `0x140000000`. Convert a static VA to RVA by subtracting that base, then add the live module base. Do not apply these offsets to another storefront/build or confuse a vtable/data address with a function.

## Complete saved finding exports

- [Project bookmarks](generated/ghidra-bookmarks.md): full saved D2/co-op notes, with address, category and unedited confidence/scope language. Includes newer carry findings missing from the older findings journal.
- [Seed functions, labels and structures](generated/native-seed.md): the initial 31 named functions, 19 labels and seven partial native structures, preserved with their original confidence and source hashes.
- [Saved research transcripts](generated/decompilation/README.md): Markdown renditions of discovered Ghidra/decompilation/finding/disassembly records, with original path and SHA-256.
- [Function documentation export](generated/function-documentation.md): current function names, signatures, comments and local/label metadata for bookmarked addresses; failures are retained explicitly.
- [Current source address index](generated/source-addresses.md): address-like constants and source context from the active source tree. This is an audit navigation index, **not a list of confirmed native APIs**.
- [Fresh MCP verification](generated/live-ghidra-verification.md): actual lookup, xrefs and decompilation of `0x140734d90` during this handoff.

The five non-function/code-site lookup results and historical program-option caveats are explained in [documentation audit notes](12-documentation-audit.md). They remain visible in the generated audit.

The project has tens of thousands of function records, many inferred or partially analyzed. Exporting our findings does not mean the game has been completely decompiled. Analyst names are not recovered official symbols; minimum known struct layouts are not full class definitions.

## Key systems and confirmed observations

| Static VA / RVA | Observation | Confidence / limit |
|---|---|---|
| `140717d50` / `717d50` | Health ratio reads current health +0x20, virtual maximum at vtable +0xd8, divides only for positive maximum | Static decompilation and persistent tool-verification example |
| `140717950` / `717950` | Native health damage entry used for observation and attributed friendly-fire filtering | Runtime fixture evidence; not every damage/non-health path covered |
| `140780470` / `780470` | Native empty damage result constructor | Filter returns native zero-damage result |
| `140614b20` / `614b20` | Equip existing inventory entry, quantity gate and slot/component transitions | Static plus existing-equipment runtime; not general insertion |
| `140619380` / `619380` | Create equipment entity for an existing item entry under native preconditions | Initial inspection static; distinguish later authored inventory solution |
| `1406182f0` / `6182f0` | Restore quantities/equip against pre-existing inventory entries | Secondary unwind range was initially misleading; not item insertion |
| `1401c9630` / `1c9630` | Fixed-order MD6 parser | Correct declaration order resolves null animation/locomotion pointers |
| `14053c5b0`, `14053eef0`, `140540a00`, `14053e770` | AnimBasic lookup, playback, stop and state | Direct/streamed clone action tests; aliases do not imply full moveset |
| `140486d40`, `140486bc0`, `140486c50`, `140486b00` | Requested velocity, facing, stance and equipment | Native locomotion controls, exact-build guards |
| `140753c10` / `753c10` | Child reference-frame updates | Owned root control must preserve native attached sword updates |
| `14054de30` / `54de30` | Capture pose snapshot into animation recovery data | Carry floor-to-scene blend investigation |
| `140700fc0`, `140700ff0` | Recovery snapshot graph weight and binding | Source-timed paired pickup experiments |
| `140755870`, `140757830`, `140757550` | Retarget setup, start and stop | Scoped carried-body attachment; preserve native orientation convention |
| `140734d90` / `734d90` | Copy visible pose to rigid bodies and zero velocities | Native release xref at `140736dc5`; v27 fixes launched body |
| `140734a10`, `140736ef0` | Ragdoll request / animated physics state | Must respect sequence and ownership |
| `1408784b0`, `14087c590` | Native entity hide/show | v26 exact owned inventory-child holster visibility and cleanup |
| `140829030`, `14082a110` | Carry45 scene/body ownership context | Scene +0x24, corpse +0xf8, phase +0x110, guarded native reads |
| `140831fe0`, `140832770` | Drop46 scene/body ownership context | Corpse +0x38, phase +0x54; high/soft paths require separate validation |
| `1408a7650`, `1408ab380`, `1407885b0` | Character restoration, maintenance and loading switch | Fixed Dunwall role restoration before gameplay; not arbitrary-save compatibility |

For exact field types, complete notes and evolving interpretation, use the exported records and current code rather than this short table.

## Investigation method

Trace the native asset inheritance and event/animation declarations, then locate code entry points and xrefs. Correlate with guarded read-only runtime state from the exact executable. Only then introduce a bounded mutation with lifetime, identity, thread, pause/load and restore guards. Preserve a known-good source/binary/assets/saves set and the failed observations. For animation/equipment, independently inspect rendered/native transforms and actual video rather than relying only on the hook's own status.

Record a finding as static, runtime or hypothesis; include the build and fixture, source paths, counterexamples and untested cases. Save Ghidra after annotation. The initial `findings.json` is not a complete current journal: later work uses bookmarks and separate records. This handoff consolidates those sources without upgrading their confidence.
