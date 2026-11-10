#!/usr/bin/env bash
# 2245 (#278): the partner leaves a corpse she possesses. Route corpse-leave-eotw-v2245 is corpse-possess-eotw-v2214 (the
# partner possesses the guard's body natively, v2214a) up to its forward press, then reads both worlds' entities and her
# possession state while inside, waits 60 s for the possession to end, reads them again, and has the host face her.
# The occupied corpse state (4, update 0x140825520) has no locomotion (confirmed static), so the body stays put; her avatar
# must reappear in the host's world where she leaves the body. Candidate progress-v2244, harness Route-MissionVM-v2245.
# Usage: Run-V2245L.sh [a]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2244
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2245l-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
sleep 60
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt ../lab-instances/host/coop-status.json.coop-saves-stall.json
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2245.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile routes/corpse-leave-eotw-v2245.json -HostCharacter corvo -HostChoice Left -PartnerCharacter corvo -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 cp ../lab-instances/host/coop-status.json.coop-saves-stall.json "$take/host-coop-saves-stall.json" 2>/dev/null && echo "$take: host coop-saves stall stacks kept"
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2"; fi
}
for x in ${*:-a}; do run corpse-leave-eotw-v2245$x; done
clean
echo done
