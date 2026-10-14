"""Crouched approaches into native or contact-authored paired body actions."""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['stance-retarget-manifest.json','paired-native-manifest-v1.json','choke-sequences-manifest-v1.json'],'crouched-pairs-v1');actors=[];victims=[]
for role in ['corvo','emily']:
    rig=rig_for(role);cm,crouch=L.load(role,'crouch_idle')
    for alias in ['paired_fast_back_tall','nonlethal_back_hold_sequence','nonlethal_back_finish_sequence','nonlethal_back_release_sequence']:
        m,action=L.load(role,alias);partner=m['partner'];vrig=rig_for(partner['role'])
        temp=Library([],'crouched-pairs-v1');temp.rows=[dict(partner,selection=dict(alias='victim'),joints=len(vrig.joints))];vm,vf=temp.load(partner['role'],'victim')
        ps=[];roots=[];vs=[];vroots=[];count=round(m['duration']*60)+1;prefix=60;cg=rig.fk(crouch[0]);ag=rig.fk(action[0]);startfeet=[cg[j].translation for j in [21,25]];endfeet=[ag[j].translation for j in [21,25]]
        for f in range(prefix):
            t=f/60;u=ease((t-.2)/.8);p=blend(sample(crouch,t/cm['duration'],True),action[0],u)
            feet=[a.lerp(b,u)for a,b in zip(startfeet,endfeet)]
            for v in feet:v.z+=.035*math.sin(math.pi*u)
            pin_feet(rig,p,feet);preview_helpers(rig,p);ps.append(p);roots.append(m['root_track'][0]);vs.append(vf[0]);vroots.append(partner['root_track'][0])
        for f in range(count):
            t=f/60;u=min(1,t/m['duration']);ps.append(sample(action,u));roots.append(row(sample([[qts(r)]for r in m['root_track']],u)[0]));v=min(1,t/partner['duration']);vs.append(sample(vf,v));vroots.append(row(sample([[qts(r)]for r in partner['root_track']],v)[0]))
        newalias='crouched_'+alias
        L.save(vrig,partner['role'],'victim_'+role+'_'+newalias,vs,vroots,'paired_victim_reference','Original native target held during pre-contact crouch rise, then original paired clock.',dict(victim_sha256=partner['sha256']));vmeta=L.reports[-1];victims.append(vmeta)
        newpartner=dict(partner,id=vmeta['id'],binary=vmeta['binary'],sha256=vmeta['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots)
        L.save(rig,role,newalias,ps,roots,'paired_actions','Crouched approach rises across the whole body into '+alias.replace('_',' ')+'. Authored foot transfer and original paired timing follow a short pre-contact rise. Target is held during that preparation; initial grab, native outcome/ragdoll and gameplay pairing remain unverified. This does not claim an attack performed entirely at crouch height.',dict(crouch_sha256=cm['sha256'],paired_sha256=m['sha256']),partner=newpartner,phase_marks=[dict(phase='crouched_preparation',at=0),dict(phase='paired_action',at=1.)])
        L.reports[-1]['equipment_policy']=m.get('equipment_policy','sword_ready');(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1])
(P/'crouched-pairs-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'crouched-pairs-victims-v1.json').write_text(json.dumps(victims,indent=2))
