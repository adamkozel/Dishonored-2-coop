# Door-session renewal after a shared checkpoint

This extends [the native direction correction](59-native-door-conflicts-and-opening-side.md) toward ordinary co-op use. The exact game executable, original Steam API DLL, isolated saves and native local controls remain unchanged. Shared AI, general campaign progression and a native session-setup UI remain incomplete.

## Problem and implementation

The old ordinary-door catalog was scoped to a temporary movement-isolation fixture. Loading retired that fixture and all old door bindings. The previous checkpoint protocol restored roles and counterpart presentation, but using doors again required a new manual arm.

Candidate763 adds an explicit `coop_door_sync session` mode. It records session IDs, map ID and the paired world epochs as persistent intent. Native actor pointers, component generations, requests and channel arms remain tied to one world. This mode does not pause AI or require movement-isolation references. Legacy `coop_door_sync on` retains its earlier one-world fixture behavior; `off` clears both modes. These are development commands in the existing native console pipeline, not the finished player-facing session UI.

During a coordinated checkpoint transaction, the session retires the old catalog and clears its saved action packet. The native load path marks recovery pending before dispatching the load. It does not require the remote peer to still advertise the old map at this point: that peer may already be loading. It retains only the captured IDs for the later comparison.

The checkpoint protocol must reach both-complete, preserve the local role, and restore the requested local counterpart before it releases recovery. The catalog is rebuilt on the native engine thread only after both world epochs differ from their old values, the original session pair and map match, fresh poses advertise ordinary gameplay, and the native mutation phase is active. All native identities are reread, and each new door receives a new arm. The initial host baseline comes from the restored checkpoint, not the pre-load desired target. The previous open request cannot be applied merely because the authored door name survives loading.

An explicit off, changed/disconnected session, failed coordinated checkpoint, uncoordinated world change or expired recovery retires intent. Changed maps require a fresh explicit arm. The recovery deadline is60 seconds; existing native operation deadlines and exact eligibility checks remain intact. Locked, blocked, linked, broken and ambiguous doors are still excluded.

Source files:

- [door_session_scope.h](../../outputs/d2-coop-lab/src/door_session_scope.h): ID-only scope comparisons and new-epoch readiness.
- [door_sync.h](../../outputs/d2-coop-lab/src/door_sync.h): opt-in session, retirement, renewal, diagnostic counters and existing native apply path.
- [checkpoint_sync.h](../../outputs/d2-coop-lab/src/checkpoint_sync.h): coordinated transaction hooks and failure retirement.
- [engine_bridge.cpp](../../outputs/d2-coop-lab/src/engine_bridge.cpp): bounded native console command dispatch.
- [door_protocol_tests.h](../../outputs/d2-coop-lab/src/door_protocol_tests.h): rejects unchanged/half-renewed worlds, loading/pending peers, foreign sessions/maps and empty intent.

## Build identity and reproducibility

Archive: [candidate763](../../work/movement-integration/candidate-door-session-v763/identity.json). DLL SHA-256 `0C500AFF27B7D7EEA4D403CBFFD0A599B375C1AADD8232D9FDD5B020E97D6377`; unchanged probe `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Protocol69 and bank67/357 clips remain unchanged. The verified755 predecessor is preserved.

Build/deploy: [Build-DoorSession-v763.ps1](../../work/movement-integration/Build-DoorSession-v763.ps1), [Deploy-DoorSession-v763.ps1](../../work/movement-integration/Deploy-DoorSession-v763.ps1), [stage_door_session_v763.py](../../work/movement-integration/stage_door_session_v763.py). Staging is one-shot; do not rerun it over the existing archive. Native ownership, door scope/protocol, action/resume/checkpoint protocol, proxy smoke and the actual357-clip banks pass.

Runtime harness: [Test-DoorRearm-v764.ps1](../../work/natural-moveset/Test-DoorRearm-v764.ps1), reproducibly adapted by [prepare_door_rearm_v764.py](../../work/movement-integration/prepare_door_rearm_v764.py). It starts with the known safe Dunwall placement, opens the closet through native input, requests one shared load while both door machines are Moving, sends a duplicate peer retry, then verifies fresh bindings and another real native use. It contains exactly one initial door-enable command per role and none after loading.

The post-load phase explicitly checks movement isolation is inactive with zero held references and reads Ramsey's native decision component: started, scheduler-active and zero pause references. This proves the door subsystem no longer requires that fixture. It does not prove AI decisions or targets are shared between the two games.

## Runtime results and visual review

| Take | Scope | Result |
| --- | --- | --- |
| [764](../../work/natural-moveset/native-door-session-reload-client-v764/result.json) | Emily requests shared checkpoint while the closet door is moving | Five checks pass. Exactly one native loading entry/finish per process, one restored counterpart each, six new door arms, no inherited open request, then fresh opening synchronizes. |
| [765](../../work/natural-moveset/native-door-session-reload-host-v765/result.json) | Corvo requests the same interrupted load | Same five checks pass with roles swapped. |
| [766](../../work/natural-moveset/native-door-session-live-ai-v766/result.json) | Session starts with native AI already active, without movement isolation | Door interaction checks pass through closing, busy input and reopening. The overall test fails its unchanged-health assertion after a native guard attacks the unattended source. Complete restoration and protected-file checks pass. |

The accepted scope is captured in [audit767](../../work/movement-integration/native-door-session-audit-v767.json). Both reload cases count one actual loading entry at caller `787a55` and one finish at `7879cd` per role. Both-ready holds retain zero measured displacement;764 holds1531ms on host and1485ms on client. Each old catalog retires six bindings and retains the original native-call count. New bindings begin at revision1 with zero native calls; fresh post-load input converges at revision2 with one receiver call. No manual door re-enable occurs after loading. Ramsey's decision scheduler is active with zero pause references in both worlds after each reload.

All six source/observer contact sheets were inspected.764/765 show native loading screens, room return and fresh closet opening in both views; camera startup generally misses the pre-load motion, which is established by component observations.766 shows bathroom closing/reopening, followed later by a guard approach and visible hit feedback on the source. The observer remains by the bathroom. Saved [combat scene summary](../../work/natural-moveset/native-door-session-live-ai-v766/combat-scene-summary.json) includes native sword charge, step-back, medium attacks and hit subscenes. The harness did not save the exact failing health value or identify the attacker handle at the moment of damage; do not fabricate either. This evidence does not establish shared enemy authority or partner-caused damage.

The live-AI failure remains a failed full receipt, not a passed neutral test. Normal enemy damage is retained. All three takes restore complete inventories, player health56, counterpart health40 and14 protected save/settings hashes. No body-animation quality acceptance is inferred from these functional recordings.

## Remaining lifecycle and broader work

This is fixed-SLOT82, same-map recovery. Arbitrary save selection, another map, dynamic door creation/eligibility changes, group/script/lock semantics and native hosting/joining UI still need implementation and evidence. Scope checks for foreign sessions/maps are unit-tested; actual remote-account disconnect/reconnect and cross-map behavior are not accepted by these tests. Old action packets are cleared and new arms/epochs are observed, but this phase does not inject a deliberately stale packet after renewal.

A source inspection identifies the next lifecycle gap: a same-world native menu makes `coopWorldPairReady` false. The ordinary channel currently retires on that failed context rather than suspending and resuming, and its operation deadlines use wall time. Persistent same-world menu handling needs a dedicated runtime check and a bounded suspension policy. Repeated shared loads within one continuously armed session, shared death/retry and long-duration play also need coverage. These gaps must stay visible in the continuation roadmap.
