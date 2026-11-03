#!/usr/bin/env bash
# 2108 (#264, step 5b done-when): candidate progress-v2106 (v2105 plus the probe writing the reason for a failed world
# digest, M2) on harness v2107, the v2103 route. The first mission must be clean in all four pairings twice in a row
# on one candidate: round 1 takes a to d, round 2 takes e to h (Corvo / Emily, Emily / Corvo, Corvo / Corvo, Emily / Emily).
# Every diff equal; the saferoom digest error, if it recurs, now names its reason in the take log (world-digest: ...).
# Usage: Run-V2108.sh [a b c d e f g h]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2106
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2108-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2107.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3
 for t in saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b c d e f g h}"
for x in $want; do
 case $x in
  a) run firstmission-dunwall-v2108a routes/firstmission-v2103.json corvo emily Left;;
  b) run firstmission-dunwall-v2108b routes/firstmission-v2103.json emily corvo Right;;
  c) run firstmission-dunwall-v2108c routes/firstmission-v2103.json corvo corvo Left;;
  d) run firstmission-dunwall-v2108d routes/firstmission-v2103.json emily emily Right;;
  e) run firstmission-dunwall-v2108e routes/firstmission-v2103.json corvo emily Left;;
  f) run firstmission-dunwall-v2108f routes/firstmission-v2103.json emily corvo Right;;
  g) run firstmission-dunwall-v2108g routes/firstmission-v2103.json corvo corvo Left;;
  h) run firstmission-dunwall-v2108h routes/firstmission-v2103.json emily emily Right;;
 esac
done
clean
echo done
