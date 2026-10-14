from pathlib import Path
import json,re,hashlib,collections
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
take='domino-native-deselect-v235';t=P/take;r=read(t/'result.json');assert r['passed']
rows=read(t/'native-deselect-diagnostics.json');native=next(x for x in rows if x['source']['targets']==1);peer=next(x for x in rows if len(x['marks']['marks'])==1 and not x['peer']['beams']);counter=collections.Counter()
for line in (t/'native-deselect-scenes.ndjson').read_text().splitlines():
 for scene in json.loads(line).get('instances',[]):
  if scene.get('state')==3 and 'power_phantasmal_cast' in scene.get('asset',''):counter[(scene['subscene'],scene['sceneDurationMs'])]+=1
summary=dict(samples=len(rows),native_first_one_target_ms=native['ms'],peer_first_one_mark_no_beam_ms=peer['ms'],min_root_weight=min(x['movement']['weight']for x in rows),clock_rejections=sorted({x['movement']['clockRejection']['count']for x in rows}),subscenes=[dict(name=k[0],duration_ms=k[1],samples=v)for k,v in counter.items()],native_final_selection_body_mapped=False)
write(t/'native-deselect-summary.json',summary)
p=R/'work/movement-integration/candidate-domino-links-v222/runtime-verification.json';v=read(p);v['nativeTargetDeselect']={'take':take,'result':r,'summary':summary,'scope':'Only selected guard1 and beam removed, guard2 mark incarnation preserved; full root continuity. Final body phase unmapped, candidate237 addresses it.'};write(p,v)
asset=R/'outputs/d2-coop-lab/research/asset-exports/phantasmal-native-v174/game1_patch/generated.decls.midnightscene.midnight.gameplay.player.base.powers.power_phantasmal_cast..midnightscene.decl'
scenes=[]
for chunk in re.split(r'\n\t{3}item\[\d+\] =',asset.read_text())[1:]:
 names=re.findall(r'm_name = "([^"]+)"',chunk);animations=re.findall(r'm_AnimName = "([^"]+)"',chunk);lengths=re.findall(r'm_length = (\d+)',chunk)
 if names and animations:scenes.append(dict(name=names[-1],animations=animations,duration_ms=list(map(int,lengths))))
write(R/'work/movement-integration/domino-native-scene-clip-alias-v237.json',dict(asset=str(asset.relative_to(R)),sha256=hashlib.sha256(asset.read_bytes()).hexdigest().upper(),scenes=scenes,interpretation='Cast_Select and Cast_final_Select use identical native left hand animation and400ms; whole-body logical selection variant21 can share authored support acting. Runtime recovery requires separate proof.'))
p=P/'review-evidence-annotations.json';ann=read(p);item=ann['items']['emily_domino_mark_link_cancel'];item['runtime_binding']='native_domino_body_marks_beam_cancel_deselect_partial_verified';item['notes']='Candidate222: standing body/markv218, partner exclusionv212, native paired beamv223/v225, aim cancellationv233 PASS13, target deselectionv235 PASS13. X cancels aim while retaining links; native reselect removes guard1/beam but keeps guard2 mark incarnation. Final_Select400ms observed and unmapped in222; candidate237 adds exact subscene alias to existing body selection; paired runtime pending. Crouch/air/holstered, fire/slide, NPC authority and outcomes remain unfinished.';write(p,ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md';s=p.read_text(encoding='utf-8-sig');heading='## Native target deselection and final body phase, v235–v238'
if heading not in s:
 s+=f'''\n\n{heading}

[v234](../../work/natural-moveset/domino-native-deselect-v234/result.json) stopped before deselection because its shortened18s setup produced9 peer samples; the requirement was10. [v235 final receipt](../../work/natural-moveset/{take}/result.json) passes13 checks after allowing up to23s to collect12 samples. Native LT on already-linked guard1 removes that target and the beam, leaving guard2. The receiver removes the matching cosmetic mark/beam and preserves the surviving mark incarnation. [Summary](../../work/natural-moveset/{take}/native-deselect-summary.json) records{len(rows)} root samples, all weight1/zero clock rejection. Native removal first appears at{native['ms']}ms after input and peer removal at{peer['ms']}ms; sequential polling means these are sampled bounds, not transport latency. Cleanup preserves inventories, health, checkpoints and14 protected hashes.

Paired temporal sheets are inspected and indexed. The lower NPC is absent at the effect position in the receiver recording; this is direct evidence that NPC location synchronization remains unfinished. The effect coordinates match the source, but this is not completed NPC attachment/world authority. The remote body enters aim; its release gesture was missing because the raw native trace uses **Cast_final_Select,400ms**, with13 observed samples. Candidate222 did not map this subscene, so the remote retained aim then faded to idle.

[Exact asset summary](../../work/movement-integration/domino-native-scene-clip-alias-v237.json) shows both Cast_Select and Cast_final_Select use `ply_leftarm_power_phantasmal_select` and400ms. [Candidate237](../../work/movement-integration/candidate-domino-final-v237/identity.json) accepts the exact final subscene as logical selection variant21 under existing state72, armed standing Emily and reciprocal arm/body/equipment guards. It reuses the existing authored whole-body selection support and native finger reference; bank47/289 and protocol44/336bytes are unchanged. The probe also emits native state IDs for audit. Full identity tests include the new alias, wrong duration/ownership/incarnation rejection, a negative unknown-prefix case, and rejection of final selection without state72. Identity, wire and proxy-load checks pass. [v238 harness](../../work/natural-moveset/Test-DominoFinalSelect-v238.ps1) requires at least4 strict native final-phase samples, peer selection weight and continuous root before declaring runtime verification. Runtime receipt, not archive identity, decides this result.

The optional diagnostic [v236 harness](../../work/natural-moveset/Test-DominoNativeDeselect-v236.ps1) was prepared whilev235 was still cleaning up; it was **not run** and is not selected by the runner. It preserves invalid transition reads as unknown rather than absence and adds raw failure objects. Thev235 final pass made that retry unnecessary. Candidate222 remains archived for rollback.
'''
 p.write_text(s,encoding='utf-8')
print(json.dumps(summary))
