# Native enemy contacts to the actual remote player

Updated14 October2026. Work follows [native perception/pursuit987](77-native-remote-perception.md). Scope is the observed native NPC sword contact family, across currently negotiated NPC bindings. [Audit991](../../work/movement-integration/npc-attack-audit-v991.json) records successful bounded runtime tests989 and990.

## Why the historical prototype is insufficient

`src/sword_link.h` requires a1HP story-character avatar, the exact escape guard1 and the old temporary Loyalists hostility override. The current custom Doppelganger is a native Player-faction40HP NPC, and the newly verified targeting comes from Ramsey and escape guard2 as well. Enabling the historical experiment would fail its identity gates. Its native local-player application helper remains useful evidence: `native_sword_damage.h` confirms constructor780260, result constructor780470, damage entry717950 and exact declaration `components/equipment/damage/npc_sword.damage` with base20, flags0x80, reactions1/1.

Takes982/984–986 independently record live enemy weapon-parent identities and hits to the clone. Those were clone damage only; the actual client player retained its original health. Persistent exact executable hash is in chapter77; original Steam installation/assets and saves remain protected.

## Candidate988 implementation

Source: `outputs/d2-coop-lab/src/npc_attack.h`, `npc_attack_protocol.h`, `npc_attack_protocol_tests.h`. Integration points are `damage_observer.h`, `npc_authority_stream.h`, `engine_bridge.cpp` and the existing lab console allowlist.

- Both instances opt in with `coop_npc_attack on`, only after native NPC stream bindings are ready. Each start creates a fresh nonce. Existing session/world epoch, local/peer actor arms, generation-bearing handles and rig identities constrain each exchange.
- Host damage callback recognizes the currently owned clone health component and a native sword weapon whose parent is a bound NPC attacker. The exact native declaration/amount/flags must agree. It copies bounded contact geometry into an engine-thread queue and returns a native empty result, preserving clone health. Capture expiry is250ms; queue capacity32. No callback performs network I/O or arbitrary native damage on a worker.
- The host sends ordered176-byte version1 contact packets using native SteamNetworkingSockets reliable transport. Packets carry no health value, damage amount or native pointer. The receiver resolves the local equivalent attacker from the existing binding.
- The actual client player receives a native717950 application with locally resolved sword declaration, native parameter/result constructors, hit offset and direction. Existing native difficulty, reactions and death processing remain responsible for the player outcome. A thread-local application scope prevents the receiver's replica filter from suppressing this one authorized call.
- Physical contacts from corresponding client-side NPC replicas are suppressed while this opt-in ownership is active. They must not add a second damage event. The current adapter recognizes only the observed native sword family; other damage families are unfinished.
- Events are consumed before application, duplicates do not apply damage twice, and host pending events require ordered acknowledgements. Gaps, changed identities/nonces, capture faults, unacknowledged work and native application failures stop the experiment. Stopping the NPC stream clears capture ownership before native teardown.

The option is off by default. This first implementation retires on a menu/world transition; it does not yet preserve/rearm attack-session intent automatically. The local host player's ordinary incoming enemy damage remains native and does not take the remote relay path. Partner friendly-fire/interaction filters remain earlier in the damage dispatch.

## Preserved build and runtime evidence

Archive: `work/movement-integration/candidate-npc-combat-v988`. DLL SHA256 C16BA967FD1876E59904BAE11D887036B16604D293068C5B62BC4344F1238481. Probe unchanged BAFEB687A25126DDFCF385E971AB0F58A5A57F5E23E2D0A40976882085F0C6AA. Both357-clip banks and movement protocol69 remain unchanged. Prior983 and966 archives are complete rollback points.

`Build-NpcAttack-v988.ps1` passes world/ownership, action, checkpoint, movement-resume, proxy smoke and real357-clip bank checks. New protocol tests reject stale sessions/epochs, role reversal, missing nonces/events, malformed geometry, nonfinite values, unknown versions/messages, native menu/death states and acknowledgement payload misuse. These are protocol checks, not proof of successful gameplay damage.

`work/natural-moveset/Test-NpcNativeAttack-v989.ps1` passes: a native Ramsey sword contact becomes one actual client-player damage event56→41; the clone remains40. Native base20 becomes15 through the player's own processing. Host event1 is acknowledged once; resending produces one duplicate and no further application or health loss. Independent damage-observer evidence identifies the actual player victim1 and attacker190318452. The client recording shows the native red damage effect and subsequent scripted retreat. Host video is retained as an unreviewed diagnostic.

`work/natural-moveset/Test-NpcNativeDeath-v990.ps1` passes: repeated native contacts produce client56→41→26→11→0. Both actual native Game Over screens appear (Corvo on host, Emily on client), while host health remains56. Exactly one coordinated retry/load completes on each peer, with original roles, native player health56, clone health40, fresh NPC arms and an explicitly rearmed attack scope. Old hits do not appear in the fresh scope. Both six-frame video sheets have been inspected. Recordings end before reload; load success is established by state/identity evidence. This capture ordering is a precaution following the unresolved graphics-module crash961, not a proven fix.

An additional host event5 arrives during the native death animation after four lethal applications. The already-dead client refuses another native application; there is no fifth subtraction. Graceful terminal acknowledgement and automatic attack renewal are follow-up work. These limitations remain visible in the audit rather than being hidden behind the overall bounded test result.

Both tests restore the isolated lab, inventory, player health and all14 protected files. Their four videos are included in the review manifest (1401 total at this publication). Original Steam data, both animation banks and local first-person behavior are unchanged.

Outstanding: parry/block and interruption behavior, more damage declarations, projectile/physics ownership, delayed contacts under latency, sustained overlapping enemies, automatic attack renewal, menu/save/map lifecycle, performance and full campaign coverage. The first actual hit, repeated lethal hits and shared retry are accepted only within the recorded Dunwall scope.
