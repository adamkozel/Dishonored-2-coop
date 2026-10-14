# Void tools for the co-op lab

## Current status (2026-10-09)

The pinned library now also builds and installs guarded, reversible packs into the physical `work/asset-game` copy. Original Steam assets and the junction-backed `work/lab-game/base` are never installer targets. See [owned equipment](owned-partner-equipment.md) for the preserved 6C53 milestone and [Doppelganger investigation](doppelganger-investigation.md) for native clone rigs, action playback, Steam state replication, tests and current gaps. Finger-pose tuning is paused. The setup notes below describe the earlier read-only phase and its then-pending work.

## Initial setup history

The user-supplied Void Explorer and Void Installer 1.2.0.0 archives are extracted under `work/tools/void-tools-1.2.0`. Iggtex mappings 1.0.0.0 are in its `Workspace/Dishonored2/Settings`. Archive hashes, DLL hashes and observed runtimes are pinned in `toolchain-lock.json`. Explorer was launched successfully and its game catalog opened. Installer is extracted but has not been run.

Author documentation: [Explorer](https://www.nexusmods.com/dishonored2/mods/30?tab=description), [Installer](https://www.nexusmods.com/dishonored2/mods/31), [text resources](https://www.nexusmods.com/dishonored2/articles/15), [UI texture mappings](https://www.nexusmods.com/dishonored2/articles/21).

## Automated read-only inspection

Run `Read-D2Assets.ps1` with the installed Codex PowerShell 7 executable. It loads the pinned, unmodified `Void.Utils.dll`, verifies the game executable hash, searches merged native catalogs, and optionally exports selected resource bytes. It calls no installer or game-resource write API. All source index hashes are checked before and after. Output is constrained to `research/asset-exports/<batch>`; each batch has a manifest containing original resource names, keys, source file/offset, patch number and extracted SHA-256.

```powershell
& outputs/d2-coop-lab/Read-D2Assets.ps1 -NamePattern 'corvo.*(collection|loadout)' 
& outputs/d2-coop-lab/Read-D2Assets.ps1 -NamePattern '^ui/lib_character_choice\.(iggy|raw\.iggytex)$' -Export -Batch character-choice
& outputs/d2-coop-lab/Test-D2AssetRigAgreement.ps1
```

The GUI runs on installed .NET 6.0.36. Automation uses PowerShell's own runtime, recorded separately. This thin library wrapper already supplies targeted CLI search/export; rewriting the GUI tool is unnecessary for those operations. No author binaries or game resource exports should be included in a redistributed mod.

## Findings used in development

Confirmed from exact installed assets:

- Story base adds inventory under component key `jiGYw3`, inherited from `inventory_civilian.cpntinventory`.
- Corvo's story definition overrides that entry with `corvo_collection` and `corvo_tutorial_loadout`: one native NPC sword, maximum quantity one and initial quantity one.
- Emily's story definition has no inventory override. This corroborates the read-only runtime finding of eight civilian accessory entries and no sword equipment.
- The Corvo tutorial sword derives from the existing Emily clone sword entity. The native NPC sword component specifies `holster_wpn`, `right_hand_wpn` and locomotion equipment state `Sword`.
- The actual character rigs are Emily 219 joints, Corvo 215 and first-person right arm 69. The base civilian rigs with 212 joints are not substitutes for those character rigs.
- `Test-D2AssetRigAgreement.ps1` independently compares Void Explorer's decoded skeletons with the preserved runtime observations. Four rig comparisons, 572 joints and all names/parents pass; maximum quaternion/translation difference is 2.663879339337427e-9. This validates our skeleton reader, not the retarget algorithm or visual sword grip.
- Native main-menu, save, Corvo/Emily pause and character-choice IGGY files and textures are exported. Character choice contains the existing `com.bsp.characterChoice:CharacterChoiceScreen`; main-menu assets contain `com.bsp.main:MainMenuButton`. These are candidates for native session UI work, not implemented co-op buttons. Texture mappings are installed; texture preview decoding has not yet been validated.

Ghidra now contains saved inventory lifecycle annotations and matching raw decompilation evidence under `../ghidra/inventory-lifecycle-*`. Static code and assets agree that the collection is populated during initialization, sorted and referenced by later equipment entries. A safe, owned pre-initialization inventory override remains a hypothesis to test. Do not append items to a live sorted inventory or borrow another entity's transient inline declaration without proving lifetime.

## Preservation at the initial read-only milestone

Explorer's workspace and all exports are separate from game files. No asset patch was installed, no game loader was rebuilt/deployed, and both running instances retained native Steam connection state 3 and gameplay state 1. Baseline validation passed for the eight original save/config files, exact Steam executable and Steam DLL, and staged loader.

`work/lab-game/base` is a junction to the Steam installation and is **not an isolated asset-mod target**. Before any Installer experiment, create a genuinely isolated asset target with its own writable index/patch files; never select the current lab junction or original Steam directory as an install destination.

Historical next step at that milestone: use these authored declarations to implement only the independently owned Emily counterpart's inventory; validate spawn/equip/unequip/reload cleanup in the bounded fixture. For grip, test retarget calibration or native Sword animation state using the decoded rig evidence. Native UI behavior, full gameplay synchronization and two-account Steam sessions remain unfinished.

