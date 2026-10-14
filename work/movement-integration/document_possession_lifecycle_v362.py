"""Record inspected lifecycle evidence; retain failures and bounded scope."""
from pathlib import Path
import json,subprocess,sys,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
notes={
 'possession-target-fx-receiver-stop-v359':'Viewed both temporal sheets. Peer halo is visible near1s and absent after receiver stream stop, while the source retains its native aim/halo. Four late packets are rejected after disarm. No implicit rearm. This verifies cosmetic retirement, not shared NPC simulation. No audio.',
 'possession-target-fx-source-pause-v360':'Viewed both temporal sheets. Source native pause menu appears, peer halo retires, stream remains disarmed. The peer halo has no aligned guard in this take: native NPC positions are still independent. Cleanup incorrectly sent a new X+LT cast after the menu had already cancelled aim;360 waits for native host entry before normal exit and restores the checkpoint.362 removes that unnecessary input. No audio.',
 'possession-target-fx-receiver-pause-v360':'Viewed both temporal sheets. Emily native pause menu is visible around1.7/2.2s; peer halo is absent after resume while source native aim continues until cancellation. This verifies retirement across native pause/resume, not shared NPC outcomes or complete power presentation. No audio.',
 'possession-target-fx-source-pause-v362':'Viewed both temporal sheets at0.7/1.1/1.7/2.2/3/5s. Peer halo/streaks appear over the guard and clear after Corvo opens the native pause menu. Source returns to normal native view without the cleanup-induced fresh cast of360. Exact before-cleanup state is default Running0 with no UsePower63, so cleanup sends neutral only. No implicit stream rearm. Bookshelf pillar obscures some body details; no audio.'
}
latest='Target-particle cancellation358, receiver-stop359, receiver-pause360 and corrected source-pause362 each pass7 lifecycle/protection checks. Reviewed game recordings show retirement. Native NPCs still simulate independently, so target position alignment is not general shared-world support. Crouched/holstered Possession contexts are under investigation; complete movement, world authority and playable co-op remain unfinished.'
for take,note in notes.items():
 t=P/take;r=json.loads((t/'result.json').read_text(encoding='utf-8-sig'));assert r['passed']
 assert all((t/(x+'-sheet.jpg')).is_file() for x in ('peer','source'))
 review=dict(inspected=True,method='Viewed paired game temporal contact sheets',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,target_particle_retirement_accepted=True,notes=note)
 (t/'visual-review.json').write_text(json.dumps(review,indent=2))
 subprocess.run([sys.executable,str(W/'record_reviewed_possession_take.py'),take,'--label',take.replace('possession-target-fx-',''),'--notes',latest],check=True)
archive=W/'candidate-possession-fx-probe-v355'
(archive/'runtime-verification-v362.json').write_text(json.dumps({take:json.loads((P/take/'result.json').read_text(encoding='utf-8-sig')) for take in notes},indent=2))
shutil.copy2(P/'Test-PossessionFxLifecycle-v362.ps1',archive/'Test-PossessionFxLifecycle-v362.ps1')
d=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=d.read_text();heading='## Target effect interruption and native cleanup,359–362'
assert heading not in s
s+='\n\n'+heading+'''

[Receiver stream stop359](../../work/natural-moveset/possession-target-fx-receiver-stop-v359/result.json), [receiver pause360](../../work/natural-moveset/possession-target-fx-receiver-pause-v360/result.json) and [corrected source pause362](../../work/natural-moveset/possession-target-fx-source-pause-v362/result.json) each pass7 checks. All paired recordings were inspected. One owned particle retires, stays absent throughout the settling interval and does not implicitly rearm the stopped stream. Source native normal mode, both complete checkpoint inventories/health, and protected14 save/settings hashes restore. Every process final exit was consumed before the next native operation.

The receiver-stop test intentionally retains source aim while stopping the peer; four late snapshots are rejected after disarm. The source-pause362 recording shows the native menu and retirement on the peer, then normal source recovery without another cast. [Source-pause360](../../work/natural-moveset/possession-target-fx-source-pause-v360/result.json) also passes, but its test cleanup unnecessarily sent X+LT after native pause had already cancelled the aim, starting a new cast.360 safely waited for occupied stance18 before issuing native exit.362 instead reads the native state and sends cancellation only while default mode and exact UsePower63 remain active; otherwise it sends neutral. This is a harness correction, not evidence that pausing automatically casts Possession.

Retained failures: receiver-stop358 used a previously clear camera floor point that was no longer suitable relative to the target and aborted before casting;359 returns to validated measured-floor placement. Source-pause359 successfully retired the effect but failed normal-mode cleanup when the redundant cancellation input started entry26; its failed receipt remains unchanged. No production DLL change was required for these harness corrections.

The source-pause360 peer recording also shows the halo at the replicated source target position without an aligned native guard. Native NPC simulation remains independent; cosmetic effect replication does not imply shared target control. Scoped effect retirement is accepted, while full power appearance, movement completion and gameplay completion remain false. [Preserved runtime receipt](../../work/movement-integration/candidate-possession-fx-probe-v355/runtime-verification-v362.json) binds these checks to DLL351/probe355.
''';d.write_text(s,encoding='utf-8')
print('Lifecycle evidence indexed; no completion promotion')
