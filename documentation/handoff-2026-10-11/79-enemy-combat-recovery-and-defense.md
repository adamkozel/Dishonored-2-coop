# Enemy combat recovery and native defense

Updated14 October2026. This continues [actual enemy-to-player contacts](78-native-enemy-to-player-contacts.md). Accepted fallback988 is preserved with audit991. Subsequent candidates are isolated experiments until their runtime receipts pass.

## Recovery implementation

Candidate992 adds attack packet version2 (still176 bytes), a client terminal outcome tied to the acknowledged contact, one-sided stop notification and retained checkpoint intent. The actual player processes the lethal native call once, acknowledges it, then reports its terminal outcome. The host retires contact production; no health value or damage amount is transmitted. A temporary, identity-checked filter can discard further hits against the owned clone during the death animation.

Only session IDs, world epochs and the opt-in intention survive recovery. Native capture ownership is cleared before teardown. Both new epochs, the same Steam session/map, fresh NPC bindings, ready avatar ownership and normal gameplay are required before automatic negotiation. Nonces and event counters start fresh. Recovery expires after60 seconds; disconnects, changed maps, explicit off and uncoordinated loads retire intent. Native menu renewal is still unfinished.

The176-byte protocol tests exercise valid terminal identity, wrong sender role, missing event and forbidden geometry. Existing ownership, checkpoint, movement-resume, proxy smoke and both real357-clip bank tests pass for992/994/997. These checks do not substitute for the following native runs.

## Failed runs retained

- [Take993](../../work/natural-moveset/npc-native-death-v993/result.json), candidate992, fails the retained-intent assertion. Six native applications reach client health0. Terminal message and acknowledgement work, but host intent disappears when its shared native death screen is entered. Full isolated restoration passes. Automatic reload was not reached.
- Candidate994 makes terminal registry cleanup tolerate expected avatar retirement. This was a plausible cause, not an established diagnosis. [Take995](../../work/natural-moveset/npc-native-death-v995/result.json) repeats the failure; its five native applications and terminal exchange work, then host intent disappears. Full restoration passes. Do not label994 a successful recovery fix.
- Inspection of all stop call sites identifies the missed `stopNpcStream("shared_death")` in `checkpoint_sync.h` when the healthy partner receives shared death. This is separate from `checkpoint_barrier`, which was already retained. Candidate997 recognizes that existing coordinated-death reason. Take998 confirms both peers retain intent at their native death screens, four contacts are acknowledged/applied, and the host discards one subsequent clone contact. One native coordinated reload and NPC renewal complete, but automatic attack negotiation fails: the recovery scope rejected the transient unavailable map identity during loading. Full restoration passes.
- Candidate1002 permits that intermediate loading identity while still requiring matching stable session/map identities and both fresh epochs to renew. Its deadline follows the actual coordinated retry, so waiting at the native death screen does not expire the intention. Diagnostics retain the reason that first retired intent. Runtime1003 is pending at this entry's creation.
- Held-block take999 on997 fails during fixture setup because the world contains both audience and escape Ramsey entities. It never tests blocking. Cleanup passes. The corrected fixture1004 selects the unique Ramsey already in the negotiated NPC catalog, instead of assuming one definition means one world entity.

Failed videos remain raw diagnostics. No original Steam asset, save, animation bank or first-person animation has been changed.

## Native defense investigation

Exact-program Ghidra queries are saved in [npc-defense-ghidra-v994.json](../../work/movement-integration/npc-defense-ghidra-v994.json); bookmarks are persisted in [handoff-npc-defense-bookmarks-v994.json](../../work/handoff-npc-defense-bookmarks-v994.json). Addresses below are for the pinned exact Steam executable, not portable offsets for arbitrary versions.

- Confirmed static: health vtable141c51588 slot+D0 points to140718fe0. The native damage entry140717950 calls it. It iterates the health component's registered protection entries at+88/count+90 and invokes140435200. Regional modifiers140719d20 are skipped when the native protected-result mask is nonzero.
- Confirmed static:140435200 compares the inverse incoming direction with the relevant actor's root basis using1406dd130. Declaration fields128/12c/130 select the directional rule; damage masks138/140 participate. A matching protection writes result+8, a result+10 value and zero scaling. Further native event handling is present.
- Hypothesis: ordinary player block can therefore work through the reconstructed native damage call. This must be demonstrated by ordinary held-block input, a native block animation, protected zero-damage result and visual review. Neither a static function nor zero health loss alone proves it.
- Open: timed parry, attacker recoil/stagger on the authoritative host, interrupted attacks, rear/side arc behavior under movement and network delay. A successful held-block result would establish only that subset.

The current damage observer already captures the native result mask and inflicted amount. Defense tests use that independent observation, rather than substituting a custom block-damage rule.

## Accepted recovery1003 and failed defense1004

[Audit1006](../../work/movement-integration/npc-recovery-audit-v1006.json) preserves all four preceding failures and the next two results. Live candidate1002 is pinned to DLL70ACF63FB8D5D2E21072996D2234A11D54DDEA82EDF015378FF1324D8E1547B7; the probe and both67/357 animation banks remain unchanged.

Take1003 passes six actual native enemy contacts through death, matching host event/ack counts, both native death screens, one shared load each, automatic fresh attack nonce2/event0 on both peers and one-sided host stop acknowledged as `peer_stopped` by the client. Both inspected six-frame sheets show the correct native death UI. Post-load negotiation is established by identity/status receipts after capture ends; this take does not include a second physical hit after reload. Restoration passes.

