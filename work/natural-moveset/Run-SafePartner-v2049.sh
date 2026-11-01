#!/usr/bin/env bash
# 2049 (#239): a partner opens a safe with the host's code. The Good Doctor from its start; the VM partner dials
# Vasco's safe (ld_combinaison_numbers_15/17/16) in its own world (harness Route-MissionVM-v2049 partnerdial), with host
# story, items per player and safe codes on. Candidate replaydecision-v2047d (the deployed one).
# Usage: Run-SafePartner-v2049.sh <letter>. Results: safe-partner-institute-v2049<letter>/
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
take="safe-partner-institute-v2049${1:-a}"
if [ -e "$take" ]; then echo "$take exists, skipped"; exit 0; fi
"$P" -NoProfile -File Route-MissionVM-v2049.ps1 -Take "$take" -MapInfo campaign/institute/institute.mapinfo -Candidate candidate-replaydecision-v2047d -StepsFile routes/safe-partner-institute-v2049.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('partnerDial'),n.get('worldDiff'))" 2>&1 | head -c 900)"
