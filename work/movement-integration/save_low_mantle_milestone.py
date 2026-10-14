import json,hashlib,shutil,datetime
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent;N=ROOT/'work/natural-moveset';SRC=ROOT/'outputs/d2-coop-lab/src'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
dll=ROOT/'work/asset-game/dinput8.dll';digest=sha(dll);assert digest=='5830AE3E10EE6AC802D4BFD7C14AEB6096555ECC766F19C4DC061A308D4BA390'
tests=['low-mantle-v6-recovery-test','low-mantle-v12-priority']
for t in tests:assert read(N/t/'result.json')['passed']
for f in read(P/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
out=P/'verified-low-mantle-5830';assert not out.exists();out.mkdir();shutil.copytree(SRC,out/'src');shutil.copy2(dll,out/'dinput8.dll');shutil.copytree(N/'low-mantle-banks-v2',out/'banks')
for name in ('build_low_mantle_bank.py','build_low_mantle_contact.py','low-mantle-bank-manifest-v2.json','Test-LowMantle.ps1','Record-IntegratedStance.ps1'):shutil.copy2(N/name,out/name)
for t in tests:shutil.copytree(N/t,out/t)
facts=dict(dll_sha256=digest,bank_format='D2MOVE12 / 113 clips',action_protocol='10 / 208 bytes',tests=tests,checks=29,complete=False,visual_accepted=False,protected_saves_unchanged=True,
 binding='Observed mantle_low_idle Scene_1 500 ms. Exactly one actual player plus optional owned right arm. Owned sword_equip_fast binds only right arm; both roles observed, 500 ms each.',
 contact='v2 contact fitted to measured Dunwall desk trajectory; world actor root remains network-controlled. Both roles filmed. Geometry-general contacts, high/vault/hang, interruptions and visual acceptance remain incomplete.',
 evidence=[f'integrated-low-mantle-{r}-v2/take.json'for r in ('corvo','emily')],failed_runs_retained=['low-mantle-v1-test','low-mantle-v2-test','low-mantle-v3-test','low-mantle-v5-contact-test'],created=datetime.datetime.now().astimezone().isoformat())
(out/'verification.json').write_text(json.dumps(facts,indent=2),encoding='utf-8');(P/'low-mantle-integration-findings.json').write_text(json.dumps(facts,indent=2),encoding='utf-8')
path=P/'native-movement-findings.json';d=read(path);v=d['confirmed_native_activation']['low_mantle'];v.update(binding=facts['binding'],scope=facts['contact'],gameplay_binding='low-mantle-v6-recovery-test:16 checks + low-mantle-v12-priority:13',recordings=facts['evidence']);path.write_text(json.dumps(d,indent=2),encoding='utf-8')
path=ROOT/'work/current-task-state.json';d=read(path);i=d['MovementIntegration'];i['LatestContinuation']=dict(complete=False,live_dll=digest,live_bank=facts['bank_format'],rollback=str(out.relative_to(ROOT)),dashboard_game_recordings=25,current_work='Native low mantle and fast sword recovery verified on both roles; contact revision filmed. Inspecting further native traversal variants. Full movement phase remains incomplete.');path.write_text(json.dumps(d,indent=2),encoding='utf-8')
print('Preserved',out.name,'29 checks, protected saves unchanged')
