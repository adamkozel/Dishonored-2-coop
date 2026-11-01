#!/usr/bin/env bash
# 2047 (#253): the host decides at a shared mission end (candidate replaydecision-v2047b, host story and items per player
# on). a: the host chooses Replay Mission on Dunwall's result screen; both worlds must end on the tower in gameplay.
# b: the mutation twin, coop_mission_decision off in the host's game (each world chooses again, as in v2044a); must be red.
# c: the Continue regression; both worlds must follow the host's Continue to the next mission.
# Usage: Run-ReplayDecision-v2047.sh. Results: replay-decision-vm-v2047a, -v2047b, continue-decision-vm-v2047c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
tower=campaign/dunwall/escape/tower/dunwall_escape_tower_p
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2047.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-replaydecision-v2047b -StepsFile routes/campaign-dunwall-vm-v2041.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer "$@" > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('missionDecision'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run replay-decision-vm-v2047a -ReplayAtResults -ExpectReplayMap "$tower"
run replay-decision-vm-v2047b -ReplayAtResults -ExpectReplayMap "$tower" -DecisionOff
run continue-decision-vm-v2047c
echo done
