#!/usr/bin/env bash
# 1906 (release 0.9.4): the nine missions on two Steam accounts (host Corvo on this PC, VM partner Emily), product mode
# and co-op saves, with the 0.9.4 binary (candidate 1214kf), one take per mission in order, on Route-MissionVM-v1891 (the
# lab's idle local partner instance stopped during each take). The Grand Palace runs on Easy through the difficulty sync
# (routes/campaign-palace-vm-v1890.json, as in campaign-palace-vm-v1890a). The take letter is the first argument
# (default a) and an optional second argument names another plan file. Results: campaign-<mission>-vm-v1906<letter>/.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v1906-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1906$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1891.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214kf -StepsFile "$steps" -CoopSaves > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done < "$PLAN"
