"""Repair stale review labels from existing receipts; do not invent fresh passes."""
from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
p=N/'review-evidence-annotations.json';a=rd(p);backup=W/'review-annotations-before-reconcile-v600.json';assert not backup.exists();shutil.copy2(p,backup)
groups=[
 (['pistol_draw_aim_fire_reload_holster'],['ranged-corvo-gun-network-v29-v1','ranged-emily-gun-network-v29-v1'],'partial_native_steam_ranged_five_phase_both_roles','Both-role standing native fire/reload/empty/equip/unequip clocks and automatic cosmetic hand prop passed on archived DLL30AB28F8. Remote projectiles, impact/damage synchronization, audio and all upgraded/contextual variants are not established by these receipts.'),
 (['crossbow_draw_aim_fire_reload_holster'],['ranged-corvo-crossbow-network-v29-v1','ranged-emily-crossbow-network-v29-v1'],'partial_native_steam_ranged_five_phase_both_roles','Both-role standing native fire/reload/empty/equip/unequip clocks and automatic cosmetic hand prop passed on archived DLL30AB28F8. Ammunition switching, projectile/impact outcomes, precise attachment and broader variants remain incomplete.'),
 (['corvo_windblast_cast'],['windblast-corvo-balanced-network-v28-v3','windblast-corvo-balanced-empty-moving-v28-v3'],'partial_native_steam_windblast_body_and_finite_world_fx','Saved native casts passed full-body phase, stance/equipment recovery, one remote finite cosmetic world effect per source effect, exact origin/axis and duplicate rejection. These cover the recorded crouched armed and holstered/moving ground fixtures; world impulses/hits, upgrade variants and production acceptance remain incomplete.'),
 (['turn_in_place_90_180','crouched_turns'],['turn-stance-corvo-knee-v4','turn-stance-corvo-unarmed-knee-v4','turn-stance-emily-knee-v4','turn-stance-emily-unarmed-knee-v4'],'partial_native_steam_stance_turns_both_roles','Saved both-role armed and unarmed tests passed continuous left/right standing/crouched/standing turns and recovery with stationary native root. This is not full per-angle90/180 acceptance or fresh regression on the current build.'),
 (['slopes_and_stairs'],['terrain-corvo-knee-v4','terrain-corvo-unarmed-knee-v4','terrain-emily-knee-v4b','terrain-emily-unarmed-knee-v4'],'partial_native_steam_dunwall_stairs_both_roles','Saved both-role armed/unarmed Dunwall staircase tests passed up/down grade-selected cycles, native height following, stopped cadence and preserved health/equipment. Arbitrary slopes, collision surfaces, broader terrain and visual acceptance remain open.')]
evidence=[]
for ids,takes,status,note in groups:
 receipts=[]
 for name in takes:
  path=N/name/'result.json';r=rd(path);assert r.get('passed',True) and not r.get('failure') and len(r.get('checks',[]))>=5,(name,r)
  receipts.append(dict(path=str(path.relative_to(R)),sha256=hashlib.sha256(path.read_bytes()).hexdigest().upper(),dll=r['dll'],checks=r['checks'],scope=r['scope']))
 for id in ids:
  item=a['items'][id];previous=item['runtime_binding'];assert previous=='not_verified',(id,previous)
  item['runtime_binding']=status;item['complete']=False;item['visual_accepted']=False;item['visual_validation']='not_accepted';item['phase_status']='work_remaining'
  item['notes']=note+' Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.'
  links=item.setdefault('evidence',[])
  for name in takes:
   ref=name+'/result.json'
   if ref not in links:links.append(ref)
  evidence.append(dict(id=id,previous=previous,new=status,receipts=receipts,currentBuildRetested=False,complete=False,visualAccepted=False))
wr(p,a);wr(W/'coverage-reconciliation-v600.json',dict(at=datetime.datetime.now().astimezone().isoformat(),scope='Review metadata repair only, backed by exact archived evidence; not new feature completion.',items=evidence))
print(json.dumps(dict(requirementsReconciled=len(evidence),newRuntimeTests=0,complete=False)))
