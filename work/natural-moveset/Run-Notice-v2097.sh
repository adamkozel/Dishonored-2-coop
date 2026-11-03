#!/usr/bin/env bash
# 2097 (#243): the co-op notice, second take. v2092a showed that a box opened in gameplay closes at once, unseen; the
# notice now waits for the pause menu or the main menu. In gameplay each role gets coop_notice_test (nothing may show),
# then presses Start (the box opens in the pause menu: frames step05 host, step12 vm), A (answers it) and Start again.
# Check-Notice-v2097.ps1 counts only each log's lines after the take began.
# a: candidate progress-v2097; b: the twin, nativechoice-v2088 (no notice), which must show none.
# Usage: Run-Notice-v2097.sh. Result: notice-eotw-v2097a, notice-eotw-v2097b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";cand="$2";shift 2
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 cp "../stage8/$cand/dinput8.dll" "../stage8/$cand/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "$take deploy failed";return; }
 hf=$(wc -l < ../lab-instances/host/engine.log)
 vf=$("$P" -NoProfile -Command ". ../../tools/vm/D2LabVM.Common.ps1;. ../../tools/vm/D2LabVM.Lab.ps1;Invoke-D2LabVM {\$p='C:\D2Lab\ws\work\lab-instances\client\engine.log';if(Test-Path \$p){@(Get-Content \$p).Count}else{0}}" 2>/dev/null | tail -1 | tr -d '\r')
 "$P" -NoProfile -File Route-MissionVM-v2089.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$cand" -StepsFile routes/notice-eotw-v2097.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? from host $hf vm $vf $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 "$P" -NoProfile -File Check-Notice-v2097.ps1 -Take "$take" -HostFrom "${hf:-0}" -VmFrom "${vf:-0}" "$@" 2>&1 | tail -c 1500
}
run notice-eotw-v2097a candidate-progress-v2097
run notice-eotw-v2097b candidate-nativechoice-v2088 -Twin
echo done
