# Partner powers that touch the world (stage 8 area 4.6, #242): design notes

Read-only research, 2026-11-02. Labels: confirmed static, confirmed runtime, hypothesis. Paths are under `outputs/d2-coop-lab/src/`.

**The world diff cannot see these powers.** `--world-digest` has no NPC positions, hit reactions, alert state, rats or fish. A take needs a check specific to each power as well as the diff (confirmed static).

## Today

- **Blink Assault:**
  - The partner's kick hit (`blink_attack.damage`) is forwarded and applied on the host by `npcCombatApplyPeerNative` (`npc_combat.h:564-580`).
  - Only the amount and the attacker are set; the flags (+0x10), reactions (+0x18) and direction (+0x3c) keep their defaults.
  - Both worlds stagger at full health, with no knockdown (confirmed runtime: partner-corvo-power-v1805a, mission-partner-powers-v1819b, v1821a/b).
  - Hypothesis: the knockdown comes from the missing reactions, flags or direction.
- **Rat and fish possession:** nothing reaches the host.
  - Every possession reader is human-only (`native_possession_context.h:12-16`, `npc_authority_stream.h:739`, `shadow_stream.h:359`; confirmed static).
  - Ambient swarms are not synced, so one rat cannot be matched across worlds (v1830a, confirmed runtime).
- **Corpse possession:** in mission-partner-powers-v1833a, the partner's cast gave no visible possession. Even a host alone possessing a corpse did not move it under walk input (carry-body-v1644b), so the pass rule of v1833 may be wrong (hypothesis). The host's corpse follow admits only lethal takedown variants, not the possession variant 1000 (`npc_authority_protocol.h:86-88`).
- **Deadly Shade:**
  - The partner's shadow is streamed as kind 2. The host spawns no puppet for kind 2 (`native_shade_host.h:166-169`) after v1729c crashed the host.
  - The shadow's kills are forwarded: partner-shade-v1732a and v1736b killed guard 1 in both worlds.
  - The host's guards never perceive or fight the shadow.

## Minimal design (highest gain first)

1. **Deadly Shade:**
   - Replay `OnCloneSpawned` on the host. Following `FUN_14043e0c0` and `FUN_14043de90`: look up `FUN_1404e51c0(game+0x341068[team], caster position)`, take its component `1439ae070` and the clone definition at +0x6d8, and spawn with that result as parent. Then remove the decoy and post the stimuli 0x73 kind 5 and 0x4f (confirmed static).
   - The change is a kind-2 branch in `shadeHostSpawn` and `serviceShadeHost`. While the host's shadow lives, the partner's shade-owned forward is off.
   - Fallback: the proven decoy puppet for kind 2.
   - Lab switch: `coop_shade_native deadly_off`.
2. **Blink Assault:**
   - Capture the partner's flags, reactions and direction for the kick and carry them in a new `NpcCombatDamage::BlinkKick` (a protocol change), then apply them on the host. Also set `hostVulnerableUntil`, so knockdown chokes are admitted.
   - First a discovery take, comparing the host's own kick parameters with the forwarded ones.
   - Lab switch: `coop_blink_kick legacy`.
3. **Corpse possession:**
   - A corpse occupation reader, found from runtime discovery. PairedHold with variant 1000 and the corpse handle, and `npcPairedCorpseFollows` admits variant 1000.
   - Lab switch: `coop_possess_corpse off`.
4. **Rats and fish:**
   - Two possessed-animal bits in the pose flags (a pose version bump).
   - The host hides the partner's avatar (`PossessionBodyVisibility` lease) and spawns a native rat (or fish) at her root, following it the way the shade puppet does. This is presence only.
   - Lab switch: `coop_possess_animal off`.

## Take plan

**Harness, Route-MissionVM-v2068:**
- A power is equipped by granting it: `coop_grant_named` with its upgrades, in `command` steps with role client.
- `cast` needs `pitch`, a right trigger after the left (Blink Assault: RT about 1.1 s after a 1000 ms LT) and a walk, or a new `partnercast` step.

**Places on Edge of the World:**
- Checkpoint guard 01 at (4.43, 59.20, 15.95), with a front stand at (4.04, 61.67, 15.97) (v1833a).
- Blink Assault: 6 m in front of the guard.
- Deadly Shade: 5 m behind it.
- Corpse: after `partnerattack` on guard 01, 2.5 m back at pitch about 40.
- Rat swarms near it drift, and the fish location is unknown.

**Steps for each power:** a `diff` before, the cast, `--npc-activity` probes, a `diff` after and a `diff` 30 s later, all required. Each power also saves its own check:
- the shade host puppet;
- the kick parameters and the host's knockdown state;
- the corpse end gap;
- the animal puppet.

**Twins:** each power's own check must go red with its switch off, while the digest stays equal.
