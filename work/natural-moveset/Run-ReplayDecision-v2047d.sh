#!/usr/bin/env bash
# 2047 (#253): second round on candidate replaydecision-v2047c (the Replay confirmation box counts as the result screen
# after a shared end; replay-decision-vm-v2047a failed there). d: the host's Replay Mission on Dunwall, both worlds must
# end on the tower in gameplay. e: the mutation twin, coop_mission_decision off in the host's game; must be red.
# Then 2048 (#254): mission-partner-kill-v2048a, the partner's lethal takedown of the host's checkpoint guard on Edge of
# the World under host story; the guard must die in both worlds.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
tower=campaign/dunwall/escape/tower/dunwall_escape_tower_p
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2047.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-replaydecision-v2047c -StepsFile routes/campaign-dunwall-vm-v2041.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer "$@" > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('missionDecision'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run replay-decision-vm-v2047d -ReplayAtResults -ExpectReplayMap "$tower"
run replay-decision-vm-v2047e -ReplayAtResults -ExpectReplayMap "$tower" -DecisionOff
k=mission-partner-kill-v2048a
if [ -e "$k" ]; then echo "$k exists, skipped"; else
 "$P" -NoProfile -File Test-MissionPartnerPowers-v2048.ps1 -Take "$k" -Candidate candidate-replaydecision-v2047c -PartnerCharacter emily -Powers kill -HostStory > "$k.log" 2>&1
 echo "$k exit $? $(python -c "import json;r=json.load(open('$k/result.json',encoding='utf-8-sig'));w=r.get('rows',[{}])[0];print(r.get('passed'),r.get('failure'),w.get('outcome'),w.get('definitions'),w.get('life'),w.get('checks'))" 2>&1 | head -c 1200)"
fi
echo done
