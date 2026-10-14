"""Publish carrier-only evidence without claiming synchronized body carrying."""
import datetime, hashlib, json, shutil
from pathlib import Path
P=Path(__file__).resolve().parent
ROOT=P.parent.parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2),encoding='utf-8');t.replace(p)
backup=P/'catalog-before-carry-carrier-v3'
backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json','review.html','review-catalog.js']:
    if not (backup/name).exists(): shutil.copy2(P/name,backup/name)
manifest=read(P/'runtime-video-manifest.json');cases={}
for character in ['corvo','emily']:
    take=f'native-carry-carrier-{character}-v3';folder=P/take
    result=read(folder/'validation.json');visual=read(folder/'visual-review.json')
    assert result['passed'] and result['carrier_gameplay_bound'] and not result['corpse_gameplay_bound']
    assert not result['complete'] and not result['visual_accepted'] and visual['inspected'] and not visual['accepted']
    assert result['actual_capture_hz']>=24 and sha(folder/'native.mp4')==result['video_sha256']
    video=take+'/native.mp4';identity=result['identity']
    row=dict(video=video,character=character,label=character.title()+' · Loaded carrier movement · corpse attachment pending',
        alias='native_carry_carrier_partial',carrier_only=True,travelling=True,gameplay_bound=True,
        corpse_gameplay_bound=False,complete=False,visual_accepted=False,
        actual_capture_hz=result['actual_capture_hz'],timing='Actual elapsed capture timing; no speedup',
        scope=identity['scope'],requirements=['carry_body_lift_idle_move_drop_throw'],
        evidence=take+'/validation.json',visual_review=take+'/visual-review.json',
        proxy_sha256=identity['dllSha256'],bank_sha256=identity['bankSha256'],
        source_role=identity['role'],observer_role=identity['peer'])
    manifest=[r for r in manifest if r['video']!=video];manifest.append(row)
    cases[character]=dict(video=video,validation=row['evidence'],visual_review=row['visual_review'],
        checks=len(result['checks']),samples=result['samples'],held_samples=result['held_samples'],
        actual_capture_hz=result['actual_capture_hz'])
annotations=read(P/'review-evidence-annotations.json');item=annotations['items']['carry_body_lift_idle_move_drop_throw']
item['runtime_binding']='partial_native_carrier_only_both_roles'
item['notes']=item['notes'].replace('Remote corpse attachment and authored carry playback are still incomplete.',
    'Remote corpse attachment remains incomplete; loaded carrier playback is now partially bound.')
note='Both characters now show standing and crouched loaded idle/forward walking from owned native carry state through Steam. Carrier gait follows distance, including slow crouch below the native first-person walk-scene threshold. Corpse attachment, pickup/release acting, physical outcomes and lifecycle acceptance remain pending; these recordings show an empty shoulder.'
if note not in item['notes']: item['notes']+=' '+note
item['carrier_runtime_evidence']=dict(cases=cases,corpse_gameplay_bound=False,complete=False,visual_accepted=False)
state=read(ROOT/'work/current-task-state.json');movement=state['MovementIntegration']
movement['CarryCarrierExperiment']=dict(cases=cases,checks=sum(c['checks']for c in cases.values()),
    samples=sum(c['samples']for c in cases.values()),dllSha256=identity['dllSha256'],protocol=29,
    bankFormat='D2MOVE31',clips=229,carrierGameplayBound=True,corpseGameplayBound=False,
    complete=False,visualAccepted=False,archive='work/movement-integration/candidate-carry-runtime-v3',
    nativeAttachmentResearch='work/natural-moveset/carry-peer-research-v1/native-retargeting-lifecycle.json',
    next='Validate native retargeting ownership and target reference-frame lifecycle before a scoped remote corpse attachment experiment.')
movement['LatestContinuation']['actual_game_videos']=len(manifest)
movement['LatestContinuation']['next']='Carrier-only carry movement bound for both roles. Continue native corpse attachment and release, then remaining movement contract. Complete movement remains false.'
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat()
html=P/'review.html';s=html.read_text(encoding='utf-8-sig')
old=":r.travelling?'Actual player inputs and network-driven world movement.'"
new=":r.carrier_only?'Native Steam drives the carrier body. Corpse attachment and physics are still pending.':r.travelling?'Actual player inputs and network-driven world movement.'"
if new not in s:
    assert s.count(old)==1;s=s.replace(old,new)
    t=html.with_suffix('.html.tmp');t.write_text(s,encoding='utf-8');t.replace(html)
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
write(ROOT/'work/current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),carrier_checks=sum(c['checks']for c in cases.values()),complete=False)))
