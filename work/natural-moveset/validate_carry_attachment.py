"""Assess a mechanics experiment; ownership alone is not successful carrying."""
import hashlib,json,math,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def distance(a,b):return math.sqrt(sum((x-y)**2 for x,y in zip(a,b)))
take=sys.argv[1];folder=P/take;rows=read(folder/'observations.json');identity=read(folder/'identity.json')
held=[r for r in rows if r.get('retarget',{}).get('count')==1 and r.get('attachment',{}).get('active')]
# Counters persist across cases in one process. An earlier case's completed
# release must never count as cleanup of this case.
initial=rows[0].get('attachment',{}) if rows else {}
after=[r for r in rows if r.get('attachment',{}).get('released',0)>initial.get('released',0) and not r['attachment']['active']]
checks=[]
def check(name,passed):checks.append(dict(check=name,passed=bool(passed)))
check('harness completed',read(folder/'result.json')['failure']is None)
check('strict reciprocal attachment observed',len(held)>=20 and all(r['retarget']['valid']for r in held))
check('one exact mapped corpse generation',len({r['retarget']['targetHandle']for r in held})==1 and all(r['retarget']['targetHandle']==r['stream']['carryLocalHandle']for r in held))
check('native release empties attachment list',len(after)>=10 and all(r['retarget']['valid']and r['retarget']['count']==0 for r in after))
final=rows[-1].get('attachment',{}) if rows else {}
check('this case starts and releases exactly one attachment',final.get('started',0)-initial.get('started',0)==1 and final.get('released',0)-initial.get('released',0)==1 and not final.get('active',True))
check('local first-person health preserved',read(folder/'health-after.json')['current']==identity['healthBefore']['current'])
if (folder/'avatar-health-before.json').exists() and (folder/'avatar-health-after.json').exists():
    check('remote avatar health preserved',read(folder/'avatar-health-before.json')['current']==read(folder/'avatar-health-after.json')['current'])
carrier_travel=distance(held[0]['observerRoot']['probeRenderPosition'],held[-1]['observerRoot']['probeRenderPosition']) if held else 0
corpse_travel=distance(held[0]['corpsePose']['root'][:3],held[-1]['corpsePose']['root'][:3]) if held else 0
separations=[distance(r['corpsePose']['root'][:3],r['observerRoot']['probeRenderPosition'])for r in held]
# Similar displacement can mean dragging the body with its original 11 m
# offset. It does not establish attachment at the carrier's shoulder.
root_follow=carrier_travel>1 and abs(corpse_travel-carrier_travel)<.5 and max(separations,default=999)<2.5
probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate,nb_frames,duration','-of','json',str(folder/'native.mp4')]))['streams'][0]
n,d=map(int,probe['avg_frame_rate'].split('/'));hz=n/d
check('at least 24 captured frames per second',hz>=24)
victim_scene=False;victim_native=False;victim_clip_count=None
if identity.get('victimSceneRequested'):
    scene_path='midnight/d2coop/carry_victim_loop.midnightscene'
    scenes=[x for x in read(folder/'midnight-held.json')['instances'] if x.get('asset')==scene_path]
    victim_scene=len(scenes)==1 and scenes[0]['state']==3 and len(scenes[0].get('bindingsResearch',[]))==1 and scenes[0]['bindingsResearch'][0]['handle']==held[0]['retarget']['targetHandle'] if held else False
    check('victim-only native scene owns exactly the corpse',victim_scene)
    input_path=folder/'corpse-midnight-input-held.json'
    if input_path.exists():
        input_words=read(input_path)['words']
        victim_clip_count=int(next(w['word'] for w in input_words if w['offset']=='0x8'),16)&0xffffff
        clip_path=folder/'corpse-native-clip-held.json'
        held_clip=next((w.get('pointerText','')for w in read(clip_path)['words']if w['offset']=='0x8'),'')if clip_path.exists()else''
        victim_native=victim_scene and victim_clip_count==1 and held_clip.endswith('/generic_carrycorpse_idle.md6anim')
    check('actual native victim carry clip is connected',victim_native)
    check('victim scene retired on release',not any(x.get('asset')==scene_path for x in read(folder/'midnight-after.json')['instances']))
    check('corpse remains dead',read(folder/'corpse-health-before.json')['current']==0 and read(folder/'corpse-health-after.json')['current']==0)
    if (folder/'corpse-physics-held.json').exists():
        def word(subject,phase,offset):
            return int(next(w['word'] for w in read(folder/f'corpse-{subject}-{phase}.json')['words'] if w['offset']==offset),16)
        modes=[word('physics',p,'0x238')&0xffffffff for p in ('before','held','after')]
        check('native physics changes ragdoll to animated and back',modes==[0,3,0])
        flags=[word('object',p,'0x368')&2 for p in ('before','held','after')]
        check('native carried flag is claimed and restored',flags==[0,2,0])
