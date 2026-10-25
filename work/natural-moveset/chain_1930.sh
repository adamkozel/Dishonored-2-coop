#!/usr/bin/env bash
# 1930 (#230): after chain_1929.sh ends, the held grenade, Heart and stun mine of each player in the other world, on
# 1214kq with Route-MissionVM-v1930, in both pairings, each after a lab relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1929.sh"; do sleep 30; done
for pair in "corvohost corvo emily" "emilyhost emily corvo"; do
  set -- $pair; t="itemprops-$1-vm-v1930a"
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  echo "items $1 $(date +%H:%M)"
  "$P" -NoProfile -File Route-MissionVM-v1930.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kq -StepsFile routes/itemprops-$1-vm-v1930.json -CoopSaves -HostCharacter $2 -PartnerCharacter $3 > $t.log 2>&1
  echo "$1 exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
echo "chain done $(date +%H:%M)"
