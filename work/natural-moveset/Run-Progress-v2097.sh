#!/usr/bin/env bash
# 2096 (#265, step 5c): the partner keeps her progression. On Edge of the World the VM partner (Emily) is granted an
# enhancement (extended_health, kept after every load) and tranquilizer bolts (kept only after a join's load); after 9 s
# her capture has saved them. Then her game is closed (-KillAfterStep 8), started again and joins the same lobby from
# its main menu, so the host's mission loads fresh (-RelaunchAfterStep 8). Check-Progress-v2096.py compares her entries.
# b first: the twin, coop_partner_progress off from the start (nothing saved), which must lose both (run before a, so no
# file of a's is there). a: candidate progress-v2097 with the switch on.
# Usage: Run-Progress-v2097.sh. Result: progress-eotw-v2097b, progress-eotw-v2097a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2097
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > progress-eotw-v2097-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";steps="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2089.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -KillAfterStep 8 -RelaunchAfterStep 8 -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 python -I Check-Progress-v2096.py "$take" "$@" 2>&1 | tail -c 1500
}
python -I -c "
import json;r=json.load(open('routes/progress-eotw-v2096.json'));r[1]['text']='coop_partner_progress off'
json.dump(r,open('routes/progress-eotw-twin-v2096.json','w'),indent=1)"
run progress-eotw-v2097b routes/progress-eotw-twin-v2096.json --twin
run progress-eotw-v2097a routes/progress-eotw-v2096.json
echo done
