# 125. Stage 8: partner powers that touch the world (area 4.6, #242)

Each partner power that changes the world becomes something the host carries out. The design notes are in [partner-powers-design.md](stage8-step1-notes/partner-powers-design.md). Labels: confirmed static, confirmed runtime, hypothesis.

The world diff has no NPC positions, hit reactions, alert state, rats or fish. So each power also needs its own check in the take.

## Deadly Shade (candidate deadlyshade-v2069)

**Before:** the host spawned no puppet for the partner's deadly shadow (kind 2). A free-standing deadly shadow had crashed the host in `FUN_1404a3f90` (partner-shade-v1729c). The shadow's kills were forwarded, but the host's guards never perceived or fought it.

**How the game does it natively** (`FUN_14043e0c0`, `OnCloneSpawned`; confirmed static):
1. The cast's decoy is replaced by a clone of an NPC near the caster.
2. `FUN_14043de90` finds that NPC in the caster team's source list: `game+0x341068[team at player+0x2e0]`, then `FUN_1404e51c0`, which returns the nearest entry within its radius, first at the caster and then at the aim.
3. The clone definition is that NPC's component `0x39ae070` at +0x6d8.
4. The clone is created with that NPC as its parent.

**The change:**
- The host does the same at the partner's streamed shade, using its own player's team.
- `shadeHostDeadlyParent` gives the parent and the definition, and `shadeHostSpawn` passes the parent.
- Lab switch: `coop_shade_native deadly_off|deadly_on`.
- Counters: `shadeHostDeadlyParents` and `shadeHostDeadlyRefused`.

**Test:** `work/natural-moveset/Test-PartnerShade-v2069.ps1`, from v1738, on the local lab. With `-Deadly`, the host must spawn a puppet and a guard must die in both worlds. `-DeadlyOff` is the twin, where the host must spawn no puppet.
