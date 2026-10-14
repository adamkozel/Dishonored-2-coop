[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspace=Split-Path (Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent) -Parent
$toolRoot=Join-Path $workspace 'work/tools'
$ghidra=Join-Path $toolRoot 'ghidra_12.1.4_PUBLIC'
$java=Join-Path $toolRoot 'jdk-21.0.12.1+1/bin/java.exe'
$jar=Join-Path $toolRoot 'ghidra-mcp-extension-6.0.0/GhidraMCP/lib/GhidraMCP-6.0.0.jar'
$runtime=Join-Path $toolRoot 'ghidra-runtime'
$project=Join-Path $PSScriptRoot 'project/Dishonored2-B671F824.gpr'
New-Item -ItemType Directory -Force $runtime | Out-Null
$health=$null
try {$health=Invoke-RestMethod 'http://127.0.0.1:8089/health' -TimeoutSec 2}catch{}
if($health){
 if($health.version-ne'6.0.0-headless'){throw 'Port 8089 belongs to a different server version'}
 if((Test-Path -LiteralPath $project)-and$health.program_name-ne'Dishonored2.exe'){throw 'Ghidra is running but the expected saved program is not loaded'}
 $health;return
}
$jars=@($jar)+@(Get-ChildItem "$ghidra/Ghidra" -Filter '*.jar' -Recurse -File | ForEach-Object FullName)
$arguments=@('-Xmx2G','-XX:ActiveProcessorCount=2','-Djava.awt.headless=true',('-Duser.home='+$runtime),('-Dapplication.settingsdir='+$runtime+'/settings'),('-Dapplication.cachedir='+$runtime+'/cache'),('-Dapplication.tempdir='+$runtime+'/tmp'),('-Dghidra.home='+$ghidra),'-Dapplication.name=GhidraMCP','-classpath',($jars -join ';'),'com.xebyte.headless.GhidraMCPHeadlessServer','--bind','127.0.0.1','--port','8089')
if(Test-Path -LiteralPath $project){$arguments+=@('--project',$project,'--program','/Dishonored2.exe')}
$argFile=Join-Path $runtime 'server.args'
($arguments|ForEach-Object {'"'+($_ -replace '\\','/')+'"'})|Set-Content -LiteralPath $argFile -Encoding utf8NoBOM
$env:GHIDRA_MCP_FILE_ROOT=$workspace
$env:GHIDRA_MCP_ALLOW_SCRIPTS='1'
$process=Start-Process -FilePath $java -ArgumentList ('@"'+$argFile+'"') -WindowStyle Hidden -PassThru -RedirectStandardOutput "$runtime/server.log" -RedirectStandardError "$runtime/server.err.log"
$process.Id | Set-Content "$runtime/server.pid"
$timer=[Diagnostics.Stopwatch]::StartNew()
do {
 Start-Sleep -Milliseconds 500
 if($process.HasExited){Get-Content "$runtime/server.err.log";throw 'Ghidra server exited during startup'}
 try {$health=Invoke-RestMethod 'http://127.0.0.1:8089/health' -TimeoutSec 2}catch{}
 if($health){
  if((Test-Path -LiteralPath $project)-and$health.program_name-ne'Dishonored2.exe'){throw 'Server started but failed to reopen the saved program; inspect server.err.log'}
  $health;return
 }
}while($timer.Elapsed.TotalSeconds-lt45)
throw 'Ghidra server did not become healthy within 45 seconds; inspect its local log'
