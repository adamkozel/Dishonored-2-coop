"""Decode every selected review movie and verify its saved animation sources.

This is delivery integrity, not visual acceptance. Content hashes cache costly
video decoding without accepting changed footage on a later audit.
"""
import json,hashlib,subprocess,datetime,concurrent.futures
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
catalog=read(P/'review-catalog.json');cachefile=P/'recording-decode-cache.json';cache=read(cachefile)if cachefile.exists()else{}
entries=[];source_errors=[]
for m in catalog['motions']:
 if set(m['roles'])!={'corvo','emily'}:source_errors.append(dict(id=m['id'],error='Both roles required'))
 for role,r in m['roles'].items():
  p=P/r['binary']
  if not p.is_file()or sha(p)!=r['sha256']:source_errors.append(dict(id=m['id'],role=role,error='Missing or changed animation bytes'))
 for v in m['videos']:
  side=read((P/v['path']).with_suffix('.json'));entries.append(dict(id=m['id'],path=v['path'],frames=side['frames'],fps=side['fps'],kind='authoring'))
for v in catalog['game_recordings']:entries.append(dict(id=v.get('alias')or v['video'],path=v['video'],kind='game'))
def check(entry):
 path=P/entry['path'];digest=sha(path);prior=cache.get(entry['path'])
 if prior and prior.get('sha256')==digest and prior.get('passed'):decoded=prior['decoded']
 else:
  run=subprocess.run(['ffprobe','-v','error','-count_frames','-select_streams','v:0','-show_entries','stream=codec_name,width,height,avg_frame_rate,nb_read_frames,duration','-of','json',str(path)],capture_output=True,text=True,timeout=180)
  if run.returncode or run.stderr.strip():return dict(**entry,sha256=digest,passed=False,error=run.stderr.strip()or'ffprobe failed')
  streams=json.loads(run.stdout)['streams']
  if len(streams)!=1:return dict(**entry,sha256=digest,passed=False,error='Expected one video stream')
  decoded=streams[0]
 valid=int(decoded['nb_read_frames'])>1 and decoded['width']>0 and decoded['height']>0
 if entry['kind']=='authoring':valid=valid and int(decoded['nb_read_frames'])==entry['frames'] and abs(float(decoded['duration'])-entry['frames']/entry['fps'])<.035
 return dict(**entry,sha256=digest,passed=valid,decoded=decoded)
with concurrent.futures.ThreadPoolExecutor(max_workers=4)as pool:
 results=list(pool.map(check,entries))
cache.update({r['path']:r for r in results});cachefile.write_text(json.dumps(cache,indent=2))
report=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=not source_errors and all(r['passed']for r in results),scope='Source byte identity and complete video decoding only; no visual or gameplay acceptance',movies=len(results),paired_role_movies=sum(r['kind']=='authoring'for r in results),actual_game_movies=sum(r['kind']=='game'for r in results),duration_seconds=sum(float(r.get('decoded',{}).get('duration',0))for r in results),source_errors=source_errors,failures=[r for r in results if not r['passed']],videos=results)
(P/'recording-integrity.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:v for k,v in report.items()if k!='videos'},indent=2),flush=True)
raise SystemExit(0 if report['passed']else 1)
