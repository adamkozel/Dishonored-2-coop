from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
cands=['v1071','v1074','v1074b','v1074c','v1074d','v1074e','v1074f','v1074g','v1074h','v1074i','v1074j']
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in cands}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1074j']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()}
plan=[
 ('mission-equipment-v1072',False,'1071: native item wheel layout read (Heart top; pistol bullets upper-right; grenade lower-right; Blink bottom; crossbow bolts lower-left; sleep darts upper-left). Opening the wheel retired the action stream on both roles (scope_or_peer_changed).'),
 ('mission-equipment-v1073',False,'Confirmed runtime: the item wheel is native logic state 8 (arkGameLogicStatePowerwheel). coop_movement_resume refused to arm (live_owned_presentation_required); stream retired again.'),
 ('mission-equipment-v1075',False,'1074 (presentation readiness accepts logic8): stream still retired (reason2) - local cinematic metadata unavailable in the wheel.'),
 ('mission-equipment-v1076',False,'1074b: stream survived the wheel opening but source holds failed (stance reader requires logic1) -> native_state_stale.'),
 ('mission-equipment-v1077',False,'1074c: follow stopped during the wheel (unknown term).'),
 ('mission-equipment-v1078',False,'1074d: built without the intended diagnostic edit (identical to 1074c); same failure.'),
 ('mission-equipment-v1079',False,'1074e diagnostic: wheel samples as logic1 with pending8 while opening, which invalidated follow (bits 1|2).'),
 ('mission-equipment-v1080',False,'1074f: follow survived the wheel; source missed 4 sends (no hold) -> stale.'),
 ('mission-equipment-v1081',False,'1074g: hold also for the pending8 transition; still stale (cause not yet visible).'),
 ('mission-equipment-v1083',False,'1074h diagnostic pairBits=0: source stream current yet stale - the action-stream wire validator still required coopWorldPairReady, rejecting packets in both directions during the wheel.'),
 ('mission-equipment-v1084',False,'1074i (validator uses presentation pair): client retired first because the host source sent no holds during its wheel.'),
 ('mission-equipment-v1086',True,'1074j: read-only probe --presentation-hold (1085) showed the player state machine is unreadable inside the wheel while the stance reader succeeds; stance-based hold proof. Stream stays ready on both roles through the wheel (48 holds); observer presents the partner Blink aim afterwards (power variants 2/3).'),
 ('mission-equipment-v1087',True,'1074j: left/right sticks do not drive wheel selection under virtual input (no highlight); stream survives again; Blink presented.'),
 ('mission-equipment-v1088',True,'1074j Corvo source: wheel open/close (47 holds, stream ready), then native D-pad Left quick slot equips the pistol and LT enters native ReloadWeapon; client presents ranged kind1 for 135 samples and attaches the native pistol prop to remote Corvo; client video: sword, neutral during the wheel, pistol drawn, reload with ejected casing, aim.'),
 ('mission-equipment-emily-v1088',True,'1074j Emily source: same sequence; host presents ranged kind1 for 153 samples with the native pistol prop on remote Emily; host video: sword, neutral during her wheel, pistol drawn, reload, aim.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert rd(T/'restoration.json')['passed'],take
    ident=rd(T/'identity.json');dll=ident['dll']
    streams={q:{k:v for k,v in rd(T/f'stream-after-{q}.json').items() if k in ('result','accepted','holdSamples','sourceMisses','unboundRefused')} for q in ('host','client') if (T/f'stream-after-{q}.json').exists()}
    cases.append(dict(take=take,streamSurvivedWheel=passed,candidate=names.get(dll,dll),source=ident.get('source'),scope=scope,streams=streams,observer=rd(T/'equipment-results.json').get('observer') if (T/'equipment-results.json').exists() else None))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';obs=role==ident.get('observer')
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        accepted=obs and take in ('mission-equipment-v1088','mission-equipment-emily-v1088')
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Partner equipment and item wheel / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=('Observer sheet inspected; bounded acceptance.' if accepted else 'Raw native recording.'),candidate=names.get(dll,dll),dll=dll,runtime_test_passed=passed,visual_accepted=accepted,requirements=(['pistol_draw_aim_fire_reload_holster','item_switch_interruptions'] if accepted else [])))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-equipment-audit-v1089.json',dict(at=at,candidate='candidate-npc-combat-v1074j',dll=C['v1074j']['dll'],probe=C['v1074j']['probe'],actionProtocol=73,
  harnesses=['../natural-moveset/Test-MissionEquipment-v1072.ps1','../natural-moveset/Test-MissionEquipment-v1088.ps1','../natural-moveset/Probe-WheelReaders-v1082.ps1','../natural-moveset/Probe-WheelHold-v1085.ps1'],cases=cases,
  findings=['1071: ranged/grenade/heart/trap props, Doppelganger visual and partner power FX no longer require the Dunwall map; the Dunwall observer-isolation fixture is required only for manual fixture requests.','Confirmed runtime: the native item wheel is arkGameLogicStatePowerwheel (logic stack InGame+Powerwheel, logic8; sampled as logic1/pending8 while opening); cinematic metadata is unavailable and the player state machine unreadable inside it, while the stance reader works.','1074j: ongoing presentation (pose receive, follow, bank lease, action stream incl. wire validator) accepts the wheel via coopPresentationReady; the source sends payload-free holds with a stance-based identity proof; starting presentation, world authority and checkpoints still require gameplay.','Confirmed runtime: native D-pad quick slots select items (wheel stick selection is not driven by the virtual controller); partner pistol draw/reload/aim with the native prop is presented on a mission map for both roles.'],
  complete=False,outstanding=['Wheel stick selection under virtual input; crossbow/grenade/heart/trap and powers via quick slots on missions.','Pause/journal menus (logic 2/4) still rely on movement resume (needs a live bank on the arming side).','The stance-based hold also applies in gameplay when the full proof fails (same live player only).']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p)
