from pathlib import Path
p=Path('work/movement-integration/read_observable_v979.py')
s=p.read_text().replace("out=W/'observable-readonly-v979'", "out=W/'observable-readonly-v980'")
s=s.replace("for handle in [1,status['probeActorHandle']]:", "for handle in [1,status['probeActorHandle']]:")
needle="            assert read(at,32)==header and q(at+0x10)==owner"
replacement='''            row['actorFlagsD0']=read(owner+0xd0,1)[0]
            row['actorFlags368']=read(owner+0x368,1)[0]
            frame=q(owner+0xf0);row['renderFrame']=hex(frame)
            if frame:
                row['frameVtableRva']=hex(q(frame)-base)
                row['boundsVirtualRva']=hex(q(q(frame)+0x80)-base)
                row['cachedBoundsMin']=list(struct.unpack('<3f',read(frame+0x1840,12)))
                row['cachedBoundsMax']=list(struct.unpack('<3f',read(frame+0x1850,12)))
            if e['type']=='.?AVarkCpntObservable@@':
                row['declVisibilityOffsets']=list(struct.unpack('<2f',read(decl+0x364,8)))
                bound=q(at+0x28)
                if bound:row['boundComponentVtableRva']=hex(q(bound)-base);row['physicsState238']=hex(struct.unpack('<I',read(bound+0x238,4))[0])
            row['physicsBodies']=[]
            for cp in components['components']:
                if cp['vtableRva'].lower()!='0x1c54008':continue
                phys=int(cp['pointer'],16);instance=q(phys+0x100)
                if not instance:continue
                world=q(instance+0x20);ids=q(instance+0x28);count=struct.unpack('<I',read(instance+0x30,4))[0]
                assert 0<count<=64 and world and ids
                bodies=q(world+0x28)
                for index in range(count):
                    bid=struct.unpack('<I',read(ids+4*index,4))[0];assert bid&0xffffff!=0xffffff
                    row['physicsBodies'].append(dict(id=bid,position=list(struct.unpack('<3f',read(bodies+(bid&0xffffff)*0xb0+0x30,12)))))
            assert read(at,32)==header and q(at+0x10)==owner'''
assert needle in s;s=s.replace(needle,replacement)
s=s.replace("print(json.dumps([{k:r.get(k) for k in ('handle','declarationName','nativeReferenceCount','nativeTimer','boundHandle','boundComponent','decl68','decl6c')} for r in rows]))", "print(json.dumps([{k:v for k,v in r.items() if k not in ('raw','declarationRaw','component')} for r in rows],indent=2))")
Path('work/movement-integration/read_observable_v980.py').write_text(s)
