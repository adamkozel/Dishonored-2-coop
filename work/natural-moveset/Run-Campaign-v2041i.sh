#!/usr/bin/env bash
# 2041 (#237, #238, #239): reruns after campaign v2040a on candidate gameover-v2041 (the partner leaves GameOver to the
# host under host story; v2040a madinventor ended on the partner's own "Sokolov dead" GameOver) with harness
# Route-MissionVM-v2017 (a screenshot grab that has not finished after 15 s is killed, and a minimized host window is
# restored first; v2040a dunwall waited 1 h 45 min on one grab). Host story, items per player and safe codes on.
# 2041b: candidate gameover-v2041b (active missions reach the partner through the gameflow mirror; co-op saves
# service gap in the status). Takes campaign-<mission>-vm-ipp-v2041b<letter>/.
# 2041c: harness v2018 (a replay prompt that opens from the result screen is answered No; bunker v2041a replayed
# the mission), Dunwall route v2041 (Ramsey from behind), candidate playerdamage-v2041d (player damage stays in its own world). Takes campaign-<mission>-vm-ipp-v2041c<letter>/.
# 2041e: candidate damagetarget-v2041e (every ActionDamage records the entity its target resolves to; madinventor v2041ca: the
# host died and the partner lost Sokolov right after the streets cable car damage). Takes campaign-<mission>-vm-ipp-v2041e<letter>/.
# 2041f: candidate charbranch-v2041f (the host refuses the partner's other-character branch requests) and harness v2019
# (failure also saves .reset.json death counters). Takes campaign-<mission>-vm-ipp-v2041f<letter>/.
# 2041g: candidate gameoverstack-v2041g (GameOver switches record their stack) and harness v2020 (failure also saves
# .loading.json). Takes campaign-<mission>-vm-ipp-v2041g<letter>/.
# 2041h: candidate gameoverupdate-v2041h (the partner's ActionGameOver update never fires under host story).
# Takes campaign-<mission>-vm-ipp-v2041h<letter>/.
# 2041i: harness v2021 (failure also saves the story NPCs, players and avatars of both games), candidate unchanged.
# Takes campaign-<mission>-vm-ipp-v2041i<letter>/.
# Usage: Run-Campaign-v2041i.sh <letter> <plan>. Results: campaign-<mission>-vm-ipp-v2041i<letter>/
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v2041i-plan.txt}"
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-ipp-v2041i$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-MissionVM-v2021.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate candidate-gameoverupdate-v2041h -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('hostStory'),n.get('worldDiff'),n.get('frameTimeouts'))" 2>&1 | head -c 600)"
done < "$PLAN"
