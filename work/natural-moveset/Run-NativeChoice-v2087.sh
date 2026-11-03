#!/usr/bin/env bash
# 2087 (#243): the Corvo partner's native choice again with harness v2087 (120 s for her chosen ending scene; her
# .native-choice.json kept at 5 s and at a timeout), then the manual twin under the same harness.
# Candidate nativechoice-v2085. Usage: Run-NativeChoice-v2087.sh. Results: nativechoice-dunwall-v2087a, -b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2087.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-nativechoice-v2085 -StepsFile routes/nativechoice-dunwall-v2084.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer "$@" > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('characters'),n.get('nativeChoiceManual'))" 2>&1 | head -c 700)"
}
run nativechoice-dunwall-v2087a -HostCharacter emily -HostChoice Right -PartnerCharacter corvo
run nativechoice-dunwall-v2087b -HostCharacter emily -HostChoice Right -PartnerCharacter corvo -NativeChoiceManual
echo done
