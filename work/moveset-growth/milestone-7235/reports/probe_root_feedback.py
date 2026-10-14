"""Check real world-root feedback, especially for turns (no asset writes)."""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'generate_locomotion.py').read_text()
source=source.replace("OUT=P/'donors'","OUT=P/'donors-feedback'")
source=source.replace("manifest={'controller'","cases=[dict(name=name,speed=speed,angle=angle,turn=turn) for name,speed,angle,turn in [('walk_forward',100,0,0),('run_forward_right',300,-45,0),('turn_left_90',0,0,90),('turn_right_90',0,0,-90),('turn_left_180',0,0,180),('turn_right_180',0,0,-180)]]\nmanifest={'controller'")
source=source.replace("'donor-manifest.json'","'donor-manifest-feedback.json'")
source=source.replace("def ease(x):",'''def multiply(a,b):
    ax,ay,az,aw=a;bx,by,bz,bw=b
    return [aw*bx+ax*bw+ay*bz-az*by,aw*by-ax*bz+ay*bw+az*bx,aw*bz+ax*by-ay*bx+az*bw,aw*bw-ax*bx-ay*by-az*bz]
def rotate(q,v):return multiply(multiply(q,[*v,0]),[-q[0],-q[1],-q[2],q[3]])[:3]
def ease(x):''')
source=source.replace("angle=math.radians(case['angle'])","angle=math.radians(case['angle']);root_pos=[0.,0.,0.];root_quat=[0.,0.,0.,1.]")
source=source.replace("t=frame/FPS;trajectory=[];distance=0","t=frame/FPS;trajectory=[];distance=0\n            assert abs(root_quat[0])+abs(root_quat[1])<1e-5,'Expected planar root motion'\n            root_yaw=math.degrees(math.atan2(2*root_quat[3]*root_quat[2],1-2*root_quat[2]*root_quat[2]))\n            instance.set_editor_property('control_root',unreal.Transform(location=unreal.Vector(*root_pos),rotation=unreal.Rotator(pitch=0,yaw=root_yaw,roll=0)))")
source=source.replace("unreal.Vector(distance*math.sin(angle),distance*math.cos(angle),0)","unreal.Vector(root_pos[0]+distance*math.sin(angle),root_pos[1]+distance*math.cos(angle),root_pos[2])")
source=source.replace("'has_root_motion':pose['has_root_motion'],'contacts'","'has_root_motion':pose['has_root_motion'],'feedback_position':root_pos[:],'feedback_rotation':root_quat[:],'contacts'")
source=source.replace("        file.write_text",'''            delta=pose['root_delta'];v=rotate(root_quat,delta[:3]);root_pos=[x+y for x,y in zip(root_pos,v)];root_quat=multiply(root_quat,delta[3:7])
            length=math.sqrt(sum(v*v for v in root_quat));root_quat=[v/length for v in root_quat]
        file.write_text''')
exec(compile(source,str(P/'generate_locomotion.py'),'exec'),{'__file__':str(P/'generate_locomotion.py'),'__name__':'__main__'})
