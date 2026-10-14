"""Verify cancellation before attachment is created, including native cleanup."""
import json,sys,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
folder=P/sys.argv[1];rows=read(folder/'observations.json');identity=read(folder/'identity.json')
initial=rows[0]['attachment'];final=rows[-1]['attachment']
active=[r for r in rows if r['attachment'].get('snapshotBound') and r['attachment'].get('attachmentPending')]
after=[r for r in rows if r['attachment']['released']>initial['released']and not r['attachment']['active']]
events=read(folder/'events.json');interruption=read(folder/'interruption.json')['interruption']
assert interruption in ['cancel','stream']
command='coop_carry_attachment off' if interruption=='cancel' else 'coop_shadow_stream off'
event=next((e for e in events if e['input'].get('command')==command),None)
def word(subject,phase,offset):return int(next(w['word']for w in read(folder/f'corpse-{subject}-{phase}.json')['words']if w['offset']==offset),16)
checks=[]
def check(name,ok):checks.append(dict(check=name,passed=bool(ok)))
check('harness completed',read(folder/'result.json')['failure']is None)
check('snapshot and pending attachment observed before cancel',active and event and active[0]['ms']<=event['ms'])
check('no reciprocal attachment created',all(r['retarget']['valid']and r['retarget']['count']==0 for r in rows) and final['started']==initial['started'])
check('one release completes within 1.2s after cancel',event and after and 0<=after[0]['ms']-event['ms']<1200 and final['released']==initial['released']+1)
check('snapshot bindings and fixture stay retired',after and all(not r['attachment']['snapshotBound']and not r['attachment']['active']for r in after))
check('corpse returns to native ragdoll mode',all((word('physics',p,'0x238')&0xffffffff)==0 for p in ['before','after']))
check('native carried flag restored',all((word('object',p,'0x368')&2)==0 for p in ['before','after']))
check('no victim scene left active',not any(s.get('asset','').startswith('midnight/d2coop/carry_victim')for s in read(folder/'midnight-after.json')['instances']))
check('corpse stays dead',read(folder/'corpse-health-before.json')['current']==read(folder/'corpse-health-after.json')['current']==0)
check('local first-person health preserved',read(folder/'health-after.json')['current']==identity['healthBefore']['current'])
check('remote avatar health preserved',read(folder/'avatar-health-before.json')['current']==read(folder/'avatar-health-after.json')['current'])
check('Steam transport stays connected',all(r['observerRoot']['connectionState']==3 for r in rows))
if interruption=='stream':
    resume=next((e for e in events if e['input'].get('command')=='coop_shadow_stream on'),None)
    resumed=[r for r in rows if resume and resume['ms']+700<=r['ms']<resume['ms']+3000]
    check('fresh two-sided action handshake resumes carrying',any(r['stream'].get('ready') and r['stream'].get('remoteState')==14 for r in resumed))
    check('retired snapshot and attachment do not replay on resume',resumed and all(not r['attachment']['active'] and not r['attachment']['snapshotBound'] and r['retarget']['count']==0 for r in resumed))
probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(folder/'native.mp4')]))['streams'][0]
n,d=map(int,probe['avg_frame_rate'].split('/'));hz=n/d
check('at least 24 captured frames per second',hz>=24)
out=dict(checks=checks,passed=all(c['passed']for c in checks),samples=len(rows),active_snapshot_samples=len(active),after_samples=len(after),interruption=interruption,
    cleanup_delay_ms=after[0]['ms']-event['ms']if after and event else None,actual_capture_hz=hz,
    video_sha256=hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper(),
    complete=False,visual_accepted=False,scope='Explicit cancellation during the unattached pose blend. No complete pickup or general world-body authority demonstrated.')
(folder/'snapshot-cancel-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(out))
