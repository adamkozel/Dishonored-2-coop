# State record: HistoricalWorkBeforeDoppelganger

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "WorkInProgress": {
    "UpdatedAt": "2026-10-09T02:20:52.6804418+02:00",
    "CandidateDll": "9677FB57F9B6B9BF5A6EAC84807BEEE47C73DFE6052977A2BA68896E20900526",
    "Scope": "Experimental actual-player body poses over native Steam; 17 mapped joints on Corvo/Emily counterparts, crouch/crouch-walk/jumps, native standing gait, 250ms freshness, both pauses and actual peer loss. Verified again after both shared reloads and deaths. Unmapped native arm motion and target bone lengths preserved.",
    "Protocol": "src/checkpoint_sync.h and checkpoint_protocol.h; explicit coop_checkpoint_sync on/off; both-ready gate after role restore",
    "PendingTest": "0B07 local native equipment test passed five checks, nine drawn samples: sword slot0, hand joint17, holster195, duplicate nonrenewal, expiry and explicit/guard cancellation restore. Probe ED46. New unbuilt source adds game-over reads and pending-restore world retirement; do not attribute current source to0B07. Next equipment pause/reload/disconnect guards, then Steam weapon-state replication. No running tool session.",
    "Correction": "Loading entry is native caller787a55 with validated owner, empty stack, pending0 and phase2 observed; player already exists. GameLocal addresses may be reused. Cached LoadGame skips asynchronous memory restart so entry consumes any remaining continuation permission.",
    "Automation": "dishonored-2-overnight-development; every 30 minutes through 08:00 Europe/Prague 2026-10-09",
    "Next": "0B07 local native equipment test passed five checks, nine drawn samples: sword slot0, hand joint17, holster195, duplicate nonrenewal, expiry and explicit/guard cancellation restore. Probe ED46. New unbuilt source adds game-over reads and pending-restore world retirement; do not attribute current source to0B07. Next equipment pause/reload/disconnect guards, then Steam weapon-state replication. No running tool session."
  },
  "Next": "BA0B archived six reports in research/milestones/ba0b and full backupwork/native-attachment-BA0B. Native child frame fix fully measured and combined fatal passed. Local native enemy assassination damage passed5checks on7610, corpse screenshot saved and backupwork/native-enemy-local-7610. Actual player strikes before Mayhew nohit because nativeinventoryheld; after native Mayhew31sec Xscene, client RTrealassassination killsguard40to0(host40). Rawwork/player-strike-after-mayhew-observations-BA0B.json. New enemy_outcome protocol212bytes bothroles optin60sec, fixedguard actualplayerassassin capture, nativedeathreceiverattributedproxy, reliablekill/ack/duplicates. First AA26runtime handshakefailed due packetdispatch mistakenlynestedunderSwordmagic, fixed now. New build deployed/restart+restore+Test-D2EnemyOutcome -SourceRole client running session12798. Need followtest, fix ifneeded, testhostreverse, lifetimeguards, regressions. Active source includesnative_enemy_damage.h,enemy_outcome.h,enemy_outcome_protocol.h,enemy_outcome_tests.cpp andformalTest-D2EnemyDamage/Outcome. Do notstopfor milestones user sleeping.",
  "VerificationAttribution": "Historical milestones retain their own DLL hashes. Current9639 relationship tests passed on both fixed guards; partner perception positive capture exists but formal test running."
}
````
