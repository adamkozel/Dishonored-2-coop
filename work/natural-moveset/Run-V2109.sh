#!/usr/bin/env bash
# 2109 (#260): candidate progress-v2107 (v2106 plus the host breakable presence: the partner breaks its breakables the
# host world lacks) on harness v2107. Edge of the World env_bottle_04_19 was absent on the host and whole on the partner
# in 2 of the last 5 progression takes (v2104b, v2107a); the end diff must be equal in all three takes, and the
# breakable-sync presence status shows what was sent, received and broken. Progression judged as in v2107.
# Usage: Run-V2109.sh. Result: progress-eotw-v2109a, b, c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2107
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2109-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
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
run progress-eotw-v2109a routes/progress-eotw-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8
python -I Check-Progress-v2107.py progress-eotw-v2109a 2>&1 | tail -c 700
head -4 progress-eotw-v2109a/diff-end.txt; for r in before after; do f=progress-eotw-v2109a/kill-$r-status/coop-status.json.breakable-sync.json; [ -f $f ] && echo "$r $(grep -o '"presence":{[^}]*}' $f)"; done
run progress-eotw-v2109b routes/progress-eotw-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8
python -I Check-Progress-v2107.py progress-eotw-v2109b 2>&1 | tail -c 700
head -4 progress-eotw-v2109b/diff-end.txt; for r in before after; do f=progress-eotw-v2109b/kill-$r-status/coop-status.json.breakable-sync.json; [ -f $f ] && echo "$r $(grep -o '"presence":{[^}]*}' $f)"; done
run progress-eotw-v2109c routes/progress-eotw-v2096.json -KillAfterStep 8 -RelaunchAfterStep 8
python -I Check-Progress-v2107.py progress-eotw-v2109c 2>&1 | tail -c 700
head -4 progress-eotw-v2109c/diff-end.txt; for r in before after; do f=progress-eotw-v2109c/kill-$r-status/coop-status.json.breakable-sync.json; [ -f $f ] && echo "$r $(grep -o '"presence":{[^}]*}' $f)"; done
clean
echo done
