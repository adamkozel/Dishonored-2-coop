from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=R/'work/movement-integration/refresh_grenade_handoff_v58.py';s=p.read_text()
s=s.replace('Mesmerize interruptions and eye/target effects, remaining movement and playable co-op beyond movement','Mesmerize eye interruptions, target beams and remaining movement; playable co-op beyond movement')
s=s.replace('Emily standing armed Mesmerize body and cosmetic world/model/body-FX lifecycle pass paired v80/v81. Correctly framed recording inspected; native eye/target effects missing. Interruption and remaining movement work continues; playable co-op incomplete.','Emily standing armed Mesmerize body/world/body-FX and native purple eye pass paired v86. Both streams remain ready. Native pauses retire world in v82; forward walking overlaps casting in v84. Eye walking/interruptions and target beams continue; playable co-op incomplete.')
s=s.replace('includes paired native Steam body and cosmetic world/model/body-FX lifecycle passes in v80/v81, with inspected source and receiver recordings. Native purple eye/target beams and synchronized NPC outcomes remain absent; interruption work is in progress.','includes paired native Steam body, cosmetic model, body particles and purple-eye lifecycle pass v86, with inspected source and receiver recordings. Exact source ID237 drives one eye,197 updates and one stop without refusal; both streams stay ready. Eye reader maximum24.658ms across221 calls still motivates performance work. Native pauses retire the pre-eye model in v82; v84 confirms forward walking during casting. Target beams and synchronized NPC outcomes remain absent; eye interruption work is in progress.')
p.write_text(s)
p=R/'documentation/handoff-2026-10-11/25-native-mesmerize-investigation.md'
with p.open('a',encoding='utf-8') as f:f.write('''

## Native eye network pass v86

[Paired v86](../../work/natural-moveset/mesmerize-eye-network-v86/result.json) passes14 checks with DLL `DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB` and probe `4967F79BFD1F4543D27E3E19782A1D02EF7EF5C9A4700BF6BA56122001BEC06D`. [Eye summary](../../work/natural-moveset/mesmerize-eye-network-v86/eye-network-summary.json) matches source ID237 to one cosmetic-model-owned native eye,197 transform updates, one stop and zero refusals. Both semantic streams remain ready through the take. Source eye sampler completes221 reads with zero failures and maximum24.658ms; this is not a performance-complete claim. The earlier failed v85 evidence remains preserved; a single successful run does not isolate its cause.

[Receiver sheet](../../work/natural-moveset/mesmerize-eye-network-v86/peer-sheet.jpg) and [source sheet](../../work/natural-moveset/mesmerize-eye-network-v86/source-sheet.jpg) were inspected. The receiver shows the whole-body casting gesture with sword retained, the native feather model and purple eye at8/12/20 seconds, and no object at26/32. The source beam is still absent on the receiver. Full visual acceptance remains false. The20-second equipment fixture causes removal; natural expiry and automatic reacquisition remain unverified. Both isolated checkpoints, native inventories/health and protected14 restore. Eye walking/interruption coverage is next, followed by target-beam data and the remaining movement/co-op work.
''')
print('Updated v86 continuation narrative')
