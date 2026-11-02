#!/usr/bin/env bash
# 2055 (#240): measure a mid-mission join on the current candidate before changing it. The host plays Dunwall alone
# (route campaign-dunwall-vm-v2041 steps 0 and 7-12: the bedroom, Mayhew, Ramsey killed and looted), then the VM partner
# joins from its main menu (-JoinAfterStep 7), and world diffs run at the join and 60 s later (recorded, not required).
# Usage: Run-JoinMeasure-v2055.sh. Result: join-measure-dunwall-v2055a
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
take=join-measure-dunwall-v2055a
if [ -e "$take" ]; then echo "$take exists, skipped";exit 0;fi
"$P" -NoProfile -File Route-MissionVM-v2054.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-jindoshwheels-v2053 -StepsFile routes/join-measure-dunwall-v2055.json -JoinAfterStep 7 -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-join'),n.get('diff-join60'),n.get('worldDiff'))" 2>&1 | head -c 900)"
echo done
