#!/usr/bin/env bash
# 2110 (#265): candidate progress-v2108 (v2107 plus found bonecharms in partner progression) on harness v2107.
# Route progress-eotw-charm-v2110: the progression route plus coop_grant_charm 13858082 (owned and equipped) and
# --bonecharms probes at steps 4, 10 and 15; the partner is killed after step 11 and rejoins. Check-Charms-v2110.py
# (charm) and Check-Progress-v2107.py --steps 3,9,14 (items) judge. b is the twin (progression off): RED.
# Usage: Run-V2110.sh. Result: progress-eotw-v2110b, progress-eotw-v2110a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2108
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2110-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
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
run progress-eotw-v2110b routes/progress-eotw-charm-twin-v2110.json -KillAfterStep 11 -RelaunchAfterStep 11
python -I Check-Charms-v2110.py progress-eotw-v2110b --twin 2>&1 | tail -c 900
python -I Check-Progress-v2107.py progress-eotw-v2110b --twin --steps 3,9,14 2>&1 | tail -c 700
run progress-eotw-v2110a routes/progress-eotw-charm-v2110.json -KillAfterStep 11 -RelaunchAfterStep 11
python -I Check-Charms-v2110.py progress-eotw-v2110a 2>&1 | tail -c 900
python -I Check-Progress-v2107.py progress-eotw-v2110a --steps 3,9,14 2>&1 | tail -c 700
grep -o '"charms":{[^}]*}' progress-eotw-v2110a/step13-status-partner-progress.json-client.json 2>/dev/null
clean
echo done
