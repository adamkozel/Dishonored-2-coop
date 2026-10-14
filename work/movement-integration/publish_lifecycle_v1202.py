from pathlib import Path
import json,datetime,subprocess,sys
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
review=rd(W/'lifecycle-review-v1202.json')  # written after inspecting the contact sheets
T=N/'mission-lifecycle-v1199d';r=rd(T/'result.json');rest=rd(T/'restoration.json');assert r['passed'] and rest['passed']
C=rd(W/'candidate-npc-combat-v1203f/identity.json');case=r['cases'][0]
scope=('Pass: the host alone loaded Edge of the World; the client followed natively (join-follow) and the product controller armed every system; the partner avatar was present on both peers '
       f"(load: host {case['load']['host']['distance']} m / client {case['load']['client']['distance']} m from the remote player) and filmed face to face. After a shared death and native checkpoint restart it was restored on both peers "
       f"(host {case['restart']['host']['distance']} m / client {case['restart']['client']['distance']} m) and filmed again.")
videos=rd(N/'runtime-video-manifest.json')
for f in sorted(T.glob('*.mp4')):
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
    path=f'{T.name}/{f.name}';role='host' if '-host' in f.name else 'client';phase=f.name.split('-')[0]
    rv=review[f.name]
    videos=[x for x in videos if x['video']!=path]
    videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Lifecycle / partner after {"map load" if phase=="load" else "checkpoint restart"} / {T.name} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
        review=rv['review'],candidate='candidate-npc-combat-v1203f',dll=C['dll'],runtime_test_passed=True,visual_accepted=rv['accepted'],requirements=['map_load_checkpoint_recovery'] if rv['accepted'] else []))
wr(N/'runtime-video-manifest.json',videos)
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'lifecycle-audit-v1202.json',dict(at=at,candidate='candidate-npc-combat-v1203f',failedTakes=[dict(take='../natural-moveset/mission-lifecycle-v1199',candidate='candidate-npc-combat-v1193',failure='harness: commanded the host while it loaded (receipt timeout); recovered with Recover-InterruptedTake'),dict(take='../natural-moveset/mission-lifecycle-v1199c',candidate='candidate-npc-combat-v1203e',failure='restart presence passed (24 s); facing at re-derived restart spots left a 1.44 m collision/geometry offset')],dll=C['dll'],harness='../natural-moveset/Test-LifecycleRecovery-v1199.ps1',case=dict(take=f'../natural-moveset/{T.name}',scope=scope,rows=case),review=review,
  supporting=['mission-renewal-audit-v1161.json (renewal of avatar/doors/coins/usables/NPC stream across two shared restarts, both roles dying)','product-controller-audit-v1185.json (product controller keeps everything armed across two shared restarts)','join-follow-audit-v1192.json (native follow load)'],
  findings=['Confirmed runtime: the partner avatar is re-created for the new world after a native map load (join-follow) and after a shared checkpoint restart, on both peers, at the remote player position.']+review.get('findings',[]),
  complete=False,outstanding=['Mission-to-mission transition (stage 6, needs a real mission end).','Manual save/load selection and disconnect/rejoin mid-mission.']))
p=N/'review-evidence-annotations.json';an=rd(p);it=an['items']['map_load_checkpoint_recovery']
it['runtime_binding']='partial_native_steam_map_load_and_checkpoint_restart_both_peers'
it['notes']=(it.get('notes') or '')+' 1199d (candidate 1203f): after a native map load (client follows the host mission) and after a shared death + native checkpoint restart, the product controller re-arms and the partner avatar is re-created at the remote player on both peers; filmed face to face from both windows. Supporting: 1160 renewal, 1183 product controller. Mission-to-mission transitions, manual save/load and rejoin remain open.'
it.setdefault('evidence',[]).append('../movement-integration/lifecycle-audit-v1202.json');wr(p,an)
C.update(runtimeVerified=True,scopedLifecycleRecoveryVerified=True,lifecycleEvidence='../lifecycle-audit-v1202.json',result='1199d pass: partner presentation live 1.7 s after a native map load and 2.9 s after a shared restart on both peers; host queue max 138 ms; usable/door hello reduction and pose-timeline rebase active (send tally: usable sync was the reliable flooder, peak 360 KB/s while binding).');wr(W/'candidate-npc-combat-v1203f/identity.json',C)
out=subprocess.run([sys.executable,'-X','utf8',str(N/'build_review_catalog.py')],cwd=N,capture_output=True,text=True);print(out.stdout[-300:],out.stderr[-500:]);assert out.returncode==0
print('ok')
