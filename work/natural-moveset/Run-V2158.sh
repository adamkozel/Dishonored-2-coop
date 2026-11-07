#!/usr/bin/env bash
# 2157 (S4, #264): Run-V2155 and Run-V2156 are two clean passes in a row on progress-v2151 (every diff equal in all
# four pairings). The sweep rows S1 (mid-mission join from the menu) and S3 (deaths and loads mid-mission) passed on
# progress-v2126; they run again on progress-v2151 with harness v2155 (a differing diff keeps logs and fails).
# a-d: S1, route sweep-s1-dunwall-v2119, the VM joins after step 9. e-h: S3, route sweep-s3-dunwall-v2120.
# 2158: sweep-s1-dunwall-v2157a (Corvo / Emily) stopped before the join: guards beat the host away from Ramsey's body
# and the loot never registered. Harness v2157 clears guards within 10 m on both loot passes. i runs S1 a again.
# Usage: Run-V2158.sh [i]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5";CAND="$6";EXTRA="$7"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 clean
 "$P" -NoProfile -File Route-MissionVM-v2157.ps1 $EXTRA -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in join join30 mayhew mayhew-death saferoom balcony streets streets-death end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 for f in "$take"/step2[0-9]-*-pickupcatchup-host.json; do [ -f "$f" ] && echo "$(basename "$f"): $(python -c "import json,sys;d=json.load(open(sys.argv[1]));print(d['result'],d['planned'],d['awarded'],d['awardedNames'],d['failedNames'])" "$f")"; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-i}"
for x in $want; do
 case $x in
  i) run sweep-s1-dunwall-v2158i routes/sweep-s1-dunwall-v2119.json corvo emily Left candidate-progress-v2151 "-JoinAfterStep 9";;
 esac
done
clean
echo done
