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
