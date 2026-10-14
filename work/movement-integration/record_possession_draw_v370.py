from pathlib import Path
import json, subprocess, sys

R=Path(__file__).resolve().parents[2]
W=R/'work/movement-integration'; P=R/'work/natural-moveset'
take='possession-crouch-holstered-network-v370'; T=P/take
read=lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
result=read(T/'result.json'); continuity=read(T/'continuity.json')
refusals=read(T/'source-motion-refusals.json')
assert result['passed'] and len(result['checks'])==13
assert continuity['minimumBodyWeight']==1 and refusals['count']==0
notes=('Viewed paired temporal game sheets at1/2.1/2.3/2.6/3/4.5s. '
       'Corvo remains crouched, extends the left casting arm while the right arm draws the sword, '
       'and returns to armed crouch. Body weight1 in79 samples, no receiver clock rejections '
       'and no source-thread motion refusals. Native first-person remains unchanged. '
       'The remote body is dark and medium-distance; fine grip/anatomy acceptance remains open. '
       'Both complete checkpoint inventories/health and protected14 hashes restored. No audio.')
review=dict(inspected=True,method='Paired temporal game contact sheets',
            evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,
            visual_accepted=False,notes=notes)
(T/'visual-review.json').write_text(json.dumps(review,indent=2))
subprocess.run([sys.executable,str(W/'record_reviewed_possession_take.py'),take,
    '--label','crouched draw/cast370','--notes',
    '370 crouched initially holstered cast passes13 checks, with zero source motion refusals and continuous body. '
    'Successful crouched human transfer is a separate test. Shared NPC gameplay and full visual acceptance remain unfinished.'],check=True)
A=W/'candidate-possession-draw-v370'
(A/'runtime-verification-draw.json').write_text(json.dumps(dict(result=result,continuity=continuity,
    sourceRefusals=refusals,visualReview=review,complete=False),indent=2))
identity=read(A/'identity.json');identity['runtimeVerified']=True
identity['runtimeVerificationScope']='Crouched initially holstered empty-target draw/cast only;13 checks. Fine visual acceptance and shared NPC gameplay remain open.'
identity['scope']=identity['scope'].replace('Runtime pending.','Crouched empty-target draw/cast370 passes13 checks.').replace('Runtime and visual validation pending;','Further contexts and fine visual validation pending;')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
p=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md'
s=p.read_text(); heading='## Crouched four-state candidate verified,370'
assert heading not in s
s+='\n\n'+heading+'\n\n'+(
    '[370 draw/cast](../../work/natural-moveset/possession-crouch-holstered-network-v370/result.json) passes13 checks. '
    'Receiver body weight remains1 in79 samples, no presentation clocks are rejected, and the bounded source-thread diagnostic records zero refusals. '
    'Both source and peer recordings were inspected at six matching times: crouch, concurrent sword draw/left-arm cast, and armed crouch recovery are visible. '
    'Dark medium-distance framing does not establish fine grip acceptance. Full checkpoint health/inventory and protected14 restoration pass. '
    '[Archived receipt](../../work/movement-integration/candidate-possession-draw-v370/runtime-verification-draw.json). '
    'This validates the initially holstered empty-target context; successful crouched transfer and shared NPC authority remain separate requirements.\n')
p.write_text(s)
print('370 draw/cast archived and indexed; completion remains false')
