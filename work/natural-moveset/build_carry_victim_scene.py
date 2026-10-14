"""Derive an isolated victim-only Midnight scene from the exact native carry loop."""
import copy, hashlib, json
from pathlib import Path
from inspect_midnight_events import parse, walk
P=Path(__file__).resolve().parent
LAB=P.parents[1]/'outputs/d2-coop-lab'
source=P/'carry-research/generated.decls.midnightscene.midnight.gameplay.player.base.carrycorpse.carry_corpse_loop..midnightscene.decl'
original=parse(source.read_text())
d=copy.deepcopy(original);e=d['edit']
def arr(items):return dict(num=len(items),**{f'item[{i}]':v for i,v in enumerate(items)})
def items(a):return [v for k,v in a.items() if k.startswith('item[')]
group=next(x for x in items(e['m_groupsInfo']) if x['m_name']=='AIGroup1')
gid=group['m_UID']['m_objUID'];e['m_groupsInfo']=arr([group])
scene=items(e['m_scenes'])[0];body=scene['m_object']
body['m_groups']=arr([g for g in items(body['m_groups']) if g['m_object']['m_handle']['groupUID']==gid])
assert len(items(body['m_groups']))==1
# Controlled binding experiment: make the sole group's UID compact while
# keeping every scene/track/key reference consistent. Not yet runtime accepted.
group['m_UID']['m_objUID']=1
for o in walk(body):
    if 'groupUID' in o:
        assert o['groupUID']==gid
        o['groupUID']=1
# Keep the native full-body victim clip and its exact native loop graph.
body['m_properties']['m_name']='D2Coop_VictimCarryLoop'
e['m_scenes']=arr([scene]);e['m_nodes']=arr([items(e['m_nodes'])[0]])
e['m_firstElement']={'m_nextType':'EAMNT_SCENE','m_nextCPoint':{'sceneUID':1,'CPointType':'EMNCPT_INPUT'}}
e['m_sceneOut']={'m_groups':{'num':0}}
e['m_graphPositions']={'num':0}
classes=[o['m_keyProp']['m_class'] for o in walk(d) if 'm_keyProp' in o]
assert classes==['arkMidnightKeyPropLookAt','arkMidnightKeyPropAnim'],classes
look=next(o['m_keyProp']['m_object'] for o in walk(d) if o.get('m_keyProp',{}).get('m_class')=='arkMidnightKeyPropLookAt')
assert look['m_influence']=='ARK_LOOKAT_INFLU_NONE'
assert all('PlayerCamera' not in str(o) and 'PlayerRightArm' not in str(o) for o in [e])
def emit(obj,depth=0):
    indent='\t'*depth
    if isinstance(obj,dict):
        return '{\n'+''.join(indent+'\t'+k+' = '+emit(v,depth+1)+(';' if not isinstance(v,dict) else '')+'\n' for k,v in obj.items())+indent+'}'
    if isinstance(obj,str):return obj if obj in ('true','false','NULL') else json.dumps(obj)
    return str(obj)
text=emit(d)+'\n';assert parse(text)==d
path=LAB/'assets/carry_victim_loop.midnightscene';path.write_text(text,encoding='utf-8')
manifest={'source':str(source),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
 'resource':'midnight/d2coop/carry_victim_loop.midnightscene','generated':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),
 'groups':['AIGroup1'],'native_clip':'generic_carrycorpse_idle','duration_ms':3000,
 'camera_tracks':0,'first_person_tracks':0,'event_tracks':0,'native_physics_handoff':'unverified','runtime_accepted':False}
(P/'carry-victim-scene-v1.json').write_text(json.dumps(manifest,indent=2))
s=(LAB/'Build-D2WindblastPresentationAssets.ps1').read_text(encoding='utf-8-sig')
s=s.replace('windblast-presentation','carry-victim').replace('Windblast visual-only','carried victim animation')
s=s.replace("Name='models/effects/d2coop/windblast_remote.fx';File='assets/windblast_remote.fx';Kind='fx';Class='fx'", "Name='midnight/d2coop/carry_victim_loop.midnightscene';File='assets/carry_victim_loop.midnightscene';Kind='midnightscene';Class='midnightScene'")
(LAB/'Build-D2CarryVictimAssets.ps1').write_text(s,encoding='utf-8')
print(json.dumps(manifest,indent=2))
