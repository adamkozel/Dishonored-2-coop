#!/usr/bin/env bash
# 2163 (#260): the checkpoint reinforcements. deadlyshade-eotw-v2071b (candidate deadlyshade-v2070) is the only take of 83
# on Edge of the World, 137 diffs, where script_spawner_checkpoint_reinf_01 and _02 were alive in the host's world and
# absent in the partner's (after the partner was placed by the checkpoint). Its route, deadlyshade-eotw-off-v2071, runs
# three times on candidate progress-v2162 and harness v2160 (every diff required; a differing diff keeps both games'
# NPC status and engine logs).
# 2164 (#260): reinf-eotw-v2163a reproduced it on progress-v2162: both reinforcements were absent in both worlds at the
# after diff, then alive in the host's world and absent in the partner's at the end, with no script node in either.
# Candidate progress-v2163 has the host announce each living NPC (present) and the partner spawn a missing one from
# its spawner. a and b run the route; t is the twin with coop_npc_present off in the partner's game (must differ).
# 2165 (#260): reinf-eotw-v2164a was equal; v2164t stopped in setup (coop_npc_present was not in Send-D2Command's
# list). The routes now keep both games' .npc-stream.json before the end diff (lifePresentSent/Received, lifeSpawned,
# lifeRefusedLast). a and c run the route, t is the twin.
# 2166 (#260): the event is rare (1 of 3 takes on v2162, none in v2164a, v2165a, t). Candidate progress-v2166 adds
# coop_lab_spawn <spawner> (a host-only spawn on demand) and the probe's --spawners list. l lists both worlds' spawners on
# Edge of the World, to pick one with no live NPC.
# Usage: Run-V2166S.sh [l]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2166
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 clean
 "$P" -NoProfile -File Route-MissionVM-v2160.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('casts'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 1200)"
 for f in "$take"/diff-*.txt; do [ -f "$f" ] && grep -q DIFFERENT "$f" && { echo "$(basename "$f"):"; head -8 "$f"; }; done
}
want="${*:-l}"
for x in $want; do
 case $x in
  l) run spawners-eotw-v2166l routes/spawners-eotw-list-v2166.json;;
 esac
done
clean
echo done
