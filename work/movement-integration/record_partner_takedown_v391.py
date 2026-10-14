"""Record scoped observed outcomes, including the retained baseline failure."""
from pathlib import Path
import json,hashlib,subprocess,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';A=W/'candidate-partner-assassination-v387'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,x:p.write_text(json.dumps(x,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
identity=read(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
manifest=read(P/'runtime-video-manifest.json');videos=[];cases={}
def index(take,key,character,label,scope,passed):
    folder=P/take;video=folder/(key+'.mp4');assert video.exists()
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
    review=dict(inspected=True,complete=False,visualAccepted=False,scope=scope,contactSheet='contact-sheet.jpg' if (folder/'contact-sheet.jpg').exists() else key+'-sheet.jpg',times=[1,2,3,4,5,6] if (folder/'contact-sheet.jpg').exists() else [1.2])
    reviewfile=folder/(key+'-visual-review.json');write(reviewfile,review)
    row=dict(video=video.relative_to(P).as_posix(),character=character,label=label,alias='native_partner_interaction_exclusion',requirements=['nonlethal_takedown_front_back_sides'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=reviewfile.relative_to(P).as_posix(),proxy_sha256=read(folder/'result.json')['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=passed,scope=scope)
    assert not any(x['video']==row['video'] for x in manifest),'Already indexed; inspect existing receipt'
    manifest.append(row);videos.append(row)
for role,take in [('host','partner-takedown-exclusion-host-v388'),('client','partner-takedown-exclusion-client-v389')]:
    folder=P/take;result=read(folder/'result.json');assert result['passed'] and not result['failure'] and result['dll']==identity['dll']
    rows=read(folder/f'{role}-rear-choke-observations.json');f=read(folder/'filter.json');assert f['after']['blocked']>f['before']['blocked'] and f['after']['readFailed']==f['before']['readFailed']==0
    assert not any(s['id']==41 for r in rows for s in r['player']['active'])
    targets=0
    for r in rows:
        g=next(x for x in r['sensors'] if 'GlobalAssassination' in x['type']);words={x['offset']:int(x['word'],16) for x in g['object']['words']}
        targets+=r['source']['probeActorHandle'] in [words[o]&0xffffffff for o in ('0x60','0x98','0xd0','0x108','0x140')]
    assert targets==0
    x=dict(role=role,take=take,passed=True,samples=len(rows),blocked=f['after']['blocked']-f['before']['blocked'],readFailures=0,partnerTargetSamples=0,nativeStates=sorted(set(s['id'] for r in rows for s in r['player']['active'])),readMaxUs=f['after']['readMaxUs'])
    write(folder/'interaction-analysis.json',x);cases[take]=x
    index(take,role+'-rear-choke','corvo' if role=='host' else 'emily',f'Partner rear choke excluded · {role} · '+('388' if role=='host' else '389'),f"Viewed1/2/3/4/5/6s. Native block replaces the previously possible partner choke; no paired hold or choke/throw HUD. {x['blocked']} exact partner eligibility rejections,0 refusal,0 partner target records across{x['samples']} samples. Both checkpoint inventories/health and14 protected hashes restore. Scope is this standing rear input only; other contexts and full movement remain incomplete.",True)
take='ordinary-guard-choke-control-v390';folder=P/take;result=read(folder/'result.json');assert result['passed'] and not result['failure']
target=int(read(folder/'guard-before.json')['handle'],16);rows=read(folder/'host-rear-choke-observations.json')
scenes=[s for r in rows for s in r['midnight']['instances'] if s.get('state')==3 and '/choke/' in s.get('asset','')]
bound=[s for s in scenes if any(b['handle']==target and b['reciprocal'] for b in s['bindingsResearch'])]
assert len(bound)>=3 and any('choke_back' in s['asset'] for s in bound) and any('choke_loop' in s['asset'] for s in bound)
f=read(folder/'filter.json');assert f['after']['otherAccepted']>f['before']['otherAccepted']
x=dict(take=take,passed=True,target=hex(target),nativeState41Samples=sum(any(s['id']==41 for s in r['player']['active']) for r in rows),targetBoundScenes=len(bound),nativeOtherAccepted=f['after']['otherAccepted']-f['before']['otherAccepted'])
write(folder/'interaction-analysis.json',x);cases[take]=x
index(take,'host-rear-choke','corvo','Ordinary guard choke control · 390','Viewed1/2/3/4/5/6s. Native guard prompts, first-person grab and choke/throw HUD remain. Exact authored guard2 appears reciprocally in native choke_back/loop scene bindings; state41 present and ordinary eligibility accepted. Both checkpoint inventories/health and14 protected hashes restore. This is native source positive-control footage, not an authored remote full-body movement approval.',True)
index('partner-rear-choke-host-v386','host-rear-choke','corvo','Baseline partner choke defect · 386','Retained failing baseline. Viewed1/2/3/4/5/6s: real native partner grab and choke/throw HUD. State41 in23/28 samples, paired choke_back/loop. Health protection alone did not prevent interaction. Collection and restoration passed; interaction exclusion failed.',False)
for role in ('host','client'):
    index('partner-context-targeting-v385',role+'-rear','corvo' if role=='host' else 'emily',f'Partner targeting baseline · {role} · 385','Viewed rear-view frame1.2s. Partner appears without a prompt although native global assassination sensor records select it. Neutral observation only; actual choke386 establishes the functional gap. Other four neutral clips are retained but not marked visually reviewed.',False)
write(P/'runtime-video-manifest.json',manifest)
receipt=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=identity['dll'],probe=identity['probe'],cases=cases,videos=videos,scope='Scoped rear choke partner exclusion in both directions plus ordinary guard positive control. Normal movement collision retained. Other contextual actions, power paths, lifetime contexts, full movement and playable co-op remain incomplete.')
write(A/'runtime-verification-v391.json',receipt);identity['runtimeVerified']=True;identity['runtimeVerificationScope']=receipt['scope'];write(A/'identity.json',identity)
for n in ('Test-PartnerTakedown-v388.ps1','Test-GuardChokeControl-v390.ps1'):shutil.copy2(P/n,A/n)
statefile=R/'work/current-task-state.json';state=read(statefile)
state['PartnerInteractionExclusion'].update(collision='Normal native collision explicitly allowed and restored383; no collision hook in387.',live=identity['dll'],latest='387: both-role rear choke exclusion388/389 and ordinary guard control390 pass. Scoped evidence only.',receipt='work/movement-integration/candidate-partner-assassination-v387/runtime-verification-v391.json',next='Broaden partner takedown rejection to lethal/crouched/drop/slide contexts, audit Far Reach and other direct powers/interactions, then continue full movement and shared-world co-op development.',complete=False)
write(statefile,state)
doc=R/'documentation/handoff-2026-10-11/34-partner-interaction-exclusion.md';t=doc.read_text(encoding='utf-8')
t=t.replace('| Focus, contextual prompts, takedowns and body handling | Not yet comprehensively audited | Reject partner identity before prompts, victim selection, animations and outcomes |','| Contextual takedowns and body handling | Rear choke rejected in both directions388/389; ordinary guard control390 passes | Lethal/crouched/drop/slide/window cases, focus, loot and carrying still require coverage |')
t+='\n\n## Scoped verified candidate387, tests388-390\n\nBoth native standing rear partner choke tests pass4 checks. Corvo rejects1668 eligibility calls and Emily1644, with zero ownership refusals. All27 samples in each recording contain no partner target in the five global sensor records and no choke state41; native running/block states0/44 remain. The viewed1/2/3/4/5/6s sheets show normal first-person sword blocking, no partner choke and no choke/throw HUD. Maximum validation samples were328/335 microseconds. Each test includes complete checkpoint inventory/health restoration and14 protected hashes.\n\nThe ordinary authored guard2 positive control390 passes4 checks, preserving native state41 and choke_back/choke_loop. Its exact generation-bearing handle appears reciprocally in native scene bindings. The viewed recording retains the native guard interaction prompts and choke/throw HUD. All checkpoints/inventories/health and14 protected hashes restore. This verifies a scoped eligibility change, not every takedown rule or full multiplayer combat authority.\n\n[Verification receipt](../../work/movement-integration/candidate-partner-assassination-v387/runtime-verification-v391.json) records build pins, measurements and six newly indexed source recordings, including the failing baseline. Other four neutral observation385 videos are retained but not claimed visually reviewed. Normal movement collision remains allowed.\n'
doc.write_text(t,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),newVideos=len(videos),cases=cases,receipt=str(A/'runtime-verification-v391.json'))))
