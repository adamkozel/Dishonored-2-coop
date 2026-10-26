#!/usr/bin/env bash
# 1928 (#230, #228): after chain_1927.sh (the 1214kp regression) ends: the partner's Bend Time, Stop Time and Possession
# in the Emily-host pairing (Route-MissionVM-v1925, powerfx-emilyhost-vm-v1925a), then the host-role Far Reach pull of the
# bottle again (Probe-ObjectCarry-v1917, -HostSlot 80), each after a lab relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1927.sh"; do sleep 30; done
t=powerfx-emilyhost-vm-v1925a
if [ ! -e "$t" ]; then
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  echo "emily host short $(date +%H:%M)"
  "$P" -NoProfile -File Route-MissionVM-v1925.ps1 -Take $t -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kp -StepsFile routes/powerfx-emilyhost-vm-v1925.json -CoopSaves -HostCharacter emily -PartnerCharacter corvo > $t.log 2>&1
  echo "emilyhost exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
fi
t=object-carry-v1928a
if [ ! -e "$t" ]; then
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -Command "& ./Probe-ObjectCarry-v1917.ps1 -Take $t -Candidate candidate-npc-combat-v1214kp -Sources host -Cases farreachbottle -HostSlot 80" > "$t.log" 2>&1
  echo "object-carry exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
fi
echo "chain done $(date +%H:%M)"
