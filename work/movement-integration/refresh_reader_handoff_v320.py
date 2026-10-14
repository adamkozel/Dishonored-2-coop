from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
t=P/'swarm-trails-v320';result=read(t/'result.json');assert result['passed']
regressions={name:read(P/name/'result.json') for name in ('grenade-reader-host-v320','grenade-reader-emily-v320','grenade-reader-equip-v320')}
assert all(v['passed'] for v in regressions.values())
cleanup=read(P/'grenade-reader-equip-v320/reader-regression-result.json');assert cleanup['passed'] and cleanup['checkpointRestored']
old=read(P/'swarm-trails-v319/power-event-history.json')['source'];new=read(t/'power-event-history.json')['source']
trails=read(t/'trails-summary.json');continuity=read(t/'continuity.json')
summary=dict(at=datetime.datetime.now().astimezone().isoformat(),candidate='candidate-swarm-trails-v320',dll=result['dll'],probe=result['probe'],
 sourceTimings={v:{k:x.get(k)for k in ('sourceReadMaxMs','sendGapMaxMs','sourceAttempts','sourceMisses','sourceReadTimings')}for v,x in [('319',old),('320',new)]},
 continuity=continuity,trails=trails,regressions=regressions,equipCleanup=cleanup,complete=False)
write(W/'candidate-swarm-trails-v320/reader-regression-verification.json',summary)
bookmarks=read(R/'work/handoff-project-bookmarks-current.json');merged={(x['address'],x['category']):x for x in bookmarks}
for x in read(W/'ghidra-bookmarks-additions-v320.json'):merged[(x['address'],x['category'])]=x
write(R/'work/handoff-project-bookmarks-current.json',sorted(merged.values(),key=lambda x:(x['address'],x['category'])))
videos=len(read(P/'runtime-video-manifest.json'))
ann=read(P/'review-evidence-annotations.json')
ann['items']['corvo_devouring_swarm_cast']['notes']=f'Grounded armed/holstered Swarm contexts retained.25 cosmetic rats receive strict native transforms through Steam with120ms interpolation, native scale and measured-distance gait.320 passes25 checks: one native summon effect and7 native rat trails, guarded per-frame transforms, finite cleanup, duplicate refusal, bodyweight1.0, checkpoint restoration and protected14.319 trail cleanup succeeded but body eligibility failed when semantic history ended235.5ms before the root pose.320 removes idle full grenade inventory scans; observed source read maximum {old["sourceReadMaxMs"]}ms -> {new["sourceReadMaxMs"]}ms and send gap {old["sendGapMaxMs"]}ms -> {new["sendGapMaxMs"]}ms. This single run supports improvement, not a universal latency guarantee. Both grenade throw regressions plus source equip/unequip reader regression pass. Source and peer trails visible in viewed temporal sheets; audio, fine grip and subjective movement acceptance remain unverified. Rat skins/advanced states, targeting marker, death effects and shared gameplay outcomes incomplete. {videos} videos indexed; full movement and playable co-op remain incomplete.'
write(P/'review-evidence-annotations.json',ann)
p=D/'31-native-devouring-swarm.md';s=p.read_text(encoding='utf-8');heading='## Candidates318–320: trail lifecycle and idle reader performance'
if heading not in s:
 s+=f'''\n\n{heading}

Candidate318 started7 peer trails and verified411 guarded transform updates, but classified3 native end-of-life records as ownership refusals. The failed take and both viewed sheets are preserved, not indexed as a successful case.319 distinguishes native completion from explicit source retirement and retains the last actual refusal reason. It completed7 effects (4 stopped,3 finished), refused0, updated416 and interpolated389 samples with no readback error. Its full test nevertheless failed body continuity.

319 eligibility evidence isolates the failure to mask1 (no semantic sample), with no native movement-clock rejection. At time361479803.1765, poseTime361479798.084459 was235.505105ms after the final semantic sample361479562.579354; the200ms hold correctly refused it. State receipt age was94ms. The root pose was only5ms behind current time despite the normal80ms delay; root-buffer restart was a hypothesis, not proven by that take. Additional root-buffer fields were added in320. Both checkpoints and14 protected files passed. Raw status files were preserved under `swarm-trails-v319/preserved-*` before further testing.

Code inspection found that idle frames searched all816 inventory entries twice for a grenade scene that usually did not exist, then repeated the full search for held-grenade presentation.320 first finds a native grenade scene before resolving the full item. Idle held presentation now checks only slot1, requiring in-range aligned entry, unique equipment membership, exact declaration/component identity, equipped/started agreement and matching double reads. Actual equip/unequip/cook/release scenes retain the full reader. Freshness,200ms semantic hold,250ms leases, replay checks and root interpolation behavior remain unchanged. Per-reader timing is exposed as `sourceReadTimings`.

[320 result](../../work/natural-moveset/swarm-trails-v320/result.json) passes25 checks. Source maximum action read falls from{old['sourceReadMaxMs']}ms to{new['sourceReadMaxMs']}ms; maximum send gap from{old['sendGapMaxMs']}ms to{new['sendGapMaxMs']}ms. All{continuity['samples']} measured body samples retain weight1.0 and zero clock rejection.7 trails start and complete, with{trails['peer']['updated']} guarded transforms and{trails['peer']['interpolated']} interpolated updates, no refusal or readback mismatch. Source `sourceReadFailures=7` still includes normal expired records; this counter is not a classified error metric. Viewed source/peer sheets show summon ribbons and smoke around the rats, then clearing by late recovery. A pillar occludes part of Corvo. No auditory or fine-grip acceptance follows from these images.

[Reader regression receipt](../../work/movement-integration/candidate-swarm-trails-v320/reader-regression-verification.json) records Corvo high throw and Emily low throw,13 checks each, plus native source equip/unequip and crouching checks. Both action recordings were visually inspected and indexed. Native first-person animation, selected held prop, cook mechanism, spark effect, release hide and empty recovery remain functioning in these fixtures. Source projectile/explosion/damage remains local. All regression cleanup and protected-file checks pass. Failed takes remain preserved separately.

Candidate320 DLL `{result['dll']}`; probe `{result['probe']}`. The archive pins source before subsequent Possession edits. The current review library has{videos} recordings; all completion/visual acceptance flags remain false. This improvement does not complete Swarm gameplay, the full moveset, or playable co-op.
'''
 p.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=videos,bookmarks=len(merged),sourceTimings=summary['sourceTimings'],regressions={k:len(v['checks'])for k,v in regressions.items()})))
