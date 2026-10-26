#!/usr/bin/env bash
# 1931 (release 0.9.6): the retakes of Run-Campaign-v1929 (plan campaign-v1931-plan.txt); 1929: Run-Campaign-v1926 on candidate 1214kq (1214kp with the version resources 0.9.6), the release
# binary; v1926a was stopped in its first take for this. The nine missions on candidate 1214kq (power effects on any install, #230) with Route-MissionVM-v1918,
# on two accounts (host Corvo on this PC, VM partner Emily), product mode and co-op saves. Each mission's route is the one
# that passed on 1214km (v1916/v1917); after Dunwall every cast selects the power first and must move the caster in both
# worlds AND show its native effect in the other world (the Blink burst, the Far Reach beam). Plan campaign-v1926-plan.txt.
# Results: campaign-<mission>-vm-v1931<letter>/.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v1926-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1931$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1918.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214kq -StepsFile "$steps" -CoopSaves -StoryFlags > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),'casts',[(c.get('role'),c.get('moved'),c.get('avatarMoved')) for c in n.get('casts') or []],'flags',[(f.get('label'),f.get('decisionDifferences')) for f in n.get('storyFlags') or []])" 2>/dev/null)"
done < "$PLAN"
