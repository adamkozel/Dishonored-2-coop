from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C42=rd(W/'candidate-npc-combat-v1042/identity.json');C56=rd(W/'candidate-npc-combat-v1056/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==C56['dll']
names={C42['dll']:'candidate-npc-combat-v1042',C56['dll']:'candidate-npc-combat-v1056'}
plan=[
 ('swim-exit-corvo-v1051',False,None,'Failed run (retained): dock wall below the spawn is too high for a swim exit; source stayed swimming at the wall (x=-29.93, z=-1.98). Harness then moved to the canal lane.'),
 ('swim-exit-corvo-v1052',True,'swim_walkout','Confirmed runtime: swimming onto the canal steps hands over directly from arkPlayerStateSwimming (alternate mode, stance7) to default-mode arkPlayerStateRunning (1c85628) at y~178, z 0.21 - no mantle state. Observer (client) video: remote Corvo rises upright out of the water and runs up the steps; swim layer fades in ~400 ms, one ~100 ms neutral frame (bank weight 0) at the handover (zoom sheet inspected).'),
 ('swim-exit-boat-corvo-v1053',True,'stream_loss','Confirmed runtime defect on 1042: jump at the moored boat entered default-mode arkPlayerStateMantle (state8, 1c82f68, stance1) but the observer bank weight fell to 0 for the rest of the take (eligibility mask 31 then 29: action stream retired). Harness passed (exit reached); presentation failed.'),
 ('swim-exit-boat-corvo-v1054',True,'mantle_ok','1042: second jump at the dock wall mantled from z -1.43 to 1.26 (rise 2.69 m): native mantle_idle.midnightscene Scene_2, 833 ms, traversal variant4 presented on the client (12 samples); stream stayed ready.'),
 ('swim-exit-boat-corvo-v1054b',None,'interrupted','Interrupted by the user after mission load, before cleanup; restored by Recover-InterruptedTake-v1055.ps1 (recovery.json passed). No test result.'),
 ('swim-exit-boat-corvo-v1054c',True,'mantle_ok','1042 repeat: dock mantle from z -1.43 (rise 2.69 m) presented as variant4; stream ready.'),
 ('swim-exit-deep-corvo-v1054d',True,'stream_loss','1042 reproduction of the defect: mantle straight from swim depth (start z -1.81, goal 1.26, rise 3.07 m, kind2 next6). Source traversal reader failed at stage2 (validTraversalGeometry rise<=3 m), 12 source misses, ground fallback refused all, both roles retired native_state_stale. Confirmed runtime cause.'),
 ('swim-exit-deep-corvo-v1057',True,'fixed','Candidate1056: same deep exit (rise 3.07 m) - reader stage0, traversal variant5 (high mantle into sneak) presented on the client for 11 samples, stream ready, zero source misses. Client video: remote Corvo climbs out with hands on the dock edge and ends crouched (sheet inspected).'),
 ('swim-exit-deep-emily-v1057',True,'fixed','Candidate1056, Emily source: deep exit (rise 3.15 m) - variant4 presented on the host for 11 samples, stream ready; one transition-frame source miss became a payload-free state24 hold instead of a refusal. Host video: remote Emily climbs out with hands on the dock edge and stands (sheet inspected).')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,kind,scope in plan:
    T=N/take;ident=rd(T/'identity.json') if (T/'identity.json').exists() else {}
    if passed is not None:
        r=rd(T/'result.json');assert bool(r.get('passed'))==passed or kind=='stream_loss' or take.endswith('v1051'),take
        assert rd(T/'restoration.json')['passed'],take
    else:assert rd(T/'recovery.json')['passed']
    dll=ident.get('dll');res=rd(T/'exit-results.json') if (T/'exit-results.json').exists() else None
    streams={r:{k:v for k,v in rd(T/f'stream-after-exit-{r}.json').items() if k in ('result','sourceMisses','unboundRefused','holdSamples')} for r in ('host','client') if (T/f'stream-after-exit-{r}.json').exists()}
    cases.append(dict(take=take,harnessPassed=passed,kind=kind,source=ident.get('source'),candidate=names.get(dll,dll),scope=scope,exit=res,streams=streams))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';observer=ident.get('observer')
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        accepted=role==observer and kind in ('swim_walkout','fixed')
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Water exit / {take} / {f.name}',travelling=True,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=('Observer view inspected; bounded acceptance.' if accepted else 'Observer view retained as defect evidence.' if role==observer else 'Source first-person view; raw.'),candidate=names.get(dll,dll),dll=dll,runtime_test_passed=kind in ('swim_walkout','mantle_ok','fixed'),visual_accepted=accepted,requirements=['dive_surface_water_exit']))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'water-exit-audit-v1058.json',dict(at=at,candidate='candidate-npc-combat-v1056',dll=C56['dll'],probe=C56['probe'],protocol=73,bank='climb-banks-v1042 (70/369) unchanged',harnesses=['../natural-moveset/Test-SwimExit-v1051.ps1','../natural-moveset/Test-SwimExit-v1052.ps1','../natural-moveset/Test-SwimExit-v1053.ps1','../natural-moveset/Test-SwimExit-v1054.ps1','../natural-moveset/Recover-InterruptedTake-v1055.ps1'],cases=cases,
  findings=['Confirmed static: native assets include swim_mantle.playerstateswimmantle (arkPlayerStateSwimMantle) and common ply_body_generic_swim_{idle,n,s,e,w,neutral} body animations; neither appeared in these exits.','Confirmed runtime: water exits use (a) a direct swimming->default running handover on steps and (b) the ordinary default-mode high mantle (state8, mantle_idle/mantle_sneak scenes, 833 ms) onto ledges; from swim depth the rise reaches 3.07-3.15 m.','Confirmed runtime (1042 defect): any native state the source cannot classify for >250 ms while not on default ground starves the action stream into permanent native_state_stale retirement on both roles.','Candidate1056: rise bound 4 m; protocol73 payload-free state24 hold keeps the stream alive through unclassified states (receiver presents nothing authored).'],
  complete=False,outstanding=['Hold path exercised only by one natural transition frame (Emily 1057); a sustained unclassified-state fixture (e.g. slope Slipping) is still needed.','arkPlayerStateSwimMantle not yet observed natively; native swim body animations are an option for later fidelity work.','Mantle onto the moored boat (1053 geometry) not re-run on 1056.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p);it=an['items']['dive_surface_water_exit']
it['runtime_binding']='partial_native_steam_water_exit_both_roles_mission_map';it['visual_validation']='bounded_actual_game_frames_inspected'
it['notes']='Dive/surface are vertical-velocity blends of native swimming (1038c-f, 1049 both roles). Water exits (1052-1057): steps hand over swimming->running (remote body rises and runs up the steps, 1052); ledge exits use the native high mantle (state8, mantle_idle/sneak, 833 ms) presented as traversal variant4/5 - candidate1056 raised the mantle rise bound to 4 m (swim-depth exits rise ~3.1 m, which retired the action stream on 1042) and adds a protocol73 hold so unclassified states no longer kill the stream. Both roles verified with observer videos (1057). Native arkPlayerStateSwimMantle not yet observed; wading/exit polish deferred.'
it.setdefault('evidence',[]).append('../movement-integration/water-exit-audit-v1058.json');wr(p,an)
C56.update(runtimeVerified=True,scopedWaterExitVerified=True,runtimeEvidence='../water-exit-audit-v1058.json');wr(W/'candidate-npc-combat-v1056/identity.json',C56)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1056 live (protocol73): water exits bound for both roles (steps walk-out, native high mantle from swim depth); action-stream hold prevents permanent retirement on unclassified native states; swimming/chain climbing both roles with visual acceptance (1043/1046/1049/1057).'
s['MovementExecutionPriority']['next']='Lift remaining Dunwall-only gameplay gates (coin/usable/checkpoint/NPC/combat/props) so the existing stage-4/5 work runs on mission maps; then remaining unbound stage-3 items (window enter/exit, levers, item switching, kick/stagger).'
wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['kind']) for c in cases])))
