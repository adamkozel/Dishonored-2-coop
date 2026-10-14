"""Cosmetic native Mesmerize model/scenes, excluding AI and gameplay tracks."""
import copy,hashlib,json
from pathlib import Path
from inspect_midnight_events import parse,walk
P=Path(__file__).resolve().parent;LAB=P.parents[1]/'outputs/d2-coop-lab'
def arr(a):return dict(num=len(a),**{f'item[{i}]':v for i,v in enumerate(a)})
def items(a):return [v for k,v in a.items()if k.startswith('item[')]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def emit(o,depth=0):
 indent='\t'*depth
 if isinstance(o,dict):return '{\n'+''.join(indent+'\t'+k+' = '+emit(v,depth+1)+(';'if not isinstance(v,dict)else'')+'\n'for k,v in o.items())+indent+'}'
 if isinstance(o,str):return o if o in ('true','false','NULL')else json.dumps(o)
 return str(o)
manifest=[]
for phase in ('in','loop','out'):
 src=LAB/f'research/asset-exports/mesmerize-world-scenes-v76/game1_patch/generated.decls.midnightscene.midnight.gameplay.powers.mesmerize_v2.mesmerize_v2_{phase}..midnightscene.decl'
 d=copy.deepcopy(parse(src.read_text()));e=d['edit'];groups=items(e['m_groupsInfo'])
 assert len(groups)==1 and groups[0]['m_name']=='Mesmerize'
 assert items(groups[0]['m_declsMD6'])==['models/interactive/animated/mesmerize_v2/mesmerize_v2.md6']
 groups[0]['m_enableFeetIK']='false';removed=[];scenes=[]
 for scene in items(e['m_scenes']):
  body=scene['m_object'];sg=items(body['m_groups']);assert len(sg)==1
  tracks=items(sg[0]['m_object']['m_lTracks']);removed.extend(t['m_object']['m_trackType']for t in tracks if t['m_object']['m_trackType']!='EMTT_ANIM')
  sg[0]['m_object']['m_lTracks']=arr([t for t in tracks if t['m_object']['m_trackType']=='EMTT_ANIM'])
  assert sg[0]['m_object']['m_lTracks']['num']==1
  scenes.append(dict(name=body['m_properties']['m_name'],duration=body['m_properties']['m_length']))
 e['m_sceneOut']={'m_groups':{'num':0}};e['m_graphPositions']={'num':0}
 assert set(o['m_keyProp']['m_class']for o in walk(d)if'm_keyProp'in o)=={'arkMidnightKeyPropAnim'}
 text=emit(d)+'\n';assert parse(text)==d
 assert all(x not in text for x in ['EMTT_TIMEDEVENT','EMTT_ACTION','EMTT_FX','PlayerBody','PlayerCamera','PlayerLeftArm'])
 dest=LAB/f'assets/mesmerize_visual_{phase}.midnightscene';dest.write_text(text,encoding='utf-8')
 manifest.append(dict(resource=f'midnight/d2coop/mesmerize_visual_{phase}.midnightscene',generated=str(dest),sha256=sha(dest),source=str(src),source_sha256=sha(src),scenes=scenes,removed_tracks=removed,event_tracks=0,runtime_verified=False,kind='midnightscene',classname='midnightScene'))
components=['components/interactive/animevolved_midnight.cpntanimevolved','components/base/midnightproxy_default.cpntmidnightproxy','components/base/audio_default.cpntaudio','components/base/referenceframe_default.cpntreferenceframe']
keys=['uNAP91','j9Gs01','zXcHv3','U_1E92']
definition={'class':'idEntity','edit':{'renderModelInfo':{'model':'models/interactive/animated/mesmerize_v2/mesmerize_v2.md6'},'m_componentDecls':{'item_begin':'true',**{f'item_add["{keys[i]}"]':{'inheritedDecl':x}for i,x in enumerate(components)}}}}
dest=LAB/'assets/mesmerize_visual.def';text=emit(definition)+'\n'
# The Midnight parser supports numeric arrays only. Normalize the four exact
# native named component keys solely for structural round-trip validation.
normalized=copy.deepcopy(definition)
for i,key in enumerate(keys):
 named=f'item_add["{key}"]';plain=f'component{i}'
 normalized['edit']['m_componentDecls'][plain]=normalized['edit']['m_componentDecls'].pop(named)
check_text=text
for i,key in enumerate(keys):check_text=check_text.replace(f'item_add["{key}"]',f'component{i}')
assert parse(check_text)==normalized;dest.write_text(text,encoding='utf-8')
assert all('mesmerize'not in c and 'attention'not in c and 'attributes'not in c for c in components)
manifest.append(dict(resource='models/powers/d2coop/mesmerize_visual.def',generated=str(dest),sha256=sha(dest),components=components,gameplay_components=0,runtime_verified=False,kind='entitydef',classname='entityDef'))
(P/'mesmerize-visual-assets-v76.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
s=(LAB/'Build-D2CarryVictimAssets.ps1').read_text(encoding='utf-8-sig').replace('carry-victim','mesmerize-visual-v76').replace('carried victim animation','cosmetic Mesmerize model animation')
old="$resources=@(@{Name='midnight/d2coop/carry_victim_loop.midnightscene';File='assets/carry_victim_loop.midnightscene';Kind='midnightscene';Class='midnightScene'})"
assert old in s
new='$resources=@('+','.join("@{Name='"+m['resource']+"';File='assets/"+Path(m['generated']).name+"';Kind='"+m['kind']+"';Class='"+m['classname']+"'}"for m in manifest)+')'
(LAB/'Build-D2MesmerizeVisualAssets.ps1').write_text(s.replace(old,new),encoding='utf-8')
print(json.dumps(dict(resources=len(manifest),event_tracks=0,gameplay_components=0,runtime_verified=False)))
