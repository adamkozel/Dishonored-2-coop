#!/usr/bin/env bash
# 1935 (#228): after chain_1934.sh ends, Emily's Far Reach NPC pull in the host role filmed again, cast sooner from 4.5 m, the camera behind
# her along the gallery (Record-Coverage-v1935 farreachpull, -HostSlot 80, 1214kq), after a fresh relaunch.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1934.sh"; do sleep 30; done
for t in coverage-v1935-farreachpull; do
  if [ -e "$t" ]; then echo "$t exists"; continue; fi
  "$P" -NoProfile -File "$M/Deploy-SourceClock.ps1" > "relaunch-$t.log" 2>&1
  echo "pull $(date +%H:%M)"
  "$P" -NoProfile -Command "& ./Record-Coverage-v1935.ps1 -Take $t -Candidate candidate-npc-combat-v1214kq -Sources host -Cases farreachpull -HostSlot 80" > "$t.log" 2>&1
  echo "farreachpull exit $? $(python -c "import json;r=json.load(open('$t/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done
echo "chain done $(date +%H:%M)"
