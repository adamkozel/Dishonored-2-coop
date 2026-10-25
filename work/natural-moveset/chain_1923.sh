#!/usr/bin/env bash
# 1923 (#230): powerfx-corvohost-vm-v1921a refused at its start (object-carry-v1921a left the host out of gameplay after a
# lost command receipt). After chain_1922.sh ends: the lab pair relaunched (Deploy-SourceClock), then the Corvo-host
# matrix again on 1214ko (powerfx-corvohost-vm-v1921b).
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1922.sh"; do sleep 20; done
t=powerfx-corvohost-vm-v1921b; if [ -e "$t" ]; then echo "$t exists"; exit 1; fi
"$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
echo "matrix corvo host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1921.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214ko -StepsFile routes/powerfx-corvohost-vm-v1921.json -CoopSaves > $t.log 2>&1
echo "corvo host exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
echo "chain done $(date +%H:%M)"
