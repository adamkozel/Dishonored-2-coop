#!/usr/bin/env bash
# 2083 (#243): movement quality in numbers on two Steam accounts, Edge of the World. The host samples a native NPC
# (addermire_roof_03, walking under keep-awake near the far partner) as a baseline, then the partner's rig while she walks,
# runs and strafes; then the partner samples the host's rig while he does the same. Harness Route-MissionVM-v2083
# (animsample), candidate animsample-v2082. Usage: Run-AnimQuality-v2083.sh. Result: animquality-eotw-v2083a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
take=animquality-eotw-v2083a
if [ -e "$take" ]; then echo "$take exists, skipped";exit 0;fi
"$P" -NoProfile -File Route-MissionVM-v2083.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate candidate-animsample-v2082 -StepsFile routes/animquality-eotw-v2083.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if 'animsample' in k]" 2>&1 | head -c 2500)"
echo done
