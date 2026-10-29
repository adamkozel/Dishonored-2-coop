# 119. Stage 8 area 4.1: the host runs the story (in progress)

Card [#237](https://github.com/adamkozel/Dishonored-2-coop/issues/237), from [the stage 8 brief](115-stage8-plan-brief.md). The goal is that the partner's game runs no story script that writes the shared world. The host runs those scripts, and the partner applies the host's results. This chapter records the work so far. Labels: confirmed static, confirmed runtime, hypothesis.

## What the partner ran before (confirmed runtime)

- **The census** (`coop_kiscule_census`, candidate census-v2011) counts every dispatched Kiscule signal by node class. In take `census-dunwall-v2011a` (the lab Dunwall route), the host ran 695 signals of 43 classes and the partner ran 684 of the same 43. The partner's game ran the story itself.
- **The classification:** a read-only helper survey classified all 274 Kiscule node classes used in the campaign, with vtable RVAs from the executable's RTTI. The groups are World (writes the shared world), LocalPlayer (acts on the player of the world that runs it), presentation, flow and unknown. The full table is game-derived and stays local. `tools/diag/gen_kiscule_policy.py` turns it into `src/kiscule_policy.h`.
- **The dry run** (candidate policy-v2012, take `census-dunwall-v2012a`): over the route, the partner ran 43 World and 10 LocalPlayer signals of its own. They were NPC relations, gameflow tasks and objectives, midnight scenes, distractions, senses, destroys, autosaves, cinematics, inventory and clues.

## Host story (`coop_host_story on|off`, off by default)

For each of the 61 World classes:
- **The host** runs the node and sends its count (the existing D2KA count mirror, generalised from its 5 grant classes).
- **The partner** never runs the node on its own: the handler is stubbed, so the partner's flow and presentation continue. It runs the node only as a replay of the host's run.
- **Requests:** when the partner reaches a node more often than the host has run it, it asks the host to run it. Requests are asked and run only while both games are in plain gameplay.

What the takes taught (each failed take is kept in `work/natural-moveset/`):

| Lesson | Take |
|---|---|
| LocalPlayer classes must run in both worlds. A host-replayed Cinematic left the partner stuck in the Mayhew scene | `hoststory-dunwall-v2015a` |
| The lab restore keeps the game processes, so host story, its twin and the counters must be reset at the end of every take | `hoststory-dunwall-v2016b` |
| With host story on through a mission intro, the host's game vanished in the opening cinematic while running requests out of flow (hypothesis for the cause). Requests now wait for plain gameplay | `hoststory-lead-dunwall-v2014c`, `v2016c` |
| A one-time request misses nodes the host already ran earlier; requests go by count | `loot-trace-on-v2018e` |
| A story-only request list misses the ActionUsable before Ramsey's ring task; any World class may be requested | `hoststory-lead-dunwall-v2019a` |

## Results so far (confirmed runtime)

- **Host-led Dunwall route:** `hoststory-dunwall-v2017a`, `v2017c`, `v2018b`, `v2019b` and `v2020b` passed.
  - The partner ran 0 World nodes of its own; about 22 were stubbed and about 23 replayed.
  - The worlds were equal (538 values) and the gameflow was equal. Objectives advanced together at every step.
  - The partner's native HUD showed the replayed objective update and mission clue.
- **Mutation twin** (`coop_host_story twin_no_block`): the take goes red ("the partner ran 22 World story nodes of its own"; `hoststory-twin-dunwall-v2017b`). A clean take right after it passes.
- **Partner-led Dunwall route:** still failing at the loot of Ramsey's body (`v2017d` to `v2020a`).
  - The worlds agree, but the ring task never completes.
  - Traces (`loot-trace-off-v2018d`, `loot-trace-on-v2018e`) show the partner's flow is identical with host story on or off up to the ring task nodes 2899/7 and 2973/7. The open question is why no request completed them on the host; take `loot-trace-on-v2021a` keeps a 256-entry event log to answer it.

## Still to do in area 4.1

- Partner-led steps (the loot, #220's partner-only steps), then the other missions.
- Player-scoped script actions reaching the partner's own player, now that LocalPlayer classes run in both worlds.
- Chaos and kill credit for partner kills.
- Story items reaching both inventories.
- Turning host story on in the product controller with an off-switch.
