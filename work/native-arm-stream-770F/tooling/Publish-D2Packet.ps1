function Publish-D2Packet {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)][string]$Path,
        [Parameter(Mandatory)][byte[]]$Bytes,
        [Parameter(Mandatory)][datetime]$ExpiresUtc,
        [ValidateRange(1,500)][int]$RetryWindowMs=150
    )
    # Same-directory rename keeps readers on a complete old or new packet.
    # Windows can temporarily refuse replacement while another handle is open.
    # Retry only sharing/access errors; never recreate or extend a control lease.
    $temporary=Join-Path (Split-Path $Path -Parent) ('packet-'+[guid]::NewGuid().ToString('N')+'.tmp')
    $watch=[Diagnostics.Stopwatch]::StartNew()
    try {
        [IO.File]::WriteAllBytes($temporary,$Bytes)
        while($true) {
            if([DateTime]::UtcNow -ge $ExpiresUtc){throw 'Packet expired before atomic publication.'}
            try {
                [IO.File]::Move($temporary,$Path,$true)
                return
            }catch{
                $cause=$_.Exception
                while($cause.InnerException){$cause=$cause.InnerException}
                $code=$cause.HResult -band 0xffff
                if($code -notin 5,32,33 -or $watch.ElapsedMilliseconds -ge $RetryWindowMs -or !(Test-Path -LiteralPath $temporary)){throw}
                Start-Sleep -Milliseconds 10
            }
        }
    }finally{
        if(Test-Path -LiteralPath $temporary){Remove-Item -LiteralPath $temporary}
    }
}
