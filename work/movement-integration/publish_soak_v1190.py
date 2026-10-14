from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
T=N/'mission-soak-v1186';r=rd(T/'result.json');assert r['passed'] and rd(T/'restoration.json')['passed']
c=r['cases'][0];s=rd(T/'soak-samples.json')
trend={k:[(x['ms']//60000,x[k]['proc']['priv']//(1<<20)) for x in s[::6]] for k in ('host','client')}
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'stability-soak-audit-v1190.json',dict(at=at,candidate='candidate-npc-combat-v1181',harness='../natural-moveset/Test-MissionSoak-v1186.ps1',
  result=dict(minutes=20,samples=c['samples'],deaths=c['deaths'],memory=c['memory'],armedEnd=c['armedEnd'],privateMemoryMBByMinute=trend,maxNpcQueueUs={'host':902511,'client':1014785},staleSamples=0),
  findings=['Confirmed runtime: 20 minutes on Edge of the World with the product controller - movement across the map every 30 s, three shared deaths/restarts (both roles) - no crash, transport never stale, every session armed at the end, handles flat.','Private memory grew steadily ~6-7 MB/min on both peers (+104 MB host, +222 MB client after the 2-minute baseline; one +119 MB step at the last restart). Not yet attributed: a control soak without co-op systems is needed to separate the game\'s own streaming growth from the mod.'],
  complete=False,outstanding=['Control soak (lab baseline without co-op systems) to attribute memory growth; longer soak (2 h).','NPC transport queue reached ~1 s during teleports/restarts (no stale state).']))
s2=rd(R/'work/current-task-state.json');s2['UpdatedAt']=at
s2['MovementExecutionPriority']['current']='1181: 20-minute product-controller soak passed (no crash, all armed, 3 shared restarts); memory growth ~6-7 MB/min to attribute.'
wr(R/'work/current-task-state.json',s2);print('ok')
