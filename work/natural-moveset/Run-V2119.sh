#!/usr/bin/env bash
# 2119 (#264, sweep S1): the partner joins from her co-op menu mid-mission, in all four pairings. The host plays alone
# through the bedroom, Mayhew and Ramsey's loot (route sweep-s1-dunwall-v2119, -JoinAfterStep 9); the VM joins from its
# main menu, a diff at the join and 30 s later (required), then both play the rest to the Void with a diff at each stage.
# Candidate progress-v2123, harness v2113 (2123: moved from progress-v2113 and harness v2111/v2112 after Run-V2116).
# Usage: Run-V2119.sh [a b c d e f g h]
cd "$(dirname "$0")"
# 2123: the first-mission done-when round runs first, on the fixes from Run-V2116.
[ -f Run-V2123.out ] || bash Run-V2123.sh > Run-V2123.out 2>&1
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2123
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2119-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2113.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -JoinAfterStep 9 -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in join join30 saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b c d}"
for x in $want; do
 case $x in
  a) run sweep-s1-dunwall-v2119a routes/sweep-s1-dunwall-v2119.json corvo emily Left;;
  b) run sweep-s1-dunwall-v2119b routes/sweep-s1-dunwall-v2119.json emily corvo Right;;
  c) run sweep-s1-dunwall-v2119c routes/sweep-s1-dunwall-v2119.json corvo corvo Left;;
  d) run sweep-s1-dunwall-v2119d routes/sweep-s1-dunwall-v2119.json emily emily Right;;
  e) run sweep-s1-dunwall-v2119e routes/sweep-s1-dunwall-v2119.json corvo emily Left;;
  f) run sweep-s1-dunwall-v2119f routes/sweep-s1-dunwall-v2119.json emily corvo Right;;
  g) run sweep-s1-dunwall-v2119g routes/sweep-s1-dunwall-v2119.json corvo corvo Left;;
  h) run sweep-s1-dunwall-v2119h routes/sweep-s1-dunwall-v2119.json emily emily Right;;
 esac
done
clean
echo done
