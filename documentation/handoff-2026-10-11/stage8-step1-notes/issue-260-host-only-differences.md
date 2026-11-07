# Issue #260: differences that appear only in one world

Read-only research, 2026-11-02. Labels: confirmed static, confirmed runtime, hypothesis.

## The bottle `env_bottle_04_19`, Edge of the World

**What "absent" means** (confirmed static): `readNativeBreakable` failed (`native_breakable.h:20-33`). A bottle is an arkMovable with one step, and its entity is removed at once on its final step (`breakable_sync.h:108-109`). So a smashed bottle shows as `<absent>`. The host has 1268 breakables against the partner's 1269.

**Evidence** (confirmed runtime):
- The bottle is absent in every host sample of deadlyshade-eotw-v2074b, v2076a, v2076b and v2076c, and 0 in every VM sample. Nothing else differs among the 1269 breakables.
- Bottles broken during play go absent in both worlds (v2074a, v2075a, v2076c/d). So the sync works for breaks caused by damage.
- The bottle stands at (0.19, 165.48, 4.14), beside the interrogation-room door.
- In door-authority-v2046b, that door's swing knocked the bottle over in one world only.

**Why the sync misses it** (confirmed static):
- A vanished entry is sent only after a damage hit noted within 5 s (`breakable_sync.h:111`).
- The catalog is built only once the sync arms, so a break before the first scan is never sent.

**Hypothesis:** a prop-rest teleport (`prop_rest_sync.h`) of the fragile bottle smashes it in the host's world at load.

**Candidate fragile-v2078:**
- Prop rest leaves breakable props alone (`breakableSkips`).
- On the host, a vanished breakable entry is sent as broken without a noted hit.

**Test:** `Run-Bottle-v2078.sh`, three takes with the spawn diff required.

## The checkpoint reinforcements, Edge of the World (deadlyshade-eotw-v2071b)

- `script_spawner_checkpoint_reinf_01` and `_02` are alive in both worlds at spawn. They vanished from the partner's world afterwards: 139 NPCs against 141 in all three "after" samples (confirmed runtime).
- No ActionSpawner or reinforcement event appears in either world's Kiscule events, so it is not the host-story mirror (confirmed runtime).
- The cause is unknown. Hypotheses: a native despawn in her world, or a stream removal (RMV1/ASH1).
- Diagnosis first: keep `.npc-stream.json` and the VM's world NPC list on that route.

## The world diff dropped NPCs (fixed 2078)

When one digest sample's NPC catalog read failed (`npcs {}`), `d2_world_diff.py` marked every NPC key unstable and silently left NPCs uncompared: "compared 2138" instead of 2279 (confirmed runtime: v2071b spawn, v2076b spawn, v2076c after).

**The fix** (`stable_view`): a sample whose category is empty while another sample of it has keys is skipped and counted as `failedSamples`. Test `test_failed_sample_is_skipped` passes, and the twin `empty_sample_unstable` is caught.

**Re-diffing the archived digests with the fixed tool** (written to the scratchpad; the evidence is not rewritten):
- farguards-eotw-v2067a and v2068a stay equal with NPCs compared (2275 to 2279).
- v2071b's two missing reinforcements are confirmed.

## Bottle takes on fragile-v2078

| Take | Result |
|---|---|
| `bottle-eotw-v2078a` | Setup fail: co-op saves did not switch on |
| `bottle-eotw-v2078b` | **Equal at spawn and 30 s later** (2279, NPCs compared). Prop rest skipped breakables 1956 times; host sent 2 rests, partner 2 |
| `bottle-eotw-v2078c` | **Equal at spawn and 30 s later** (2279) |

Two equal spawns in a row, against 3 of 4 failures before: good evidence for the prop-rest cause, not yet conclusive. The step 6 runs keep watching it.

## The reinforcements again, 2163 to 2166

**Reproduced** (confirmed runtime).

- `Run-V2163R.sh` ran v2071b's route three times on progress-v2162. `reinf-eotw-v2163a` showed the difference again; v2163b and c were equal.
- In v2163a, `script_spawner_checkpoint_reinf_01` and `_02` were in both worlds at spawn. At the after diff they were in neither (2277 values compared). At the end they were alive in the host's world and absent in the partner's.
- Neither world's Kiscule events show a spawner node.

