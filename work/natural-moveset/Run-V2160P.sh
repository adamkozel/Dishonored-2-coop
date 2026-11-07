#!/usr/bin/env bash
# 2158 (#265): partner progression per character, on candidate progress-v2151 and harness v2158 (the route's rejoin
# step kills the VM game, starts it again and joins as the named character). Route progress-eotw-perchar-v2158: Emily
# earns extended_health and charm 13858082, rejoins as Corvo (must start without them), Corvo earns tranquilizer bolts
# and charm 3997576305, rejoins as Emily (her set back, not his), then as Corvo (his set back, not hers).
# Check-PerChar-v2158.py judges them. The twin (coop_partner_progress off) must lose both sets.
# 2159: progress-eotw-v2158b rejoined as Emily, not Corvo: a lab game asks for the character on a join only with
# coop_character_choice on. Harness v2159 turns it on for a route rejoin.
# 2160: progress-eotw-v2159a PASSED per character (Corvo started without Emily's set and each got only its own back),
# but its end diff, like the twin's, had script_spawner_bfbuilding_exterminato_1 dead only in the host's world.
# Candidate progress-v2160 sends a known NPC death again after its body left the host's catalog. Harness v2160 keeps
# the twin's progression off across its relaunches (route progress-eotw-perchar-twin-v2160).
# Usage: Run-V2160P.sh [b a]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2160
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 clean
 "$P" -NoProfile -File Route-MissionVM-v2160.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -PauseCheckNote "$@" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.endswith('-character') or k.startswith('diff-')]" 2>&1 | head -c 1500)"
}
want="${*:-b a}"
for x in $want; do
 case $x in
  b) run progress-eotw-v2160b routes/progress-eotw-perchar-twin-v2160.json; python -I Check-PerChar-v2158.py progress-eotw-v2160b --twin 2>&1 | tail -c 1200;;
  a) run progress-eotw-v2160a routes/progress-eotw-perchar-v2158.json; python -I Check-PerChar-v2158.py progress-eotw-v2160a 2>&1 | tail -c 1200;;
 esac
done
clean
echo done
