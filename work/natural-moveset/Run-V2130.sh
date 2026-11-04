#!/usr/bin/env bash
# 2126: S2 again on harness v2115, which finds partner NPCs in the entity list by definition name (Run-V2121 b: the
# npc-activity catalog names spawners, so partnerattack found no mortimer_ramsey).
# 2123: moved to candidate progress-v2123 and harness v2113 after Run-V2116 (wrap fix, prompt re-arm, armwait).
# 2121 (#264, sweep S2): the partner does the story beats, in all four pairings. Route sweep-s2-dunwall-v2121 is the
# first-mission route with the partner entering every trigger volume, killing Ramsey (partnerattack) and looting him
# (partnerloot: done when the host's gameflow completes the node); the host still uses the ring lockers and talks to
# Meagan. Diffs at every stage. Candidate progress-v2113, harness v2112 (trigger role, partnerloot).
# 2130: S2 again on progress-v2126 and harness v2118 (the partner's health hold stays on after the fight) with up to 160
# attacks (route sweep-s2-dunwall-v2130). Run-V2126: Ramsey's guards killed the partner after the fight in b, c and d,
# and in a Ramsey blocked at 10 health for the last 50 of 80 attacks.
# Usage: Run-V2130.sh [a b c d e f g h]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2126
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2130-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2118.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in mayhew ramsey-loot saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b c d}"
for x in $want; do
 case $x in
  a) run sweep-s2-dunwall-v2130a routes/sweep-s2-dunwall-v2130.json corvo emily Left;;
  b) run sweep-s2-dunwall-v2130b routes/sweep-s2-dunwall-v2130.json emily corvo Right;;
  c) run sweep-s2-dunwall-v2130c routes/sweep-s2-dunwall-v2130.json corvo corvo Left;;
  d) run sweep-s2-dunwall-v2130d routes/sweep-s2-dunwall-v2130.json emily emily Right;;
  e) run sweep-s2-dunwall-v2130e routes/sweep-s2-dunwall-v2130.json corvo emily Left;;
  f) run sweep-s2-dunwall-v2130f routes/sweep-s2-dunwall-v2130.json emily corvo Right;;
  g) run sweep-s2-dunwall-v2130g routes/sweep-s2-dunwall-v2130.json corvo corvo Left;;
  h) run sweep-s2-dunwall-v2130h routes/sweep-s2-dunwall-v2130.json emily emily Right;;
 esac
done
clean
echo done
