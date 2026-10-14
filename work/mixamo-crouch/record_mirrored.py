"""Record the exact, reversible candidate and preserve its sources and evidence."""
import json,hashlib,shutil,datetime
from pathlib import Path
root=Path(__file__).resolve().parents[2];work=root/'work/mixamo-crouch';lab=root/'outputs/d2-coop-lab'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def write(p,v):p.write_text(json.dumps(v,indent=2)+'\n')
dll=sha(lab/'bin/dinput8.dll');assert dll=='1A46BD1A97E5B263BC9C4AF1F749EFF933FD830C08ABF48699573DADEB21B303'
sources={}
for char,posture,space,stride in [('corvo',35,70,2.04),('emily',50,50,2.05)]:
 p=work/f'source/crouched-walking-{char}-mirrored-inplace-60fps.fbx'
 sources[char]={'File':str(p.relative_to(root)),'Sha256':sha(p),'Posture':posture,'ArmSpace':space,'Speed':50,'Overdrive':50,'Mirror':True,'InPlace':True,'Trim':[0,100],'Export':'Binary FBX 7700, With Skin, 60 fps, no keyframe reduction','CycleDistanceMeters':stride}
report={'At':datetime.datetime.now().astimezone().isoformat(),'Dll':dll,'SourceUrl':'https://www.mixamo.com/#/?page=1&query=Crouched+Walking&type=Motion%2CMotionPack','Clip':'Crouched Walking','Description':'Crouched Walk','Sources':sources,
 'Conversion':read(work/'mirrored-conversion.json'),'Reproduce':'Run prepare_mirrored.py through Blender MCP using preserved source FBXs and native skeleton/standing/crouching captures; copy generated mirrored clip/grip headers into src; Build-D2Lab.ps1. Native transforms are unchanged except mapped rotations, authored pelvis offset, and the captured weapon-socket shift. No runtime FBX loader.',
 'Cadence':'One cycle per 2.04 m Corvo / 2.05 m Emily, approximately one third slower than previous 1.356/1.367 m cycles. This is user-directed artistic tuning, not a newly measured no-slip stride. No phase advance without horizontal presentation-root travel.',
 'Grip':'Native NPC closed-hand sword_idle wrist/finger pose retained. Observed first-person attachment flip (~179.78 degrees) transferred through anatomical palm axes derived from thumb/pinky wrist children to NPC attachment rotation and translation. The rest of the first-person arm pose is not copied. Native first-person rigs remain untouched.',
 'SessionMode':'coop_mixamo_crouch on explicitly enables the existing actor/world/session until off or invalidation; 30000 retains the bounded test. Per-frame freshness, known rig, native equipment arming and standing/airborne gates remain. Teleport-sized crouch steps cancel. Checkpoint restoration requires fresh native reference and explicit re-arm.',
 'Runtime':{c:read(work/f'mirror-{c}-runtime.json') for c in ['corvo','emily']},
 'Verified':['Both mirrored sources place the sword wrist ahead of the free wrist throughout the complete baked cycle','Measured forward travel matches phase; phase holds at rest for both characters','Native standing recovery stops donor writes and recrouching resumes','Both first-person arm rigs remain native; real slash observation passed','Preceding 1031 build (same health code): native health-damage protection: both roles, actor/sword ownership both directions, assassination, pause and duplicates','Preceding 1031 build (same health code): real sword contact preserves 56 player / 40 clone health','Preceding 1031 build: sword render transform agrees with native attachment within 0.000008 m in coherent samples','Original executable, Steam DLL and isolated save/config baseline verified'],
 'VisualEvidence':['work/mixamo-crouch/mirror-corvo-side.png','work/mixamo-crouch/mirror-emily-view.png','work/mixamo-crouch/mirror-restored-corvo.png','work/mixamo-crouch/mirror-restored-emily.png','work/mixamo-crouch/corvo-native-crouch-reference.png'],
 'Limitations':['Forward donor only: backwards/sideways still reuse the forward cycle; turning, slopes, foot locking and additional clips remain','Slower artistic cadence can increase foot sliding; no claim of contact-perfect gait','Grip flip implemented, but full crouched attack/parry transitions and visual parity throughout combat remain unfinished','After native health plus real-strike regression, strict native avatar-sword query failed for both clones and explicit donor arm refused. Health stayed unchanged. Exact failing identity field/trigger not isolated. Protected fixture restored; do not claim disarm or all non-health harm prevention','No jumping/sliding/climbing/leaning/power coverage established by this clip','No automatic activation across checkpoint/world replacement; still an isolated explicit experiment'],
 'ExcludedEvidence':['mirror-corvo-start.png observer pointed away','mirror-final-corvo.png and mirror-final-*-state.json were captured after equipment refusal with donor inactive; not candidate proof','Early motion check sampled before asynchronous input hold ended; harness corrected and rerun'],
 'Protected':{'OriginalSteamAssetsModified':False,'FirstPersonModified':False,'StableMilestone':'work/friendly-fire-345C','PriorCandidate':'work/mixamo-crouch/008c-before-mirror'}}
