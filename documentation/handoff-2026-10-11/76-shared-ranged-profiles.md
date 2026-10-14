# Shared native ranged profiles

Updated 14 October 2026. The preserved integrated pistol milestone is candidate948 / [audit952](../../work/movement-integration/npc-ranged-audit-v952.json). This chapter extends that pipeline; complete combat and projectile replication remain unfinished.

## Confirmed native regular bolt observation

[Diagnostic953](../../work/natural-moveset/npc-crossbow-inspect-v953/result.json) used a normal controller trigger in the isolated Dunwall checkpoint. The selected native Corvo crossbow was equipped, started and quantity1. A generation-validated `arkProjectile` with vtable RVA1c5e758 and parent1 supplied the inflictor; the attacker was the actual local `arkPlayer`1. Callback snapshots were taken before transient projectile identity could disappear. No game-owned pointer or handle is sent over the network.

The declaration was `components/equipment/damage/bolt.damage`, key7b198783ddf206e3, native base amount15, flags0x100, reaction codes2/2, single hit region2/mask4 and hitDistance0. Its native declaration maximum is15. The exact declaration and source snapshots are retained in that take. Candidate948 intentionally lacked this profile and rejected it. The diagnostic and protected restoration passed; that is **not** a crossbow replication pass.

Ghidra's persistent exact-executable project contains the finding at140717950 under `D2Coop native ranged callback evidence v953`; the save receipt is [here](../../work/handoff-npc-crossbow-bookmark-v953.json).

## Shared implementation

Candidate954 replaces pistol-specific profile comparisons with a small exact-declaration table in `npc_combat_protocol.h`. Spread, regular bullet and regular bolt share the same ownership capture, contact geometry, ordered requests/commits, native parameter constructors, native damage call, duplicate handling and checkpoint retirement. Profiles define weapon family, declaration key, native flag, base amount bounds and reaction code. Exact local declaration RTTI, manager, path, maximum, flag and reaction must still agree before application.

Callback capture validates the selected weapon family against the observed source and distinguishes native weapon spread from transient projectile contact. The transport layout remains276 bytes; version6 rejects earlier pistol-only peers. Tests cover native bolt nonterminal/terminal results and rejected mixed weapon/declaration, out-of-range amount, zero damage and old-version packets, as well as the retained pistol tests.

The regular crossbow remains a contact-damage extension. It does not spawn a second gameplay projectile, replay native weapon firing or claim that embedded bolts, pickup recovery, wall impacts, special ammunition or projectile physics synchronize. Source-class and AI-state-dependent damage modifiers still require runtime evidence in each relevant context. Original first-person weapon behavior is retained.

## Runtime plan and acceptance boundary

`work/natural-moveset/Test-NpcCrossbow-v956.ps1` reuses the recorded pistol test, selects the native loaded crossbow, verifies the existing67/357 body bank and prop, and sends at most two normal inputs. It permits a living victim and checks matching health, duplicates, shared checkpoint, renewed identities and restoration. Native muzzle-flash creation is not required for a crossbow. Terminal victims additionally require the existing pre-death ownership and stopped-AI checks. Video inspection is separate from counter validation. Results are pending until the actual receipts are published.

Build/deploy scripts and candidate source/binaries are preserved under `work/movement-integration`. Source is `outputs/d2-coop-lab/src`. The original Steam game/assets/saves and the role-specific movement banks are unchanged.

## Runtime milestone954 and retained failures

[Audit959](../../work/movement-integration/npc-crossbow-audit-v959.json) checks host956 and client958 on candidate954 (DLLF7B860243A8D2CF6819DC7E145BD855ED96AA9B96C83733A4B763F57181376BA). Both pass native contact outcomes, duplicate suppression,67/357 body/prop counters, native death, shared checkpoint/rearm and protected restoration. Host956 used one head contact; client958 used two contacts, including a nonterminal first hit. Body-region-dependent native outcomes are retained rather than flattened to a fixed damage number.

Client957 is a failed full test: both contacts synchronized40→25→10, but the test teleported the source before the second shot, creating a0.19m vertical discontinuity and a movement-clock rejection. The reusable `Test-NpcRangedProfile-v960.ps1` removes that unnecessary reposition and accepts explicit weapon, aim, maximum inputs and nonterminal coverage parameters. No game movement bounds were relaxed.

Visual review is narrower than counter acceptance. Host956 source recording shows native crossbow hit/death; its observer is partly obstructed by furniture. Client958 source shows two contacts, reload and death, but the observer is heavily obscured by foliage. Client957's clearer observer shows Emily's existing ranged body/prop, yet its complete test failed as stated. No complete paired choreography, embedded bolt or corpse-physics acceptance follows.

Take960 was specifically intended to leave the guard alive. It instead produced a matching lethal head hit and fails that requested coverage. Both games restored cleanly. A controller aim-snapping hypothesis is under read-only investigation; no aim settings were changed. A separate one-shot client case961 is pending.

## Living-victim reload failure961 and independent recovery963

