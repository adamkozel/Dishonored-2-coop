#!/usr/bin/env bash
# 2106 (#264): candidate progress-v2104 (v2103 plus the ended mission for the partner: the host gameflow mirror sends completed
# missions, a partner node at 0 gets 3) on the v2103 route. Expect the end diff equal (v2103a-e: Escape Dunwall node bf0f0d6f
# host 3, partner 0) and, on harness v2106 (one retry of a failed world digest), the saferoom diff run (M2).
# Takes: a Corvo / Emily, b Emily / Corvo.
# Usage: Run-V2106.sh [a b]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2104
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2106-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2106.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 for t in saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b}"
for x in $want; do
 case $x in
  a) run firstmission-dunwall-v2106a routes/firstmission-v2103.json corvo emily Left;;
  b) run firstmission-dunwall-v2106b routes/firstmission-v2103.json emily corvo Right;;
 esac
done
clean
echo done
