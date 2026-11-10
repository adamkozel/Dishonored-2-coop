#!/usr/bin/env bash
# 2245 (#281 follow-up): the Institute key door. institute-cure-partner-vm-v2245c and d ended with ld_door_glass_350cm_01_2_14
# and _1_24 open in the host world [2,2,0,0] and closed and locked in the partner world [0,0,1,0]. Route institute-keydoor-v2245
# is the cure route up to the host key door, then both worlds door-sync status and a diff, twice 30 s apart.
# Candidate progress-v2244. a Corvo host, Emily partner; b Emily host, Corvo partner. Usage: Run-V2245K.sh [a]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2244
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2245k-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
sleep 60
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt ../lab-instances/host/coop-status.json.coop-saves-stall.json
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";hc="$2";pc="$3";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2245.ps1 -Take "$take" -MapInfo campaign/institute/institute.mapinfo -Candidate "$CAND" -StepsFile routes/institute-keydoor-v2245.json -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 cp ../lab-instances/host/coop-status.json.coop-saves-stall.json "$take/host-coop-saves-stall.json" 2>/dev/null && echo "$take: host coop-saves stall stacks kept"
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3"; fi
}
for x in ${*:-a}; do
 case $x in
  a) run institute-keydoor-vm-v2245a corvo emily;;
 esac
done
clean
echo done
