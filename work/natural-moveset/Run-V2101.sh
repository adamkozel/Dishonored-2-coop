#!/usr/bin/env bash
# 2101: candidate progress-v2102 (v2100 plus the #263 co-op saves trace) on harness Route-MissionVM-v2101.ps1.
# m3control-eotw-v2101a (inventory row M3): the progression route's first nine steps with nothing killed, then the docks
# worker 770189828 (worker_male_small_01, whose AI pause count rose 0 to 1 and stayed after the partner's game was
# killed in progress-eotw-v2097a/b and v2098b) is probed at the kill's moments: at once, +15 s and +75 s. A count that
# rises here too is the game's own; one that stays 0 points at our partner-loss cleanup.
# progress-eotw-v2101b (twin, coop_partner_progress off) and v2101a (on): the progression takes with -PauseCheckNote, so
# the pause is recorded (notes.pauseCheckNoted) and the take goes on to the relaunch; Check-Progress-v2096.py judges.
# Usage: Run-V2101.sh. Result: m3control-eotw-v2101a, progress-eotw-v2101b, progress-eotw-v2101a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2102
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2101-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2101.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" "$@" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'),r.get('notes',{}).get('pauseCheckNoted'))" 2>&1 | head -c 600)"
}
run m3control-eotw-v2101a routes/m3control-eotw-v2101.json
for f in m3control-eotw-v2101a/*ai-decision*host.json; do [ -f "$f" ] && echo "$f $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));print(d.get('pauseReferences'),d.get('pauseState'))")"; done
run progress-eotw-v2101b routes/progress-eotw-twin-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8 -PauseCheckNote
python -I Check-Progress-v2096.py progress-eotw-v2101b --twin 2>&1 | tail -c 1500
run progress-eotw-v2101a routes/progress-eotw-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8 -PauseCheckNote
python -I Check-Progress-v2096.py progress-eotw-v2101a 2>&1 | tail -c 1500
clean
echo done
