#!/usr/bin/env bash
# 1915 (release 0.9.5): the nine missions on two Steam accounts (host Corvo on this PC, VM partner Emily), product mode
# and co-op saves, with the 0.9.5 binary (candidate 1214kl), one take per mission in order, on Route-MissionVM-v1915.
# New against 1906: both players cast their power near the start of every mission from The Good Doctor on (the cast
# step records each caster's displacement in its own world and of its avatar in the other), and -StoryFlags diffs both
# worlds' decision flags at every mission's start and end (the story flag mirror of #220). The Grand Palace runs on Easy
# through the difficulty sync. The take letter is the first argument (default a) and an optional second argument names
# another plan file. Results: campaign-<mission>-vm-v1915<letter>/.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v1915-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1915$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1915.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214kl -StepsFile "$steps" -CoopSaves -StoryFlags > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),'casts',[(c.get('role'),c.get('moved'),c.get('avatarMoved')) for c in n.get('casts') or []],'flags',[(f.get('label'),f.get('decisionDifferences')) for f in n.get('storyFlags') or []])" 2>/dev/null)"
done < "$PLAN"
