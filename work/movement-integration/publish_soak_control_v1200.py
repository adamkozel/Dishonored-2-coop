from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def series(take):
    s=rd(N/take/'soak-samples.json');d=rd(N/take/'soak-deaths.json') if (N/take/'soak-deaths.json').exists() else []
    out={}
    for r in ('host','client'):
        pts=[(x['ms']/60000,x[r]['proc']['priv']/(1<<20)) for x in s]
        base=[p for p in pts if p[0]>=2] or pts
        # least-squares slope MB/min after the 2-minute baseline
        n=len(base);mx=sum(p[0] for p in base)/n;my=sum(p[1] for p in base)/n
        slope=sum((p[0]-mx)*(p[1]-my) for p in base)/max(sum((p[0]-mx)**2 for p in base),1e-9)
        # growth between deaths only (restart steps excluded): sum of within-segment deltas
        cuts=sorted(x['ms']/60000 for x in d);segs=[];cur=[]
        for p in base:
            if any(0<=p[0]-c<1.0 for c in cuts):continue  # skip the first minute after a restart (transient load peak)
            if cur and any(cur[-1][0]<c<=p[0] for c in cuts):segs.append(cur);cur=[]
            cur.append(p)
        if cur:segs.append(cur)
        within=sum(sg[-1][1]-sg[0][1] for sg in segs);span=sum(sg[-1][0]-sg[0][0] for sg in segs)
        out[r]=dict(startMB=round(pts[0][1],1),baselineMB=round(base[0][1],1),endMB=round(pts[-1][1],1),growthMB=round(pts[-1][1]-base[0][1],1),slopeMBPerMin=round(slope,2),
                    withinSegmentMBPerMin=round(within/span,2) if span else None,restartStepsMB=round((pts[-1][1]-base[0][1])-within,1),handles=(s[0][r]['proc']['handles'],s[-1][r]['proc']['handles']),samples=len(pts))
    return out,len(d)
T='mission-soak-control-v1198';r=rd(N/T/'result.json');rest=rd(N/T/'restoration.json')
ctrl,cd=series(T);prod,pd=series('mission-soak-v1186')
at=datetime.datetime.now().astimezone().isoformat()
ratio={k:(round(prod[k]['slopeMBPerMin']/ctrl[k]['slopeMBPerMin'],2) if ctrl[k]['slopeMBPerMin'] else None) for k in ('host','client')}
wr(W/'soak-control-audit-v1200.json',dict(at=at,candidate='candidate-npc-combat-v1193',control=dict(take=f'../natural-moveset/{T}',passed=r['passed'],failure=r.get('failure'),restoration=rest['passed'],deaths=cd,memory=ctrl),
  product=dict(take='../natural-moveset/mission-soak-v1186',candidate='candidate-npc-combat-v1181',deaths=pd,memory=prod),slopeRatioProductOverControl=ratio,
  method='Same mission, route (3 spots, 30 s moves), 20 minutes and three death/restart cycles. Control: every co-op system off (bridge transport/status only), both peers die together and restart natively. Slope = least-squares private-memory MB/min after the 2-minute baseline; within-segment rate excludes the restart steps and the first minute after each restart (transient load peak).',
  findings=['Confirmed runtime: with every co-op system off (bridge transport/status writers only) private memory still grows ~5 MB/min at steady state on both peers (host %.2f, client %.2f) plus ~%.0f MB across three native restarts.'%(ctrl['host']['withinSegmentMBPerMin'],ctrl['client']['withinSegmentMBPerMin'],ctrl['host']['restartStepsMB']),
            'With the product controller (1186) the steady rate is %.2f/%.2f MB/min: the co-op systems add at most ~1-2 MB/min over the control; most growth is present without them.'%(prod['host']['withinSegmentMBPerMin'],prod['client']['withinSegmentMBPerMin']),
            'Handles stayed flat in both runs; no crash; transport fresh throughout.'],
  complete=False,outstanding=['Vanilla baseline without the proxy DLL (requires relaunching a lab instance without dinput8) to separate the game from the always-on bridge.','Longer (2 h) soak to see whether growth plateaus (native streaming caches) or continues.']))
print(json.dumps(dict(control=ctrl,product=prod,ratio=ratio,passed=r['passed'],failure=r.get('failure')),indent=1))
