#!/usr/bin/env bash
# 2084 (#243): the partner's first-mission character choice answered by the DLL (no harness presses on her logic 14).
# a: partner Corvo (host Emily); b: partner Emily (host Corvo); c: the twin, coop_native_choice manual on the partner (her
# screen must stay unanswered: the harness fails after 25 s). The harness checks both worlds' characters at the end.
# Candidate nativechoice-v2084, harness Route-MissionVM-v2084. Usage: Run-NativeChoice-v2084.sh. Results: nativechoice-dunwall-v2084a/b/c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2084.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-nativechoice-v2084 -StepsFile routes/nativechoice-dunwall-v2084.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer "$@" > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('characters'),n.get('nativeChoiceManual'))" 2>&1 | head -c 700)"
}
run nativechoice-dunwall-v2084a -HostCharacter emily -HostChoice Right -PartnerCharacter corvo
run nativechoice-dunwall-v2084b -HostCharacter corvo -HostChoice Left -PartnerCharacter emily
run nativechoice-dunwall-v2084c -HostCharacter corvo -HostChoice Left -PartnerCharacter corvo -NativeChoiceManual
echo done
