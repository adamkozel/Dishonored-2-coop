from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
checks={t:read(P/t/'result.json') for t in ['domino-partner-picker-v212','domino-guard-regression-v213','domino-two-target-source-v214','domino-guard-near-v215','domino-guard-grounded-v216']}
assert checks['domino-partner-picker-v212']['passed'] and checks['domino-two-target-source-v214']['passed']
v={'recorded':datetime.datetime.now().astimezone().isoformat(),'candidate':211,'partner_exclusion_verified':True,'ordinary_native_two_guard_linking_verified':True,'paired_mark_regression_with_candidate211_verified':False,'dll':checks['domino-partner-picker-v212']['dll'],'scope':'Exact current partner excluded by native picker; ordinary NPC selection and two-target source linking still work. Paired body/mark regression failed its scripted aim, so remains pending. No shared NPC outcome, link replication or completed power claim.','takes':checks}
(R/'work/movement-integration/candidate-domino-partner-v211/runtime-verification.json').write_text(json.dumps(v,indent=2),encoding='utf-8')
f=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=f.read_text(encoding='utf-8-sig')+'''

## Partner exclusion and native connection evidence, v212–v217

The final [v212 partner receipt](../../work/natural-moveset/domino-partner-picker-v212/result.json) passed nine checks and completed cleanup. The native picker rejected 1,712 exact partner candidates, with zero read failures; 478 valid target snapshots stayed empty and mana stayed 100. The inspected native video shows Corvo in the aiming corridor without a target mark. This proves the scoped native target exclusion, not general shared-damage behavior.

[v214 source observation](../../work/natural-moveset/domino-two-target-source-v214/result.json) passed nine checks using the same candidate211 DLL. Native controller input selected two existing guards, producing two directed connections but one visible beam. All 477 target reads were valid; 420 contained marks. Source video and the [raw connection header](../../work/natural-moveset/domino-two-target-source-v214/network-connections-header.json) are preserved. Ordinary enemy selection therefore still works with the partner filter. No remote NPC synchronization is implied.

Paired regressions v213 and v216 failed because scripted aiming did not retain a visible guard. The v216 source sheet shows a pillar obscuring the aim direction; its nearest-upper-floor query had selected tutorial guard1 after he moved upstairs, unlike the successful v214 selection of guard2. The strict selection-phase check correctly refused both takes. v215 failed earlier because its fixed floor expectation belonged to the old source position: the new position is 66.4036484, versus the previous66.4370575. v216 uses three native stable position/motion samples at the measured floor. These are harness failures, not accepted power evidence. All final receipts include checkpoint recovery and fourteen protected save/settings hashes. Both videos of each captured failed take are retained in the review manifest. v215 had no video.

### Connection layout and endpoint creation

[Saved exact-executable decompilation](../../work/ghidra-domino-link-fx-v216.json) establishes that `1404dd500` reads two selected node pointers at connection+8/+10, computes its interpolated end using`1404db9e0`, and creates its visible link through`1404e2de0`. The network declaration's +b0 field supplies`phlink_links_01.fx`; +a8 supplies the second optional effect. Native`4e2de0` stores start position/basis at FX record+38/+44, end position/basis at+80/+8c, and callback at+128. The caller passes a null callback. It owns the link effect on the local player (handle1), unlike the temporary owners of individual target marks.

The v214 runtime network has connection list/count at+15b0/+15b8, pool/capacity at+15c0/+15c8, capacity32, and stride0x80. Both live directed entries had state1 and progress1 at+28. Connection0 linked lower→upper, with FX401 at+2c and owner1 at+30. Its reverse connection had sentinel FX~0 and owner0x3fff0010. Replication must preserve observed beam presence rather than synthesize a second beam for the reverse gameplay edge.

The new [read-only connection reader](../../outputs/d2-coop-lab/src/native_domino_links.h) validates the current native power/network, selected NPC generations, bounded aligned connection pool, unique directed pairs, native beam declaration and player ownership, orthonormal start/end bases, endpoint proximity to native interpolation, null callback, and a second coherent identity snapshot. It rejects unplugging/retired connections and concurrent identity changes. [Synthetic adversarial tests](../../outputs/d2-coop-lab/src/native_domino_links_tests.h) pass, including duplicate/recycled/foreign identities, invalid progress, displaced endpoints and races. [Probe217 archive](../../work/movement-integration/probe-domino-links-v217/identity.json) records the working tool version; the production DLL remains candidate211. Runtime verification of this new reader is pending its v217 take. No new connection method is called by this reader.
'''
f.write_text(s,encoding='utf-8')
print('Recorded candidate211 bounded verification and v217 research status')
