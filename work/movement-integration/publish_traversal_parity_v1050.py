from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C42=rd(W/'candidate-npc-combat-v1042/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==C42['dll']
def obs(T,kind):
    p=T/f'{kind}-results.json';return rd(p)['observer'] if p.exists() else None
plan=[
 ('traversal-emily-v1046','Role parity, Emily (client) as source, both modes. Host observer camera video shows remote Emily grab the chain, climb up and down, release and land (sheet inspected). Swim video framed open sea (dock vantage faced away from the lane); swimming numeric only.',dict(swim=False,climb=True)),
 ('traversal-emily-swimview-v1047','Emily swim with the observer at the spawn yawed at the lane and pitched 22 deg down (native look input). Numeric pass; visual not accepted: remote body not visible.',dict(swim=False)),
 ('traversal-emily-swimview-v1048','Diagnostic: observer bridge remote/render position and native view sampled. Projection shows the lane at x=-26.8 runs just beyond a boat moored alongside the dock and the swimmer (root ~-1.9, eye at the waterline, surface ~0 per native1035) was behind it from the dock-level camera. Numeric pass; visual not accepted.',dict(swim=False)),
 ('traversal-emily-swimview-v1049','Emily swim, lane 5.5 m off the dock, observer pitched 50 deg down then held 9 m above the dock edge (coop_observer_camera). Host video shows remote Emily treading at entry, horizontal stroke with lead arm extended, dive/surface and tread again (sheet and crops inspected).',dict(swim=True)),
 ('traversal-corvo-swimview-v1049','Corvo (host) as source with the same overhead framing. Client video shows remote Corvo treading, stroking and surfacing (sheet inspected); faint while submerged during the dive.',dict(swim=True))]
cases=[];videos=rd(N/'runtime-video-manifest.json')
for take,note,visual in plan:
    T=N/take;r=rd(T/'result.json');ident=rd(T/'identity.json');assert r['passed'] and rd(T/'restoration.json')['passed'],take
    assert ident['dll']==C42['dll'],take
    sw,cl=obs(T,'swim'),obs(T,'climb');src='emily' if ident['source']=='client' else 'corvo'
    parts=[]
    if sw:parts.append(f'swim {sw["swimming"]}/{sw["samples"]} samples swimWeight>0.5, stroke {sw["moving"]}, dive {sw["sinking"]}, surface {sw["rising"]}')
    if cl:parts.append(f'climb {cl["climbing"]}/{cl["samples"]} samples climbWeight>0.5, ascend {cl["ascending"]}, descend {cl["descending"]}, {cl["phases"]} cycle phases')
    scope=f'Candidate1042 (confirmed runtime): {src} ({ident["source"]}) native '+' and '.join(k for k in ('swimming state22' if sw else '','chain climbing state23' if cl else '') if k)+f' presented on the {ident["observer"]} remote {src} body with bank70 in Edge of the World; '+'; '.join(parts)+'. '+note
    cases.append(dict(take=take,passed=True,source=ident['source'],sourceCharacter=src,observer=ident['observer'],bank=ident['bank'],dll=ident['dll'],swim=sw,climb=cl,visualAccepted=visual,scope=scope))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';kind=f.name.split('-')[0]
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        isObserver=role==ident['observer'];accepted=isObserver and visual.get(kind,False)
        review=('Observer view of the remote '+src+': '+('sheet inspected, accepted for bounded '+kind+' presentation.' if accepted else 'inspected, remote body not adequately framed; not accepted.' if isObserver and kind in visual else 'raw.')) if isObserver else f'Source first-person view ({src}); raw native recording.'
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Traversal role parity / {kind} / {take} / {f.name}',travelling=True,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate='candidate-npc-combat-v1042',dll=ident['dll'],runtime_test_passed=True,visual_accepted=accepted))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'traversal-parity-audit-v1050.json',dict(at=at,candidate='candidate-npc-combat-v1042',dll=C42['dll'],protocol=72,bank='climb-banks-v1042 (70/369): Corvo FD50D187, Emily 500BEC1D',harnesses=['../natural-moveset/Test-TraversalCoop-v1046.ps1','../natural-moveset/Test-TraversalCoop-v1047.ps1','../natural-moveset/Test-TraversalCoop-v1048.ps1','../natural-moveset/Test-TraversalCoop-v1049.ps1'],cases=cases,
  roleParity=dict(swimming=dict(corvoSource=['swim-coop-eotw-v1038c..f (numeric)','traversal-corvo-swimview-v1049 (visual)'],emilySource=['traversal-emily-v1046','traversal-emily-swimview-v1047/1048/1049 (visual 1049)']),
                  chainClimbing=dict(corvoSource=['climb-coop-eotw-v1043 (visual)'],emilySource=['traversal-emily-v1046 (visual)'])),
  findings=['Confirmed runtime: native swimming eye sits at the waterline (root ~-2.0, view ~-0.1, surface ~0 in native1035), so the faithfully placed remote body is mostly submerged; it is visible from a steep overhead view, not from dock level.','Confirmed runtime: presentation is role-symmetric; the Emily bank70 on the host and Corvo bank70 on the client both drive swim/climb layers from the same protocol72 states.','Harness: Set-ObserverPitch + immediate coop_observer_camera hold after an elevated setviewpos gives a stable overhead vantage (falls ~0.3 m before the hold).'],
  complete=False,
  outstanding=['Swim mantle/water exit (no native exit state bound yet), underwater-only acting.','Chain hand-over-hand acting and grab/release transitions (clips 367/368 reserved); true ladders and top exits in other maps.','Polish (head height at the waterline, stroke cadence vs native speed ~3.5 m/s) deferred to the animator; editable sources kept.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p)
it=an['items']['swim_idle_directions'];it['runtime_binding']='partial_native_steam_swimming_both_roles_mission_map';it['visual_validation']='bounded_actual_game_frames_inspected'
it['notes']='1035/1038c-f/1046/1049: native swimming (Edge of the World, native campaign command) is arkPlayerStateSwimming in the alternate mode table, sent as action state22 (protocol71+); the receiver presents authored tread/stroke/up/down blended by replicated horizontal speed and vertical velocity, root and depth network-owned. Both roles verified as source (Corvo 1038c-f/1049, Emily 1046/1047/1048/1049). Overhead actual-game videos (1049, both roles) show tread, horizontal stroke and dive/surface on the remote body (bounded visual acceptance). Swim mantle/exit and polish remain open.'
it['evidence']=[e for e in it.get('evidence',[])]+['../movement-integration/traversal-parity-audit-v1050.json']
it2=an['items']['ladder_up_down_entry_exit'];it2['runtime_binding']='partial_native_steam_chain_climb_both_roles_mission_map';it2['visual_validation']='bounded_actual_game_frames_inspected'
it2['notes']='1041/1043/1046: native arkClimbable chain climbing in Edge of the World (ordinary interact -> alternate-mode state29 arkPlayerStateClimb) is sent as action state23 (protocol72); the receiver presents bank70 authored ladder-contact ascend/descend cycles advanced by replicated vertical distance. Both roles verified as source with observer-camera videos (Corvo 1043 on client, Emily 1046 on host: grab, climb, descent, release, landing). Chain hand-over-hand acting, entry/exit transitions and true ladders/top exits remain open.'
it2['evidence']=[e for e in it2.get('evidence',[])]+['../movement-integration/traversal-parity-audit-v1050.json']
wr(p,an)
C42.update(scopedClimbPresentationVerified=True,scopedSwimPresentationVerified=True,roleParityVerified='swim+climb both sources (1043/1046/1049)',runtimeEvidence='../traversal-parity-audit-v1050.json');wr(W/'candidate-npc-combat-v1042/identity.json',C42)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1042 live: remote native swimming (state22) and chain climbing (state23) verified for both source roles with bounded actual-game visual acceptance (1043/1046/1049) in Edge of the World; map-agnostic action stream; missions load natively with intro skip.'
s['MovementExecutionPriority']['next']='Roadmap stage3: next unbound items (window enter/exit, swim exit/mantle, lever/button and other interactions, kick/stagger, item switching), lifting remaining Dunwall-only gameplay gates where an item needs a mission map.'
wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['visualAccepted']) for c in cases])))
