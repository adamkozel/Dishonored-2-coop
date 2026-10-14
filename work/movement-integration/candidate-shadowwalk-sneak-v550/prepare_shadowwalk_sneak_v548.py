from pathlib import Path
import shutil, json, hashlib, struct
import numpy as np

R = Path(__file__).resolve().parents[2]
W = R/'work/movement-integration'
N = R/'work/natural-moveset'
S = R/'outputs/d2-coop-lab/src'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
assert rd(W/'native-operation-owner.json')['state'] == 'finished'
evidence = rd(N/'native-shadowwalk-crouch-source-v547b/shadowwalk-observations.json')
assert any('trans_in_sneak' in x['scene'] and x['durationMs'] == [1000] for x in evidence['scenes'])
B = W/'shadowwalk-sneak-pre-v548'
B.mkdir(exist_ok=False)

def edit(name, pairs):
    p = S/name
    shutil.copy2(p, B/name)
    s = p.read_text(encoding='utf-8-sig')
    for a,b in pairs:
        assert s.count(a) == 1, (name,a,s.count(a))
        s = s.replace(a,b)
    p.write_text(s, encoding='utf-8')

edit('native_player_shadowwalk_state.h', [
    ('(s.player.current==1&&phase==0)', '(s.player.current==1&&(phase==0||phase==2))')])
edit('native_player_shadowwalk_scene.h', [
    ('// Only native516b/520 standing-entry and natural-exit contexts. The sustained', '// Native516b/520 standing and547b crouched sneak entry. Low-ceiling crouch,\n// wall, attack and rat-hole contexts remain unverified. The sustained'),
    ('    if(phase==5&&asset==prefix+', '    if(phase==3&&asset==prefix+"trans_in_sneak.midnightscene"&&sub=="emily"&&duration==1000)return 35;\n    if(phase==5&&asset==prefix+'),
    ('            if(asset!=expected)continue;', '            const bool sneak=native.phase==3&&asset=="midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene";\n            if(asset!=expected&&!sneak)continue;')])
edit('power_presentation.h', [
    ('1000,1000,1133};return v<=34?', '1000,1000,1133,1000};return v<=35?'),
    ('s.variant<=34', 's.variant<=35')])
edit('shadow_action_protocol.h', [
    ('version=56,', 'version=57,'), ('p.version==56', 'p.version==57'),
    ('variant>=32&&variant<=34', 'variant>=32&&variant<=35'),
    ('p.powerVariant>=29&&p.powerVariant<=34', 'p.powerVariant>=29&&p.powerVariant<=35')])
edit('movement_bank.h', [
    ('maximumBytes=32+335*', 'maximumBytes=32+336*'),
    ('    bool hasShadowWalk()const', '    bool hasShadowWalkSneak()const{return version>=59;}\n    bool hasShadowWalk()const'),
    ('        if(variant==32)return', '        if(variant==35)return sampleParameter(335,joint,amount);\n        if(variant==32)return'),
    ('331,335};return v<=58?', '331,335,336};return v<=59?'),
    ('v<=58;++v', 'v<=59;++v')])
edit('movement_playback.h', [
    ('const bool shadowWalkActive=power.variant>=32&&power.variant<=34&&p.bank.hasShadowWalk()', 'const bool shadowWalkActive=((power.variant>=32&&power.variant<=34&&p.bank.hasShadowWalk())||(power.variant==35&&p.bank.hasShadowWalkSneak()))'),
    ('if(power.variant==32&&p.powerVariant!=32)', 'if((power.variant==32||power.variant==35)&&p.powerVariant!=power.variant)'),
    ('||(p.powerVariant>=32&&p.powerVariant<=34&&p.bank.hasShadowWalk()))){', '||(p.powerVariant>=32&&p.powerVariant<=34&&p.bank.hasShadowWalk())||(p.powerVariant==35&&p.bank.hasShadowWalkSneak()))){')])
edit('shadowwalk_fx.h', [('p.powerVariant>=32&&p.powerVariant<=34', 'p.powerVariant>=32&&p.powerVariant<=35')])
edit('world_identity_tests.cpp', [
    ('                        constexpr auto alternate=inv+0x7d000;', '''                        {auto low=sw;low.d(physics+0x50,1);low.d(physics+0x54,1);low.d(active,6);
                         low.q(mode+0x10+6*8,ground);low.q(ground,base+0x1c84258);low.d(ground+8,6);low.d(pc+0xe4,2);
                         require(inspect(low)&&!observed.form&&observed.phase==2,"observed547b native sneaking preparation accepted");
                         low.d(pc+0xe4,3);require(!inspect(low),"unobserved default-mode crouched transition not inferred");
                         low.d(pc+0xe4,2);low.q(ground,base+0x1c85628);require(!inspect(low),"crouched phase cannot borrow standing class");}
                        constexpr auto alternate=inv+0x7d000;'''),
    ('                        require(!nativeShadowWalkVariant(4,', '''                        require(nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene","emily",1000)==35,"547b exact native sneaking entry resource and clock");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene","emily",999),"sneaking scene requires recorded clock");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene","Corvo",1000),"Corvo cannot borrow Emily sneaking form");
                        require(!nativeShadowWalkVariant(4,''')])
