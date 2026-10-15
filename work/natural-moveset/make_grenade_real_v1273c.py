"""1273c (#14 real grenade, 1214au): Test-GrenadeReal-v1273b.ps1 with a clear line of throw. In 1273b the thrower stood on the
line from (-33,-153) to the guard, which crosses the office door: both grenades blew the door apart and never reached the
guard. The thrower now stands -Distance metres (default 3.5) in front of the guard along its facing, on the same walkway,
facing it; the other peer turns in place and holds its free observer camera 3 m to the guard's side, 1.8 m above its feet."""
from pathlib import Path
N = Path(__file__).resolve().parent
src = (N / 'Test-GrenadeReal-v1273b.ps1').read_bytes().decode('utf-8')
crlf = '\r\n' in src
s = src.replace('\r\n', '\n')
def rep(old, new, count=1):
    global s
    assert s.count(old) == count, (s.count(old), old[:90])
    s = s.replace(old, new)
rep("[string]$Take='grenade-real-v1273b',", "[string]$Take='grenade-real-v1273c',")
rep("[double]$Distance=5,", "[double]$Distance=3.5,")
rep("# 1273b (#14, 1214au)", "# 1273c (#14, 1214au): thrower in front of the guard on its walkway; the other peer films with the free camera. Generated\n# by make_grenade_real_v1273c.py.\n# 1273b (#14, 1214au)")
rep("""   # Stand on the line from the fixture's open spot (-33,-153) to the guard, $Distance m from it, facing it.
   $g=Guard $Thrower;$vx=-33-[double]$g.position[0];$vy=-153-[double]$g.position[1];$vl=[Math]::Max([Math]::Sqrt($vx*$vx+$vy*$vy),0.01);$fx=-$vx/$vl;$fy=-$vy/$vl;$yaw=[Math]::Atan2($fy,$fx)*180/[Math]::PI
   C $Thrower ('setviewpos {0} {1} {2} {3} 0' -f (F ([double]$g.position[0]-$Distance*$fx)),(F ([double]$g.position[1]-$Distance*$fy)),(F ([double]$g.position[2]+1.7)),$yaw.ToString('F4',$inv));Start-Sleep -Milliseconds 500""",
"""   # Stand $Distance m in front of the guard along its facing (same walkway), facing it.
   $g=Guard $Thrower;$gx=[double]$g.orientation[0];$gy=[double]$g.orientation[1];$tx=[double]$g.position[0]+$Distance*$gx;$ty=[double]$g.position[1]+$Distance*$gy;$yaw=[Math]::Atan2(-$gy,-$gx)*180/[Math]::PI
   C $Thrower ('setviewpos {0} {1} {2} {3} 0' -f (F $tx),(F $ty),(F ([double]$g.position[2]+1.7)),$yaw.ToString('F4',$inv));Start-Sleep -Milliseconds 500
   $obs=if($Thrower-eq'host'){'client'}else{'host'};$mx=([double]$g.position[0]+$tx)/2;$my=([double]$g.position[1]+$ty)/2;$cx=$mx-3.0*$gy;$cy=$my+3.0*$gx;$cz=[double]$g.position[2]+1.8
   $op=(Q $obs '--player').candidatePosition;$cyaw=[Math]::Atan2($my-$cy,$mx-$cx)*180/[Math]::PI
   C $obs ('setviewpos {0} {1} {2} {3} 0' -f (F ([double]$op[0])),(F ([double]$op[1])),(F ([double]$op[2]+1.7)),$cyaw.ToString('F4',$inv));Start-Sleep -Milliseconds 300
   try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $obs -Pitch 12 -Quick|Out-Null}catch{}
   C $obs ('coop_observer_camera hold_at {0} {1} {2}' -f (F $cx),(F $cy),(F $cz));Start-Sleep -Milliseconds 400""")
rep("""   foreach($c in @($captures)){try{if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}};$captures.Clear()""",
    """   foreach($c in @($captures)){try{if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}};$captures.Clear();C $obs 'coop_observer_camera off'""")
out = s.replace('\n', '\r\n') if crlf else s
(N / 'Test-GrenadeReal-v1273c.ps1').write_bytes(out.encode('utf-8'))
print('ok')
