from pathlib import Path
import json,collections
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
take='domino-native-cancel-v233';t=P/take;r=read(t/'result.json');assert r['passed']
rows=read(t/'native-cancel-diagnostics.json');counter=collections.Counter()
for line in (t/'native-cancel-scenes.ndjson').read_text().splitlines():
 for scene in json.loads(line).get('instances',[]):
  if scene.get('state')==3 and 'power_phantasmal_cast' in scene.get('asset',''):counter[(scene['subscene'],scene['sceneDurationMs'])]+=1
summary=dict(samples=len(rows),first_ms=rows[0]['ms'],last_ms=rows[-1]['ms'],min_body_weight=min(x['movement']['weight']for x in rows),max_cast_weight=max(x['movement']['powerWeight']for x in rows),native_targets=sorted({x['source']['targets']for x in rows}),native_connections=sorted({len(x['source']['connections'])for x in rows}),peer_marks=sorted({len(x['peerMarks']['marks'])for x in rows}),peer_beams=sorted({len(x['peer']['beams'])for x in rows}),subscenes=[dict(name=k[0],duration_ms=k[1],samples=v)for k,v in counter.items()])
write(t/'native-cancel-summary.json',summary)
p=R/'work/movement-integration/candidate-domino-links-v222/runtime-verification.json';v=read(p);v['nativeAimCancel']={'take':take,'result':r,'summary':summary,'scope':'Native X while aiming cancels gesture and state72; retains established links. Peer matches and recovers full body. Native target deselection is separate.'};write(p,v)
p=P/'review-evidence-annotations.json';ann=read(p);item=ann['items']['emily_domino_mark_link_cancel'];item['runtime_binding']='native_domino_body_marks_beam_aim_cancel_partial_verified';item['notes']='Candidate222: standing body/mark flowv218, partner exclusionv212, strict beam readerv221, paired native Steam cosmetic beam+2marksv223/v225. Native X aim cancellationv233 PASS13 preserves established source/peer links and recovers full-weight idle; separate peer stop cleans cosmetics without touching source links. Native target deselection/final casts, crouch/air/holstered/interruptions, NPC authority and damage remain unfinished.';write(p,ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md';s=p.read_text(encoding='utf-8-sig');heading='## Native aim-cancel behavior verified, v233'
if heading not in s:
 s+=f'''\n\n{heading}

[v232 paired diagnostics](../../work/natural-moveset/domino-native-cancel-v232/native-cancel-diagnostics.json) resolve the initial hypothesis: native X while LT is held cancels the aiming state72 and gesture but **preserves established target links**. Eleven valid source/receiver snapshots from820–3506ms retain two selected targets, two directed native connections, two marks and one visible beam. The peer retains full body weight1 and zero cast weight in idle. Its retained effects were correct native behavior; forcing link deletion would have diverged from the source. The old failed assertion is retained.

[Independent v233 final receipt](../../work/natural-moveset/{take}/result.json) passes13 checks with the corrected native expectation and a ceiling-facing aim that avoids an interactable object. [Summary](../../work/natural-moveset/{take}/native-cancel-summary.json) records{summary['samples']} paired snapshots from{summary['first_ms']} to{summary['last_ms']}ms, minimum body weight{summary['min_body_weight']}, maximum cast weight{summary['max_cast_weight']}, unchanged native target count and peer FX incarnations. Native aim enters Cast_In/Cast_Loop; X exits without the ordinary LT-release Cast_Select_out. Current body interruption recovery handles this while retaining its root stream. The receiver bank is then stopped separately; cosmetic FX retire while source native links persist. Native inventory, health, both checkpoints and14 protected file hashes pass.

Both actual-game recordings and temporal sheets were inspected and indexed. Source hand and remote body lower while the established beam remains; independent receiver presentation stop removes only peer effects. This establishes scoped standing armed aim-cancel behavior, not complete Domino, final animation/grip acceptance or synchronized NPC outcomes. Native target deselection is the next distinct test. [Harnessv234](../../work/natural-moveset/Test-DominoNativeDeselect-v234.ps1) re-aims through native controls at the already-linked lower guard and checks that only his mark/beam retire, preserving the other mark incarnation and body stream.
'''
 p.write_text(s,encoding='utf-8')
print(json.dumps(summary))
