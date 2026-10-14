from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
take='domino-mark-fade-v209';result=read(P/take/'result.json');assert result['passed']
archive=R/'work/movement-integration/candidate-domino-world-v203'
record=dict(at=datetime.datetime.now().astimezone().isoformat(),take=take,passed=True,checks=result['checks'],dll=result['dll'],probe=result['probe'],continuity=read(P/take/'continuity.json'),world=read(P/take/'world-status.json'),retirement=read(P/take/'mark-stop-timeline.json'),visualReview=read(P/take/'visual-review.json'),limitations=result['scope'])
(archive/'runtime-verification.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
f=D/'30-native-domino-investigation.md';s=f.read_text(encoding='utf-8-sig')
s+='''
## Native mark retirement and first bounded world proof, v204–v209

The initial v204 counter assertion incorrectly counted packets rejected before receiver playback became ready. v205 therefore compares counters with a captured ready-state baseline. v206 passes body/identity/position checks but fails a700ms effect-record removal assertion. Its inspected receiver video exposes the important remaining limitation: the mark follows source-world NPC coordinates even when the corresponding receiver NPC is elsewhere. No attachment or gameplay acceptance is claimed.

[v208 diagnostic](../../work/natural-moveset/domino-mark-retirement-v208/mark-stop-timeline.json) retains the original failed assertion and records the exact same receiver-owned FX ID2262 and frozen transform after700,1200 and2200ms, absent4200ms. Local ownership stops once; it is not a new/restarted mark. [Stop decompilation](../../work/ghidra-domino-stop-v207.json) and [particle stop](../../work/ghidra-domino-particle-stop-v208.json) distinguish stopping components from removing the manager ID record. Native1a31f0 calls223c20; particle220ca0 calls228080, which starts stop timing and clears flag0x4. Native1a3250 uses a conditional component stop instead.1a1880/1a1990 remove pending requests, not the effect-ID map itself. The existing stop routine is retained; no broader or unowned effect is deleted.

[Original FX export](../../outputs/d2-coop-lab/research/asset-exports/domino-mark-fx-v207/manifest.json) confirms a looping native particle group and native stop sound. [Particle export](../../outputs/d2-coop-lab/research/asset-exports/domino-mark-particles-v209/manifest.json) preserves its authored emission/lifetime data. Both use pinned Void read-only export and verify original indices. Exact recorded lifetime, rather than inferred asset semantics alone, determines the test bound.

[v209 final receipt](../../work/natural-moveset/domino-mark-fade-v209/result.json) passes11 checks after complete cleanup and process exit. Candidate203 sends the exact native mark incarnation through Steam, creates one receiver-owned native validated mark, maintains the source world transform, and recovers four body phases.68 continuity samples keep body weight1.0 with zero clock rejections. Receiver shutdown retires the local mark slot by700ms, freezes its transform, creates no further mark, and the native record disappears3272ms after the stop request. The source native mark remains independently alive until checkpoint cleanup. Both inventories/health and all14 protected save/settings hashes pass.

[Inspected paired capture](../../work/natural-moveset/domino-mark-fade-v209/visual-review.json) shows the receiver glow fade after16.143s and disappear in the18s frame while the source still has its mark. Manager-record absence follows at19.154s. The observer camera cannot establish detailed body/hand quality. This is **standing armed single-target cosmetic replication**, not complete Domino, synchronized NPC attachment, shared damage, multi-target links or final visual acceptance. [Preserved runtime verification](../../work/movement-integration/candidate-domino-world-v203/runtime-verification.json) is separate from the immutable candidate creation identity.

The next investigation is partner target exclusion. [Component lifecycle decompilation](../../work/ghidra-domino-component-lifecycle-v210.json) confirms native network acquisition on component start and c8 target reset on equip. Existing eligibility447460 is called by both447410 and447520; returning false there can produce type2/out-of-mana feedback in68ed50. That is an unsuitable unqualified exclusion point. No Domino eligibility hook is installed yet.
'''
f.write_text(s,encoding='utf-8')
f=R/'work/movement-integration/refresh_grenade_handoff_v58.py';s=f.read_text(encoding='utf-8-sig')
a=s.index("m['LatestContinuation']['in_progress']=");b=s.index("\n",a)
s=s[:a]+"m['LatestContinuation']['in_progress']='Candidate203 adds bounded native Domino mark replication to verified body44/bank47. v209 passes11 checks,68 continuity samples weight1,zero clock rejection and bounded native particle retirement3272ms; paired source/receiver sheets are inspected. v204/v206/v208 retain failed startup/retirement assertions and their diagnosis. Source-world mark coordinates are not synchronized NPC attachment; links, partner exclusion and shared gameplay remain unfinished. Chapters29/30 preserve evidence. Full movement and playable co-op remain incomplete.'"+s[b:]
a=s.index('Latest scoped results:');b=s.index('\n\n',a)
s=s[:a]+'''Latest scoped results: [Chapter29](29-airborne-root-continuity.md) records armed/holstered Mesmerize air continuity. [Chapter30](30-native-domino-investigation.md) records native Domino body phases, scoped target ownership and candidate203 cosmetic mark replication. v209 passes11 checks with continuous body weight and native mark retirement3272ms after stopping; source/receiver recordings show independent ownership and fade. Earlier failed assertions are retained with diagnoses. Mark positions still follow the source world; synchronized NPC attachment, multi-target links, partner exclusion and shared gameplay remain unfinished.'''+s[b:]
f.write_text(s,encoding='utf-8')
print('Recorded Domino world proof and documented limitations')
