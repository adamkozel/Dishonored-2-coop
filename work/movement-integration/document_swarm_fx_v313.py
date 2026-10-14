from pathlib import Path
import json,collections,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
for take in ('swarm-fx-observe-v312','swarm-diagnostic-v313'):
 rows=[json.loads(x) for x in (P/take/'swarm-effects.ndjson').read_text().splitlines() if x.strip()]
 groups=collections.defaultdict(list)
 for row in rows:
  if row['valid']:
   for fx in row['effects']:groups[fx['id']].append((row,fx))
 summary=dict(take=take,samples=len(rows),valid=sum(x['valid'] for x in rows),failures=dict(collections.Counter(str(x['failureLine'])for x in rows if not x['valid'])),effects=[])
 for id,entries in groups.items():
  first,fx=entries[0];last=entries[-1][0]
  matched=[r for r,f in entries if r.get('equipmentValid') and r.get('worldHandle')==f['owner']]
  summary['effects'].append(dict(id=id,name=fx['name'],owner=fx['owner'],parent=fx['parent'],ownerDefinition=fx['ownerDefinition'],samples=len(entries),firstTick=first['tick'],lastTick=last['tick'],observedSpanMs=last['tick']-first['tick'],currentEquipmentWorldMatches=len(matched),origin=fx['origin']))
 (P/take/'fx-observation-summary.json').write_text(json.dumps(summary,indent=2))
p=P/'swarm-diagnostic-v313/visual-review.json';v=read(p);v['notes']=v['notes'].replace('Native source attacks victim; remote cosmetic rats only; outcome synchronization not claimed.','Native source rat gameplay remains local; peer contains cosmetic rats only; outcome synchronization not claimed.');p.write_text(json.dumps(v,indent=2))
doc=D/'31-native-devouring-swarm.md'
section='''

## Native effect ownership investigation and continuity diagnostics, v311–313

[311](../../work/natural-moveset/swarm-fx-observe-v311/result.json) failed because the new read-only probe mode was missing from the argument allowlist. It exited before reading the game; its zero-byte FX file is not evidence that effects were absent. Existing23 gameplay checks and protected cleanup passed, but the whole run remains failed. [312](../../work/movement-integration/candidate-swarm-fx-observe-v312/identity.json) corrects admission. An invalid-PID CLI smoke reached the explicit “not Dishonored2.exe” guard(exit4), confirming command admission without touching the game. The first smoke assertion incorrectly expected exit3; that assertion was corrected after reading the code and output.

[Native312](../../work/natural-moveset/swarm-fx-observe-v312/result.json) collected476 observations,458 stable and18 rejected as changing. [Its FX summary](../../work/natural-moveset/swarm-fx-observe-v312/fx-observation-summary.json) contains13 IDs: one aiming marker, one summon burst, seven rat trails and four death effects. The burst record remains present for at least11,922ms, despite its authored emitter duration of1s. Record lifetime is not emission duration. Trails last approximately1.8–2.2s in the observed map and use temporary owners with no entity definition; name/proximity alone cannot prove the rat attachment. Death effects use `fx/worldfx.def`. No receiver FX is created in312.

312 fails body continuity: four sampled weights at3.984–4.296s fall below.95, minimum.254896. Clock rejection counters stay0. Action telemetry records one packet253.26ms old exceeding the250ms admission limit. The old telemetry does not identify the precise eligibility predicate responsible; attributing the drop to a specific buffer reset would be a hypothesis. Do not relax freshness gates based on this run. Both checkpoint restores and protected14 hashes pass. The two contact sheets were viewed but this failed run is not indexed as passing.

[Candidate313](../../work/movement-integration/candidate-swarm-diagnostic-v313/identity.json) adds a bounded16-record diagnostic for movement eligibility. Mask bits:1 missing semantic sample;2 unsupported physical context;4 expired stream lease;8 peer unready;16 state absent/older than250ms;32 unsupported action context;64 native shadow action owns actor. It records pose/semantic times, state age and physical context on failure. Existing predicates and freshness limits are unchanged. The external research watcher now polls every100ms and reads the validated equipment's current world handle. Its checks remain read-only. Camera pitch20 improves the combined caster/floor view, though a pillar still partly occludes Corvo.

[313 passes23 checks](../../work/natural-moveset/swarm-diagnostic-v313/result.json): minimum body weight1.0 across53 continuity samples, no clock rejections and no eligibility failures. [137 FX observations](../../work/natural-moveset/swarm-diagnostic-v313/fx-observation-summary.json) include135 valid equipment-bound reads. All108 summon observations have an owner equal to the exact power component+D4 world handle. This supports source attribution. It does not establish that reducing observer load fixes the312 reliability issue; the failure did not reproduce. Both sheets were inspected and paired videos indexed(743 total). Visual acceptance remains false; rat color, native action states, VFX and shared outcomes still need work.

Read-only exports are in [native-swarm-fx-v311](../../outputs/d2-coop-lab/research/asset-exports/native-swarm-fx-v311/manifest.json). Exact static evidence: [world-spawn function1406a7a30](../../work/movement-integration/swarm-spawn-function-decompile-v313.txt), [allocation call sites](../../work/movement-integration/swarm-fx-allocation-calls-v312.txt), [rat FX helpers](../../work/movement-integration/swarm-fx-helpers-decompile-v312.txt). Correction: the summon axis is a random Z rotation produced through1408ac000/14010e6a0, not the world entity's orientation. Origin comes from entity+300. Component declaration+A0 supplies the FX;19d100 allocates it with the world owner;1a2ff0 starts it. Rat+80 is a generic effect ID used for more than spawn, including burning; rat+104 ownership remains under investigation. The Ghidra project was saved with confirmed/static distinctions.

## Source-owned summon burst candidate314

[Candidate314](../../work/movement-integration/candidate-swarm-spawn-fx-v314/identity.json) adds a separate176-byte native Steam cosmetic packet and a native-derived effect. Source discovery requires a newly observed world after stream baseline, the current strict equipment/world relationship, exact declaration+A0, unique FX owner identity, origin within10cm of the world actor, a current cast25 and a750ms discovery window(max8 scans). An already-present world is baselined and cannot replay on late join. Sender and receiver consume the event once, even if native playback fails.

The receiver validates sessions, worlds, map, tokens, source clock and ordering; then requires the matching presented Swarm world and power event before starting the finite effect. It never resolves source handles in the receiving game or invokes Swarm/AI/damage APIs. The effect is owned by the remote character, anchored to the observed source transform, stopped after1600ms or ownership retirement. A dedicated isolated resource keeps the native summon particle and sound event and removes only rumble. [Asset provenance](../../outputs/d2-coop-lab/assets/swarm_spawn_remote.provenance.json), [installer](../../outputs/d2-coop-lab/Build-D2SwarmSpawnFxAssets.ps1), original-index hashes and clone backup receipts preserve reproducibility. Native sound-event configuration is not an auditory quality check; current videos have no audio track.

Protocol tests cover malformed packets, wrong worlds/roles, stale cast, reflected/nonfinite matrices, duplicate source effects/casts/worlds, late-join baselining and bounded discovery. Build/load/protocol tests pass. Runtime/visual result pending. This first burst path does not cover the aiming marker, per-rat spawn trails, death/burning/despawn, attack sounds or shared damage.
'''
assert '## Native effect ownership investigation and continuity diagnostics, v311–313' not in doc.read_text()
doc.write_text(doc.read_text()+section)
print('Wrote312/313 evidence summaries and detailed handoff through candidate314')
