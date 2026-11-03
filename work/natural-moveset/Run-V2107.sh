#!/usr/bin/env bash
# 2107 (#265, #267): candidate progress-v2105 (v2104 plus exact join restores of ammunition, elixirs, runes, gear,
# whalebone and money) on harness Route-MissionVM-v2107.ps1, whose partner-loss check counts as many raised NPCs as the
# host's keep-awake releases at the kill as native LOD pauses (notes.kill.lodPaused), so no -PauseCheckNote: a real leak
# fails the take. Check-Progress-v2107.py judges (changed from fresh, equal after the rejoin).
# progress-eotw-v2107b: twin, coop_partner_progress off, must be RED.
# progress-eotw-v2107a: progression on, must PASS (bolts 3 -> 1 -> 1, extended_health 0 -> 1 -> 1).
# progress-eotw-v2107c: the #267 twin, keep-awake off on the host from the start: the docks worker must be paused before
# the kill (native LOD pause) and not rise at it; progression must PASS as in a.
# Usage: Run-V2107.sh. Result: progress-eotw-v2107b, v2107a, v2107c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2105
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2107-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2107.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" "$@" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));k=r.get('notes',{}).get('kill',{});print(r.get('passed'),r.get('failure'),k.get('keepAwakeReleased'),k.get('lodPaused'),k.get('pausedLonger'),k.get('pauseBefore',{}).get('770189828'),k.get('pauseAfter',{}).get('770189828'))" 2>&1 | head -c 800)"
}
run progress-eotw-v2107b routes/progress-eotw-twin-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8
python -I Check-Progress-v2107.py progress-eotw-v2107b --twin 2>&1 | tail -c 1500
run progress-eotw-v2107a routes/progress-eotw-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8
python -I Check-Progress-v2107.py progress-eotw-v2107a 2>&1 | tail -c 1500
run progress-eotw-v2107c routes/progress-eotw-keepawake-v2107.json -KillAfterStep 9 -RelaunchAfterStep 9
python -I Check-Progress-v2107.py progress-eotw-v2107c --steps 4,8,12 2>&1 | tail -c 1500
clean
echo done