**How a spawner spawns** (d2-ghidra, confirmed static; a helper agent's research, rechecked).

- `arkCpntSpawner` has vtable +0x1c57d78. It is listed at +0x268/+0x270 of the spawner entity, which has vtable +0x1c5e948 and its name at +0x20.
- `0x14075b770(component)` spawns now:
  - it takes a pooled NPC (+0x40, count at +0x48), or creates one;
  - it places and activates it, and lists it among the spawner's children (+0x30, count at +0x38);
  - it returns 1.
- +0x20 counts a pending spawn request.
- The two other paths queue a spawn request:
  - a help request (message 0xc6, `0x14075ae60`), heard by spawners whose definition sets +0xa3 or +0xa4;
  - an alarm (message 0x1a0).
- **Hypothesis:** the host's guards called for help when they saw the partner's avatar. The partner's copies of those guards follow the host, so they call no one.

**Candidate progress-v2163** (life state *present*, host only).

- The host announces each living unbound NPC of its world: once per partner world, during the resend window, and when a new one appears.
- A partner world still missing that NPC 10 s later spawns it from the spawner of the same name, with up to three tries.
- Story NPCs and placed corpses are left out.
- Lab switch: `coop_npc_present on|off`. Status: `lifePresentSent`/`Received`, `lifeSpawned`, `lifeSpawnRefused`, and `lifeRefusedLast` reasons.
- The same candidate keeps the partner's last Blink Assault kick parameters (`blinkKickInfo`, #261).

| Take | Result |
|---|---|
| `reinf-eotw-v2164a` | Equal throughout; the event did not happen. |
| `reinf-eotw-v2164t` | Setup fail: `coop_npc_present` was not in `Send-D2Command.ps1`'s list (added). |
| `reinf-eotw-v2164b` | Every route diff equal. The final check after the route then had the two reinforcements alive only in the host's world: they appeared after the end diff, possibly inside the partner's 10 s wait. No status was kept then. |
| `reinf-eotw-v2165t` (twin), `v2165a` | Equal; the event did not happen. Present messages flowed (host sent 606, partner received 606); nothing needed spawning. |

The event is rare, so **candidate progress-v2166** makes it happen on demand:

- `coop_lab_spawn <spawner>` spawns from that spawner in this world only.
- The probe's `--spawners` lists every spawner with its live, pooled and pending counts.
- `Run-V2166S.sh` lists both worlds' spawners on Edge of the World, to pick one with no live NPC.

## The spawn listing and three more rounds, 2166 to 2171

- **`spawners-eotw-v2166l`:** both worlds list 141 spawners.
  - Eight hold no live NPC and one pooled (dormant) NPC: the checkpoint, outpost and Addermire reinforcements, the shrine's Outsider, and a dead civilian.
  - A pooled NPC is listed in the catalog and digested as alive.
- **`reinf-eotw-v2167a`** (progress-v2166; route `reinf-eotw-v2167`, which adds a 40 s tail, both spawner lists and the status) had the event again. The partner spawned nothing:
  - the host had announced the reinforcements while they were pooled;
  - its spawners then made new ones under the same key, and nothing was announced again (`lifePresentSent` stayed 606).
  - The chain was stopped before take b; the protected files were checked unchanged.
- **progress-v2168** announces a key again when its NPC handle changes.
  - `reinf-eotw-v2168a` differed at the after diff: the host woke its pooled reinforcements in place (same handle) while the partner's were gone.
  - v2168b was equal. The chain was stopped after b.
- **progress-v2169** (confirmed static: dormant is `actor+0xd1` bit 0, set by `0x14087c350` and cleared by the activation `0x14087caa0`):
  - The host announces only active NPCs, and again every 20 s.
  - The partner spawns any key the host announced within 30 s that has had no active NPC locally for 10 s; the native spawn wakes a pooled NPC first.
  - `reinf-eotw-v2169a`, `b`, `c` and the twin `t` were equal. The event did not happen. There were no spurious spawns (`lifeSpawned` 0 in each) while the partner received 1430 to 2305 announcements.

### The on-demand test, 2170

`coop_lab_spawn script_spawner_outpost_reinf_01` is a host-only spawn: the host wakes its pooled reinforcement.

| Take | Result |
|---|---|
| `labspawn-eotw-v2170a` (progress-v2169) | **PASS** (confirmed runtime). Before: 0 children and 1 pooled in both worlds. The host reported `labSpawn: spawned`, and the partner woke her copy on the first try (`lifeSpawned` 1, `spawned`). After: 1 child and an empty pool in both worlds; the diffs were equal. |
| `labspawn-eotw-v2170t` (twin, `coop_npc_present off`) | **RED as required by the spawner counts:** the host had 1 child and the partner 0, still pooled. But the world diff read equal, because the digest counted the pooled NPC alive. |

**progress-v2171** digests a dormant NPC as `dormant`, so the world diff sees an NPC one world has woken and the other has not. `Run-V2171L.sh` runs the twin and the test again.

| Take | Result |
|---|---|
| `labspawn-eotw-v2171t` (twin, progress-v2171) | **RED by the world diff itself:** after the host's spawn, `script_spawner_outpost_reinf_01 host=alive client=dormant` (after and final diffs). At spawn both read dormant, equal. |
| `labspawn-eotw-v2171a` | **PASS.** The partner woke her copy on the first try (`lifeSpawned` 1, `spawned`). The spawn and after diffs were equal. |

**#260 is done** (confirmed runtime, progress-v2171).

- A host-only spawn reaches the partner's world, proven on demand with a twin.
- The world diff now sees woken-versus-dormant NPCs.
- The bottle was fixed by the presence change (progress-v2124).
- The natural reinforcement event, about 1 in 3 to 1 in 8 runs of v2071b's route, follows the same path. The step 6 runs keep watching for it.
