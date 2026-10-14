from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1091/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
T0=N/'mission-npc-census-v1090';T=N/'mission-npc-stream-v1092'
assert rd(T0/'result.json')['passed'] and rd(T0/'restoration.json')['passed'] and rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed']
res=rd(T/'npc-results.json')
videos=rd(N/'runtime-video-manifest.json')
for f in sorted(T.glob('*.mp4')):
    path=f'{T.name}/{f.name}';role='host' if '-host' in f.name else 'client'
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
    videos=[x for x in videos if x['video']!=path]
    videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'World authority / shared NPC stream / {T.name} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope='Candidate1091 shared NPC authority on Edge of the World (9 dock NPCs).',review='Raw native recording.',candidate='candidate-npc-combat-v1091',dll=C['dll'],runtime_test_passed=True))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-npc-audit-v1093.json',dict(at=at,candidate='candidate-npc-combat-v1091',dll=C['dll'],harnesses=['../natural-moveset/Test-MissionNpcCensus-v1090.ps1','../natural-moveset/Test-MissionNpcStream-v1092.ps1'],
  census=dict(take='mission-npc-census-v1090',npcs=141,identicalAcrossCharacters=True,uniqueReciprocal=133,scriptedCorpses=22,within20m=5,within40m=17,within80m=34),
  stream=dict(take='mission-npc-stream-v1092',hostReady=res['hostReady'],clientReady=res['clientReady'],clientBodyApplied=res['clientBody'],pairs=res['pairs']),
  findings=['Confirmed runtime: Edge of the World authored NPC sets are identical for Corvo and Emily (141, unlike pickups).','1091: generic NPC authority/combat/partner perception no longer require the Dunwall map (coopNpcPairReady); legacy guard-link experiments still do.','Confirmed runtime: the NPC authority stream binds the 9 eligible dock NPCs within 60 m of the host on both peers; client replicas follow host authority (max deviation 5 mm, including two NPCs that moved 0.1-0.19 m); 2 of 132 unbound NPCs diverged between worlds by up to 2.3 m within 10 s, showing why authority is needed.'],
  complete=False,outstanding=['Eligibility excludes NPCs whose decision/reflex AI is not idle (e.g. scripted walkers such as docks_docker_10); needs AI-flag semantics before widening.','Dynamic relevance (rebinding as players move) instead of a bind-time nearest set.','NPC combat on missions with hostile guards; perception of both players; shared outcomes (stage 4).']))
wr(N/'runtime-video-manifest.json',videos)
C.update(runtimeVerified=True,scopedMissionNpcStreamVerified=True,runtimeEvidence='../mission-npc-audit-v1093.json');wr(W/'candidate-npc-combat-v1091/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1091 live: stage-4 bridge - shared NPC authority stream runs on mission maps (nearest eligible NPCs around the host); partner equipment/item wheel, pickups, doors on missions verified.'
s['MovementExecutionPriority']['next']='Stage 4: NPC eligibility for non-idle AI, dynamic relevance, NPC combat with hostile guards on missions, perception of both players; checkpoint/shared death on missions.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
