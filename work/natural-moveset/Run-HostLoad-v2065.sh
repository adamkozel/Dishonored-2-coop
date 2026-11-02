#!/usr/bin/env bash
# 2064 (#240): a host load with the partner connected. Both play Dunwall from the start (route campaign-dunwall-vm-v2041
# steps 0 and 7-12: the bedroom, Mayhew, Ramsey killed and looted), a diff is recorded, then the host loads its last
# checkpoint (console RestartFromCheckpoint, the pause menu's load) and the world diff after both reload is required.
# Candidate joinexpiry-v2063, harness v2059. a: the take; b: the twin, coop_checkpoint_sync off on the host before its load.
# 2065: candidate sharedload-v2065; the host opens Ramsey ring locker after the loot (a change to undo), and the twin
# switches coop_shared_load off on the host (v2064b: coop_checkpoint_sync off was re-armed by the product, both reloaded).
# Usage: Run-HostLoad-v2065.sh. Results: hostload-dunwall-v2065a, -v2065b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2059.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-sharedload-v2065 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-opened'),n.get('diff-reload'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run hostload-dunwall-v2065a routes/hostload-dunwall-v2065.json
run hostload-dunwall-v2065b routes/hostload-dunwall-off-v2065.json
echo done
