import json,math,hashlib,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
for name in sys.argv[1:]:
    D=P/name;identity=read(D/'identity.json');rows=read(D/'observations.json');raw=read(D/'result.json');checks=[]
    def check(ok,message):
        checks.append(dict(passed=bool(ok),check=message))
    check(not raw['failure'],'harness completed with native controls released')
    check(identity['exeSha256']=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180','exact verified executable')
    held=[r for r in rows if r['source'] and r['source'].get('carrySceneValid') and r['source']['carryScene']['variant'] in (2,3) and 1000<r['ms']<14500]
    check(len(held)>50,'sustained owned native carry source samples')
    check(all(r['source']['motionValid'] and r['source']['motion']==r['source']['stance'] for r in held),'carry preserves independently observed native grounded/crouched movement')
    check(all(r['stream'] and r['stream']['ready'] and r['stream']['corpseMutations'] is False for r in held),'Steam carry remains live; target corpse stays read-only')
    check(sum(bool(r['movement'] and r['movement']['version']==31 and r['movement']['carryWeight']>.95 and r['movement']['weight']>.95) for r in held)>=len(held)*.95,'sustained authored carrier pose publication')
    check({r['source']['stance'] for r in held}=={0,1},'both standing and crouching observed')
    check(all(sum(r['source']['stance']==stance and r['movement'] and r['movement']['speed']>.4 and r['movement']['carryMove']>.9 for r in held)>=3 for stance in (0,1)),'remote legs step during measured standing and slow crouched travel, including native FP idle scenes')
    targets={r['stream']['carryLocalHandle'] for r in held if r['stream']};gens={r['source']['carryScene']['corpseHandle'] for r in held}
    world=read(D/'observer-world-before.json');matches=[n for n in world['npcs'] if n['spawnerName']=='ld_spawner_alexi_escape' and n['definition']=='models/characters/def/story/alexi_mayhew.def' and n['unique'] and n['reciprocal']]
    check(len(matches)==1 and targets=={int(matches[0]['handle'],16)} and len(gens)==1,'peer maps the unique reciprocal authored corpse through its own entity pool')
    check(all(r['movement']['nativeFeet'] and r['movement']['currentFeet'] for r in held if r['movement']),'authored knee references and native feet remain enabled')
    check(any(r['source'] and not r['source'].get('carrySceneValid') and r['ms']>17000 for r in rows),'native release returns source to ordinary movement')
    before=identity['healthBefore'];after=read(D/'health-after.json')
    check(before['current']==after['current'] and before['maximum']==after['maximum'],'local native player health unchanged')
    valid=[r for r in rows if r['source'] and r['movement'] and r['ms']<14500]
    travelled=sum(math.dist(a['source']['position'][:2],b['source']['position'][:2]) for a,b in zip(valid,valid[1:]))
    phases=sum((b['movement']['carryPhase']-a['movement']['carryPhase'])%1 for a,b in zip(valid,valid[1:]))
    bank=next(x for x in read(P/'carry-bank-manifest-v1.json') if x['role']==identity['character']);stride=next(x['cycle_distance_m'] for x in bank['source'] if x['clip']==226)
    check(travelled>.6 and abs(phases*stride-travelled)<.20,'carrier gait phase tracks native travelled distance within 20 cm sampling tolerance')
    capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(D/'native.mp4')]))['streams'][0]
    hz=int(capture['nb_frames'])/float(capture['duration']);check(hz>=24,'real-time recording has at least 24 captured frames per second')
    result=dict(passed=all(x['passed'] for x in checks),checks=checks,samples=len(rows),held_samples=len(held),identity=identity,
                actual_capture_hz=hz,capture=capture,video_sha256=sha(D/'native.mp4'),source_distance_m=travelled,phase_distance_m=phases*stride,
                carrier_gameplay_bound=True,corpse_gameplay_bound=False,complete=False,visual_accepted=False,
                remaining=['Matching corpse attachment and physics/ragdoll handoff','Pickup and release carrier acting','Directional carry locomotion and final load/contact quality','Shared target contention, interruptions, death, reload and peer loss'])
    (D/'validation.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
    print(name,json.dumps({k:result[k] for k in ('passed','samples','held_samples','actual_capture_hz','source_distance_m','phase_distance_m')}))
    for c in checks:
        if not c['passed']:print('FAILED',c['check'])
    if not result['passed']:sys.exit(1)
