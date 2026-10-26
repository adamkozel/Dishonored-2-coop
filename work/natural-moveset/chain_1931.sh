#!/usr/bin/env bash
# 1931 (release 0.9.6): after chain_1930.sh ends, the retakes of the 1214kq regression (Edge of the World, the Dust
# District, Death to the Empress; Run-Campaign-v1931.sh a campaign-v1931-plan.txt), then Emily's Far Reach NPC pull in
# both roles (Route-MissionVM-v1931), each after a lab relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1930.sh"; do sleep 30; done
"$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > relaunch-campaign-v1931a.log 2>&1
echo "retakes $(date +%H:%M)"
bash Run-Campaign-v1931.sh a campaign-v1931-plan.txt > campaign-v1931-run.log 2>&1
cat campaign-v1931-run.log
for pair in "corvohost corvo emily" "emilyhost emily corvo"; do
  set -- $pair; t="farreachpull-$1-vm-v1931a"
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -File Route-MissionVM-v1931.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kq -StepsFile routes/farreachpull-$1-vm-v1931.json -CoopSaves -HostCharacter $2 -PartnerCharacter $3 > $t.log 2>&1
  echo "$1 pull exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
echo "chain done $(date +%H:%M)"
