#!/usr/bin/env bash
# 1926 (#230): the powers powerfx-*-vm-v1923a left, on 1214kp with Route-MissionVM-v1924: the host's Possession (with its
# parasite upgrade) in the Corvo-host pairing; the partner's Bend Time, Stop Time and Possession in the Emily-host pairing.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
for pair in "corvohost corvo emily" "emilyhost emily corvo"; do
  set -- $pair; t="powerfx-$1-vm-v1924a"
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  echo "matrix $1 $(date +%H:%M)"
  "$P" -NoProfile -File Route-MissionVM-v1924.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kp -StepsFile routes/powerfx-$1-vm-v1924.json -CoopSaves -HostCharacter $2 -PartnerCharacter $3 > $t.log 2>&1
  echo "$1 exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
echo "chain done $(date +%H:%M)"
