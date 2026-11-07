#!/usr/bin/env bash
# 2163 (#260): the checkpoint reinforcements. deadlyshade-eotw-v2071b (candidate deadlyshade-v2070) is the only take of 83
# on Edge of the World, 137 diffs, where script_spawner_checkpoint_reinf_01 and _02 were alive in the host's world and
# absent in the partner's (after the partner was placed by the checkpoint). Its route, deadlyshade-eotw-off-v2071, runs
# three times on candidate progress-v2162 and harness v2160 (every diff required; a differing diff keeps both games'
# NPC status and engine logs).
# Usage: Run-V2163R.sh [a b c]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2162
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
want="${*:-a b c}"
for x in $want; do
 case $x in
  a) run reinf-eotw-v2163a routes/deadlyshade-eotw-off-v2071.json;;
  b) run reinf-eotw-v2163b routes/deadlyshade-eotw-off-v2071.json;;
  c) run reinf-eotw-v2163c routes/deadlyshade-eotw-off-v2071.json;;
 esac
done
clean
echo done
