#!/usr/bin/env bash
# 1888 (#213): the nine missions on two Steam accounts (host Corvo on this PC, VM partner Emily), product mode and co-op
# saves, with the 0.9.2 binary (candidate 1214ka), one take per mission in order. Each line of campaign-v1888-plan.txt is
# "<mission> <mapinfo> <steps file>"; a failed mission does not stop the run. Results: campaign-<mission>-vm-v1888a/.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
while read -r m mapinfo steps; do
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1888a"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1887.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214ka -StepsFile "$steps" -CoopSaves > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done < campaign-v1888-plan.txt