edit('shadow_action_protocol_tests.cpp', [
    ('for(unsigned v=32;v<=34;++v)', 'for(unsigned v=32;v<=35;++v)'),
    ('bad.version=54;check(!validShadowAction(bad,ec,eh),"old peer cannot claim unsupported shadow body contract")', 'bad.version=56;check(!validShadowAction(bad,ec,eh),"old peer cannot claim unsupported shadow sneak contract")')])
edit('movement_bank_tests.cpp', [
    ('format=(argc==1?1:58);format<=58;', 'format=(argc==1?1:59);format<=59;'),
    ('        check(bank.hasShadowWalk()==', '''        check(bank.hasShadowWalkSneak()==(bank.version>=59),"sneaking form requires complete bank59");
        if(bank.hasShadowWalkSneak()){
            for(unsigned j=0;j<joints;++j)for(unsigned k=0;k<7;++k){
                check(std::fabs(bank.sampleShadowWalk(35,j,0,0,0)[k]-bank.sampleParameter(334,j,1)[k])<1e-6f,"sneak entry starts at existing recovered crouch");
                check(std::fabs(bank.sampleShadowWalk(35,j,1,0,0)[k]-bank.sampleParameter(334,j,0)[k])<1e-6f,"sneak entry ends at existing low form");
            }
        }
        check(bank.hasShadowWalk()==''')])

out = N/'shadowwalk-sneak-banks-v548'
out.mkdir(exist_ok=False)
reports = []
for role in ('corvo','emily'):
    old = (N/'shadowwalk-banks-v523'/f'{role}-shadowwalk-bank.bin').read_bytes()
    magic,version,joints,count,fps,layout = struct.unpack_from('<8sIIIIQ',old)
    assert (magic,version,count,fps)==(b'D2MOVE58',58,335,60)
    entry=24+121*joints*28
    poses=np.frombuffer(old,dtype='<f4',offset=32+334*entry+24,count=121*joints*7).reshape(121,joints,7)
    # Reverse the existing authored low-form-to-crouch exit, excluding its
    # duplicate interpolation sentinel. Network root and native scene clock own
    # travel/timing. This does not borrow the first-person hand transforms.
    sneak=poses[:120][::-1].copy()
    sneak=np.concatenate((sneak,sneak[:1]))
    assert np.isfinite(sneak).all()
    assert np.max(np.abs(sneak[:,:,:4].__pow__(2).sum(axis=2)-1))<1e-4
    bank=struct.pack('<8sIIIIQ',b'D2MOVE59',59,joints,336,fps,layout)+old[32:]
    bank+=struct.pack('<IIIIff',335,121,0,0,0.,1.)+sneak.tobytes()
    assert bank[32:len(old)]==old[32:]
    path=out/f'{role}-shadowwalk-sneak-bank.bin'
    path.write_bytes(bank)
    reports.append(dict(role=role,sha256=hashlib.sha256(bank).hexdigest().upper(),bank=str(path.relative_to(N)),version=59,clips=336,newClip=335,sourceClip=334,operation='Reverse first120 stored poses; append initial sentinel',duration=1.,prior335PayloadsUnchanged=True,scope='Crouched sneak entry body candidate. Native547b scene timing; runtime and visual acceptance pending. Corvo offline only.'))
(N/'shadowwalk-sneak-bank-manifest-v548.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')

s=(W/'Build-ShadowWalkMaterial-v544.ps1').read_text(encoding='utf-8-sig').replace('v544','v548').replace('Build544','Build548').replace('actual335','actual336').replace('shadowwalk-banks-v523','shadowwalk-sneak-banks-v548').replace('-shadowwalk-bank.bin','-shadowwalk-sneak-bank.bin')
(W/'Build-ShadowWalkSneak-v548.ps1').write_text(s,encoding='utf-8')
print(json.dumps(dict(prepared=True,bank=59,clips=336,protocol=57,liveUnchanged=True)))
