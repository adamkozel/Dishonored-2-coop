#!/usr/bin/env bash
# 2071 (#242): the partner's Deadly Shade on two Steam accounts, Edge of the World (the local shade harness no longer
# reaches a cast: partner-shade-v2069a, v2070a/c). The VM partner (Emily) gets Doppelganger with Deadly Shade and casts
# at checkpoint guard 01 from v1833a's front stand; the host's .shade-host.json is kept right after and 12 s later, and
# the diffs after and 30 s later are required. Candidate deadlyshade-v2070, harness Route-MissionVM-v2071 (status step).
# a: the take; b: the twin, coop_shade_native deadly_off on the host (no host puppet).
# 2074: harness v2074 partnerface puts her 6 m behind entrance_guard_02 facing it (v2071a: a ledge; v2072/v2073:
# Dunwall has no Mark yet, so no power can be cast). 2075: candidate deadlyshade-v2075 (nearest NPC as parent).
# 2076: candidate deadlyshade-v2076 (the host player as parent).
# v2076c/d: the spawn diff recorded, not required (v2076a/b stopped on env_bottle_04_19,
# absent on the host at spawn, issue 260). 2077: candidate deadlyshade-v2077; diffs recorded, not required (env_bottle_04_19, issue 260;
# the diff cannot see shades), judged on .shade-host.json and no crash. 2079: candidate shadedecoy-v2079 (the decoy puppet for her deadly shadow on a host without a
# clone definition; v2077a and v2078a lost the host at the cast). Usage: Run-DeadlyShade-v2079.sh. Results: deadlyshade-eotw-v2079a, -b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2074.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate candidate-shadedecoy-v2079 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('casts'),n.get('diff-after'),n.get('diff-end'))" 2>&1 | head -c 900)"
}
run deadlyshade-eotw-v2079a routes/deadlyshade-eotw-v2077.json
run deadlyshade-eotw-v2079b routes/deadlyshade-eotw-off-v2077.json
echo done
