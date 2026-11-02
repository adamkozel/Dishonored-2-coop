#!/usr/bin/env bash
# 2095 (#264, step 5b): measure the first mission (A Long Day in Dunwall) on two Steam accounts in all four pairings on
# the newest candidate. The route is the full campaign-dunwall-vm-v2041 route (casts, deaths, the bedroom, Mayhew,
# Ramsey and his loot, the ring locker, the safe room, the balcony, the streets, Meagan) with a recorded world diff after
# each stage (routes/firstmission-dunwall-v2095.json). Every failure and every difference becomes its own row in
# documentation/handoff-2026-10-11/stage8-step1-notes/first-mission-inventory.md.
# Takes: firstmission-dunwall-v2095a host Corvo / partner Emily, b Emily / Corvo, c Corvo / Corvo, d Emily / Emily.
# Usage: Run-FirstMission-v2095.sh [a b c d]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-notice-v2092
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > firstmission-dunwall-v2095-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";hc="$2";pc="$3";side="$4"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2089.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile routes/firstmission-dunwall-v2095.json -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 2500)"
}
want="${*:-a b c d}"
for x in $want; do
 case $x in
  a) run firstmission-dunwall-v2095a corvo emily Left;;
  b) run firstmission-dunwall-v2095b emily corvo Right;;
  c) run firstmission-dunwall-v2095c corvo corvo Left;;
  d) run firstmission-dunwall-v2095d emily emily Right;;
 esac
done
echo done
