#!/usr/bin/env bash
# 2086 (#243): movement quality with the mover in the observer's view (animquality-eotw-v2085a: a rig 200 m from the
# observer is not animated, FROZEN). Dunwall start, the tower bedroom: host and partner 4.7 m apart facing each other;
# the partner walks, runs and strafes while the host samples her rig, then the reverse. Then the native choice take for a
# Corvo partner again (nativechoice-dunwall-v2085a stopped on co-op saves). Harness v2086, candidate nativechoice-v2085.
# Usage: Run-AnimQuality-v2086.sh. Results: animquality-dunwall-v2086a, nativechoice-dunwall-v2086a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
take=animquality-dunwall-v2086a
if [ ! -e "$take" ]; then
 "$P" -NoProfile -File Route-MissionVM-v2086.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-nativechoice-v2085 -StepsFile routes/animquality-dunwall-v2086.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if 'animsample' in k]" 2>&1 | head -c 2500)"
fi
take=nativechoice-dunwall-v2086a
if [ ! -e "$take" ]; then
 "$P" -NoProfile -File Route-MissionVM-v2086.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-nativechoice-v2085 -StepsFile routes/nativechoice-dunwall-v2084.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer -HostCharacter emily -HostChoice Right -PartnerCharacter corvo > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('characters'))" 2>&1 | head -c 700)"
fi
echo done
