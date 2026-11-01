#!/usr/bin/env bash
# 2047 (#253): third round on candidate replaydecision-v2047d (a dispatched Replay also permits the memory continuation
# its LoadGame queues; replay-decision-vm-v2047d failed there). f: the host's Replay Mission on Dunwall, both worlds must
# end on the tower in gameplay. g: the Continue regression on the same candidate.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
tower=campaign/dunwall/escape/tower/dunwall_escape_tower_p
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2047.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-replaydecision-v2047d -StepsFile routes/campaign-dunwall-vm-v2041.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer "$@" > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('missionDecision'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run replay-decision-vm-v2047f -ReplayAtResults -ExpectReplayMap "$tower"
run continue-decision-vm-v2047g
echo done
