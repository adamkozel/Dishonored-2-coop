# Release payload classification (#30)

The release must not redistribute game files. This folder records where each game-derived payload comes from, so the installer can rebuild it from the player's own install. All findings are confirmed static unless a file says otherwise.

| File | Content |
|---|---|
| `bank-clips.json.gz` (gzip) | Every clip of the shipped movement banks (`use-banks-v1212`, 374 clips per role), traced through 77 bank manifests to its sources. Each clip records its category (`native-sampled`, `mixamo` or `authored`), donor files, native assets, Mixamo sources and whether it embeds the game's bind translations. |
| `chain_table.md`, `chain_table.json` | The bank chain from `D2MOVE02` (`native-gaits`) to `D2MOVE71` (`use-banks-v1212`): manifests, builders, the clips each version added, and parent hashes. |
| `bank-tools/` | The read-only scripts that built the two tables. They were run once from the session scratchpad, so their paths point there. |
| `decl-match.json` | Each of the 45 decls in `game1voidmod.resources`, matched against the game's decls of the same type by line Jaccard and `difflib` ratio. |

**Movement banks:**
- Category counts are the same for both roles: 291 native-sampled, 65 Mixamo and 18 authored.
- Every frame stores the game skeleton's bone lengths.
- Plan, posted on #30: ship bitwise XOR patches against bases the installer rebuilds from the player's own files.

**Decls:**
- The prop defs, `emily_partner.def`, `windblast_remote.fx` and the small animtrees and components are ours.
- The rest are edits of game decls. Examples:
  - the doppelganger defs (from `deadlyshadow_entity.def`);
  - `muzzle_remote.particledt` (2 lines changed in 5068);
  - the carry, mesmerize, swarm-rat and doppelganger scenes and effects;
  - `doppelganger_despawn_remote.fx`, which is identical to `decoy_despawn_01.fx`.
- These ship as recipes: line ranges copied from the player's decls plus our inserted text.

**Indexes:**
- `game2/3/4voidmod.index` are byte-identical to the player's `game2/3/4_patch.index`.
- `game1voidmod.index` is `game1_patch.index` with our 45 entries appended: uncompressed, contiguous from offset 4 of `game1voidmod.resources`, flags2 28.
- `master.index` is the original with:
  - `gameN_patch.index` renamed to `gameNvoidmod.index`;
  - the patch count raised from 24 to 28;
  - four `gameNvoidmod.resources` entries (with their game numbers) appended.
