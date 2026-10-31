#!/usr/bin/env bash
# 2044 (#253): the host chooses Replay Mission on its first co-op result screen (harness Route-MissionVM-v2022
# -ReplayAtResults: X on Results, then Yes). Records the native Replay load (kind and slot in the failure
# .checkpoint.json) and what each world does after it. Candidate loadmap-v2041m, host story and items per player on.
# Usage: Run-Replay-v2044.sh <letter>. Results: replay-dunwall-vm-v2044<letter>/
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
take="replay-dunwall-vm-v2044$L"
if [ -e "$take" ]; then echo "$take exists, skipped"; exit 0; fi
"$P" -NoProfile -File Route-MissionVM-v2022.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-loadmap-v2041m -StepsFile routes/campaign-dunwall-vm-v2041.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer -ReplayAtResults > "$take.log" 2>&1
echo "exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('pairing'))" 2>&1 | head -c 600)"
