from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
cands=['v1123','v1123b','v1123c','v1123d','v1123e']
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in cands}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1123e']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()}
plan=[
 ('mission-usable-v1124',False,'1123 census: 269 usables per role, 104 identical toggles on both characters, all 104 paired. Chosen vise sat 2 m from dock workers; the host took damage and aim failed.'),
 ('mission-usable-v1125',False,'Interior sink: the visible "Sink - Open" prompt and the tap ("Use") belong to other usables; no toggle changed.'),
 ('mission-usable-v1126',False,'1123b (reversed toggles): 145 shared toggles paired; census diff after each press found no change (aim landed on the countertop/tap).'),
 ('mission-usable-v1127',False,'Diagnostic: press 150/400/900 ms at three heights on the sink, sync not armed: no change; camera on the countertop.'),
 ('mission-usable-v1128',False,'Window target: one transient context blip retired all 145 client channels (world_input_or_connection_scope_changed) -> 1123c.'),
 ('mission-usable-v1129',False,'1123c: sndctrl_13 from outside reads "Locked from the inside"; inside aims at the frame.'),
 ('mission-usable-v1130',False,'Lateral sweep inside sndctrl_13: no prompt (blinds).'),
 ('mission-usable-v1131',False,'Street trash bin: the teleport stand was outside streamed geometry; the host fell (Game Over). Lab restored.'),
 ('mission-usable-v1132',False,'Apartment cabinet from the floor proven by the door take: no prompt.'),
 ('mission-usable-v1134',False,'Focus-steered (1133 reader): outside focus = script_invisible_window_locked_191; no focus inside.'),
 ('mission-usable-v1135',False,'Discovery sweep: the host focused script_rich_small_gp_window_handle_149 and opened sndctrl_14; the client converged natively (revision 2). The client then found no focus on the open window.'),
 ('mission-usable-v1136',False,'Known stand first: the host opened sndctrl_14 again; the client had no focus at origin heights.'),
 ('mission-usable-v1137',False,'Aims at the live driver entity position: the client still had no focus on the open window.'),
 ('mission-usable-v1138',False,'The host could not refocus its own opened handle either, so this is not client-specific (open sash moves the handle).'),
 ('mission-usable-v1139',False,'The client session arm was refused right after the intro cinematic (initial_catalog_refused) -> harness arm retry; 1123d arms by identity scope.'),
 ('mission-usable-v1140',True,'Pass (1123c): host native use of window_handle_149 opened sndctrl_14 (client converged 370 ms); client native use of window_handle_146 closed sndctrl_11 (host committed, converged 57 ms). The client use happened after the 150 s recording ended.'),
 ('mission-usable-v1141',False,'1123d: the session stopped with unsupported_native_phase during host attempts -> 1123e (item wheel and tutorial modal suspend sessions).'),
 ('mission-usable-v1142',False,'1123e instrumented: sessions active with no suspensions; focused single presses sometimes did not register natively; the client did not drive its toggle.'),
 ('mission-usable-v1143',False,'Press retry while focus holds: both directions converged; the harness waited 90 s for a 150 s recording (Recording failed).'),
 ('mission-usable-v1144',True,'Pass (1123e), recorded: host opens sndctrl_14 (client converges 221 ms), client closes sndctrl_11 (host converges 58 ms); 145 channels paired on both peers; sessions active; Dunwall fixture restored. Client video: window open with outside view and native "Window" prompt, then closed after Emily\'s use.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed,take;assert rd(T/'restoration.json')['passed'],take
    dll=rd(T/'identity.json')['dll'];shared=rd(T/'shared-toggles.json') if (T/'shared-toggles.json').exists() else {}
    cases.append(dict(take=take,passed=passed,candidate=names.get(dll,dll),scope=scope,failure=r.get('failure'),cases=r.get('cases'),sharedToggles=len(shared.get('shared',[]))))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        final=take=='mission-usable-v1144'
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'World authority / shared usables on missions / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
            review=('Client sheet inspected: sndctrl_11 open (outside view, native Window prompt) then closed after the client use.' if final and role=='client' else ('Host sheet inspected: blinds hide the sash; open state confirmed by native census only.' if final else 'Raw native recording.')),
            candidate=names.get(dll,dll),dll=dll,runtime_test_passed=passed,visual_accepted=final and role=='client'))
