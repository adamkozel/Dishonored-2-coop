#!/usr/bin/env bash
# 1889 (#213, #177): the nine missions on two Steam accounts (host Corvo on this PC, VM partner Emily), product mode and
# co-op saves, with the 0.9.3 binary (candidate 1214kb), one take per mission in order. Each line of
# campaign-v1889-plan.txt is "<mission> <mapinfo> <steps file>"; a failed mission does not stop the run. The take letter
# is the first argument (default a). Results: campaign-<mission>-vm-v1889<letter>/.
# The 1888 runner read the CRLF plan's carriage return into the steps path, so all nine v1888a takes failed at once;
# this one strips it.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1889$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1889.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214kb -StepsFile "$steps" -CoopSaves > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
done < campaign-v1889-plan.txt
