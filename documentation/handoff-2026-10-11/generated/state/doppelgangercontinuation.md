# State record: DoppelgangerContinuation

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "at": "2026-10-12T23:06:37.829204+02:00",
  "live": "1A3F5720DA7431418D613A9DF932B953B2AE304BF75826DD77D8FF82B2887279",
  "probe": "AEAEAEEB2E85620D2599EF946E8C8076C326C1DAF6452FA4153AF0A7C82F3EB9",
  "receipt": "work/movement-integration/doppelganger-cancel-verification-v505.json",
  "complete": false,
  "latest": "Native493/494 show standing/crouched Emily keeps the sword sheathed throughout the466/1200/666ms Doppelganger phases. 495 appends six complete-body hands-free phase clips to bank57/331, retaining all325 previous payloads; semantic53 accepts the observed native equipment context. 496/497 pass remote body/clone/FX and standing-recovery checks. Standing/crouched armed evidence491 and continuous clone/material-reuse evidence485/486/488 remain preserved. Airborne/cancellation/upgrades/multiple clones, shared outcomes and visual acceptance remain open; whole movement/playable co-op incomplete. Native cancellation505: confirmed X-with-LT cancels with no source activation/mana/clone scene and no remote clone/FX;503 standing armed and504 crouched holstered recover full body. Wrong-input499/500 and array-assertion502 failures are preserved. Native air, other interruptions, upgrades/multiple clones, late join and shared gameplay remain unfinished. Airborne517: diagnostic512 passes paired513/510,20checks each; earlier509 landing dropout remains unresolved. Actual source/remote recordings reviewed in temporal samples; no final visual acceptance.",
  "next": "Continue remaining native movement/power recovery and shared-world co-op. Partner-only interaction edge cases stay deferred; ordinary movement collision retained.",
  "cloneOwnershipReceipt": "work/movement-integration/candidate-doppelganger-world-probe-v416/runtime-verification-v420.json",
  "cloneTransportCandidate": "work/movement-integration/candidate-doppelganger-stream-v418/identity.json",
  "bodyReceipt": "work/movement-integration/candidate-doppelganger-visual-v427/runtime-verification-v431.json",
  "airborneResearch": {
    "at": "2026-10-12T23:18:16.224595+02:00",
    "candidate": "candidate-doppelganger-air-v512",
    "identity": {
      "created": "2026-10-12T23:16:33.926752+02:00",
      "dll": "F33AC7D2DF348D1D5F1FF651EFD894FDFEEF45CAAB7122DDDE1C4ACE99F40EA8",
      "probe": "3C2A4EF6FF6EB91572D28C4BC93E800499C8DD6EB7FEA919394CE4751300CF0B",
      "runtimeVerified": false,
      "previous": "candidate-doppelganger-air-v508",
      "protocol": 54,
      "bankVersion": 57,
      "clips": 331,
      "cloneWireVersion": 4,
      "scope": "Diagnostic512 freezes at most32 movement inputs and16 root samples on the first clock rejection.509 failed landing continuity; no guard relaxation or gameplay changes. Native airborne extension508 remains experimental until corrected runtime checks pass.",
      "banks": [
        {
          "path": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\movement-integration\\candidate-doppelganger-air-v512\\banks\\corvo-doppelganger-holstered-bank.bin",
          "sha256": "4265E96AEA1AEF1CD34E9AE1692E24C2768C0AD0106041E4D874B32E168BAA86"
        },
        {
          "path": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\movement-integration\\candidate-doppelganger-air-v512\\banks\\emily-doppelganger-holstered-bank.bin",
          "sha256": "1A188F00EAAB3A38F92C85C2A271766FDE3C1E9DC59C333658D1E5258E58F42E"
        }
      ],
      "assets": [
        {
          "resource": "models/effects/d2coop/doppelganger_spawn_remote.fx",
          "generated": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\assets\\doppelganger_spawn_remote.fx",
          "sha256": "5083F005AD5FD92E6155F8C7505E82FDF633CDAB56FB6ECA574AE7C481440A92",
          "source": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\research\\asset-exports\\native-doppelganger-fx-v436\\game1_patch\\generated.decls.fx.models.effects.gameplay.powers.doppelganger.decoy.spawn_despawn.decoy_spawn_01..fx.decl",
          "source_sha256": "9FF273CF05F37DA0A52943003A05960C2062281ADC8B56928D984CF47310FC86",
          "kind": "fx",
          "classname": "fx",
          "runtime_verified": false,
          "removed": [
            "m_rumbles",
            "sound_events/game_defined/bsp_ai_properties_02"
          ],
          "native_particles_unchanged": true
        },
        {
          "resource": "midnight/d2coop/doppelganger_fx_spawn.midnightscene",
          "generated": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\assets\\doppelganger_fx_spawn.midnightscene",
          "sha256": "D080A1E1AF76093AB88995B71A0511A0206E46B51CDEB9D2601A2FEC087A3A5A",
          "source": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\research\\asset-exports\\native-doppelganger-v403\\game1_patch\\generated.decls.midnightscene.midnight.gameplay.human.common.doppelganger.doppelganger_spawn..midnightscene.decl",
          "source_sha256": "11683936C802FBD2567B625D70F4C040A9D7E87240E1A0C7F80021027EC72B87",
          "kind": "midnightscene",
          "classname": "midnightScene",
          "runtime_verified": false,
          "removed_tracks": [
            "EMTT_FACEFX",
            "EMTT_ANIM",
            "EMTT_SFX"
          ],
          "sockets": [
            "decoy_root",
            "sl_l_arm",
            "sl_r_arm",
            "decoy_spawn_l_hand",
            "decoy_spawn_r_hand"
          ],
          "scenes": [
            {
              "m_length": 1000,
              "m_name": "Scene_1",
              "m_speechChecksum": 0,
              "m_forbidHoldToSkipScene": "false"
            }
          ],
          "animation_tracks": 0,
          "gameplay_event_tracks": 0
        },
        {
          "resource": "models/effects/d2coop/doppelganger_despawn_remote.fx",
          "generated": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\assets\\doppelganger_despawn_remote.fx",
          "sha256": "8F5FF5B1A947156834C57B45D53B50F706CB7EEBF44552A2B257B1D2E072617A",
          "source": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\research\\asset-exports\\native-doppelganger-fx-v436\\game1_patch\\generated.decls.fx.models.effects.gameplay.powers.doppelganger.decoy.spawn_despawn.decoy_despawn_01..fx.decl",
          "source_sha256": "6CF35F30E3577645308FDEDA3D0579D09B7D6D20672C1FBCB2A4FE6404A45F0D",
          "kind": "fx",
          "classname": "fx",
          "runtime_verified": false,
          "removed": [],
          "native_particles_unchanged": true
        },
        {
          "resource": "midnight/d2coop/doppelganger_fx_despawn.midnightscene",
          "generated": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\assets\\doppelganger_fx_despawn.midnightscene",
          "sha256": "AF2E6038672C6FD47F1CCD6A812A35415F50DF2288B754F97F5B6E01AAD86E46",
          "source": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\outputs\\d2-coop-lab\\research\\asset-exports\\native-doppelganger-v403\\game1_patch\\generated.decls.midnightscene.midnight.gameplay.human.common.doppelganger.doppelganger_dispawn..midnightscene.decl",
          "source_sha256": "69200A51EC22B2C3312208A96CB2EA132E6E86D994C0690ED5099C3F3D5A1281",
          "kind": "midnightscene",
          "classname": "midnightScene",
          "runtime_verified": false,
          "removed_tracks": [
            "EMTT_FACEFX",
            "EMTT_ANIM"
          ],
          "sockets": [
            "decoy_spawn_r_hand",
            "decoy_spawn_l_hand",
            "decoy_root",
            "sl_l_arm",
            "sl_r_arm"
          ],
          "scenes": [
            {
              "m_length": 466,
              "m_name": "Scene_1",
              "m_speechChecksum": 0,
              "m_forbidHoldToSkipScene": "false"
            }
          ],
          "animation_tracks": 0,
          "gameplay_event_tracks": 0
        }
      ]
    },
    "passed": false,
    "complete": false,
    "native": [
      "native-doppelganger-air-release-v506",
      "native-doppelganger-air-entry-holstered-v507"
    ],
    "failed": "doppelganger-air-release-armed-v509",
    "failure": "Native source and remote phases work, but landing rejects -0.15257263m over18.9581ms with prior airState3, dropping body weight to0. Candidate512 adds bounded local evidence only; no guard changes.",
    "next": "Retain bounded first-failure movement/root trace. Do not broaden motion guards without evidence. Continue Shadow Walk and larger co-op gaps; revisit this intermittent landing failure with a concrete trace.",
    "receipt": "work/movement-integration/doppelganger-air-verification-v517.json",
    "recordedPasses": [
      "doppelganger-air-landing-trace-v513",
      "doppelganger-air-entry-holstered-v510"
    ]
  }
}
````
