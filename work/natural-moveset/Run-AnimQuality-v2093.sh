#!/usr/bin/env bash
# 2088 (#243): movement quality in view with both players on free floor (animquality-dunwall-v2086a put the partner on
# furniture, where her own position wobbled 4-7 cm). Host at the bedroom trigger, partner at her spawn, 13 m apart facing
# each other. Harness v2088 adds a fixture guard (the mover rests within 5 mm before sampling).
# a: the take; b: the twin, the partner back on the v2086a furniture spot (the guard must stop it as FIXTURE).
# 2091: Edge of the World docks walkway, floor spots players walked in earlier takes: host (-31.2,182.1), partner
# (-31.2,177.5), 4.6 m apart facing each other; moves along the walkway (v2090a: the tower spot was furniture again).
# 2090: the partner 2.5 m from the host on the in-view line of v2086a (v2089a: a wall between them, both rigs FROZEN);
# moves mostly sideways. 2089: harness v2089 (the guard keeps each position as one array), candidate nativechoice-v2088.
# 2093: the native baseline. The host on the docks walkway samples five native civilians within 4-17 m (positions from
# farguards-eotw-v2068b) while they go about their routines; the partner stands behind the host. Whichever walk give the
# native foot-skate numbers the limits are calibrated from (partner-quality-plan.md). Result: animquality-baseline-eotw-v2093a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2089.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate candidate-nativechoice-v2088 -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if 'animsample' in k]" 2>&1 | head -c 2500)"
}
run animquality-baseline-eotw-v2093a routes/animquality-baseline-eotw-v2093.json

echo done