it=an['items']['item_switch_interruptions'];it['runtime_binding']='partial_native_steam_wheel_interruption_and_quickslot_switch_both_roles_mission_map';it['visual_validation']='bounded_actual_game_frames_inspected'
it['notes']=(it.get('notes') or '')+' 1088 (candidate1074j, Edge of the World, both roles): opening/closing the native item wheel no longer retires partner presentation (protocol73 holds; neutral body during the wheel), and a native D-pad quick-slot switch from sword to pistol is presented with the native pistol prop. Arbitrary-time interruption of other items remains open.'
it.setdefault('evidence',[]).append('../movement-integration/mission-equipment-audit-v1089.json')
it2=an['items']['pistol_draw_aim_fire_reload_holster'];it2['notes']=(it2.get('notes') or '')+' 1088: pistol draw/reload/aim presentation with the native prop also verified on a mission map (Edge of the World) for both roles after the 1071 gate lift.'
it2.setdefault('evidence',[]).append('../movement-integration/mission-equipment-audit-v1089.json')
wr(p,an)
C['v1074j'].update(runtimeVerified=True,scopedWheelPresentationVerified=True,scopedMissionEquipmentVerified=True,runtimeEvidence='../mission-equipment-audit-v1089.json');wr(W/'candidate-npc-combat-v1074j/identity.json',C['v1074j'])
for k in cands[:-1]:
    C[k].update(runtimeVerified=False,result='superseded within the 1071-1074j item-wheel series; see audit',runtimeEvidence='../mission-equipment-audit-v1089.json');wr(W/f'candidate-npc-combat-{k}/identity.json',C[k])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1074j live (protocol73): partner equipment/power presentation runs on mission maps and survives the native item wheel; shared pickups and doors on missions; water exits, swim/climb both roles.'
s['MovementExecutionPriority']['next']='Checkpoint/shared death on mission maps (checkpoint_sync Dunwall-only), usable sync intersection, NPC combat authority on missions (stage 4); remaining unbound stage-3 items.'
wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['streamSurvivedWheel']) for c in cases])))
