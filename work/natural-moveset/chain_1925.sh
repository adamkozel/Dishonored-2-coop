#!/usr/bin/env bash
# 1925 (#230): candidate 1214kp (1214ko + the time-bend status finite), then both power effect matrices with
# Route-MissionVM-v1923 (cast on land at the Addermire station, facing retries, on Easy), each after a lab relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
echo "build $(date +%H:%M)"
"$P" -NoProfile -File "$M/Build-Candidate-v1214kp.ps1" > "$M/candidate-run-v1214kp.log" 2>&1 || { echo "build failed"; exit 1; }
echo "deploy $(date +%H:%M)"
"$P" -NoProfile -File "$M/Deploy-Candidate-v1214kp.ps1" > "$M/deploy-run-v1214kp.log" 2>&1 || { echo "deploy failed"; exit 1; }
for pair in "corvohost corvo emily" "emilyhost emily corvo"; do
  set -- $pair; t="powerfx-$1-vm-v1923a"
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  echo "matrix $1 $(date +%H:%M)"
  "$P" -NoProfile -File Route-MissionVM-v1923.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kp -StepsFile routes/powerfx-$1-vm-v1923.json -CoopSaves -HostCharacter $2 -PartnerCharacter $3 > $t.log 2>&1
  echo "$1 exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
echo "chain done $(date +%H:%M)"
