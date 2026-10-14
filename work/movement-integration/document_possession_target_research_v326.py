from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
for take in ('possession-human-v324','possession-human-v326'):
 t=P/take;r=read(t/'result.json');assert not r['passed'] and (t/'source-sheet.jpg').is_file()
 write(t/'visual-review.json',dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),evidence=['source-sheet.jpg'],notes='Source temporal sheet viewed: native human target highlight, tunnel travel, altered host vision and Leave Host Body prompt.326 additionally shows normal-input exit and target recovery. No remote body, target-control authority, replicated VFX or audio verification. Overall run failed its recorded assertion or cleanup check; original receipt unchanged.',visual_accepted=False,complete=False))
summary=dict(at=datetime.datetime.now().astimezone().isoformat(),results={t:read(P/t/'result.json')for t in ('possession-human-v324','possession-human-v325','possession-human-v326')},targetObservation=read(P/'possession-human-v326/target-observation-summary.json'),overallPassed=False,complete=False)
write(W/'possession-target-investigation-through-v326.json',summary)
old=read(R/'work/handoff-project-bookmarks-current.json');merged={(b['address'],b['category']):b for b in old}
for b in read(W/'ghidra-bookmarks-additions-v326.json'):merged[(b['address'],b['category'])]=b
write(R/'work/handoff-project-bookmarks-current.json',sorted(merged.values(),key=lambda b:(b['address'],b['category'])))
f=D/'32-native-possession-investigation.md';s=f.read_text();heading='## Human target investigation and retained failures324–326'
if heading not in s:
 s+='\n\n'+heading+'''

[Exact upgrade exports](../../outputs/d2-coop-lab/research/asset-exports/possession-upgrades-v324/manifest.json) identify `possession_parasite.inventoryitem` as TYPE_POWER_UPGRADE20 granting `ARK_ATTRIBUTE_POSSESSION_ALLOW_HUMANS`. Other upgrades independently permit corpses, potions, host chaining or increased duration; no meaning is inferred merely from filenames. [Target and sensor declarations](../../outputs/d2-coop-lab/research/asset-exports/possession-target-types-v324/manifest.json) name a15-metre sensor, `possession_target`/`camera_tag` joints, human target/travel/tunnel/vision/expiry/exit effects and native audio events. These are asset facts; their peer reproduction remains unimplemented. Original resource/index hashes were unchanged by read-only exports.

324 grants only the exact human upgrade plus power through already verified native inventory APIs, then uses ordinary LT input toward a reciprocal tutorial guard. It visibly entered the host and observed native400ms release, but its assertion incorrectly equated internal state14 with physics stance. The failed result remains unchanged.325 added a generation-checked reader but failed because its watcher CLI option was omitted from the argument allowlist. Its target also missed; mana remained100. It is not a successful transfer receipt.

326 registers and preflights the read-only watcher, refreshes the same guard's identity/position immediately before a500ms native trigger input, and uses LT120 to leave the host.185 valid owned-target samples include159 occupied-context samples. Internal states0/1/8/9 and inactive14 are observed; physics stance18 and nondefault mode accompany occupation. The exact target `arkCpntNPCPossession` vtable1b641a8, weak-reference generation, entity handle and unique component membership are bracketed by repeated reads. Mana100→80 reflects expenditure plus native replenishment, not a claimed20-point base cost. Native assets specify a40-point cost and20-point replenishment. Source footage shows highlight, tunnel, host vision and normal exit. The run still fails quantity cleanup; both checkpoint restores and protected14 checks pass. No failed run is indexed as complete or used as evidence that shared target authority works.

The strict diagnostic uses internal mode `[machine+2F8]`, controller base+410, state+5D0, target handle+63C, weak generation+640 and component+648. [Transition function824e40](../../work/movement-integration/possession-state-transition-decompile-v325.txt) establishes14 as the inactive sentinel, correcting the initial hypothesis. Physics stance is independently read from the player's physics component+50; active-state IDs come from the current mode. Runtime entry uses active state26/vtable1c79878 (`arkPlayerStateEnterPossessedHost`). Do not conflate these three enums.

326 also exposed fixture interference: its acquisition loop attempted to re-equip after the native power temporarily changed slots, and triggered early restoration.327 changes acquisition to equip only until the first native-ready observation and adds per-sample fixture/cleanup telemetry. Its runtime result is pending. Every experiment reloads the isolated checkpoint, including failures. [Collected exact receipts](../../work/movement-integration/possession-target-investigation-through-v326.json) preserve all limitations and pins.

Next work: prove native inventory cleanup, then admit successful release and host lifecycle into a separately validated remote presentation path. Body disappearance/reappearance, native target/exit VFX, shared host control and NPC outcomes, interruption/disconnect/death/load behavior, animal/corpse/chain variants and all alternate movement contexts remain outstanding. The target diagnostic is not yet used to drive networking or mutate the target.
''';f.write_text(s,encoding='utf-8')
print(json.dumps(dict(bookmarks=len(merged),failedTakes=3)))
