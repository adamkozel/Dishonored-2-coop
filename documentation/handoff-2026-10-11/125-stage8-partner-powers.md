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
| `deadlyshade-dunwall-v2073a` | deadlyshade-v2070 | Harness: no cast (`sent 0`; the partner's world had no shade). **Cause (confirmed runtime, VM screenshot):** at the start of A Long Day in Dunwall the partner has no Mark and no mana arc, so no power can be cast; host Corvo's avatar also stood in her view. Diffs equal (538) |
| `deadlyshade-dunwall-v2073b` | deadlyshade-v2070 | The same (twin route) |

**Next take: `deadlyshade-eotw-v2074`**, on Edge of the World, where the partner has the Mark and mana. Harness v2074 adds `partnerface`: the partner stands 6 m behind an NPC of her own world, found by spawner name through `--npc-activity`, and faces it. She casts behind `addermirestation_entrance_guard_02`.
| `deadlyshade-eotw-v2074a` | deadlyshade-v2070 | **First real cast.** `partnerface` put the partner 6 m behind `addermirestation_entrance_guard_02` (stand (53.07, 22.04, 19.0)). Her deadly shadow reached the host as kind 2, present (event 1). The host refused it 814 times with `deadly_no_parent` (confirmed runtime): its own player team's source list had no entry near her. It also retried every frame. Diffs equal (2137) |
| `deadlyshade-eotw-v2074b` | deadlyshade-v2070 | Stopped at the spawn diff, before the twin's cast: breakable `env_bottle_04_19` gone on the host, present for the partner, before anyone moved. **Open, hypothesis:** physics at load; the step 6 runs watch it |

**Candidate deadlyshade-v2075:**
- When the team list has no entry, the nearest world NPC within 15 m of the partner's root becomes the parent.
- The host tries once per 500 ms.
- `.shade-host.json` gains `deadly`, `deadlyParents`, `deadlyNearest` and `deadlyRefused`.
| `deadlyshade-eotw-v2075a` | deadlyshade-v2075 | The partner's deadly shadow reached the host again (kind 2, present). The nearest-NPC fallback chose a candidate 27 times; each had no clone definition, so `deadly_no_parent`. Diffs equal (2278, 2277) |
| `deadlyshade-eotw-v2075b` | deadlyshade-v2075 | Twin: with `deadly_off` no host attempt; diffs equal |

**Finding (confirmed static, plus a hypothesis):**
- Component `0x39ae070` is only referenced by `OnCloneSpawned` and by its type-descriptor getter (`0x1408bab70`). By its strings it is most likely `arkCpntPowerClone`: hypothesis.
- The team source list that `FUN_1404e51c0` searches therefore holds the player team's own members. Natively, the deadly shadow is the caster's own clone, parented to the caster. No guard has the component.

**Candidate deadlyshade-v2076:** when the list has no entry near her, the host's own player is the parent, and its clone definition is used. The spawn is at her shade.
| `deadlyshade-eotw-v2076a` | deadlyshade-v2076 | Stopped at the spawn diff, before the cast: `env_bottle_04_19` absent on the host (#260) |
| `deadlyshade-eotw-v2076b` | deadlyshade-v2076 | The same |
| `deadlyshade-eotw-v2076c` | deadlyshade-v2076 | Spawn diff not required. The cast reached the host (kind 2). With the host's player as parent, 28 refusals: Corvo, without Doppelganger, has no clone definition. The diff after the cast differs only by the same bottle, which stays absent on the host |
| `deadlyshade-eotw-v2076d` | deadlyshade-v2076 | Twin: no host attempt; every diff equal (the bottle was present this time) |

**Candidate deadlyshade-v2077:** with no clone definition, the deadly shadow's own definition (`deadlyshadow_entity.def`) is created parented to the host's player, not free-standing. Counter: `deadlyOwnDefinition`. Its take records the diffs without requiring them and is judged on `.shade-host.json` and on no crash.
| `deadlyshade-eotw-v2077a` | deadlyshade-v2077 | **Host process lost before the cast:** the host's process was gone at step 7, a wait after `partnerface`. There is no Windows crash record for the game and no error in its engine log. Unexplained, recorded as a host loss. Spawn diff equal |
| `deadlyshade-eotw-v2077b` | deadlyshade-v2077 | Twin: the cast reached the host (kind 2) with `deadly_off`, so no host attempt. Every diff equal (2279, with NPCs compared by the fixed diff tool) |

**Next:** `deadlyshade-eotw-v2078a` on fragile-v2078, which carries the v2077 code, after the #260 bottle takes.
| `deadlyshade-eotw-v2078a` | fragile-v2078 (v2077 code) | **The host process was lost again at the cast.** The harness records completed steps only, so v2077a's loss was at the cast too, not before it. That makes 2 of 2 host losses when the host spawned `deadlyshadow_entity.def` parented to its player, and none in the twins. **Conclusion:** the deadly shadow's own definition crashes the host when parented to its player, as it did free-standing |

**Candidate shadedecoy-v2079:** on a host without a clone definition, the partner's deadly shadow gets the proven decoy puppet (`decoy_entity.def`, free-standing). The host's guards perceive and fight it, and her shadow's kills are still forwarded. This is the research's fallback.
