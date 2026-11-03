#!/usr/bin/env bash
# 2123 (#264, step 5b done-when): the first mission in all four pairings twice in a row on candidate progress-v2123 and
# harness v2113. Run-V2116 (progress-v2111): six of eight reached the Void; the safe-room doors were read before the
# partner's toggle session re-armed after the streets load (K3 transient, a b d e f), Alexi's death expired on arrival in
# c (a tick wrap), b's Meagan presses did not land and h's guard fought the host over Ramsey. v2123 fixes the wrap and
# re-arms the shared systems at once after a load; v2113 waits for the toggle session (armwait), adds a talk second pass
# and takes down guards at the body before the second loot pass.
# Usage: Run-V2123.sh [a b c d e f g h]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2123
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2123-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
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
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-','armwait-','loot','talk'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b c d e f g h}"
for x in $want; do
 case $x in
  a) run firstmission-dunwall-v2123a routes/firstmission-v2103.json corvo emily Left;;
  b) run firstmission-dunwall-v2123b routes/firstmission-v2103.json emily corvo Right;;
  c) run firstmission-dunwall-v2123c routes/firstmission-v2103.json corvo corvo Left;;
  d) run firstmission-dunwall-v2123d routes/firstmission-v2103.json emily emily Right;;
  e) run firstmission-dunwall-v2123e routes/firstmission-v2103.json corvo emily Left;;
  f) run firstmission-dunwall-v2123f routes/firstmission-v2103.json emily corvo Right;;
  g) run firstmission-dunwall-v2123g routes/firstmission-v2103.json corvo corvo Left;;
  h) run firstmission-dunwall-v2123h routes/firstmission-v2103.json emily emily Right;;
 esac
done
clean
echo done
