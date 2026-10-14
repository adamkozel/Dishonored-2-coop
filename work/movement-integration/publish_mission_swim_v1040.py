from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C33=rd(W/'candidate-npc-combat-v1033/identity.json');C37=rd(W/'candidate-npc-combat-v1037/identity.json');C39=rd(W/'candidate-npc-combat-v1039/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==C39['dll']
names={C33['dll']:'candidate-npc-combat-v1033',C37['dll']:'candidate-npc-combat-v1037',C39['dll']:'candidate-npc-combat-v1039',rd(W/'candidate-npc-combat-v1026/identity.json')['dll']:'candidate-npc-combat-v1026'}
plan=[
 ('corvo-defense-presentation-v1030',True,None),
 ('mission-eotw-v1034',True,'Native developer command campaign campaign/edge_of_the_world/eotw.mapinfo low corvo loads Edge of the World in the isolated host; ~70 s intro (logic5) then gameplay at the harbor dock; census 11164 entities incl. 3 arkClimbable chains, 371 usables, 27 doors. Dunwall fixture restored.'),
 ('native-swim-v1035',True,'Native swimming observed: alternate mode, sole active state0 arkPlayerStateSwimming (1c7dc68), stance7; forward/dive/surface/back inputs stay inside the same state. First-person video inspected.'),
 ('mission-coop-eotw-v1036',True,'Co-op bring-up on Edge of the World: both roles load the mission natively, transport fresh, partner Doppelgangers spawned, client remote Corvo follows host within 0.4 mm while walking; client video shows native remote Corvo on the Karnaca dock.'),
 ('swim-coop-eotw-v1038',False,'Candidate1037: no remote swim presentation; mask 29/31 showed the action stream never started on the mission map.'),
 ('swim-coop-eotw-v1038b',False,'Candidate1037 diagnostic: shadow stream refused on both roles (unavailable_or_busy) - armProbePairReady was restricted to the Dunwall tower map.'),
 ('swim-coop-eotw-v1038c',True,None),('swim-coop-eotw-v1038d',True,None),('swim-coop-eotw-v1038e',True,None),('swim-coop-eotw-v1038f',True,None)]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,note in plan:
    T=N/take;r=rd(T/'result.json') if (T/'result.json').exists() else {}
    if take.startswith('swim-coop-eotw-v1038') and passed:
        assert r.get('passed') and rd(T/'restoration.json')['passed'],take
        sw=rd(T/'swim-results.json')['client'];scope=f'Candidate1039: host native swimming (state22, protocol71) presented on the client remote Corvo body with bank69: {sw["swimming"]}/{sw["samples"]} samples swimWeight>0.5, stroke {sw["moving"]}, dive {sw["sinking"]}, surface {sw["rising"]}. Visual review: client camera placements did not frame the mostly submerged swimmer.'
        summary=dict(swim=sw)
    elif take=='corvo-defense-presentation-v1030':
        scope=rd(T/'verified.json')['scope'];summary=rd(T/'presentation-results.json')
    else:
        assert passed==bool(r.get('passed')),take
        scope=('Passed: ' if passed else 'Failed run: ')+note;summary=dict(failure=r.get('failure'))
    dll=rd(T/'identity.json')['dll'] if (T/'identity.json').exists() else (C39['dll'] if take.endswith(('c','d','e','f')) else C37['dll'])
    cases.append(dict(take=take,passed=passed,scope=scope,candidate=names.get(dll,dll),result=summary))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if f.name.startswith('host') else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Mission fixtures / swimming / {take} / {f.name}',travelling=True,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Raw native recording; see audit scope for inspected sheets.',candidate=names.get(dll,dll),dll=dll,runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
gates=['coin_sync.h','usable_sync.h','checkpoint_sync.h (shared checkpoint/death: Dunwall SLOT82 only)','doppelganger_visual.h','ranged_prop.h','grenade_prop.h','heart_prop.h','trap_prop.h','native_power_fx.h','domino_link_presentation.h / domino_mark_presentation.h / possession_fx_stream.h / swarm_rat_visual_fixture.h (also tied to observer-isolation movement fixture)','guard_link.h / damage_protocol.h (legacy guard experiments)']
wr(W/'mission-swim-audit-v1040.json',dict(at=at,live=dict(candidate='candidate-npc-combat-v1039',dll=C39['dll'],protocol=71,bank='swim-banks-v1037 (69/365)'),cases=cases,complete=False,
  remainingDunwallOnlyGameplayGates=gates,
  outstanding=['Visual acceptance of remote swimming (needs a standing observer vantage over water).','Swim mantle/water exit, underwater-only acting and Emily-as-swimmer runs.','Other mission maps, ladders/chains (3 arkClimbable chains in Edge of the World), windows.','Lift remaining Dunwall-only gameplay gates for map-agnostic play.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p)
it=an['items']['swim_idle_directions'];it['runtime_binding']='partial_native_steam_swimming_corvo_source_mission_map'
it['notes']='1035/1038c-f: native swimming (Edge of the World, via native campaign command) is arkPlayerStateSwimming in the alternate mode table; candidate1039 sends it as action state22 (protocol71) and the receiver presents bank69 authored tread/stroke/up/down blended by replicated horizontal speed and vertical velocity, root and depth network-owned. Four passing runs (~187 swim samples each). Emily-as-swimmer, swim mantle/exit and visual acceptance remain open.'
it.setdefault('evidence',[]).append('../movement-integration/mission-swim-audit-v1040.json')
it2=an['items']['dive_surface_water_exit'];it2['notes']=(it2.get('notes') or '')+' 1038c-f: dive and surface are presented as vertical-velocity blends of the native swimming state (no separate native dive state exists); water exit/ledge mantle not yet bound.'
wr(p,an)
for ident,path in ((C37,W/'candidate-npc-combat-v1037/identity.json'),(C39,W/'candidate-npc-combat-v1039/identity.json')):
    ident.update(runtimeEvidence='../mission-swim-audit-v1040.json')
C37.update(runtimeVerified=False,result='1038/1038b failed: action stream restricted to the Dunwall map');wr(W/'candidate-npc-combat-v1037/identity.json',C37)
C39.update(runtimeVerified=True,scopedSwimPresentationVerified=True,scopedMissionCoopVerified=True,scope='1039 (live): campaign fixture command (1033) + native swimming state22/protocol71/bank69 + map-agnostic action stream; verified 1038c-f in Edge of the World; mission co-op bring-up 1036 on 1033.');wr(W/'candidate-npc-combat-v1039/identity.json',C39)
C33.update(runtimeVerified=True,scopedCampaignFixtureVerified=True,runtimeEvidence='../mission-swim-audit-v1040.json');wr(W/'candidate-npc-combat-v1033/identity.json',C33)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1039 live: missions beyond the Dunwall tower load natively in the isolated lab (campaign command); co-op bring-up verified in Edge of the World; remote native swimming bound (state22, bank69) and verified numerically; defense presentation both roles.'
s['MovementExecutionPriority']['next']='Roadmap stage3: climbable chains/ladders in Edge of the World, swim exit, windows; lift remaining Dunwall-only gameplay gates; visual acceptance captures.'
wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases])))
