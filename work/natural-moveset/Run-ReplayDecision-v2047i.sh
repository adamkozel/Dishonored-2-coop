#!/usr/bin/env bash
# 2047i (#253): the Continue regression on candidate replaydecision-v2047d again; continue-decision-vm-v2047h was cut off
# at step 3 by a PC shutdown (2026-11-01) and is kept as interrupted evidence.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
tower=campaign/dunwall/escape/tower/dunwall_escape_tower_p
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2047.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-replaydecision-v2047d -StepsFile routes/campaign-dunwall-vm-v2041.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer "$@" > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('missionDecision'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run continue-decision-vm-v2047i
echo done
