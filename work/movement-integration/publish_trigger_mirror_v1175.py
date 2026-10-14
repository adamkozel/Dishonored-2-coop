from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1171/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
T=N/'mission-trigger-mirror-v1174';r=rd(T/'result.json');assert r['passed'] and rd(T/'restoration.json')['passed']
scope='Pass, client-originated: the client walked the same trigger route while the host stayed at the docks; all 9 client-local script-trigger events replayed natively in the host world for Corvo (none missing/rejected); native tutorial modal (logic 12) on both peers; host HUD showed the objective update and HOSTILE TERRITORY without moving.'
videos=rd(N/'runtime-video-manifest.json')
for f in sorted(T.glob('*.mp4')):
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
    path=f'{T.name}/{f.name}';role='host' if '-host' in f.name else 'client'
    videos=[x for x in videos if x['video']!=path]
    videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Mission progression / mirrored trigger volumes (client origin) / {T.name} / {f.name}',travelling=role=='client',gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
        review=('Host sheet inspected: Corvo stays at the docks; objective update banner and HOSTILE TERRITORY indicator appear (modal itself fell between sampled frames; logic 12 measured).' if role=='host' else 'Raw native recording.'),
        candidate='candidate-npc-combat-v1171',dll=C['dll'],runtime_test_passed=True,visual_accepted=role=='host'))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-trigger-mirror-audit-v1175.json',dict(at=at,candidate='candidate-npc-combat-v1171',dll=C['dll'],harnesses=['../natural-moveset/Test-TriggerMirror-v1174.ps1'],cases=[dict(take=T.name,passed=True,scope=scope,cases=r['cases'])],
  findings=['Confirmed runtime: trigger mirroring is symmetric; either player\'s scripted trigger crossing advances the partner\'s mission script natively (audits 1173 and 1175).'],complete=False,
  outstanding=['Full mission walk with the mirror on; mission end/level transition.']))
wr(N/'runtime-video-manifest.json',videos)
C.update(clientOriginTriggerMirrorVerified=True,clientOriginEvidence='../mission-trigger-mirror-audit-v1175.json');wr(W/'candidate-npc-combat-v1171/identity.json',C)
print('ok',len(videos))
