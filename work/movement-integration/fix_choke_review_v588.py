from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
note='Emily586 passes12 numerical/native/Steam/root/restoration checks, but the inspected observer video is below the balcony and does not show her body. This is not visual coverage. Correct camera recording is required; visualAccepted and complete remain false.'
p=N/'choke-network-emily-v586/observer-visual-review.json';r=rd(p);r['observations']=[note];wr(p,r)
p=N/'runtime-video-manifest.json';vs=rd(p)
for v in vs:
 if v['video']=='choke-network-emily-v586/observer.mp4':v['scope']+=' '+note
wr(p,vs)
for p in (W/'choke-verification-v587.json',W/'candidate-choke-network-v584/runtime-verification-v587.json'):
 r=rd(p);r['limitations'].append(note);wr(p,r)
p=R/'work/current-task-state.json';r=rd(p);r['ChokeContinuation']['latest']+=' '+note;r['MovementExecutionPriority']['current']=r['ChokeContinuation']['latest'];wr(p,r)
p=N/'review-evidence-annotations.json';r=rd(p);r['items']['nonlethal_takedown_front_back_sides']['notes']+=' '+note;wr(p,r)
s=(N/'Test-ChokeNetwork-v585.ps1').read_text(encoding='utf-8-sig').replace('ChokeNetwork585','ChokeNetwork589')
a='''     $ox=$px-2*$fx-2*$fy;$oy=$py-2*$fy+2*$fx;$oyaw=[Math]::Atan2($py-$oy,$px-$ox)*180/[Math]::PI
     C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f $ox.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oy.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oyaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))'''
b='''     $cameraTrials=[Collections.Generic.List[object]]::new();$cameraOk=$false
     foreach($side in @(2,-2,0)){
      $ox=$px-2*$fx-$side*$fy;$oy=$py-2*$fy+$side*$fx;$oyaw=[Math]::Atan2($py-$oy,$px-$ox)*180/[Math]::PI
      C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f $ox.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oy.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oyaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
      Start-Sleep -Milliseconds 600;$feet=(Q $peer '--player-states').position
      $cameraTrials.Add([pscustomobject]@{side=$side;x=$ox;y=$oy;actual=$feet})
      if([math]::Abs($feet[2]-$g.position[2])-lt.3-and[math]::Abs($feet[0]-$ox)-lt.6-and[math]::Abs($feet[1]-$oy)-lt.6){$cameraOk=$true;$ox=[double]$feet[0];$oy=[double]$feet[1];break}
     }
     $cameraTrials|ConvertTo-Json -Depth 5|Set-Content "$out/observer-floor-trials.json"
     if(!$cameraOk){throw 'No same-floor observer position found'}'''
assert s.count(a)==1;s=s.replace(a,b)
a='''     $g|ConvertTo-Json -Depth 8|Set-Content "$out/guard-before-input.json"
     Start-Sleep'''
b='''     $g|ConvertTo-Json -Depth 8|Set-Content "$out/guard-before-input.json"
     $oyaw=[Math]::Atan2($py-$oy,$px-$ox)*180/[Math]::PI
     C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f $ox.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oy.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oyaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
     Start-Sleep'''
assert s.count(a)==1;s=s.replace(a,b)
# Preserve enough capture/test time when rejecting an unsafe camera candidate.
s=s.replace("'-t','14'","'-t','18'").replace("'--watch-midnight','13000'","'--watch-midnight','17000'").replace('$watch.ElapsedMilliseconds-lt12000','$watch.ElapsedMilliseconds-lt16000')
(N/'Test-ChokeNetwork-v589.ps1').write_text(s)
print('Corrected Emily586 visual record and prepared same-floor camera test589.')
