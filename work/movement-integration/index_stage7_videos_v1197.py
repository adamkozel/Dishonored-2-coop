from pathlib import Path
import json,datetime,subprocess,sys
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
videos=rd(N/'runtime-video-manifest.json');before=len(videos)
takes=[('mission-product-auto-v1183','candidate-npc-combat-v1181',True,'Product controller / auto-armed session across shared restarts',
        'Pass: only coop_product_auto on was sent; the DLL controller armed all 17 co-op steps on both peers and kept them armed through two shared mission restarts (doors, coins, usables, NPC stream renewed; partner avatar restored 2/2).',{}),
       ('mission-join-follow-v1189','candidate-npc-combat-v1188',False,'Join-follow / client loads host mission natively',
        'Functional pass but harness flagged Recording failed (ffmpeg exit code across the native map load); recordings intact. Client on Dunwall loaded Edge of the World once from the host\'s published mission.',
        {'client':'Client sheet inspected: Dunwall, then the Edge of the World loading screen, then Emily at the docks.'}),
       ('mission-join-follow-v1191','candidate-npc-combat-v1188',True,'Join-follow / client loads host mission natively',
        'Pass: host published campaign/edge_of_the_world/eotw.mapinfo (sent 15); the client on Dunwall loaded it natively once (received 14, refused 0) and reached same_mission.',{})]
for take,cand,passed,label,scope,reviews in takes:
    T=N/take;C=rd(W/cand/'identity.json')
    for f in sorted(T.glob('*.mp4')):
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'{label} / {take} / {f.name}',travelling=role=='client',gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
            review=reviews.get(role,'Raw native recording.'),candidate=cand,dll=C['dll'],runtime_test_passed=passed,visual_accepted=False))
wr(N/'runtime-video-manifest.json',videos)
out=subprocess.run([sys.executable,'-X','utf8',str(N/'build_review_catalog.py')],cwd=N,capture_output=True,text=True)
print(before,'->',len(videos));print(out.stdout[-400:],out.stderr[-800:])
assert out.returncode==0
