from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';REL=R/'work/release'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1181/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
cases=[]
for take,passed,scope in (('mission-product-auto-v1182',False,'Harness helper PS shadowed by the PowerShell alias ps (Get-Process); failed before arming; lab restored.'),
                          ('mission-product-auto-v1183',True,'Pass: the harness only sent coop_product_auto on; the DLL controller armed all 17 steps on both peers and kept them armed through two shared mission restarts: doors, coins, usables and NPC stream renewed and the partner avatar was restored 2/2. At the post-restart sample the client had 0 bound NPC channels while the host had 9 (client binds on host Hello; convergence not captured by that sample).')):
    T=N/take;r=rd(T/'result.json');assert r['passed']==passed and rd(T/'restoration.json')['passed'],take
    cases.append(dict(take=take,passed=passed,scope=scope,failure=r.get('failure')))
pk=rd(REL/'release-install-v1184/result.json');assert pk['passed']
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'product-controller-audit-v1185.json',dict(at=at,candidate='candidate-npc-combat-v1181',dll=C['dll'],cases=cases,
  package=dict(path='../../outputs/release/d2coop-v1181',files=13,added=['d2coop/d2coop.cfg','d2coop/corvo-choke-bank.bin','d2coop/emily-choke-bank.bin'],test='../release/release-install-v1184/result.json',cases=pk['cases']),
  findings=['Confirmed runtime (lab): the product session controller alone arms and maintains the whole co-op feature set per shared world and across shared mission restarts.','Product activation (no lab env + d2coop/d2coop.cfg) is implemented and packaged; it has not been booted in a real installed game here.'],
  complete=False,outstanding=['Boot a product-mode install (scratch copy of the game) and verify activation without the lab environment.','Two-account online validation.','Native in-game UI for host/join/character choice; stability soak.']))
C.update(runtimeVerified=True,scopedProductControllerVerified=True,runtimeEvidence='../product-controller-audit-v1185.json');wr(W/'candidate-npc-combat-v1181/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1181 live: product session controller arms/maintains all co-op systems by itself (lab-verified across two shared mission restarts); release package d2coop-v1181 adds product activation (d2coop.cfg + partner banks), install/rollback scratch-tested.'
s['MovementExecutionPriority']['next']='Stage 7: stability soak; product-mode boot from a scratch install; two-account validation (user); in-game UI; stage 6 mission transition.'
wr(R/'work/current-task-state.json',s);print('ok')
