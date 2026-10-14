from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v1026'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
take='corvo-defense-presentation-v1030';T=N/take
r=rd(T/'result.json');assert r['passed'] and rd(T/'restoration.json')['passed'] and rd(T/'verified.json')['passed']
pres=rd(T/'presentation-results.json')
scope=rd(T/'verified.json')['scope']
videos=rd(N/'runtime-video-manifest.json')
for role in ('host','client'):
    path=f'{take}/{role}.mp4'
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
    review='Raw native diagnostic, visual review not yet entered.'
    if (T/'visual-review.json').exists():
        vr=rd(T/'visual-review.json').get(role)
        if vr:review=vr['observed']+' Limits: '+vr['limits']
    videos=[x for x in videos if x['video']!=path]
    videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Remote defense presentation (Corvo defender) / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=A.name,dll=pin['dll'],runtime_test_passed=True))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'defense-parity-audit-v1031.json',dict(at=at,candidate=A.name,dll=pin['dll'],cases=[dict(take=take,passed=True,scope=scope,result=pres)],complete=False,
   outstanding=['Crouched/sneak block impacts, heavy attacks and parry-win remain unexercised for both roles.','Donor clip acting/transition/contact unreviewed.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p)
for k in ('parry_recovery','parry_directions'):
    it=an['items'][k];it['runtime_binding']='partial_native_steam_block_impact_and_timed_parry_both_roles'
    it['notes']=it['notes'].replace('Verified with Emily as defender and Corvo rendering (1023/1025/1028c; host observer-camera recording 1028c). Corvo-as-defender, crouched','Verified with Emily as defender (1023/1025/1028c) and Corvo as defender against host-native enemies (1030: 4 blocks, 2 timed parries -> client clips357 x13, 358 x11). Crouched')
    it.setdefault('evidence',[]).append('../movement-integration/defense-parity-audit-v1031.json')
wr(p,an)
pin.update(scopedDefensePresentationBothRoles=True,runtimeEvidence2='../defense-parity-audit-v1031.json');wr(A/'identity.json',pin)
print(json.dumps(dict(recordings=len(videos),presentation=pres)))
