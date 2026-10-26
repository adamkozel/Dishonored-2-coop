#!/usr/bin/env bash
# 1933 (#230): after chain_1932.sh ends, the held Heart and trap of each player in the other world, one item per take
# (the lab grant equips only into a free left hand), in both pairings on 1214kq with Route-MissionVM-v1933, each after
# a lab relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1932.sh"; do sleep 30; done
for spec in "heart corvohost corvo emily" "trap corvohost corvo emily" "heart emilyhost emily corvo" "trap emilyhost emily corvo"; do
  set -- $spec; t="itemprops-$1-$2-vm-v1933a"
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  echo "$1 $2 $(date +%H:%M)"
  "$P" -NoProfile -File Route-MissionVM-v1933.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kq -StepsFile routes/itemprops-$1-$2-vm-v1933.json -CoopSaves -HostCharacter $3 -PartnerCharacter $4 > $t.log 2>&1
  echo "$1 $2 exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
echo "chain done $(date +%H:%M)"
