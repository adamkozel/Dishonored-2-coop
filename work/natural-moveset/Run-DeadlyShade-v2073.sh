#!/usr/bin/env bash
# 2071 (#242): the partner's Deadly Shade on two Steam accounts, Edge of the World (the local shade harness no longer
# reaches a cast: partner-shade-v2069a, v2070a/c). The VM partner (Emily) gets Doppelganger with Deadly Shade and casts
# at checkpoint guard 01 from v1833a's front stand; the host's .shade-host.json is kept right after and 12 s later, and
# the diffs after and 30 s later are required. Candidate deadlyshade-v2070, harness Route-MissionVM-v2071 (status step).
# a: the take; b: the twin, coop_shade_native deadly_off on the host (no host puppet).
# 2072: Dunwall at the start, from the stand of partner-shade-v1736b toward guard 1 (v2071a: the Edge of the World stand
# was a ledge; she slid off and cast into the wall). v2072c/d: the host health hold on (v2072a/b: the host died in the waits).
# 2073: she casts from her spawn toward the NPC 9.5 m away (v2072a: the v1736b stand is not
# streamed at the Dunwall start, she fell to z -515). Usage: Run-DeadlyShade-v2073.sh. Results: deadlyshade-dunwall-v2073a, -b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2071.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-deadlyshade-v2070 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('casts'),n.get('diff-after'),n.get('diff-end'))" 2>&1 | head -c 900)"
}
run deadlyshade-dunwall-v2073a routes/deadlyshade-dunwall-v2073.json
run deadlyshade-dunwall-v2073b routes/deadlyshade-dunwall-off-v2073.json
echo done
