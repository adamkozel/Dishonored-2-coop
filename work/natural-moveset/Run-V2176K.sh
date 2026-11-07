#!/usr/bin/env bash
# 2174 (#261): the Blink Assault knockdown on two accounts. Only partner-corvo-power-v1805a ever landed a real kick
# (v2080b, v2081a and v2172a to d landed sword hits or nothing), so the host applies one partner-style kick request
# itself (coop_lab_blink_kick, progress-v2173) to the bound guard nearest it, after the partner stands at the Edge of the
# World checkpoint. a: coop_blink_kick throw (the throw-hit block, the guard must go down: downSent > 0); t: native.
# 2175: v2174a read only refused; progress-v2175 names the refusing check.
# 2176: v2175a: partner_far (the lab kick took the guard nearest the host); progress-v2176 takes the one nearest the partner.
# Usage: Run-V2176K.sh [a t]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2176
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 clean
 "$P" -NoProfile -File Route-MissionVM-v2160.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('casts'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 1200)"
 for f in "$take"/diff-*.txt; do [ -f "$f" ] && grep -q DIFFERENT "$f" && { echo "$(basename "$f"):"; head -8 "$f"; }; done
}
want="${*:-a t}"
for x in $want; do
 case $x in
  a) run blinkthrow-eotw-v2176a routes/blinkthrow-eotw-v2174.json;;
  t) run blinkthrow-eotw-v2176t routes/blinkthrow-eotw-twin-v2174.json;;
 esac
done
clean
echo done
