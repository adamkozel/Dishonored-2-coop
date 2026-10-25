#!/usr/bin/env bash
# 1917 (goal criterion 2, powers): the five missions whose 0.9.5 takes record no power use, again on candidate 1214km
# with Route-MissionVM-v1917. The cast step first proves the caster can move, then retries the cast until the caster
# moved in its own world and its avatar in the other. The casts sit where both players have arrived on the mission's
# map (Edge of the World in Karnaca, the Royal Conservatory at the Conservatory, A Crack in the Slab in the manor and
# the Dust District, the Grand Palace in the palace streets); A Long Day in Dunwall records that no power is equipped
# before the Mark. Plan campaign-v1917-plan.txt. Results: campaign-<mission>-vm-v1917<letter>/.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v1917-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v1917$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v1917.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-npc-combat-v1214km -StepsFile "$steps" -CoopSaves -StoryFlags > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),'casts',[(c.get('role'),c.get('moved'),c.get('avatarMoved')) for c in n.get('casts') or []],'flags',[(f.get('label'),f.get('decisionDifferences')) for f in n.get('storyFlags') or []])" 2>/dev/null)"
done < "$PLAN"
