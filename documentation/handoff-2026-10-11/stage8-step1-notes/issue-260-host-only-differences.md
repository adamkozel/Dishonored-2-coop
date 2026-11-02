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
