#!/usr/bin/env bash
# 1920 (#228, Emily's powers in the host role): after chain_1919.sh ends, one Record-Coverage-v1917 take per case
# (-HostSlot 80, candidate 1214kn), each after a fresh lab relaunch (Deploy-SourceClock): coverage-v1917a lost a host
# command receipt after its first case and its fixture restore failed. Then Probe-ObjectCarry-v1917 (the Far Reach pull
# of the bottle) the same way.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1919.sh"; do sleep 30; done
n=0
for c in farreachcancel farreachfar darkvision doppelganger mesmerize; do
  n=$((n+1)); t="coverage-v1920-$c"
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -Command "& ./Record-Coverage-v1917.ps1 -Take $t -Candidate candidate-npc-combat-v1214kn -Sources host -Cases $c -HostSlot 80" > "$t.log" 2>&1
  echo "$c exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
t=object-carry-v1920a
if [ ! -e "$t" ]; then
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  "$P" -NoProfile -Command "& ./Probe-ObjectCarry-v1917.ps1 -Take $t -Candidate candidate-npc-combat-v1214kn -Sources host -Cases farreachbottle -HostSlot 80" > "$t.log" 2>&1
  echo "object-carry exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
fi
