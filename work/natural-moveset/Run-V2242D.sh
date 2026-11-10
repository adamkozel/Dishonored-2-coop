#!/usr/bin/env bash
# 2242 (#279): a door the host's world opens by itself, on candidate progress-v2240. s: doorsync-eotw-v2230 again (the host's
# native open and close of env_interiors_door_glass_350cm_01_2 must reach the partner); t: the mutation twin
# doorsync-eotw-twin-v2242 (the product maintenance and the door sync off in both games first; the opened diff must differ).
# Harness Route-MissionVM-v2238.
# Usage: Run-V2242D.sh [s t]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
deploy(){
 [ "$DEPLOYED" = "$1" ] && return
 echo "candidate $1"
 cp "../stage8/$1/dinput8.dll" "../stage8/$1/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "v2242d-deploy-$1.log" 2>&1 || { echo "deploy failed"; exit 1; }
 # 2218 (#263): let both games settle after a deploy.
 sleep 60
 DEPLOYED="$1"
}
run(){
 take="$1";cand="$2";steps="$3";hc="$4";pc="$5";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 deploy "$cand";clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2238.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$cand" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
 # 2222 (#263): a take that stops in the co-op saves switch runs once more under the name with 2 appended.
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3" "$4" "$5"; fi
}
for x in ${*:-s t}; do
 case $x in
  s) run doorsync-eotw-v2242s candidate-progress-v2240 routes/doorsync-eotw-v2230.json corvo corvo;;
  t) run doorsync-eotw-v2242t candidate-progress-v2240 routes/doorsync-eotw-twin-v2242.json corvo corvo;;
 esac
done
clean
echo done
