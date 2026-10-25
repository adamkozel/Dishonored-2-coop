#!/usr/bin/env bash
# 1916 (release 0.9.5, #225): the nine-mission regression of Run-Campaign-v1915 on candidate 1214km, whose mission
# autosave reader no longer holds game.details while a native save renames over it (campaign-dunwall-vm-v1915a lost the
# host's first Dunwall autosave to it and its checkpoint sync never armed). Same harness (Route-MissionVM-v1915: casts
# from The Good Doctor on, story flags diffed at every start and end, co-op saves, the Grand Palace on Easy), same plan
# and routes; the co-op save folders keep the saves of earlier takes, as a player's do. The take letter is the first
# argument (default a) and an optional second argument names another plan file. Results: campaign-<mission>-vm-v1916<letter>/.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v1915-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1916$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1915.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214km -StepsFile "$steps" -CoopSaves -StoryFlags > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),'casts',[(c.get('role'),c.get('moved'),c.get('avatarMoved')) for c in n.get('casts') or []],'flags',[(f.get('label'),f.get('decisionDifferences')) for f in n.get('storyFlags') or []])" 2>/dev/null)"
done < "$PLAN"
