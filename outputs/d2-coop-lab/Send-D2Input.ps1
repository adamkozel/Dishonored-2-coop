[CmdletBinding()]
param(
    [Parameter(Mandatory)][ValidateSet('host','client')][string]$Role,
    [ValidateSet('Neutral','A','B','X','Y','Start','Back','Up','Down','Left','Right','RightShoulder','LeftShoulder','LeftThumb','RightThumb')][string[]]$Button=@('Neutral'),
    [ValidateRange(-32768,32767)][int]$LeftX=0,
    [ValidateRange(-32768,32767)][int]$LeftY=0,
    [ValidateRange(-32768,32767)][int]$RightX=0,
    [ValidateRange(-32768,32767)][int]$RightY=0,
    [ValidateRange(0,255)][int]$LeftTrigger=0,
    [ValidateRange(0,255)][int]$RightTrigger=0,
    [ValidateRange(100,10000)][int]$DurationMs=400
)
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'tooling/Publish-D2Packet.ps1')
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$instanceRoot=Join-Path $workspaceRoot "work\lab-instances\$Role"
$launch=Get-Content -LiteralPath (Join-Path $instanceRoot 'launch.json') -Raw | ConvertFrom-Json
$process=Get-Process -Id $launch.pid -ErrorAction Stop
if($process.Path -ne $launch.executable) { throw 'The recorded lab process no longer matches.' }
$buttons=@{RightShoulder=0x0200;LeftShoulder=0x0100;LeftThumb=0x0040;RightThumb=0x0080;Neutral=0;A=0x1000;B=0x2000;X=0x4000;Y=0x8000;Start=0x10;Back=0x20;Up=1;Down=2;Left=4;Right=8}
$buttonMask=0
foreach($b in $Button){$buttonMask=$buttonMask -bor $buttons[$b]}
$sequence=[uint32]([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds() % 4294967296L)
if($sequence -eq 0) {$sequence=1}
$stream=[IO.MemoryStream]::new()
$writer=[IO.BinaryWriter]::new($stream)
$writer.Write([uint32]0x49433244)
$writer.Write([uint16]1)
$writer.Write([uint16]40)
$writer.Write($sequence)
$expiresUtc=[DateTime]::UtcNow.AddMilliseconds($DurationMs)
$writer.Write([uint64]$expiresUtc.ToFileTimeUtc())
$writer.Write([uint16]$buttonMask)
$writer.Write([byte]$LeftTrigger); $writer.Write([byte]$RightTrigger)
$writer.Write([int16]$LeftX); $writer.Write([int16]$LeftY)
$writer.Write([int16]$RightX); $writer.Write([int16]$RightY)
$writer.Write([uint64]0)
$writer.Flush()
try {
    Publish-D2Packet -Path (Join-Path $instanceRoot 'input.bin') -Bytes $stream.ToArray() -ExpiresUtc $expiresUtc
} finally {
    $writer.Dispose(); $stream.Dispose()
}
[pscustomobject]@{Role=$Role;Sequence=$sequence;Button=$Button;DurationMs=$DurationMs;LeftX=$LeftX;LeftY=$LeftY;RightX=$RightX;RightY=$RightY}
