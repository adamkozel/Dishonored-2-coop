#!/usr/bin/env bash
# 1929 (release 0.9.6): candidate 1214kq (1214kp with the version resources 0.9.6), the nine-mission regression on it
# (Run-Campaign-v1929.sh a), then the partner's Bend Time, Stop Time and Possession in the Emily-host pairing
# (powerfx-emilyhost-vm-v1929a) and the host-role Far Reach bottle pull (object-carry-v1929a), each after a relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
echo "build $(date +%H:%M)"
"$P" -NoProfile -File "$M/Build-Release096-v1214kq.ps1" > "$M/candidate-run-v1214kq.log" 2>&1 || { echo "build failed"; exit 1; }
echo "deploy $(date +%H:%M)"
"$P" -NoProfile -File "$M/Deploy-Release096-v1214kq.ps1" > "$M/deploy-run-v1214kq.log" 2>&1 || { echo "deploy failed"; exit 1; }
echo "regression $(date +%H:%M)"
bash Run-Campaign-v1929.sh a > campaign-v1929-run.log 2>&1
echo "regression done $(date +%H:%M)"
cat campaign-v1929-run.log
t=powerfx-emilyhost-vm-v1929a
if [ ! -e "$t" ]; then
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -File Route-MissionVM-v1925.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kq -StepsFile routes/powerfx-emilyhost-vm-v1925.json -CoopSaves -HostCharacter emily -PartnerCharacter corvo > $t.log 2>&1
  echo "emilyhost exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
fi
t=object-carry-v1929a
if [ ! -e "$t" ]; then
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -Command "& ./Probe-ObjectCarry-v1917.ps1 -Take $t -Candidate candidate-npc-combat-v1214kq -Sources host -Cases farreachbottle -HostSlot 80" > "$t.log" 2>&1
  echo "object-carry exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
fi
echo "chain done $(date +%H:%M)"
