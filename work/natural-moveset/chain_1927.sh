#!/usr/bin/env bash
# 1927 (release 0.9.6): after chain_1926.sh ends, the lab pair relaunched and the nine-mission regression on 1214kp
# (Run-Campaign-v1926.sh a).
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1926.sh"; do sleep 20; done
"$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > relaunch-campaign-v1926a.log 2>&1
echo "regression $(date +%H:%M)"
bash Run-Campaign-v1926.sh a > campaign-v1926-run.log 2>&1
echo "regression done $(date +%H:%M)"
cat campaign-v1926-run.log
