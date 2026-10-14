from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';REL=R/'work/release'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1178/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
ri=rd(REL/'release-install-v1177b/result.json');os_=rd(REL/'online-session-v1179/result.json');assert ri['passed'] and os_['passed']
h=os_['rows']['host'];c=os_['rows']['client']
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'stage7-release-session-audit-v1180.json',dict(at=at,candidate='candidate-npc-combat-v1178',dll=C['dll'],
  packaging=dict(package='../../outputs/release/d2coop-v1171-r2',superseded='../../outputs/release/d2coop-v1171 (helper H shadowed by the PowerShell alias h; never installed)',
     payload='dinput8.dll, base/game1..4voidmod.{index,resources}, modified base/master.index (exactly the files that differ from an original install; read-only comparison: 147 small shared files byte-identical incl. Dishonored2.exe and steam_api64.dll)',
     test='../release/release-install-v1177b/result.json',cases=ri['cases']),
  onlineSession=dict(test='../release/online-session-v1179/result.json',
     host=dict(phase=h['phase'],result=h['result'],lobbyCreated=h['lobby']!='0',listener=h['listener']!=0,inviteOpened=h['inviteOpened']),
     client=dict(phase=c['phase'],result=c['result'],lobbyEntered=c['lobby']==h['lobby'],ownerIsSelf=c['owner']==c['self']),
     loopbackRecovered=os_['rows']['loopbackRecovered'],
     note='The invite-dialog screenshot shows the user\'s Steam friends list; it stays local evidence and is not published. The dialog remained open on the host lab window after the take (closing it via desktop control was declined by the user).'),
  findings=['Confirmed: the release payload is exactly dinput8.dll + 4 voidmod packages + modified master.index; the installer refuses unsupported executables or a non-original master.index without changes and the rollback restores byte-identical originals (scratch targets only; the Steam install was only read).',
    'Confirmed runtime: through the game\'s own steam_api64 the host creates a friends-only Steam lobby, sets lobby data and rich presence, listens P2P and opens Steam\'s native invite overlay; the second lab instance (same Steam account) enters that lobby and correctly refuses to connect to itself.',
    'The shipped DLL is inert without lab environment variables: a product activation path (no lab policy/input/command file; host/join through Steam UI) is still required.'],
  complete=False,outstanding=['Two-account online validation (host/join/invite/disconnect/reconnect) - requires a second Steam account and the user.','Product activation path in the proxy (no lab environment) and in-game host trigger; skip the invite dialog in automated lab hosting.','Native in-game UI for host/join/character choice; sustained stability soak.']))
C.update(runtimeVerified=True,scopedOnlineLobbyVerified=True,runtimeEvidence='../stage7-release-session-audit-v1180.json');wr(W/'candidate-npc-combat-v1178/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1178 live: stage 7 started - hash-guarded release package with byte-identical rollback (scratch-tested) and native Steam lobby/rich presence/invite/P2P session flow (host verified; join blocked only by the single test account).'
s['MovementExecutionPriority']['next']='Stage 7: product activation path (no lab env), in-game host trigger, stability soak; two-account validation needs the user; stage 6 mission transition needs a real mission end.'
wr(R/'work/current-task-state.json',s);print('ok')
