"""Bounded comparison against the sample's second pretrained controller."""
import unreal,json,pathlib
P=pathlib.Path(__file__).resolve().parent
report={}
for name in ('AGC_AnimGenCharacter','AGC_AnimGenNPC'):
 obj=unreal.load_asset('/AnimGenExample/Characters/UEFN_Mannequin/Animations/'+name)
 report[name]=json.loads(unreal.D2ProbeLibrary.describe_object(obj))
(P/'controller-properties.json').write_text(json.dumps(report,indent=2))
source=(P/'generate_locomotion.py').read_text()
source=source.replace("OUT=P/'donors'","OUT=P/'donors-npc'").replace("Animations/AGC_AnimGenCharacter'","Animations/AGC_AnimGenNPC'")
source=source.replace("manifest={'controller'","cases=[dict(name=name,speed=speed,angle=angle,turn=0) for name,speed,angle in [('walk_forward',100,0),('run_forward',300,0),('sprint_forward',500,0),('run_left',300,90),('run_back',300,180)]]\nmanifest={'controller'")
source=source.replace("'donor-manifest.json'","'donor-manifest-npc.json'")
exec(compile(source,str(P/'generate_locomotion.py'),'exec'),{'__file__':str(P/'generate_locomotion.py'),'__name__':'__main__'})
