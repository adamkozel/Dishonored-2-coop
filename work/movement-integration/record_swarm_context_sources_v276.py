from pathlib import Path
import json,hashlib,datetime,subprocess,collections,struct
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_devouring_swarm_cast'];summaries=[]
for take,low,empty in [('swarm-native-crouched-observe-v271',1,False),('swarm-native-standing-holstered-observe-v273',0,True),('swarm-native-crouched-holstered-observe-v275',1,True)]:
 t=P/take;r=read(t/'result.json');assert r['passed'] and (t/'source-sheet.jpg').exists()
 rows=read(t/'states.json');assert all(x['player']['stance']==low and x['sword']['equipped']!=empty for x in rows)
 summary=dict(result=r,observations=read(t/'swarm-observations.json'),rows=len(rows),contextPreserved=True,complete=False,visualAccepted=False)
 write(t/'coverage-summary.json',summary);summaries.append(summary)
 review=f'{take}/visual-review.json'
 write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Actual-game source temporal sheet at 1,2.1,2.7,3.6,4.5,5.5 seconds',evidence=['source-sheet.jpg'],visual_accepted=False,complete=False,notes='Native Corvo left-hand cast, targeting preview and spawned rats visible. Requested grounded stance and sword context persist throughout all recorded source rows. Local first-person remains native. Source-only observation does not establish remote body, remote rats/VFX, partner safety or shared enemy outcomes. AI held; original inventories and health restored, checkpoint retires swarm, protected14 passed.'))
 video=P/(take+'.mp4');fp=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));a,b=map(float,fp['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='corvo',label=f"Swarm native source · {r['stance']} · {'holstered' if empty else 'armed'} · {take.rsplit('-',1)[1]}",alias='swarm_native_observation',requirements=['corvo_devouring_swarm_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=a/b,video_sha256=digest,checks_passed=True,scope=r['scope'])
 videos=[x for x in videos if x['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
write(P/'runtime-video-manifest.json',videos)
item['notes']+=' Source contexts v271/v273/v275 each PASS6: crouched armed, standing holstered and crouched holstered; exact phase durations and native sword/stance persist. v274 retained failure was one non-atomic terminal sample with empty bindings after eight reciprocal samples of the same scene; v275 now bounds such terminal observations without weakening runtime identity checks. Remote context binding is pending verification.'
write(P/'review-evidence-annotations.json',ann)
write(W/'swarm-source-context-matrix-v276.json',dict(contexts=summaries,retainedFailure=read(P/'swarm-native-crouched-holstered-observe-v274/result.json'),complete=False))
# Keep raw runtime fields separate from static semantic interpretations.
take=P/'swarm-native-crouched-holstered-observe-v274';raw=[read(take/f'world-components-{i}.json') for i in range(2)]
def words(obj):return b''.join(int(x['word'],16).to_bytes(8,'little') for x in obj['words'])
agents=[]
for i in range(3):
 records=[]
 for snap in raw:
  data=words(snap['liveRatPrefix']);off=i*0x108
  records.append(dict(videoTime=snap['videoTime'],referenceFrame=hex(struct.unpack_from('<Q',data,off+8)[0]),entity=hex(struct.unpack_from('<Q',data,off+16)[0]),ownerComponent=hex(struct.unpack_from('<Q',data,off+24)[0]),positionCandidate=list(struct.unpack_from('<3f',data,off+0xd0)),initialPositionCandidate=list(struct.unpack_from('<3f',data,off+0x34))))
 agents.append(dict(index=i,samples=records))
write(W/'swarm-live-rat-observations-v276.json',dict(source='swarm-native-crouched-holstered-observe-v274/world-components-{0,1}.json',observedStride=264,agentPrefixSamples=agents,confirmed='RTTI arkRat at component+0x80, three repeated vtables at0x108 stride, owner component reciprocal in each sampled prefix; +8 reference-frame RTTI, +0x10 swarm-agent entity RTTI, +0x78 rat-animation component RTTI.',hypotheses='Live positions at +0xD0 change while initial positions at +0x34 remain fixed. Requires reciprocal entity/reference-frame validation and static setter/update trace before using them as network transforms. Array at component+0x68 is initialization data, not proven live coordinates.',remoteWorldImplemented=False))
print(json.dumps(dict(sourceContexts=len(summaries),videos=len(videos),liveAgentPrefixes=len(agents))))