Client961 did produce one nonterminal native bolt contact: both instances40→25 with no death. The subsequent shared checkpoint lost the client process. Its loader log reports access violationc0000005, read address0x10, in `nvwgf2umx.dll`+0x300d7a. This locates the fault in a graphics-driver module; it does not establish the root cause or absolve mod rendering/resource lifetime. Both full test and automatic restoration failed and remain failed. Logs/status are preserved in the take's `crash-snapshot`. Separate recovery963 restarted only the isolated pair, reloaded the checkpoint and restored inventory; its own receipt is kept separately. No original installation, driver or global setting was changed.

Harness964 finishes its12-second native-window combat recordings before initiating world teardown, and status reads fail promptly if a known game process exits. This removes concurrent recording across reload as a possible factor; it is not a demonstrated crash fix. The following positive or negative test must retain that distinction.

The read-only live aim probe962b confirms controller model1 and strength1, mouse model0 in both restored instances. Runtime name fields are RVA397d6e0,397d760,397d7e0 respectively, with matching text and cached integer values. The debug reference describes model1 as snapping and0 as disabled. This supports a targeting explanation for unexpected head contacts; no aim preference was changed. Ghidra bookmark and save receipt: `work/handoff-aim-bookmark-v962.json`. The initial962 query raced world teardown and exited before obtaining a base; its failed read is retained separately.

## Bounded exact-aim helper966

A further one-shot client take965 also selected the head, so it fails its nonterminal-specific requirement despite matching lethal damage. Restoration passes. Repeating uncontrolled aim samples is not a reliable region-coverage strategy. Candidate966 adds only exact native console mode commands0/1/2 to the lab command allowlist and a read-only `--aim-settings` query. The query checks the exact cvar names, current text, cached integer and repeat-read stability at the runtime-verified fields. No default value or regular gameplay preference is changed.

Harness967 optionally records the current controller/mouse modes, sets controller mode0 through the native console, verifies the readback and restores the original controller mode before reload and in its failure cleanup. Restoration also checks that mouse mode did not change. Builds and existing protocol/ownership/357-clip checks pass. The helper is for deterministic native body-region testing, not a new first-person gameplay behavior. Capture still ends before checkpoint, and the unresolved961 graphics-module exception remains on the defect list.

## Published living-target investigation969

[Audit969](../../work/movement-integration/npc-nonterminal-audit-v969.json) retains960/961/965/967/968 and ten additional recordings. Candidate966 is live, with its original954 and948 milestones intact. Client967 passes one ordinary native bolt, matching40→25 health and zero deaths, duplicate rejection, body/prop counters, completed shared checkpoint/rearm and protected restoration. Six inspected frames per view show the guard react and recover upright; Emily is partly outside/obstructed in the observer view, so full paired choreography and grip are not accepted. Both original aim modes were restored. Recording ended before reload. This one success does not resolve crash961.

Host968 still hit region2 and killed the target with controller mode0. Its nonterminal requirement fails; automatic restoration passes. The contact direction in967 was approximately[0.09799,0.99444,0.03851], region3, and in968[0.10290,0.99353,0.04801], region2: both slightly upward despite requested native teleport pitch13.1. Controller snapping alone is therefore an insufficient explanation. No cause is assigned to another sensor without evidence.

The existing read-only `--player` query includes actual native view position/orientation. Harness970 reuses `Set-NativeInteractionAim-v663.ps1`, already used for interactions, to settle ordinary controller look on the guard's measured torso height and verify the camera ray before firing. It does not write camera matrices or relax movement bounds. This is a harness correction under investigation, not a change to normal first-person controls. Its results must be recorded separately.

## Measured aim and two-player contact results972

[Audit972](../../work/movement-integration/npc-dual-audit-v972.json) accepts host970: one measured native torso hit leaves both guards25 health, zero deaths, duplicate rejection and a complete shared checkpoint/rearm. Together with client967, both source roles now have living-victim reload coverage. This does not resolve the earlier961 crash.

Dual971 issues ordinary crossbow inputs12ms apart from Corvo and Emily. These are input-publication times, not simultaneous native callback execution. The host orders two native contacts40→25→10, both instances agree, one client request completes, no request remains pending, and duplicate attempts do not apply extra damage. Shared checkpoint/rearm and original aim/settings/protected restoration pass. The audit retains actual callback ticks. Four new recordings bring the runtime manifest to1383 at this publication.

The source and observer views have limits: host970 shows a torso hit and upright recovery, but its observer is obstructed. Dual971's host view is largely blocked by cloth/furniture; the client view shows a hit/reaction and recovery. Full paired choreography/grip is not accepted from these views.

Exact Ghidra callback140886d10 parses optional yaw/pitch and posts a deferred eventC2; therefore the observed settled-camera mismatch does not prove that `setviewpos` ignores pitch. The measured-camera helper avoids depending on that unresolved downstream behavior. [Persistent bookmark970](../../work/handoff-measured-aim-bookmark-v970.json) records this distinction.

Follow-on native enemy perception research is in [chapter77](77-native-remote-perception.md). Candidate966 remains the accepted combat fallback; experimental later candidates are separately identified.
