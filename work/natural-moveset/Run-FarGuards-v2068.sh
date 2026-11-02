#!/usr/bin/env bash
# 2067 (#241): guards far from the host on two Steam accounts. Edge of the World: the host stays at the docks, the VM
# partner stands ~205 m inland (activation-range-v2001a's far point) with --npc-activity probes and required diffs, then
# kills addermirestation_entrance_guard_02 in its own world (harness Route-MissionVM-v2067 partnerattack) and the diff
# after must be equal. Candidate farguards-v2067 (keep-awake, eviction dwell).
# a: the take; b: the twin, coop_npc_keep_awake off on the host (its --npc-activity must show the host's guards near the
# partner paused again). 2068: harness v2068 (partnerattack finds the NPC by spawner name
# through --npc-activity; v2067a/b found none). Usage: Run-FarGuards-v2068.sh. Results: farguards-eotw-v2068a, -v2068b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2068.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate candidate-farguards-v2067 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-far'),n.get('diff-kill'),n.get('diff-end'))" 2>&1 | head -c 900)"
}
run farguards-eotw-v2068a routes/farguards-eotw-v2067.json
run farguards-eotw-v2068b routes/farguards-eotw-off-v2067.json
echo done
