# interactions: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-15T19:24:55.573562+02:00. All fields below retain the catalog wording.

## door_open_close_locked

Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). The recordings show the door state but not the partner's body, so the door use cannot be judged visually for either character. 782/783/784: four consecutive same-map fixed-SLOT82 recoveries pass under original checkpoint arms: reload requested by each player, Corvo death, and Emily death with Corvo paused. Native shared GameOver, survivor health56 and zero input displacement, exactly one load/counterpart per role, fresh six-door catalog and native use after every retry.779 corrected GameOver13 mislabeled as wheel;782 closes pending-death retirement ordering gap reproduced in780.778 diagnostic race and note-quantity cleanup discrepancy retained and resolved. All item names/quantities, health and14 protected files restored.16 new paired recordings reviewed/indexed; complete co-op and movement remain unfinished. 828–834: exact owned native state-list reader and indexed animation API verified, then one Steam desired-state adapter covers four Dunwall windows. Ordinary host opening/client closing, whole-family batch, duplicate no-op, native menu retention with deferred local intent, one shared checkpoint each and fresh client use pass. Protected files and full isolated baseline restored. Other48 state-list objects, quest/AI side effects and full co-op remain incomplete.

````json
{
  "id": "door_open_close_locked",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_ordinary_door_continuous_manual_and_death_checkpoint_recovery",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "door open close locked",
  "motions": [
    "sword_walk_door_open_ccw_left",
    "sword_walk_door_open_ccw_right",
    "sword_walk_door_open_cw_left",
    "sword_walk_door_open_cw_right"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). The recordings show the door state but not the partner's body, so the door use cannot be judged visually for either character. 782/783/784: four consecutive same-map fixed-SLOT82 recoveries pass under original checkpoint arms: reload requested by each player, Corvo death, and Emily death with Corvo paused. Native shared GameOver, survivor health56 and zero input displacement, exactly one load/counterpart per role, fresh six-door catalog and native use after every retry.779 corrected GameOver13 mislabeled as wheel;782 closes pending-death retirement ordering gap reproduced in780.778 diagnostic race and note-quantity cleanup discrepancy retained and resolved. All item names/quantities, health and14 protected files restored.16 new paired recordings reviewed/indexed; complete co-op and movement remain unfinished. 828–834: exact owned native state-list reader and indexed animation API verified, then one Steam desired-state adapter covers four Dunwall windows. Ordinary host opening/client closing, whole-family batch, duplicate no-op, native menu retention with deferred local intent, one shared checkpoint each and fresh client use pass. Protected files and full isolated baseline restored. Other48 state-list objects, quest/AI side effects and full co-op remain incomplete.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-door-api-corvo-v708/host-unlocked-door-tap.mp4",
    "native-door-api-corvo-v708/observer.mp4",
    "native-door-api-corvo-v711/host-unlocked-door-tap.mp4",
    "native-door-api-corvo-v711/observer.mp4",
    "native-door-sync-corvo-v717/host-unlocked-door-tap.mp4",
    "native-door-sync-corvo-v717/observer.mp4",
    "native-door-api-corvo-v714/host-unlocked-door-tap.mp4",
    "native-door-api-corvo-v714/observer.mp4",
    "native-door-api-emily-v715/client-unlocked-door-tap.mp4",
    "native-door-api-emily-v715/observer.mp4",
    "native-door-sync-emily-v718/client-unlocked-door-tap.mp4",
    "native-door-sync-emily-v718/observer.mp4",
    "native-door-sync-corvo-v719/host-unlocked-door-tap.mp4",
    "native-door-sync-corvo-v719/observer.mp4",
    "native-door-traversal-emily-v722/host-unlocked-door-tap.mp4",
    "native-door-traversal-emily-v722/observer.mp4",
    "native-door-traversal-emily-v723/host-unlocked-door-tap.mp4",
    "native-door-traversal-emily-v723/observer.mp4",
    "native-door-traversal-corvo-v724/client-unlocked-door-tap.mp4",
    "native-door-traversal-corvo-v724/observer.mp4",
    "native-door-catalog-corvo-v729/host-unlocked-door-tap.mp4",
    "native-door-catalog-corvo-v729/observer.mp4",
    "native-door-catalog-second-emily-v730/client-unlocked-door-tap.mp4",
    "native-door-catalog-second-emily-v730/observer.mp4",
    "native-door-catalog-second-corvo-v731/host-unlocked-door-tap.mp4",
    "native-door-catalog-second-corvo-v731/observer.mp4",
    "native-door-mixed-host-first-v732/host-unlocked-door-tap.mp4",
    "native-door-mixed-host-first-v732/observer.mp4",
    "native-door-mixed-client-first-v733/client-unlocked-door-tap.mp4",
    "native-door-mixed-client-first-v733/observer.mp4",
    "native-door-catalog-second-view-v734/client-unlocked-door-tap.mp4",
    "native-door-catalog-second-view-v734/observer.mp4",
    "native-door-reload-host-v736/host-unlocked-door-tap.mp4",
    "native-door-reload-host-v736/observer.mp4",
    "native-door-reload-host-v737/host-unlocked-door-tap.mp4",
    "native-door-reload-host-v737/observer.mp4",
    "native-door-reload-client-v738/client-unlocked-door-tap.mp4",
    "native-door-reload-client-v738/observer.mp4",
    "native-door-reload-client-v741/client-unlocked-door-tap.mp4",
    "native-door-reload-client-v741/observer.mp4",
    "native-door-retry-probe-client-v743/client-unlocked-door-tap.mp4",
    "native-door-retry-probe-client-v743/observer.mp4",
    "native-door-retry-probe-client-v744/client-unlocked-door-tap.mp4",
    "native-door-retry-probe-client-v744/observer.mp4",
    "native-door-retry-probe-host-v745/host-unlocked-door-tap.mp4",
    "native-door-retry-probe-host-v745/observer.mp4",
    "native-door-overlap-host-first-v747/host-unlocked-door-tap.mp4",
    "native-door-overlap-host-first-v747/observer.mp4",
    "native-door-overlap-opposite-host-v748/host-unlocked-door-tap.mp4",
    "native-door-overlap-opposite-host-v748/observer.mp4",
    "native-door-close-reopen-host-v750/host-unlocked-door-tap.mp4",
    "native-door-close-reopen-host-v750/observer.mp4",
    "native-door-close-handoff-host-v751/host-unlocked-door-tap.mp4",
    "native-door-close-handoff-host-v751/observer.mp4",
    "native-door-busy-use-host-v752/host-unlocked-door-tap.mp4",
    "native-door-busy-use-host-v752/observer.mp4",
    "native-door-busy-use-client-v753/client-unlocked-door-tap.mp4",
    "native-door-busy-use-client-v753/observer.mp4",
    "native-door-simultaneous-sides-v754/host-unlocked-door-tap.mp4",
    "native-door-simultaneous-sides-v754/observer.mp4",
    "native-door-simultaneous-sides-host-v756/host-unlocked-door-tap.mp4",
    "native-door-simultaneous-sides-host-v756/observer.mp4",
    "native-door-simultaneous-sides-client-v757/client-unlocked-door-tap.mp4",
    "native-door-simultaneous-sides-client-v757/observer.mp4",
    "native-door-busy-use-host-v758/host-unlocked-door-tap.mp4",
    "native-door-busy-use-host-v758/observer.mp4",
    "native-door-busy-use-client-v759/client-unlocked-door-tap.mp4",
    "native-door-busy-use-client-v759/observer.mp4",
    "native-door-simultaneous-sides-reverse-v760/host-unlocked-door-tap.mp4",
    "native-door-simultaneous-sides-reverse-v760/observer.mp4",
    "native-door-session-reload-client-v764/client-unlocked-door-tap.mp4",
    "native-door-session-reload-client-v764/observer.mp4",
    "native-door-session-reload-host-v765/host-unlocked-door-tap.mp4",
    "native-door-session-reload-host-v765/observer.mp4",
    "native-door-session-live-ai-v766/host-unlocked-door-tap.mp4",
    "native-door-session-live-ai-v766/observer.mp4",
    "native-door-menu-baseline-v768/host.mp4",
    "native-door-menu-baseline-v768/client.mp4",
    "native-door-menu-host-v770/host.mp4",
    "native-door-menu-host-v770/client.mp4",
    "native-door-menu-client-v771/host.mp4",
    "native-door-menu-client-v771/client.mp4",
    "native-door-menu-moving-host-v772/host.mp4",
    "native-door-menu-moving-host-v772/client.mp4",
    "native-door-menu-moving-client-v773/host.mp4",
    "native-door-menu-moving-client-v773/client.mp4",
    "native-door-menu-use-host-v774/host.mp4",
    "native-door-menu-use-host-v774/client.mp4",
    "native-door-menu-use-client-v775/host.mp4",
    "native-door-menu-use-client-v775/client.mp4",
    "native-door-menu-reload-client-v776/client-unlocked-door-tap.mp4",
    "native-door-menu-reload-client-v776/observer.mp4",
    "native-continuous-recovery-v778/01-host-reload/host.mp4",
    "native-continuous-recovery-v778/01-host-reload/client.mp4",
    "native-continuous-recovery-v780/01-host-reload/host.mp4",
    "native-continuous-recovery-v780/01-host-reload/client.mp4",
    "native-continuous-recovery-v780/02-client-reload/host.mp4",
    "native-continuous-recovery-v780/02-client-reload/client.mp4",
    "native-continuous-recovery-v780/03-corvo-death/host.mp4",
    "native-continuous-recovery-v780/03-corvo-death/client.mp4",
    "native-continuous-recovery-v783/01-host-reload/host.mp4",
    "native-continuous-recovery-v783/01-host-reload/client.mp4",
    "native-continuous-recovery-v783/02-client-reload/host.mp4",
    "native-continuous-recovery-v783/02-client-reload/client.mp4",
    "native-continuous-recovery-v783/03-corvo-death/host.mp4",
    "native-continuous-recovery-v783/03-corvo-death/client.mp4",
    "native-continuous-recovery-v783/04-emily-death-paused-survivor/host.mp4",
    "native-continuous-recovery-v783/04-emily-death-paused-survivor/client.mp4",
    "native-window-input-v828d/client.mp4",
    "native-window-input-v828d/host.mp4",
    "native-window-api-v830b/client.mp4",
    "native-window-api-v830b/host.mp4",
    "shared-window-family-v832b/client.mp4",
    "shared-window-family-v832b/host.mp4",
    "shared-window-recovery-v833b/fresh-client.mp4",
    "shared-window-recovery-v833b/fresh-host.mp4",
    "shared-window-recovery-v833b/menu-client.mp4",
    "shared-window-recovery-v833b/menu-host.mp4",
    "shared-window-recovery-v833b/reload-client.mp4",
    "shared-window-recovery-v833b/reload-host.mp4",
    "native-door-corvo-v638/host-door-tap.mp4",
    "native-door-corvo-v638/observer.mp4",
    "native-unlocked-door-corvo-v640/host-unlocked-door-tap.mp4",
    "native-unlocked-door-corvo-v640/observer.mp4",
    "native-unlocked-door-corvo-v641/host-unlocked-door-tap.mp4",
    "native-unlocked-door-corvo-v641/observer.mp4",
    "native-unlocked-door-emily-v642/client-unlocked-door-tap.mp4",
    "native-unlocked-door-emily-v642/observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-unlocked-door-corvo-v641/result.json",
    "native-unlocked-door-emily-v642/result.json",
    "native-door-api-corvo-v714/result.json",
    "native-door-api-emily-v715/result.json",
    "native-door-sync-emily-v718/result.json",
    "native-door-sync-corvo-v719/result.json",
    "native-door-traversal-emily-v723/result.json",
    "native-door-traversal-corvo-v724/result.json",
    "native-door-catalog-corvo-v729/result.json",
    "native-door-catalog-second-emily-v730/result.json",
    "native-door-catalog-second-corvo-v731/result.json",
    "native-door-mixed-host-first-v732/result.json",
    "native-door-mixed-client-first-v733/result.json",
    "native-door-catalog-second-view-v734/result.json",
    "native-door-reload-host-v737/result.json",
    "native-door-reload-client-v741/result.json",
    "native-door-retry-probe-client-v744/result.json",
    "native-door-retry-probe-host-v745/result.json",
    "native-door-busy-use-host-v752/result.json",
    "native-door-busy-use-client-v753/result.json",
    "native-door-simultaneous-sides-host-v756/result.json",
    "native-door-simultaneous-sides-client-v757/result.json",
    "native-door-busy-use-host-v758/result.json",
    "native-door-busy-use-client-v759/result.json",
    "native-door-simultaneous-sides-reverse-v760/result.json",
    "native-door-session-reload-client-v764/result.json",
    "native-door-session-reload-host-v765/result.json",
    "native-door-session-live-ai-v766/result.json",
    "native-door-menu-baseline-v768/result.json",
    "native-door-menu-host-v770/result.json",
    "native-door-menu-client-v771/result.json",
    "native-door-menu-moving-host-v772/result.json",
    "native-door-menu-moving-client-v773/result.json",
    "native-door-menu-use-host-v774/result.json",
    "native-door-menu-use-client-v775/result.json",
    "native-door-menu-reload-client-v776/result.json",
    "native-continuous-recovery-v783/result.json",
    "native-window-input-v828d/result.json",
    "native-window-api-v830b/result.json",
    "shared-window-family-v832b/result.json",
    "shared-window-recovery-v833b/result.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json"
  ]
}
````

