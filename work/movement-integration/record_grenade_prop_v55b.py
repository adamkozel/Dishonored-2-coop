"""Index inspected prop takes; refresh exact measured pins and handoff state."""
import json,hashlib,datetime,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();videos=read(P/'runtime-video-manifest.json');cases={}
for take in ['grenade-prop-corvo-idle-v55','grenade-prop-corvo-close-v55b','grenade-prop-emily-close-v55b']:
 result=read(P/take/'result.json');cases[take]=result;video=P/f'{take}.mp4';assert(P/take/'native-sheet.jpg').exists()
 notes='Native cosmetic grenade visible with the authored hand pose and hidden at expiry. Close views remain dark and partly occluded; detailed finger contact is not accepted. Test is fixed-pose presentation, not network action binding.' if result['passed'] else 'Initial Hide completion check failed because the parked-prop helper rejects pendingHide. Original failed take retained. Corrected v55b uses the native hidden flag.'
 review=f'{take}/visual-review.json';write(P/review,dict(inspected=True,at=now,evidence='native-sheet.jpg',method='Actual-game temporal sheet inspected',visual_accepted=False,complete=False,notes=notes))
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=video.name,character=result['character'],label=f'Grenade native cosmetic prop · {take}',alias='grenade_idle',requirements=['grenade_equip_aim_throw'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=n/d,video_sha256=sha(video),checks_passed=result['passed'],scope=result['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row)
write(P/'runtime-video-manifest.json',videos)
live=sha(W/'asset-game/dinput8.dll');probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
archives=sorted((W/'movement-integration').glob('candidate-grenade-*'));archive=next(a for a in archives if(a/'identity.json').exists()and read(a/'identity.json').get('dll')==live);identity=read(archive/'identity.json');assert probe==identity['probe']
checks=[]
for f in read(W/'movement-integration/protected-save-hashes-before-v4.json'):
 actual=sha(Path(f['Path']));checks.append(dict(path=f['Path'],expected=f['Hash'],actual=actual,passed=actual==f['Hash']))
for name,expected in [('Dishonored2.exe','B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'),('steam_api64.dll','71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8')]:
 for folder in [Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2'),W/'asset-game']:
  f=folder/name;actual=sha(f);checks.append(dict(path=str(f),expected=expected,actual=actual,passed=actual==expected))
assert all(c['passed']for c in checks);write(W/'handoff-integrity-verification.json',dict(at=now,passed=True,checks=checks,liveProxy=live,probe=probe,candidate=archive.name))
state=read(W/'current-task-state.json');m=state['MovementIntegration'];m['GrenadePropV55b']=dict(cases=cases,archive='movement-integration/candidate-grenade-prop-v55b',authoring='natural-moveset/grenade-authoring-manifest-v55.json',remote_network_verified=False,complete=False)
m['LatestContinuation'].update(live=live,liveBuild=live,probe=probe,protocol=identity['protocol'],bank='Standard D2MOVE41/251 preserved; isolated grenade candidate D2MOVE42/260',actual_game_videos=len(videos),next='Validate live grenade network actions, mechanisms and interruption cleanup; continue full co-op',in_progress='Grenade body/prop network candidate; movement and playable co-op incomplete',complete=False);state['UpdatedAt']=now;write(W/'current-task-state.json',state)
ann=read(P/'review-evidence-annotations.json');item=ann['items']['grenade_equip_aim_throw'];item['runtime_binding']='native_source_and_manual_prop_verified_network_pending';item['notes']='Native source phases and manual left-hand cosmetic prop verified for both roles. v55b fixes Hide completion. Nine-phase authored candidate and v56 network code are under test. No shared projectile, detonation or damage authority; fine grip, mechanism, VFX, locomotion, crouch and interruption acceptance remain.';item['native_integration'].update({v['video']:v for v in videos if v['video'].startswith('grenade-prop-')});write(P/'review-evidence-annotations.json',ann)
f=D/'01-current-state.md';s=f.read_text();mark='<!-- GRENADE-LIVE-V55 -->';end='<!-- /GRENADE-LIVE-V55 -->';section=f'''{mark}
## Current measured grenade continuation

Updated {now}. This supersedes earlier live pins below. Live DLL `{live}`, probe `{probe}` and semantic protocol {identity['protocol']} match [the archived candidate](../../work/movement-integration/{archive.name}/identity.json). [All eighteen protected file checks pass](../../work/handoff-integrity-verification.json). Original Steam executable/API remain unchanged.

Both roles pass three manual grenade prop attachment/expiry checks on v55b. The initial v55 cleanup failure is retained with its cause. Actual close-up sheets were inspected; dark/occluded finger contact is not accepted. [Authoring and prop evidence](19-grenade-authoring-and-prop.md). A separate nine-phase D2MOVE42/260 bank preserves the prior 251 clips byte-for-byte and is being tested over the native Steam semantic stream. Source gameplay continues to own its projectile; remote projectile/world authority remains absent.

{len(videos)} actual-game videos are indexed. This is an evidence count, not completion. Movement and playable co-op remain incomplete; autonomous work continues beyond movement.
{end}'''
if mark in s:s=s[:s.index(mark)]+section+s[s.index(end)+len(end):]
else:s=s.replace('\n','\n\n'+section+'\n',1)
f.write_text(s)
f=D/'19-grenade-authoring-and-prop.md';s=f.read_text();mark='## Native runtime attachment v55b'
if mark not in s:s+='''

## Native runtime attachment v55b

[Corvo close-up](../../work/natural-moveset/grenade-prop-corvo-close-v55b/result.json) and [Emily close-up](../../work/natural-moveset/grenade-prop-emily-close-v55b/result.json) pass attachment, finite native Hide and unchanged inventory/health/protected-file checks. Their actual-game sheets and recordings are indexed in the review dashboard. The grenade remains associated with the authored left hand, then disappears. Lighting and occlusion limit finger-contact judgment; final grip quality is unaccepted.

The initial [v55 take](../../work/natural-moveset/grenade-prop-corvo-idle-v55/result.json) failed its Hide completion assertion. `rangedPropIsHidden` is a reuse predicate that also requires `pendingHide == false`. Calling it before clearing that flag could never complete cleanup. v55b reads native entity flag `+0xd0 & 1` after native Hide, matching the established ranged cleanup path. The failed take and build are preserved; no result was relabeled.

The ten-second review pose lease is re-armed separately from the fifteen-second prop lease for this fixture. A transient native pose between review leases is not a normal gameplay transition acceptance result. The candidate [v56 bank builder](../../work/natural-moveset/build_grenade_bank_v56.py) appends nine grenade phases for each role, preserving all previous payload bytes. [Native Steam test](../../work/natural-moveset/Test-GrenadeNetwork-v56.ps1) records source and remote views and resets both temporary worlds after a real native throw. Network binding is pending until its actual results are linked.
'''
f.write_text(s);print(json.dumps(dict(live=live,archive=archive.name,videos=len(videos),integrity=len(checks))))
