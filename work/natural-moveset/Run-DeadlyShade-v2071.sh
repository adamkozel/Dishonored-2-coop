#!/usr/bin/env bash
# 2071 (#242): the partner's Deadly Shade on two Steam accounts, Edge of the World (the local shade harness no longer
# reaches a cast: partner-shade-v2069a, v2070a/c). The VM partner (Emily) gets Doppelganger with Deadly Shade and casts
# at checkpoint guard 01 from v1833a's front stand; the host's .shade-host.json is kept right after and 12 s later, and
# the diffs after and 30 s later are required. Candidate deadlyshade-v2070, harness Route-MissionVM-v2071 (status step).
# a: the take; b: the twin, coop_shade_native deadly_off on the host (no host puppet).
# Usage: Run-DeadlyShade-v2071.sh. Results: deadlyshade-eotw-v2071a, -v2071b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2071.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate candidate-deadlyshade-v2070 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('casts'),n.get('diff-after'),n.get('diff-end'))" 2>&1 | head -c 900)"
}
run deadlyshade-eotw-v2071a routes/deadlyshade-eotw-v2071.json
run deadlyshade-eotw-v2071b routes/deadlyshade-eotw-off-v2071.json
echo done
