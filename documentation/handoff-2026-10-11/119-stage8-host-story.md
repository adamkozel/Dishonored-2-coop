# 119. Stage 8 area 4.1: the host runs the story (in progress)

Card [#237](https://github.com/adamkozel/Dishonored-2-coop/issues/237), from [the stage 8 brief](115-stage8-plan-brief.md). The goal is that the partner's game runs no story script that writes the shared world. The host runs those scripts, and the partner applies the host's results. This chapter records the work so far. Labels: confirmed static, confirmed runtime, hypothesis.

## What the partner ran before (confirmed runtime)

- **The census** (`coop_kiscule_census`, candidate census-v2011) counts every dispatched Kiscule signal by node class. In take `census-dunwall-v2011a` (the lab Dunwall route), the host ran 695 signals of 43 classes and the partner ran 684 of the same 43. The partner's game ran the story itself.
- **The classification:** a read-only helper survey classified all 274 Kiscule node classes used in the campaign, with vtable RVAs from the executable's RTTI. The groups are World (writes the shared world), LocalPlayer (acts on the player of the world that runs it), presentation, flow and unknown. The full table is game-derived and stays local. `tools/diag/gen_kiscule_policy.py` turns it into `src/kiscule_policy.h`.
- **The dry run** (candidate policy-v2012, take `census-dunwall-v2012a`): over the route, the partner ran 43 World and 10 LocalPlayer signals of its own. They were NPC relations, gameflow tasks and objectives, midnight scenes, distractions, senses, destroys, autosaves, cinematics, inventory and clues.

## Host story (`coop_host_story on|off`, off by default)

For each of the 56 World classes (61 at first; `BOTH_WORLDS` in the generator lists the five that run in both worlds, with the reasons in the table below):
- **The host** runs the node and sends its count (the existing D2KA count mirror, generalised from its 5 grant classes).
- **The partner** never runs the node on its own: the handler is stubbed, so the partner's flow and presentation continue. It runs the node only as a replay of the host's run.
- **Requests:** when the partner reaches a node more often than the host has run it, it asks the host to run it. The request goes out at once; the host runs it only while its own game is in plain gameplay.

What the takes taught (each failed take is kept in `work/natural-moveset/`):

| Lesson | Take |
|---|---|
| LocalPlayer classes must run in both worlds. A host-replayed Cinematic left the partner stuck in the Mayhew scene | `hoststory-dunwall-v2015a` |
| The lab restore keeps the game processes, so host story, its twin and the counters must be reset at the end of every take | `hoststory-dunwall-v2016b` |
| With host story on through a mission intro, the host's game vanished in the opening cinematic while running requests out of flow (hypothesis for the cause). Requests now wait for plain gameplay | `hoststory-lead-dunwall-v2014c`, `v2016c` |
| A one-time request misses nodes the host already ran earlier; requests go by count | `loot-trace-on-v2018e` |
| A story-only request list misses the ActionUsable before Ramsey's ring task; any World class may be requested | `hoststory-lead-dunwall-v2019a` |
| A request made while the partner's game is in a cinematic was skipped. Requests then waited in a queue for the partner's plain gameplay (candidate hoststory-v2023) | `hoststory-lead-onlytask-v2022e` |
| The task that ends Dunwall (1361/7) runs inside the partner's ending cinematic, so a queued request never went out and the mission never ended. Requests now go out at once in any game state; the host still runs them only in its own plain gameplay, which is what kept the host safe in intros (candidate hoststory-v2026) | `hoststory-lead-dunwall-v2025a` |
| A request the host ran never reached the partner's world: the host calls the handler directly, which queues nothing, and the partner had stubbed its own run. The tower safe-room doors (`env_interiors_safe_door`, `_6`, `_9`) opened only in the host's world; host story off kept them equal. The host now sends each request it ran back as a host run, which the partner replays (candidate hoststory-v2027) | `hoststory-lead-dunwall-v2026b`, control `leadoff-diffs-v2026a` |
| Safe dial digits (`ld_combinaison_numbers_15` to `_17`) differ between the worlds with host story off too: combinations are drawn per world. That belongs to area 4.3 (safes) | `leadoff-diffs-v2026a` |
| ActionSpecialLoot attaches an item entity to an NPC's loot (confirmed static: its Attach and Detach handlers add the item to the target's loot component). Ramsey's ring is a different entity for Corvo and for Emily, so the class places per-player items and runs in both worlds (candidate hoststory-v2024) | `ring-onlyloot-v2023c` failed, `ring-onlyhealth-v2023c` passed |
| A blocked LevelTransition deadlocks the partner-led ending: the partner's request waits for plain gameplay, which its ending cinematic never reaches, and the partner sits in a black screen. The mission-end mirror coordinates transitions, so LevelTransition runs in both worlds (v2025) | `boat-trace-v2024c` |
| The native dispatcher (0x1407c0e30) calls the node's vtable+0x38 step after any input handler that returns nonzero, as the stub does (confirmed static). ActionMidnightControl, Goto and EntitySpawnerControl do their work in that step (a midnight scene, an NPC goto, a spawn), so the stub never blocked them and the host's replay ran them a second time. A midnight scene that cannot start yet re-signals its node with input 0, which has no input descriptor to stub: 76 such retries were counted as fallbacks. The three classes run in both worlds (v2025) | `boat-trace-v2024c` |

