#!/usr/bin/env bash
# 2041 (#237, #238, #239): reruns after campaign v2040a on candidate gameover-v2041 (the partner leaves GameOver to the
# host under host story; v2040a madinventor ended on the partner's own "Sokolov dead" GameOver) with harness
# Route-MissionVM-v2017 (a screenshot grab that has not finished after 15 s is killed, and a minimized host window is
# restored first; v2040a dunwall waited 1 h 45 min on one grab). Host story, items per player and safe codes on.
# 2041b: candidate gameover-v2041b (active missions reach the partner through the gameflow mirror; co-op saves
# service gap in the status). Takes campaign-<mission>-vm-ipp-v2041b<letter>/.
# Usage: Run-Campaign-v2041b.sh <letter> <plan>. Results: campaign-<mission>-vm-ipp-v2041b<letter>/
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v2041b-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-ipp-v2041b$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v2017.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-gameover-v2041b -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('hostStory'),n.get('worldDiff'),n.get('frameTimeouts'))" 2>&1 | head -c 600)"
done < "$PLAN"