drop_native=False
interruption=read(folder/'interruption.json').get('interruption','none') if (folder/'interruption.json').exists() else 'none'
if interruption!='none':
    events=read(folder/'events.json')
    command='coop_carry_attachment off' if interruption=='cancel' else 'coop_shadow_stream off'
    event=next((e for e in events if e.get('input',{}).get('command')==command),None)
    check('interruption retires attachment within 1.2 seconds',bool(event) and bool(after) and 0<=after[0]['ms']-event['ms']<1200)
    if interruption=='stream':
        resume=next((e for e in events if e.get('input',{}).get('command')=='coop_shadow_stream on'),None)
        resumed=[r for r in rows if resume and r['ms']>=resume['ms']+700 and r['ms']<resume['ms']+3000]
        check('action stream resumes with fresh carrying state',any(r.get('stream',{}).get('ready') and r['stream'].get('remoteState')==14 for r in resumed))
        check('resumed action stream does not replay attachment',bool(resumed) and all(not r['attachment']['active'] and r['retarget']['count']==0 for r in resumed))
    check('Steam transport stays connected through interruption',all(r.get('observerRoot',{}).get('connectionState')==3 for r in rows))
if identity.get('nativeDropExpected'):
    drop_rows=[r for r in rows if r.get('attachment',{}).get('dropStage')==2]
    check('source drop reaches victim playback',bool(drop_rows))
    path=folder/'midnight-drop.json'
    scenes=[x for x in read(path)['instances']if x.get('asset','').startswith('midnight/d2coop/carry_victim_drop_')] if path.exists() else []
    check('native drop scene binds only the same corpse',len(scenes)==1 and scenes[0]['state']==3 and len(scenes[0].get('bindingsResearch',[]))==1 and bool(held) and scenes[0]['bindingsResearch'][0]['handle']==held[0]['retarget']['targetHandle'])
    clip_path=folder/'corpse-native-clip-drop.json'
    clip=next((w.get('pointerText','')for w in read(clip_path)['words']if w['offset']=='0x8'),'')if clip_path.exists()else''
    expected_variant=drop_rows[0]['attachment']['dropVariant']if drop_rows else 0
    suffix='generic_carrycorpse_droplow_sneak.md6anim'if expected_variant==6 else'generic_carrycorpse_droplow.md6anim'
    drop_native=clip.endswith('/'+suffix)
    check('actual native victim drop clip is connected',drop_native)
    check('native drop scene retires',not any(x.get('asset','').startswith('midnight/d2coop/carry_victim_drop_')for x in read(folder/'midnight-after.json')['instances']))
    arm=next((e for e in read(folder/'events.json')if e.get('input',{}).get('command','').startswith('coop_carry_attachment ')),None)
    lease=int(arm['input']['command'].split()[-1]) if arm else 0
    check('source drop releases before the fixture timeout',bool(after) and bool(arm) and after[0]['ms']<arm['ms']+lease-300)
    if (folder/'bank-before.json').exists() and read(folder/'bank-before.json')['version']>=32:
        check('authored carrier follows the same native drop state',any(r.get('movement',{}).get('carryVariant')==expected_variant and r['movement'].get('carryWeight',0)>.9 for r in rows))
result=dict(identity=identity,samples=len(rows),held_samples=len(held),checks=checks,mechanical_checks_passed=all(x['passed']for x in checks),
    post_release_samples=len(after),release_counter_delta=final.get('released',0)-initial.get('released',0),
    interruption=interruption,
    root_follows_carrier=root_follow,carrier_displacement_m=carrier_travel,corpse_displacement_m=corpse_travel,
    maximum_carrier_root_separation_m=max(separations,default=None),
    actual_capture_hz=hz,video_sha256=hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper(),
    native_victim_scene_active=victim_scene,victim_animation_input_count=victim_clip_count,
    native_victim_animation=victim_native,native_victim_drop_animation=drop_native,visual_accepted=False,complete=False,
    scope='Explicitly armed attachment mechanics only. Root movement, corpse pose and physics handoff must be judged separately.')
t=folder/'validation.json.tmp';t.write_text(json.dumps(result,indent=2));t.replace(folder/'validation.json')
print(json.dumps({k:v for k,v in result.items()if k not in ['identity','checks']}))
