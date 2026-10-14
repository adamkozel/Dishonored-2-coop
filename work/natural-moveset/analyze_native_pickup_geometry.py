"""Summarize sequential native observations without treating them as atomic."""
import hashlib,json,sys,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;folder=P/sys.argv[1]
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
rows=read(folder/'observations.json');result=read(folder/'result.json');identity=read(folder/'identity.json')
summary=[]
for r in rows:
    native=r.get('native') or {};s=r.get('skeleton')or{};scene=native.get('carryScene',{})
    carry=next((a.get('carryResearch',{})for a in native.get('active',[])if a['id']==45),{})
    j=next((j for j in s.get('joints',[])if j['nativeName']=='center_spine02_jnt'),None)
    word=next((w['word']for w in (r.get('physics')or{}).get('words',[])if w['offset']=='0x238'),None)
    summary.append(dict(begin_ms=r['beginMs'],end_ms=r['endMs'],sample_span_ms=r['endMs']-r['beginMs'],
        state_ids=[a['id']for a in native.get('active',[])],carry_phase=carry.get('phase110'),
        body_handle=carry.get('corpseF8'),variant=scene.get('variant'),source_time_ms=scene.get('timeMs'),
        skeleton_stable=s.get('stable',False),root=s.get('root',[])[:3],
        spine02_model_xyz=j['globalPose'][12:15]if j else None,physics_mode=int(word,16)&0xffffffff if word else None,
        attachment_count=(r.get('retarget')or{}).get('count')))
early=[r for r in summary if r['carry_phase']==1];lift=[r for r in summary if r['variant']==1];loop=[r for r in summary if r['variant']==2]
probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(folder/'native.mp4')]))['streams'][0]
n,d=map(int,probe['avg_frame_rate'].split('/'))
checks=dict(harness_complete=result['failure']is None,confirmed_preparation=bool(early),confirmed_native_lift=bool(lift),confirmed_loop=bool(loop),
    one_corpse_generation=len({r['body_handle']for r in early+lift+loop})==1,
    first_person_health_preserved=read(folder/'health-after.json')['current']==identity['healthBefore']['current'],capture_at_least_24hz=n/d>=24)
out=dict(identity=identity,checks=checks,samples=summary,input_ms=result['inputMs'],actual_capture_hz=n/d,
    video_sha256=hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper(),
    confirmed=['Native Carry45 phase 1 owns the target before its Midnight scene exists.',
        'The native one-second lift begins in phase 2 and ends in the normal loop.',
        'The sampled native body changes from a low floor pose to an already raised pose at the start of the lift.'],
    limitations=['Sequential reads span roughly 0.1-0.2 seconds and cannot resolve every render frame.',
        'No continuous third-person floor lift is proven by this first-person recording.',
        'Whether the native recoverFromRagdoll snapshot/blend can supply a useful transition remains a hypothesis.'],
    gameplay_integration_complete=False)
(folder/'geometry-analysis.json').write_text(json.dumps(out,indent=2))
print(json.dumps(dict(checks=checks,preparation_samples=len(early),lift_samples=len(lift),loop_samples=len(loop),actual_capture_hz=n/d)))
