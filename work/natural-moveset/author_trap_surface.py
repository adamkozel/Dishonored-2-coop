"""Native spring-razor presentation and shared trap-to-wall placement.

Both items explicitly share the native stun-mine scenes. Their native 236ms
fire marker is metadata only: no animation event, trap, or damage executes.
"""
import sys,math,json,hashlib,copy
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from inspect_midnight_events import parse,walk
ROOT=P.parent.parent;L=Library(['items-retarget-manifest-v2.json','library-retarget-manifest.json'],'trap-surface-v1')
asset=read(ROOT/'outputs/d2-coop-lab/research/asset-exports/natural-mine-place-scene/manifest.json')['Resources'][0];decl=parse(Path(asset['ExportFile']).read_text());event=next(o for o in walk(decl)if o.get('m_keyProp',{}).get('m_object',{}).get('m_event')=='ARK_MDNT_MSG_WEAPON_FIRE');release_ms=event['m_startTime'];assert release_ms==236
component=next(r for r in read(ROOT/'outputs/d2-coop-lab/research/asset-exports/natural-elixir-references/manifest.json')['Resources']if r['Name'].endswith('springrazor.cpntweaponplaceable'))
refs=dict(scene=asset['Name'],scene_sha256=asset['Sha256'],component=component['Name'],component_sha256=component['Sha256'],native_place_duration_ms=300,native_fire_marker_ms=release_ms,events_executed=False)
(P/'item-research/native-trap-scenes.json').write_text(json.dumps(refs,indent=2));(P/'item-research/springrazor.cpntweaponplaceable').write_text(Path(component['ExportFile']).read_text());(P/'item-research/gadget_stunmine_place.midnightscene').write_text(Path(asset['ExportFile']).read_text())
vertices=[[x,y,z]for x in [.50,.56]for y in [-.60,.85]for z in [0,2.1]];indices=[]
for q in [(0,4,6,2),(1,3,7,5),(0,1,5,4),(2,6,7,3),(0,2,3,1),(4,5,7,6)]:indices.extend(q[k]for k in [0,1,2,0,2,3])
(P/'contact-fixtures/trap-wall.json').write_text(json.dumps(dict(vertices=vertices,indices=indices,weights=[[['fixture_root',1.]]for _ in vertices],scope='Neutral wall fixture, not native scene geometry')))
for role in ['corvo','emily']:
 rig=rig_for(role);sm,stand=L.load(role,'sword_idle');im,idle=L.load(role,'stunmine_idle');pm,place=L.load(role,'stunmine_place');bm,sequence=L.load(role,'stunmine_sequence')
 # Exact shared-action variant, using the separately confirmed razor model.
 props=copy.deepcopy(bm['props']);props[0].update(kind='spring_razor',model='item-models/spring-razor.json',skeleton='contact-fixtures/skeleton.json',mechanism=None)
 for segment in props[0].get('mechanism_segments',[]):segment['mechanism']=None
 L.save(rig,role,'springrazor_sequence',sequence,bm['root_track'],'ranged_and_items','Native spring-razor model on the full-body mine sequence. The exact component confirms shared native stun-mine scenes. This connected held-item sequence does not detach onto a surface; see the separate wall-placement study. Grip and gameplay binding remain unverified.',dict(shared_action_sha256=bm['sha256'],native_scene=refs),props=props,phase_marks=bm['phase_marks'])
 for kind in ['stunmine','springrazor']:
  prop0=copy.deepcopy(im['props'][0]);model='item-models/spring-razor.json'if kind=='springrazor'else prop0['model'];data=read(P/model);rot=Matrix.Rotation(-math.pi/2,3,'Y');back=max((rot@Vector(v)).x for v in data['vertices']);surface=rot.to_4x4();surface.translation=Vector((.50-back,.20,1.20));attachment=qts(prop0['attachment_track'][0]);wrist=surface@attachment.inverted();ps=[];roots=[];tracks=[];errors=[];release=1+release_ms/1000
  for f in range(193):
   t=f/60;p=sample(idle,t/im['duration'],True);g=rig.fk(p);feet=[g[j].translation.copy()for j in [21,25]]
   reach=ease((t-.3)/.82)*(1-ease((t-1.3)/.85));p[2].translation+=Vector((.07,0,-.025))*reach
   for j,w in [(3,.35),(4,.35),(5,.30)]:rig.orient(p,j,Matrix.Rotation(math.radians(9*w*reach),3,'Y')@rig.fk(p)[j].to_3x3())
   pin_feet(rig,p,feet);g=rig.fk(p);target=g[9].translation.lerp(wrist.translation,reach);orientation=g[9].to_quaternion().slerp(wrist.to_quaternion(),reach).to_matrix()
   rig.solve(p,7,8,9,target,g[7].translation+Vector((.1,.4,-.25)));rig.orient(p,9,orientation)
   # Use native placement finger articulation only inside its own phase.
   if 1<=t<=1.3:
    ph=sample(place,(t-1)/.3)
    for j,info in enumerate(rig.joints):
     if info['nativeName'].startswith(('left_thumb','left_index','left_middle','left_ring','left_pinky')):p[j]=ph[j].copy()
   if t>1.65:p=blend(p,sample(stand,t/sm['duration'],True),ease((t-1.65)/1.1))
   preview_helpers(rig,p);g=rig.fk(p);equipment=g[9]@attachment if t<release else surface
   if reach>.999:errors.append((g[9].translation-wrist.translation).length)
   ps.append(p);roots.append(identity());tracks.append(row(equipment))
  prop=copy.deepcopy(prop0);prop.update(kind='spring_razor'if kind=='springrazor'else'stun_mine',model=model,parent_joint=0,attachment_track=tracks,visible=[True]*len(ps),mechanism=None)
  if kind=='springrazor':prop['skeleton']='contact-fixtures/skeleton.json'
  wall=dict(kind='fixture_trap_wall',model='contact-fixtures/trap-wall.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=[identity()for _ in ps],visible=[True]*len(ps),mechanism=None,fixture=True,events=False)
  L.save(rig,role,kind+'_wall_place_sequence',ps,roots,'ranged_and_items','Complete-body wall placement: anticipation, reach/press, native 236ms release marker and armed recovery. The native item follows the free hand until release, then remains on a neutral wall. Its support face is fitted from actual mesh bounds. Fine finger grip, floor/ceiling variants, collision validation and native trap ownership remain pending. No trap or damage is generated.',dict(idle_sha256=im['sha256'],place_sha256=pm['sha256'],recovery_sha256=sm['sha256'],native_scene=refs),props=[prop,wall],phase_marks=[dict(phase='prepare',at=0),dict(phase='native_place',at=1),dict(phase='detach_reference_only',at=release),dict(phase='recovery',at=1.3)],max_wrist_target_error_m=max(errors,default=0),surface_normal=[-1,0,0])
  L.reports[-1]['preview_view']=[-5.5,-7,3.7]
  print('TRAP_CONTACT',role,kind,max(errors,default=0),flush=True)
 for m in L.reports:
  if m['role']==role:m['equipment_policy']='sword_plus_native_trap';(P/m['binary']).with_suffix('.json').write_text(json.dumps(m,indent=2))
L.finish('trap-surface-manifest-v1.json')
