#!/usr/bin/env bash
# 2243 (#244, step 6): the two Emily-host Empress takes of Run-V2241P again on progress-v2240. campaign-empress-vm-v2241b and
# v2241d stopped on the boat: the host's skiff use had focus, the presses arrived, and no level transition started in
# either world. This reruns them under new names to see whether it repeats.
# 2241 (#244, step 6): Run-V2237P on candidate progress-v2240 (the digest that writes each NPC key once, progress-v2239, and the
# co-op saves watchdog of #263): the four pairings through the first, a middle and the last mission. A stall's
# coop-saves-stall.json is kept with its take.
# 2237 (#244, step 6): the four pairings on the release line, every diff required by the routes: the first mission
# (A Long Day in Dunwall), a middle one (The Royal Conservatory) and the last (Death to the Empress), each in all four
# pairings (host / partner): a Corvo / Emily, b Emily / Corvo, c Corvo / Corvo, d Emily / Emily. The earlier pairing takes
# ran on progress-v2180 to v2204; this repeats them on the release candidate. A take that stops in the co-op saves switch
# (#263) runs once more as <take>2.
# Usage: [CAND=candidate-progress-vNNNN] Run-V2241P.sh [da db dc dd ka kb kc kd ea eb ec ed]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND="${CAND:-candidate-progress-v2240}"
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "v2243e-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
sleep 60
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt ../lab-instances/host/coop-status.json.coop-saves-stall.json
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";mapinfo="$2";steps="$3";hc="$4";pc="$5";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2185.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 cp ../lab-instances/host/coop-status.json.coop-saves-stall.json "$take/host-coop-saves-stall.json" 2>/dev/null && echo "$take: host coop-saves stall stacks kept"
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],'worldDiff',n.get('worldDiff'),[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1500)"
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3" "$4" "$5"; fi
}
D=campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo;DR=routes/campaign-dunwall-vm-v2041.json
K=campaign/void/void_02/void_02.mapinfo;KR=routes/campaign-curator-vm-v2041.json
E=campaign/boat/dunwall_return/boat_dunwall_return.mapinfo;ER=routes/campaign-empress-vm-v1931.json
for x in ${*:-eb ed}; do
 case $x in
  da) run campaign-dunwall-vm-v2241a $D $DR corvo emily;;  db) run campaign-dunwall-vm-v2241b $D $DR emily corvo;;
  dc) run campaign-dunwall-vm-v2241c $D $DR corvo corvo;;  dd) run campaign-dunwall-vm-v2241d $D $DR emily emily;;
  ka) run campaign-curator-vm-v2241a $K $KR corvo emily;;  kb) run campaign-curator-vm-v2241b $K $KR emily corvo;;
  kc) run campaign-curator-vm-v2241c $K $KR corvo corvo;;  kd) run campaign-curator-vm-v2241d $K $KR emily emily;;
  ea) run campaign-empress-vm-v2241a $E $ER corvo emily;;  eb) run campaign-empress-vm-v2243b $E $ER emily corvo;;
  ec) run campaign-empress-vm-v2241c $E $ER corvo corvo;;  ed) run campaign-empress-vm-v2243d $E $ER emily emily;;
 esac
done
clean
echo done
