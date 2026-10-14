from pathlib import Path
import json,datetime,hashlib,subprocess,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();audit=rd(W/'continuous-recovery-audit-v784.json');assert audit['passed']
A=W/'candidate-death-barrier-v782';pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']==audit['candidate']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert (A/'src'/p.name).exists() and sha(p)==sha(A/'src'/p.name),p
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
cases=[(778,'01-host-reload',False),(780,'01-host-reload',True),(780,'02-client-reload',True),(780,'03-corvo-death',False)]+[(783,c['id'],True) for c in audit['cases']]
for version,cycle,ok in cases:
    take=f'native-continuous-recovery-v{version}';root=N/take;f=root/cycle;result=rd(root/'result.json')
    for role in ('host','client'):
        p=f/f'{role}.mp4';sheet=f'{role}-sheet.jpg';assert p.exists() and (f/sheet).exists()
        sheets=[sheet]
        if version==783 and cycle=='03-corvo-death':sheets.append(f'{role}-death-detail.jpg')
        assert all((f/s).exists() for s in sheets)
        observations=['Native loading UI, room return and fresh closet opening visible in successful recovery; corresponding native identity/queue/role assertions are in the runtime receipts.']
        if 'death' in cycle:observations=['Native death screen visible in both views;'+(' paused survivor goes from native Pause to GameOver, then both return through loading and fresh door use.' if 'paused' in cycle else ' dense783 detail sheets show the brief shared death UI before retry; full sheets show loading and resumed door use.' if version==783 else ' retained780 failure stays on GameOver and aborts before retry after source door-session retirement.')]
        if version==778:observations=['Retained778 harness failure: both native loading screens and room return visible; no fresh-use proof. Stale diagnostic retirement baseline and one inventory quantity discrepancy documented; later native cleanup restored the captured quantities.']
        note=' '.join(observations)+' Functional recovery evidence; no character-animation quality acceptance. '+audit['scope']
        review=f/f'{role}-review-v784.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=sheets,observations=observations))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);char='corvo' if role=='host' else 'emily'
        evidence=(f/'result.json') if (f/'result.json').exists() else root/'result.json'
        v=dict(video=rel,kind='game',character=char,label=f'{char.title()} · continuous recovery v{version} · {cycle}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=ok,complete=False,visual_accepted=False,evidence=evidence.relative_to(N).as_posix(),binding_audit='../../work/movement-integration/continuous-recovery-audit-v784.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=ok,scope=note)
        if rel in by:by[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='782/783/784: four consecutive same-map fixed-SLOT82 recoveries pass under original checkpoint arms: reload requested by each player, Corvo death, and Emily death with Corvo paused. Native shared GameOver, survivor health56 and zero input displacement, exactly one load/counterpart per role, fresh six-door catalog and native use after every retry.779 corrected GameOver13 mislabeled as wheel;782 closes pending-death retirement ordering gap reproduced in780.778 diagnostic race and note-quantity cleanup discrepancy retained and resolved. All item names/quantities, health and14 protected files restored.16 new paired recordings reviewed/indexed; complete co-op and movement remain unfinished.'
nextWork='Proceed to shared pickups/inventory and enemy/world authority using exact native APIs and existing movement pipeline. Retain continuous recovery regression. General saves/maps, native session UI, separate-account Steam validation and missing movement/VFX contexts remain; animator polish stays deferred.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
entry=dict(at=at,candidate=A.name,receipt='work/movement-integration/continuous-recovery-audit-v784.json',details='documentation/handoff-2026-10-11/62-continuous-checkpoint-death-recovery.md',runtimeVerified=True,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=audit['scope'])
state['ContinuousRecoveryContinuation']=entry;state['DoorContinuation'].update(entry);state['DoorContinuation']['worldSynchronizedScope']=audit['scope'];state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_continuous_manual_and_death_checkpoint_recovery',notes=latest);item['evidence']=list(dict.fromkeys(item.get('evidence',[])+['native-continuous-recovery-v783/result.json']));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../continuous-recovery-audit-v784.json',runtimeScope=audit['scope']);wr(A/'identity.json',pin)
for name in ('Test-ContinuousRecovery-v778.ps1','Restore-LabNoteInventory-v779.ps1'):shutil.copy2(N/name,A/name)
p=D/'62-continuous-checkpoint-death-recovery.md';t=p.read_text();t=t.replace('Runtime result783: pending. Do not mark complete until the audit and paired recordings have been inspected.','[Run783](../../work/natural-moveset/native-continuous-recovery-v783/result.json) passes all four cycles and five grouped checks. [Independent audit784](../../work/movement-integration/continuous-recovery-audit-v784.json) rechecks the underlying native observations. Both survivors retain health56 and input displacement0; four native loads per role complete with four fresh catalog renewals and one avatar per role per cycle. Complete inventory names/quantities and player/counterpart health56/40 match the initial baseline after cleanup. All14 protected save/settings hashes remain unchanged.\n\nAll eight successful recordings were inspected through contact sheets; the Corvo-death pair also has dense7–11second sheets showing the brief native GameOver UI. Eight earlier recordings from retained778/780 evidence were also reviewed and indexed, for16 additions to the dashboard. The initial broad Corvo-death sheets skip over the brief menu; use the detail sheets or video for that visual claim.\n\nLive DLL SHA-256: `'+pin['dll']+'`; probe: `'+pin['probe']+'`. All archived source files match the live source tree. Prior769/779 binaries remain preserved.')
p.write_text(t)
p=D/'README.md';t=p.read_text();line='- [Continuous shared checkpoints and native death/retry](62-continuous-checkpoint-death-recovery.md)'
if line not in t:p.write_text(t.rstrip()+'\n\n'+line+'\n')
shutil.copy2(R/'outputs/d2-coop-lab/research/native-sword-setup-latest.json',W/'native-sword-setup-v784.json')
bookmarks=rd(R/'work/handoff-project-bookmarks.json');add=rd(R/'work/handoff-project-bookmarks-recovery-v784.json')
lookup={(x['address'],x.get('category','')):x for x in bookmarks}
for x in add:
    key=(x['address'],x.get('category',''))
    if key in lookup:lookup[key].update(x)
    else:bookmarks.append(x)
wr(R/'work/handoff-project-bookmarks.json',bookmarks)
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),candidate=pin['dll'],next=nextWork)))
