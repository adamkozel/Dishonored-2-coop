"""Sample indexed installed native clips offline; never drive in-game events."""
from pathlib import Path
import json,hashlib,subprocess,time,sys
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
expanded='--expanded' in sys.argv
all_catalogs='--all-catalogs' in sys.argv
source=read(P/'native-source-index.json' if expanded else P.parent/'moveset-growth/native-fullbody-candidates.json')
exports=ROOT/'outputs/d2-coop-lab/research/asset-exports'
asset_files=sorted(exports.glob('natural-native-expanded-*/manifest.json')) if expanded else [exports/'natural-native-library/manifest.json']
if all_catalogs:asset_files+=sorted(exports.glob('natural-native-other-*/manifest.json'))
assets={'Resources':[r for p in asset_files for r in read(p)['Resources']]}
paths={}
for r in sorted(assets['Resources'],key=lambda r:(r['Patch'],'patch' in r['Catalog'])):
    paths[r['Name']]=r
exe=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe')
helper=P/'build/Release/d2_sample_edge.exe';out=P/'native-samples';out.mkdir(exist_ok=True)
skroot=ROOT/'outputs/d2-coop-lab/research/asset-exports/doppelganger-animation/game1_patch'
unique={}
for e in source['entries']:unique.setdefault((e['asset'],e['skeleton']),[]).append(e)
rows=[];began=time.time()
for idx,((asset,sk),entries) in enumerate(unique.items()):
    key=hashlib.sha256((asset+'\n'+sk).encode()).hexdigest()[:16]
    r=paths.get(asset);role='corvo' if 'base_tall' in sk else 'emily';part='tall' if role=='corvo' else 'female'
    skeleton=skroot/f'generated.skeleton.models.characters.{part}.base.base_{part}.edgeskl'
    path=out/(key+'.edgepose');log=out/(key+'.json')
    row=dict(id=key,asset=asset,skeleton=sk,source_role=role,aliases=sorted(set(e['alias'] for e in entries)),families=sorted(set(f for e in entries for f in e['families'])),status='missing_asset',runtime_verified=False)
    if r:
        row.update(asset_sha256=r['Sha256'].lower(),export_file=r['ExportFile'])
        if log.exists():
            prior=read(log)
            if prior.get('asset_sha256')!=row['asset_sha256']:raise RuntimeError('Pinned source changed')
            row.update(prior)
            row['families']=sorted(set(f for e in entries for f in e['families']))
            row['aliases']=sorted(set(e['alias'] for e in entries))
        elif not path.exists():
            run=subprocess.run([str(helper),str(exe),r['ExportFile'],str(skeleton),str(path)],capture_output=True,text=True,timeout=40)
            if run.returncode==0:
                row.update(status='native_pose_sampled_offline',sampling=json.loads(run.stdout),sample_file=str(path.relative_to(P)),sample_sha256=hashlib.sha256(path.read_bytes()).hexdigest())
            else:row.update(status='sampling_failed',exit_code=run.returncode,error=(run.stderr+run.stdout)[-2500:])
            log.write_text(json.dumps(row,indent=2))
        else:raise RuntimeError('Unmanifested output exists: '+str(path))
    rows.append(row)
    if idx%25==0:print(idx+1,'/',len(unique),'sampled',sum(r['status']=='native_pose_sampled_offline' for r in rows),flush=True)
result=dict(complete_moveset=False,scope='Native source poses only; authoring, role adaptation, event-free runtime binding and recorded visual acceptance remain separate.',seconds=time.time()-began,total=len(rows),sampled=sum(r['status']=='native_pose_sampled_offline' for r in rows),rows=rows)
(P/('native-allcatalog-sample-manifest.json' if all_catalogs else 'native-expanded-sample-manifest.json' if expanded else 'native-sample-manifest.json')).write_text(json.dumps(result,indent=2));print('SAMPLED',result['sampled'],'OF',len(rows),flush=True)
