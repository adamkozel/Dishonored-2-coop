#!/usr/bin/env bash
# 1917: after Run-Campaign-v1917.sh b ends, the Royal Conservatory on campaign-v1917c-plan.txt (letter c), then Emily's
# powers in the host role on the local lab pair: Record-Coverage-v1917 (coverage-v1917a) and Probe-ObjectCarry-v1917
# (object-carry-v1917a), both with -HostSlot 80 on candidate 1214km.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
while ps -ef | grep -v grep | grep -q "Run-Campaign-v1917.sh b"; do sleep 20; done
echo "start c $(date +%H:%M)"
bash Run-Campaign-v1917.sh c campaign-v1917c-plan.txt > campaign-v1917c-run.log 2>&1
echo "c done: $(cat campaign-v1917c-run.log)"
for t in coverage-v1917a object-carry-v1917a; do if [ -e "$t" ]; then echo "$t exists"; exit 1; fi; done
"$P" -NoProfile -File Record-Coverage-v1917.ps1 -Take coverage-v1917a -Candidate candidate-npc-combat-v1214km -Sources host -Cases farreach,farreachcancel,farreachfar,darkvision,doppelganger,mesmerize -HostSlot 80 > coverage-v1917a.log 2>&1
echo "coverage exit $? $(python -c "import json;r=json.load(open('coverage-v1917a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
"$P" -NoProfile -File Probe-ObjectCarry-v1917.ps1 -Take object-carry-v1917a -Candidate candidate-npc-combat-v1214km -Sources host -Cases farreachbottle -HostSlot 80 > object-carry-v1917a.log 2>&1
echo "object-carry exit $? $(python -c "import json;r=json.load(open('object-carry-v1917a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
