# Historical note: outputs/d2-coop-lab/analysis/void-tools/owned-partner-equipment.md

Original: [outputs/d2-coop-lab/analysis/void-tools/owned-partner-equipment.md](<../../../../outputs/d2-coop-lab/analysis/void-tools/owned-partner-equipment.md>)

SHA-256: `53452DD2E95CF13C30FA07816DAB008828CF88BFD4901FDF0EC656073C3410C4`; 3053 bytes.

````markdown
# Owned partner equipment (6C53)

The exact Steam B671F824 executable loaded the newly authored models/characters/def/d2coop/emily_partner.def through native declaration lookup. It inherits story Emily and overrides only inventory component jiGYw3 with the native Corvo tutorial sword collection/loadout. Native spawning creates an independently owned inventory and weapon.

Asset installation uses the pinned Void.Utils 1.2.0.0 library on work/asset-game, a physical 42.155 GiB game copy. The original work/lab-game/base remains a junction to Steam and is never an installer target. Start-D2Lab -AssetExperiment explicitly selects the physical copy. Original indices, executable, Steam DLL and eight original save/config files remain unchanged. Pre-experiment role profiles are in work/pre-partner-assets-profiles; prior DBA7 source/binaries remain archived.

Confirmed native APIs: RVA1ea30 resolves a declaration type from manager RVA24367e0; RVA3cc0 resolves a named declaration with default creation disabled. The native touchDecl caller verifies the three-argument ABI. Known Emily and custom Emily resolve; an absent name returns null. Lookups are restricted to the engine thread/gameplay and verified instruction prefixes, type name, definition vtable and exact resource name. Persistent Ghidra annotations and partner-asset-decompilations.json preserve the evidence. No borrowed inline inventory pointer is used.

Loader SHA256: 6C535EA2CB5C939F104BB924E57CD5943751DD4B989D6CA9BE4DB1F5A634ABB7.
Source/binaries/assets/scripts snapshot: work/native-partner-equipment-6C53.
Evidence snapshot: research/milestones/6c53.

Validated:
- Both owned swords: four local native equip/expiry/cancellation cycles, stable quantities and weapon handles.
- Bidirectional real player draw/holster/redraw over native Steam loopback, duplicate rejection, stream-only stop and fixed expiry.
- Independent render reconstruction: 7 Corvo and 8 Emily stable samples; maximum position errors 0.000006061 and 0.000008158 native units, basis errors below 0.000038.
- Either native pause and blocked rearm, either stream disable, duplicated shared checkpoint reload: both leases restore or retire exactly once.
- Real host death and real client death, both native death screens and coordinated retry: both old swords clean up once and new-world native inventories start holstered.

This is still a bounded Dunwall experiment with fifteen-second equipment opt-ins, not complete co-op. Hand grip, complete animation/aim/combat parity, general world/progression synchronization, native session UI and real two-account Steam sessions remain unfinished.

Grip investigation: historical DBA7 source/target phase-3 matrices match all 21 streamed joints across 2604 comparisons (maximum rotation element error 0.001079). A later render overwrite is not supported by this evidence. The first-person arm entity has a separately rotated world frame which the existing stream omits; testing model-frame conversion is the next hypothesis, alongside differences in finger rest poses.

````