## Results so far (confirmed runtime)

- **Host-led Dunwall route:** `hoststory-dunwall-v2017a`, `v2017c`, `v2018b`, `v2019b` and `v2020b` passed.
  - The partner ran 0 World nodes of its own; about 22 were stubbed and about 23 replayed.
  - The worlds were equal (538 values) and the gameflow was equal. Objectives advanced together at every step.
  - The partner's native HUD showed the replayed objective update and mission clue.
- **Mutation twin** (`coop_host_story twin_no_block`): the take goes red ("the partner ran 22 World story nodes of its own"; `hoststory-twin-dunwall-v2017b`). A clean take right after it passes.
- **Partner-led Dunwall route, passed with candidate hoststory-v2027** (`hoststory-lead-dunwall-v2027a`):
  - Both games ended the mission into the Void, and the worlds there were equal.
  - The partner ran no World node of its own, with 0 fallbacks; 140 runs were blocked, 120 requested and 148 replayed.
  - The host-led take on the same candidate passed too (`hoststory-dunwall-v2027b`, 538 values equal).
  - Still open: the mid-route diff after the safe room shows the three tower safe-room doors open on the host and closed on the partner, though every requested safe-room node now reaches the partner.
- **The safe-room doors** (`saferoom-trace-v2028a`, confirmed runtime):
  - When the safe opens, both games run the same burst of about 45 World nodes within 50 ms. It includes ActionUsable 1511628513 four times, once per safe door.
  - The host opens all four doors (`env_interiors_safe_door`, `_5`, `_6`, `_9`). The partner blocks the burst, and the host's runs arrive 13 s later as direct handler calls.
  - By then the node's target input holds its last value, so all four replays hit `_5`, the only door that opened in the partner's world.
  - Replaying a node out of flow cannot rebuild data inputs that changed in between. Door and safe states need a result-level sync, which is area 4.3 ([#239](https://github.com/adamkozel/Dishonored-2-coop/issues/239)).
- **Meagan's talk** registered in 2 of the 5 partner-led takes from v2024 to v2028 (confirmed runtime, ordering):
  - When the partner's own flow reached ActionControlTalk 1340523551 before the host's run arrived, the scene went on (`hoststory-lead-dunwall-v2027a`).
  - When the host's run arrived 16 ms first, the handler ran out of flow. The partner's own run was then stubbed, and the flow never reached task 1429/7 (`saferoom-trace-v2028a`).
  - Candidate hoststory-v2029 holds such a host run for up to 750 ms. If the partner's flow reaches the node in that window, it runs in flow, counted as the host's run (`in_flow`, `hostStoryInFlow`).
- **Checkpoint, candidate hoststory-v2029** (confirmed runtime):
  - **Partner-led:** `hoststory-lead-dunwall-v2029a` and `v2029b` both passed. Meagan's scene started by itself, as with host story off. The worlds in the Void were equal. The partner ran no World node of its own: 128 blocked, 125 requested, 136 replayed and 12 run in flow.
  - **Host-led:** `hoststory-dunwall-v2029c` passed with 538 values equal. The partner's World nodes now almost all run in flow on the host's count: 19 in flow, 1 blocked.
  - **Remaining difference:** only the safe-room doors mid-route, which go to area 4.3.
- **Partner-led Dunwall route before v2027:** failed at the loot of Ramsey's body (`v2017d` to `v2023a`): the worlds agreed, but the ring task never completed. Two causes were found by bisection, with exemptions (`coop_host_story exempt <Class>`):
  - Host story on with every class exempt passed (`allexempt-v2022d`), and host story off passed (`lead-off-v2022c`).
  - **Request timing:** blocking only ActionGameflowTask failed. The ring task nodes 2899/7 and 2973/7 run in the instant the loot scene ends, while the partner's game is still in its cinematic state, so the request was skipped. Fixed in v2023 by deferring requests.
  - **The ring on the partner's Ramsey:** blocking Health and ActionSpecialLoot together failed. Blocking ActionInteractable, ActionUsable or ActionNPCInteraction alone passed (`ring-only*-v2023c`). The split put it on ActionSpecialLoot (`ring-onlyloot-v2023c` failed, `ring-onlyhealth-v2023c` passed). With it blocked, the partner's loot showed one `interactWith` and `moneyAdded` but no `itemAdded`: the partner's ring was never attached. Fixed in v2024: the class now runs in both worlds.

## The product switch (candidates hoststory-v2030 and v2031)

- **On by default in the product.** `d2coop.cfg` `host_story=0` turns it off: the proxy then sets `D2COOP_HOST_STORY=0` and the host never turns it on. The 1.0 README must document this off-switch.
- **The host decides.** It sends its setting as control flag 4 in its D2CT hello. The partner follows the host's flag once the hello is valid, so both games always agree.
- **Who follows what:** in the product (`productMode`) the host takes `d2coop.cfg` and the partner follows the host. Under the lab product controller (`coop_product_auto`, as the VM takes use), the partner follows the host too, and the host keeps the lab's `coop_host_story` commands.
- **v2030's lesson** (`campaign-dunwall-vm-hoststory-v2030a`, confirmed runtime): the VM partner runs under the lab product controller, not `productMode`, so the follow rule never ran. It refused 145 host-story runs, yet the worlds still ended equal. v2031 lets the partner follow under the lab controller too.
- **The two-account harness** `Route-MissionVM-v2013 -HostStory` turns host story on in the host's game, diffs mid-route, and fails when the VM did not follow, when the VM ran a World node of its own or fell back, or when the final world diff differs. v2014 also keeps the last 400 lines of both games' logs on a failure.

## Two-account campaign under host story (candidate hoststory-v2031, confirmed runtime)

Host Corvo on this PC and VM partner Emily on the second Steam account, both on co-op saves, with story flags diffed. Takes `campaign-<mission>-vm-hoststory-v2031a`. In every mission that reached its end, the VM followed host story with 0 World runs of its own and 0 fallbacks, and the story flags and gameflow were equal.

| Mission | Result | World diff at the end | Partner: blocked / asked / in flow / replayed |
|---|---|---|---|
| A Long Day in Dunwall | pass | equal (286) | 84 / 75 / 34 / 116 |
| Edge of the World | differ | 6 safe-dial values (area 4.3) | 217 / 205 / 97 / 256 |
| The Good Doctor | differ | 2 lab glass breakables on the boat, absent in the VM world (`b_hyp_arrived_lab_graduated_cylinder_01`, `b_hyp_arrived_lab_retort_01`) | 67 / 62 / 39 / 557 |
| The Clockwork Mansion | stopped | the host died in the streets after its health hold went off (game over) | - |
| The Royal Conservatory | stopped | the VM game vanished during the Meagan exit, with no application error or WER report | - |
| Dust District | differ | 4 safe-dial values (area 4.3) | 172 / 139 / 66 / 224 |
| The Grand Palace | pass | equal (608) | 239 / 202 / 75 / 305 |
| The Dreadful Wale (bunker) | pass | equal (480) | 304 / 250 / 141 / 336 |
| Death to the Empress | pass | equal (249, main menu after the credits) | 668 / 583 / 318 / 650 |

- **#94 (credits):** at the Empress ending both worlds showed the same gameflow counts `[226,0,0,14,0,8,0]` (`step33-talk` and `step34-expect` mission-end snapshots). The partner no longer keeps the two end-game nodes active that #94 recorded (`[226,2,2,12,0,6,0]`).
- **Re-runs** of The Good Doctor, The Clockwork Mansion and The Royal Conservatory with harness v2014: `campaign-<mission>-vm-hoststory-v2031b`.

## Still to do in area 4.1

- #220's partner-only steps under host story: the partner cures Hypatia (no route yet) and confronts the Duke's double.
- Player-scoped script actions reaching the partner's own player, now that LocalPlayer classes run in both worlds.
- Story items reaching both inventories (with area 4.2).
- The tutorial trainer watching both players.
- EntitySpawnerControl host-only: the partner must skip the node's vtable+0x38 step as well as its input handler, and the host's spawn must reach the partner through the NPC streams.
