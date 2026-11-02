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

| Take | Candidate | Result |
|---|---|---|
| `partner-shade-v2069a` | deadlyshade-v2069 | Harness fail: her shade never appeared. Guard 1 now stands at (-57.9, -144.5, 61.5) facing the stairs, so the stand 6 m behind it put her against a wall and no cast happened. The restore found the client's checkpoint inventory changed. The lab was recovered (`Recover-InterruptedTake-v1055`, into v2069b) |
| `partner-shade-v2069b` | deadlyshade-v2069 | Harness fail: observer pitch did not settle (lab left dirty by v2069a) |
| `partner-shade-v2070a` | deadlyshade-v2070 | Harness fail: observer pitch did not settle; the cast video was not finalised. The restore passed |
| `partner-shade-v2070c` | deadlyshade-v2070 | Harness fail: from the fixed fallback stand with a clear view, still no shade (doppelganger world `sent 0`). The local shade harness no longer reaches a cast; the cause, for example the partner's mana, is not known |

**Candidate deadlyshade-v2070:** looks up the deadly shadow's parent NPC at the partner's root first, then at her shade, as `FUN_14043de90` does at the caster and the aim.

**Next take: `deadlyshade-eotw-v2071`** on two Steam accounts (harness v2071 with a `status` step).
- The VM partner gets Doppelganger with Deadly Shade and casts at checkpoint guard 01 from v1833a's front stand.
- The host's `.shade-host.json` is saved, and the diffs after the cast and at the end are required.
- The twin sets `deadly_off`.

| Take | Candidate | Result |
|---|---|---|
| `deadlyshade-eotw-v2071a` | deadlyshade-v2070 | Harness fail as a shade test, though it "passed": no cast happened. The partner slid off the v1833a stand (z 15.97 to 13.98) to the foot of the checkpoint wall, faced the wall, and her doppelganger stream sent 0. Her HUD shows mana. Diffs equal (2279) |
| `deadlyshade-eotw-v2071b` | deadlyshade-v2070 | Not a twin (no cast). The diff after the cast differs in 2 values: `script_spawner_checkpoint_reinf_01` and `_02` alive on the host, absent for the partner. These are checkpoint reinforcements that spawned in the host's world only, with no power involved. **Open, a hypothesis to check in the step 6 runs:** a reinforcement spawn reaches only the host's world |

**Next take: `deadlyshade-dunwall-v2072`.** The partner casts at the start of Dunwall, from partner-shade-v1736b's stand toward guard 1.
| `deadlyshade-dunwall-v2072a` | deadlyshade-v2070 | Harness fail before the cast. The partner was set at partner-shade-v1736b's stand (-55.4, -144.7, 61.0), which is not streamed at the start of Dunwall: she fell to z -515 and died, and the shared GameOver ended the host. Restore passed |
| `deadlyshade-dunwall-v2072b` | deadlyshade-v2070 | The same (twin route) |
| `deadlyshade-dunwall-v2072c` | deadlyshade-v2070 | The same, with the host health hold on (a shared death all the same) |
| `deadlyshade-dunwall-v2072d` | deadlyshade-v2070 | The same (twin route) |

**Next take: `deadlyshade-dunwall-v2073`.** She casts from her spawn in the tower toward the NPC 9.5 m away, at (-34.0, -160.6, 66.6).
