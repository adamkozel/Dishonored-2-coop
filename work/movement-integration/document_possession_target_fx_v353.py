from pathlib import Path
import json, shutil, subprocess, sys
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
notes={
 'possession-target-fx-network-v351':'Viewed both temporal sheets. Native source target has bright white/green head particles during aim, occupied vision and native exit. Peer Corvo casts, disappears and returns; target is outside the initial body-centered view. Ten received snapshots and one native FX start/stop establish lifecycle, not particle appearance or target alignment. No audio.',
 'possession-target-fx-view-v352':'Viewed both temporal sheets. Native source bright target particles and exit remain visible. Peer initial camera faces across the hall and misses the source target; fallback observer floor was beside the target and used atan2(0,0) before collision correction. Late view shows native guard. Numeric particle lifecycle passes but particle appearance/alignment is not demonstrated.353 excludes close target placement and recalculates yaw from actual floor coordinates. No audio.'
}
for take,note in notes.items():
 t=P/take;r=json.loads((t/'result.json').read_text(encoding='utf-8-sig'));assert r['passed'] and len(r['checks'])==18
 review=dict(inspected=True,method='Viewed source and peer temporal contact sheets',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes=note)
 (t/'visual-review.json').write_text(json.dumps(review,indent=2))
 subprocess.run([sys.executable,str(W/'record_reviewed_possession_take.py'),take,'--label',take.rsplit('-v',1)[-1]+' target particles','--notes','Release and occupied body lifecycle, post-exit draw/walk and menu cleanup are verified in scoped Dunwall tests.350 target FX reader matches15/15 native records.351/352 native Steam target-particle lifecycle passes18 checks each, but observer framing does not demonstrate appearance or NPC alignment.353 repairs the camera fixture; shared NPC authority and complete movement remain unfinished.'],check=True)
a=W/'candidate-possession-target-fx-v351'
receipts=[]
for take in notes:
 t=P/take;receipts.append(dict(take=take,result=json.loads((t/'result.json').read_text(encoding='utf-8-sig')),network=json.loads((t/'network-fx-summary.json').read_text(encoding='utf-8-sig')),visualAccepted=False))
(a/'runtime-verification-v353.json').write_text(json.dumps(receipts,indent=2))
for p in [R/'outputs/d2-coop-lab/research/possession-target-fx-presentation-asset-install.json',P/'Test-PossessionTargetFxView-v352.ps1',P/'Test-PossessionTargetFxView-v353.ps1']:
 shutil.copy2(p,a/p.name)
d=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=d.read_text();assert '## Target-particle transport runtime,351–353' not in s
s+='''

## Target-particle transport runtime,351–353

[351 passes18 checks](../../work/natural-moveset/possession-target-fx-network-v351/result.json): ten Steam snapshots produce one receiver-native effect, ten owned transform updates and one stop, with no refusal or lost ownership. The live in-process scoped reader maximum was3.3168ms over133 recorded reads, including startup; sustained performance is not established by a maximum. The source-native target ID351 maps only as an opaque key to receiver-native effect245. The production clone owns the cosmetic effect; no native receiver NPC is mutated. Full checkpoint inventories/health and protected14 hashes pass; final native process exit0 consumed.

The [351 visual review](../../work/natural-moveset/possession-target-fx-network-v351/visual-review.json) shows the source effect clearly but the receiver camera frames Corvo, leaving the effect target outside view. [352 passes18 checks](../../work/natural-moveset/possession-target-fx-view-v352/result.json) with one additional start/stop and ten additional transform updates. Its [visual review](../../work/natural-moveset/possession-target-fx-view-v352/visual-review.json) identifies a separate camera-harness defect: a valid floor fallback is adjacent to the target, and yaw was calculated before collision changed the camera position. These numeric passes are not complete visual acceptance.

353 excludes camera points within1.8m of the target and calculates final yaw from the actual accepted floor coordinates. This changes the harness only; DLL351/probe350 and bank52/316 remain unchanged. Production particle retirement on cancellation, stream interruption and native menus still needs targeted evidence. The351 result scope inherited an obsolete sentence saying VFX is unimplemented; its eighteenth check and network receipt document the actual new target-particle implementation.352 corrects that scope. No claims of shared NPC control or gameplay completion follow from these cosmetic effects.
''';d.write_text(s,encoding='utf-8')
print('351/352 reviewed and archived, visual acceptance remains false')
