"""Build victim-only native drop clips, with physics timing owned by the fixture.

No camera, player arms, inventory, damage, spawning or native event tracks are
included. Native ragdoll event times are retained as explicit metadata. The
original declarations and the established carry-loop derivative stay intact.
"""
import copy, hashlib, json
from pathlib import Path
from inspect_midnight_events import parse, walk

P = Path(__file__).resolve().parent
LAB = P.parents[1] / 'outputs/d2-coop-lab'

def items(a): return [v for k,v in a.items() if k.startswith('item[')]
def arr(a): return dict(num=len(a), **{f'item[{i}]':v for i,v in enumerate(a)})
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def emit(obj, depth=0):
    indent = '\t' * depth
    if isinstance(obj,dict):
        return '{\n' + ''.join(indent+'\t'+k+' = '+emit(v,depth+1)+(';' if not isinstance(v,dict) else '')+'\n' for k,v in obj.items()) + indent+'}'
    if isinstance(obj,str): return obj if obj in ('true','false','NULL') else json.dumps(obj)
    return str(obj)

manifest = []
for suffix, variant, ragdoll in [('drop_low_stand',4,656),('drop_low_sneak',6,500)]:
    source = P/f'carry-research/generated.decls.midnightscene.midnight.gameplay.player.base.carrycorpse.carry_corpse_{suffix}..midnightscene.decl'
    d = copy.deepcopy(parse(source.read_text()))
    e = d['edit']
    group = next(g for g in items(e['m_groupsInfo']) if g['m_name']=='AIGroup1')
    gid = group['m_UID']['m_objUID']
    e['m_groupsInfo'] = arr([group]); group['m_UID']['m_objUID'] = 1
    scene = items(e['m_scenes'])[0]; body = scene['m_object']
    victim = next(g for g in items(body['m_groups']) if g['m_object']['m_handle']['groupUID']==gid)
    events = [o for o in walk(victim) if 'm_keyProp' in o and o['m_keyProp']['m_object'].get('m_event')=='ARK_MDNT_MSG_GO_TO_RAGDOLL']
    assert len(events)==1 and events[0]['m_startTime']==ragdoll
    native_tracks = items(victim['m_object']['m_lTracks'])
    victim['m_object']['m_lTracks'] = arr([t for t in native_tracks if t['m_object']['m_trackType'] in ('EMTT_LOOKAT','EMTT_ANIM')])
    body['m_groups'] = arr([victim])
    for o in walk(body):
        if 'groupUID' in o:
            assert o['groupUID']==gid
            o['groupUID']=1
    body['m_properties']['m_name']='D2Coop_Victim_'+suffix
    e['m_scenes']=arr([scene]);e['m_nodes']={'num':0}
    e['m_firstElement']={'m_nextType':'EAMNT_SCENE','m_nextCPoint':{'sceneUID':1,'CPointType':'EMNCPT_INPUT'}}
    e['m_sceneOut']={'m_groups':{'num':0}};e['m_graphPositions']={'num':0}
    classes=[o['m_keyProp']['m_class'] for o in walk(d) if 'm_keyProp' in o]
    assert sorted(classes)==['arkMidnightKeyPropAnim','arkMidnightKeyPropLookAt']
    text=emit(d)+'\n';assert parse(text)==d
    assert 'PlayerBody' not in text and 'PlayerRightArm' not in text and 'EMTT_EVENT' not in text
    dest=LAB/f'assets/carry_victim_{suffix}.midnightscene';dest.write_text(text,encoding='utf-8')
    manifest.append(dict(variant=variant,source=str(source),source_sha256=sha(source),generated=str(dest),sha256=sha(dest),
        resource=f'midnight/d2coop/carry_victim_{suffix}.midnightscene',ragdoll_ms=ragdoll,
        duration_ms=body['m_properties']['m_length'],native_events_executed=False,runtime_accepted=False))

(P/'carry-release-scenes-v1.json').write_text(json.dumps(manifest,indent=2))
s=(LAB/'Build-D2CarryVictimAssets.ps1').read_text(encoding='utf-8-sig')
s=s.replace('carry-victim','carry-release').replace('carried victim animation','carried victim release animation')
old="$resources=@(@{Name='midnight/d2coop/carry_victim_loop.midnightscene';File='assets/carry_victim_loop.midnightscene';Kind='midnightscene';Class='midnightScene'})"
new='$resources=@('+','.join("@{Name='"+m['resource']+"';File='assets/"+Path(m['generated']).name+"';Kind='midnightscene';Class='midnightScene'}" for m in manifest)+')'
assert s.count(old)==1
(LAB/'Build-D2CarryReleaseAssets.ps1').write_text(s.replace(old,new),encoding='utf-8')
print(json.dumps(manifest,indent=2))
