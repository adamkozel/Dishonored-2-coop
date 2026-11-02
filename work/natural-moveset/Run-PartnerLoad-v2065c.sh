#!/usr/bin/env bash
# 2065c (#240): the partner's own load with the host connected. As hostload-dunwall-v2065a, but the VM partner sends
# RestartFromCheckpoint: it must become a request the host carries out for both worlds (diff after the reload required).
# Usage: Run-PartnerLoad-v2065c.sh. Result: hostload-dunwall-v2065c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
take=hostload-dunwall-v2065c
if [ -e "$take" ]; then echo "$take exists, skipped";exit 0;fi
"$P" -NoProfile -File Route-MissionVM-v2059.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-sharedload-v2065 -StepsFile routes/partnerload-dunwall-v2065.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-reload'),n.get('worldDiff'))" 2>&1 | head -c 900)"
echo done
