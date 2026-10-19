# ranged_and_items: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-19T02:49:54.398822+02:00. All fields below retain the catalog wording.

## pistol_draw_aim_fire_reload_holster

Visual sweep 1260: accepted (corvo accept, emily accept). Both partners read the pistol draw, aim, fire and reload, and the holster and re-equip, cleanly, with the native prop in hand; the aim-stance yaw offset is only a polish issue. Both-role standing native fire/reload/empty/equip/unequip clocks and automatic cosmetic hand prop passed on archived DLL30AB28F8. Remote projectiles, impact/damage synchronization, audio and all upgraded/contextual variants are not established by these receipts. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope. 1088: pistol draw/reload/aim presentation with the native prop also verified on a mission map (Edge of the World) for both roles after the 1071 gate lift.

````json
{
  "id": "pistol_draw_aim_fire_reload_holster",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_ranged_five_phase_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "pistol draw aim fire reload holster",
  "motions": [
    "sword_gun_fire",
    "sword_gun_idle",
    "sword_gun_reload",
    "sword_gun_snapshot_back",
    "sword_gun_snapshot_front",
    "sword_gun_snapshot_left",
    "sword_gun_snapshot_right",
    "sword_gun_unequip",
    "sword_gun_in",
    "sword_gun_out",
    "sword_gun_equip_fast",
    "sword_gun_equip"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Both partners read the pistol draw, aim, fire and reload, and the holster and re-equip, cleanly, with the native prop in hand; the aim-stance yaw offset is only a polish issue. Both-role standing native fire/reload/empty/equip/unequip clocks and automatic cosmetic hand prop passed on archived DLL30AB28F8. Remote projectiles, impact/damage synchronization, audio and all upgraded/contextual variants are not established by these receipts. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope. 1088: pistol draw/reload/aim presentation with the native prop also verified on a mission map (Edge of the World) for both roles after the 1071 gate lift.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "ranged-corvo-gun-network-v29-v1.mp4",
    "ranged-corvo-gun-network-v29-v1-source.mp4",
    "ranged-emily-gun-network-v29-v1.mp4",
    "ranged-emily-gun-network-v29-v1-source.mp4",
    "ranged-corvo-muzzle-v27-v2.mp4",
    "ranged-corvo-muzzle-v27-v2-source.mp4",
    "ranged-emily-muzzle-v27-v2.mp4",
    "ranged-emily-muzzle-v27-v2-source.mp4",
    "ranged-corvo-crouch-gun-v30-v4.mp4",
    "ranged-corvo-crouch-gun-v30-v4-source.mp4",
    "ranged-corvo-stance-gun-v30-v4b.mp4",
    "ranged-corvo-stance-gun-v30-v4b-source.mp4",
    "ranged-emily-empty-gun-cache-0537-v1.mp4",
    "ranged-emily-empty-gun-cache-0537-v1-source.mp4",
    "ranged-corvo-gun-air-v28-v3.mp4",
    "ranged-corvo-gun-air-v28-v3-source.mp4",
    "ranged-corvo-crouch-gun-moving-v30-v4.mp4",
    "ranged-corvo-crouch-gun-moving-v30-v4-source.mp4",
    "ranged-corvo-crouch-moving-muzzle-bd99-v1.mp4",
    "ranged-corvo-crouch-moving-muzzle-bd99-v1-source.mp4",
    "ranged-emily-standing-muzzle-bd99-v1.mp4",
    "ranged-emily-standing-muzzle-bd99-v1-source.mp4",
    "mission-equipment-v1088/equip-client.mp4",
    "mission-equipment-emily-v1088/equip-host.mp4"
  ],
  "evidence": [
    "ranged-corvo-gun-network-v29-v1/result.json",
    "ranged-emily-gun-network-v29-v1/result.json",
    "ranged-corvo-muzzle-v27-v2/result.json",
    "ranged-emily-muzzle-v27-v2/result.json",
    "ranged-corvo-crouch-gun-v30-v4/result.json",
    "ranged-corvo-stance-gun-v30-v4b/result.json",
    "ranged-emily-empty-gun-cache-0537-v1/result.json",
    "ranged-corvo-gun-air-v28-v3/result.json",
    "ranged-corvo-crouch-gun-moving-v30-v4/result.json",
    "ranged-corvo-crouch-moving-muzzle-bd99-v1/result.json",
    "ranged-emily-standing-muzzle-bd99-v1/result.json",
    "../movement-integration/mission-equipment-audit-v1089.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "coverage-v1330e/result.json"
  ],
  "complete": true,
  "visual_accepted": false,
  "completion": {
    "audit": "completion-audit-v1338-ranged-items.json",
    "decision": "complete",
    "scope": "The pistol in both roles: native draw, three shots with the reloads they cause, holster (X held) and redraw; the partner presents the gun (kind 1) through variants 0, 3, 4, 5 and 6 with every shot counted.",
    "takes": [
      {
        "take": "coverage-v1330e",
        "cases": [
          "host-pistol",
          "client-pistol"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330e/host-pistol-observer.mp4",
        "verdict": "Corvo raises the pistol at arm's length for each shot, lowers it to reload, puts it away and draws it again."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: ranged-corvo-gun-network-v29-v1/result.json](<../../../../work/natural-moveset/ranged-corvo-gun-network-v29-v1/result.json>)
- [evidence: ranged-emily-gun-network-v29-v1/result.json](<../../../../work/natural-moveset/ranged-emily-gun-network-v29-v1/result.json>)
- [evidence: ranged-corvo-muzzle-v27-v2/result.json](<../../../../work/natural-moveset/ranged-corvo-muzzle-v27-v2/result.json>)
- [evidence: ranged-emily-muzzle-v27-v2/result.json](<../../../../work/natural-moveset/ranged-emily-muzzle-v27-v2/result.json>)
- [evidence: ranged-corvo-crouch-gun-v30-v4/result.json](<../../../../work/natural-moveset/ranged-corvo-crouch-gun-v30-v4/result.json>)
- [evidence: ranged-corvo-stance-gun-v30-v4b/result.json](<../../../../work/natural-moveset/ranged-corvo-stance-gun-v30-v4b/result.json>)
- [evidence: ranged-emily-empty-gun-cache-0537-v1/result.json](<../../../../work/natural-moveset/ranged-emily-empty-gun-cache-0537-v1/result.json>)
- [evidence: ranged-corvo-gun-air-v28-v3/result.json](<../../../../work/natural-moveset/ranged-corvo-gun-air-v28-v3/result.json>)
- [evidence: ranged-corvo-crouch-gun-moving-v30-v4/result.json](<../../../../work/natural-moveset/ranged-corvo-crouch-gun-moving-v30-v4/result.json>)
- [evidence: ranged-corvo-crouch-moving-muzzle-bd99-v1/result.json](<../../../../work/natural-moveset/ranged-corvo-crouch-moving-muzzle-bd99-v1/result.json>)
- [evidence: ranged-emily-standing-muzzle-bd99-v1/result.json](<../../../../work/natural-moveset/ranged-emily-standing-muzzle-bd99-v1/result.json>)
- [evidence: ../movement-integration/mission-equipment-audit-v1089.json](<../../../../work/movement-integration/mission-equipment-audit-v1089.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [in_game_recordings: ranged-corvo-gun-network-v29-v1.mp4](<../../../../work/natural-moveset/ranged-corvo-gun-network-v29-v1.mp4>)
- [in_game_recordings: ranged-corvo-gun-network-v29-v1-source.mp4](<../../../../work/natural-moveset/ranged-corvo-gun-network-v29-v1-source.mp4>)
- [in_game_recordings: ranged-emily-gun-network-v29-v1.mp4](<../../../../work/natural-moveset/ranged-emily-gun-network-v29-v1.mp4>)
- [in_game_recordings: ranged-emily-gun-network-v29-v1-source.mp4](<../../../../work/natural-moveset/ranged-emily-gun-network-v29-v1-source.mp4>)
- [in_game_recordings: ranged-corvo-muzzle-v27-v2.mp4](<../../../../work/natural-moveset/ranged-corvo-muzzle-v27-v2.mp4>)
- [in_game_recordings: ranged-corvo-muzzle-v27-v2-source.mp4](<../../../../work/natural-moveset/ranged-corvo-muzzle-v27-v2-source.mp4>)
- [in_game_recordings: ranged-emily-muzzle-v27-v2.mp4](<../../../../work/natural-moveset/ranged-emily-muzzle-v27-v2.mp4>)
- [in_game_recordings: ranged-emily-muzzle-v27-v2-source.mp4](<../../../../work/natural-moveset/ranged-emily-muzzle-v27-v2-source.mp4>)
- [in_game_recordings: ranged-corvo-crouch-gun-v30-v4.mp4](<../../../../work/natural-moveset/ranged-corvo-crouch-gun-v30-v4.mp4>)
- [in_game_recordings: ranged-corvo-crouch-gun-v30-v4-source.mp4](<../../../../work/natural-moveset/ranged-corvo-crouch-gun-v30-v4-source.mp4>)
- [in_game_recordings: ranged-corvo-stance-gun-v30-v4b.mp4](<../../../../work/natural-moveset/ranged-corvo-stance-gun-v30-v4b.mp4>)
- [in_game_recordings: ranged-corvo-stance-gun-v30-v4b-source.mp4](<../../../../work/natural-moveset/ranged-corvo-stance-gun-v30-v4b-source.mp4>)
- [in_game_recordings: ranged-emily-empty-gun-cache-0537-v1.mp4](<../../../../work/natural-moveset/ranged-emily-empty-gun-cache-0537-v1.mp4>)
- [in_game_recordings: ranged-emily-empty-gun-cache-0537-v1-source.mp4](<../../../../work/natural-moveset/ranged-emily-empty-gun-cache-0537-v1-source.mp4>)
- [in_game_recordings: ranged-corvo-gun-air-v28-v3.mp4](<../../../../work/natural-moveset/ranged-corvo-gun-air-v28-v3.mp4>)
- [in_game_recordings: ranged-corvo-gun-air-v28-v3-source.mp4](<../../../../work/natural-moveset/ranged-corvo-gun-air-v28-v3-source.mp4>)
- [in_game_recordings: ranged-corvo-crouch-gun-moving-v30-v4.mp4](<../../../../work/natural-moveset/ranged-corvo-crouch-gun-moving-v30-v4.mp4>)
- [in_game_recordings: ranged-corvo-crouch-gun-moving-v30-v4-source.mp4](<../../../../work/natural-moveset/ranged-corvo-crouch-gun-moving-v30-v4-source.mp4>)
- [in_game_recordings: ranged-corvo-crouch-moving-muzzle-bd99-v1.mp4](<../../../../work/natural-moveset/ranged-corvo-crouch-moving-muzzle-bd99-v1.mp4>)
- [in_game_recordings: ranged-corvo-crouch-moving-muzzle-bd99-v1-source.mp4](<../../../../work/natural-moveset/ranged-corvo-crouch-moving-muzzle-bd99-v1-source.mp4>)
- [in_game_recordings: ranged-emily-standing-muzzle-bd99-v1.mp4](<../../../../work/natural-moveset/ranged-emily-standing-muzzle-bd99-v1.mp4>)
- [in_game_recordings: ranged-emily-standing-muzzle-bd99-v1-source.mp4](<../../../../work/natural-moveset/ranged-emily-standing-muzzle-bd99-v1-source.mp4>)
- [in_game_recordings: mission-equipment-v1088/equip-client.mp4](<../../../../work/natural-moveset/mission-equipment-v1088/equip-client.mp4>)
- [in_game_recordings: mission-equipment-emily-v1088/equip-host.mp4](<../../../../work/natural-moveset/mission-equipment-emily-v1088/equip-host.mp4>)
## crossbow_draw_aim_fire_reload_holster

Visual sweep 1260: accepted (corvo accept, emily accept). The crossbow aim, fire, reload and holster read correctly for both partners with the crossbow prop in hand; the stance reuses the generic one-arm ranged pose. Both-role standing native fire/reload/empty/equip/unequip clocks and automatic cosmetic hand prop passed on archived DLL30AB28F8. Ammunition switching, projectile/impact outcomes, precise attachment and broader variants remain incomplete. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.

````json
{
  "id": "crossbow_draw_aim_fire_reload_holster",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_ranged_five_phase_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "crossbow draw aim fire reload holster",
  "motions": [
    "crossbow_equip",
    "crossbow_idle",
    "crossbow_fire",
    "crossbow_reload_in",
    "crossbow_reload_loop",
    "crossbow_reload_out",
    "crossbow_ooa",
    "crossbow_unequip",
    "crossbow_sequence",
    "crossbow_equip_cancel_30",
    "crossbow_equip_cancel_70",
    "crossbow_ammo_regular_to_sleep",
    "crossbow_ammo_sleep_to_regular",
    "crossbow_reload_native_timing"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The crossbow aim, fire, reload and holster read correctly for both partners with the crossbow prop in hand; the stance reuses the generic one-arm ranged pose. Both-role standing native fire/reload/empty/equip/unequip clocks and automatic cosmetic hand prop passed on archived DLL30AB28F8. Ammunition switching, projectile/impact outcomes, precise attachment and broader variants remain incomplete. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "ranged-corvo-crossbow-network-v29-v1.mp4",
    "ranged-corvo-crossbow-network-v29-v1-source.mp4",
    "ranged-emily-crossbow-network-v29-v1.mp4",
    "ranged-emily-crossbow-network-v29-v1-source.mp4",
    "ranged-corvo-crossbow-native-state-v26-v5.mp4",
    "ranged-corvo-crossbow-native-state-v26-v5-source.mp4",
    "ranged-emily-crossbow-native-state-v26-v5.mp4",
    "ranged-emily-crossbow-native-state-v26-v5-source.mp4",
    "ranged-emily-crouch-crossbow-v30-v4.mp4",
    "ranged-emily-crouch-crossbow-v30-v4-source.mp4",
    "ranged-emily-stance-crossbow-v30-v4.mp4",
    "ranged-emily-stance-crossbow-v30-v4-source.mp4",
    "ranged-emily-empty-crossbow-cache-0537-v1.mp4",
    "ranged-emily-empty-crossbow-cache-0537-v1-source.mp4",
    "ranged-corvo-standing-crossbow-0537-v1.mp4",
    "ranged-corvo-standing-crossbow-0537-v1-source.mp4",
    "ranged-emily-crossbow-air-v28-v1.mp4",
    "ranged-emily-crossbow-air-v28-v1-source.mp4",
    "ranged-emily-crouch-crossbow-moving-v30-v4.mp4",
    "ranged-emily-crouch-crossbow-moving-v30-v4-source.mp4"
  ],
  "evidence": [
    "ranged-corvo-crossbow-network-v29-v1/result.json",
    "ranged-emily-crossbow-network-v29-v1/result.json",
    "ranged-corvo-crossbow-native-state-v26-v5/result.json",
    "ranged-emily-crossbow-native-state-v26-v5/result.json",
    "ranged-emily-crouch-crossbow-v30-v4/result.json",
    "ranged-emily-stance-crossbow-v30-v4/result.json",
    "ranged-emily-empty-crossbow-cache-0537-v1/result.json",
    "ranged-corvo-standing-crossbow-0537-v1/result.json",
    "ranged-emily-crossbow-air-v28-v1/result.json",
    "ranged-emily-crouch-crossbow-moving-v30-v4/result.json",
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "coverage-v1330e/result.json"
  ],
  "complete": true,
  "visual_accepted": false,
  "completion": {
    "audit": "completion-audit-v1338-ranged-items.json",
    "decision": "complete",
    "scope": "The crossbow in both roles: native draw, three bolts with reloads, holster and redraw; the partner presents the crossbow (kind 2) through variants 0 and 7-10, three shots each.",
    "takes": [
      {
        "take": "coverage-v1330e",
        "cases": [
          "host-crossbow",
          "client-crossbow"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330e/host-crossbow-observer.mp4",
        "verdict": "Corvo extends the wrist crossbow and holds it level for each bolt, recocks between shots, then lowers the arm to holster and raises it again."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: ranged-corvo-crossbow-network-v29-v1/result.json](<../../../../work/natural-moveset/ranged-corvo-crossbow-network-v29-v1/result.json>)
- [evidence: ranged-emily-crossbow-network-v29-v1/result.json](<../../../../work/natural-moveset/ranged-emily-crossbow-network-v29-v1/result.json>)
- [evidence: ranged-corvo-crossbow-native-state-v26-v5/result.json](<../../../../work/natural-moveset/ranged-corvo-crossbow-native-state-v26-v5/result.json>)
- [evidence: ranged-emily-crossbow-native-state-v26-v5/result.json](<../../../../work/natural-moveset/ranged-emily-crossbow-native-state-v26-v5/result.json>)
- [evidence: ranged-emily-crouch-crossbow-v30-v4/result.json](<../../../../work/natural-moveset/ranged-emily-crouch-crossbow-v30-v4/result.json>)
- [evidence: ranged-emily-stance-crossbow-v30-v4/result.json](<../../../../work/natural-moveset/ranged-emily-stance-crossbow-v30-v4/result.json>)
- [evidence: ranged-emily-empty-crossbow-cache-0537-v1/result.json](<../../../../work/natural-moveset/ranged-emily-empty-crossbow-cache-0537-v1/result.json>)
- [evidence: ranged-corvo-standing-crossbow-0537-v1/result.json](<../../../../work/natural-moveset/ranged-corvo-standing-crossbow-0537-v1/result.json>)
- [evidence: ranged-emily-crossbow-air-v28-v1/result.json](<../../../../work/natural-moveset/ranged-emily-crossbow-air-v28-v1/result.json>)
- [evidence: ranged-emily-crouch-crossbow-moving-v30-v4/result.json](<../../../../work/natural-moveset/ranged-emily-crouch-crossbow-moving-v30-v4/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [in_game_recordings: ranged-corvo-crossbow-network-v29-v1.mp4](<../../../../work/natural-moveset/ranged-corvo-crossbow-network-v29-v1.mp4>)
- [in_game_recordings: ranged-corvo-crossbow-network-v29-v1-source.mp4](<../../../../work/natural-moveset/ranged-corvo-crossbow-network-v29-v1-source.mp4>)
- [in_game_recordings: ranged-emily-crossbow-network-v29-v1.mp4](<../../../../work/natural-moveset/ranged-emily-crossbow-network-v29-v1.mp4>)
- [in_game_recordings: ranged-emily-crossbow-network-v29-v1-source.mp4](<../../../../work/natural-moveset/ranged-emily-crossbow-network-v29-v1-source.mp4>)
- [in_game_recordings: ranged-corvo-crossbow-native-state-v26-v5.mp4](<../../../../work/natural-moveset/ranged-corvo-crossbow-native-state-v26-v5.mp4>)
- [in_game_recordings: ranged-corvo-crossbow-native-state-v26-v5-source.mp4](<../../../../work/natural-moveset/ranged-corvo-crossbow-native-state-v26-v5-source.mp4>)
- [in_game_recordings: ranged-emily-crossbow-native-state-v26-v5.mp4](<../../../../work/natural-moveset/ranged-emily-crossbow-native-state-v26-v5.mp4>)
- [in_game_recordings: ranged-emily-crossbow-native-state-v26-v5-source.mp4](<../../../../work/natural-moveset/ranged-emily-crossbow-native-state-v26-v5-source.mp4>)
- [in_game_recordings: ranged-emily-crouch-crossbow-v30-v4.mp4](<../../../../work/natural-moveset/ranged-emily-crouch-crossbow-v30-v4.mp4>)
- [in_game_recordings: ranged-emily-crouch-crossbow-v30-v4-source.mp4](<../../../../work/natural-moveset/ranged-emily-crouch-crossbow-v30-v4-source.mp4>)
- [in_game_recordings: ranged-emily-stance-crossbow-v30-v4.mp4](<../../../../work/natural-moveset/ranged-emily-stance-crossbow-v30-v4.mp4>)
- [in_game_recordings: ranged-emily-stance-crossbow-v30-v4-source.mp4](<../../../../work/natural-moveset/ranged-emily-stance-crossbow-v30-v4-source.mp4>)
- [in_game_recordings: ranged-emily-empty-crossbow-cache-0537-v1.mp4](<../../../../work/natural-moveset/ranged-emily-empty-crossbow-cache-0537-v1.mp4>)
- [in_game_recordings: ranged-emily-empty-crossbow-cache-0537-v1-source.mp4](<../../../../work/natural-moveset/ranged-emily-empty-crossbow-cache-0537-v1-source.mp4>)
- [in_game_recordings: ranged-corvo-standing-crossbow-0537-v1.mp4](<../../../../work/natural-moveset/ranged-corvo-standing-crossbow-0537-v1.mp4>)
- [in_game_recordings: ranged-corvo-standing-crossbow-0537-v1-source.mp4](<../../../../work/natural-moveset/ranged-corvo-standing-crossbow-0537-v1-source.mp4>)
- [in_game_recordings: ranged-emily-crossbow-air-v28-v1.mp4](<../../../../work/natural-moveset/ranged-emily-crossbow-air-v28-v1.mp4>)
- [in_game_recordings: ranged-emily-crossbow-air-v28-v1-source.mp4](<../../../../work/natural-moveset/ranged-emily-crossbow-air-v28-v1-source.mp4>)
- [in_game_recordings: ranged-emily-crouch-crossbow-moving-v30-v4.mp4](<../../../../work/natural-moveset/ranged-emily-crouch-crossbow-moving-v30-v4.mp4>)
- [in_game_recordings: ranged-emily-crouch-crossbow-moving-v30-v4-source.mp4](<../../../../work/natural-moveset/ranged-emily-crouch-crossbow-moving-v30-v4-source.mp4>)
## bolt_or_ammo_switch

Regular/sleep-bolt changes are recorded in both directions with native bolt models and exact native attachment tags. The lower/holster/equip choreography is authored, not a confirmed native FP ammo-selection gesture. Inventory state, selection timing, additional ammunition and live equipment binding remain pending.

````json
{
  "id": "bolt_or_ammo_switch",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "bolt or ammo switch",
  "motions": [
    "crossbow_ammo_regular_to_sleep",
    "crossbow_ammo_sleep_to_regular"
  ],
  "deliverable_kind": "animation",
  "notes": "Regular/sleep-bolt changes are recorded in both directions with native bolt models and exact native attachment tags. The lower/holster/equip choreography is authored, not a confirmed native FP ammo-selection gesture. Inventory state, selection timing, additional ammunition and live equipment binding remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "completion": {
    "audit": "completion-audit-v1340-switching.json",
    "decision": "complete",
    "scope": "With the crossbow drawn, selecting sleep darts (the native ammo switch, equip change 64) and switching back to bolts in both roles: the partner presents the crossbow through both switches (59 samples, variants 0 and 7-10), fires one dart, and never drops the presentation.",
    "takes": [
      {
        "take": "mission-coverage-v1328j",
        "cases": [
          "host-boltswitch",
          "client-boltswitch"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328j/host-boltswitch-observer.mp4",
        "verdict": "Corvo holds the crossbow aimed through the dart selection, raises it to fire a dart, re-aims after switching back and finally lowers his arm to holster."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true,
  "evidence": [
    "mission-coverage-v1328j/result.json"
  ]
}
````

- [evidence: mission-coverage-v1328j/result.json](<../../../../work/natural-moveset/mission-coverage-v1328j/result.json>)
## grenade_equip_aim_throw

Audit 1267: release pop fixed in bank 1266 (pelvis travel scaled to 25%); throw accepted from both windows. Visual sweep 1260: reject (corvo reject, emily reject). The cook and throw are recognizable for both partners, but on the high throw the body pops sideways at release (Corvo about 0.5-0.75 m over two frames, Emily about 0.35 m in one frame); the low throws are acceptable. Native standing cooking/release/empty recovery drives the isolated remote body and held prop over Steam. Crouched phases have passed in the individually recorded contexts. Passed takes also verify six-joint native mechanisms, owned in-hand effects and specific same-world menu recovery cases; consult each result for its exact scope. Release hide, duplicate refusal, inventory rollback and checkpoint restoration checked. Projectile/explosion/world/damage replication, untested movement/stance/air contexts, broader interruption recovery and fine hand/foot acceptance remain incomplete.

````json
{
  "id": "grenade_equip_aim_throw",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_network_ground_grenade_partial_verified",
  "visual_validation": "accepted_functional_both_roles_bank_1266",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "grenade equip aim throw",
  "motions": [
    "sword_grenade_in",
    "sword_grenade_jumpaway_toss",
    "sword_grenade_launch",
    "sword_grenade_launch_down"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1267: release pop fixed in bank 1266 (pelvis travel scaled to 25%); throw accepted from both windows. Visual sweep 1260: reject (corvo reject, emily reject). The cook and throw are recognizable for both partners, but on the high throw the body pops sideways at release (Corvo about 0.5-0.75 m over two frames, Emily about 0.35 m in one frame); the low throws are acceptable. Native standing cooking/release/empty recovery drives the isolated remote body and held prop over Steam. Crouched phases have passed in the individually recorded contexts. Passed takes also verify six-joint native mechanisms, owned in-hand effects and specific same-world menu recovery cases; consult each result for its exact scope. Release hide, duplicate refusal, inventory rollback and checkpoint restoration checked. Projectile/explosion/world/damage replication, untested movement/stance/air contexts, broader interruption recovery and fine hand/foot acceptance remain incomplete.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-grenade-actions-corvo-high-v54.mp4",
    "native-grenade-actions-corvo-low-v54.mp4",
    "native-grenade-actions-emily-high-v54.mp4",
    "native-grenade-equip-corvo-v52-v2.mp4",
    "native-grenade-equip-corvo-v52-v3.mp4",
    "native-grenade-equip-emily-v52.mp4",
    "native-grenade-reader-corvo-high-v54.mp4",
    "native-grenade-reader-emily-low-v54.mp4",
    "native-grenade-scene-corvo-v53.mp4",
    "native-grenade-scene-emily-v53.mp4",
    "native-grenade-scene-emily-v53-v2.mp4",
    "native-grenade-scene-emily-v53-v3.mp4",
    "grenade-network-corvo-high-v56h.mp4",
    "grenade-network-corvo-high-v56h-source.mp4",
    "grenade-prop-corvo-idle-v55.mp4",
    "grenade-prop-corvo-close-v55b.mp4",
    "grenade-prop-emily-close-v55b.mp4",
    "grenade-network-corvo-high-v56i.mp4",
    "grenade-network-corvo-high-v56i-source.mp4",
    "grenade-network-corvo-low-v56i.mp4",
    "grenade-network-corvo-low-v56i-source.mp4",
    "grenade-network-corvo-low-v56i2.mp4",
    "grenade-network-corvo-low-v56i2-source.mp4",
    "grenade-network-emily-high-v56i.mp4",
    "grenade-network-emily-high-v56i-source.mp4",
    "grenade-network-emily-low-v56i.mp4",
    "grenade-network-emily-low-v56i-source.mp4",
    "grenade-network-emily-low-v56i2.mp4",
    "grenade-network-emily-low-v56i2-source.mp4",
    "grenade-mechanism-corvo-front-v57b.mp4",
    "grenade-mechanism-corvo-front-v57b-source.mp4",
    "grenade-mechanism-emily-front-v57b.mp4",
    "grenade-mechanism-emily-front-v57b-source.mp4",
    "grenade-fx-corvo-close-v58.mp4",
    "grenade-fx-corvo-close-v58-source.mp4",
    "grenade-fx-emily-close-v58.mp4",
    "grenade-fx-emily-close-v58-source.mp4",
    "grenade-interrupt-receiver-stop-v58b.mp4",
    "grenade-interrupt-receiver-stop-v58b-source.mp4",
    "grenade-interrupt-source-pause-v58.mp4",
    "grenade-interrupt-source-pause-v58-source.mp4",
    "grenade-interrupt-receiver-pause-v58.mp4",
    "grenade-interrupt-receiver-pause-v58-source.mp4",
    "grenade-interrupt-receiver-pause-v58b.mp4",
    "grenade-interrupt-receiver-pause-v58b-source.mp4",
    "grenade-resume-source-v59.mp4",
    "grenade-resume-source-v59-source.mp4",
    "grenade-resume-receiver-emily-v59.mp4",
    "grenade-resume-receiver-emily-v59-source.mp4",
    "grenade-impact-state-corvo-v59.mp4",
    "grenade-impact-state-corvo-v59-source.mp4",
    "grenade-crouch-observe-corvo-v60.mp4",
    "grenade-crouch-observe-emily-v62.mp4",
    "grenade-crouch-network-corvo-v62b.mp4",
    "grenade-crouch-network-corvo-v62b-source.mp4",
    "grenade-crouch-moving-emily-v62b.mp4",
    "grenade-crouch-moving-emily-v62b-source.mp4",
    "grenade-crouch-low-moving-corvo-v62b.mp4",
    "grenade-crouch-low-moving-corvo-v62b-source.mp4",
    "grenade-crouch-low-moving-corvo-v62c.mp4",
    "grenade-crouch-low-moving-corvo-v62c-source.mp4",
    "grenade-crouch-low-emily-v62c.mp4",
    "grenade-crouch-low-emily-v62c-source.mp4",
    "grenade-air-observe-corvo-v63.mp4",
    "grenade-air-observe-emily-v63.mp4",
    "grenade-air-network-corvo-v63.mp4",
    "grenade-air-network-corvo-v63-source.mp4",
    "grenade-air-network-corvo-v63b.mp4",
    "grenade-air-network-corvo-v63b-source.mp4",
    "grenade-air-network-emily-v63b.mp4",
    "grenade-air-network-emily-v63b-source.mp4",
    "grenade-reader-host-v320.mp4",
    "grenade-reader-host-v320-source.mp4",
    "grenade-reader-emily-v320.mp4",
    "grenade-reader-emily-v320-source.mp4",
    "grenade-network-host-v1266a.mp4",
    "grenade-network-client-v1266a.mp4"
  ],
  "native_integration": {
    "native-grenade-actions-corvo-high-v54.mp4": {
      "video": "native-grenade-actions-corvo-high-v54.mp4",
      "character": "corvo",
      "label": "Native grenade source only · native-grenade-actions-corvo-high-v54",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-actions-corvo-high-v54/result.json",
      "visual_review": "native-grenade-actions-corvo-high-v54/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.069637883008358,
      "video_sha256": "B9A1438EFBA74164404DB9BCD607B99FC399617A7670C17FA0ACB0D3DA65D4E6",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "native-grenade-actions-corvo-low-v54.mp4": {
      "video": "native-grenade-actions-corvo-low-v54.mp4",
      "character": "corvo",
      "label": "Native grenade source only · native-grenade-actions-corvo-low-v54",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-actions-corvo-low-v54/result.json",
      "visual_review": "native-grenade-actions-corvo-low-v54/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.916666666666668,
      "video_sha256": "05A5FEF28AE7AE86E5CBC675992524696F9C1FACCD439A631CDBB510E247108B",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "native-grenade-actions-emily-high-v54.mp4": {
      "video": "native-grenade-actions-emily-high-v54.mp4",
      "character": "emily",
      "label": "Native grenade source only · native-grenade-actions-emily-high-v54",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-actions-emily-high-v54/result.json",
      "visual_review": "native-grenade-actions-emily-high-v54/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 24.5,
      "video_sha256": "2D6D1FC5565B0AC5633C6752158621EB6AE0477F5F214BF3D681BE45D937B392",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "native-grenade-equip-corvo-v52-v2.mp4": {
      "video": "native-grenade-equip-corvo-v52-v2.mp4",
      "character": "corvo",
      "label": "Native grenade source only · native-grenade-equip-corvo-v52-v2",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-equip-corvo-v52-v2/result.json",
      "visual_review": "native-grenade-equip-corvo-v52-v2/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.166666666666668,
      "video_sha256": "B06983DFEC102304A19B2613AE0911BC4CAFA194E13B96B4C2D5D10B65CDDA2E",
      "checks_passed": false,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "native-grenade-equip-corvo-v52-v3.mp4": {
      "video": "native-grenade-equip-corvo-v52-v3.mp4",
      "character": "corvo",
      "label": "Native grenade source only · native-grenade-equip-corvo-v52-v3",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-equip-corvo-v52-v3/result.json",
      "visual_review": "native-grenade-equip-corvo-v52-v3/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.6875,
      "video_sha256": "74BB4E05B5460E16FED18D854DCBA7AD9B5DE7D53E7E4804DF373D94E61A998D",
      "checks_passed": true,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "native-grenade-equip-emily-v52.mp4": {
      "video": "native-grenade-equip-emily-v52.mp4",
      "character": "emily",
      "label": "Native grenade source only · native-grenade-equip-emily-v52",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-equip-emily-v52/result.json",
      "visual_review": "native-grenade-equip-emily-v52/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.125,
      "video_sha256": "D75DC7A2CB7E032C5FAC362BFD354CB51589A096BA2BB271A97CFE95CA5C6D65",
      "checks_passed": true,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "native-grenade-reader-corvo-high-v54.mp4": {
      "video": "native-grenade-reader-corvo-high-v54.mp4",
      "character": "corvo",
      "label": "Native grenade source only · native-grenade-reader-corvo-high-v54",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-reader-corvo-high-v54/result.json",
      "visual_review": "native-grenade-reader-corvo-high-v54/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 22.833333333333332,
      "video_sha256": "8DA794BA8C16DF391A9E9DE5A0C588CF7F49372F9F36C18BBBA677779C017A5C",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "native-grenade-reader-emily-low-v54.mp4": {
      "video": "native-grenade-reader-emily-low-v54.mp4",
      "character": "emily",
      "label": "Native grenade source only · native-grenade-reader-emily-low-v54",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-reader-emily-low-v54/result.json",
      "visual_review": "native-grenade-reader-emily-low-v54/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 23.583333333333332,
      "video_sha256": "259F3620EEAB81192CDF7543BE8727CA258ECD5C008D1F94D44FF12896B6C8F6",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "native-grenade-scene-corvo-v53.mp4": {
      "video": "native-grenade-scene-corvo-v53.mp4",
      "character": "corvo",
      "label": "Native grenade source only · native-grenade-scene-corvo-v53",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-scene-corvo-v53/result.json",
      "visual_review": "native-grenade-scene-corvo-v53/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.875,
      "video_sha256": "CC397FC1A862860F61590F9C678F395B0B726838CFD6752C3E5C9B38CBD29B3E",
      "checks_passed": true,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "native-grenade-scene-emily-v53.mp4": {
      "video": "native-grenade-scene-emily-v53.mp4",
      "character": "emily",
      "label": "Native grenade source only · native-grenade-scene-emily-v53",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-scene-emily-v53/result.json",
      "visual_review": null,
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 24.989561586638832,
      "video_sha256": "AE97DBD5B7D3A344F09F5DDB5128428F225B361023CF4D1A6E5D093A6C7E198A",
      "checks_passed": false,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "native-grenade-scene-emily-v53-v2.mp4": {
      "video": "native-grenade-scene-emily-v53-v2.mp4",
      "character": "emily",
      "label": "Native grenade source only · native-grenade-scene-emily-v53-v2",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-scene-emily-v53-v2/result.json",
      "visual_review": null,
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 25.36534446764092,
      "video_sha256": "30D345B0681262D2F9FC53C4237AC46230844EA3F32D43B8D7EC624B6E4E4AB6",
      "checks_passed": false,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "native-grenade-scene-emily-v53-v3.mp4": {
      "video": "native-grenade-scene-emily-v53-v3.mp4",
      "character": "emily",
      "label": "Native grenade source only · native-grenade-scene-emily-v53-v3",
      "alias": "grenade_equip_aim_throw",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-grenade-scene-emily-v53-v3/result.json",
      "visual_review": "native-grenade-scene-emily-v53-v3/visual-review.json",
      "proxy_sha256": "C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1",
      "actual_capture_hz": 23.862212943632567,
      "video_sha256": "F68340EB50B20CD7B3CF643FDFFBCBAA9F255452ECFFFD05E75A5160F7F5ED0A",
      "checks_passed": true,
      "scope": "Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed."
    },
    "grenade-prop-corvo-idle-v55.mp4": {
      "video": "grenade-prop-corvo-idle-v55.mp4",
      "character": "corvo",
      "label": "Grenade native cosmetic prop · grenade-prop-corvo-idle-v55",
      "alias": "grenade_idle",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-prop-corvo-idle-v55/result.json",
      "visual_review": "grenade-prop-corvo-idle-v55/visual-review.json",
      "proxy_sha256": "9660D36E1104FED8D40F412908C019F453268178FD0209718343C1B312F2FC5C",
      "actual_capture_hz": 27.058823529411764,
      "video_sha256": "D64148BFB7503C946304242AE58CBBDE070E5BB7A8AF4CD8514937C32E07C7A9",
      "checks_passed": false,
      "scope": "Bounded native grenade cosmetic attachment and authored idle experiment. No source grenade grant, projectile, native gameplay event or network grenade binding. Visual acceptance separate."
    },
    "grenade-prop-corvo-close-v55b.mp4": {
      "video": "grenade-prop-corvo-close-v55b.mp4",
      "character": "corvo",
      "label": "Grenade native cosmetic prop · grenade-prop-corvo-close-v55b",
      "alias": "grenade_idle",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-prop-corvo-close-v55b/result.json",
      "visual_review": "grenade-prop-corvo-close-v55b/visual-review.json",
      "proxy_sha256": "AFE5E632E62C54F2A8DAFC4456566F76722D092923C7C726B3D2016DD7C14D84",
      "actual_capture_hz": 27.58823529411765,
      "video_sha256": "1C6FD55E9B22ADB11DF28B15E3F596447F92D5C6311F2798AD1FB903E68C82BA",
      "checks_passed": true,
      "scope": "Bounded native grenade cosmetic attachment and authored idle experiment. No source grenade grant, projectile, native gameplay event or network grenade binding. Visual acceptance separate."
    },
    "grenade-prop-emily-close-v55b.mp4": {
      "video": "grenade-prop-emily-close-v55b.mp4",
      "character": "emily",
      "label": "Grenade native cosmetic prop · grenade-prop-emily-close-v55b",
      "alias": "grenade_idle",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-prop-emily-close-v55b/result.json",
      "visual_review": "grenade-prop-emily-close-v55b/visual-review.json",
      "proxy_sha256": "AFE5E632E62C54F2A8DAFC4456566F76722D092923C7C726B3D2016DD7C14D84",
      "actual_capture_hz": 27.764705882352942,
      "video_sha256": "91E86AC1203F66E3345C20FDA6C216A4C04297148343C6E819D4683AF918E2DC",
      "checks_passed": true,
      "scope": "Bounded native grenade cosmetic attachment and authored idle experiment. No source grenade grant, projectile, native gameplay event or network grenade binding. Visual acceptance separate."
    },
    "grenade-network-corvo-high-v56h.mp4": {
      "video": "grenade-network-corvo-high-v56h.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-network-corvo-high-v56h",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-high-v56h/result.json",
      "visual_review": "grenade-network-corvo-high-v56h/visual-review.json",
      "proxy_sha256": "90B403EC9DD37CC4EBE2B5A95F27EC386CE76774529267473B3FBAEE725DBD59",
      "actual_capture_hz": 27.66016713091922,
      "video_sha256": "08375EF10308B57E5CF99A332B7F7F1B34E3BAE1C88FFC5CBF9B8E11550E051B",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-high-v56h-source.mp4": {
      "video": "grenade-network-corvo-high-v56h-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-network-corvo-high-v56h",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-high-v56h/result.json",
      "visual_review": "grenade-network-corvo-high-v56h/visual-review.json",
      "proxy_sha256": "90B403EC9DD37CC4EBE2B5A95F27EC386CE76774529267473B3FBAEE725DBD59",
      "actual_capture_hz": 27.827298050139277,
      "video_sha256": "FD533D288B35BB05C27016FFD55A4EE97C3019DEECDF5C3041B85AD3BCD91369",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-high-v56i.mp4": {
      "video": "grenade-network-corvo-high-v56i.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-network-corvo-high-v56i",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-high-v56i/result.json",
      "visual_review": "grenade-network-corvo-high-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.5,
      "video_sha256": "0CE0DCE1974430408E60822AC07A434BFAA9BECAD4C8D0D5681F0A658B641C0D",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-high-v56i-source.mp4": {
      "video": "grenade-network-corvo-high-v56i-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-network-corvo-high-v56i",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-high-v56i/result.json",
      "visual_review": "grenade-network-corvo-high-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.166666666666668,
      "video_sha256": "2A45972C45B96830DE01DAF1BCC5C240C4282EB6A9A7EC572C46559D54007068",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-low-v56i.mp4": {
      "video": "grenade-network-corvo-low-v56i.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-network-corvo-low-v56i",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-low-v56i/result.json",
      "visual_review": "grenade-network-corvo-low-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "038D9F8C129A144B14512CC2636088A9F012A0F4F4902CDD76AFC59C6678F736",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-low-v56i-source.mp4": {
      "video": "grenade-network-corvo-low-v56i-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-network-corvo-low-v56i",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-low-v56i/result.json",
      "visual_review": "grenade-network-corvo-low-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "92DE28B007AF4E7958DBD275071DB514F5DFADEFAFA47CE2830CE49D05E92603",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-low-v56i2.mp4": {
      "video": "grenade-network-corvo-low-v56i2.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-network-corvo-low-v56i2",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-low-v56i2/result.json",
      "visual_review": "grenade-network-corvo-low-v56i2/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.0,
      "video_sha256": "05E0CC8ECC73A8E7700961202825854640304EEC63BCD00A9F597853508525DD",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-corvo-low-v56i2-source.mp4": {
      "video": "grenade-network-corvo-low-v56i2-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-network-corvo-low-v56i2",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-corvo-low-v56i2/result.json",
      "visual_review": "grenade-network-corvo-low-v56i2/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "6892567DE125A34C5E9A652CCD9CABCBDF72CA65033821079EBEFA16BDF3E191",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-emily-high-v56i.mp4": {
      "video": "grenade-network-emily-high-v56i.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-network-emily-high-v56i",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-emily-high-v56i/result.json",
      "visual_review": "grenade-network-emily-high-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "1E01676C5C25592934D93BE8DE51790E538EACA86EE16A6F88D258193FB6776D",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-emily-high-v56i-source.mp4": {
      "video": "grenade-network-emily-high-v56i-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-network-emily-high-v56i",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-emily-high-v56i/result.json",
      "visual_review": "grenade-network-emily-high-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.75,
      "video_sha256": "5EAD100B5914F9EF679D89DB0F1A2285057F7DEDA00AE1B0F78485F67E093031",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-emily-low-v56i.mp4": {
      "video": "grenade-network-emily-low-v56i.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-network-emily-low-v56i",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-emily-low-v56i/result.json",
      "visual_review": "grenade-network-emily-low-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "F404172C9A4FEAD52ECBD69C2FDD68ACED96033D70F46756CBBA8C04AAB5DD75",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-emily-low-v56i-source.mp4": {
      "video": "grenade-network-emily-low-v56i-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-network-emily-low-v56i",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-emily-low-v56i/result.json",
      "visual_review": "grenade-network-emily-low-v56i/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "83DBF89ECBE46A8F12B0742B735D654913F826448EB35C29CBEF2F6FD1A9C72D",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-emily-low-v56i2.mp4": {
      "video": "grenade-network-emily-low-v56i2.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-network-emily-low-v56i2",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-emily-low-v56i2/result.json",
      "visual_review": "grenade-network-emily-low-v56i2/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "E4FF6CADAC9BBBD273C03E3A21FD1A7810FCD524C6F915F38AF48C1352597B96",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-network-emily-low-v56i2-source.mp4": {
      "video": "grenade-network-emily-low-v56i2-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-network-emily-low-v56i2",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-network-emily-low-v56i2/result.json",
      "visual_review": "grenade-network-emily-low-v56i2/visual-review.json",
      "proxy_sha256": "035642275AC27A738315BE9E28B59FBC06212320FA27CEA0C07897AD478684BB",
      "actual_capture_hz": 27.75,
      "video_sha256": "B628A830148481F41C65159B2D7C15059D24C4BF9DA5ED4DA5208B288DD54B22",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-mechanism-corvo-front-v57b.mp4": {
      "video": "grenade-mechanism-corvo-front-v57b.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-mechanism-corvo-front-v57b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-mechanism-corvo-front-v57b/result.json",
      "visual_review": "grenade-mechanism-corvo-front-v57b/visual-review.json",
      "proxy_sha256": "D35D9138F74C6316D44770CDC95CCB8B7739844DA7E914FDC894B0BBB5554E8A",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "73FD8466E11109A20158770E14EDC943015D3587781683B69D2B2488CAB6587B",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-mechanism-corvo-front-v57b-source.mp4": {
      "video": "grenade-mechanism-corvo-front-v57b-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-mechanism-corvo-front-v57b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-mechanism-corvo-front-v57b/result.json",
      "visual_review": "grenade-mechanism-corvo-front-v57b/visual-review.json",
      "proxy_sha256": "D35D9138F74C6316D44770CDC95CCB8B7739844DA7E914FDC894B0BBB5554E8A",
      "actual_capture_hz": 27.166666666666668,
      "video_sha256": "4AB0142DC9E36A17EDE1C93A904AAC2175E1BBE32968CBB42D645A7DA5F3A752",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-mechanism-emily-front-v57b.mp4": {
      "video": "grenade-mechanism-emily-front-v57b.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-mechanism-emily-front-v57b",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-mechanism-emily-front-v57b/result.json",
      "visual_review": "grenade-mechanism-emily-front-v57b/visual-review.json",
      "proxy_sha256": "D35D9138F74C6316D44770CDC95CCB8B7739844DA7E914FDC894B0BBB5554E8A",
      "actual_capture_hz": 27.75,
      "video_sha256": "B69A9B8ACC17DB1E9DB03BBD305DB1031AF9BE9C952F424FD064D84967E8EFA5",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-mechanism-emily-front-v57b-source.mp4": {
      "video": "grenade-mechanism-emily-front-v57b-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-mechanism-emily-front-v57b",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-mechanism-emily-front-v57b/result.json",
      "visual_review": "grenade-mechanism-emily-front-v57b/visual-review.json",
      "proxy_sha256": "D35D9138F74C6316D44770CDC95CCB8B7739844DA7E914FDC894B0BBB5554E8A",
      "actual_capture_hz": 28.0,
      "video_sha256": "B0B36A5692DCE655929C6597BAC3D4473AFDC6AD368A8082BF1A8A25EF3E9A6D",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-fx-corvo-close-v58.mp4": {
      "video": "grenade-fx-corvo-close-v58.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-fx-corvo-close-v58",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-fx-corvo-close-v58/result.json",
      "visual_review": "grenade-fx-corvo-close-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.5,
      "video_sha256": "E759DA456D30173B8D63A30345EA09F76AE041C045C85C507D68D2DA6FA8C03A",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-fx-corvo-close-v58-source.mp4": {
      "video": "grenade-fx-corvo-close-v58-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-fx-corvo-close-v58",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-fx-corvo-close-v58/result.json",
      "visual_review": "grenade-fx-corvo-close-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.25,
      "video_sha256": "FC292DBDCE021B22F997425F248865ACFF22B2D44D080DAD24A0D77954ACECEA",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-fx-emily-close-v58.mp4": {
      "video": "grenade-fx-emily-close-v58.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-fx-emily-close-v58",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-fx-emily-close-v58/result.json",
      "visual_review": "grenade-fx-emily-close-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "FA64B08373650C6A1594952D07D1419114E4334980BC5244B6401EBA2DF3219D",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-fx-emily-close-v58-source.mp4": {
      "video": "grenade-fx-emily-close-v58-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-fx-emily-close-v58",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-fx-emily-close-v58/result.json",
      "visual_review": "grenade-fx-emily-close-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.75,
      "video_sha256": "290C538C964F81C52B898B2891A3C2DF691F4E3C36859E090211632952FC01F3",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate."
    },
    "grenade-interrupt-receiver-stop-v58b.mp4": {
      "video": "grenade-interrupt-receiver-stop-v58b.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-interrupt-receiver-stop-v58b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-receiver-stop-v58b/result.json",
      "visual_review": "grenade-interrupt-receiver-stop-v58b/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.827298050139277,
      "video_sha256": "D9488F3F48894B61386A1B1D5D5F77D2731E33FEF48CED918369761A62D91ECC",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-receiver-stop-v58b-source.mp4": {
      "video": "grenade-interrupt-receiver-stop-v58b-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-interrupt-receiver-stop-v58b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-receiver-stop-v58b/result.json",
      "visual_review": "grenade-interrupt-receiver-stop-v58b/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.66016713091922,
      "video_sha256": "3C9B720E0C06D2CD7D97040C467D4BB6DB9728B32235A0005B13F3D08DD39918",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-source-pause-v58.mp4": {
      "video": "grenade-interrupt-source-pause-v58.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-interrupt-source-pause-v58",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-source-pause-v58/result.json",
      "visual_review": "grenade-interrupt-source-pause-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "625F5B8B5ADB574E51C21E97AD15AF55D45ACA8412E70D6BFA11F0B48FC922E7",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-source-pause-v58-source.mp4": {
      "video": "grenade-interrupt-source-pause-v58-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-interrupt-source-pause-v58",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-source-pause-v58/result.json",
      "visual_review": "grenade-interrupt-source-pause-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "0C6F95700BCD3695DF898A6D9D26346B7422D45B4D250D28B8CDB76D42A28452",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-receiver-pause-v58.mp4": {
      "video": "grenade-interrupt-receiver-pause-v58.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-interrupt-receiver-pause-v58",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-receiver-pause-v58/result.json",
      "visual_review": "grenade-interrupt-receiver-pause-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "5AB3383149B29EC9472E0F95A629218EFF2450E39434DF6240F91A265E205983",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-receiver-pause-v58-source.mp4": {
      "video": "grenade-interrupt-receiver-pause-v58-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-interrupt-receiver-pause-v58",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-receiver-pause-v58/result.json",
      "visual_review": "grenade-interrupt-receiver-pause-v58/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "2E6E87611336CDE0477EA33A163FD9B0959FD349715ECBF828BCF8F1DDDA0EB9",
      "checks_passed": false,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-receiver-pause-v58b.mp4": {
      "video": "grenade-interrupt-receiver-pause-v58b.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-interrupt-receiver-pause-v58b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-receiver-pause-v58b/result.json",
      "visual_review": "grenade-interrupt-receiver-pause-v58b/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "92A9640D8925344B014BE2C1D2B227D44D92EE400AE80CC954297FFABA0F01E9",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-interrupt-receiver-pause-v58b-source.mp4": {
      "video": "grenade-interrupt-receiver-pause-v58b-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-interrupt-receiver-pause-v58b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-interrupt-receiver-pause-v58b/result.json",
      "visual_review": "grenade-interrupt-receiver-pause-v58b/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "F723868C086D2C269FE9DE26BEEDC8738E5BAF80C777FA215E7BB998378C377F",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-resume-source-v59.mp4": {
      "video": "grenade-resume-source-v59.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-resume-source-v59",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-resume-source-v59/result.json",
      "visual_review": "grenade-resume-source-v59/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.57660167130919,
      "video_sha256": "7AA6965321BCD601B2DCC1B3768F6646DB5A0503FF80AA360CCF5F1E4BC4A29D",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-resume-source-v59-source.mp4": {
      "video": "grenade-resume-source-v59-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-resume-source-v59",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-resume-source-v59/result.json",
      "visual_review": "grenade-resume-source-v59/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 26.916666666666668,
      "video_sha256": "70BA37B39477B26754EAE25E945C8885484C011C36F52367CFF6F0D3B919ACEC",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-resume-receiver-emily-v59.mp4": {
      "video": "grenade-resume-receiver-emily-v59.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-resume-receiver-emily-v59",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-resume-receiver-emily-v59/result.json",
      "visual_review": "grenade-resume-receiver-emily-v59/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.75,
      "video_sha256": "1B063FB8E3CA02AF6ACCF3CC6CCD34542A1DCCFB2BB8EB584AF7863DB2179B60",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-resume-receiver-emily-v59-source.mp4": {
      "video": "grenade-resume-receiver-emily-v59-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-resume-receiver-emily-v59",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-resume-receiver-emily-v59/result.json",
      "visual_review": "grenade-resume-receiver-emily-v59/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.75,
      "video_sha256": "3B94D5E52795192054F1282C9975D47DCC60ADECC12BC6D33CDE903F1FF6E083",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-impact-state-corvo-v59.mp4": {
      "video": "grenade-impact-state-corvo-v59.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-impact-state-corvo-v59",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-impact-state-corvo-v59/result.json",
      "visual_review": "grenade-impact-state-corvo-v59/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "B0FC1D867F0D9BE9D58B3310C8BBFAE17BE9BC6C86C6B44366C92BE1D9292713",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-impact-state-corvo-v59-source.mp4": {
      "video": "grenade-impact-state-corvo-v59-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-impact-state-corvo-v59",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-impact-state-corvo-v59/result.json",
      "visual_review": "grenade-impact-state-corvo-v59/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "F6C4AB66BEF0D00CBCBDABEB38E9214D5240872D240FF0B76D074EFBDAAB6B02",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-observe-corvo-v60.mp4": {
      "video": "grenade-crouch-observe-corvo-v60.mp4",
      "character": "corvo",
      "label": "Native source observation · grenade-crouch-observe-corvo-v60",
      "alias": "grenade_crouch_observation",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-observe-corvo-v60/result.json",
      "visual_review": "grenade-crouch-observe-corvo-v60/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "D8D8DE01F98A24A626AA88C207BA677D3466C252B2EF4D463EEEA2573C669201",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "grenade-crouch-observe-emily-v62.mp4": {
      "video": "grenade-crouch-observe-emily-v62.mp4",
      "character": "emily",
      "label": "Native source observation · grenade-crouch-observe-emily-v62",
      "alias": "grenade_crouch_observation",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-observe-emily-v62/result.json",
      "visual_review": "grenade-crouch-observe-emily-v62/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "C9C6820586C01FCD9F96CF87E635BA215BA15D42C48F54FAD0854C1258340AB2",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "grenade-crouch-network-corvo-v62b.mp4": {
      "video": "grenade-crouch-network-corvo-v62b.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-crouch-network-corvo-v62b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-network-corvo-v62b/result.json",
      "visual_review": "grenade-crouch-network-corvo-v62b/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.916666666666668,
      "video_sha256": "C8510B5CB2D0CBB66D99EB5168185AC5B4018B2D7B1AF69C0DE10A139367027D",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-network-corvo-v62b-source.mp4": {
      "video": "grenade-crouch-network-corvo-v62b-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-crouch-network-corvo-v62b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-network-corvo-v62b/result.json",
      "visual_review": "grenade-crouch-network-corvo-v62b/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "292AE90FED4D747972827D6F47D3DCC6EAF88337C39BF4B368F66F562DF29339",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-moving-emily-v62b.mp4": {
      "video": "grenade-crouch-moving-emily-v62b.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-crouch-moving-emily-v62b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-moving-emily-v62b/result.json",
      "visual_review": "grenade-crouch-moving-emily-v62b/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "33B59142847B4696C698009DF33CC048C355C5F3279CD8992CCFC4CCF9BE6BC9",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-moving-emily-v62b-source.mp4": {
      "video": "grenade-crouch-moving-emily-v62b-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-crouch-moving-emily-v62b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-moving-emily-v62b/result.json",
      "visual_review": "grenade-crouch-moving-emily-v62b/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.506925207756233,
      "video_sha256": "D42A47C437268EC68278477828EDCF7CBB536C0F6E6FD1C80D86ED4E62EA242C",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-low-moving-corvo-v62b.mp4": {
      "video": "grenade-crouch-low-moving-corvo-v62b.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-crouch-low-moving-corvo-v62b",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-low-moving-corvo-v62b/result.json",
      "visual_review": "grenade-crouch-low-moving-corvo-v62b/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "7C087C42604F5DD908CE9FB95FA4646FBD756E6467643798A5ECF638C1295576",
      "checks_passed": false,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-low-moving-corvo-v62b-source.mp4": {
      "video": "grenade-crouch-low-moving-corvo-v62b-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-crouch-low-moving-corvo-v62b",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-low-moving-corvo-v62b/result.json",
      "visual_review": "grenade-crouch-low-moving-corvo-v62b/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.75,
      "video_sha256": "3F7CE01E365353163CC33B1B907CE6239497642009902567F7D542F1E1FDAE85",
      "checks_passed": false,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-low-moving-corvo-v62c.mp4": {
      "video": "grenade-crouch-low-moving-corvo-v62c.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-crouch-low-moving-corvo-v62c",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-low-moving-corvo-v62c/result.json",
      "visual_review": "grenade-crouch-low-moving-corvo-v62c/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.6536312849162,
      "video_sha256": "AA8EE49660EF7DEEBF84D9D8E9E611F128BD4053E3A24D7E52A509C0338FC5A2",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-low-moving-corvo-v62c-source.mp4": {
      "video": "grenade-crouch-low-moving-corvo-v62c-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-crouch-low-moving-corvo-v62c",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-low-moving-corvo-v62c/result.json",
      "visual_review": "grenade-crouch-low-moving-corvo-v62c/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.99442896935933,
      "video_sha256": "F08F7D0E78477B30168D3A9414867D247815012B08CEF29CF50A23E756F56E15",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-low-emily-v62c.mp4": {
      "video": "grenade-crouch-low-emily-v62c.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-crouch-low-emily-v62c",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-low-emily-v62c/result.json",
      "visual_review": "grenade-crouch-low-emily-v62c/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.5,
      "video_sha256": "D90956502C1CB8DFBA6ADB1674082502E54C7A9BAE80494FF3195D9CBDD398E7",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-crouch-low-emily-v62c-source.mp4": {
      "video": "grenade-crouch-low-emily-v62c-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-crouch-low-emily-v62c",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-crouch-low-emily-v62c/result.json",
      "visual_review": "grenade-crouch-low-emily-v62c/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.75,
      "video_sha256": "7E981F5333F63D780BF4A24C331443D86C96868ED9C8D7C9EAD34FC5A566ACF2",
      "checks_passed": true,
      "scope": "Crouched native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-air-observe-corvo-v63.mp4": {
      "video": "grenade-air-observe-corvo-v63.mp4",
      "character": "corvo",
      "label": "Native source observation · grenade-air-observe-corvo-v63",
      "alias": "grenade_air_observation",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-observe-corvo-v63/result.json",
      "visual_review": "grenade-air-observe-corvo-v63/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 26.916666666666668,
      "video_sha256": "1F4AECE573E4C5D5B4A04E34ABE1783E896C67D0A5ECD0640D6897CFBB026DC8",
      "checks_passed": true,
      "scope": "Source native one-grenade jump/cook/release observation only. Input alone does not establish airborne overlap; inspect exact state/scene rows. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "grenade-air-observe-emily-v63.mp4": {
      "video": "grenade-air-observe-emily-v63.mp4",
      "character": "emily",
      "label": "Native source observation · grenade-air-observe-emily-v63",
      "alias": "grenade_air_observation",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-observe-emily-v63/result.json",
      "visual_review": "grenade-air-observe-emily-v63/visual-review.json",
      "proxy_sha256": "CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F",
      "actual_capture_hz": 27.5,
      "video_sha256": "E86CF3E0B04C74860725B514AB07A461C3284CBA0E1882956BD0D2D569E8F471",
      "checks_passed": true,
      "scope": "Source native one-grenade jump/cook/release observation only. Input alone does not establish airborne overlap; inspect exact state/scene rows. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "grenade-air-network-corvo-v63.mp4": {
      "video": "grenade-air-network-corvo-v63.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-air-network-corvo-v63",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-network-corvo-v63/result.json",
      "visual_review": "grenade-air-network-corvo-v63/visual-review.json",
      "proxy_sha256": "C4AA318837D220E821F9FBF00FF3D744F39ED5AA922297914EEB57F0FC3CEAC7",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "B1D201B59D51F55C57901FA9B60FD26C653504B14A7D375854ED97B6CCA2C2DC",
      "checks_passed": false,
      "scope": "Airborne native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-air-network-corvo-v63-source.mp4": {
      "video": "grenade-air-network-corvo-v63-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-air-network-corvo-v63",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-network-corvo-v63/result.json",
      "visual_review": "grenade-air-network-corvo-v63/visual-review.json",
      "proxy_sha256": "C4AA318837D220E821F9FBF00FF3D744F39ED5AA922297914EEB57F0FC3CEAC7",
      "actual_capture_hz": 26.925207756232687,
      "video_sha256": "A9806AB21A1676DA0E4395F18ECDF5E2CF377A58EB8F7C4BA34A7B366466C51A",
      "checks_passed": false,
      "scope": "Airborne native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-air-network-corvo-v63b.mp4": {
      "video": "grenade-air-network-corvo-v63b.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-air-network-corvo-v63b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-network-corvo-v63b/result.json",
      "visual_review": "grenade-air-network-corvo-v63b/visual-review.json",
      "proxy_sha256": "28F1DB269BD455B85D6CA86C34F13DBD007C3EFDF7DCFB6F468911C10E6FC0AF",
      "actual_capture_hz": 28.166666666666668,
      "video_sha256": "3319FE62E28EDE8DCD12489A43C20217C2D5108C728E362E9ABCD5E9C490B8F7",
      "checks_passed": true,
      "scope": "Airborne native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-air-network-corvo-v63b-source.mp4": {
      "video": "grenade-air-network-corvo-v63b-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-air-network-corvo-v63b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-network-corvo-v63b/result.json",
      "visual_review": "grenade-air-network-corvo-v63b/visual-review.json",
      "proxy_sha256": "28F1DB269BD455B85D6CA86C34F13DBD007C3EFDF7DCFB6F468911C10E6FC0AF",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "E80A7B9DCD76F0DF01A9C46A1C80CE05D30EC0DC76964C1495BC29BFC4C82FA6",
      "checks_passed": true,
      "scope": "Airborne native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-air-network-emily-v63b.mp4": {
      "video": "grenade-air-network-emily-v63b.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-air-network-emily-v63b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-network-emily-v63b/result.json",
      "visual_review": "grenade-air-network-emily-v63b/visual-review.json",
      "proxy_sha256": "28F1DB269BD455B85D6CA86C34F13DBD007C3EFDF7DCFB6F468911C10E6FC0AF",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "AAFA29235FD82BD5D635ADA2F5A8F36B65ADAAFE93D521F4BA3D213810A4A6AF",
      "checks_passed": true,
      "scope": "Airborne native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-air-network-emily-v63b-source.mp4": {
      "video": "grenade-air-network-emily-v63b-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-air-network-emily-v63b",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-air-network-emily-v63b/result.json",
      "visual_review": "grenade-air-network-emily-v63b/visual-review.json",
      "proxy_sha256": "28F1DB269BD455B85D6CA86C34F13DBD007C3EFDF7DCFB6F468911C10E6FC0AF",
      "actual_capture_hz": 27.083333333333332,
      "video_sha256": "372300D8979B1CE7F19553CD831F9CC3F853490CBD1428F33549962EC247FCF3",
      "checks_passed": true,
      "scope": "Airborne native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-reader-host-v320.mp4": {
      "video": "grenade-reader-host-v320.mp4",
      "character": "corvo",
      "label": "Grenade native network receiver · grenade-reader-host-v320",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-reader-host-v320/result.json",
      "visual_review": "grenade-reader-host-v320/visual-review.json",
      "proxy_sha256": "B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50",
      "actual_capture_hz": 24.333333333333332,
      "video_sha256": "AF59CC5638394CE5772F08882EE55F8BDFE72AD2257C17E18B8FDB79525A6B6C",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-reader-host-v320-source.mp4": {
      "video": "grenade-reader-host-v320-source.mp4",
      "character": "corvo",
      "label": "Grenade native network source · grenade-reader-host-v320",
      "alias": "grenade_throw_high",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-reader-host-v320/result.json",
      "visual_review": "grenade-reader-host-v320/visual-review.json",
      "proxy_sha256": "B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50",
      "actual_capture_hz": 26.0,
      "video_sha256": "554164F0740F2A9971887E4532458930C31A689F875F4C677663375FDA6E9AC8",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-reader-emily-v320.mp4": {
      "video": "grenade-reader-emily-v320.mp4",
      "character": "emily",
      "label": "Grenade native network receiver · grenade-reader-emily-v320",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-reader-emily-v320/result.json",
      "visual_review": "grenade-reader-emily-v320/visual-review.json",
      "proxy_sha256": "B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50",
      "actual_capture_hz": 27.150837988826815,
      "video_sha256": "D5CE9CB7C0DA6E0CEBE0483DBAF5DC8CFC56A6E2BF6B8719CA655C635D65F846",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "grenade-reader-emily-v320-source.mp4": {
      "video": "grenade-reader-emily-v320-source.mp4",
      "character": "emily",
      "label": "Grenade native network source · grenade-reader-emily-v320",
      "alias": "grenade_throw_low",
      "requirements": [
        "grenade_equip_aim_throw"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-reader-emily-v320/result.json",
      "visual_review": "grenade-reader-emily-v320/visual-review.json",
      "proxy_sha256": "B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50",
      "actual_capture_hz": 24.916666666666668,
      "video_sha256": "82D199D01195A691A107F890F7A01FD28D3728C01F4670392234E9294EADF6E1",
      "checks_passed": true,
      "scope": "Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json",
    "coverage-v1330e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1338-ranged-items.json",
    "decision": "complete",
    "scope": "Cooking (left trigger held) and throwing a grenade in both roles; the partner presents grenade variants 0 and 3-6 (hold, wind-up, release, recovery).",
    "takes": [
      {
        "take": "coverage-v1330e",
        "cases": [
          "host-grenade",
          "client-grenade"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330e/client-grenade-observer.mp4",
        "verdict": "Emily draws her arm back over her shoulder with the grenade, steps into the throw with her coat swinging, and recovers to the armed stance."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [in_game_recordings: native-grenade-actions-corvo-high-v54.mp4](<../../../../work/natural-moveset/native-grenade-actions-corvo-high-v54.mp4>)
- [in_game_recordings: native-grenade-actions-corvo-low-v54.mp4](<../../../../work/natural-moveset/native-grenade-actions-corvo-low-v54.mp4>)
- [in_game_recordings: native-grenade-actions-emily-high-v54.mp4](<../../../../work/natural-moveset/native-grenade-actions-emily-high-v54.mp4>)
- [in_game_recordings: native-grenade-equip-corvo-v52-v2.mp4](<../../../../work/natural-moveset/native-grenade-equip-corvo-v52-v2.mp4>)
- [in_game_recordings: native-grenade-equip-corvo-v52-v3.mp4](<../../../../work/natural-moveset/native-grenade-equip-corvo-v52-v3.mp4>)
- [in_game_recordings: native-grenade-equip-emily-v52.mp4](<../../../../work/natural-moveset/native-grenade-equip-emily-v52.mp4>)
- [in_game_recordings: native-grenade-reader-corvo-high-v54.mp4](<../../../../work/natural-moveset/native-grenade-reader-corvo-high-v54.mp4>)
- [in_game_recordings: native-grenade-reader-emily-low-v54.mp4](<../../../../work/natural-moveset/native-grenade-reader-emily-low-v54.mp4>)
- [in_game_recordings: native-grenade-scene-corvo-v53.mp4](<../../../../work/natural-moveset/native-grenade-scene-corvo-v53.mp4>)
- [in_game_recordings: native-grenade-scene-emily-v53.mp4](<../../../../work/natural-moveset/native-grenade-scene-emily-v53.mp4>)
- [in_game_recordings: native-grenade-scene-emily-v53-v2.mp4](<../../../../work/natural-moveset/native-grenade-scene-emily-v53-v2.mp4>)
- [in_game_recordings: native-grenade-scene-emily-v53-v3.mp4](<../../../../work/natural-moveset/native-grenade-scene-emily-v53-v3.mp4>)
- [in_game_recordings: grenade-network-corvo-high-v56h.mp4](<../../../../work/natural-moveset/grenade-network-corvo-high-v56h.mp4>)
- [in_game_recordings: grenade-network-corvo-high-v56h-source.mp4](<../../../../work/natural-moveset/grenade-network-corvo-high-v56h-source.mp4>)
- [in_game_recordings: grenade-prop-corvo-idle-v55.mp4](<../../../../work/natural-moveset/grenade-prop-corvo-idle-v55.mp4>)
- [in_game_recordings: grenade-prop-corvo-close-v55b.mp4](<../../../../work/natural-moveset/grenade-prop-corvo-close-v55b.mp4>)
- [in_game_recordings: grenade-prop-emily-close-v55b.mp4](<../../../../work/natural-moveset/grenade-prop-emily-close-v55b.mp4>)
- [in_game_recordings: grenade-network-corvo-high-v56i.mp4](<../../../../work/natural-moveset/grenade-network-corvo-high-v56i.mp4>)
- [in_game_recordings: grenade-network-corvo-high-v56i-source.mp4](<../../../../work/natural-moveset/grenade-network-corvo-high-v56i-source.mp4>)
- [in_game_recordings: grenade-network-corvo-low-v56i.mp4](<../../../../work/natural-moveset/grenade-network-corvo-low-v56i.mp4>)
- [in_game_recordings: grenade-network-corvo-low-v56i-source.mp4](<../../../../work/natural-moveset/grenade-network-corvo-low-v56i-source.mp4>)
- [in_game_recordings: grenade-network-corvo-low-v56i2.mp4](<../../../../work/natural-moveset/grenade-network-corvo-low-v56i2.mp4>)
- [in_game_recordings: grenade-network-corvo-low-v56i2-source.mp4](<../../../../work/natural-moveset/grenade-network-corvo-low-v56i2-source.mp4>)
- [in_game_recordings: grenade-network-emily-high-v56i.mp4](<../../../../work/natural-moveset/grenade-network-emily-high-v56i.mp4>)
- [in_game_recordings: grenade-network-emily-high-v56i-source.mp4](<../../../../work/natural-moveset/grenade-network-emily-high-v56i-source.mp4>)
- [in_game_recordings: grenade-network-emily-low-v56i.mp4](<../../../../work/natural-moveset/grenade-network-emily-low-v56i.mp4>)
- [in_game_recordings: grenade-network-emily-low-v56i-source.mp4](<../../../../work/natural-moveset/grenade-network-emily-low-v56i-source.mp4>)
- [in_game_recordings: grenade-network-emily-low-v56i2.mp4](<../../../../work/natural-moveset/grenade-network-emily-low-v56i2.mp4>)
- [in_game_recordings: grenade-network-emily-low-v56i2-source.mp4](<../../../../work/natural-moveset/grenade-network-emily-low-v56i2-source.mp4>)
- [in_game_recordings: grenade-mechanism-corvo-front-v57b.mp4](<../../../../work/natural-moveset/grenade-mechanism-corvo-front-v57b.mp4>)
- [in_game_recordings: grenade-mechanism-corvo-front-v57b-source.mp4](<../../../../work/natural-moveset/grenade-mechanism-corvo-front-v57b-source.mp4>)
- [in_game_recordings: grenade-mechanism-emily-front-v57b.mp4](<../../../../work/natural-moveset/grenade-mechanism-emily-front-v57b.mp4>)
- [in_game_recordings: grenade-mechanism-emily-front-v57b-source.mp4](<../../../../work/natural-moveset/grenade-mechanism-emily-front-v57b-source.mp4>)
- [in_game_recordings: grenade-fx-corvo-close-v58.mp4](<../../../../work/natural-moveset/grenade-fx-corvo-close-v58.mp4>)
- [in_game_recordings: grenade-fx-corvo-close-v58-source.mp4](<../../../../work/natural-moveset/grenade-fx-corvo-close-v58-source.mp4>)
- [in_game_recordings: grenade-fx-emily-close-v58.mp4](<../../../../work/natural-moveset/grenade-fx-emily-close-v58.mp4>)
- [in_game_recordings: grenade-fx-emily-close-v58-source.mp4](<../../../../work/natural-moveset/grenade-fx-emily-close-v58-source.mp4>)
- [in_game_recordings: grenade-interrupt-receiver-stop-v58b.mp4](<../../../../work/natural-moveset/grenade-interrupt-receiver-stop-v58b.mp4>)
- [in_game_recordings: grenade-interrupt-receiver-stop-v58b-source.mp4](<../../../../work/natural-moveset/grenade-interrupt-receiver-stop-v58b-source.mp4>)
- [in_game_recordings: grenade-interrupt-source-pause-v58.mp4](<../../../../work/natural-moveset/grenade-interrupt-source-pause-v58.mp4>)
- [in_game_recordings: grenade-interrupt-source-pause-v58-source.mp4](<../../../../work/natural-moveset/grenade-interrupt-source-pause-v58-source.mp4>)
- [in_game_recordings: grenade-interrupt-receiver-pause-v58.mp4](<../../../../work/natural-moveset/grenade-interrupt-receiver-pause-v58.mp4>)
- [in_game_recordings: grenade-interrupt-receiver-pause-v58-source.mp4](<../../../../work/natural-moveset/grenade-interrupt-receiver-pause-v58-source.mp4>)
- [in_game_recordings: grenade-interrupt-receiver-pause-v58b.mp4](<../../../../work/natural-moveset/grenade-interrupt-receiver-pause-v58b.mp4>)
- [in_game_recordings: grenade-interrupt-receiver-pause-v58b-source.mp4](<../../../../work/natural-moveset/grenade-interrupt-receiver-pause-v58b-source.mp4>)
- [in_game_recordings: grenade-resume-source-v59.mp4](<../../../../work/natural-moveset/grenade-resume-source-v59.mp4>)
- [in_game_recordings: grenade-resume-source-v59-source.mp4](<../../../../work/natural-moveset/grenade-resume-source-v59-source.mp4>)
- [in_game_recordings: grenade-resume-receiver-emily-v59.mp4](<../../../../work/natural-moveset/grenade-resume-receiver-emily-v59.mp4>)
- [in_game_recordings: grenade-resume-receiver-emily-v59-source.mp4](<../../../../work/natural-moveset/grenade-resume-receiver-emily-v59-source.mp4>)
- [in_game_recordings: grenade-impact-state-corvo-v59.mp4](<../../../../work/natural-moveset/grenade-impact-state-corvo-v59.mp4>)
- [in_game_recordings: grenade-impact-state-corvo-v59-source.mp4](<../../../../work/natural-moveset/grenade-impact-state-corvo-v59-source.mp4>)
- [in_game_recordings: grenade-crouch-observe-corvo-v60.mp4](<../../../../work/natural-moveset/grenade-crouch-observe-corvo-v60.mp4>)
- [in_game_recordings: grenade-crouch-observe-emily-v62.mp4](<../../../../work/natural-moveset/grenade-crouch-observe-emily-v62.mp4>)
- [in_game_recordings: grenade-crouch-network-corvo-v62b.mp4](<../../../../work/natural-moveset/grenade-crouch-network-corvo-v62b.mp4>)
- [in_game_recordings: grenade-crouch-network-corvo-v62b-source.mp4](<../../../../work/natural-moveset/grenade-crouch-network-corvo-v62b-source.mp4>)
- [in_game_recordings: grenade-crouch-moving-emily-v62b.mp4](<../../../../work/natural-moveset/grenade-crouch-moving-emily-v62b.mp4>)
- [in_game_recordings: grenade-crouch-moving-emily-v62b-source.mp4](<../../../../work/natural-moveset/grenade-crouch-moving-emily-v62b-source.mp4>)
- [in_game_recordings: grenade-crouch-low-moving-corvo-v62b.mp4](<../../../../work/natural-moveset/grenade-crouch-low-moving-corvo-v62b.mp4>)
- [in_game_recordings: grenade-crouch-low-moving-corvo-v62b-source.mp4](<../../../../work/natural-moveset/grenade-crouch-low-moving-corvo-v62b-source.mp4>)
- [in_game_recordings: grenade-crouch-low-moving-corvo-v62c.mp4](<../../../../work/natural-moveset/grenade-crouch-low-moving-corvo-v62c.mp4>)
- [in_game_recordings: grenade-crouch-low-moving-corvo-v62c-source.mp4](<../../../../work/natural-moveset/grenade-crouch-low-moving-corvo-v62c-source.mp4>)
- [in_game_recordings: grenade-crouch-low-emily-v62c.mp4](<../../../../work/natural-moveset/grenade-crouch-low-emily-v62c.mp4>)
- [in_game_recordings: grenade-crouch-low-emily-v62c-source.mp4](<../../../../work/natural-moveset/grenade-crouch-low-emily-v62c-source.mp4>)
- [in_game_recordings: grenade-air-observe-corvo-v63.mp4](<../../../../work/natural-moveset/grenade-air-observe-corvo-v63.mp4>)
- [in_game_recordings: grenade-air-observe-emily-v63.mp4](<../../../../work/natural-moveset/grenade-air-observe-emily-v63.mp4>)
- [in_game_recordings: grenade-air-network-corvo-v63.mp4](<../../../../work/natural-moveset/grenade-air-network-corvo-v63.mp4>)
- [in_game_recordings: grenade-air-network-corvo-v63-source.mp4](<../../../../work/natural-moveset/grenade-air-network-corvo-v63-source.mp4>)
- [in_game_recordings: grenade-air-network-corvo-v63b.mp4](<../../../../work/natural-moveset/grenade-air-network-corvo-v63b.mp4>)
- [in_game_recordings: grenade-air-network-corvo-v63b-source.mp4](<../../../../work/natural-moveset/grenade-air-network-corvo-v63b-source.mp4>)
- [in_game_recordings: grenade-air-network-emily-v63b.mp4](<../../../../work/natural-moveset/grenade-air-network-emily-v63b.mp4>)
- [in_game_recordings: grenade-air-network-emily-v63b-source.mp4](<../../../../work/natural-moveset/grenade-air-network-emily-v63b-source.mp4>)
- [in_game_recordings: grenade-reader-host-v320.mp4](<../../../../work/natural-moveset/grenade-reader-host-v320.mp4>)
- [in_game_recordings: grenade-reader-host-v320-source.mp4](<../../../../work/natural-moveset/grenade-reader-host-v320-source.mp4>)
- [in_game_recordings: grenade-reader-emily-v320.mp4](<../../../../work/natural-moveset/grenade-reader-emily-v320.mp4>)
- [in_game_recordings: grenade-reader-emily-v320-source.mp4](<../../../../work/natural-moveset/grenade-reader-emily-v320-source.mp4>)
- [in_game_recordings: grenade-network-host-v1266a.mp4](<../../../../work/natural-moveset/grenade-network-host-v1266a.mp4>)
- [in_game_recordings: grenade-network-client-v1266a.mp4](<../../../../work/natural-moveset/grenade-network-client-v1266a.mp4>)
## mine_or_razor_place

Visual sweep 1260: accepted (corvo accept, emily accept). The trap equip, ready, crouched place, empty or reload, and holster read correctly for both partners; contact with the floor is approximate. 575 with Emily576 and Corvo578: Corvo spring razor and Emily stun mine each pass at least15 native Steam presentation checks. Shared observed equip500, holster500, ready-in200/loop666, place300 and empty333 or observed reload866 clocks drive existing authored bank62/348; selected idle and static native wrist props included. Stock consumption hides remote held gadget. Both checkpoints and14 protected hashes restored. Failed570 clock-offset interpretation corrected from probe definition; Emily572 observed native reload866 instead of expected empty333, now represented explicitly; both observer sheets viewed, fine polish deferred. Corvo576 failed its ready-in body-weight check: phase3 arrived, but a250ms semantic freshness gap briefly faded overall body weight. Unchanged Corvo578 passed all15 checks. This intermittent continuity issue remains unresolved; no timing guards or assertions were relaxed. Visual polish remains unaccepted: fine grip, precise surface reach/contact, weapon clearance and continuous quality. Native source places one actual trap; remote body/held prop never spawns a trap or runs a native placement/damage event. Shared placed-trap worlds/outcomes remain separate. Additional stocked reload contexts, ready-out cancellation, slide, airborne, upgraded items and both gadget kinds on each role still need coverage. Holstering is inferred only with a unique acquired trap when neither is selected; ambiguous inventories fall back instead of guessing. Complete moveset and playable co-op remain unfinished; prioritize broad existing-pipeline bindings and defer polish. Corvo576 failed its ready-in body-weight check: phase3 arrived, but a250ms semantic freshness gap briefly faded overall body weight. Unchanged Corvo578 passed all15 checks. This intermittent continuity issue remains unresolved; no timing guards or assertions were relaxed.

````json
{
  "id": "mine_or_razor_place",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_trap_body_and_held_prop",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "mine or razor place",
  "motions": [
    "stunmine_equip",
    "stunmine_idle",
    "stunmine_ready_in",
    "stunmine_ready_loop",
    "stunmine_place",
    "stunmine_reload",
    "stunmine_ready_out",
    "stunmine_ooa",
    "stunmine_unequip",
    "stunmine_sequence",
    "stunmine_equip_cancel_30",
    "stunmine_equip_cancel_70",
    "springrazor_sequence",
    "stunmine_wall_place_sequence",
    "springrazor_wall_place_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The trap equip, ready, crouched place, empty or reload, and holster read correctly for both partners; contact with the floor is approximate. 575 with Emily576 and Corvo578: Corvo spring razor and Emily stun mine each pass at least15 native Steam presentation checks. Shared observed equip500, holster500, ready-in200/loop666, place300 and empty333 or observed reload866 clocks drive existing authored bank62/348; selected idle and static native wrist props included. Stock consumption hides remote held gadget. Both checkpoints and14 protected hashes restored. Failed570 clock-offset interpretation corrected from probe definition; Emily572 observed native reload866 instead of expected empty333, now represented explicitly; both observer sheets viewed, fine polish deferred. Corvo576 failed its ready-in body-weight check: phase3 arrived, but a250ms semantic freshness gap briefly faded overall body weight. Unchanged Corvo578 passed all15 checks. This intermittent continuity issue remains unresolved; no timing guards or assertions were relaxed. Visual polish remains unaccepted: fine grip, precise surface reach/contact, weapon clearance and continuous quality. Native source places one actual trap; remote body/held prop never spawns a trap or runs a native placement/damage event. Shared placed-trap worlds/outcomes remain separate. Additional stocked reload contexts, ready-out cancellation, slide, airborne, upgraded items and both gadget kinds on each role still need coverage. Holstering is inferred only with a unique acquired trap when neither is selected; ambiguous inventories fall back instead of guessing. Complete moveset and playable co-op remain unfinished; prioritize broad existing-pipeline bindings and defer polish. Corvo576 failed its ready-in body-weight check: phase3 arrived, but a250ms semantic freshness gap briefly faded overall body weight. Unchanged Corvo578 passed all15 checks. This intermittent continuity issue remains unresolved; no timing guards or assertions were relaxed.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-trap-corvo-razor-v568/source.mp4",
    "native-trap-emily-stun-v568/source.mp4",
    "trap-network-corvo-razor-v570/source.mp4",
    "trap-network-corvo-razor-v570/observer.mp4",
    "trap-network-corvo-razor-v572/source.mp4",
    "trap-network-corvo-razor-v572/observer.mp4",
    "trap-network-emily-stun-v572/source.mp4",
    "trap-network-emily-stun-v572/observer.mp4",
    "trap-network-corvo-razor-v576/source.mp4",
    "trap-network-corvo-razor-v576/observer.mp4",
    "trap-network-emily-stun-v576/source.mp4",
    "trap-network-emily-stun-v576/observer.mp4",
    "trap-network-corvo-razor-v578/source.mp4",
    "trap-network-corvo-razor-v578/observer.mp4"
  ],
  "complete": true,
  "visual_accepted": false,
  "evidence": [
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "coverage-v1330e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1338-ranged-items.json",
    "decision": "complete",
    "scope": "Placing a stun mine and a spring razor on the floor in both roles; the partner presents trap variants 0, 4, 5 and 7 (hold, bend, place, recovery).",
    "takes": [
      {
        "take": "coverage-v1330e",
        "cases": [
          "host-stunmine",
          "host-springrazor",
          "client-stunmine",
          "client-springrazor"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330e/host-stunmine-observer.mp4",
        "verdict": "Corvo looks down, bends and reaches toward the floor to set the mine, then straightens."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [in_game_recordings: native-trap-corvo-razor-v568/source.mp4](<../../../../work/natural-moveset/native-trap-corvo-razor-v568/source.mp4>)
- [in_game_recordings: native-trap-emily-stun-v568/source.mp4](<../../../../work/natural-moveset/native-trap-emily-stun-v568/source.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v570/source.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v570/source.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v570/observer.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v570/observer.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v572/source.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v572/source.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v572/observer.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v572/observer.mp4>)
- [in_game_recordings: trap-network-emily-stun-v572/source.mp4](<../../../../work/natural-moveset/trap-network-emily-stun-v572/source.mp4>)
- [in_game_recordings: trap-network-emily-stun-v572/observer.mp4](<../../../../work/natural-moveset/trap-network-emily-stun-v572/observer.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v576/source.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v576/source.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v576/observer.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v576/observer.mp4>)
- [in_game_recordings: trap-network-emily-stun-v576/source.mp4](<../../../../work/natural-moveset/trap-network-emily-stun-v576/source.mp4>)
- [in_game_recordings: trap-network-emily-stun-v576/observer.mp4](<../../../../work/natural-moveset/trap-network-emily-stun-v576/observer.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v578/source.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v578/source.mp4>)
- [in_game_recordings: trap-network-corvo-razor-v578/observer.mp4](<../../../../work/natural-moveset/trap-network-corvo-razor-v578/observer.mp4>)
## heart_equip_use_holster

Visual sweep 1260: accepted (corvo accept, emily accept). The Heart equip, use, crouched hold and holster read naturally for both partners with the held prop. 564/565: Corvo and Emily each pass11 native Steam Heart body/prop checks; exact equip533/use1000/holster333 scenes plus selected idle, existing crouch composition, hide lifecycle and restored checkpoints. Bank60/340 appends four existing clips, prior336 payloads unchanged. Both actual observer sheets viewed; fine polish and internal Heart mechanism remain deferred. Fine grip/contact and continuous visual quality unaccepted; temporal sheets only. Crouch composes the existing lower-body locomotion with authored Heart posture; no new crouch authoring. Heart internal mechanism/heartbeat, secret-target reactions, dialogue and discovery outcomes are not synchronized. Airborne, sword-combat combinations, sprint/slide and interruptions beyond tested stream stop remain unverified. Trap animations still lack runtime binding. Complete moveset and playable co-op remain unfinished.

````json
{
  "id": "heart_equip_use_holster",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_ground_heart_body_prop",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "heart equip use holster",
  "motions": [
    "heart_equip",
    "heart_idle",
    "heart_use",
    "heart_unequip",
    "heart_sequence",
    "heart_equip_cancel_30",
    "heart_equip_cancel_70"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The Heart equip, use, crouched hold and holster read naturally for both partners with the held prop. 564/565: Corvo and Emily each pass11 native Steam Heart body/prop checks; exact equip533/use1000/holster333 scenes plus selected idle, existing crouch composition, hide lifecycle and restored checkpoints. Bank60/340 appends four existing clips, prior336 payloads unchanged. Both actual observer sheets viewed; fine polish and internal Heart mechanism remain deferred. Fine grip/contact and continuous visual quality unaccepted; temporal sheets only. Crouch composes the existing lower-body locomotion with authored Heart posture; no new crouch authoring. Heart internal mechanism/heartbeat, secret-target reactions, dialogue and discovery outcomes are not synchronized. Airborne, sword-combat combinations, sprint/slide and interruptions beyond tested stream stop remain unverified. Trap animations still lack runtime binding. Complete moveset and playable co-op remain unfinished.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-heart-corvo-v563/source.mp4",
    "heart-network-corvo-v565/source.mp4",
    "heart-network-corvo-v565/observer.mp4",
    "native-heart-emily-v563/source.mp4",
    "heart-network-emily-v565/source.mp4",
    "heart-network-emily-v565/observer.mp4"
  ],
  "complete": true,
  "visual_accepted": false,
  "evidence": [
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "coverage-v1330e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1338-ranged-items.json",
    "decision": "complete",
    "scope": "Raising the Heart (left trigger held) and lowering it in both roles; the partner presents Heart variants 0 and 3.",
    "takes": [
      {
        "take": "coverage-v1330e",
        "cases": [
          "host-heart",
          "client-heart"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330e/host-heart-observer.mp4",
        "verdict": "Corvo lifts the Heart in his left hand in front of his chest, holds it there, and lowers it again."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [in_game_recordings: native-heart-corvo-v563/source.mp4](<../../../../work/natural-moveset/native-heart-corvo-v563/source.mp4>)
- [in_game_recordings: heart-network-corvo-v565/source.mp4](<../../../../work/natural-moveset/heart-network-corvo-v565/source.mp4>)
- [in_game_recordings: heart-network-corvo-v565/observer.mp4](<../../../../work/natural-moveset/heart-network-corvo-v565/observer.mp4>)
- [in_game_recordings: native-heart-emily-v563/source.mp4](<../../../../work/natural-moveset/native-heart-emily-v563/source.mp4>)
- [in_game_recordings: heart-network-emily-v565/source.mp4](<../../../../work/natural-moveset/heart-network-emily-v565/source.mp4>)
- [in_game_recordings: heart-network-emily-v565/observer.mp4](<../../../../work/natural-moveset/heart-network-emily-v565/observer.mp4>)
## health_elixir

Audit 1264 (1214j): a consumed health elixir (native consume, quantity drop) presents a hand-to-mouth reach about 1 s later in both roles (1263a). Visual sweep 1260: reject (corvo reject, emily reject). Walking and stopping stay continuous, but neither partner shows any drinking gesture, so the elixir action is not represented (the authored drinking clip is still unused). Native source consumption observed with inventory/health and checkpoint restoration checks. Recorded Steam receiver walking/stopping remains continuous through native consumption. Native callback and wheel use show no FP drinking gesture in these recordings. The queued native event is a drink sound, not a body scene. Authored drinking clip remains unused; remote drink sound and gameplay health synchronization remain unimplemented. Fine visual quality and untested actions are not accepted.

````json
{
  "id": "health_elixir",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_health_elixir_drink_reach_presented_both_roles",
  "visual_validation": "accepted_functional_both_roles_hand_to_mouth",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "health elixir",
  "motions": [
    "empty_idlebreak_drink_flask"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1264 (1214j): a consumed health elixir (native consume, quantity drop) presents a hand-to-mouth reach about 1 s later in both roles (1263a). Visual sweep 1260: reject (corvo reject, emily reject). Walking and stopping stay continuous, but neither partner shows any drinking gesture, so the elixir action is not represented (the authored drinking clip is still unused). Native source consumption observed with inventory/health and checkpoint restoration checks. Recorded Steam receiver walking/stopping remains continuous through native consumption. Native callback and wheel use show no FP drinking gesture in these recordings. The queued native event is a drink sound, not a body scene. Authored drinking clip remains unused; remote drink sound and gameplay health synchronization remain unimplemented. Fine visual quality and untested actions are not accepted.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "potion-wheel-corvo-v64b.mp4",
    "potion-wheel-corvo-v64c.mp4",
    "potion-health-corvo-v64d.mp4",
    "potion-health-emily-v64d.mp4",
    "potion-gameplay-corvo-v65.mp4",
    "potion-gameplay-emily-v65.mp4",
    "potion-movement-corvo-v65.mp4",
    "potion-movement-corvo-v65-receiver.mp4",
    "potion-movement-emily-v65.mp4",
    "potion-movement-emily-v65-receiver.mp4",
    "potion-statistics-corvo-v66.mp4",
    "potion-statistics-corvo-v66-receiver.mp4",
    "potion-wheel-statistics-emily-v66.mp4",
    "potion-wheel-control-corvo-v66.mp4",
    "native-potion-drink-host-v1263a-receiver.mp4",
    "native-potion-drink-client-v1263a-receiver.mp4"
  ],
  "native_integration": {
    "potion-wheel-corvo-v64b.mp4": {
      "video": "potion-wheel-corvo-v64b.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-wheel-corvo-v64b",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-wheel-corvo-v64b/result.json",
      "visual_review": "potion-wheel-corvo-v64b/visual-review.json",
      "proxy_sha256": "0570A9AD817651C0C10CEDD389EF75EE42EB80073D6F1D0536DE0A9DDC0206C9",
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "F7B8E81756E76C0A66DC92D049F7FEC657CC3804423463410FB52BF1259D22CD",
      "checks_passed": false,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-wheel-corvo-v64c.mp4": {
      "video": "potion-wheel-corvo-v64c.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-wheel-corvo-v64c",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-wheel-corvo-v64c/result.json",
      "visual_review": "potion-wheel-corvo-v64c/visual-review.json",
      "proxy_sha256": "0570A9AD817651C0C10CEDD389EF75EE42EB80073D6F1D0536DE0A9DDC0206C9",
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "9BD770E9FC89874B4B53B142974FDBA7E596E12D37BD8DB7C0F71093EA811FA0",
      "checks_passed": false,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-health-corvo-v64d.mp4": {
      "video": "potion-health-corvo-v64d.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-health-corvo-v64d",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-health-corvo-v64d/result.json",
      "visual_review": "potion-health-corvo-v64d/visual-review.json",
      "proxy_sha256": "0570A9AD817651C0C10CEDD389EF75EE42EB80073D6F1D0536DE0A9DDC0206C9",
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "3D5998BA4F320796372F06119734B361CA4B3D2EC8E43CC7221A5138111AF748",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-health-emily-v64d.mp4": {
      "video": "potion-health-emily-v64d.mp4",
      "character": "emily",
      "label": "Native elixir source observation · potion-health-emily-v64d",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-health-emily-v64d/result.json",
      "visual_review": "potion-health-emily-v64d/visual-review.json",
      "proxy_sha256": "0570A9AD817651C0C10CEDD389EF75EE42EB80073D6F1D0536DE0A9DDC0206C9",
      "actual_capture_hz": 27.333333333333332,
      "video_sha256": "04B4CC0A41108EDF3F1B13AC87CB9CDC0928A53B078D60D560B891C25B19DF9E",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-gameplay-corvo-v65.mp4": {
      "video": "potion-gameplay-corvo-v65.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-gameplay-corvo-v65",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-gameplay-corvo-v65/result.json",
      "visual_review": "potion-gameplay-corvo-v65/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.77777777777778,
      "video_sha256": "2F19DEC643207E446C3C8B446C066EA605C69A1ED737C491E65CFF51554C68A7",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-gameplay-emily-v65.mp4": {
      "video": "potion-gameplay-emily-v65.mp4",
      "character": "emily",
      "label": "Native elixir source observation · potion-gameplay-emily-v65",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-gameplay-emily-v65/result.json",
      "visual_review": "potion-gameplay-emily-v65/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "90CB931507E218F93146B87D2FAB7B028D81945EF82CE2D665D7B1CAA5FA781B",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-movement-corvo-v65.mp4": {
      "video": "potion-movement-corvo-v65.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-movement-corvo-v65",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-movement-corvo-v65/result.json",
      "visual_review": "potion-movement-corvo-v65/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.444444444444443,
      "video_sha256": "22821918AD6B15B2AAAD7476F1C9404599574E5DB3955698E4C3CC692933917C",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native health consumption, over Steam. No dedicated drink gesture, remote drink audio or remote health synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-movement-corvo-v65-receiver.mp4": {
      "video": "potion-movement-corvo-v65-receiver.mp4",
      "character": "corvo",
      "label": "Remote movement during native health use · potion-movement-corvo-v65",
      "alias": "health_use_movement_continuity",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-movement-corvo-v65/result.json",
      "visual_review": "potion-movement-corvo-v65/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.77777777777778,
      "video_sha256": "4911003CE0356FF4E5509BB5CBB619554B55E8490DACF4C3531236975CCB8DA6",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native health consumption, over Steam. No dedicated drink gesture, remote drink audio or remote health synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-movement-emily-v65.mp4": {
      "video": "potion-movement-emily-v65.mp4",
      "character": "emily",
      "label": "Native elixir source observation · potion-movement-emily-v65",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-movement-emily-v65/result.json",
      "visual_review": "potion-movement-emily-v65/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.992565055762082,
      "video_sha256": "D81AB9D45076B72C3B68C03283FAC6B1651964DF9AC4515749EFF24E11A57702",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native health consumption, over Steam. No dedicated drink gesture, remote drink audio or remote health synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-movement-emily-v65-receiver.mp4": {
      "video": "potion-movement-emily-v65-receiver.mp4",
      "character": "emily",
      "label": "Remote movement during native health use · potion-movement-emily-v65",
      "alias": "health_use_movement_continuity",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-movement-emily-v65/result.json",
      "visual_review": "potion-movement-emily-v65/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 28.104089219330856,
      "video_sha256": "611E4E5D10DA85BC35237F7E56DC29A1C0CC25D37704772595D631EE67D465BA",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native health consumption, over Steam. No dedicated drink gesture, remote drink audio or remote health synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-statistics-corvo-v66.mp4": {
      "video": "potion-statistics-corvo-v66.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-statistics-corvo-v66",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-statistics-corvo-v66/result.json",
      "visual_review": "potion-statistics-corvo-v66/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 26.98884758364312,
      "video_sha256": "E5ACC0CBBD4DD0D0793F04795D69B3DEFE50B8A79025FACA3A6E2B8F7B6331FE",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native health consumption, over Steam. No dedicated drink gesture, remote drink audio or remote health synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-statistics-corvo-v66-receiver.mp4": {
      "video": "potion-statistics-corvo-v66-receiver.mp4",
      "character": "corvo",
      "label": "Remote movement during native health use · potion-statistics-corvo-v66",
      "alias": "health_use_movement_continuity",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-statistics-corvo-v66/result.json",
      "visual_review": "potion-statistics-corvo-v66/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 26.20817843866171,
      "video_sha256": "C488F6532B3C7D8C83C979B9320A3F0BDC98310ABBF3D69D14ADB82727123A53",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native health consumption, over Steam. No dedicated drink gesture, remote drink audio or remote health synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-wheel-statistics-emily-v66.mp4": {
      "video": "potion-wheel-statistics-emily-v66.mp4",
      "character": "emily",
      "label": "Native elixir source observation · potion-wheel-statistics-emily-v66",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-wheel-statistics-emily-v66/result.json",
      "visual_review": "potion-wheel-statistics-emily-v66/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.444444444444443,
      "video_sha256": "BD8766CFC7B69DE341F808108D0D7235E342F4E6D5CB4FDE32E8C2ED93981F76",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-wheel-control-corvo-v66.mp4": {
      "video": "potion-wheel-control-corvo-v66.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-wheel-control-corvo-v66",
      "alias": "health_elixir_native_observation",
      "requirements": [
        "health_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-wheel-control-corvo-v66/result.json",
      "visual_review": "potion-wheel-control-corvo-v66/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.54646840148699,
      "video_sha256": "0851B6EED7E61E2F10DF1CB6E06B80AFAB15F2D34B7BD1B66C7E6F3767DFF4C3",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "../movement-integration/gestures-counter-audit-v1264.json",
    "coverage-v1330e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1338-ranged-items.json",
    "decision": "complete",
    "scope": "Drinking a health elixir in both roles: the native drink is presented as the use reach, hand to mouth (18-19 samples), and recovers.",
    "takes": [
      {
        "take": "coverage-v1330e",
        "cases": [
          "host-health",
          "client-health"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330e/client-health-observer.mp4",
        "verdict": "Emily brings her hand up to her mouth to drink, holds it briefly and lowers it."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: ../movement-integration/gestures-counter-audit-v1264.json](<../../../../work/movement-integration/gestures-counter-audit-v1264.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [in_game_recordings: potion-wheel-corvo-v64b.mp4](<../../../../work/natural-moveset/potion-wheel-corvo-v64b.mp4>)
- [in_game_recordings: potion-wheel-corvo-v64c.mp4](<../../../../work/natural-moveset/potion-wheel-corvo-v64c.mp4>)
- [in_game_recordings: potion-health-corvo-v64d.mp4](<../../../../work/natural-moveset/potion-health-corvo-v64d.mp4>)
- [in_game_recordings: potion-health-emily-v64d.mp4](<../../../../work/natural-moveset/potion-health-emily-v64d.mp4>)
- [in_game_recordings: potion-gameplay-corvo-v65.mp4](<../../../../work/natural-moveset/potion-gameplay-corvo-v65.mp4>)
- [in_game_recordings: potion-gameplay-emily-v65.mp4](<../../../../work/natural-moveset/potion-gameplay-emily-v65.mp4>)
- [in_game_recordings: potion-movement-corvo-v65.mp4](<../../../../work/natural-moveset/potion-movement-corvo-v65.mp4>)
- [in_game_recordings: potion-movement-corvo-v65-receiver.mp4](<../../../../work/natural-moveset/potion-movement-corvo-v65-receiver.mp4>)
- [in_game_recordings: potion-movement-emily-v65.mp4](<../../../../work/natural-moveset/potion-movement-emily-v65.mp4>)
- [in_game_recordings: potion-movement-emily-v65-receiver.mp4](<../../../../work/natural-moveset/potion-movement-emily-v65-receiver.mp4>)
- [in_game_recordings: potion-statistics-corvo-v66.mp4](<../../../../work/natural-moveset/potion-statistics-corvo-v66.mp4>)
- [in_game_recordings: potion-statistics-corvo-v66-receiver.mp4](<../../../../work/natural-moveset/potion-statistics-corvo-v66-receiver.mp4>)
- [in_game_recordings: potion-wheel-statistics-emily-v66.mp4](<../../../../work/natural-moveset/potion-wheel-statistics-emily-v66.mp4>)
- [in_game_recordings: potion-wheel-control-corvo-v66.mp4](<../../../../work/natural-moveset/potion-wheel-control-corvo-v66.mp4>)
- [in_game_recordings: native-potion-drink-host-v1263a-receiver.mp4](<../../../../work/natural-moveset/native-potion-drink-host-v1263a-receiver.mp4>)
- [in_game_recordings: native-potion-drink-client-v1263a-receiver.mp4](<../../../../work/natural-moveset/native-potion-drink-client-v1263a-receiver.mp4>)
## mana_elixir

Audit 1290: in product mode the lab grants a mana potion, but the native drink needs a natively spent pool (power use); the view waits for a map with powers. Audit 1264 (1214j): the mana elixir uses the same quantity-drop gesture path as the health elixir (kind 2); not yet recorded. Visual sweep 1260: reject (corvo reject, emily reject). Walking and stopping are clean, but there is no drink gesture for either partner, so the mana-elixir action is not represented. Native source consumption observed with inventory, resource and checkpoint restoration checks. Recorded Steam receiver walking/stopping remains continuous through native consumption. No FP drinking gesture is visible after wheel closure in the inspected source recordings. The queued native event is a drink sound, not a body scene; wheel logic suppresses it. Authored drinking clip remains unused; remote drink sound and resource synchronization remain unimplemented. Fine visual quality and untested actions are not accepted.

````json
{
  "id": "mana_elixir",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_network_movement_during_mana_use_partial_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "mana elixir",
  "motions": [
    "empty_idlebreak_drink_flask"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1290: in product mode the lab grants a mana potion, but the native drink needs a natively spent pool (power use); the view waits for a map with powers. Audit 1264 (1214j): the mana elixir uses the same quantity-drop gesture path as the health elixir (kind 2); not yet recorded. Visual sweep 1260: reject (corvo reject, emily reject). Walking and stopping are clean, but there is no drink gesture for either partner, so the mana-elixir action is not represented. Native source consumption observed with inventory, resource and checkpoint restoration checks. Recorded Steam receiver walking/stopping remains continuous through native consumption. No FP drinking gesture is visible after wheel closure in the inspected source recordings. The queued native event is a drink sound, not a body scene; wheel logic suppresses it. Authored drinking clip remains unused; remote drink sound and resource synchronization remain unimplemented. Fine visual quality and untested actions are not accepted.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "potion-mana-corvo-v67.mp4",
    "potion-mana-movement-corvo-v68.mp4",
    "potion-mana-movement-corvo-v68-receiver.mp4",
    "potion-mana-movement-emily-v69.mp4",
    "potion-mana-movement-emily-v69-receiver.mp4"
  ],
  "native_integration": {
    "potion-mana-corvo-v67.mp4": {
      "video": "potion-mana-corvo-v67.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-mana-corvo-v67",
      "alias": "mana_elixir_native_observation",
      "requirements": [
        "mana_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-mana-corvo-v67/result.json",
      "visual_review": "potion-mana-corvo-v67/visual-review.json",
      "proxy_sha256": "399CDA1770D53232C6DDDA59EAA6862783DF5120938F59D56F9EFF8816B25AE1",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "ABFA1639C6C855A351D23B7821C0223A425868BC70C4C6438EC09D02614A42D5",
      "checks_passed": true,
      "scope": "Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints."
    },
    "potion-mana-movement-corvo-v68.mp4": {
      "video": "potion-mana-movement-corvo-v68.mp4",
      "character": "corvo",
      "label": "Native elixir source observation · potion-mana-movement-corvo-v68",
      "alias": "mana_elixir_native_observation",
      "requirements": [
        "mana_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-mana-movement-corvo-v68/result.json",
      "visual_review": "potion-mana-movement-corvo-v68/visual-review.json",
      "proxy_sha256": "241507FB9BB498829200914893015985C5C79533BCD0C53CEC84FF5F5334480E",
      "actual_capture_hz": 27.88888888888889,
      "video_sha256": "703959A388C01D5468CD43A1851F7935D9786DFB41AEE9031E75FE817F3798DB",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native potion consumption, over Steam. No dedicated drink gesture, remote drink audio or remote resource synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-mana-movement-corvo-v68-receiver.mp4": {
      "video": "potion-mana-movement-corvo-v68-receiver.mp4",
      "character": "corvo",
      "label": "Remote movement during native mana use · potion-mana-movement-corvo-v68",
      "alias": "mana_use_movement_continuity",
      "requirements": [
        "mana_elixir"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-mana-movement-corvo-v68/result.json",
      "visual_review": "potion-mana-movement-corvo-v68/visual-review.json",
      "proxy_sha256": "241507FB9BB498829200914893015985C5C79533BCD0C53CEC84FF5F5334480E",
      "actual_capture_hz": 27.77777777777778,
      "video_sha256": "BAC574E7083979D5CF00C0FA5C4C334B62EA94FF4F62364E6DF6A77CB681620C",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native potion consumption, over Steam. No dedicated drink gesture, remote drink audio or remote resource synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-mana-movement-emily-v69.mp4": {
      "video": "potion-mana-movement-emily-v69.mp4",
      "character": "emily",
      "label": "Native elixir source observation · potion-mana-movement-emily-v69",
      "alias": "mana_elixir_native_observation",
      "requirements": [
        "mana_elixir"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-mana-movement-emily-v69/result.json",
      "visual_review": "potion-mana-movement-emily-v69/visual-review.json",
      "proxy_sha256": "241507FB9BB498829200914893015985C5C79533BCD0C53CEC84FF5F5334480E",
      "actual_capture_hz": 27.11111111111111,
      "video_sha256": "81B4E6501A42CC781DA255EEEBCC92E7D17817478200B836937212A7E0E2623E",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native potion consumption, over Steam. No dedicated drink gesture, remote drink audio or remote resource synchronization is implemented; all outcomes reset through isolated checkpoints."
    },
    "potion-mana-movement-emily-v69-receiver.mp4": {
      "video": "potion-mana-movement-emily-v69-receiver.mp4",
      "character": "emily",
      "label": "Remote movement during native mana use · potion-mana-movement-emily-v69",
      "alias": "mana_use_movement_continuity",
      "requirements": [
        "mana_elixir"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "potion-mana-movement-emily-v69/result.json",
      "visual_review": "potion-mana-movement-emily-v69/visual-review.json",
      "proxy_sha256": "241507FB9BB498829200914893015985C5C79533BCD0C53CEC84FF5F5334480E",
      "actual_capture_hz": 27.88888888888889,
      "video_sha256": "20FD9244320C3EB3414144FB2F39E0B99CF128D8D774E74526BA20ED15A15BC9",
      "checks_passed": true,
      "scope": "Existing remote body walking/stopping continuity through source native potion consumption, over Steam. No dedicated drink gesture, remote drink audio or remote resource synchronization is implemented; all outcomes reset through isolated checkpoints."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/ranged_interactions-review.json",
    "../movement-integration/gestures-counter-audit-v1264.json",
    "../movement-integration/mana-attempt-audit-v1290.json",
    "coverage-v1330e/result.json",
    "coverage-v1330i/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1346-mana.json",
    "decision": "complete",
    "scope": "Drinking a mana elixir in both roles after a power spent mana (Corvo 100 -> 50, Emily 100 -> 40): the partner presents the drink as the hand-to-mouth use reach (18-19 samples) and the pool is refilled to 100.",
    "takes": [
      {
        "take": "coverage-v1330i",
        "cases": [
          "host-manaelixir",
          "client-manaelixir"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1330i/client-manaelixir-observer.mp4",
        "verdict": "Emily thrusts her hand out to cast Mesmerize, the spirit forms above her, then she turns and lifts her hand to her mouth to drink before lowering it."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/ranged_interactions-review.json](<../../../../work/movement-integration/visual-sweep-v1260/ranged_interactions-review.json>)
- [evidence: ../movement-integration/gestures-counter-audit-v1264.json](<../../../../work/movement-integration/gestures-counter-audit-v1264.json>)
- [evidence: ../movement-integration/mana-attempt-audit-v1290.json](<../../../../work/movement-integration/mana-attempt-audit-v1290.json>)
- [evidence: coverage-v1330e/result.json](<../../../../work/natural-moveset/coverage-v1330e/result.json>)
- [evidence: coverage-v1330i/result.json](<../../../../work/natural-moveset/coverage-v1330i/result.json>)
- [in_game_recordings: potion-mana-corvo-v67.mp4](<../../../../work/natural-moveset/potion-mana-corvo-v67.mp4>)
- [in_game_recordings: potion-mana-movement-corvo-v68.mp4](<../../../../work/natural-moveset/potion-mana-movement-corvo-v68.mp4>)
- [in_game_recordings: potion-mana-movement-corvo-v68-receiver.mp4](<../../../../work/natural-moveset/potion-mana-movement-corvo-v68-receiver.mp4>)
- [in_game_recordings: potion-mana-movement-emily-v69.mp4](<../../../../work/natural-moveset/potion-mana-movement-emily-v69.mp4>)
- [in_game_recordings: potion-mana-movement-emily-v69-receiver.mp4](<../../../../work/natural-moveset/potion-mana-movement-emily-v69-receiver.mp4>)
## item_switch_interruptions

Three native item switches and equip cancellation at 30/70 percent are recorded, with at most one left-hand item visible. Arbitrary-time interruption, inventory state and runtime prop binding remain pending. 1088 (candidate1074j, Edge of the World, both roles): opening/closing the native item wheel no longer retires partner presentation (protocol73 holds; neutral body during the wheel), and a native D-pad quick-slot switch from sword to pistol is presented with the native pistol prop. Arbitrary-time interruption of other items remains open.

````json
{
  "id": "item_switch_interruptions",
  "family": "ranged_and_items",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_wheel_interruption_and_quickslot_switch_both_roles_mission_map",
  "visual_validation": "bounded_actual_game_frames_inspected",
  "native_reference_family_count": 251,
  "priority": 3,
  "title": "item switch interruptions",
  "motions": [
    "crossbow_equip_cancel_30",
    "crossbow_equip_cancel_70",
    "heart_equip_cancel_30",
    "heart_equip_cancel_70",
    "stunmine_equip_cancel_30",
    "stunmine_equip_cancel_70",
    "item_switch_crossbow_to_heart",
    "item_switch_heart_to_stunmine",
    "item_switch_stunmine_to_crossbow"
  ],
  "deliverable_kind": "animation",
  "notes": "Three native item switches and equip cancellation at 30/70 percent are recorded, with at most one left-hand item visible. Arbitrary-time interruption, inventory state and runtime prop binding remain pending. 1088 (candidate1074j, Edge of the World, both roles): opening/closing the native item wheel no longer retires partner presentation (protocol73 holds; neutral body during the wheel), and a native D-pad quick-slot switch from sword to pistol is presented with the native pistol prop. Arbitrary-time interruption of other items remains open.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "mission-equipment-v1088/equip-client.mp4",
    "mission-equipment-emily-v1088/equip-host.mp4"
  ],
  "evidence": [
    "../movement-integration/mission-equipment-audit-v1089.json",
    "mission-coverage-v1328j/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1340-switching.json",
    "decision": "complete",
    "scope": "Aiming the pistol and switching to the crossbow mid-aim, selecting Blink, holstering and redrawing in both roles: the partner presents the pistol (20-21 samples), then the crossbow (24-26), lowers it for Blink and presents the redrawn crossbow (27-28), with no presentation drop.",
    "takes": [
      {
        "take": "mission-coverage-v1328j",
        "cases": [
          "host-itemswitch",
          "client-itemswitch"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328j/client-itemswitch-observer.mp4",
        "verdict": "Emily raises the pistol, swaps to the wrist crossbow without lowering her arm fully, drops the arm when Blink is selected, then raises the crossbow again and puts it away."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/mission-equipment-audit-v1089.json](<../../../../work/movement-integration/mission-equipment-audit-v1089.json>)
- [evidence: mission-coverage-v1328j/result.json](<../../../../work/natural-moveset/mission-coverage-v1328j/result.json>)
- [in_game_recordings: mission-equipment-v1088/equip-client.mp4](<../../../../work/natural-moveset/mission-equipment-v1088/equip-client.mp4>)
- [in_game_recordings: mission-equipment-emily-v1088/equip-host.mp4](<../../../../work/natural-moveset/mission-equipment-emily-v1088/equip-host.mp4>)

