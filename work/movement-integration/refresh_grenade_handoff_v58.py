"""Refresh measured pins without changing any existing video review judgment."""
import json, hashlib, datetime, re, struct
from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work'; P=W/'natural-moveset'; D=R/'documentation/handoff-2026-10-11'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat()
live=sha(W/'asset-game/dinput8.dll'); probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
archives=[a for a in (W/'movement-integration').glob('candidate-*') if (a/'identity.json').exists()]
archive=next((a for a in archives if isinstance((pins:=read(a/'identity.json')),dict) and pins.get('dll')==live and pins.get('probe')==probe),None)
probeArchive=archive
if archive is None:
    # Read-only diagnostic probes may advance independently of the live proxy.
    # Keep each artifact's actual provenance; do not pretend both were deployed together.
    archive=next(a for a in archives if isinstance((pins:=read(a/'identity.json')),dict) and pins.get('dll')==live)
    probeArchive=next(a for a in archives if isinstance((pins:=read(a/'identity.json')),dict) and pins.get('probe')==probe)
checks=[]
for f in read(W/'movement-integration/protected-save-hashes-before-v4.json'):
    actual=sha(Path(f['Path']));checks.append(dict(path=f['Path'],expected=f['Hash'],actual=actual,passed=actual==f['Hash']))
