param([Parameter(Mandatory)][string]$Path,[Parameter(Mandatory)][string]$Out,[int]$DurationMs=20000,[int]$IntervalMs=25)
# Read-only: copies a lab status JSON snapshot (with a local timestamp) as one
# NDJSON line whenever its content changes.
$sw=[Diagnostics.Stopwatch]::StartNew();$prev='';$w=[IO.StreamWriter]::new($Out,$false)
try{while($sw.ElapsedMilliseconds-lt$DurationMs){
 try{$t=[IO.File]::ReadAllText($Path)}catch{$t=''}
 if($t-and$t-ne$prev){$prev=$t;$w.WriteLine('{"ms":'+$sw.ElapsedMilliseconds+',"tick":'+[Environment]::TickCount64+',"status":'+$t.Trim()+'}')}
 Start-Sleep -Milliseconds $IntervalMs}}finally{$w.Dispose()}