at=datetime.datetime.now().astimezone().isoformat()
census=rd(N/'usable-census-dunwall-v1123.json')
wr(W/'mission-usable-audit-v1145.json',dict(at=at,candidate='candidate-npc-combat-v1123e',dll=C['v1123e']['dll'],
  harnesses=['../natural-moveset/Test-MissionUsable-v1144.ps1','../natural-moveset/Probe-UseFocus-v1133.ps1'],
  census=dict(dunwall=dict(usables=census['count'],evidence='../natural-moveset/usable-census-dunwall-v1123.json'),edgeOfTheWorld=dict(usablesPerRole=269,sharedTogglesOriginal=104,sharedTogglesWithReversed=145,evidence='../natural-moveset/mission-usable-v1144/usable-census-host.json')),
  focusProbe=dict(evidence='../natural-moveset/use-focus-v1133',sensorInteraction='arkCpntPlayerSensorInteraction+0x80 high dword = focused entity handle (0x3fff.... = none)',playerInteraction='arkCpntPlayerInteraction+0x38 = focused usable component'),
  cases=cases,
  findings=['Confirmed static/runtime: usables are state-list components; a structural two-state toggle family (entries enter 255 / exit i or uniformly reversed, no apply-on-finish, uniform custom prompt, no transition time, flags bit 0 clear, not a vault) replaces the Dunwall name rule; declaration names are per-map and only compared between peers.',
   'Confirmed runtime: Edge of the World has 269 usables per role; 145 toggles are identical on Corvo and Emily and all pair.',
   'Confirmed runtime (1133): windows are operated through a separate *_window_handle_* UsableStatic; native focus is readable at arkCpntPlayerSensorInteraction+0x80 (high dword).',
   'Confirmed runtime (1144): ordinary native use by the host (open) and by the client (close) converges natively on the peer on a mission map.',
   'Robustness found on the way: transient channel context blips pause instead of retiring (1123c); session arm checks identity scope (1123d); item wheel (8) and tutorial modals (12) suspend door/usable/coin sessions (1123e).'],
  complete=False,outstanding=['Levers/cranks with scripted consequences (script_lever_*) are structurally eligible; their script side effects on the replica are unverified (hypothesis: native index request replays the authored state change).','Multi-state devices (typewriters, safes, 5/10-state) and vaults keep their own semantics.','Usable sync across mission checkpoints/reloads (stage 6).','Native press reliability: a focused single press sometimes does not register; harness retries.']))
wr(N/'runtime-video-manifest.json',videos)
C['v1123e'].update(runtimeVerified=True,scopedMissionUsableVerified=True,runtimeEvidence='../mission-usable-audit-v1145.json')
wr(W/'candidate-npc-combat-v1123e/identity.json',C['v1123e'])
for k in cands[:-1]:
    C[k].update(runtimeVerified=(k=='v1123c'),result='intermediate step in the 1123 usable series; see audit',runtimeEvidence='../mission-usable-audit-v1145.json');wr(W/f'candidate-npc-combat-{k}/identity.json',C[k])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1123e live: stage 5 on missions - shared two-state usables (windows/cupboards/sinks; 145 toggles paired on Edge of the World) by ordinary native use from both roles; sessions survive item wheel and tutorial modals; plus stage-4 mission combat/corpse authority from 1113d.'
s['MovementExecutionPriority']['next']='Stage 6: player death and shared checkpoint recovery on missions (usable/door/coin session renewal across mission checkpoints); then scripted levers and multi-state devices; dynamic NPC relevance.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos),[(c['take'],c['passed']) for c in cases if c['passed']])