for name,expected in [('Dishonored2.exe','B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'),('steam_api64.dll','71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8')]:
    for folder in [Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2'),W/'asset-game']:
        f=folder/name;actual=sha(f);checks.append(dict(path=str(f),expected=expected,actual=actual,passed=actual==expected))
assert all(c['passed'] for c in checks)
write(W/'handoff-integrity-verification.json',dict(at=now,passed=True,checks=checks,liveProxy=live,probe=probe,candidate=archive.name,probeCandidate=probeArchive.name))
videos=read(P/'runtime-video-manifest.json'); cases={}
for f in P.glob('swarm-*/result.json'):cases[f.parent.name]=read(f)
for pattern in ['grenade-reader-*/result.json','possession-*/result.json','grenade-network-*/result.json','grenade-mechanism-*/result.json','grenade-fx-*/result.json','grenade-interrupt-*/result.json','grenade-resume-*/result.json','stun-network-*/result.json','grenade-crouch-network-*/result.json','grenade-crouch-moving-*/result.json','grenade-crouch-low-*/result.json','grenade-air-network-*/result.json','potion-*/result.json','mesmerize-*/result.json','domino-*/result.json']:
    for f in P.glob(pattern):cases[f.parent.name]=read(f)
state=read(W/'current-task-state.json');m=state['MovementIntegration']
m['GrenadeNetworkContinuation']=dict(at=now,cases=cases,complete=False,scope='Isolated cosmetic presentation in the individual recorded standing/crouched contexts; source alone owns projectile/world/damage',mechanismVerified=['grenade-mechanism-corvo-front-v57b','grenade-mechanism-emily-front-v57b'])
identity=read(archive/'identity.json')
# Derive current pins from the exact archived source and bank headers. Old
# candidates omit these metadata fields; historic37/42/260 defaults silently
# misreported newer live builds in the current-state documentation.
protocolSource=(archive/'src/shadow_action_protocol.h').read_text(encoding='utf-8-sig')
protocol=int(re.search(r'\bversion\s*=\s*(\d+)\s*,\s*size\s*=\s*sizeof\(CoopShadowActionPacket\)',protocolSource)[1])
bankHeaders=[]
bankPaths=list((archive/'banks').glob('*.bin'))
if not bankPaths:
    # Unchanged large banks remain in an earlier immutable candidate archive.
    # Explicit hashes preserve provenance without duplicating the payload.
    for entry in identity.get('banks',[]):
        bank=(R/entry['path']).resolve()
        assert bank.is_relative_to(R.resolve()) and sha(bank)==entry['sha256'],entry
        bankPaths.append(bank)
for bank in bankPaths:
    with bank.open('rb') as stream: header=struct.unpack('<8sIIIIQ',stream.read(32))
    assert header[0]==f'D2MOVE{header[1]:02d}'.encode(),bank
    bankHeaders.append((header[1],header[3]))
assert bankHeaders and len(set(bankHeaders))==1,bankHeaders
bankVersion,clips=bankHeaders[0]
dominoContinuation=read(P/'review-evidence-annotations.json')['items']['emily_domino_mark_link_cancel']['notes']
swarmContinuation=read(P/'review-evidence-annotations.json')['items']['corvo_devouring_swarm_cast']['notes']
m['LatestContinuation'].update(at=now,live=live,liveBuild=live,probe=probe,protocol=protocol,bankVersion=bankVersion,clips=clips,actual_game_videos=len(videos),next='Observe and integrate native Domino, complete remaining movement, and continue toward playable co-op beyond movement',in_progress='Both-source/receiver menu recovery passes v120/v121 with fresh active snapshots. Native aim cancellation passes v123. Native countdown expiry passes v129 while the equipment remains selected; v125 retains its failed invalid-versus-absent assertion. v131 observes all native crouched phases. The v133 candidate appends complete-body context clips without altering the earlier276 payloads. Consult individual final receipts and handoff chapters27/28 for crouched runtime results. Shared NPC gameplay authority, full movement and playable co-op remain incomplete.',complete=False)
m['LatestContinuation']['in_progress']=dominoContinuation+' '+swarmContinuation+' '+read(P/'review-evidence-annotations.json')['items']['corvo_possession_enter_loop_exit']['notes']
m['LatestContinuation']['next']='Validate remote Possession target particles and cleanup, then close remaining movement/power gaps and implement shared NPC/world authority toward playable co-op.'
partner=state.get('PartnerInteractionExclusion',{})
if partner:
    m['LatestContinuation']['next']=partner.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+partner.get('latest','')+' '+partner.get('requirement','')
doppelganger=state.get('DoppelgangerContinuation',{})
if doppelganger:
    m['LatestContinuation']['next']=doppelganger.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+doppelganger.get('latest','')
shadowwalk=state.get('ShadowWalkContinuation',{})
if shadowwalk:
    m['LatestContinuation']['next']=shadowwalk.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+shadowwalk.get('latest','')
unbound=state.get('UnboundPresentationContinuation',{})
if unbound:
    m['LatestContinuation']['next']=unbound.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+unbound.get('latest','')
heart=state.get('HeartContinuation',{})
if heart:
    m['LatestContinuation']['next']=heart.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+heart.get('latest','')
trap=state.get('TrapContinuation',{})
if trap:
    m['LatestContinuation']['next']=trap.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+trap.get('latest','')
choke=state.get('ChokeContinuation',{})
if choke:
    m['LatestContinuation']['next']=choke.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+choke.get('latest','')
lethal=state.get('LethalContinuation',{})
if lethal:
    m['LatestContinuation']['next']=lethal.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+lethal.get('latest','')
combatTarget=state.get('CombatTargetContinuation',{})
if combatTarget:
    m['LatestContinuation']['next']=combatTarget.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+combatTarget.get('latest','')
crouchCombat=state.get('CrouchCombatContinuation',{})
if crouchCombat:
    m['LatestContinuation']['next']=crouchCombat.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+crouchCombat.get('latest','')
kick=state.get('KickContinuation',{})
if kick:
    m['LatestContinuation']['next']=kick.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+kick.get('latest','')
heavy=state.get('HeavyCombatContinuation',{})
if heavy:
    m['LatestContinuation']['next']=heavy.get('next',m['LatestContinuation']['next'])
    m['LatestContinuation']['in_progress']+=' '+heavy.get('latest','')
# Preserve historical summaries, but let the most recent dated work select
# the next action instead of a fixed list position selecting stale work.
continuations=('NpcCombatContinuation','NpcAuthorityContinuation','SharedUsableContinuation','SharedPickupContinuation','ContinuousRecoveryContinuation','DoorContinuation','GaitBatchContinuation','AutomaticCrouchContinuation','ClimbSwimInvestigation','InteractionBatchContinuation','DiagnosticTimingContinuation')
for key in sorted(continuations,key=lambda key:state.get(key,{}).get('at','')):
    continuation=state.get(key,{})
    if continuation:
        m['LatestContinuation']['next']=continuation.get('next',m['LatestContinuation']['next'])
        m['LatestContinuation']['in_progress']+=' '+continuation.get('latest','')
m['LatestContinuation']['probeCandidate']=probeArchive.name
state['UpdatedAt']=now;write(W/'current-task-state.json',state)
f=D/'01-current-state.md';s=f.read_text(encoding='utf-8-sig');a='<!-- GRENADE-LIVE-V55 -->';b='<!-- /GRENADE-LIVE-V55 -->'
section=f'''{a}
## Current measured co-op continuation

Updated {now}. Live DLL `{live}` and probe `{probe}` match [the preserved candidate](../../work/movement-integration/{archive.name}/identity.json). Semantic protocol {protocol}; isolated candidate bank {bankVersion}/{clips}. [All eighteen protected file checks pass](../../work/handoff-integrity-verification.json).

Newest scoped movement work: {state.get('MovementExecutionPriority',{}).get('current','See individual chapters.')} [Target-bearing sword combat](43-target-bearing-sword-combat.md) records the current correction and retained test failures.

Both roles and both standing throw branches pass network-presentation checks. Both roles additionally pass native mechanism and held-effect checks; same-world source/receiver menu recovery cases pass with fresh tokens. [Detailed grenade evidence and failures](20-grenade-stream-development.md), [native stun continuity](21-native-stun-development.md), [crouched grenade contexts](22-crouched-grenade-development.md) and [airborne grenade contexts](23-airborne-grenade-development.md) record their exact scopes. Native source projectile, explosions and world damage remain local.

[Native elixir development](24-native-elixir-investigation.md) confirms both-role health use and normal-gameplay mana use, plus remote walking/stopping continuity through both consumables. Remote drink sound, resource outcome synchronization and broader interruptions remain incomplete. [Mesmerize development](25-native-mesmerize-investigation.md) and [partner/target findings](26-mesmerize-targets-and-partner-exclusion.md) record body/world/eye presentation, walking and pause evidence. Partner exclusion through item deselection passes v95; ordinary guard targeting passes v99. Scoped readers pass v105/v107, with recorded v107 eye/link maxima of 0.3833/0.4515 ms over 219 calls each. Target beam/trail/head presentation passes v107's 14 checks; precise target alignment and shared NPC gameplay remain incomplete. [Avatar AI lifetime and interruption regression](27-presentation-ai-lifetime.md) records the v109 crash and ownership repair, followed by passing v120/v121 menu recovery with fresh active snapshots. [Cancellation, native lifetime and crouched contexts](28-mesmerize-cancel-lifetime-and-crouch.md) records v123 cancellation, v125's retained failed assertion, corrected v129 native-expiry proof, v131 phase provenance and the v133 whole-body crouch candidate. Individual receipts distinguish runtime checks from visual acceptance. Full movement and playable co-op remain incomplete.

Latest scoped results: [Chapter29](29-airborne-root-continuity.md) records armed/holstered Mesmerize air continuity. [Chapter30](30-native-domino-investigation.md) records native Domino phases, partner exclusion, target marks, directed connections and beam ownership. {dominoContinuation} Refer to individual completed receipts for runtime verification. Failed fixture placements, input/state assertions and their corrections remain preserved. Native NPC positions and shared gameplay outcomes remain unsynchronized; complete movement and playable co-op are unfinished.

{len(videos)} actual-game videos are indexed. Movement and playable co-op remain incomplete; autonomous work continues beyond movement.

[Chapter31](31-native-devouring-swarm.md) records native Swarm source, body integration, test repairs and current world-component findings. {swarmContinuation}
[Chapter32](32-native-possession-investigation.md) records empty-target remote body binding, exact target ownership, native human-host entry/exit, fixture corrections, and outstanding remote lifecycle/VFX work. {read(P/'review-evidence-annotations.json')['items']['corvo_possession_enter_loop_exit']['notes']}
[Partner interaction exclusion](34-partner-interaction-exclusion.md): {partner.get('latest','See scoped evidence and remaining acceptance matrix.')} {partner.get('requirement','')} Broader direct interaction exclusions remain incomplete.
[Emily Doppelganger cast and clone lifecycle](35-doppelganger-cast-and-clone-lifecycle.md): {doppelganger.get('latest','See chapter35 for scoped source and runtime evidence.')}
{b}'''
assert a in s and b in s
f.write_text(s[:s.index(a)]+section+s[s.index(b)+len(b):],encoding='utf-8')
print(json.dumps(dict(live=live,probe=probe,archive=archive.name,videos=len(videos),integrity=len(checks),completedTakes=len(cases))))


