#!/usr/bin/env bash
# 2226: the hour session on the release line, candidate progress-v2226 (soak-hour-vm-v2211h passed on v2211).
# 2211: the hour session again on candidate progress-v2211 (the reflow, own-only NPCs and story ints); soak-hour-vm-v2208h was
# interrupted by a PC shutdown at step 33, equal after Dunwall.
# 2208: candidate progress-v2208 (the story reflow; soak-hour-vm-v2206h differed after Edge of the World on The Good Doctor's elevator buttons).
# 2206: candidate progress-v2205, route soak-hour-vm-v2206: Dunwall, Edge of the World and The Good Doctor, then 13 minutes on
# the Dreadful Wale (a walk and a minute twelve times, a diff every four; soak-hour-vm-v2203h passed three missions in 52 minutes, every
# diff equal, then stopped at the boat talk to Meagan after the natural transition).
# 2203: harness v2185 (a talk step tries three passes; soak-hour-vm-v2200h stopped at the Dunwall end talk after 14 minutes).
# 2200: progress-v2198, harness v2184, route soak-hour-vm-v2199 (the talk to Meagan at The Good Doctor end holds health:
# soak-hour-vm-v2197h passed Dunwall and Edge of the World with equal diffs, then the host died there after 44 minutes).
# 2186 (#244, step 6): one session of an hour or more on two Steam accounts with zero differences. Emily hosts on this
# PC, Corvo is the VM partner. They join once and play four missions in a row without leaving: A Long Day in Dunwall,
# Edge of the World, The Good Doctor and The Clockwork Mansion (route soak-hour-vm-v2186, the four campaign routes
# chained; each one ends with both players in the next mission). After each mission, a world diff is required, and so
# is the final diff. Candidate progress-v2180, harness Route-MissionVM-v2182, host story, items per player, co-op saves.
# 2186b: -HostChoice Right (soak-hour-vm-v2186a: the host native choice took Corvo after 9 minutes).
# Usage: Run-V2186H.sh [letter]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
# 2190: on candidate progress-v2190 (every finished mission state reaches a following partner).
CAND=candidate-progress-v2226
take="soak-hour-vm-v2226$L"
if [ -e "$take" ]; then echo "$take exists, skipped"; echo done; exit 0; fi
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
"$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
start=$(date +%s)
"$P" -NoProfile -File Route-MissionVM-v2185.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile routes/soak-hour-vm-v2206.json -HostCharacter emily -HostChoice Right -PartnerCharacter corvo -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
rc=$?
echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('worldDiff'),[ (k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
echo done
