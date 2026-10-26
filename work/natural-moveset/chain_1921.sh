#!/usr/bin/env bash
# 1921 (#230, #228): candidate 1214ko (native power effects and partner props on any install), then the power effect
# matrix on two accounts with Route-MissionVM-v1920 (every effect expected; Dark Vision judged on frames): Corvo host with
# Emily partner, then Emily host with Corvo partner. Then Emily's powers in the host role on the local lab pair, one
# Record-Coverage-v1917 case per take after a fresh relaunch, and the Far Reach pull (Probe-ObjectCarry-v1917).
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
echo "build $(date +%H:%M)"
"$P" -NoProfile -File "$M/Build-Candidate-v1214ko.ps1" > "$M/candidate-run-v1214ko.log" 2>&1 || { echo "build failed"; exit 1; }
echo "deploy $(date +%H:%M)"
"$P" -NoProfile -File "$M/Deploy-Candidate-v1214ko.ps1" > "$M/deploy-run-v1214ko.log" 2>&1 || { echo "deploy failed"; exit 1; }
for t in powerfx-corvohost-vm-v1920a powerfx-emilyhost-vm-v1920a; do if [ -e "$t" ]; then echo "$t exists"; exit 1; fi; done
echo "matrix corvo host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1920.ps1 -Take powerfx-corvohost-vm-v1920a -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214ko -StepsFile routes/powerfx-corvohost-vm-v1920.json -CoopSaves > powerfx-corvohost-vm-v1920a.log 2>&1
echo "corvo host exit $? $(python -c "import json;r=json.load(open('powerfx-corvohost-vm-v1920a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
echo "matrix emily host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1920.ps1 -Take powerfx-emilyhost-vm-v1920a -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214ko -StepsFile routes/powerfx-emilyhost-vm-v1920.json -CoopSaves -HostCharacter emily -PartnerCharacter corvo > powerfx-emilyhost-vm-v1920a.log 2>&1
echo "emily host exit $? $(python -c "import json;r=json.load(open('powerfx-emilyhost-vm-v1920a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
for c in farreachcancel farreachfar darkvision doppelganger mesmerize; do
  t="coverage-v1921-$c"; if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -Command "& ./Record-Coverage-v1917.ps1 -Take $t -Candidate candidate-npc-combat-v1214ko -Sources host -Cases $c -HostSlot 80" > "$t.log" 2>&1
  echo "$c exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
t=object-carry-v1921a
if [ ! -e "$t" ]; then
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -Command "& ./Probe-ObjectCarry-v1917.ps1 -Take $t -Candidate candidate-npc-combat-v1214ko -Sources host -Cases farreachbottle -HostSlot 80" > "$t.log" 2>&1
  echo "object-carry exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
fi
echo "chain done $(date +%H:%M)"
