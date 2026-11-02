#!/usr/bin/env bash
# 2053 (#239): a partner solves the Jindosh lock under the host. Dust District to the lock (route campaign-dust-vm-v1931
# steps 0-11), then the VM partner turns the wheels in its own world (harness Route-MissionVM-v2053 partnerjindosh,
# read back from the host's lock), a required world diff on the Dust map, candidate jindoshwheels-v2053.
# a: the take; b: the mutation twin, coop_safe_dials off on the host (must be red).
# Usage: Run-JindoshPartner-v2053.sh. Results: jindosh-partner-dust-v2053a, -v2053b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2053.ps1 -Take "$take" -MapInfo campaign/boat/dust/boat_dust.mapinfo -Candidate candidate-jindoshwheels-v2053 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('partnerJindosh'),n.get('diff-jindosh'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run jindosh-partner-dust-v2053a routes/jindosh-partner-dust-v2053.json
run jindosh-partner-dust-v2053b routes/jindosh-partner-dust-off-v2053.json
echo done
