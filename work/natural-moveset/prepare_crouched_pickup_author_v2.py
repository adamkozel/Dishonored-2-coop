"""Keep v1 comparison; fit v2 to the actual crouched remote victim paths."""
from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'author_crouched_pickup_v1.py').read_text()
t=t.replace('crouched-pickup-sources-v1','crouched-pickup-sources-v2').replace('crouched-pickup-banks-v1','crouched-pickup-banks-v2')
t=t.replace("read(P/'carry-pickup-contact-bank-manifest-v5.json')", "read(P/'crouched-pickup-bank-manifest-v1.json')")
t=t.replace("(b'D2MOVE34',34,n,232,60)","(b'D2MOVE35',35,n,233,60)")
t=t.replace("f'native-paired-pickup-{role}-v24-fast/contact-analysis.json'", "f'native-crouched-pickup-drop-{role}-v28/contact-analysis.json'")
t=t.replace("  native_phase=max(0,(u*1.4-.30)/1.1);v.z-=.674*ease((native_phase-.08)/.72)\n","")
old=" low=get(76,0);loaded=get(227,0);loadedG=rig.fk(loaded);fingers=descendants(rig,9)"
new=""" held=read(P/f'native-crouched-pickup-drop-{role}-v28/held-contact-analysis.json')
 carrier=read(P/f'native-crouched-pickup-drop-{role}-v28/carrier-held.json')
 shoulder=Vector(next(j['globalPose'][12:15]for j in carrier['joints']if j['nativeName']=='left_arm_jnt'))
 support_delta=Vector(held['left_target'])-shoulder
 def support(p):
  p=copy_pose(p);g=rig.fk(p);rotation=g[9].to_3x3().copy();goal=g[7].translation+support_delta
  rig.solve(p,7,8,9,goal,g[7].translation+Vector((.1,.35,-.18)));rig.orient(p,9,rotation);preview_helpers(rig,p)
  return p
 low=get(76,0);loaded=support(get(227,0));loadedG=rig.fk(loaded);fingers=descendants(rig,9)"""
assert old in t;t=t.replace(old,new)
t=t.replace("Vector((.26*reach,0,-.06*reach))","Vector((.50*reach,0,-.20*reach))")
t=t.replace("  for j in [3,4,5]:p[j]=p[j]@Matrix.Rotation(.055*reach,4,'Y')", "  g=rig.fk(p);rig.orient(p,3,Matrix.Rotation(.18*reach,3,'Y')@g[3].to_3x3())")
t=t.replace('min(.16,max(0,v.length-length+.02))','min(.30,max(0,v.length-length+.02))')
t=t.replace("  feet=[startG[j].translation.lerp(endG[j].translation,load)for j in [21,25]]", "  feet=[startG[j].translation.lerp(endG[j].translation,load)for j in [21,25]]\n  feet[0].x+=.20*reach;feet[0].y+=.04*reach\n  step_phase=t/.30 if t<.30 else (t-.52)/.52 if t>.52 else -1\n  if 0<step_phase<1:feet[0].z+=.035*math.sin(math.pi*step_phase)")
old="   rig.solve(p,a,b,c,target(u,key),g[a].translation+Vector((.1,.35*side,-.18)));rig.orient(p,c,rotation)"
new="   contact=target(u,key)\n   if c==9:contact=contact.lerp(g[7].translation+support_delta,ease((t-1.05)/.35))\n   rig.solve(p,a,b,c,contact,g[a].translation+Vector((.1,.35*side,-.18)));rig.orient(p,c,rotation)"
assert old in t;t=t.replace(old,new)
start=t.index(' bank=bytearray(data);');end=t.index(' path=out/',start)
t=t[:start]+""" bank=bytearray(data)
 for cid in [227,228,230,232]:
  if cid==232:frames=[sample(poses,f/119)for f in range(120)]
  else:
   frames=[]
   for f in range(120):
    old=get(cid,f/119);fixed=support(old)
    p=blend(old,fixed,1-ease((f/119)/.35)) if cid==230 else fixed
    frames.append(p)
  at=32+cid*entry+24
  bank[at:at+121*n*28]=b''.join(struct.pack('<7f',*row(m))for pose in frames+[frames[0]]for m in pose)
 assert len(bank)==len(data)
 for cid in range(233):
  if cid not in [227,228,230,232]:assert bank[32+cid*entry:32+(cid+1)*entry]==data[32+cid*entry:32+(cid+1)*entry]
"""+t[end:]
t=t.replace("added_clips=[232]","changed_clips=[227,228,230,232]")
t=t.replace('crouched-pickup-source-manifest-v1','crouched-pickup-source-manifest-v2').replace('crouched-pickup-bank-manifest-v1.json\').write_text','crouched-pickup-bank-manifest-v2.json\').write_text')
t=t.replace('crouched-pickup-v1-authoring','crouched-pickup-v2-authoring')
t=t.replace("standing_trace_sha256=trace['source_sha256'],native_crouch_offset_m=.674", "crouched_trace_sha256=trace['source_sha256'],held_contact=held['source'],approach_step_m=.20")
t=t.replace('The prior remote victim contact path is lowered during the same snapshot\nblend by the 0.674 m offset measured in native crouched Carry45 references.','Uses actual v28 remote crouched victim contact paths and a small authored\napproach step; adjusts loaded wrist contact in the isolated v2 bank.')
(P/'author_crouched_pickup_v2.py').write_text(t)
print('CROUCHED_PICKUP_AUTHOR_V2_READY')
