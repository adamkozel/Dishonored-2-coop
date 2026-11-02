# Mid-mission join and host quickload (stage 8 area 4.4, #240)

Read-only research, 2026-11-01. Labels: **CS** confirmed static, **CR** confirmed runtime (take cited), **H** hypothesis. Paths are under `outputs/d2-coop-lab/src/` unless given in full.

## 1. A mid-mission join today

- **The partner loads her own mission start, not the host's save** (CS).
  - The host publishes D2MF each second (`session_product.h`). The joiner runs `campaign <mapinfo> <chaos> <character>`, only from a publication under 3 s old and never from the title screen.
  - Placement happens once per followed map or new connection: 1.2 m behind the host, from 25 m away (`join_warp.h`; chapter 106).
- **Harness:** in `Route-MissionVM-v2053.ps1`, `JoinNow` records the distance, gameflow and NPC life, but does not fail on NPC differences. The difference detector runs only at the end, or at a `diff` step.
- **What the partner then receives:**
  - **Current state only:** story flags (D2SF, every 2 s); gameflow (D2GF; mission nodes stay per world); chaos (D2CH); doors (when a channel arms); pickup presence (D2PP, with catch-up); safe codes (epoch-checked) (CS).
  - **The NPC life roster:**
    - It sends the current state of unbound NPCs, but only when a state rises above what was last sent (`npc_authority_stream.h`, `processNpcLife`). The partner gives up after 5 refused tries.
    - Entries reset only on the host's own world epoch, not on a new partner world (CS). So after a rejoin from a reloaded game, deaths the host already sent are never sent again (H).
    - It is two-way: the host still applies a partner's states, except for named characters under host story (#254) (CS).
  - **Kiscule host story (a log by count):**
    - The host sends cumulative run counts per node; the partner runs nodes until its count matches (`kiscule_action_mirror.h`). Sends made before the connection queue up, at most 1024.
    - The partner drops pending entries after 60 s (CS), while a VM load takes 170-180 s. So the host's pre-join log can be lost on a slow join (H).

## 2. Past join takes (CR)

- **`campaign-join-vm-v1703a` and `v1714b`:** `ld_spawner_ramsey_escape` and `ld_spawner_alexi_escape` are dead for the host and alive in the partner's world at the join. Gameflow is equal. In the Void (1703a), `Mission_DW` stayed 0 for the partner while the host's completed.
- **`route-dunwall-join-v1682a`:** Ramsey converged (`lifeApplied` 1). Alexi stayed alive after `lifeRefused` 5.
- **`route-resumed-join-v1871c`:** the host's banana stayed in her world (fixed by the catch-up in v1876a). 4 usable differences remained: the safe dials, before area 4.3.
- **`campaign-titlejoin-vm-v1744a` and `peer-loss-dunwall-vm-v2010a`:** equal (538 values), but weak proofs: nothing was killed before the join.

## 3. A host quickload today

- **The world epoch** rises on any change of game pointer, player or map (`engine_bridge.cpp`), so any load bumps it (CS).
- **`checkpointIntercept`** (`checkpoint_sync.h`) consumes any kind 0 or kind 2 request it does not own. The request becomes a shared restart from the **last paired autosave**, not the chosen save (CS).
  - Whether the pause-menu Load or the quickload key goes through the console `LoadGame` is H. Chapter 118: "the partner does not reload when the host loads".
- **Renewal after a load:** door and NPC sessions renew only when **both** epochs are new (`door_session_scope.h`, CS). After a host-only load they may stall (H).
- **Stale messages:** doors, usables, the NPC stream, coins, the world observer and safes drop by epoch. D2SF, D2GF and the Kiscule packets check only the session or map (CS).
- **No take has run a host load with a partner connected.**

## 4. Plan

**Join:**
1. **NPC life snapshot:** reset the roster's `sent` marks when the peer's session or epoch changes, so the host sends its whole roster to each new partner world. A partner's state becomes a request the host decides. Retry with a backoff instead of giving up after 5 tries, and fall back to native removal when a script holds the NPC (Alexi).
2. **Kiscule log:** when a new partner world appears on the host's map, the host sends its host-story counts again. The partner keeps entries for the host's current map instead of dropping them after 60 s.
3. **Mission nodes:** decide whether a join snapshot applies them, or whether the detector lists them as expected.

- **Takes:** VM, Dunwall, `-JoinAfterStep 5` after a kill and a knockout, with `diff` steps (`require`) at the join and 60 s later.
- **Mutation twin:** `coop_join_snapshot off` on the host, which must come back DIFFERENT and name Ramsey and Alexi.

**Host quickload:**
1. **Measure first:** the host saves, kills a worker, takes a pickup and loads, with the partner connected. Keep the checkpoint trace, both epochs, `.reset.json` and the detector.
2. **Shared load:** a host kind-2 load of slot S becomes a shared transaction carrying S. The partner loads her save paired with S (`autosave_pair.h` records the pairing), and falls back to the join snapshot without one. A partner's own load becomes a request to the host.
3. **Stale messages:** add source and target world epochs to D2SF, D2GF and the Kiscule packets, with a dropped counter.

- **Takes:** save, then a later host load. The diff must be EQUAL after the load and at the end.
- **Mutation twins:** the shared load off, which must come back DIFFERENT; and the epoch filter off.
