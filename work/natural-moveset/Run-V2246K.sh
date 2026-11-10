#!/usr/bin/env bash
# 2246 (#281): candidate progress-v2246 unlocks linked door leaves (the Institute key door stayed locked in the partner world,
# institute-keydoor-vm-v2245a: native_unlock_refused). a: the key-door route on v2246, doors must be equal. t: its twin with
# coop_door_linked off in both worlds first (the leaves must stay locked there again). e (Corvo host) and f (Emily host): the
# whole partner cure, route institute-cure-partner-v2245e (v2245 plus the soak route boat steps: two clears, health hold at
# Meagan Foster; v2245c and d lost her at the boat). Harness Route-MissionVM-v2245. Usage: Run-V2246K.sh [a t e f]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2246
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2246k-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
sleep 60
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt ../lab-instances/host/coop-status.json.coop-saves-stall.json
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";hc="$2";pc="$3";route="$4";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2245.ps1 -Take "$take" -MapInfo campaign/institute/institute.mapinfo -Candidate "$CAND" -StepsFile "routes/$route.json" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 cp ../lab-instances/host/coop-status.json.coop-saves-stall.json "$take/host-coop-saves-stall.json" 2>/dev/null && echo "$take: host coop-saves stall stacks kept"
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3" "$4"; fi
}
for x in ${*:-a}; do
 case $x in
  a) run institute-keydoor-vm-v2246a corvo emily institute-keydoor-v2245;;
  t) run institute-keydoor-vm-v2246t corvo emily institute-keydoor-v2246t;;
  e) run institute-cure-partner-vm-v2246e corvo emily institute-cure-partner-v2245e;;
  f) run institute-cure-partner-vm-v2246f emily corvo institute-cure-partner-v2245e;;
 esac
done
clean
echo done
