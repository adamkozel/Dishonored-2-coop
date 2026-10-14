from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1059c/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
plan=[('mission-door-v1066',False,'Door sync armed on Edge of the World without code change: 16 doors catalogued and paired on both peers (session active). Only candidate tried was not opened by the host: entity origin is the hinge, aim hit the frame. Restoration first reported a native inventory read failure; repaired by re-running Restore-LabNoteInventory (note-restoration-repair passed, both inventories match baseline).'),
 ('mission-door-v1067',False,'Yaw sweep: host ordinary open (offset -20 deg) converged natively on the client (revision2 applied). Client close never targeted the opened panel (swung into the room).'),
 ('mission-door-v1068',False,'Wider close sweep (+-90 deg) still could not target the inward-swung panel from outside; host open converged again.'),
 ('mission-door-v1069',True,'Role parity with opens: host opened env_interiors_door_interior_350cm_01_19 (client native door opened, revision applied); client opened env_interiors_door_medium_template_19 (host native door opened). Five other paired doors were already open (state2) on both peers. Dunwall fixture restored.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed,take
    rest=rd(T/'restoration.json');repair=(T/'note-restoration-repair/result.json');assert rest['passed'] or (repair.exists() and rd(repair)['passed']),take
    ident=rd(T/'identity.json');cases.append(dict(take=take,passed=passed,dll=ident['dll'],scope=scope,failure=r.get('failure'),cases=r.get('cases'),pairing=rd(T/'door-pairing.json')))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'World authority / shared doors / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Raw native recording; client close sheet inspected for 1068 (door seen open on the client).',candidate='candidate-npc-combat-v1059c',dll=ident['dll'],runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-door-audit-v1070.json',dict(at=at,candidate='candidate-npc-combat-v1059c',dll=C['dll'],harnesses=[f'../natural-moveset/Test-MissionDoor-v10{n}.ps1' for n in (66,67,68,69)],cases=cases,
  findings=['Confirmed runtime: door sync (chapters 56-61) has no map gate and pairs per door, so per-character worlds intersect naturally; 16 doors pair in Edge of the World.','Confirmed runtime: ordinary native door use by either role converges natively on the peer on a mission map.','Harness: door entity origins are hinges; aim needs a yaw sweep; inward-swinging open panels are not targetable from outside.'],
  complete=False,outstanding=['Closing on mission maps (Dunwall-verified), locked doors/keys, door side corrections, checkpoint renewal on missions.','Usable sync (windows/levers) still gated to Dunwall with a 128 cap; EotW has 371 usables - needs the identity-list intersection used for pickups.']))
wr(N/'runtime-video-manifest.json',videos)
C.update(scopedMissionDoorVerified=True,doorEvidence='../mission-door-audit-v1070.json');wr(W/'candidate-npc-combat-v1059c/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1059c live: shared pickups (identity-list intersection) and shared doors verified on Edge of the World with ordinary native use by both roles (1064, 1069); water exits and swim/climb both roles (1056/1057).'
s['MovementExecutionPriority']['next']='Usable sync (windows/levers) on missions via identity-list intersection; checkpoint/shared death on mission maps; Doppelganger visual/props gates; then NPC combat authority on missions (stage 4).'
wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases])))
