from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';F=N/'native-continuous-recovery-v780'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();pin=rd(W/'candidate-recovery-v779/identity.json');result=rd(F/'result.json')
assert result['passed'] and result['dll']==pin['dll'] and len(result['cycles'])==4,result
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
initial=rd(F/'session-before.json');cases=[];indexed=[];name='ld_door_350cm_int_rich_wood_01_3'
for cycle in result['cycles']:
    f=F/cycle['id'];a=rd(f/'after.json');b=rd(f/'before.json');retiring=rd(f/'before-interruption.json');used=rd(f/'fresh-door-use.json')
    c=dict(**cycle,roles={})
    for role in ('host','client'):
        ar=a[role]['reset'];br=b[role]['reset'];ds=a[role]['doors']['session'];old=b[role]['doors']['session'];ii=initial[role]['reset']
        assert ar['arm']==ii['arm'] and ar['peerArm']==ii['peerArm'] and ar['digest']==ii['digest']
        assert ar['completed']==br['completed']+1 and ar['loads']==br['loads']+1 and ar['aborted']==br['aborted'] and ar['maxDistance']<=.001
        assert ar['avatarReady'] and not ar['avatarPending'] and ar['avatarRestored']==br['avatarRestored']+1 and ar['avatarRefused']==br['avatarRefused']
        assert ds['reloads']==old['reloads']+1 and ds['rearms']==old['rearms']+1 and ds['retiredChannels']==old['retiredChannels']+6
        assert ds['retiredNativeCalls']==old['retiredNativeCalls']+sum(e['nativeCalls'] for e in retiring[role]['doors']['entries'])
        assert a[role]['bridge']['localEpoch']!=b[role]['bridge']['localEpoch'] and a[role]['bridge']['probeFollowEnabled']
        for oe in b[role]['doors']['entries']:
            ne=next(e for e in a[role]['doors']['entries'] if e['name']==oe['name'])
            assert ne['arm']>oe['arm'] and ne['revision']==ne['applied']==1 and ne['nativeCalls']==0
        roles=rd(f/f'role-avatar-{role}.json');assert roles['character']['character']==(1 if role=='host' else 2) and len(roles['avatars'])==1
        ai=rd(f/f'postload-ai-{role}.json');assert not ai['isolation']['active'] and not ai['isolation']['heldReferences'] and ai['decision']['started'] and ai['decision']['schedulerActive'] and not ai['decision']['pauseReferences']
        beforeLoad=rd(f/f'loading-before-{role}.json');afterLoad=rd(f/f'loading-after-{role}.json');ev=[e for e in afterLoad['events'] if e['sequence']>beforeLoad['switches']]
        assert sum(e['caller']=='0x787a55' and e['to']==15 for e in ev)==1
        assert sum(e['caller']=='0x7879cd' and e['to']==1 for e in ev)==1
        use=used[role];assert use['native']['current']==use['native']['pending']==2 and use['native']['side']==use['door']['side']
        assert use['door']['revision']==use['door']['applied']==2 and use['door']['nativeCalls']==(0 if role==cycle['source'] else 1)
        c['roles'][role]=dict(epoch=a[role]['bridge']['localEpoch'],checkpointArm=ar['arm'],loads=ar['loads'],completed=ar['completed'],rearms=ds['rearms'],maxDistance=ar['maxDistance'])
    assert a['host']['reset']['transaction']==a['client']['reset']['transaction']
    assert a['host']['reset']['releasedAt']>=a['client']['reset']['readyAt'] and a['client']['reset']['releasedAt']>=a['host']['reset']['readyAt']
    if cycle['death']:
        death=rd(f/'death-screens.json');block=rd(f/'survivor-input-block.json');assert block['distance']<=.001
        for role in ('host','client'):
            g=death[role]['gameover'];assert g['logic']==13 and not g['pending'] and g['flash']!='0x0' and all(p!='0x0' for p in g['showFunction'])
            assert death[role]['health']['current']==(0 if role==cycle['source'] else 56)
            assert death[role]['pair']['reset']['deathCount']==b[role]['reset']['deathCount']+1
            assert not death[role]['pair']['doors']['enabled'] and death[role]['pair']['doors']['session']['enabled']
        c['survivorInputDistance']=block['distance']
    cases.append(c)
for role in ('host','client'):
    before=rd(F/f'inventory-before-{role}.json');after=rd(F/f'inventory-restored-{role}.json')
    assert sorted((i['name'],i['quantity']) for i in before['items'])==sorted((i['name'],i['quantity']) for i in after['items'])
assert rd(N/'native-continuous-recovery-v778/note-restored-v779b/result.json')['passed']
for p in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(p['Path']))==p['Hash']
scope='Four consecutive same-map fixed-SLOT82 recoveries under original checkpoint arms and persistent door-session intent: both manual requests and both real fall deaths, including a paused survivor. Native shared death UI, unchanged survivor health, both-ready retry, fresh epochs/roles/counterparts, new six-door bindings and fresh use. Finite NPC fixture released and native AI independently active after each load. Not arbitrary saves, full campaign, shared enemy authority or complete movement.'
wr(W/'continuous-recovery-audit-v781.json',dict(at=at,passed=True,candidate=pin['dll'],cases=cases,checks=result['checks'],scope=scope,complete=False))
print(json.dumps(dict(passed=True,cycles=len(cases),deaths=sum(c['death'] for c in cases),candidate=pin['dll'],scope=scope)))