write(lab/'research/mixamo-mirrored-1a46.json',report)
feas=read(lab/'research/mixamo-crouch-feasibility.json');feas['status']='Both_roles_mirrored_distance_driven_runtime_verified_crouch_flip_candidate';feas['decision']='Continue isolated mirrored candidate 1a46; user-requested slower cadence and front sword arm implemented; preserve stable 345C. Full crouched combat and post-contact equipment handling remain open.';feas['LatestCandidate']='research/mixamo-mirrored-1a46.json';feas['resumeRecipe']=['Read latest candidate report and live state first; previous login blocker is resolved.','Use existing authenticated Mixamo and connected Blender; sources and conversions pinned.','Restore using Test-D2PlayerArms.ps1 -Doppelganger only when needed, bind follow exact, Enable-D2ShadowCrouch.ps1 -Style upright, then explicit coop_mixamo_crouch on for each role.','Continue crouched combat/equipment investigation without claiming complete moveset or no-harm coverage.'];write(lab/'research/mixamo-crouch-feasibility.json',feas)
statePath=root/'work/current-task-state.json';state=read(statePath)
if not (work/'state-before-mirrored-record.json').exists():shutil.copy2(statePath,work/'state-before-mirrored-record.json')
state['UpdatedAt']=report['At'];state['LoaderSha256']=dll
state['ActiveTest']={'Dll':dll,'Fixture':'Two isolated physical-copy instances, native Steam sockets, windowed, background physical input disabled','Experiment':'Mirrored Mixamo crouch with native attachment flip; Corvo posture35/space70, Emily50/50','Status':'Both roles measured, visual candidate live; explicit per-session activation','Reports':['research/mixamo-mirrored-1a46.json'],'FullMovesetReady':False}
state['MixamoExperiment']={'Status':'Mirrored candidate live; slower distance phase, native sword flip and standing recovery verified','Clip':'Crouched Walking','Description':'Crouched Walk','InPlaceRequired':True,'UserLikesPreview':True,'BlenderMcpConnected':True,'DonorDownloaded':True,'RuntimeImprovementProven':True,'LatestVisualApprovalPending':True,'Report':'research/mixamo-mirrored-1a46.json'}
state['WorkInProgress']='Crouch candidate 1a46 uses mirrored donor and slower phase for both roles. Native sword attachment flip and socket shift transferred. Stable 345C archived. Friendly-fire health checks pass but post-contact native sword validation failure needs investigation; broader no-harm coverage remains incomplete.'
state['Next']='Investigate post-contact sword ownership/equipment failure and crouched sword-action blending; complete directional clips/foot sliding tests. Preserve current candidate and native first-person behavior; continue broader co-op requirements.'
state['Live']=[]
for role in ['host','client']:
 l=read(root/f'work/lab-instances/{role}/launch.json');b=read(root/f'work/lab-instances/{role}/coop-status.json');m=read(root/f'work/lab-instances/{role}/coop-status.json.mixamo.json')
 state['Live'].append({'Role':role,'Pid':l['pid'],'Epoch':b['localEpoch'],'Handle':b['probeActorHandle'],'Follow':b['probeFollowEnabled'],'NativeInterface':b['nativeInterface'],'BodyStream':b['bodyStreamEnabled'],'NativeCrouchReferenceReady':b['nativeCrouchReferenceReady'],'Mixamo':m})
state['FriendlyFireRequirement']['Remaining'].append('Post-contact native sword validation failure: isolate weapon/equipment side effects despite unchanged health')
write(statePath,state)
archive=work/'1a46-mirrored-candidate';archive.mkdir(exist_ok=True)
for folder in ['src','assets','bin']:
 if not (archive/folder).exists():shutil.copytree(lab/folder,archive/folder)
for name in ['Build-D2Lab.ps1','Start-D2Lab.ps1','Restart-D2Lab.ps1','Enable-D2ShadowCrouch.ps1','Enable-D2MixamoCrouch.ps1']:shutil.copy2(lab/name,archive/name)
for name in ['prepare_mirrored.py','Test-MirroredCandidate.ps1','mirrored-conversion.json','after-contact-equipment-blocker.json']:shutil.copy2(work/name,archive/name)
shutil.copy2(lab/'research/mixamo-mirrored-1a46.json',archive/'report.json')
manifest={str(p.relative_to(archive)):sha(p) for p in archive.rglob('*') if p.is_file() and p.name!='manifest.json'}
write(archive/'manifest.json',{'Dll':dll,'Files':manifest,'Donors':sources,'NativeInputs':{p.name:sha(p) for p in work.glob('*fp-*.json')}})
print(json.dumps({'Dll':dll,'Archive':str(archive),'Files':len(manifest),'Live':[{'Role':v['Role'],'Active':v['Mixamo']['active']} for v in state['Live']]}))


