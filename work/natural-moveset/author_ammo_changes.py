"""Native bolt models/tags and source-timed reload in a free full-body study.

Ammo changes use authored holster/equip presentation. This is not a claim
that the native FP ammo-selection action uses that exact gesture or timing.
"""
import sys,math,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses as raw_poses
from rig_math import ease,NativeRig
L=Library(['items-retarget-manifest-v2.json'],'ammo-changes-v1');research=P/'item-research';research.mkdir(exist_ok=True)
inputs=[]
for batch in ['natural-bolt-models','natural-crossbow-model-tags','natural-crossbow-ammo']:
 for r in read(P.parent.parent/'outputs/d2-coop-lab/research/asset-exports'/batch/'manifest.json')['Resources']:
  if r['Name'].endswith(('.md6','.cpntweaponcrossbow','bolt_regular_attachment.def','bolt_tranquilizer_attachment.def')):
   dst=research/Path(r['ExportFile']).name;shutil.copyfile(r['ExportFile'],dst);inputs.append(dict(asset=r['Name'],sha256=r['Sha256'],file=str(dst.relative_to(P))))
(research/'native-bolt-sources.json').write_text(json.dumps(inputs,indent=2))
for role in ['corvo','emily']:
 rig=rig_for(role);crossrig=NativeRig(read(P/f'item-mechanisms/crossbow_{role}-skeleton.json')['joints']);boltindex=next(i for i,j in enumerate(crossrig.joints)if j['nativeName']=='bolt_jnt');tag=Matrix.Translation((-.04 if role=='emily'else 0,0,0));cache={}
 def bolt_tracks(prop):
  out=[]
  for f,tr in enumerate(prop['attachment_track']):
   t=f/60;segment=next((s for s in prop.get('mechanism_segments',[])if s['start']<=t<s['end']),None);mechanism=segment['mechanism']if segment else prop['mechanism'];phase=(t-segment['start'])/(segment['end']-segment['start'])if segment else t/max(.001,(len(prop['attachment_track'])-1)/60)
   if mechanism:
    key=mechanism['sample_sha256']
    if key not in cache:
     data=raw_poses(P/mechanism['sample_file']);assert data['sha256']==key;cache[key]=[[qts(v)for v in frame]for frame in data['frames']]
    local=sample(cache[key],phase)
   else:local=crossrig.bind
   out.append(row(qts(tr)@crossrig.fk(local)[boltindex]@tag))
  return out
 def bolt_prop(kind,tracks,visible):
  return dict(kind='bolt_'+kind,model=f'item-models/bolt-{kind}.json',skeleton='contact-fixtures/skeleton.json',parent_joint=9,attachment_track=tracks,visible=visible,mechanism=None,events=False,attachment_evidence='Native md6 bolt tag -> bolt_jnt; Emily tag offset -0.04m X, Corvo identity')
 for alias,order in [('crossbow_ammo_regular_to_sleep',['idle','unequip','equip','idle']),('crossbow_ammo_sleep_to_regular',['idle','unequip','equip','idle']),('crossbow_reload_native_timing',['idle','reload_in','reload_loop','reload_out','idle'])]:
  ps=[];track=[];visible=[];marks=[];segments=[];sources=[];split=None;reload_start=None
  for phase in order:
   m,body=L.load(role,'crossbow_'+phase);duration=.65 if phase=='idle'else {'reload_in':.333,'reload_loop':.234,'reload_out':.333}.get(phase,m['duration']);count=max(2,round(duration*60));start=len(ps);marks.append(dict(phase=phase,at=start/60));prev=ps[-1]if ps else None;pr=m['props'][0]
   if phase=='equip':split=start
   if phase=='reload_in':reload_start=start/60
   for f in range(count):
    u=f/max(1,count-1);p=sample(body,u,phase=='idle')
    if prev is not None:p=blend(prev,p,ease(f/60/.10))
    preview_helpers(rig,p);ps.append(p);at=min(len(pr['attachment_track'])-1,round(u*(len(pr['attachment_track'])-1)));track.append(pr['attachment_track'][at]);visible.append(pr['visible'][at])
   segments.append(dict(start=start/60,end=len(ps)/60,mechanism=pr['mechanism']));sources.append(dict(alias=m['selection']['alias'],sha256=m['sha256']))
  prop=dict(pr,attachment_track=track,visible=visible,mechanism_segments=segments);bt=bolt_tracks(prop)
  if reload_start is not None:
   ammo=[bolt_prop('regular',bt,[visible[f]and(f/60<reload_start or f/60>=reload_start+.467)for f in range(len(ps))])]
   marks.append(dict(phase='native_reload_event_reference_only',at=reload_start+.467))
   purpose='Ordinary crossbow reload body study at exact native scene timing: 333ms in, 234ms loop, 333ms out; bolt appears at the native 467ms reload event. Native geometry and bolt tag are used; no gameplay event is executed. Native component-driven mechanism effects, palm contact and runtime equipment state still need validation.'
  else:
   old,new=('regular','sleep')if alias.endswith('to_sleep')else('sleep','regular');assert split is not None
   ammo=[bolt_prop(old,bt,[v and f<split for f,v in enumerate(visible)]),bolt_prop(new,bt,[v and f>=split for f,v in enumerate(visible)])];assert all(sum(int(x['visible'][f])for x in ammo)<=1 for f in range(len(ps)))
   marks.append(dict(phase='ammo_representation_switch_while_hidden',at=split/60))
   purpose='Full-body ammunition-change presentation using existing holster/equip acting, native regular/sleep bolt geometry and exact native bolt tags. The bolt type changes while the weapon is hidden. This is an authored presentation choice; the native FP ammo-selection gesture/timing, inventory ownership and runtime equipment binding remain unverified.'
  L.save(rig,role,alias,ps,[identity()for _ in ps],'ranged_and_items',purpose,dict(clips=sources,native_sources=inputs),props=[prop]+ammo,phase_marks=marks)
  L.reports[-1]['equipment_policy']='sword_plus_native_crossbow';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
L.finish('ammo-changes-manifest-v1.json')
