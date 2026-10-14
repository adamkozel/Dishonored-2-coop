from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';REL=R/'work/release';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1193/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
b0=rd(REL/'product-boot-v1187/result.json');b1=rd(REL/'product-boot-v1194/result.json');assert not b0['passed'] and b1['passed']
pk=rd(REL/'release-install-v1195/result.json');assert pk['passed']
PKG=R/'outputs/release/d2coop-v1193';man=rd(PKG/'manifest.json')
for f in man['files']:assert sha(PKG/'files'/f['path'])==f['sha256'],f['path']
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'product-boot-audit-v1196.json',dict(at=at,candidate='candidate-npc-combat-v1193',dll=C['dll'],
  cases=[dict(take='../release/product-boot-v1187',candidate='candidate-npc-combat-v1188',passed=False,failure=b0['failure'],scope='Standalone process from the lab game copy, no lab environment, temporary d2coop/d2coop.cfg and isolated fs_savepath: product mode activated (status in d2coop, partner assets detected, controller waiting_partner) but Steam interfaces were not acquired (lazy acquisition on host/join, so the join callback was unregistered); the lab loopback listener also opened.'),
         dict(take='../release/product-boot-v1194',candidate='candidate-npc-combat-v1193',passed=True,scope='Same boot on 1193: product mode active, Steam interfaces ready at boot (join callback registered), no UDP 31426 loopback endpoint, no session started by itself; the game reached its native boot screens; process closed and temporary d2coop folder removed; protected files unchanged; lab instances stayed fresh.',online=b1['rows']['online'],product=b1['rows']['product'],loopbackEndpoints=b1['rows']['loopbackEndpoints'])],
  package=dict(path='../../outputs/release/d2coop-v1193',release='r4',files=len(man['files']),readme='README.txt',test='../release/release-install-v1195/result.json',cases=pk['cases']),
  findings=['Confirmed runtime: product mode activates outside the lab environment, acquires the game\'s own Steam interfaces at boot and stays idle (no loopback, no session) until F7 / Steam join.','Release r4 (d2coop-v1193) supersedes r3 (d2coop-v1181, no Steam-at-boot); install/rollback passes all six scratch cases.'],
  complete=False,outstanding=['Two-account online validation (user): F7 host, invite/Join Game, disconnect and reconnect.','Control soak to attribute memory growth; longer soak.','In-game UI/character choice; stage 6 mission transition.']))
