# Runs INSIDE the VM on the interactive desktop (start it with Start-D2LabVMInteractive):
# records the Dishonored 2 window with Windows Graphics Capture for -Seconds and stores the clip.
# WGC delivers a frame each time the game presents a changed image, so the clip's frame
# timestamps measure the delivered frame rate. Analyse with Get-D2LabVMFps (host side).
param([string]$Name = 'fps', [int]$Seconds = 10)
$dir = 'C:\D2Lab\fps'; New-Item -ItemType Directory -Force $dir | Out-Null
$out = Join-Path $dir "$Name.mp4"; $log = Join-Path $dir "$Name.log"; $done = Join-Path $dir "$Name.done"
Remove-Item $out, $log, $done -ErrorAction SilentlyContinue
try {
    $p = Get-Process Dishonored2 -ErrorAction Stop | Select-Object -First 1
    $h = $p.MainWindowHandle.ToInt64(); if (!$h) { throw 'Dishonored 2 has no window on this desktop' }
    & C:\D2Lab\tools\ffmpeg.exe -hide_banner -loglevel warning -nostdin -f lavfi `
        -i "gfxcapture=hwnd=${h}:capture_cursor=0:max_framerate=240:width=-2:height=-2,hwdownload,format=bgra" `
        -t $Seconds -an -c:v libx264 -preset ultrafast -bf 0 -crf 23 -pix_fmt yuv420p -fps_mode passthrough -y $out 2> $log
    "ok $LASTEXITCODE" | Set-Content $done
} catch { "error $($_.Exception.Message)" | Set-Content $done }
