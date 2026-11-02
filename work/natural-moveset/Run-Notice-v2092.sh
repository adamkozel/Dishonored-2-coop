#!/usr/bin/env bash
# 2092 (#243): the on-screen co-op notice (version mismatch) on two accounts. In gameplay, the host and then the partner
# get coop_notice_test; each game must open the one-button notice box (step screenshots) and log COOP: notice shown once
# (Check-Notice-v2092.ps1).
# a: candidate notice-v2092; b: the twin, the previous candidate nativechoice-v2088 (no notice), which must show none.
# Usage: Run-Notice-v2092.sh. Result: notice-eotw-v2092a, notice-eotw-v2092b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";cand="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 cp "../stage8/$cand/dinput8.dll" "../stage8/$cand/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "$take deploy failed";return; }
 "$P" -NoProfile -File Route-MissionVM-v2089.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$cand" -StepsFile routes/notice-eotw-v2092.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 "$P" -NoProfile -File Check-Notice-v2092.ps1 -Take "$take" 2>&1 | tail -c 1500
}
run notice-eotw-v2092a candidate-notice-v2092
run notice-eotw-v2092b candidate-nativechoice-v2088
echo done
