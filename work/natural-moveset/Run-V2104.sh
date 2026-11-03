#!/usr/bin/env bash
# 2104: candidate progress-v2103 on harness Route-MissionVM-v2105.ps1 (v2105 adds host status snapshots at the kill for
# #267; v2104 is v2101 with the paused-NPC loop fixed: it overwrote
# the host snapshot, so v2101a/b failed "The host did not move ( m)"). The pause is #267; -PauseCheckNote records it.
# progress-eotw-v2104b (twin, coop_partner_progress off) and v2104a (on): the progression takes with -PauseCheckNote, so
# the pause is recorded (notes.pauseCheckNoted) and the take goes on to the relaunch; Check-Progress-v2096.py judges.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2103
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2104-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2105.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" "$@" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'),r.get('notes',{}).get('pauseCheckNoted'))" 2>&1 | head -c 600)"
}
run progress-eotw-v2104b routes/progress-eotw-twin-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8 -PauseCheckNote
python -I Check-Progress-v2096.py progress-eotw-v2104b --twin 2>&1 | tail -c 1500
run progress-eotw-v2104a routes/progress-eotw-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8 -PauseCheckNote
python -I Check-Progress-v2096.py progress-eotw-v2104a 2>&1 | tail -c 1500
clean
echo done
