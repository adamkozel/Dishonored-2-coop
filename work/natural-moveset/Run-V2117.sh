#!/usr/bin/env bash
# 2117 (#265): candidate progress-v2112 (crafted bonecharms carried) on harness v2111.
# Route progress-eotw-crafted-v2117: the v2110 charm route plus coop_lab_crafted_charm (a crafted charm, owned and
# equipped); --bonecharms probes at steps 4, 11 and 16; the partner is killed after step 12 and rejoins. Check-Crafted-v2117.py
# (both charms) and Check-Progress-v2107.py --steps 3,10,15 (items) judge. b is the twin (progression off): RED.
# Usage: Run-V2117.sh. Result: progress-eotw-v2117b, progress-eotw-v2117a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2112
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2117-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2111.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" "$@" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));k=r.get('notes',{}).get('kill',{});print(r.get('passed'),r.get('failure'),k.get('keepAwakeReleased'),k.get('lodPaused'),k.get('pausedLonger'),k.get('pauseBefore',{}).get('770189828'),k.get('pauseAfter',{}).get('770189828'))" 2>&1 | head -c 800)"
}
run progress-eotw-v2117b routes/progress-eotw-crafted-twin-v2117.json -KillAfterStep 12 -RelaunchAfterStep 12
python -I Check-Crafted-v2117.py progress-eotw-v2117b --twin 2>&1 | tail -c 900
python -I Check-Progress-v2107.py progress-eotw-v2117b --twin --steps 3,10,15 2>&1 | tail -c 700
run progress-eotw-v2117a routes/progress-eotw-crafted-v2117.json -KillAfterStep 12 -RelaunchAfterStep 12
python -I Check-Crafted-v2117.py progress-eotw-v2117a 2>&1 | tail -c 900
python -I Check-Progress-v2107.py progress-eotw-v2117a --steps 3,10,15 2>&1 | tail -c 700
grep -o '"crafted":{[^}]*}' progress-eotw-v2117a/step14-status-partner-progress.json-client.json 2>/dev/null
clean
echo done
