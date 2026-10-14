import json,hashlib,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
for take in sys.argv[1:]:
 f=P/take;rows=read(f/'observations.json');i=read(f/'identity.json');events=read(f/'events.json');after=read(f/'after.json');health=read(f/'health-after.json')
 high=[(r,s)for r in rows for s in r['carryScenes']if s['asset'].endswith('carry_corpse_drop_high.midnightscene')]
 lift=[r for r in rows if r['native']and r['native'].get('carryScene',{}).get('variant')==1]
 loaded=[r for r in rows if r['native']and r['native'].get('carryScene',{}).get('variant')==2]
 probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(f/'native.mp4')]))
 a,b=map(int,probe['streams'][0]['avg_frame_rate'].split('/'));hz=a/b
 checks=dict(native_lift_observed=bool(lift),native_loaded_observed=bool(loaded),right_trigger_sent=any(e['input'].get('rt')==255 for e in events),high_scene_observed=bool(high),high_duration_666ms=bool(high)and all(s['sceneDurationMs']==666 for r,s in high),drop46_correlated=bool(high)and all(any(a['id']==46 for a in r['native']['active'])for r,s in high if r['native']),carry_retired=not any(a['id']in[45,46]for a in after['active']),health_preserved=health['current']==i['healthBefore']['current'],capture_24hz=hz>=24)
 out=dict(passed=all(checks.values()),checks=checks,role=i['role'],source='Native first-person source observation only',highSamples=len(high),highObservedMs=[min((r['ms']for r,s in high),default=0),max((r['ms']for r,s in high),default=0)],highDurations=sorted({s['sceneDurationMs']for r,s in high}),highSubscenes=sorted({s['subscene']for r,s in high}),nativeDropVariantsExplicitlyUnrecognized=True,actual_capture_hz=hz,video_sha256=hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper(),proxy_sha256=i['proxySha256'],probe_sha256=i['probeSha256'],example=high[0][1]if high else None,complete=False,remoteThrowImplemented=False,scope='Normal X pickup followed by RT from standing carry on the known Mayhew fixture; source scene identity/timing correlation, not replicated throw physics or full native state phase validation.')
 (f/'native-throw-observation-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps({k:v for k,v in out.items()if k!='example'}))