- [evidence: native-unlocked-door-corvo-v641/result.json](<../../../../work/natural-moveset/native-unlocked-door-corvo-v641/result.json>)
- [evidence: native-unlocked-door-emily-v642/result.json](<../../../../work/natural-moveset/native-unlocked-door-emily-v642/result.json>)
- [evidence: native-door-api-corvo-v714/result.json](<../../../../work/natural-moveset/native-door-api-corvo-v714/result.json>)
- [evidence: native-door-api-emily-v715/result.json](<../../../../work/natural-moveset/native-door-api-emily-v715/result.json>)
- [evidence: native-door-sync-emily-v718/result.json](<../../../../work/natural-moveset/native-door-sync-emily-v718/result.json>)
- [evidence: native-door-sync-corvo-v719/result.json](<../../../../work/natural-moveset/native-door-sync-corvo-v719/result.json>)
- [evidence: native-door-traversal-emily-v723/result.json](<../../../../work/natural-moveset/native-door-traversal-emily-v723/result.json>)
- [evidence: native-door-traversal-corvo-v724/result.json](<../../../../work/natural-moveset/native-door-traversal-corvo-v724/result.json>)
- [evidence: native-door-catalog-corvo-v729/result.json](<../../../../work/natural-moveset/native-door-catalog-corvo-v729/result.json>)
- [evidence: native-door-catalog-second-emily-v730/result.json](<../../../../work/natural-moveset/native-door-catalog-second-emily-v730/result.json>)
- [evidence: native-door-catalog-second-corvo-v731/result.json](<../../../../work/natural-moveset/native-door-catalog-second-corvo-v731/result.json>)
- [evidence: native-door-mixed-host-first-v732/result.json](<../../../../work/natural-moveset/native-door-mixed-host-first-v732/result.json>)
- [evidence: native-door-mixed-client-first-v733/result.json](<../../../../work/natural-moveset/native-door-mixed-client-first-v733/result.json>)
- [evidence: native-door-catalog-second-view-v734/result.json](<../../../../work/natural-moveset/native-door-catalog-second-view-v734/result.json>)
- [evidence: native-door-reload-host-v737/result.json](<../../../../work/natural-moveset/native-door-reload-host-v737/result.json>)
- [evidence: native-door-reload-client-v741/result.json](<../../../../work/natural-moveset/native-door-reload-client-v741/result.json>)
- [evidence: native-door-retry-probe-client-v744/result.json](<../../../../work/natural-moveset/native-door-retry-probe-client-v744/result.json>)
- [evidence: native-door-retry-probe-host-v745/result.json](<../../../../work/natural-moveset/native-door-retry-probe-host-v745/result.json>)
- [evidence: native-door-busy-use-host-v752/result.json](<../../../../work/natural-moveset/native-door-busy-use-host-v752/result.json>)
- [evidence: native-door-busy-use-client-v753/result.json](<../../../../work/natural-moveset/native-door-busy-use-client-v753/result.json>)
- [evidence: native-door-simultaneous-sides-host-v756/result.json](<../../../../work/natural-moveset/native-door-simultaneous-sides-host-v756/result.json>)
- [evidence: native-door-simultaneous-sides-client-v757/result.json](<../../../../work/natural-moveset/native-door-simultaneous-sides-client-v757/result.json>)
- [evidence: native-door-busy-use-host-v758/result.json](<../../../../work/natural-moveset/native-door-busy-use-host-v758/result.json>)
- [evidence: native-door-busy-use-client-v759/result.json](<../../../../work/natural-moveset/native-door-busy-use-client-v759/result.json>)
- [evidence: native-door-simultaneous-sides-reverse-v760/result.json](<../../../../work/natural-moveset/native-door-simultaneous-sides-reverse-v760/result.json>)
- [evidence: native-door-session-reload-client-v764/result.json](<../../../../work/natural-moveset/native-door-session-reload-client-v764/result.json>)
- [evidence: native-door-session-reload-host-v765/result.json](<../../../../work/natural-moveset/native-door-session-reload-host-v765/result.json>)
- [evidence: native-door-session-live-ai-v766/result.json](<../../../../work/natural-moveset/native-door-session-live-ai-v766/result.json>)
- [evidence: native-door-menu-baseline-v768/result.json](<../../../../work/natural-moveset/native-door-menu-baseline-v768/result.json>)
- [evidence: native-door-menu-host-v770/result.json](<../../../../work/natural-moveset/native-door-menu-host-v770/result.json>)
- [evidence: native-door-menu-client-v771/result.json](<../../../../work/natural-moveset/native-door-menu-client-v771/result.json>)
- [evidence: native-door-menu-moving-host-v772/result.json](<../../../../work/natural-moveset/native-door-menu-moving-host-v772/result.json>)
- [evidence: native-door-menu-moving-client-v773/result.json](<../../../../work/natural-moveset/native-door-menu-moving-client-v773/result.json>)
- [evidence: native-door-menu-use-host-v774/result.json](<../../../../work/natural-moveset/native-door-menu-use-host-v774/result.json>)
- [evidence: native-door-menu-use-client-v775/result.json](<../../../../work/natural-moveset/native-door-menu-use-client-v775/result.json>)
- [evidence: native-door-menu-reload-client-v776/result.json](<../../../../work/natural-moveset/native-door-menu-reload-client-v776/result.json>)
- [evidence: native-continuous-recovery-v783/result.json](<../../../../work/natural-moveset/native-continuous-recovery-v783/result.json>)
- [evidence: native-window-input-v828d/result.json](<../../../../work/natural-moveset/native-window-input-v828d/result.json>)
- [evidence: native-window-api-v830b/result.json](<../../../../work/natural-moveset/native-window-api-v830b/result.json>)
- [evidence: shared-window-family-v832b/result.json](<../../../../work/natural-moveset/shared-window-family-v832b/result.json>)
- [evidence: shared-window-recovery-v833b/result.json](<../../../../work/natural-moveset/shared-window-recovery-v833b/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [in_game_recordings: native-door-api-corvo-v708/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-api-corvo-v708/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-api-corvo-v708/observer.mp4](<../../../../work/natural-moveset/native-door-api-corvo-v708/observer.mp4>)
- [in_game_recordings: native-door-api-corvo-v711/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-api-corvo-v711/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-api-corvo-v711/observer.mp4](<../../../../work/natural-moveset/native-door-api-corvo-v711/observer.mp4>)
- [in_game_recordings: native-door-sync-corvo-v717/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-sync-corvo-v717/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-sync-corvo-v717/observer.mp4](<../../../../work/natural-moveset/native-door-sync-corvo-v717/observer.mp4>)
- [in_game_recordings: native-door-api-corvo-v714/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-api-corvo-v714/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-api-corvo-v714/observer.mp4](<../../../../work/natural-moveset/native-door-api-corvo-v714/observer.mp4>)
- [in_game_recordings: native-door-api-emily-v715/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-api-emily-v715/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-api-emily-v715/observer.mp4](<../../../../work/natural-moveset/native-door-api-emily-v715/observer.mp4>)
- [in_game_recordings: native-door-sync-emily-v718/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-sync-emily-v718/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-sync-emily-v718/observer.mp4](<../../../../work/natural-moveset/native-door-sync-emily-v718/observer.mp4>)
- [in_game_recordings: native-door-sync-corvo-v719/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-sync-corvo-v719/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-sync-corvo-v719/observer.mp4](<../../../../work/natural-moveset/native-door-sync-corvo-v719/observer.mp4>)
- [in_game_recordings: native-door-traversal-emily-v722/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-traversal-emily-v722/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-traversal-emily-v722/observer.mp4](<../../../../work/natural-moveset/native-door-traversal-emily-v722/observer.mp4>)
- [in_game_recordings: native-door-traversal-emily-v723/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-traversal-emily-v723/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-traversal-emily-v723/observer.mp4](<../../../../work/natural-moveset/native-door-traversal-emily-v723/observer.mp4>)
- [in_game_recordings: native-door-traversal-corvo-v724/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-traversal-corvo-v724/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-traversal-corvo-v724/observer.mp4](<../../../../work/natural-moveset/native-door-traversal-corvo-v724/observer.mp4>)
- [in_game_recordings: native-door-catalog-corvo-v729/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-catalog-corvo-v729/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-catalog-corvo-v729/observer.mp4](<../../../../work/natural-moveset/native-door-catalog-corvo-v729/observer.mp4>)
- [in_game_recordings: native-door-catalog-second-emily-v730/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-catalog-second-emily-v730/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-catalog-second-emily-v730/observer.mp4](<../../../../work/natural-moveset/native-door-catalog-second-emily-v730/observer.mp4>)
- [in_game_recordings: native-door-catalog-second-corvo-v731/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-catalog-second-corvo-v731/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-catalog-second-corvo-v731/observer.mp4](<../../../../work/natural-moveset/native-door-catalog-second-corvo-v731/observer.mp4>)
- [in_game_recordings: native-door-mixed-host-first-v732/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-mixed-host-first-v732/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-mixed-host-first-v732/observer.mp4](<../../../../work/natural-moveset/native-door-mixed-host-first-v732/observer.mp4>)
- [in_game_recordings: native-door-mixed-client-first-v733/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-mixed-client-first-v733/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-mixed-client-first-v733/observer.mp4](<../../../../work/natural-moveset/native-door-mixed-client-first-v733/observer.mp4>)
- [in_game_recordings: native-door-catalog-second-view-v734/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-catalog-second-view-v734/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-catalog-second-view-v734/observer.mp4](<../../../../work/natural-moveset/native-door-catalog-second-view-v734/observer.mp4>)
- [in_game_recordings: native-door-reload-host-v736/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-reload-host-v736/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-reload-host-v736/observer.mp4](<../../../../work/natural-moveset/native-door-reload-host-v736/observer.mp4>)
- [in_game_recordings: native-door-reload-host-v737/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-reload-host-v737/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-reload-host-v737/observer.mp4](<../../../../work/natural-moveset/native-door-reload-host-v737/observer.mp4>)
- [in_game_recordings: native-door-reload-client-v738/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-reload-client-v738/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-reload-client-v738/observer.mp4](<../../../../work/natural-moveset/native-door-reload-client-v738/observer.mp4>)
- [in_game_recordings: native-door-reload-client-v741/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-reload-client-v741/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-reload-client-v741/observer.mp4](<../../../../work/natural-moveset/native-door-reload-client-v741/observer.mp4>)
- [in_game_recordings: native-door-retry-probe-client-v743/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-retry-probe-client-v743/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-retry-probe-client-v743/observer.mp4](<../../../../work/natural-moveset/native-door-retry-probe-client-v743/observer.mp4>)
- [in_game_recordings: native-door-retry-probe-client-v744/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-retry-probe-client-v744/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-retry-probe-client-v744/observer.mp4](<../../../../work/natural-moveset/native-door-retry-probe-client-v744/observer.mp4>)
- [in_game_recordings: native-door-retry-probe-host-v745/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-retry-probe-host-v745/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-retry-probe-host-v745/observer.mp4](<../../../../work/natural-moveset/native-door-retry-probe-host-v745/observer.mp4>)
- [in_game_recordings: native-door-overlap-host-first-v747/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-overlap-host-first-v747/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-overlap-host-first-v747/observer.mp4](<../../../../work/natural-moveset/native-door-overlap-host-first-v747/observer.mp4>)
- [in_game_recordings: native-door-overlap-opposite-host-v748/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-overlap-opposite-host-v748/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-overlap-opposite-host-v748/observer.mp4](<../../../../work/natural-moveset/native-door-overlap-opposite-host-v748/observer.mp4>)
- [in_game_recordings: native-door-close-reopen-host-v750/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-close-reopen-host-v750/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-close-reopen-host-v750/observer.mp4](<../../../../work/natural-moveset/native-door-close-reopen-host-v750/observer.mp4>)
- [in_game_recordings: native-door-close-handoff-host-v751/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-close-handoff-host-v751/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-close-handoff-host-v751/observer.mp4](<../../../../work/natural-moveset/native-door-close-handoff-host-v751/observer.mp4>)
- [in_game_recordings: native-door-busy-use-host-v752/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-busy-use-host-v752/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-busy-use-host-v752/observer.mp4](<../../../../work/natural-moveset/native-door-busy-use-host-v752/observer.mp4>)
- [in_game_recordings: native-door-busy-use-client-v753/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-busy-use-client-v753/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-busy-use-client-v753/observer.mp4](<../../../../work/natural-moveset/native-door-busy-use-client-v753/observer.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-v754/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-v754/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-v754/observer.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-v754/observer.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-host-v756/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-host-v756/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-host-v756/observer.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-host-v756/observer.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-client-v757/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-client-v757/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-client-v757/observer.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-client-v757/observer.mp4>)
- [in_game_recordings: native-door-busy-use-host-v758/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-busy-use-host-v758/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-busy-use-host-v758/observer.mp4](<../../../../work/natural-moveset/native-door-busy-use-host-v758/observer.mp4>)
- [in_game_recordings: native-door-busy-use-client-v759/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-busy-use-client-v759/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-busy-use-client-v759/observer.mp4](<../../../../work/natural-moveset/native-door-busy-use-client-v759/observer.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-reverse-v760/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-reverse-v760/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-simultaneous-sides-reverse-v760/observer.mp4](<../../../../work/natural-moveset/native-door-simultaneous-sides-reverse-v760/observer.mp4>)
- [in_game_recordings: native-door-session-reload-client-v764/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-session-reload-client-v764/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-session-reload-client-v764/observer.mp4](<../../../../work/natural-moveset/native-door-session-reload-client-v764/observer.mp4>)
- [in_game_recordings: native-door-session-reload-host-v765/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-session-reload-host-v765/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-session-reload-host-v765/observer.mp4](<../../../../work/natural-moveset/native-door-session-reload-host-v765/observer.mp4>)
- [in_game_recordings: native-door-session-live-ai-v766/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-session-live-ai-v766/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-session-live-ai-v766/observer.mp4](<../../../../work/natural-moveset/native-door-session-live-ai-v766/observer.mp4>)
- [in_game_recordings: native-door-menu-baseline-v768/host.mp4](<../../../../work/natural-moveset/native-door-menu-baseline-v768/host.mp4>)
- [in_game_recordings: native-door-menu-baseline-v768/client.mp4](<../../../../work/natural-moveset/native-door-menu-baseline-v768/client.mp4>)
- [in_game_recordings: native-door-menu-host-v770/host.mp4](<../../../../work/natural-moveset/native-door-menu-host-v770/host.mp4>)
- [in_game_recordings: native-door-menu-host-v770/client.mp4](<../../../../work/natural-moveset/native-door-menu-host-v770/client.mp4>)
- [in_game_recordings: native-door-menu-client-v771/host.mp4](<../../../../work/natural-moveset/native-door-menu-client-v771/host.mp4>)
- [in_game_recordings: native-door-menu-client-v771/client.mp4](<../../../../work/natural-moveset/native-door-menu-client-v771/client.mp4>)
- [in_game_recordings: native-door-menu-moving-host-v772/host.mp4](<../../../../work/natural-moveset/native-door-menu-moving-host-v772/host.mp4>)
- [in_game_recordings: native-door-menu-moving-host-v772/client.mp4](<../../../../work/natural-moveset/native-door-menu-moving-host-v772/client.mp4>)
- [in_game_recordings: native-door-menu-moving-client-v773/host.mp4](<../../../../work/natural-moveset/native-door-menu-moving-client-v773/host.mp4>)
- [in_game_recordings: native-door-menu-moving-client-v773/client.mp4](<../../../../work/natural-moveset/native-door-menu-moving-client-v773/client.mp4>)
- [in_game_recordings: native-door-menu-use-host-v774/host.mp4](<../../../../work/natural-moveset/native-door-menu-use-host-v774/host.mp4>)
- [in_game_recordings: native-door-menu-use-host-v774/client.mp4](<../../../../work/natural-moveset/native-door-menu-use-host-v774/client.mp4>)
- [in_game_recordings: native-door-menu-use-client-v775/host.mp4](<../../../../work/natural-moveset/native-door-menu-use-client-v775/host.mp4>)
- [in_game_recordings: native-door-menu-use-client-v775/client.mp4](<../../../../work/natural-moveset/native-door-menu-use-client-v775/client.mp4>)
- [in_game_recordings: native-door-menu-reload-client-v776/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-door-menu-reload-client-v776/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-door-menu-reload-client-v776/observer.mp4](<../../../../work/natural-moveset/native-door-menu-reload-client-v776/observer.mp4>)
- [in_game_recordings: native-continuous-recovery-v778/01-host-reload/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v778/01-host-reload/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v778/01-host-reload/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v778/01-host-reload/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v780/01-host-reload/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v780/01-host-reload/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v780/01-host-reload/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v780/01-host-reload/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v780/02-client-reload/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v780/02-client-reload/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v780/02-client-reload/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v780/02-client-reload/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v780/03-corvo-death/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v780/03-corvo-death/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v780/03-corvo-death/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v780/03-corvo-death/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/01-host-reload/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/01-host-reload/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/01-host-reload/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/01-host-reload/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/02-client-reload/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/02-client-reload/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/02-client-reload/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/02-client-reload/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/03-corvo-death/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/03-corvo-death/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/03-corvo-death/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/03-corvo-death/client.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/04-emily-death-paused-survivor/host.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/04-emily-death-paused-survivor/host.mp4>)
- [in_game_recordings: native-continuous-recovery-v783/04-emily-death-paused-survivor/client.mp4](<../../../../work/natural-moveset/native-continuous-recovery-v783/04-emily-death-paused-survivor/client.mp4>)
- [in_game_recordings: native-window-input-v828d/client.mp4](<../../../../work/natural-moveset/native-window-input-v828d/client.mp4>)
- [in_game_recordings: native-window-input-v828d/host.mp4](<../../../../work/natural-moveset/native-window-input-v828d/host.mp4>)
- [in_game_recordings: native-window-api-v830b/client.mp4](<../../../../work/natural-moveset/native-window-api-v830b/client.mp4>)
- [in_game_recordings: native-window-api-v830b/host.mp4](<../../../../work/natural-moveset/native-window-api-v830b/host.mp4>)
- [in_game_recordings: shared-window-family-v832b/client.mp4](<../../../../work/natural-moveset/shared-window-family-v832b/client.mp4>)
- [in_game_recordings: shared-window-family-v832b/host.mp4](<../../../../work/natural-moveset/shared-window-family-v832b/host.mp4>)
- [in_game_recordings: shared-window-recovery-v833b/fresh-client.mp4](<../../../../work/natural-moveset/shared-window-recovery-v833b/fresh-client.mp4>)
- [in_game_recordings: shared-window-recovery-v833b/fresh-host.mp4](<../../../../work/natural-moveset/shared-window-recovery-v833b/fresh-host.mp4>)
- [in_game_recordings: shared-window-recovery-v833b/menu-client.mp4](<../../../../work/natural-moveset/shared-window-recovery-v833b/menu-client.mp4>)
- [in_game_recordings: shared-window-recovery-v833b/menu-host.mp4](<../../../../work/natural-moveset/shared-window-recovery-v833b/menu-host.mp4>)
- [in_game_recordings: shared-window-recovery-v833b/reload-client.mp4](<../../../../work/natural-moveset/shared-window-recovery-v833b/reload-client.mp4>)
- [in_game_recordings: shared-window-recovery-v833b/reload-host.mp4](<../../../../work/natural-moveset/shared-window-recovery-v833b/reload-host.mp4>)
- [in_game_recordings: native-door-corvo-v638/host-door-tap.mp4](<../../../../work/natural-moveset/native-door-corvo-v638/host-door-tap.mp4>)
- [in_game_recordings: native-door-corvo-v638/observer.mp4](<../../../../work/natural-moveset/native-door-corvo-v638/observer.mp4>)
- [in_game_recordings: native-unlocked-door-corvo-v640/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-unlocked-door-corvo-v640/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-unlocked-door-corvo-v640/observer.mp4](<../../../../work/natural-moveset/native-unlocked-door-corvo-v640/observer.mp4>)
- [in_game_recordings: native-unlocked-door-corvo-v641/host-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-unlocked-door-corvo-v641/host-unlocked-door-tap.mp4>)
- [in_game_recordings: native-unlocked-door-corvo-v641/observer.mp4](<../../../../work/natural-moveset/native-unlocked-door-corvo-v641/observer.mp4>)
- [in_game_recordings: native-unlocked-door-emily-v642/client-unlocked-door-tap.mp4](<../../../../work/natural-moveset/native-unlocked-door-emily-v642/client-unlocked-door-tap.mp4>)
- [in_game_recordings: native-unlocked-door-emily-v642/observer.mp4](<../../../../work/natural-moveset/native-unlocked-door-emily-v642/observer.mp4>)
## lever_button_crank_wheel

Visual sweep 1260: accepted (corvo accept, emily accept). Mechanism use reads as a step-in, reach or crank and recovery for both partners; hand targeting on the mechanism is approximate. Reach, operation and recovery studies with full-body weight transfer and neutral contact fixtures. Palm grip is fitted independently from the wrist. Exact native target geometry, fingertip contact, continuous wheel regripping and game-state binding remain pending. 1210c (audit 1211): confirmed runtime native source - each native use enters player state 75 (vtable 0x1c86ab8) briefly; no native player lever/button animation exists, so binding = state 75 + focused usable -> authored remote reach/operate clip. 1213b/1214c (audit 1215): native use (state 75 + focused entity) drives the authored interaction on the partner body through Steam (protocol 74, bank 71 clips 369-373), both roles; generic reach on a Dunwall window handle verified, Emily visual accepted. Lever/crank/wheel variants classified by name but untested on real mechanisms; host-source clip ended early once. Audit 1229: real mechanisms, both roles - generic: client/host, lever: client/host, crank: client/host, wheel_valve: client/host, key_lock: client/host. Audit 1235 (#6, 1213k): hand reach toward the used entity origin, both roles on the vise; Corvo and Emily crank reaches visually accepted.

````json
{
  "id": "lever_button_crank_wheel",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_use_state75_real_mechanisms_generic_lever_crank_wheel_valve_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "lever button crank wheel",
  "motions": [
    "interact_button_sequence",
    "interact_lever_sequence",
    "interact_crank_sequence",
    "interact_wheel_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Mechanism use reads as a step-in, reach or crank and recovery for both partners; hand targeting on the mechanism is approximate. Reach, operation and recovery studies with full-body weight transfer and neutral contact fixtures. Palm grip is fitted independently from the wrist. Exact native target geometry, fingertip contact, continuous wheel regripping and game-state binding remain pending. 1210c (audit 1211): confirmed runtime native source - each native use enters player state 75 (vtable 0x1c86ab8) briefly; no native player lever/button animation exists, so binding = state 75 + focused usable -> authored remote reach/operate clip. 1213b/1214c (audit 1215): native use (state 75 + focused entity) drives the authored interaction on the partner body through Steam (protocol 74, bank 71 clips 369-373), both roles; generic reach on a Dunwall window handle verified, Emily visual accepted. Lever/crank/wheel variants classified by name but untested on real mechanisms; host-source clip ended early once. Audit 1229: real mechanisms, both roles - generic: client/host, lever: client/host, crank: client/host, wheel_valve: client/host, key_lock: client/host. Audit 1235 (#6, 1213k): hand reach toward the used entity origin, both roles on the vise; Corvo and Emily crank reaches visually accepted.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "use-interaction-v1214c/use-client-observed.mp4",
    "mission-mechanisms-v1227b/use-host-env_vise_01_1_3-observed.mp4",
    "use-reach-eotw-v1234/use-host-env_vise_01_1_3-observed.mp4",
    "use-reach-eotw-v1234b/use-client-env_vise_01_1_3-observed.mp4"
  ],
  "evidence": [
    "../movement-integration/native-combat-use-observations-v1211.json",
    "../movement-integration/use-interaction-audit-v1215.json",
    "../movement-integration/use-variants-audit-v1229.json",
    "../movement-integration/use-reach-audit-v1235.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json"
  ]
}
````

- [evidence: ../movement-integration/native-combat-use-observations-v1211.json](<../../../../work/movement-integration/native-combat-use-observations-v1211.json>)
- [evidence: ../movement-integration/use-interaction-audit-v1215.json](<../../../../work/movement-integration/use-interaction-audit-v1215.json>)
- [evidence: ../movement-integration/use-variants-audit-v1229.json](<../../../../work/movement-integration/use-variants-audit-v1229.json>)
- [evidence: ../movement-integration/use-reach-audit-v1235.json](<../../../../work/movement-integration/use-reach-audit-v1235.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [in_game_recordings: use-interaction-v1214c/use-client-observed.mp4](<../../../../work/natural-moveset/use-interaction-v1214c/use-client-observed.mp4>)
- [in_game_recordings: mission-mechanisms-v1227b/use-host-env_vise_01_1_3-observed.mp4](<../../../../work/natural-moveset/mission-mechanisms-v1227b/use-host-env_vise_01_1_3-observed.mp4>)
- [in_game_recordings: use-reach-eotw-v1234/use-host-env_vise_01_1_3-observed.mp4](<../../../../work/natural-moveset/use-reach-eotw-v1234/use-host-env_vise_01_1_3-observed.mp4>)
- [in_game_recordings: use-reach-eotw-v1234b/use-client-env_vise_01_1_3-observed.mp4](<../../../../work/natural-moveset/use-reach-eotw-v1234b/use-client-env_vise_01_1_3-observed.mp4>)
## pickup_loot_steal

Audit 1264 (1214j): a native pickup (LootObject47) presents the generic use reach toward the item in both roles (coin and grapes, product controller, 1261b). Body loot and pickpocketing not yet recorded. Visual sweep 1260: reject (corvo reject, emily reject). Both partners stay visible and stable, but neither makes any pickup or reach gesture (standing or crouched), so the loot action is not represented. 700/701 measures recurring55–65ms diagnostic batches, with over100ms spikes.702 staggers71 writers across frames (max4 and4ms soft budget), retaining core report cadence and all gameplay/250ms freshness guards.703Corvo/704Emily pass24 scoped runtime checks; baseline701 passes8. Both roles keep Steam ready and body weight>.999 during crouched loot. No status/slice reaches50ms in either new capture, versus52/51 source/observer stalls in baseline. Inventories/health14protected restored. Six game sheets inspected; rear views limit grip/contact judgment. No all-gap recovery or full movement claim. 785–789: exact native coin lifecycle and currency path traced in Ghidra. Ordinary Corvo/Emily pickup adds one coin, repeated input does not recredit. Guarded direct claim/reward/removal passes both roles with three native calls and duplicate no-op; checkpoint restores currency0, full quantities, health and coin.787 retirement-bit refusal and788 inventory-reader failure retained. No shared pickup relay yet; extra native entity event and reliable host ordering remain. 790/791: exact native coin fixture now includes pickedUp(true,false) entity/script event and variant cleanup, matching the decompiled native ordering before removal. Both roles pass currency0->1, five direct calls, duplicate no-op, outside-fixture refusal, and checkpoint/full inventory/health/coin restoration. Original788 feasibility and failed attempts are preserved. Native UI money notifications and coin removal recorded in both views. No shared Steam pickup channel yet. 790/791: exact native coin fixture now includes pickedUp(true,false) entity/script event and variant cleanup, matching the decompiled native ordering before removal. Both roles pass currency0->1, five direct calls, duplicate no-op, outside-fixture refusal, and checkpoint/full inventory/health/coin restoration. Original788 feasibility and failed attempts are preserved. Native UI money notifications recorded in both views; coin removal visible for Corvo, while Emily camera points away and removal is established by native reads. No shared Steam pickup channel yet. 794–798: first bounded native Steam coin channel passes ordinary Corvo and Emily pickups, simultaneous native awards, duplicate packets, pause/resume and coordinated fixed-SLOT82 reload. Both new coin bindings reject retired packets, then a fresh ordinary Emily pickup rewards each wallet once. Worker-thread source observation uses an immutable lease and bounded queue. All test inventories/currency/health restored; actual paired videos retained, including failed793 observer and797 fixture takes. Exact one authored coin only; movement and full shared-world work remain incomplete. 800–808: one bounded native currency catalog replaces the single-item binding. Both games match19 ordinary type1 objects across6 values. Native API batch alternates sources and verifies all19 awards/removals (258 each), individual receipts and duplicate no-ops. Ordinary same-coin simultaneous pickup gives1 each with0 extra recipient calls. Catalog shared checkpoint/rebind/stale packets and fresh Emily pickup pass. Full catalog/inventory/currency/health baseline restored; outside-fixture exercise makes0 calls. Failed ordinary pouch/cupboard approaches retained. Functional family coverage; movement and playable co-op remain incomplete. 810–818: shared pickup catalog expands by native family to24 objects (19 currency,4 ammunition,1 stored health potion). API source/Steam recipient batch gives258 money,6 bullets and1 potion each, with unchanged health; native box modifier2->3 is preserved. Partial ammo9->10 and full ammo/potion zero-grant recipients retire objects and acknowledge once; duplicate no-ops pass. Mixed-catalog ordinary coin shared-checkpoint/rebind/old-packet tests pass. Three ordinary stack approach/isolation failures are retained; ordinary ammo/potion remains unverified. Complete baseline restored. Movement and playable co-op remain incomplete. 820–826: native food family expands Dunwall catalog to51 objects (27 health food,19 currency,4 ammo,1 stored potion). Batch gives14 health to native cap70,258 money,6 bullets and1 potion each;24 full-health food no-ops retire correctly. Unequal host/client health, duplicate rejection and coordinated checkpoint renewal pass. Representative ordinary ammo, stored potion and food inputs now pass. Previous potion setup failures were test-fixture preconditions, not relay failures. Complete native baseline/protected files restored. Movement/playable co-op remain incomplete.

````json
{
  "id": "pickup_loot_steal",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_pickup_reach_presented_both_roles_product",
  "visual_validation": "accepted_functional_both_roles_generic_reach",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "pickup loot steal",
  "motions": [
    "empty_distraction_loot_ground_in",
    "empty_distraction_loot_ground_loop",
    "empty_distraction_loot_ground_out",
    "empty_distraction_loot_ground_out_fast"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1264 (1214j): a native pickup (LootObject47) presents the generic use reach toward the item in both roles (coin and grapes, product controller, 1261b). Body loot and pickpocketing not yet recorded. Visual sweep 1260: reject (corvo reject, emily reject). Both partners stay visible and stable, but neither makes any pickup or reach gesture (standing or crouched), so the loot action is not represented. 700/701 measures recurring55–65ms diagnostic batches, with over100ms spikes.702 staggers71 writers across frames (max4 and4ms soft budget), retaining core report cadence and all gameplay/250ms freshness guards.703Corvo/704Emily pass24 scoped runtime checks; baseline701 passes8. Both roles keep Steam ready and body weight>.999 during crouched loot. No status/slice reaches50ms in either new capture, versus52/51 source/observer stalls in baseline. Inventories/health14protected restored. Six game sheets inspected; rear views limit grip/contact judgment. No all-gap recovery or full movement claim. 785–789: exact native coin lifecycle and currency path traced in Ghidra. Ordinary Corvo/Emily pickup adds one coin, repeated input does not recredit. Guarded direct claim/reward/removal passes both roles with three native calls and duplicate no-op; checkpoint restores currency0, full quantities, health and coin.787 retirement-bit refusal and788 inventory-reader failure retained. No shared pickup relay yet; extra native entity event and reliable host ordering remain. 790/791: exact native coin fixture now includes pickedUp(true,false) entity/script event and variant cleanup, matching the decompiled native ordering before removal. Both roles pass currency0->1, five direct calls, duplicate no-op, outside-fixture refusal, and checkpoint/full inventory/health/coin restoration. Original788 feasibility and failed attempts are preserved. Native UI money notifications and coin removal recorded in both views. No shared Steam pickup channel yet. 790/791: exact native coin fixture now includes pickedUp(true,false) entity/script event and variant cleanup, matching the decompiled native ordering before removal. Both roles pass currency0->1, five direct calls, duplicate no-op, outside-fixture refusal, and checkpoint/full inventory/health/coin restoration. Original788 feasibility and failed attempts are preserved. Native UI money notifications recorded in both views; coin removal visible for Corvo, while Emily camera points away and removal is established by native reads. No shared Steam pickup channel yet. 794–798: first bounded native Steam coin channel passes ordinary Corvo and Emily pickups, simultaneous native awards, duplicate packets, pause/resume and coordinated fixed-SLOT82 reload. Both new coin bindings reject retired packets, then a fresh ordinary Emily pickup rewards each wallet once. Worker-thread source observation uses an immutable lease and bounded queue. All test inventories/currency/health restored; actual paired videos retained, including failed793 observer and797 fixture takes. Exact one authored coin only; movement and full shared-world work remain incomplete. 800–808: one bounded native currency catalog replaces the single-item binding. Both games match19 ordinary type1 objects across6 values. Native API batch alternates sources and verifies all19 awards/removals (258 each), individual receipts and duplicate no-ops. Ordinary same-coin simultaneous pickup gives1 each with0 extra recipient calls. Catalog shared checkpoint/rebind/stale packets and fresh Emily pickup pass. Full catalog/inventory/currency/health baseline restored; outside-fixture exercise makes0 calls. Failed ordinary pouch/cupboard approaches retained. Functional family coverage; movement and playable co-op remain incomplete. 810–818: shared pickup catalog expands by native family to24 objects (19 currency,4 ammunition,1 stored health potion). API source/Steam recipient batch gives258 money,6 bullets and1 potion each, with unchanged health; native box modifier2->3 is preserved. Partial ammo9->10 and full ammo/potion zero-grant recipients retire objects and acknowledge once; duplicate no-ops pass. Mixed-catalog ordinary coin shared-checkpoint/rebind/old-packet tests pass. Three ordinary stack approach/isolation failures are retained; ordinary ammo/potion remains unverified. Complete baseline restored. Movement and playable co-op remain incomplete. 820–826: native food family expands Dunwall catalog to51 objects (27 health food,19 currency,4 ammo,1 stored potion). Batch gives14 health to native cap70,258 money,6 bullets and1 potion each;24 full-health food no-ops retire correctly. Unequal host/client health, duplicate rejection and coordinated checkpoint renewal pass. Representative ordinary ammo, stored potion and food inputs now pass. Previous potion setup failures were test-fixture preconditions, not relay failures. Complete native baseline/protected files restored. Movement/playable co-op remain incomplete.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-interactions-corvo-coin-v667/coin-source.mp4",
    "native-interactions-corvo-coin-v667/coin-observer.mp4",
    "native-interactions-emily-coin-v668/coin-source.mp4",
    "native-interactions-emily-coin-v668/coin-observer.mp4",
    "native-interactions-emily-crouch-coin-v695/coin-observer.mp4",
    "native-interactions-emily-crouch-coin-v695/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v699/coin-observer.mp4",
    "native-interactions-corvo-crouch-coin-v699/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v701/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v701/coin-observer.mp4",
    "native-interactions-corvo-crouch-coin-v703/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v703/coin-observer.mp4",
    "native-interactions-emily-crouch-coin-v704/coin-source.mp4",
    "native-interactions-emily-crouch-coin-v704/coin-observer.mp4",
    "native-coin-reward-v786/host.mp4",
    "native-coin-reward-v786/client.mp4",
    "native-coin-reward-v786c/host.mp4",
    "native-coin-reward-v786c/client.mp4",
    "native-coin-api-v787/host.mp4",
    "native-coin-api-v788/host.mp4",
    "native-coin-api-v788/client.mp4",
    "native-coin-api-v788b/host.mp4",
    "native-coin-api-v788b/client.mp4",
    "native-coin-api-v790b/host.mp4",
    "native-coin-api-v790b/client.mp4",
    "shared-coin-host-v793/client.mp4",
    "shared-coin-host-v793/host.mp4",
    "shared-coin-host-v795/client.mp4",
    "shared-coin-host-v795/host.mp4",
    "shared-coin-client-v795/client.mp4",
    "shared-coin-client-v795/host.mp4",
    "shared-coin-simultaneous-v796/client.mp4",
    "shared-coin-simultaneous-v796/host.mp4",
    "shared-coin-menu-v796/client.mp4",
    "shared-coin-menu-v796/host.mp4",
    "shared-coin-recovery-v797/client.mp4",
    "shared-coin-recovery-v797/host.mp4",
    "shared-coin-recovery-v797b/client.mp4",
    "shared-coin-recovery-v797b/fresh-client.mp4",
    "shared-coin-recovery-v797b/fresh-host.mp4",
    "shared-coin-recovery-v797b/host.mp4",
    "shared-coin-recovery-v797b/reload-client.mp4",
    "shared-coin-recovery-v797b/reload-host.mp4",
    "shared-coin-catalog-v801b/client.mp4",
    "shared-coin-catalog-v801b/host.mp4",
    "shared-currency-catalog-v801c/client.mp4",
    "shared-currency-catalog-v801c/host.mp4",
    "shared-currency-catalog-v801d/client.mp4",
    "shared-currency-catalog-v801d/host.mp4",
    "shared-currency-recovery-v802/client.mp4",
    "shared-currency-recovery-v802/fresh-client.mp4",
    "shared-currency-recovery-v802/fresh-host.mp4",
    "shared-currency-recovery-v802/host.mp4",
    "shared-currency-recovery-v802/reload-client.mp4",
    "shared-currency-recovery-v802/reload-host.mp4",
    "shared-currency-batch-v805/client.mp4",
    "shared-currency-batch-v805/host.mp4",
    "shared-currency-simultaneous-v806/client.mp4",
    "shared-currency-simultaneous-v806/host.mp4",
    "shared-pickup-stack-batch-v812/client.mp4",
    "shared-pickup-stack-batch-v812/host.mp4",
    "shared-pickup-recovery-v813/client.mp4",
    "shared-pickup-recovery-v813/fresh-client.mp4",
    "shared-pickup-recovery-v813/fresh-host.mp4",
    "shared-pickup-recovery-v813/host.mp4",
    "shared-pickup-recovery-v813/reload-client.mp4",
    "shared-pickup-recovery-v813/reload-host.mp4",
    "shared-pickup-capacity-v815/client.mp4",
    "shared-pickup-capacity-v815/host.mp4",
    "shared-food-catalog-batch-v822/client.mp4",
    "shared-food-catalog-batch-v822/host.mp4",
    "shared-food-recovery-v823/client.mp4",
    "shared-food-recovery-v823/fresh-client.mp4",
    "shared-food-recovery-v823/fresh-host.mp4",
    "shared-food-recovery-v823/host.mp4",
    "shared-food-recovery-v823/reload-client.mp4",
    "shared-food-recovery-v823/reload-host.mp4",
    "shared-pickup-potion-input-v824/client.mp4",
    "shared-pickup-potion-input-v824/host.mp4",
    "shared-pickup-food-input-v824/client.mp4",
    "shared-pickup-food-input-v824/host.mp4",
    "shared-pickup-ammo-input-v824/client.mp4",
    "shared-pickup-ammo-input-v824/host.mp4",
    "native-interactions-corvo-v662/coin-observer.mp4",
    "native-interactions-corvo-v662/coin-source.mp4",
    "native-interactions-corvo-coin-v664/coin-observer.mp4",
    "native-interactions-corvo-coin-v664/coin-source.mp4",
    "native-interactions-emily-coin-v665/coin-observer.mp4",
    "native-interactions-emily-coin-v665/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v686/coin-observer.mp4",
    "native-interactions-corvo-crouch-coin-v686/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v688/coin-observer.mp4",
    "native-interactions-corvo-crouch-coin-v688/coin-source.mp4",
    "native-interactions-corvo-crouch-coin-v692/coin-observer.mp4",
    "native-interactions-corvo-crouch-coin-v692/coin-source.mp4",
    "gestures-v1261b/host-coin-observer.mp4",
    "gestures-v1261b/client-coin-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-interactions-corvo-coin-v667/result.json",
    "native-interactions-emily-coin-v668/result.json",
    "native-interactions-emily-crouch-coin-v695/result.json",
    "native-interactions-corvo-crouch-coin-v699/result.json",
    "native-interactions-corvo-crouch-coin-v701/result.json",
    "native-interactions-corvo-crouch-coin-v703/result.json",
    "native-interactions-emily-crouch-coin-v704/result.json",
    "native-coin-reward-v786c/result.json",
    "native-coin-api-v788b/result.json",
    "native-coin-api-v790b/result.json",
    "shared-coin-host-v795/result.json",
    "shared-coin-client-v795/result.json",
    "shared-coin-simultaneous-v796/result.json",
    "shared-coin-menu-v796/result.json",
    "shared-coin-recovery-v797b/result.json",
    "shared-currency-recovery-v802/result.json",
    "shared-currency-batch-v805/result.json",
    "shared-currency-simultaneous-v806/result.json",
    "currency-catalog-restoration-v807/result.json",
    "shared-pickup-stack-batch-v812/result.json",
    "shared-pickup-recovery-v813/result.json",
    "shared-pickup-capacity-v815/result.json",
    "shared-pickup-ammo-input-v816/result.json",
    "shared-pickup-potion-input-v816b/result.json",
    "shared-pickup-potion-input-v816c/result.json",
    "pickup-catalog-restoration-v817/result.json",
    "shared-food-catalog-batch-v822/result.json",
    "shared-food-recovery-v823/result.json",
    "shared-pickup-potion-input-v824/result.json",
    "shared-pickup-food-input-v824/result.json",
    "shared-pickup-ammo-input-v824/result.json",
    "pickup-catalog-restoration-v825/result.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "../movement-integration/gestures-counter-audit-v1264.json"
  ]
}
````

- [evidence: native-interactions-corvo-coin-v667/result.json](<../../../../work/natural-moveset/native-interactions-corvo-coin-v667/result.json>)
- [evidence: native-interactions-emily-coin-v668/result.json](<../../../../work/natural-moveset/native-interactions-emily-coin-v668/result.json>)
- [evidence: native-interactions-emily-crouch-coin-v695/result.json](<../../../../work/natural-moveset/native-interactions-emily-crouch-coin-v695/result.json>)
- [evidence: native-interactions-corvo-crouch-coin-v699/result.json](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v699/result.json>)
- [evidence: native-interactions-corvo-crouch-coin-v701/result.json](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v701/result.json>)
- [evidence: native-interactions-corvo-crouch-coin-v703/result.json](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v703/result.json>)
- [evidence: native-interactions-emily-crouch-coin-v704/result.json](<../../../../work/natural-moveset/native-interactions-emily-crouch-coin-v704/result.json>)
- [evidence: native-coin-reward-v786c/result.json](<../../../../work/natural-moveset/native-coin-reward-v786c/result.json>)
- [evidence: native-coin-api-v788b/result.json](<../../../../work/natural-moveset/native-coin-api-v788b/result.json>)
- [evidence: native-coin-api-v790b/result.json](<../../../../work/natural-moveset/native-coin-api-v790b/result.json>)
- [evidence: shared-coin-host-v795/result.json](<../../../../work/natural-moveset/shared-coin-host-v795/result.json>)
- [evidence: shared-coin-client-v795/result.json](<../../../../work/natural-moveset/shared-coin-client-v795/result.json>)
- [evidence: shared-coin-simultaneous-v796/result.json](<../../../../work/natural-moveset/shared-coin-simultaneous-v796/result.json>)
- [evidence: shared-coin-menu-v796/result.json](<../../../../work/natural-moveset/shared-coin-menu-v796/result.json>)
- [evidence: shared-coin-recovery-v797b/result.json](<../../../../work/natural-moveset/shared-coin-recovery-v797b/result.json>)
- [evidence: shared-currency-recovery-v802/result.json](<../../../../work/natural-moveset/shared-currency-recovery-v802/result.json>)
- [evidence: shared-currency-batch-v805/result.json](<../../../../work/natural-moveset/shared-currency-batch-v805/result.json>)
- [evidence: shared-currency-simultaneous-v806/result.json](<../../../../work/natural-moveset/shared-currency-simultaneous-v806/result.json>)
- [evidence: currency-catalog-restoration-v807/result.json](<../../../../work/natural-moveset/currency-catalog-restoration-v807/result.json>)
- [evidence: shared-pickup-stack-batch-v812/result.json](<../../../../work/natural-moveset/shared-pickup-stack-batch-v812/result.json>)
- [evidence: shared-pickup-recovery-v813/result.json](<../../../../work/natural-moveset/shared-pickup-recovery-v813/result.json>)
- [evidence: shared-pickup-capacity-v815/result.json](<../../../../work/natural-moveset/shared-pickup-capacity-v815/result.json>)
- [evidence: shared-pickup-ammo-input-v816/result.json](<../../../../work/natural-moveset/shared-pickup-ammo-input-v816/result.json>)
- [evidence: shared-pickup-potion-input-v816b/result.json](<../../../../work/natural-moveset/shared-pickup-potion-input-v816b/result.json>)
- [evidence: shared-pickup-potion-input-v816c/result.json](<../../../../work/natural-moveset/shared-pickup-potion-input-v816c/result.json>)
- [evidence: pickup-catalog-restoration-v817/result.json](<../../../../work/natural-moveset/pickup-catalog-restoration-v817/result.json>)
- [evidence: shared-food-catalog-batch-v822/result.json](<../../../../work/natural-moveset/shared-food-catalog-batch-v822/result.json>)
- [evidence: shared-food-recovery-v823/result.json](<../../../../work/natural-moveset/shared-food-recovery-v823/result.json>)
- [evidence: shared-pickup-potion-input-v824/result.json](<../../../../work/natural-moveset/shared-pickup-potion-input-v824/result.json>)
- [evidence: shared-pickup-food-input-v824/result.json](<../../../../work/natural-moveset/shared-pickup-food-input-v824/result.json>)
- [evidence: shared-pickup-ammo-input-v824/result.json](<../../../../work/natural-moveset/shared-pickup-ammo-input-v824/result.json>)
- [evidence: pickup-catalog-restoration-v825/result.json](<../../../../work/natural-moveset/pickup-catalog-restoration-v825/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: ../movement-integration/gestures-counter-audit-v1264.json](<../../../../work/movement-integration/gestures-counter-audit-v1264.json>)
- [in_game_recordings: native-interactions-corvo-coin-v667/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-coin-v667/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-coin-v667/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-coin-v667/coin-observer.mp4>)
- [in_game_recordings: native-interactions-emily-coin-v668/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-coin-v668/coin-source.mp4>)
- [in_game_recordings: native-interactions-emily-coin-v668/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-coin-v668/coin-observer.mp4>)
- [in_game_recordings: native-interactions-emily-crouch-coin-v695/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-crouch-coin-v695/coin-observer.mp4>)
- [in_game_recordings: native-interactions-emily-crouch-coin-v695/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-crouch-coin-v695/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v699/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v699/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v699/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v699/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v701/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v701/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v701/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v701/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v703/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v703/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v703/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v703/coin-observer.mp4>)
- [in_game_recordings: native-interactions-emily-crouch-coin-v704/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-crouch-coin-v704/coin-source.mp4>)
- [in_game_recordings: native-interactions-emily-crouch-coin-v704/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-crouch-coin-v704/coin-observer.mp4>)
- [in_game_recordings: native-coin-reward-v786/host.mp4](<../../../../work/natural-moveset/native-coin-reward-v786/host.mp4>)
- [in_game_recordings: native-coin-reward-v786/client.mp4](<../../../../work/natural-moveset/native-coin-reward-v786/client.mp4>)
- [in_game_recordings: native-coin-reward-v786c/host.mp4](<../../../../work/natural-moveset/native-coin-reward-v786c/host.mp4>)
- [in_game_recordings: native-coin-reward-v786c/client.mp4](<../../../../work/natural-moveset/native-coin-reward-v786c/client.mp4>)
- [in_game_recordings: native-coin-api-v787/host.mp4](<../../../../work/natural-moveset/native-coin-api-v787/host.mp4>)
- [in_game_recordings: native-coin-api-v788/host.mp4](<../../../../work/natural-moveset/native-coin-api-v788/host.mp4>)
- [in_game_recordings: native-coin-api-v788/client.mp4](<../../../../work/natural-moveset/native-coin-api-v788/client.mp4>)
- [in_game_recordings: native-coin-api-v788b/host.mp4](<../../../../work/natural-moveset/native-coin-api-v788b/host.mp4>)
- [in_game_recordings: native-coin-api-v788b/client.mp4](<../../../../work/natural-moveset/native-coin-api-v788b/client.mp4>)
- [in_game_recordings: native-coin-api-v790b/host.mp4](<../../../../work/natural-moveset/native-coin-api-v790b/host.mp4>)
- [in_game_recordings: native-coin-api-v790b/client.mp4](<../../../../work/natural-moveset/native-coin-api-v790b/client.mp4>)
- [in_game_recordings: shared-coin-host-v793/client.mp4](<../../../../work/natural-moveset/shared-coin-host-v793/client.mp4>)
- [in_game_recordings: shared-coin-host-v793/host.mp4](<../../../../work/natural-moveset/shared-coin-host-v793/host.mp4>)
- [in_game_recordings: shared-coin-host-v795/client.mp4](<../../../../work/natural-moveset/shared-coin-host-v795/client.mp4>)
- [in_game_recordings: shared-coin-host-v795/host.mp4](<../../../../work/natural-moveset/shared-coin-host-v795/host.mp4>)
- [in_game_recordings: shared-coin-client-v795/client.mp4](<../../../../work/natural-moveset/shared-coin-client-v795/client.mp4>)
- [in_game_recordings: shared-coin-client-v795/host.mp4](<../../../../work/natural-moveset/shared-coin-client-v795/host.mp4>)
- [in_game_recordings: shared-coin-simultaneous-v796/client.mp4](<../../../../work/natural-moveset/shared-coin-simultaneous-v796/client.mp4>)
- [in_game_recordings: shared-coin-simultaneous-v796/host.mp4](<../../../../work/natural-moveset/shared-coin-simultaneous-v796/host.mp4>)
- [in_game_recordings: shared-coin-menu-v796/client.mp4](<../../../../work/natural-moveset/shared-coin-menu-v796/client.mp4>)
- [in_game_recordings: shared-coin-menu-v796/host.mp4](<../../../../work/natural-moveset/shared-coin-menu-v796/host.mp4>)
- [in_game_recordings: shared-coin-recovery-v797/client.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797/client.mp4>)
- [in_game_recordings: shared-coin-recovery-v797/host.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797/host.mp4>)
- [in_game_recordings: shared-coin-recovery-v797b/client.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797b/client.mp4>)
- [in_game_recordings: shared-coin-recovery-v797b/fresh-client.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797b/fresh-client.mp4>)
- [in_game_recordings: shared-coin-recovery-v797b/fresh-host.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797b/fresh-host.mp4>)
- [in_game_recordings: shared-coin-recovery-v797b/host.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797b/host.mp4>)
- [in_game_recordings: shared-coin-recovery-v797b/reload-client.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797b/reload-client.mp4>)
- [in_game_recordings: shared-coin-recovery-v797b/reload-host.mp4](<../../../../work/natural-moveset/shared-coin-recovery-v797b/reload-host.mp4>)
- [in_game_recordings: shared-coin-catalog-v801b/client.mp4](<../../../../work/natural-moveset/shared-coin-catalog-v801b/client.mp4>)
- [in_game_recordings: shared-coin-catalog-v801b/host.mp4](<../../../../work/natural-moveset/shared-coin-catalog-v801b/host.mp4>)
- [in_game_recordings: shared-currency-catalog-v801c/client.mp4](<../../../../work/natural-moveset/shared-currency-catalog-v801c/client.mp4>)
- [in_game_recordings: shared-currency-catalog-v801c/host.mp4](<../../../../work/natural-moveset/shared-currency-catalog-v801c/host.mp4>)
- [in_game_recordings: shared-currency-catalog-v801d/client.mp4](<../../../../work/natural-moveset/shared-currency-catalog-v801d/client.mp4>)
- [in_game_recordings: shared-currency-catalog-v801d/host.mp4](<../../../../work/natural-moveset/shared-currency-catalog-v801d/host.mp4>)
- [in_game_recordings: shared-currency-recovery-v802/client.mp4](<../../../../work/natural-moveset/shared-currency-recovery-v802/client.mp4>)
- [in_game_recordings: shared-currency-recovery-v802/fresh-client.mp4](<../../../../work/natural-moveset/shared-currency-recovery-v802/fresh-client.mp4>)
- [in_game_recordings: shared-currency-recovery-v802/fresh-host.mp4](<../../../../work/natural-moveset/shared-currency-recovery-v802/fresh-host.mp4>)
- [in_game_recordings: shared-currency-recovery-v802/host.mp4](<../../../../work/natural-moveset/shared-currency-recovery-v802/host.mp4>)
- [in_game_recordings: shared-currency-recovery-v802/reload-client.mp4](<../../../../work/natural-moveset/shared-currency-recovery-v802/reload-client.mp4>)
- [in_game_recordings: shared-currency-recovery-v802/reload-host.mp4](<../../../../work/natural-moveset/shared-currency-recovery-v802/reload-host.mp4>)
- [in_game_recordings: shared-currency-batch-v805/client.mp4](<../../../../work/natural-moveset/shared-currency-batch-v805/client.mp4>)
- [in_game_recordings: shared-currency-batch-v805/host.mp4](<../../../../work/natural-moveset/shared-currency-batch-v805/host.mp4>)
- [in_game_recordings: shared-currency-simultaneous-v806/client.mp4](<../../../../work/natural-moveset/shared-currency-simultaneous-v806/client.mp4>)
- [in_game_recordings: shared-currency-simultaneous-v806/host.mp4](<../../../../work/natural-moveset/shared-currency-simultaneous-v806/host.mp4>)
- [in_game_recordings: shared-pickup-stack-batch-v812/client.mp4](<../../../../work/natural-moveset/shared-pickup-stack-batch-v812/client.mp4>)
- [in_game_recordings: shared-pickup-stack-batch-v812/host.mp4](<../../../../work/natural-moveset/shared-pickup-stack-batch-v812/host.mp4>)
- [in_game_recordings: shared-pickup-recovery-v813/client.mp4](<../../../../work/natural-moveset/shared-pickup-recovery-v813/client.mp4>)
- [in_game_recordings: shared-pickup-recovery-v813/fresh-client.mp4](<../../../../work/natural-moveset/shared-pickup-recovery-v813/fresh-client.mp4>)
- [in_game_recordings: shared-pickup-recovery-v813/fresh-host.mp4](<../../../../work/natural-moveset/shared-pickup-recovery-v813/fresh-host.mp4>)
- [in_game_recordings: shared-pickup-recovery-v813/host.mp4](<../../../../work/natural-moveset/shared-pickup-recovery-v813/host.mp4>)
- [in_game_recordings: shared-pickup-recovery-v813/reload-client.mp4](<../../../../work/natural-moveset/shared-pickup-recovery-v813/reload-client.mp4>)
- [in_game_recordings: shared-pickup-recovery-v813/reload-host.mp4](<../../../../work/natural-moveset/shared-pickup-recovery-v813/reload-host.mp4>)
- [in_game_recordings: shared-pickup-capacity-v815/client.mp4](<../../../../work/natural-moveset/shared-pickup-capacity-v815/client.mp4>)
- [in_game_recordings: shared-pickup-capacity-v815/host.mp4](<../../../../work/natural-moveset/shared-pickup-capacity-v815/host.mp4>)
- [in_game_recordings: shared-food-catalog-batch-v822/client.mp4](<../../../../work/natural-moveset/shared-food-catalog-batch-v822/client.mp4>)
- [in_game_recordings: shared-food-catalog-batch-v822/host.mp4](<../../../../work/natural-moveset/shared-food-catalog-batch-v822/host.mp4>)
- [in_game_recordings: shared-food-recovery-v823/client.mp4](<../../../../work/natural-moveset/shared-food-recovery-v823/client.mp4>)
- [in_game_recordings: shared-food-recovery-v823/fresh-client.mp4](<../../../../work/natural-moveset/shared-food-recovery-v823/fresh-client.mp4>)
- [in_game_recordings: shared-food-recovery-v823/fresh-host.mp4](<../../../../work/natural-moveset/shared-food-recovery-v823/fresh-host.mp4>)
- [in_game_recordings: shared-food-recovery-v823/host.mp4](<../../../../work/natural-moveset/shared-food-recovery-v823/host.mp4>)
- [in_game_recordings: shared-food-recovery-v823/reload-client.mp4](<../../../../work/natural-moveset/shared-food-recovery-v823/reload-client.mp4>)
- [in_game_recordings: shared-food-recovery-v823/reload-host.mp4](<../../../../work/natural-moveset/shared-food-recovery-v823/reload-host.mp4>)
- [in_game_recordings: shared-pickup-potion-input-v824/client.mp4](<../../../../work/natural-moveset/shared-pickup-potion-input-v824/client.mp4>)
- [in_game_recordings: shared-pickup-potion-input-v824/host.mp4](<../../../../work/natural-moveset/shared-pickup-potion-input-v824/host.mp4>)
- [in_game_recordings: shared-pickup-food-input-v824/client.mp4](<../../../../work/natural-moveset/shared-pickup-food-input-v824/client.mp4>)
- [in_game_recordings: shared-pickup-food-input-v824/host.mp4](<../../../../work/natural-moveset/shared-pickup-food-input-v824/host.mp4>)
- [in_game_recordings: shared-pickup-ammo-input-v824/client.mp4](<../../../../work/natural-moveset/shared-pickup-ammo-input-v824/client.mp4>)
- [in_game_recordings: shared-pickup-ammo-input-v824/host.mp4](<../../../../work/natural-moveset/shared-pickup-ammo-input-v824/host.mp4>)
- [in_game_recordings: native-interactions-corvo-v662/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-v662/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-v662/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-v662/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-coin-v664/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-coin-v664/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-coin-v664/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-coin-v664/coin-source.mp4>)
- [in_game_recordings: native-interactions-emily-coin-v665/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-coin-v665/coin-observer.mp4>)
- [in_game_recordings: native-interactions-emily-coin-v665/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-coin-v665/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v686/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v686/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v686/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v686/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v688/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v688/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v688/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v688/coin-source.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v692/coin-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v692/coin-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-crouch-coin-v692/coin-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-crouch-coin-v692/coin-source.mp4>)
- [in_game_recordings: gestures-v1261b/host-coin-observer.mp4](<../../../../work/natural-moveset/gestures-v1261b/host-coin-observer.mp4>)
- [in_game_recordings: gestures-v1261b/client-coin-observer.mp4](<../../../../work/natural-moveset/gestures-v1261b/client-coin-observer.mp4>)
## key_use

Visual sweep 1260: insufficient (corvo insufficient, emily accept). Emily's lock reach reads acceptably; there is no usable recording of the Corvo partner using a lock, so the item cannot be fully accepted. Reach, insert/turn and recovery study using a neutral key fixture. Exact lock geometry, key inventory/visibility and gameplay timing remain pending. 1210c (audit 1211): native use enters player state 75 briefly (same source as levers); no native key animation; binding pending. 1213b (audit 1215): key/lock uses map to clip 373 by focused entity name through the same native use source; not yet runtime-tested on a real lock. Audit 1229: real mechanisms, both roles - generic: client/host, lever: client/host, crank: client/host, wheel_valve: client/host, key_lock: client/host.

````json
{
  "id": "key_use",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_use_state75_real_lock_both_roles",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "key use",
  "motions": [
    "interact_key_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: insufficient (corvo insufficient, emily accept). Emily's lock reach reads acceptably; there is no usable recording of the Corvo partner using a lock, so the item cannot be fully accepted. Reach, insert/turn and recovery study using a neutral key fixture. Exact lock geometry, key inventory/visibility and gameplay timing remain pending. 1210c (audit 1211): native use enters player state 75 briefly (same source as levers); no native key animation; binding pending. 1213b (audit 1215): key/lock uses map to clip 373 by focused entity name through the same native use source; not yet runtime-tested on a real lock. Audit 1229: real mechanisms, both roles - generic: client/host, lever: client/host, crank: client/host, wheel_valve: client/host, key_lock: client/host.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "use-variants-dunwall-v1228b/use-client-ld_ring_locker_01_1_lock-observed.mp4"
  ],
  "evidence": [
    "../movement-integration/native-combat-use-observations-v1211.json",
    "../movement-integration/use-interaction-audit-v1215.json",
    "../movement-integration/use-variants-audit-v1229.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json"
  ]
}
````

- [evidence: ../movement-integration/native-combat-use-observations-v1211.json](<../../../../work/movement-integration/native-combat-use-observations-v1211.json>)
- [evidence: ../movement-integration/use-interaction-audit-v1215.json](<../../../../work/movement-integration/use-interaction-audit-v1215.json>)
- [evidence: ../movement-integration/use-variants-audit-v1229.json](<../../../../work/movement-integration/use-variants-audit-v1229.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [in_game_recordings: use-variants-dunwall-v1228b/use-client-ld_ring_locker_01_1_lock-observed.mp4](<../../../../work/natural-moveset/use-variants-dunwall-v1228b/use-client-ld_ring_locker_01_1_lock-observed.mp4>)
## read_object_or_note

Visual sweep 1260: reject (corvo reject, emily reject). In the latest takes the partner stays visible and recovers to walking, but makes no reading gesture (the older takes vanish during the note), so the read action itself is not represented. 666/668 coin continuity passes20 checks.681/683/685 ReadNote4 and ordinary post-close walking pass28 checks: both bodies remain visible, reading root error/drift0, fresh Steam tokens and pinned357-clip bank recover, inventories/health14protected restored. Both source/observer sheets inspected. Failed682 retained: stream retired after root age250ms despite earlier recovery. No authored reading gesture/shared item or document outcome; fine acting deferred. Routine Mayhew reset now uses verified native skip680 (scene4.619s, full reset20.857s).

````json
{
  "id": "read_object_or_note",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_note_visible_root_hold_and_resume_both_roles",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "read object or note",
  "motions": [
    "activity_counter_read_book_loop",
    "activity_sitting_table_read_out",
    "activity_sitting_table_read_loop",
    "activity_sitting_table_read_in",
    "activity_counter_read_book_in_walkright",
    "activity_counter_read_book_out_walk"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: reject (corvo reject, emily reject). In the latest takes the partner stays visible and recovers to walking, but makes no reading gesture (the older takes vanish during the note), so the read action itself is not represented. 666/668 coin continuity passes20 checks.681/683/685 ReadNote4 and ordinary post-close walking pass28 checks: both bodies remain visible, reading root error/drift0, fresh Steam tokens and pinned357-clip bank recover, inventories/health14protected restored. Both source/observer sheets inspected. Failed682 retained: stream retired after root age250ms despite earlier recovery. No authored reading gesture/shared item or document outcome; fine acting deferred. Routine Mayhew reset now uses verified native skip680 (scene4.619s, full reset20.857s).",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-interactions-corvo-note-v685/note-source.mp4",
    "native-interactions-corvo-note-v685/note-observer.mp4",
    "native-interactions-emily-note-v683/note-source.mp4",
    "native-interactions-emily-note-v683/note-observer.mp4",
    "native-interactions-corvo-note-v669/note-observer.mp4",
    "native-interactions-corvo-note-v669/note-source.mp4",
    "native-interactions-corvo-note-v671/note-observer.mp4",
    "native-interactions-corvo-note-v671/note-source.mp4",
    "native-interactions-emily-note-v672/note-observer.mp4",
    "native-interactions-emily-note-v672/note-source.mp4",
    "native-interactions-corvo-note-v674/note-observer.mp4",
    "native-interactions-corvo-note-v674/note-source.mp4",
    "native-interactions-corvo-note-v677/note-observer.mp4",
    "native-interactions-corvo-note-v677/note-source.mp4",
    "native-interactions-corvo-note-v682/note-observer.mp4",
    "native-interactions-corvo-note-v682/note-source.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-interactions-corvo-note-v685/result.json",
    "../../work/movement-integration/readable-resume-audit-v684.json",
    "native-interactions-emily-note-v683/result.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json"
  ]
}
````

- [evidence: native-interactions-corvo-note-v685/result.json](<../../../../work/natural-moveset/native-interactions-corvo-note-v685/result.json>)
- [evidence: ../../work/movement-integration/readable-resume-audit-v684.json](<../../../../work/movement-integration/readable-resume-audit-v684.json>)
- [evidence: native-interactions-emily-note-v683/result.json](<../../../../work/natural-moveset/native-interactions-emily-note-v683/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [in_game_recordings: native-interactions-corvo-note-v685/note-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v685/note-source.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v685/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v685/note-observer.mp4>)
- [in_game_recordings: native-interactions-emily-note-v683/note-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-note-v683/note-source.mp4>)
- [in_game_recordings: native-interactions-emily-note-v683/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-note-v683/note-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v669/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v669/note-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v669/note-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v669/note-source.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v671/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v671/note-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v671/note-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v671/note-source.mp4>)
- [in_game_recordings: native-interactions-emily-note-v672/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-note-v672/note-observer.mp4>)
- [in_game_recordings: native-interactions-emily-note-v672/note-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-note-v672/note-source.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v674/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v674/note-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v674/note-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v674/note-source.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v677/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v677/note-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v677/note-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v677/note-source.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v682/note-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v682/note-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-note-v682/note-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-note-v682/note-source.mp4>)
## eat_drink

Audit 1264 (1214j): eating native food (grapes, LootObject47) presents the reach to the plate in both roles while the item is taken in both worlds (1261b). Visual sweep 1260: reject (corvo reject, emily reject). Both partners stay continuous but show no eating gesture, so the eat action is not represented. 693 exact crouched LootObject47 now preserves Sneaking6/raw1. Both-role native classification verified;695Emily passes11 checks,699Corvo passes10/11 with body-weight dips retained as failure.696/697 native grapes consumption passes22: source health56->61, peer56 unchanged, source grapes removed, full body/Steam continuity. All source/observer sheets inspected; inventories/health14protected restored. No new reach/eating gesture or shared item/healing outcome. Protocol69 bank67/357 unchanged.

````json
{
  "id": "eat_drink",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_food_reach_presented_both_roles_product",
  "visual_validation": "accepted_functional_both_roles_generic_reach",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "eat drink",
  "motions": [
    "empty_idlebreak_drink_flask",
    "activity_drink_standing_out",
    "activity_drink_standing_loop_a",
    "activity_drink_standing_in",
    "empty_distraction_eat_apple_out",
    "empty_distraction_eat_apple_loop",
    "empty_distraction_eat_apple_in"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1264 (1214j): eating native food (grapes, LootObject47) presents the reach to the plate in both roles while the item is taken in both worlds (1261b). Visual sweep 1260: reject (corvo reject, emily reject). Both partners stay continuous but show no eating gesture, so the eat action is not represented. 693 exact crouched LootObject47 now preserves Sneaking6/raw1. Both-role native classification verified;695Emily passes11 checks,699Corvo passes10/11 with body-weight dips retained as failure.696/697 native grapes consumption passes22: source health56->61, peer56 unchanged, source grapes removed, full body/Steam continuity. All source/observer sheets inspected; inventories/health14protected restored. No new reach/eating gesture or shared item/healing outcome. Protocol69 bank67/357 unchanged.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-interactions-corvo-v663/food-source.mp4",
    "native-interactions-corvo-v663/food-observer.mp4",
    "native-interactions-corvo-food-v696/food-observer.mp4",
    "native-interactions-corvo-food-v696/food-source.mp4",
    "native-interactions-emily-food-v697/food-observer.mp4",
    "native-interactions-emily-food-v697/food-source.mp4",
    "native-interactions-corvo-v662/food-observer.mp4",
    "native-interactions-corvo-v662/food-source.mp4",
    "gestures-v1261b/host-food-observer.mp4",
    "gestures-v1261b/client-food-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-interactions-corvo-v663/result.json",
    "native-interactions-corvo-food-v696/result.json",
    "native-interactions-emily-food-v697/result.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "../movement-integration/gestures-counter-audit-v1264.json"
  ]
}
````

- [evidence: native-interactions-corvo-v663/result.json](<../../../../work/natural-moveset/native-interactions-corvo-v663/result.json>)
- [evidence: native-interactions-corvo-food-v696/result.json](<../../../../work/natural-moveset/native-interactions-corvo-food-v696/result.json>)
- [evidence: native-interactions-emily-food-v697/result.json](<../../../../work/natural-moveset/native-interactions-emily-food-v697/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: ../movement-integration/gestures-counter-audit-v1264.json](<../../../../work/movement-integration/gestures-counter-audit-v1264.json>)
- [in_game_recordings: native-interactions-corvo-v663/food-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-v663/food-source.mp4>)
- [in_game_recordings: native-interactions-corvo-v663/food-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-v663/food-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-food-v696/food-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-food-v696/food-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-food-v696/food-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-food-v696/food-source.mp4>)
- [in_game_recordings: native-interactions-emily-food-v697/food-observer.mp4](<../../../../work/natural-moveset/native-interactions-emily-food-v697/food-observer.mp4>)
- [in_game_recordings: native-interactions-emily-food-v697/food-source.mp4](<../../../../work/natural-moveset/native-interactions-emily-food-v697/food-source.mp4>)
- [in_game_recordings: native-interactions-corvo-v662/food-observer.mp4](<../../../../work/natural-moveset/native-interactions-corvo-v662/food-observer.mp4>)
- [in_game_recordings: native-interactions-corvo-v662/food-source.mp4](<../../../../work/natural-moveset/native-interactions-corvo-v662/food-source.mp4>)
- [in_game_recordings: gestures-v1261b/host-food-observer.mp4](<../../../../work/natural-moveset/gestures-v1261b/host-food-observer.mp4>)
- [in_game_recordings: gestures-v1261b/client-food-observer.mp4](<../../../../work/natural-moveset/gestures-v1261b/client-food-observer.mp4>)
## carry_object_place_throw

Audit 1269 (1214m): holding a native movable (HoldObject49) presents a held reach and its throw finishes it, both roles; the object itself is not drawn in the partner's hand and movables are not world-synced yet. Recorded pickup, idle, walking, placement and throw studies with two palm contacts and a neutral box. Placement reverses the lift study; lowering dynamics need refinement. The release trajectory is illustrative, with no native object attachment or physics ownership.

````json
{
  "id": "carry_object_place_throw",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_hold_object_gesture_both_roles_product",
  "visual_validation": "accepted_functional_both_roles_held_reach",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "carry object place throw",
  "motions": [
    "carry_object_pickup",
    "carry_object_idle",
    "carry_object_walk",
    "carry_object_place",
    "carry_object_throw"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1269 (1214m): holding a native movable (HoldObject49) presents a held reach and its throw finishes it, both roles; the object itself is not drawn in the partner's hand and movables are not world-synced yet. Recorded pickup, idle, walking, placement and throw studies with two palm contacts and a neutral box. Placement reverses the lift study; lowering dynamics need refinement. The release trajectory is illustrative, with no native object attachment or physics ownership.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "object-carry-v1269c/host-bottle-observer.mp4",
    "object-carry-v1269c/client-bottle-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/object-carry-audit-v1269.json"
  ]
}
````

- [evidence: ../movement-integration/object-carry-audit-v1269.json](<../../../../work/movement-integration/object-carry-audit-v1269.json>)
- [in_game_recordings: object-carry-v1269c/host-bottle-observer.mp4](<../../../../work/natural-moveset/object-carry-v1269c/host-bottle-observer.mp4>)
- [in_game_recordings: object-carry-v1269c/client-bottle-observer.mp4](<../../../../work/natural-moveset/object-carry-v1269c/client-bottle-observer.mp4>)
## shop_dialogue_gestures

Candidates require animation review and appropriate game-state/contact binding.

````json
{
  "id": "shop_dialogue_gestures",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "shop dialogue gestures",
  "motions": [
    "gesture_armcrossed_openhand",
    "posture_talkcenter",
    "gesture_openhand_think",
    "empty_distraction_clipboard_loop_talk"
  ],
  "deliverable_kind": "animation",
  "notes": "Candidates require animation review and appropriate game-state/contact binding.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": []
}
````

## sit_stand

Not applicable (audit 1240, confirmed static): D2 has no player sitting state among its arkPlayerState classes; the only sitting strings are NPC AI providers. Seated story moments are native cinematics, covered by cutscene_enter_exit. The authored NPC bench clips remain in the library unbound.

````json
{
  "id": "sit_stand",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "sit stand",
  "motions": [
    "empty_distraction_sitting_bench_loop",
    "empty_distraction_sitting_bench_out",
    "empty_distraction_sitting_bench_out_fast",
    "empty_distraction_sitting_bench_in_walkright",
    "empty_distraction_sitting_bench_out_walk",
    "empty_distraction_sitting_bench_in"
  ],
  "deliverable_kind": "not_applicable_no_native_player_state",
  "notes": "Not applicable (audit 1240, confirmed static): D2 has no player sitting state among its arkPlayerState classes; the only sitting strings are NPC AI providers. Seated story moments are native cinematics, covered by cutscene_enter_exit. The authored NPC bench clips remain in the library unbound.",
  "phase_status": "not_applicable",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/not-applicable-audit-v1240.json"
  ]
}
````

- [evidence: ../movement-integration/not-applicable-audit-v1240.json](<../../../../work/movement-integration/not-applicable-audit-v1240.json>)
## scripted_scene_pose_handoff

A context-dependent animation ownership transition. Audit 1241 (1213m): on the native confined mantle the partner holds the scene pose at full weight, then blends from that actual pose to locomotion over ~250 ms (traversal weight 0.34/0.07/0.02 while locomotion returns 0.49/0.89/0.97), in both roles. Use scenes recover the same way (vise reach videos, audit 1235). Other scene families, an unclassified-state frame at the scene end and visual acceptance (#33) remain open.

````json
{
  "id": "scripted_scene_pose_handoff",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "missing",
  "runtime_binding": "partial_native_scene_exit_blend_to_locomotion_measured_window_mantle_both_roles",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "scripted scene pose handoff",
  "motions": [],
  "deliverable_kind": "lifecycle_transition",
  "notes": "A context-dependent animation ownership transition. Audit 1241 (1213m): on the native confined mantle the partner holds the scene pose at full weight, then blends from that actual pose to locomotion over ~250 ms (traversal weight 0.34/0.07/0.02 while locomotion returns 0.49/0.89/0.97), in both roles. Use scenes recover the same way (vise reach videos, audit 1235). Other scene families, an unclassified-state frame at the scene end and visual acceptance (#33) remain open.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/presentation-hold-audit-v1241.json",
    "../movement-integration/window-traversal-audit-v1238.json",
    "../movement-integration/use-reach-audit-v1235.json"
  ]
}
````

- [evidence: ../movement-integration/presentation-hold-audit-v1241.json](<../../../../work/movement-integration/presentation-hold-audit-v1241.json>)
- [evidence: ../movement-integration/window-traversal-audit-v1238.json](<../../../../work/movement-integration/window-traversal-audit-v1238.json>)
- [evidence: ../movement-integration/use-reach-audit-v1235.json](<../../../../work/movement-integration/use-reach-audit-v1235.json>)
## cutscene_enter_exit

Native cutscene animation remains scene-specific. Entry/exit ownership and blends must be tested against real scenes; one generic clip cannot cover them. Audit 1239 (#8, 1213l): both peers enter the native Mayhew scene together, the partner presentation retires for the scene and is live again 1.57 s after it; visual accepted on both windows.

````json
{
  "id": "cutscene_enter_exit",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "missing",
  "runtime_binding": "native_cinematic5_partner_retired_and_restored_both_roles",
  "visual_validation": "accepted_mayhew_scene_both_windows",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "cutscene enter exit",
  "motions": [],
  "deliverable_kind": "lifecycle_transition",
  "notes": "Native cutscene animation remains scene-specific. Entry/exit ownership and blends must be tested against real scenes; one generic clip cannot cover them. Audit 1239 (#8, 1213l): both peers enter the native Mayhew scene together, the partner presentation retires for the scene and is live again 1.57 s after it; visual accepted on both windows.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "cutscene-transition-v1236b/scene-host.mp4",
    "cutscene-transition-v1236b/scene-client.mp4"
  ],
  "evidence": [
    "../movement-integration/cutscene-transition-audit-v1239.json"
  ]
}
````

- [evidence: ../movement-integration/cutscene-transition-audit-v1239.json](<../../../../work/movement-integration/cutscene-transition-audit-v1239.json>)
- [in_game_recordings: cutscene-transition-v1236b/scene-host.mp4](<../../../../work/natural-moveset/cutscene-transition-v1236b/scene-host.mp4>)
- [in_game_recordings: cutscene-transition-v1236b/scene-client.mp4](<../../../../work/natural-moveset/cutscene-transition-v1236b/scene-client.mp4>)
## map_load_checkpoint_recovery

Visual sweep 1260: insufficient (corvo insufficient, emily accept). The Emily partner visibly re-appears upright after both the map load and the checkpoint restart; no recording shows the Corvo partner on the client side. This is a lifecycle integration requirement, not a standalone animation clip. 1199d (candidate 1203f): after a native map load (client follows the host mission) and after a shared death + native checkpoint restart, the product controller re-arms and the partner avatar is re-created at the remote player on both peers; filmed face to face from both windows. Supporting: 1160 renewal, 1183 product controller. Mission-to-mission transitions, manual save/load and rejoin remain open.

````json
{
  "id": "map_load_checkpoint_recovery",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "missing",
  "runtime_binding": "partial_native_steam_map_load_and_checkpoint_restart_both_peers",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "map load checkpoint recovery",
  "motions": [],
  "deliverable_kind": "lifecycle_transition",
  "notes": "Visual sweep 1260: insufficient (corvo insufficient, emily accept). The Emily partner visibly re-appears upright after both the map load and the checkpoint restart; no recording shows the Corvo partner on the client side. This is a lifecycle integration requirement, not a standalone animation clip. 1199d (candidate 1203f): after a native map load (client follows the host mission) and after a shared death + native checkpoint restart, the product controller re-arms and the partner avatar is re-created at the remote player on both peers; filmed face to face from both windows. Supporting: 1160 renewal, 1183 product controller. Mission-to-mission transitions, manual save/load and rejoin remain open.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "mission-lifecycle-v1199d/load-host.mp4",
    "mission-lifecycle-v1199d/restart-host.mp4"
  ],
  "evidence": [
    "../movement-integration/lifecycle-audit-v1202.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json"
  ]
}
````

- [evidence: ../movement-integration/lifecycle-audit-v1202.json](<../../../../work/movement-integration/lifecycle-audit-v1202.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [in_game_recordings: mission-lifecycle-v1199d/load-host.mp4](<../../../../work/natural-moveset/mission-lifecycle-v1199d/load-host.mp4>)
- [in_game_recordings: mission-lifecycle-v1199d/restart-host.mp4](<../../../../work/natural-moveset/mission-lifecycle-v1199d/restart-host.mp4>)
## incapacitation_and_rescue_if_supported

No separate rescue moveset is planned under the requested shared-death policy: either death resets both players from the same checkpoint. Keep this original conditional requirement visible without inventing a rescue animation.

````json
{
  "id": "incapacitation_and_rescue_if_supported",
  "family": "interactions",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "missing",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 47,
  "priority": 3,
  "title": "incapacitation and rescue if supported",
  "motions": [],
  "deliverable_kind": "conditional_not_planned",
  "notes": "No separate rescue moveset is planned under the requested shared-death policy: either death resets both players from the same checkpoint. Keep this original conditional requirement visible without inventing a rescue animation.",
  "phase_status": "excluded_by_policy",
  "power_visual_requirements": [],
  "in_game_recordings": []
}
````


