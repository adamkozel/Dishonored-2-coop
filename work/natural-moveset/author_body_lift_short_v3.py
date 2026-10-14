"""Separate short pickup study. Native first-person timing is inspiration only.

This authors new 1.4s choreography rather than accelerating the 4.8s v2 study.
The carried body arcs in front of the shoulder while both legs drive the lift.
No first-person wrist constraints, root writes, gameplay events or installation.
"""
import sys, math, json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants
L=Library(['carry-body-manifest-v3.json','carry-victim-retarget-manifest.json','library-retarget-manifest.json','interaction-retarget-manifest.json'],'body-lift-short-v3')
vrig=rig_for('corvo');dm,death=L.load('corvo','empty_stinghit_death');bm,body=L.load('corvo','generic_carrycorpse_idle')
actors=[];victims=[];diagnostics=[]
for role in ['corvo','emily']:
    rig=rig_for(role);im,idle=L.load(role,'empty_idle');cm,loaded=L.load(role,'carry_body_idle');gm,grasp=L.load(role,'sword_doppelganger_assassination_fastback');gr=sample(grasp,.24)
    attach=qts(cm['partner']['root_track'][0]);rotation=attach.to_3x3()
    floor=copy_pose(death[-1]);fg=vrig.fk(floor);axis=fg[12].translation-fg[2].translation
    yaw=-math.pi/2-math.atan2(axis.y,axis.x)
    floor[2]=rotation.inverted().to_4x4()@Matrix.Rotation(yaw,4,'Z')@floor[2];floor[2].translation=Vector()
    fg=vrig.fk(floor);low=min((rotation@fg[j].translation).z for j in [2,5,9,12,16,21,25])
    floorcenter=Vector((.40,.04,.105-low));finalcenter=(attach@vrig.fk(body[0])[2]).translation
    folded=copy_pose(body[0]);folded[2].translation=Vector()
    start=copy_pose(idle[0]);g0=rig.fk(start);feet=[g0[j].translation.copy()for j in [21,25]]
    ps=[];vs=[];roots=[];vroots=[];contact_errors=[];head_clearance=[]
    duration=1.4
    for frame in range(85):
        t=frame/60
        gather=ease(t/.32);drive=ease((t-.30)/.68);settle=ease((t-.98)/.30)
        down=gather*(1-drive)
        p=copy_pose(start);p[2].translation+=Vector((.045,-.025,-.65))*down
        # Legs support the trunk. Bend at the hips, then distribute a smaller
        # curve through the spine; do not make the head a fixed hand target.
        rig.orient(p,2,Matrix.Rotation(math.radians(50*down),3,'Y')@rig.fk(p)[2].to_3x3())
        for j,angle in [(3,5),(4,6),(5,4)]:
            rig.orient(p,j,Matrix.Rotation(math.radians(angle*down),3,'Y')@rig.fk(p)[j].to_3x3())
        # Roll the load before raising it. The outward arc leaves room for
        # the carrier's head during the fast shoulder transfer.
        shape=ease((t-.27)/.48);v=blend(floor,folded,shape)
        center=floorcenter.lerp(finalcenter,drive)
        arc=math.sin(math.pi*drive)
        center+=Vector((.16,.16,.07))*arc
        world=rotation.to_4x4();world.translation=center;vg=vrig.fk(v)
        left=world@vg[19].translation.lerp(vg[20].translation,.72)+Vector((.015,-.065,.035))
        right=world@vg[5].translation+Vector((-.015,-.035,-.05))
        w=ease((t-.06)/.24);wr=w*(1-ease((t-.75)/.25))
        g=rig.fk(p)
        # A bounded whole-trunk reach, followed by anatomical two-bone solves.
        if wr>.01:
            mid=(left+right)*.5;shoulder=(g[7].translation+g[14].translation)*.5
            shift=(mid+Vector((-.24,0,.26))-shoulder)*wr*.65
            shift.x=max(-.12,min(.28,shift.x));shift.y=max(-.18,min(.18,shift.y));shift.z=max(-.16,min(.18,shift.z))
            p[2].translation+=shift
        # Lean the head/trunk slightly away from the load during transfer.
        rig.orient(p,4,Matrix.Rotation(math.radians(-10*arc),3,'X')@rig.fk(p)[4].to_3x3())
        g=rig.fk(p)
        for k,(a,b,j) in enumerate([(19,20,21),(23,24,25)]):
            rot=g[j].to_3x3();target=feet[k].copy();target.y+=(.045 if k==0 else -.035)*gather*(1-settle)
            rig.solve(p,a,b,j,target,g[a].translation+Vector((.8,0,-.1)));rig.orient(p,j,rot)
        for a,b,j,target,weight,sign in [(7,8,9,left,w,1),(14,15,16,right,wr,-1)]:
            prior=copy_pose(p);g=rig.fk(p);rot=rig.fk(gr)[j].to_3x3()
            rig.solve(p,a,b,j,target,g[a].translation+Vector((.12,.32*sign,-.26)));rig.orient(p,j,rot)
            for k in descendants(rig,j):p[k]=gr[k].copy()
            for k in [a,b,j,*descendants(rig,j)]:p[k]=blend([prior[k]],[p[k]],weight)[0]
            if weight>.999:contact_errors.append(dict(t=t,joint=j,error=(rig.fk(p)[j].translation-target).length,target=list(target),shoulder=list(g[a].translation)))
        p=blend(p,loaded[0],settle)
        g=rig.fk(p)
        # Diagnostics are screening data, not a substitute for a visual review.
        head=g[12].translation
        head_clearance.append(dict(t=t,distance=min((head-world@vg[j].translation).length for j in [2,3,4,5,11,12])))
        preview_helpers(rig,p);preview_helpers(vrig,v)
        ps.append(p);vs.append(v);roots.append(identity());vroots.append(row(world))
    alias='carry_body_short_lift_study'
    L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'carried_body_reference','Short authored roll-and-hoist study; grey stand-in only, no live NPC or physics.',dict(death_sha256=dm['sha256'],loaded_sha256=bm['sha256']))
    target=L.reports[-1];victims.append(target)
    partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=duration,root_track=vroots,placement=vroots[0],appearance='Neutral native body stand-in',placement_basis='Authored short lift study; no runtime contact validation')
    L.save(rig,role,alias,ps,roots,'interactions','New short whole-body gather, roll, leg-driven hoist and shoulder settle. Separate screening candidate; native timing, contact and gameplay binding remain unverified.',dict(idle_sha256=im['sha256'],loaded_sha256=cm['sha256'],grasp_sha256=gm['sha256']),partner=partner,phase_marks=[dict(phase='gather',at=.30),dict(phase='drive',at=.60),dict(phase='shoulder_settle',at=.98)],preview_caption='AUTHORING STUDY - new 1.4s choreography - game timing and contact unverified')
    m=L.reports[-1];m['equipment_policy']='hands_free';(P/m['binary']).with_suffix('.json').write_text(json.dumps(m,indent=2));actors.append(m)
    diagnostics.append(dict(role=role,max_contact_error=max(e['error']for e in contact_errors),worst_contacts=sorted(contact_errors,key=lambda e:e['error'],reverse=True)[:8],closest_head=sorted(head_clearance,key=lambda e:e['distance'])[:8],visual_accepted=False))
(P/'body-lift-short-manifest-v3.json').write_text(json.dumps(actors,indent=2))
(P/'body-lift-short-victims-v3.json').write_text(json.dumps(victims,indent=2))
(P/'carry-research/lift-short-screening-v3.json').write_text(json.dumps(diagnostics,indent=2))
print(json.dumps(diagnostics),flush=True)
