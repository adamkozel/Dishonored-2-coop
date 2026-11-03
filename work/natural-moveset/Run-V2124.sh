#!/usr/bin/env bash
# 2124 (#260): the forced presence test on progress-v2124 and harness v2111, take a only. bottlepresence-eotw-v2118a
# (progress-v2113) logged why presence broke nothing: missing 1, targets 0, the bottle first seen 1 to 2 s before each
# list, because the maintenance round's repeated coop_breakable_sync on cleared the catalog about every 12 s. v2124
# keeps the catalog on a repeated request. Earlier:
# bottlepresence-eotw-v2115a broke nothing (twin b RED as required). The partner status is read 20 s and 45 s in. Here the
# partner's breakable sync is turned off, the host breaks env_bottle_04_19 through the native step advance with no
# break message (as a bottle broken during the host's load), and the partner's sync is turned back on with an empty
# catalog. Within 45 s her presence must break her copy: the later diff equal, presence.broken >= 1. The twin keeps
# the partner's sync off, so nothing can fix it: the later diff must show the bottle (host absent or broken, partner 0).
# Usage: Run-V2124.sh
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2124
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2124-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2111.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-spawn'),n.get('diff-later'))" 2>&1 | head -c 900)"
 [ -f "$take/diff-later.txt" ] && head -5 "$take/diff-later.txt"
 for f in "$take"/*breakable-sync.json-*.json; do [ -f "$f" ] && echo "$f $(grep -o '"presence":{[^}]*},"labBreak":"[a-z_]*"' "$f")"; done
}
run bottlepresence-eotw-v2124a routes/bottlepresence-eotw-v2118.json
echo done
