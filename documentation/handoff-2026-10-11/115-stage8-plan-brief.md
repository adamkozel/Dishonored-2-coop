# 115. Stage 8 brief: from 0.9.6 to a flawless 1.0

Status: **proposed, waiting for the user's approval** (2026-10-29). Once approved, this becomes stage 8 of [the roadmap](02-roadmap.md) and its cards go on the board.

## 1. The goal in one sentence

Two players can play the whole Dishonored 2 campaign together, as any pairing of Corvo and Emily, and both games always show the same world: the same story, the same items, the same doors, safes and guards. Nothing drifts apart, and nothing has to be explained away in the release notes.

## 2. Where we are (0.9.6)

The mod already does a lot. The campaign plays end to end on two Steam accounts, movement and powers look native, and combat with nearby guards is decided by the host. The release notes still list problems, though, and almost all of them have one cause.

**The cause:** for story, items, doors, levers, safes and guards far away, **both games run the game logic on their own** and then copy each other's events. Two copies of the same logic slowly disagree. coop-anything names this as the classic co-op mistake ([chapter 114](114-coop-anything-playbook.md)), and every project that got story co-op working moved away from it.

| Problem in the 0.9.6 notes | Why it happens | Fixed in |
|---|---|---|
| Safe combinations differ between the two worlds | Each game rolls its own random code | Step 4.3 |
| After joining mid-mission, some characters the host already dealt with are still in your world | Your world ran its own copy of the mission start | Step 4.4 |
| A story step done only in the partner's game doesn't count (curing Hypatia, #220) | The partner's game ran the step, but the host's game owns the mission record | Step 4.1 |
| In the credits the partner keeps two end-game script nodes active (#94) | Both games run the ending script | Step 4.1 |
| Tutorial: Corvo fails the stealth lesson when he notices either player, and a failure restarts both games | Two trainers, one in each game, both watching | Step 4.1 and 4.5 |
| Corvo partner's Blink Assault only staggers; rat and fish possession happen only in her world; Deadly Shade isn't fought by the host's guards (#168, #131) | These powers act on the partner's own copy of the world | Step 4.6 |
| The partner picks their character on the game's own screen in the first mission | A leftover of the first mission's own flow | Step 5 |

Two notes stay true by design: co-op is for **two players**, and partners still can't hurt or interact with each other directly. The slow memory growth over many hours is the game's own and happens without the mod too; we'll measure it again but not promise a fix.

## 3. The idea: the host decides, the partner asks

Both players still run the full game on their own PC, and that doesn't change. What changes is **who is allowed to decide**.

- **The host's game decides** everything about the shared world: story, items, doors, safes, guards, chaos.
- **The partner's game shows** what the host decided. It never runs a story script by itself.
- **When the partner does something to the world** (opens a door, picks something up, turns a safe dial, walks into a trigger), their game sends a request. The host's game does it with the partner's character as the one acting, and sends the result back. Both games then show the same result.
- **Each player stays in charge of their own character:** movement, camera, aiming and casting stay instant on your own PC.

| What | Today (0.9.6) | In 1.0 |
|---|---|---|
| Story, objectives, mission decisions, ending | Both games run it, events are copied | **Host** runs it, partner shows the result |
| Items and pickups | Both games, copied | **Host** decides who got it |
| Doors, levers, lifts, switches | Both games, copied | **Host** |
| Safes and their codes | Each game has its own | **Host**: one safe, one code |
| Guards near the host (within 55 m) | Host | Host (unchanged) |
| Guards far from the host | Each game on its own | **Host**, or the closer player if step 1 shows the game only runs guards near a player |
| Combat damage | Host | Host (unchanged) |
| Random rolls (loot, bonecharms, spawns) | Each game rolls its own | **Host** rolls once |
| Chaos level | Each game keeps its own | **Host**: one shared level |
| Joining mid-mission | Partner's game rebuilds from its own start | Partner gets the host's world, then follows along |
| Your movement, camera, aim, casting | Your own game | Your own game (unchanged) |
| Your coins, runes, bonecharms | Your own | Your own (decision 3 below) |

## 4. The steps

The steps run in this order. Each one has a clear finish line, and none of them pauses to ask you anything: when something breaks, the fix is to move that piece of control to the host, which is exactly what the plan does. The only thing that stops for you is the final playtest with your friend.

### Step 1: Measure before changing
- **Activation range:** find how far around a player Dishonored 2 keeps guards and scripts awake. In Gothic 3, host control broke as soon as the two players split up, because the engine only ran the world around the host's camera. This decides how step 4.5 handles far guards.
- **One-player assumptions:** list every place where the game assumes there is a single player, for example "the player" in a script, a trigger that only checks the local player, or a safe that rolls its code at load.
- **Where the game writes state:** find the functions that actually write story flags, objectives, item ownership, door and safe state, and chaos. These are the hooks steps 2 and 4 need.
- **Video capture check:** confirm our test recordings capture the game window only, never the desktop.
- **Done when:** the measurements are written up as a chapter with each finding labelled (confirmed static, confirmed runtime or hypothesis), and step 4's order is confirmed or changed because of them.

### Step 2: The difference detector and the flight recorder
- **Difference detector:** after every load and checkpoint, once the world has settled, both games compute a fingerprint of the shared world (story, items, doors, safes, guards, chaos) and compare. If they differ, it names exactly what differs.
- **Flight recorder:** each game keeps the last few minutes of what was sent, received and applied. When the detector finds a difference, the recordings from both PCs are lined up to find the first moment they disagreed.
- **Honest tests:** every test first checks it's really testing two connected players on the right map, both roles, and that traffic is actually flowing. A skipped check counts as SKIP, never as a pass, and a test that compared nothing hasn't passed.
- **Done when:** the detector runs in every lab and VM test, today's known drift (the safe codes, for example) shows up as a named difference, and a deliberately broken build makes the test go red.

### Step 3: Bad connections, crashes, rejoining
- **Network trouble:** a relay on the lab's connection adds lag, packet loss and a 5-second blackout, using standard profiles from good Wi-Fi to a very bad link. The lab's partner address becomes configurable so this is possible.
- **Steam connection life:** a keep-alive every second and a 20-second timeout, a clean leave, and a clear message when the two players have different mod versions.
- **Failures:** either game crashing, leaving and rejoining, and a quickload mid-session.
- **Done when:** every profile passes with zero differences after recovery, each request runs exactly once, and recovery from a 5-second blackout is measured.

### Step 4: The host takes control, one area at a time
Each area follows the same recipe:
1. The partner's game stops making that kind of decision itself.
2. Partner actions in that area become requests to the host.
3. The host sends results the partner's game applies without running any script.
4. A test proves zero differences.
5. A "broken" switch that brings the old bug back must make that test go red.

Every area also has an **off-switch** back to today's behaviour, so a problem in one area never blocks the rest of the game. The areas:

1. **Story, objectives, mission decisions, the ending.** The partner's story scripts run zero times, and a test counts this. Fixes #220 and #94, and the tutorial trainer.
2. **Items and pickups.** One owner per item, decided by the host. No duplicates, nothing lost.
3. **Doors, levers, lifts, switches and safes.** One safe code for both players.
4. **Joining mid-mission and quickload.** The partner receives the host's world, then the live updates. A host quickload rewinds both games together, and stale messages from before the load are dropped. No leftover characters.
5. **Guards far from the host,** built on what step 1 measured: either the host runs them, or the player closer to them does, with clean handoffs between players and no guard ever created twice.
6. **Partner powers that touch the world:** Blink Assault, possession of rats, fish and corpses, Deadly Shade. Each becomes a request the host carries out.

- **Done when:** all six areas are on, the difference detector shows zero differences through every mission, and the partner's objectives, prompts and HUD still work.

### Step 5: Partner quality
- **Movement measured in numbers:** a probe on the partner's third-person body measures foot sliding, jitter, snapping and how far the pose is from where it should be, so movement quality becomes a test.
- **Polish:** the first mission's character choice uses our own *Character* dialog, and the messages and menus are reviewed for anything that still feels unfinished.
- **Done when:** the movement numbers pass their limits in both roles and all four pairings.

### Step 6: Prove it and release 1.0
- A one-hour session on two Steam accounts with zero differences.
- The whole campaign on two Steam accounts (this PC and the VM) in one pairing, and the other three pairings through the first, a middle and the last mission.
- Save and load with the partner present.
- An update from 0.9.6, and a fresh install from GitHub on the clean VM.
- The release notes' known limitations shrink to the two-player limit and the game's own memory use.
- **Then 1.0 is published.** The last step, a playtest with your friend, is yours (a Needs-you card).

## 5. How we'll work

- **One main session** (this one) builds the mod, runs every lab and VM test, checks the results and runs the GitHub loop. There is only one lab, and every result updates the same two state files, so lab work goes through one place in order.
- **Helper agents** do the work that doesn't need the game, in parallel: Ghidra research, offline tools (the analyzers, the network relay), and an independent review of each change before it ships. I check what they produce before anything is committed.
- **Multi-agent workflows** only when you say "use a workflow", for example for step 1's survey.
- **Model:** Opus 5.5 at high effort for normal work. I'll suggest switching to extra effort for the hardest design and debugging stretches.
- **Tools:** d2-ghidra first for reverse engineering; the coop-anything knowledge server and skills; REA as a second opinion if you approve decision 2.
- **GitHub:**
  - Stage 8 goes on the board, with one card per step and per area of step 4.
  - Every result is committed, pushed to a draft PR and merged when its card is done, as CLAUDE.md describes.
  - Each step ends with a chapter in these docs and a short update to you.
- **Safety rules don't change:**
  - The original Steam install, its settings and the 14 protected files are never touched.
  - Tests run only in the lab copies, and the lab is restored after every test.
  - The VM game never runs at its default settings.
  - Your saves are only ever read as data.

## 6. Risks and what we do about them

| Risk | What we do |
|---|---|
| The game's script system has no clean way to apply a result without running the script | Step 1 looks for the writer functions first. Where there is no clean path, that area keeps today's copy approach behind its off-switch and is listed honestly in the notes. |
| Guards are only active near a player, so the host can't run guards near a far partner | Step 1 measures it. If so, step 4.5 uses closest-player ownership with host-checked handoffs, as Gothic 3's mod did. |
| A host-side request feels slower for the partner | Movement and aim stay local. Only world actions wait for the host, and step 3 measures how long they take on bad connections. |
| Step 4 is big | It's split into six areas, each with its own card, test and off-switch, so progress is always visible and nothing is half-done. |
| A Windows Defender false positive (#25) | Already a Needs-you card; it doesn't block the work. |

## 7. Not in scope

- More than two players.
- A fake Steam (mock `steam_api`, test AppID): we always use the game's own Steam.
- Other co-op mods' code for Dishonored 2: we never open them.
- The human playtest: that's yours, as the last step.

## 8. Decisions for you

1. **The test tools' code.** coop-anything's sync test tools are MIT-licensed, but their code was generalised from fnv-coop, a co-op mod for another game.
   - **(a)** use it, with its license notice in `third_party/coop-anything/`. This is faster.
   - **(b)** write our own versions from the designs. This is slower.
   - Recommended: **(a)**. These are generic test tools, not Dishonored 2 game code.
2. **REA.** Change "Ghidra only through d2-ghidra" to "d2-ghidra first, REA as a read-only second opinion". Recommended: **yes**.
3. **Shared or per player.**
   - **Shared:** items in the world (what one of you takes is gone for both) and the chaos level, decided by the host.
   - **Per player:** coins, runes, bonecharms and notes.
   - Recommended: **as listed**.
4. **The final playtest with your friend** stays the last step before 1.0, as a Needs-you card. Recommended: **yes**.

Reply **"approved"** to take all four recommendations, or "approved" with your own answers.