(R/'outputs/release/d2coop-v1181/SUPERSEDED.json').write_text(json.dumps(dict(status='superseded',reason='Product mode acquired Steam lazily (an in-game Steam Join/invite before F7 was missed) and opened the lab loopback listener; found by product-boot-v1187. Superseded by d2coop-v1193 (r4).')),encoding='utf-8')
C.update(runtimeVerified=True,scopedProductBootVerified=True,runtimeEvidence='../product-boot-audit-v1196.json');wr(W/'candidate-npc-combat-v1193/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1193 live: product-mode boot verified outside the lab environment (Steam at boot, no loopback, idle until F7/Steam join); release r4 outputs/release/d2coop-v1193 (README, install/rollback scratch-tested); join-follow verified on 1188.'
s['MovementExecutionPriority']['next']='Stage 7: control soak to attribute memory growth; longer soak; in-game UI; two-account validation (user). Stage 6: mission transition.'
wr(R/'work/current-task-state.json',s)
# Docs: chapter 90 and current state.
p=D/'90-release-packaging-and-steam-sessions.md';t=p.read_text(encoding='utf-8')
t=t.replace('# Stage 7: release packaging, native Steam sessions and product mode (candidates 1171–1181)','# Stage 7: release packaging, native Steam sessions and product mode (candidates 1171–1193)')
i=t.index('## What stage 7 still needs')
add='''## Stability soak (1186, audit 1190)

[Audit 1190](../../work/movement-integration/stability-soak-audit-v1190.json). Candidate 1181, Edge of the World, product controller only.
- 20 minutes with movement across the map every 30 s and three shared deaths/restarts (both roles).
- No crash, the transport never went stale, every session was armed at the end and handle counts stayed flat (confirmed runtime).
- Private memory grew about 6–7 MB/min on both peers. Not yet attributed: a control soak without co-op systems must separate the game's own streaming from the mod.
- The NPC transport queue reached about 1 s during teleports and restarts, with no stale state.

## Join-follow (candidate 1188, audit 1192)

[Audit 1192](../../work/movement-integration/join-follow-audit-v1192.json).
- The host publishes its current mission on `D2MF` packets: the `idDeclMapInfo` name from `idCommonLocal+0x2b8`, and chaos from the loaded map name.
- A joiner on another map loads the same mission natively with the game's own `campaign <mapinfo> <low|high> emily`. It does this once per host map, from gameplay or a menu, never while loading or in a cinematic.
- Product mode follows automatically; the lab uses `coop_follow_host on`.

**Runtime:**
- **1191, pass.** The host on Edge of the World published 15 packets. The client on Dunwall received 14, loaded the mission once and reached `same_mission`.
- **1189.** The same functional pass, but flagged *Recording failed*: ffmpeg's exit code across the map load. Both recordings are intact; the client video shows Dunwall, the loading screen, then Emily at the docks. The harness now checks recording duration instead.

## Product-mode boot (candidate 1193, audit 1196)

[Audit 1196](../../work/movement-integration/product-boot-audit-v1196.json). [`Test-ProductBoot`](../../work/release/Test-ProductBoot-v1194.ps1) starts a standalone process from the lab game copy:
- no lab environment;
- a temporary `d2coop\\d2coop.cfg` and partner banks;
- an isolated `fs_savepath`.

The process is closed and the temporary folder removed afterwards.

**1187 (candidate 1188), failed.**
- Product mode activated by itself: status in `d2coop`, partner assets detected, controller `waiting_partner`.
- Steam interfaces were only acquired on host or join, so the `GameLobbyJoinRequested` callback wasn't registered: an in-game Steam *Join Game* or accepted invite before F7 would be missed.
- The one-PC lab loopback listener also opened.

**Candidate 1193 fixes both.**
- Product mode acquires the game's own Steam interfaces at boot, retrying each second.
- It never opens the lab loopback; it waits for a Steam session.

**1194 (candidate 1193), pass.**
- Steam ready at boot, with no UDP 31426 endpoint.
- No session started by itself.
- The game reached its native boot screens.
- Cleanup was complete, the protected files were unchanged and the lab stayed healthy.

**Package r4** (`outputs/release/d2coop-v1193`, [`build_release_v1195.py`](../../work/release/build_release_v1195.py)):
- Contains the same 13 files plus a player `README.txt` (install, F7 host, Steam join, F8 leave, the uninstall, and the fact that online play is not yet validated).
- Install/rollback passes all six scratch cases ([result](../../work/release/release-install-v1195/result.json)).
- r3 (`d2coop-v1181`) is marked superseded.

'''
t=t[:i]+add+t[i:]
old='- **Product-mode boot.** Product mode has not yet run in an installed game (a scratch copy outside the lab).\n'
assert t.count(old)==1;t=t.replace(old,'')
old='- **Native in-game UI** for host/join and character choice, and a sustained stability soak.'
assert t.count(old)==1;t=t.replace(old,'- **Native in-game UI** for host/join and character choice (the host currently plays Corvo).\n- **Soak attribution**: a control soak without co-op systems, then a longer soak.')
p.write_text(t,encoding='utf-8')
p=D/'01-current-state.md';t=p.read_text(encoding='utf-8')
old='Updated 2026-10-14. Live candidate-npc-combat-v1181 ([identity](../../work/movement-integration/candidate-npc-combat-v1181/identity.json)).'
assert t.count(old)==1
t=t.replace(old,'''Updated 2026-10-14. Live candidate-npc-combat-v1193 ([identity](../../work/movement-integration/candidate-npc-combat-v1193/identity.json)). Latest (stage 7):
- Product mode boots outside the lab environment. Steam is acquired at boot, there is no lab loopback, and the game stays idle until F7 or a Steam join (audit 1196).
- A joiner follows the host's mission natively (audit 1192).
- A 20-minute soak passed; memory growth is still unattributed (audit 1190).
- Release r4 is `outputs/release/d2coop-v1193`, with a README and scratch-tested install/rollback ([chapter 90](90-release-packaging-and-steam-sessions.md)).

Open: a control soak, two-account validation (needs the user), in-game UI and the mission transition. Earlier headers below are history.

Previous header (1181, history): Live candidate-npc-combat-v1181 ([identity](../../work/movement-integration/candidate-npc-combat-v1181/identity.json)).''',1)
p.write_text(t,encoding='utf-8');print('ok')
