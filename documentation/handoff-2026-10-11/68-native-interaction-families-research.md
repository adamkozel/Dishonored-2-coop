# Native interaction families: state lists, static uses and loot

Continuation827, 2026-10-13. This is read-only research following the [completed bounded food/stack pickup pass](67-food-and-grouped-pickup-validation.md). Live candidate821 is unchanged. No new native interaction mutation, state setter or network channel has been executed in this research. It identifies reusable systems for the next implementation.

## Exact runtime census

[Inspect-InteractionFamilies-v827.ps1](../../work/natural-moveset/Inspect-InteractionFamilies-v827.ps1) inspected113 entities per role: all current ArkUsable and ArkMovable objects, selected named static containers/drawers, and three NPC samples. It validated isolated PID/path/start, native gameplay, reciprocal component ownership, stable world and surviving entity identity. [Result](../../work/natural-moveset/native-interaction-families-v827/result.json), [host catalog](../../work/natural-moveset/native-interaction-families-v827/catalog-host.json), [client catalog](../../work/natural-moveset/native-interaction-families-v827/catalog-client.json) and per-component/declaration dumps preserve the evidence. All14 protected files matched. No game commands or synthetic inputs were sent.

The host census contains52 `arkCpntUsableWithStateList` components,20 `arkCpntUsableStatic` components and3 `arkCpntLoot` components on the sampled NPCs. These are not all instances of one ordinary door system:

- Safe doors and `esc_env_electric_cabinet_doors_01` use the state-list component, with two declared states in the inspected examples.
- Several lamps, sinks, clocks and audiographs also use the state-list component. A shared base component does not imply identical scripted side effects.
- Ring-locker open/lock interactors use the static usable component. They need their own event/progression observation rather than an invented open-state field.
- Named shelf drawers and small containers in this census have reference frame, rigid body and health components but no usable component. Their names alone do not establish that they are interactive containers.
- The sampled NPC loot components belong to actor inventories. Their lifetime and carried/spawned loot path remain separate from the51 static Simple pickups already validated.

[Paired differences](../../work/natural-moveset/native-interaction-families-v827/paired-differences.json) are retained. Role-specific audiographs and differently named spawned clockwork hack panels show why all runtime entity names cannot be assumed to match across worlds. Selection must distinguish stable authored objects from role-specific and spawned identities.

## Confirmed static structure and call path

The [saved Ghidra transcript](../../work/natural-moveset/native-interaction-families-v827/ghidra-usable-state-list-v827.json) includes searches, exact vtable bytes, callback disassembly, decompilation and cross-references. The original executable remains SHA-256`B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Runtime module bases were measured separately and saved in the census directory; pointer values from old worlds must not be reused.

The state-list component vtable maps to static address`141c5bfc8` (RVA`1c5bfc8`); its declaration vtable is`141c5c0c8`. Confirmed accesses in the code:

| Field | Observed use |
|---|---|
| component+0x08 / +0x10 | Declaration / owning entity |
| component+0x20 | Enabled byte used by native prompt selection |
| component+0x28 | Current state index |
| component+0x2c | Queued state index, initialized to-1 |
| component+0x30 mask0x02 | Transition-in-progress flag |
| component+0x34 | Native transition time from state entry+0x0c |
| component+0xd0 | Instigator entity handle for native use/notifications |
| component+0xd8 | Native animation component used by state application |
| declaration+0x70 / +0x78 low24 | State array / bounded count |
| declaration+0x80 | Initial state index |
| state entry stride0x80 | Per-state animation/prompt/timing data |

`14077a880` applies a state through native animation and notifications. It chooses entry/transition animations using entry bytes+8/+9, updates native animation scheduling and emits native manager events0x98/0x99 plus script descriptors`1439acf70`/`1439acfc0`. It accepts transition/event-control arguments. Their full gameplay implications require runtime observation before using this as a network entry point. Writing only the current index would omit these effects.

`14077aec0` advances to the next state, wrapping by the declared count, and refuses while the transition flag is set. It delegates to`14077a880` with native transition/notification behavior and marks the owner for servicing. `14077ad80` restores the declared initial state, queuing at+0x2c when a transition is active. `14077ae00` reads an instigator from its event argument, notifies another native system when valid and different from the owner, then calls the cycle function. This argument/lifetime must be verified; do not fabricate an event layout from the decompiler alone.

`14077b3b0` initializes state/transition data and registers callbacks for event IDs0x96,0x6e and0x14d. Callback disassembly shows0x6e routes to native cycle after resetting the instigator;0x14d routes to initial-state restoration;0x96 dispatches through vtable+0xf0 to the instigator-aware handler. `14077b610` removes these callbacks on shutdown. `14077b7e0` serializes the state index and a transition-related bit, making checkpoint behavior part of the native contract. `14077ace0` selects the native interaction prompt from declaration/state context. UI should continue using that prompt.

The Ghidra project contains saved bookmark`D2Coop_UsableStateList_827` at`14077a880`. The attempted lookup at`14077c640` returned no defined function; its raw address is retained in the transcript and is not assigned guessed behavior.

## Implementation direction, not validated behavior

Use one owned state-list reader and one host-ordered transition protocol for eligible authored objects, adapting the existing door session/checkpoint machinery. First capture an ordinary safe/cabinet interaction and its actual state, animation, collision and script notifications. Confirm the native entry point and queue behavior while moving before enabling remote application. Use desired state/revisions so duplicate network delivery cannot toggle twice. Preserve per-world identities and retire them across checkpoint load, just as the existing door/pickup channels do.

Initially select explicit compatible behavior/declaration classes, not arbitrary object names or all52 components. Static locker triggers, role-specific dialogue devices and NPC loot remain separate native families until their progression/ownership outcomes are observed. This keeps the work grouped without treating every entity or consumable as a custom implementation task.
