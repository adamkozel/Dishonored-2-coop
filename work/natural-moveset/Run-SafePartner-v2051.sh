#!/usr/bin/env bash
# 2050 (#239): a partner opens a safe with the host's code, on candidate safedials-v2050 (the partner's dial turns are
# host requests, D2SD). a: Vasco's safe on The Good Doctor, both worlds must end equal (harness Route-MissionVM-v2050
# partnerdial waits for the host's lock). b: the mutation twin, coop_safe_dials off on the host (must be red).
# 2051: harness v2051 (each press sweeps five bearings at a stand; v2050a lost focus on dial _17) and coop_safe_dials
# allowed in Send-D2Command (v2050b was refused). Usage: Run-SafePartner-v2051.sh. Results: safe-partner-institute-v2051a, -v2051b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2051.ps1 -Take "$take" -MapInfo campaign/institute/institute.mapinfo -Candidate candidate-safedials-v2050 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('partnerDial'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run safe-partner-institute-v2051a routes/safe-partner-institute-v2050.json
run safe-partner-institute-v2051b routes/safe-partner-institute-off-v2050.json
echo done
