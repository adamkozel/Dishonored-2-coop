#!/usr/bin/env bash
# 2031 (#237, stage 8.4.1): the campaign missions under host story on candidate hoststory-v2031, with
# Route-MissionVM-v2013 -HostStory on two accounts (host Corvo on this PC, VM partner Emily), co-op saves and story
# flags. Each mission's route is the one that passed for 0.9.6 (v1926; v1931 for eotw, dust and empress). The VM
# partner must follow the host into host story, run no World node of its own, and end with the worlds equal.
# 2036 (#237, #238): all nine missions with host story and items per player on (Route-MissionVM-v2015), candidate itemsperplayer-v2036. Results: campaign-<mission>-vm-ipp-v2036<letter>/
# Results: campaign-<mission>-vm-hoststory-v2031<letter>/.
# 2040: harness v2016 (Go on a logic-9 transition choice; per-step pickup catch-up and kiscule status), candidate safecodes-v2039 (host story, items per player, safe codes). Results: campaign-<mission>-vm-ipp-v2040<letter>/
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v2040-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-ipp-v2040$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v2016.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-safecodes-v2039 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('hostStory'),n.get('worldDiff'))" 2>&1 | head -c 600)"
done < "$PLAN"
