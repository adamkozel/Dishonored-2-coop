$ErrorActionPreference='Stop'
$frames=Get-Content "$PSScriptRoot/controller-video-4/timestamps.json" -Raw|ConvertFrom-Json
$lines=[Collections.Generic.List[string]]::new();$lines.Add('ffconcat version 1.0')
for($i=0;$i-lt$frames.Count;$i++){
 $lines.Add("file 'controller-video-4/$($frames[$i].path)'")
 $duration=if($i+1-lt$frames.Count){($frames[$i+1].ms-$frames[$i].ms)/1000}else{.07}
 $lines.Add('duration '+$duration.ToString('F9',[Globalization.CultureInfo]::InvariantCulture))
}
$lines.Add("file 'controller-video-4/$($frames[-1].path)'")
[IO.File]::WriteAllLines("$PSScriptRoot/runtime.ffconcat",$lines,[Text.UTF8Encoding]::new($false))
& ffmpeg -hide_banner -loglevel error -y -safe 0 -f concat -i "$PSScriptRoot/runtime.ffconcat" -ss 1.4 -t 9.8 -vf fps=30 -c:v libx264 -crf 18 -pix_fmt yuv420p -movflags +faststart "$PSScriptRoot/d2-fullbody-sequence.mp4"
if($LASTEXITCODE){throw 'Runtime encoding failed'}
& ffmpeg -hide_banner -loglevel error -y -safe 0 -f concat -i "$PSScriptRoot/preview.ffconcat" -vf fps=30 -c:v libx264 -crf 18 -pix_fmt yuv420p -movflags +faststart "$PSScriptRoot/blender-sequence.mp4"
if($LASTEXITCODE){throw 'Authoring encoding failed'}
[pscustomobject]@{Runtime="$PSScriptRoot/d2-fullbody-sequence.mp4";Authoring="$PSScriptRoot/blender-sequence.mp4";ActualCaptureHz=($frames.Count-1)/(($frames[-1].ms-$frames[0].ms)/1000);DurationSource='Per-frame monotonic timestamp; no playback speedup';NoAudio=$true}
