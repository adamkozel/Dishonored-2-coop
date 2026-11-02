#!/usr/bin/env bash
# 2088 (#243): movement quality in view with both players on free floor (animquality-dunwall-v2086a put the partner on
# furniture, where her own position wobbled 4-7 cm). Host at the bedroom trigger, partner at her spawn, 13 m apart facing
# each other. Harness v2088 adds a fixture guard (the mover rests within 5 mm before sampling).
# a: the take; b: the twin, the partner back on the v2086a furniture spot (the guard must stop it as FIXTURE).
# 2090: the partner 2.5 m from the host on the in-view line of v2086a (v2089a: a wall between them, both rigs FROZEN);
# moves mostly sideways. 2089: harness v2089 (the guard keeps each position as one array), candidate nativechoice-v2088.
# Usage: Run-AnimQuality-v2089.sh. Result: animquality-dunwall-v2090a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2089.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-nativechoice-v2088 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if 'animsample' in k]" 2>&1 | head -c 2500)"
}
run animquality-dunwall-v2090a routes/animquality-dunwall-v2090.json

echo done
