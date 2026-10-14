"""Reproducible native cosmetic FX/material derivatives; does not install them."""
from pathlib import Path
import copy,hashlib,json
from inspect_midnight_events import parse,walk
R=Path(__file__).resolve().parents[2];L=R/'outputs/d2-coop-lab';A=L/'assets'
def arr(xs):return {'num':len(xs),**{f'item[{i}]':x for i,x in enumerate(xs)}}
def items(d):return [d[f'item[{i}]']for i in range(d['num'])]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def emit(o,n=0):
 if isinstance(o,dict):return '{\n'+''.join('\t'*(n+1)+k+' = '+emit(v,n+1)+(';'if not isinstance(v,dict)else'')+'\n'for k,v in o.items())+'\t'*n+'}'
 if isinstance(o,str):return o if o in ('true','false','NULL')else json.dumps(o)
 return str(o)
manifest=[]
def write(d,path,resource,kind,classname,source,**details):
 text=emit(d)+'\n';assert parse(text)==d
 path.write_text(text,encoding='utf-8')
 manifest.append(dict(resource=resource,generated=str(path),sha256=sha(path),source=str(source),source_sha256=sha(source),kind=kind,classname=classname,runtime_verified=False,**details))
for phase in ('spawn','despawn'):
 src=L/f'research/asset-exports/native-doppelganger-fx-v436/game1_patch/generated.decls.fx.models.effects.gameplay.powers.doppelganger.decoy.spawn_despawn.decoy_{phase}_01..fx.decl'
 original=parse(src.read_text());d=copy.deepcopy(original);removed=[]
 for g in items(d['edit']['m_groups']):
  if 'm_rumbles'in g:removed.append('m_rumbles');g['m_rumbles']=arr([])
  if 'm_sounds'in g:
   sounds=items(g['m_sounds']);keep=[]
   for sound in sounds:
    name=sound['m_startSoundEvent']['m_soundEvent']
    if name=='sound_events/game_defined/bsp_ai_properties_02':removed.append(name)
    else:
     assert name in ('sound_events/bsp/bsp_power/bsp_decoy/bsp_decoy_cast_spawn','sound_events/bsp/bsp_power/bsp_decoy/bsp_decoy_baffle_despawn');keep.append(sound)
   g['m_sounds']=arr(keep)
 for old,new in zip(items(original['edit']['m_groups']),items(d['edit']['m_groups'])):assert old['m_particleSystems']==new['m_particleSystems']
 dest=A/f'doppelganger_{phase}_remote.fx';resource=f'models/effects/d2coop/doppelganger_{phase}_remote.fx'
 write(d,dest,resource,'fx','fx',src,removed=removed,native_particles_unchanged=True)
 source_phase='dispawn'if phase=='despawn'else'spawn'
 src=L/f'research/asset-exports/native-doppelganger-v403/game1_patch/generated.decls.midnightscene.midnight.gameplay.human.common.doppelganger.doppelganger_{source_phase}..midnightscene.decl'
 original=parse(src.read_text());d=copy.deepcopy(original);e=d['edit'];groups=items(e['m_groupsInfo']);assert len(groups)==1
 groups[0]['m_enableFeetIK']='false'
 assert groups[0]['m_name']=='AIGroup1'
 # Limit the current experiment to the exact219-joint Emily cosmetic clone.
 groups[0]['m_declsMD6']=arr(['models/characters/female/emily/head01/emily_head01.md6'])
 removed=[];sockets=[];scene_info=[]
 assert e['m_scenes']['num']==1
 for scene in items(e['m_scenes']):
  body=scene['m_object'];sg=items(body['m_groups']);assert len(sg)==1
  tracks=items(sg[0]['m_object']['m_lTracks']);keep=[]
  for track in tracks:
   t=track['m_object'];kind=t['m_trackType']
   if kind not in ('EMTT_FX','EMTT_MATERIALRENDERPARM'):removed.append(kind);continue
   keep.append(track)
   if kind=='EMTT_FX':
    for key in items(t['m_keys']):
     p=key['m_keyProp']['m_object'];sockets.append(p['m_attachmentSocketName'])
     old=f'models/effects/gameplay/powers/doppelganger/decoy/spawn_despawn/decoy_{phase}_01.fx'
     if p['m_declFx']==old:p['m_declFx']=resource
     else:assert p['m_declFx']==f'models/effects/gameplay/powers/doppelganger/decoy/spawn_despawn/decoy_{phase}_body_01.fx'
  assert len(keep)==2
  sg[0]['m_object']['m_lTracks']=arr(keep)
  assert body['m_properties']['m_length']==(1000 if phase=='spawn' else 466)
  scene_info.append(body['m_properties'])
 assert sorted(sockets)==sorted(['decoy_root','sl_l_arm','sl_r_arm','decoy_spawn_l_hand','decoy_spawn_r_hand'])
 e['m_sceneOut']={'m_groups':arr([])};e['m_graphPositions']=arr([])
 assert {o['m_keyProp']['m_class']for o in walk(d)if'm_keyProp'in o}=={'arkMidnightKeyPropFx','arkMidnightKeyPropMaterialRenderParm'}
 assert all('Player'not in str(o)for o in groups)
 write(d,A/f'doppelganger_fx_{phase}.midnightscene',f'midnight/d2coop/doppelganger_fx_{phase}.midnightscene','midnightscene','midnightScene',src,removed_tracks=removed,sockets=sockets,scenes=scene_info,animation_tracks=0,gameplay_event_tracks=0)
manifest_path=R/'work/natural-moveset/doppelganger-fx-assets-v441.json';manifest_path.write_text(json.dumps(manifest,indent=2),encoding='utf-8')
s=(L/'Build-D2DoppelgangerVisualAssets.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-visual-v423','doppelganger-fx-v441').replace('remote cosmetic Emily clone body experiment','remote cosmetic Emily clone summon/disappearance effects')
start=s.index('$resources=@(');end=s.index('\n',start)
resources='$resources=@('+','.join("@{Name='"+m['resource']+"';File='assets/"+Path(m['generated']).name+"';Kind='"+m['kind']+"';Class='"+m['classname']+"'}"for m in manifest)+')'
s=s[:start]+resources+s[end:]
(L/'Build-D2DoppelgangerFxAssets.ps1').write_text(s,encoding='utf-8')
print(json.dumps(dict(resources=len(manifest),native_particles_unchanged=True,animation_tracks=0,gameplay_event_tracks=0,installed=False,runtime_verified=False)))
