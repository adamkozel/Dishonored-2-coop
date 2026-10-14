"""Produce independent seeded locomotion donors using the proven native evaluator.

Only evaluated pose/root/contact data is saved. No native D2 files or sample
assets are changed. Directional/loop suitability is assessed after generation.
"""
import json, math, pathlib, unreal

P=pathlib.Path(__file__).resolve().parent
OUT=P/'donors';OUT.mkdir(exist_ok=True)
FPS=60
CONTROLLER='/AnimGenExample/Characters/UEFN_Mannequin/Animations/AGC_AnimGenCharacter'
MESH='/AnimGenExample/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin'
actors=unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
directions=[('forward',0),('forward_left',45),('left',90),('back_left',135),('back',180),('back_right',-135),('right',-90),('forward_right',-45)]
cases=[{'name':'idle','speed':0,'angle':0,'turn':0}]
for gait,speed in (('walk',100),('run',300),('sprint',500)):
    for direction,angle in directions:
        cases.append({'name':gait+'_'+direction,'speed':speed,'angle':angle,'turn':0})
for turn in (-180,-90,90,180):
    cases.append({'name':'turn_'+('left' if turn>0 else 'right')+'_'+str(abs(turn)),'speed':0,'angle':0,'turn':turn})

def ease(x):x=max(0,min(1,x));return x*x*(3-2*x)
def speed(t,case):return case['speed']*ease((t-1)/.5)*(1-ease((t-6.5)/.5))
def yaw(t,case):return -case['turn']*ease((t-2)/1.5)

manifest={'controller':CONTROLLER,'mesh':MESH,'fps':FPS,'seed':1234,'cases':[],'claims':'Donor generation only; not a retargeted or playable moveset'}
for case in cases:
    file=OUT/(case['name']+'.json')
    if file.exists():raise RuntimeError('Never overwrite existing donor '+str(file))
    actor=actors.spawn_actor_from_class(unreal.SkeletalMeshActor,unreal.Vector(),transient=True)
    component=actor.get_component_by_class(unreal.SkeletalMeshComponent)
    report={'name':case['name'],'request':case,'fps':FPS,'seed':1234,'controller':CONTROLLER,'frames':[]}
    try:
        component.set_skeletal_mesh_asset(unreal.load_asset(MESH))
        component.set_anim_instance_class(unreal.D2AnimGenInstance)
        instance=component.get_anim_instance()
        instance.set_editor_property('controller',unreal.load_asset(CONTROLLER))
        instance.set_editor_property('desired_tags',[])
        instance.set_editor_property('control_root',unreal.Transform())
        angle=math.radians(case['angle'])
        for frame in range(8*FPS):
            t=frame/FPS;trajectory=[];distance=0
            for k in range(61):
                future=k/FPS
                if k:distance+=speed(t+future-.5/FPS,case)/FPS
                sample=unreal.TransformTrajectorySample()
                sample.set_editor_property('position',unreal.Vector(distance*math.sin(angle),distance*math.cos(angle),0))
                a=math.radians(yaw(t+future,case))
                sample.set_editor_property('facing',unreal.Quat(0,0,math.sin(a/2),math.cos(a/2)))
                sample.set_editor_property('time_in_seconds',future)
                trajectory.append(sample)
            tr=unreal.TransformTrajectory();tr.set_editor_property('samples',trajectory)
            instance.set_editor_property('trajectory',tr)
            pose=json.loads(unreal.D2ProbeLibrary.step_and_read_pose(component,1/FPS))
            assert 'error' not in pose,pose
            assert instance.get_editor_property('controls_valid'),'Native control schema mismatch'
            assert all(math.isfinite(v) for row in pose['local'] for v in row),'Nonfinite pose'
            if not frame:report.update({'names':pose['names'],'parents':pose['parents']})
            report['frames'].append({'time':t,'local':pose['local'],'root_delta':pose['root_delta'],
                'has_root_motion':pose['has_root_motion'],'contacts':[instance.get_curve_value('contact_l'),instance.get_curve_value('contact_r')]})
        file.write_text(json.dumps(report,separators=(',',':')))
        manifest['cases'].append({'name':case['name'],'frames':len(report['frames']),'file':str(file),'schema_valid':True})
        (P/'donor-manifest.json').write_text(json.dumps(manifest,indent=2))
        unreal.log('D2_MOVESET_GENERATED '+case['name'])
    finally:actors.destroy_actor(actor)
unreal.log('D2_MOVESET_BATCH_COMPLETE '+str(len(cases)))
