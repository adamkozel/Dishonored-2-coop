$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation active'}
 & 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -X utf8 "$PSScriptRoot/archive_shadowwalk_fx_v533.py"
 if($LASTEXITCODE){throw 'Archive/stage failed'}
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
