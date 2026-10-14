# keyhole-native-findings-v2.json

Original: [work/movement-integration/keyhole-native-findings-v2.json](<../../../../work/movement-integration/keyhole-native-findings-v2.json>)

SHA-256: `FF70A83E4868724A5E24F084AB918DA10B03FCE563B2E4E80D1BA0D54BA81F56`; 2159 bytes.

## Original saved record

````json
{
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "loaderSha256": "53571DDE303A45B861703D1AE05B037F0E35D881DC77FC17593966D82CEDB334",
  "confirmedRuntime": [
    "Native out-scene may leave both midnight manager lists around 200ms before Peeking24 retires. The remaining +ec phase is 3 then 0. A bounded owner/door/arm-validated endpoint handoff prevents stale streams.",
    "Early B during entry clears Peeking+24 scene handle to zero, leaves phase+ec=3 briefly, then returns directly to ground/original crouch. There is no native out-scene in that path.",
    "Early cancellation is bridged only from a freshly confirmed scene for at most400ms, with unchanged state/door/arms and absent scene in both manager lists. It holds the last confirmed pose instead of inventing a full exit.",
    "Eight side/stance/role cases pass native Steam phase/equipment/stance/health checks. Both originally drawn and originally holstered repeats are included.",
    "Standing Corvo and crouched Emily early cancellation pass8 checks each. Same-world keyhole menu pause/resume passes5 checks each, with new Steam tokens and the same native scene.",
    "Crouched full-body donor revision2 trims the donor's standing entry/exit, retaining low movement. Prior183 bank clips remain unchanged."
  ],
  "remaining": [
    "Pause visual continuity: suspended movement bank temporarily returns to native idle/hidden avatar before recovery. Functional ownership checks do not establish acceptable visual pause behavior.",
    "Final continuous video quality, exact door/hand/head alignment, other geometry and door variants.",
    "Death, reload, disconnect and world-change coverage during keyhole.",
    "The complete movement phase and other co-op systems remain incomplete."
  ],
  "testFixture": "Expanded exact NPC isolation holds five observed Dunwall NPCs (10 Decision/Reflex references), including two balcony tutorial guards. Their definition uses dunwall_guard_male_small_01.def. Pausing AI does not reset prior position/momentum; clean protected-checkpoint restoration was necessary for unoccluded reverse-side recording."
}

````
