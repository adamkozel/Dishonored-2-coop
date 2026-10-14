from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';REL=R/'work/release'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1188/identity.json')
cases=[]
for take,passed,scope in (('mission-join-follow-v1189',False,'Functional pass (client loaded Edge of the World once on the host\'s published mission, same map on both peers) but flagged "Recording failed": ffmpeg returned non-zero across the native map load. Both recordings are intact (~150 s); the client video shows Dunwall, the Edge of the World loading screen, then Emily at the docks. Lab restored.'),
                          ('mission-join-follow-v1191',True,'Pass: host published campaign/edge_of_the_world/eotw.mapinfo (sent 15); the client on Dunwall loaded it natively once (campaign ... low emily, loads 1, received 14, refused 0) and reached same_mission. Harness now accepts recordings >=140 s regardless of the ffmpeg exit code. Lab restored.')):
    T=N/take;r=rd(T/'result.json');assert r['passed']==passed and rd(T/'restoration.json')['passed'],take
    for v in ('follow-host.mp4','follow-client.mp4'):assert (T/v).stat().st_size>1_000_000,(take,v)
    cases.append(dict(take=take,passed=passed,scope=scope,failure=r.get('failure'),videos=[f'../natural-moveset/{take}/follow-host.mp4',f'../natural-moveset/{take}/follow-client.mp4']))
boot=rd(REL/'product-boot-v1187/result.json')
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'join-follow-audit-v1192.json',dict(at=at,candidate='candidate-npc-combat-v1188',dll=C['dll'],cases=cases,
  productBoot=dict(take='../release/product-boot-v1187',passed=boot['passed'],failure=boot['failure'],product=boot['rows']['product'],dataRemoved=boot['rows']['dataRemoved'],closed=boot['rows']['closed'],
    finding='Confirmed runtime: a standalone process from the lab game copy, with no lab environment and a temporary d2coop/d2coop.cfg, activated product mode by itself (status in d2coop, partner assets detected, controller waiting_partner) and was cleaned up afterwards. Failed only on Steam readiness: Steam interfaces were acquired lazily on host/join, so the GameLobbyJoinRequested callback was not registered at boot (an in-game Steam Join/accepted invite would be missed), and the lab loopback listener still opened in product mode. Fixed in the next candidate.'),
  findings=['Confirmed runtime (lab): join-follow - a joiner on a different map loads the host\'s mission natively with the game\'s own campaign command and converges on the same map.','Product-mode activation works outside the lab environment; Steam acquisition at boot and loopback suppression are required (next candidate).'],
  complete=False,outstanding=['Rerun the product-mode boot with the Steam-at-boot fix.','Two-account online validation (user).','In-game UI; longer soak; stage 6 mission transition.']))
C.update(runtimeVerified=True,scopedJoinFollowVerified=True,runtimeEvidence='../join-follow-audit-v1192.json');wr(W/'candidate-npc-combat-v1188/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1188 live: join-follow verified (client loads the host mission natively, same map). Product-mode boot from a standalone process activates but Steam was not acquired at boot (join callback unregistered) and the loopback listener opened; fixing in the next candidate.'
s['MovementExecutionPriority']['next']='Candidate with Steam-at-boot + no loopback in product mode; rerun product boot; release r4; control soak; two-account validation (user); in-game UI; stage 6 mission transition.'
wr(R/'work/current-task-state.json',s);print('ok')
