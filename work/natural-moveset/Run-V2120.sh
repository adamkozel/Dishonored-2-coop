#!/usr/bin/env bash
# 2123: moved to candidate progress-v2123 and harness v2113 after Run-V2116 (wrap fix, prompt re-arm, armwait).
# 2120 (#264, sweep S3): deaths and checkpoint reloads mid-mission, in all four pairings. The full first-mission route
# (both start in the mission, both die at the start as before) plus a host death after Mayhew and a partner death on the
# streets (route sweep-s3-dunwall-v2120), each followed by a diff once both worlds reloaded. Candidate progress-v2113,
# harness v2111.
# Usage: Run-V2120.sh [a b c d e f g h]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2123
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2120-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2113.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in mayhew-death saferoom balcony streets streets-death end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b c d}"
for x in $want; do
 case $x in
  a) run sweep-s3-dunwall-v2120a routes/sweep-s3-dunwall-v2120.json corvo emily Left;;
  b) run sweep-s3-dunwall-v2120b routes/sweep-s3-dunwall-v2120.json emily corvo Right;;
  c) run sweep-s3-dunwall-v2120c routes/sweep-s3-dunwall-v2120.json corvo corvo Left;;
  d) run sweep-s3-dunwall-v2120d routes/sweep-s3-dunwall-v2120.json emily emily Right;;
  e) run sweep-s3-dunwall-v2120e routes/sweep-s3-dunwall-v2120.json corvo emily Left;;
  f) run sweep-s3-dunwall-v2120f routes/sweep-s3-dunwall-v2120.json emily corvo Right;;
  g) run sweep-s3-dunwall-v2120g routes/sweep-s3-dunwall-v2120.json corvo corvo Left;;
  h) run sweep-s3-dunwall-v2120h routes/sweep-s3-dunwall-v2120.json emily emily Right;;
 esac
done
clean
echo done
