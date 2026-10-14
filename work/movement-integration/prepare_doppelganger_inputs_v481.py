from pathlib import Path

R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
def read(p): return p.read_text(encoding='utf-8-sig')
def new(p,t):
    assert not p.exists(),p
    p.write_text(t,encoding='utf-8')
new(W/'Build-DoppelgangerVisual-v481.ps1',read(W/'Build-DoppelgangerVisual-v479.ps1').replace('v479','v481').replace('Build427','Build481'))
s=read(W/'prepare_doppelganger_fx_v479.py').replace('v479','v481').replace("previous = W / 'candidate-doppelganger-fx-v475'","previous = W / 'candidate-doppelganger-fx-v479'")
s=s.replace('475 plus exact refused joint matrix/sequence capture in source status. Native478 independent later read names leaf anchor_jnt but was already valid again. Bounds unchanged; capture the actual rejected sample before choosing a fix','Native480 exact rigid leaf marker offsets exceed former3m bound. Wire4 keeps10712 bytes; only verified219-rig leaves1/10/17 allow10m, other joints3m. Shared sender/receiver/interpolation validation and regression cases; cadence475 and finite native FX468 retained')
s=s.replace('DoppelgangerFxDeploy452','DoppelgangerFxDeploy481')
new(W/'prepare_doppelganger_fx_v481.py',s)
s=read(N/'Test-DoppelgangerFx-v480.ps1')
s=s.replace(" $epochStart=@{", " $cloneStart|ConvertTo-Json -Depth 8|Set-Content \"$out/sender-start.json\"\n $epochStart=@{")
needle=" $checks.Add('Native clone identity/root/lifetime traverses Steam with exact accepted packet digests and authoritative receiver retirement')"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 $transportHealth=[ordered]@{packetRefusedDelta=([long]$senderTail.packetRefused-[long]$cloneStart.packetRefused);poseReadFailedDelta=([long]$senderTail.poseReadFailed-[long]$cloneStart.poseReadFailed);sendFailedDelta=([long]$senderTail.sendFailed-[long]$cloneStart.sendFailed);preparedDelta=([long]$senderTail.prepared-[long]$cloneStart.prepared);sentDelta=([long]$senderTail.sent-[long]$cloneStart.sent)}
 $transportHealth|ConvertTo-Json|Set-Content "$out/transport-health.json"
 if($transportHealth.packetRefusedDelta-or$transportHealth.poseReadFailedDelta-or$transportHealth.sendFailedDelta){throw 'Native clone pose validation, read or Steam send dropped a sample'}
 $checks.Add('Native clone body and helper poses stream without validation rejection, failed native read or failed Steam send')
''')
new(N/'Test-DoppelgangerFx-v482.ps1',s)
new(W/'Run-DoppelgangerFx-v482.ps1',read(W/'Run-DoppelgangerFx-v476.ps1').replace('476','482').replace('477','483'))
print('Prepared481 build/archive and482/483 two-cast harness')
