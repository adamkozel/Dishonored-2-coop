"""Produce a standalone sibling of the preserved low-mantle contact author."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'build_low_mantle_contact.py').read_text(encoding='utf-8')
replacements=[
 ('low-mantle-sources-v2','crouch-mantle-sources-v1'),
 ("OUT=P/'low-mantle-banks-v2'","OUT=P/'crouch-mantle-banks-v1'"),
 ("observed=read(P/'low-mantle-v3-test/states.json')","observed=read(P/'native-traversal-variants-corvo-v1/states.json')"),
 ("if r['source']!='host' or not r['armed'] or r['trial']!=1 or not r['native'].get('traversalSceneValid'):continue","if r['trial']!='crouch-normal' or not any(a['id']==8 for a in r['state']['active']):continue"),
 ("s=r['native'];a=s['active'][0]['mantleResearch'];delta=", "s=r['state'];a=next(a for a in s['active'] if a['id']==8)['mantleResearch'];scene=next(v for v in r['midnight']['instances'] if v['id']==a['scene24']);delta="),
 ("s['traversalScene']['timeMs']/500","min(499,scene['clockWords'][0])/500"),
 ("data=(P/'low-mantle-banks-v1'","data=(P/'low-mantle-banks-v2'"),
 ("bank=bytearray(data[:32+111*entry])","bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE13',13,n,115,fps,layout)+data[32:])"),
 ("stand=previous(59 if unarmed else 0)","stand=previous(76 if unarmed else 17)"),
 ("111+int(unarmed)","113+int(unarmed)"),
 ("native_low_mantle_contact","native_low_mantle_crouched_exit_contact"),
 ("bank[:32+111*entry]==data[:32+111*entry]","bank[32:len(data)]==data[32:]"),
 ("low-mantle-v3-test/states.json: host armed trial1","native-traversal-variants-corvo-v1/states.json: crouch-normal"),
 ("low-mantle-source-manifest-v2.json","crouch-mantle-source-manifest-v1.json"),
 ("low-mantle-bank-manifest-v2.json","crouch-mantle-bank-manifest-v1.json"),
 ("standing recovery","crouched recovery"),
]
for a,b in replacements:
 assert a in s,a
 s=s.replace(a,b)
s=s.replace('samples=[(0,0,0)]+samples+[(1,span,height)]','samples=[(0,0,0)]+sorted(dict((v[0],v)for v in samples).values())+[(1,span,height)]')
(P/'build_crouch_mantle_bank.py').write_text(s,encoding='utf-8')
print('Generated standalone crouch mantle author, preserving the v12 author')
