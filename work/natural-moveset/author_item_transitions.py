"""Connected equipment changes and interrupts, with free whole-body motion.

These clips contain no inventory, projectile or damage events. Interrupts
explicitly switch to empty equipment before the next item becomes visible.
"""
import sys, math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['items-retarget-manifest-v2.json','library-retarget-manifest.json'],'item-transitions-v1')
for role in ['corvo','emily']:
    rig=rig_for(role);im,idle=L.load(role,'sword_idle')
    for kind in ['crossbow','heart','stunmine']:
        # Interruption begins after visible equip; recovery is a finite
        # authored blend, not a frame reset or camera-space constraint.
        em,entry=L.load(role,kind+'_equip');om,out=L.load(role,kind+'_unequip')
        for point in [.3,.7]:
            ps=[];tracks=[];vis=[];phase=[]
            partdur=max(.15,em['duration']*point)
            for f in range(round((partdur+.65)*60)+1):
                t=f/60
                if t<=partdur:
                    u=t/partdur*point;p=sample(entry,u);prop=em['props'][0];at=min(len(prop['attachment_track'])-1,round(u*(len(prop['attachment_track'])-1)))
                    tr=prop['attachment_track'][at];visible=prop['visible'][at]
                else:
                    u=(t-partdur)/.65;p=blend(sample(entry,point),sample(out,u),ease(u/.25));p=blend(p,sample(idle,u,True),ease((u-.5)/.5))
                    prop=om['props'][0];at=min(len(prop['attachment_track'])-1,round(u*(len(prop['attachment_track'])-1)));tr=prop['attachment_track'][at];visible=u<.65
                preview_helpers(rig,p);ps.append(p);tracks.append(tr);vis.append(visible)
            definition=dict(em['props'][0]);definition.update(attachment_track=tracks,visible=vis,mechanism_segments=[])
            alias=f'{kind}_equip_cancel_{round(point*100)}'
            L.save(rig,role,alias,ps,[identity()for _ in ps],'ranged_and_items','Whole-body equip interruption at '+str(round(point*100))+' percent, followed by holster and standing recovery. Native prop remains hand-local. Inventory change, network interruption priority and arbitrary-time blending are not implemented by this offline study.',dict(equip_sha256=em['sha256'],holster_sha256=om['sha256'],idle_sha256=im['sha256']),props=[definition],phase_marks=[dict(phase='cancel',at=partdur)])
    for first,second in [('crossbow','heart'),('heart','stunmine'),('stunmine','crossbow')]:
        ps=[];root=[];marks=[];segments=[];source=[]
        for kind,phase in [(first,'idle'),(first,'unequip'),(second,'equip'),(second,'idle')]:
            m,fs=L.load(role,kind+'_'+phase);d=min(1.,m['duration'])if phase=='idle'else m['duration'];count=round(d*60)+1;start=len(ps);marks.append(dict(phase=kind+'_'+phase,at=start/60));prior=ps[-1]if ps else None
            for f in range(count):
                t=f/60;u=min(1,t/m['duration']);p=sample(fs,u,phase=='idle')
                if prior is not None:p=blend(prior,p,ease(t/.12))
                preview_helpers(rig,p);ps.append(p)
            segments.append((start,count,m,phase));source.append(dict(alias=m['selection']['alias'],sha256=m['sha256']))
        props=[]
        for kind in [first,second]:
            template=next(m['props'][0]for _,_,m,_ in segments if m['selection']['alias'].startswith(kind+'_'));tr=[template['attachment_track'][0]for _ in ps];visible=[False for _ in ps]
            for start,count,m,phase in segments:
                if not m['selection']['alias'].startswith(kind+'_'):continue
                pr=m['props'][0]
                for f in range(count):
                    at=min(len(pr['attachment_track'])-1,round(f));tr[start+f]=pr['attachment_track'][at];visible[start+f]=pr['visible'][at]
            props.append(dict(template,attachment_track=tr,visible=visible,mechanism_segments=[]))
        assert all(sum(int(pr['visible'][i])for pr in props)<=1 for i in range(len(ps)))
        L.save(rig,role,'item_switch_'+first+'_to_'+second,ps,[identity()for _ in ps],'ranged_and_items','Connected '+first+' to '+second+' equipment change. Full-body holster/equip acting with native prop geometry; at most one left-hand item is visible. Character inventory availability, exact grip and runtime state binding remain pending.',dict(clips=source),props=props,phase_marks=marks)
L.finish('item-transitions-manifest-v1.json')