Take1004 fails defense acceptance. Twenty-three samples identify actual `arkPlayerStateBlock` (state44, vtableRva1c81a68) and `sword_parry_idle`; two relayed guard2 contacts nevertheless inflict5 each (56→51→46), with no protected-result mask. A read-only protection-list snapshot at the beginning finds zero entries. The client video shows a block pose, but the camera is aimed at flowers and the actual attacking guard is not consistently in front. Therefore this does not establish whether an earlier native defense gate is bypassed or the incoming contacts lie outside the block arc. The fixture must measure/follow the actual attacker before drawing that conclusion. Restoration passes; no defense success is claimed.

Follow-up static facts:1407806d0 tests a DamageInfo region bit at+20. Health effect slot+E0 points to14071a7c0, whose inherited140717f50 emits an owner event carrying the native DamageInfo and result. Those facts do not establish authoritative enemy recoil. Additional transcripts are in [npc-defense-followups-v1000.json](../../work/movement-integration/npc-defense-followups-v1000.json).

## Native pre-damage block resolution and accepted held block 1013c

Updated 14 October 2026. Evidence: [findings](../../work/movement-integration/npc-block-native-v1011.json), [audit1014](../../work/movement-integration/npc-block-audit-v1014.json), [candidate1012](../../work/movement-integration/candidate-npc-combat-v1012/identity.json). Live DLL `989B3EBC71AF3D899940D0039028AA2C97768C5CFA6FE8481CF4DB51365D1B5C`; probe and both 67/357 banks unchanged; movement protocol 69; enemy-attack packet version 3 (still 176 bytes).

### Why 1004/1008 failed (confirmed)

- **Confirmed runtime (local control 1011c, build1002, no relay):** three native enemies engaged the held-blocking client player for about 12 seconds. Health stayed 56 and the damage observer recorded **zero** `npc_sword` calls to 717950. Two zero-amount `npc_closecombat.damage` calls appeared (inflictor = attacker, result flags 177). Earlier harness-only failures 1011 and 1011b are retained.
- **Confirmed static:** the block is decided by the attacker's weapon before any damage event. `arkCpntWeaponMeleeNPC` victim resolution `1406357d0` fetches the victim's `arkCpntBlock` (descriptor `1439edc20`), tests the attacker's bounds centre (`878230`) against a camera-based cone (`60d750`/`60d8b0`, requires PlayerStateMachine state 0x2c = `arkPlayerStateBlock`), requires weapon decl+0x180 (blockable), then calls the block state's `859980(state, attacker, component+0x3428 bit2 heavy)`. Result 0 = block, 1 = parry-lose for the attacker, 2 = parry-win (`636140` damages/staggers the attacker). Results 0/1 post event 0xe6 `{victim, result}` to the attacker (`8112c0` on `810240()+0xc8e8`), handled by `arkCpntAICombatAttack` `1405cc280` (result 1 → `StartParryLose`). Only an undefended hit takes vtable+0x138 (`634320`) to the damage event.
- The relay called 717950 directly, bypassing this resolution. The empty protection list in 1004/1008 was expected: block is not a health protection entry. `arkPlayer`'s damage-event override `1407846f0` is only a possession redirect.
- Ghidra: functions renamed (`D2_WeaponMeleeNPC_ResolveVictimBlock`, `D2_CpntBlock_AttackerInCone`, `D2_PlayerStateBlock_ResolveIncomingMelee`, …), bookmark category `D2Coop`, program saved.

### Candidate 1012

- Host: when the clone receives a captured native sword contact, the weapon's own `arkCpntWeaponMeleeNPC` (vtable/owner checked) supplies blockable and heavy bits in the hit packet. No health or damage value is sent.
- Receiver: before any native damage, runs the native cone `60d750` and block-state resolution `859980` for the actual player against the local replica of the authoritative attacker. A defended contact never reaches 717950; the native block impact/parry presentation is the game's own. Exact-image prologues are checked first.
- The acknowledgement carries Damage/Blocked/ParryLose/ParryWin. For Blocked/ParryLose the host posts native event 0xe6 to its authoritative attacker, with the owned clone as victim. **Parry-win attacker stagger is not mirrored yet.**
- Build/tests: world/ownership (incl. new v3 flag/outcome protocol cases), action, checkpoint, resume, proxy smoke and both real 357-clip banks pass.

### Runtime 1013c — PASS (scoped)

Two relayed Ramsey sword contacts during an ordinary held RightShoulder block were both resolved natively as **Blocked**. Client health stayed 56 (no sword damage call), host acknowledged two Blocked outcomes and posted two native attacker events with no failures, clone stayed 40, the duplicate resend was rejected with no extra effect, and restoration plus all 14 protected files passed. 17 samples show the native block scene. The client sheet shows the guard held toward Ramsey and the native "Defense Blocking" tutorial prompt after the first defended contact, with no damage effect ([visual review](../../work/natural-moveset/npc-native-block-v1013c/visual-review.json)).

Harness failures retained: 1013 (PowerShell 7 read-only `$Error` assignment in the aim helper, before any contact) and 1013b (run under Windows PowerShell 5.1, whose `File.Move` overload is missing; its "protected file changed" was a 5.1 false positive — an independent PowerShell 7 check found all 14 unchanged, and the fixture was restored before 1013c). **Test scripts must run under the pinned PowerShell 7** (`.cache/codex-runtimes/.../native/powershell/pwsh.exe`), which `Start-D2Lab.ps1` also requires.

### Still open (hypotheses / unverified)

- Host attacker block reaction is supported only by the posted-event count; the host camera was parked away, so it is not visually verified.
- Timed parry (results 1/2), parry-win stagger on the host, heavy/unblockable attacks, side/rear arcs under movement and latency, several simultaneous attackers, close-combat kicks (`npc_closecombat`) and other damage families.
