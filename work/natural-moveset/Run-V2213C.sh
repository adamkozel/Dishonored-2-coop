#!/usr/bin/env bash
# 2224: runs on candidate progress-v2224.
# 2222: runs on candidate progress-v2222.
# 2220: runs on candidate progress-v2220 (dead own spawns removed too).
# 2219: runs on candidate progress-v2219 (every own spawn per key removed).
# 2215b: d reruns c (institute-cure-partner-vm-v2215a stopped in the co-op saves switch, #263) on candidate progress-v2217.
# 2215: c, route institute-cure-partner-v2215: the burner valve and the goblet sit about 45 degrees below the view on the
# lab table (z 28.0, the camera 28.75); v2213a looked level at the blinds six times. Stands 0.9 m out, aim offsets
# 0 to 20 degrees, and focus: a press only while the native use focus is on the target, each try logged.
# 2213 (#237, #220, step 6): a partner-only decision step under host story. The host makes Hypatia's antidote; the
# partner chokes Grim Alexandra, which cures Hypatia (route institute-cure-partner-v2034, written for area 4.1 and
# never run). hypatia_is_cured must be 1 in both worlds, every diff equal, and both reach The Clockwork Mansion.
# The 0.9.6 notes listed this as a limitation (a step done only in the partner's game did not count).
# Candidate progress-v2211, harness Route-MissionVM-v2213 (v2185 plus the choke step).
#  a Corvo host, Emily partner; b Emily host, Corvo partner.
# Usage: Run-V2213C.sh [a b]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2224
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2213-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# 2218 (#263): the four co-op saves stalls of 2026-11-09 were the first take after a deploy; let both games settle first.
sleep 60
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";hc="$2";pc="$3";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File "${H:-Route-MissionVM-v2213.ps1}" -Take "$take" -MapInfo campaign/institute/institute.mapinfo -Candidate "$CAND" -StepsFile "${4:-routes/institute-cure-partner-v2034.json}" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
 # 2222 (#263): a take that stops in the co-op saves switch runs once more under the name with 2 appended.
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3" "$4"; fi
}
for x in ${*:-a}; do
 case $x in
  a) run institute-cure-partner-vm-v2213a corvo emily;;
  b) run institute-cure-partner-vm-v2213b emily corvo;;
  c) run institute-cure-partner-vm-v2215a corvo emily routes/institute-cure-partner-v2215.json;;
  d) run institute-cure-partner-vm-v2215b corvo emily routes/institute-cure-partner-v2215.json;;
  e) H=Route-MissionVM-v2215.ps1 run institute-cure-partner-vm-v2215c corvo emily routes/institute-cure-partner-v2215b.json;;
  f) H=Route-MissionVM-v2215.ps1 run institute-cure-partner-vm-v2215d corvo emily routes/institute-cure-partner-v2215b.json;;
 esac
done
clean
echo done
