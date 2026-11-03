#!/usr/bin/env bash
# 2113 (#260): a forced test of the host's breakable presence (progress-v2110: the v2107 presence, generated names left out). The bottle of #260 broke in the
# host's load in 1 of 2 takes only, and in none of the v2109 takes. Here, with breakable sync off in both games, the
# host breaks env_bottle_04_19 itself (harness v2109 breakhost), so no break message reaches the partner; sync is then
# turned back on (its catalog starts empty, as after a load). Within 45 s the partner must break her copy from the
# host's presence list: the later diff equal, her presence status broken >= 1. The twin on progress-v2106 (no
# presence) must keep the difference (host absent, partner 0).
# Usage: Run-V2113.sh
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";cand="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 cp "../stage8/$cand/dinput8.dll" "../stage8/$cand/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed";return; }
 "$P" -NoProfile -File Route-MissionVM-v2109.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$cand" -StepsFile routes/bottlepresence-eotw-v2113.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('breakTarget'),n.get('diff-spawn'),n.get('diff-later'))" 2>&1 | head -c 900)"
 [ -f "$take/diff-later.txt" ] && head -5 "$take/diff-later.txt"
 for f in "$take"/*breakable-sync-*.json; do [ -f "$f" ] && echo "$f $(grep -o '"presence":{[^}]*}' "$f")"; done
}
run bottlepresence-eotw-v2113a candidate-progress-v2110
run bottlepresence-eotw-v2113b candidate-progress-v2106
echo done
