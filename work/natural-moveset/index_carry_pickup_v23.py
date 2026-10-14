"""Index native-input carrier evidence separately from complete paired pickup."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-pickup-v23';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={}
for take in ['native-carrier-pickup-corvo-v23','native-carrier-pickup-emily-v23','native-carrier-pickup-crouched-corvo-v23']:
    f=P/take;v=read(f/'validation.json');i=read(f/'identity.json');low=i['crouched']
    assert v['passed'] and hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    review=dict(inspected=True,accepted=False,complete=False,method='Actual game temporal contact sheet spanning entry, gather, hoist and loaded pose.',
        observations=('Crouched Corvo retains the existing crouch and transitions to loaded crouch; the standing gather is correctly not used. There is no crouched pickup choreography in this case.' if low else 'Ordinary source pickup visibly drives remote gather, deep forward bend, rise and loaded shoulder pose. The remote victim remains on the floor: this verifies carrier-only binding, not contact. The gather is still too deep to call finished.'),
        limitations=['Native holster moves the original clone sword to holster_wpn; the visible extended blade and attachment appearance still need review.','No remote victim choreography in these takes.','Hand contact, arbitrary floor poses and crouched pickup remain incomplete.'])
    write(f/'visual-review.json',review)
    row=dict(video=take+'/native.mp4',character=i['character'],label=i['character'].title()+(' · Crouched pickup continuity' if low else ' · Native-input standing carrier pickup'),
        alias='native_carry_preparation_crouched' if low else 'carry_body_short_lift_study',requirements=['carry_body_lift_idle_move_drop_throw'],
        gameplay_bound=not low,complete=False,visual_accepted=False,evidence=take+'/validation.json',visual_review=take+'/visual-review.json',
        actual_capture_hz=v['actual_capture_hz'],proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],scope=i['scope'],timing='Native source clock over native Steam; actual elapsed capture')
    manifest=[m for m in manifest if m['video']!=row['video']];manifest.append(row)
    cases[take]=dict(evidence=row['evidence'],video=row['video'],checks=len(v['checks']),video_sha256=v['video_sha256'],visual_review=row['visual_review'],scope=i['scope'])
identity=read(P.parent/'movement-integration/candidate-carry-pickup-v23/identity.json');identity.update(runtimeValidated=True,cases=cases,validation_scope='Both standing carrier-only roles; Corvo crouched continuity. No complete paired pickup or visual acceptance.')
write(P.parent/'movement-integration/candidate-carry-pickup-v23/identity.json',identity)
findings=dict(build=identity['sha256'],cases=cases,confirmed=[
    'Exact native Carry45+Equip53 preparation now transmits as variant7 with stable corpse identity; standing and crouched base states remain distinguished.',
    'Preparation uses a bounded local observation clock (300ms authored gather), not a fabricated native scene duration. In keeps the native 1000ms clock.',
    'v33 appends one standing carrier clip to each v32 bank; previous payload is byte-identical.',
    'Both standing roles execute gather/hoist/loaded carrier poses from ordinary player input and native Steam semantics.',
    'Corvo crouched pickup keeps stream and bank active and refuses the standing hoist.',
    'Source/observer/avatar health unchanged; native equipment lease restores after stream stop in captured Emily and crouched Corvo evidence.'
],limitations=['Carrier gather remains visually deep.','Native flags and holster tag are verified, but extended holstered blade appearance still needs work.','The remote body remains on the floor in these carrier-only recordings.','Crouched gather, no-preparation pickup, interruption cases, matched live victim path and arbitrary corpse authority remain incomplete.'],complete=False,visual_accepted=False)
write(P/'carry-pickup-findings-v23.json',findings)
write(P/'runtime-video-manifest.json',manifest)
a=read(P/'review-evidence-annotations.json');a['items']['carry_body_lift_idle_move_drop_throw']['native_carrier_pickup']=cases
a['items']['carry_body_lift_idle_move_drop_throw']['notes']='v23 binds standing gather/hoist to observed native preparation and native In clock, verified for both roles. Corvo crouched continuity passes, with no standing lift applied. These are carrier-only recordings: the body remains on the floor. v24 paired snapshot transfer is a separate unvalidated experiment. Contact, crouched lifting, throws and general body authority remain incomplete.'
write(P/'review-evidence-annotations.json',a)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration']
m['NativeCarrierPickup']=findings;m['LatestContinuation'].update(actual_game_videos=len(manifest),next='Validate bounded live victim transfer on the same native pickup timeline; retain unaccepted contact status.',complete=False)
live=read(P.parent/'movement-integration/candidate-carry-paired-v24/identity.json')
m['LatestContinuation'].update(live=live['sha256'],liveBuild=live['sha256'],in_progress='candidate-carry-paired-v24; paired runtime validation pending')
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),carrier_cases=len(cases),complete=False)))
