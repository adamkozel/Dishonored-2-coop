#!/usr/bin/env bash
# 2095 (#264, step 5b): measure the first mission (A Long Day in Dunwall) on two Steam accounts in all four pairings on
# the newest candidate (progress-v2097). The route is the full campaign-dunwall-vm-v2041 route (casts, deaths, the bedroom, Mayhew,
# Ramsey and his loot, the ring locker, the safe room, the balcony, the streets, Meagan) with a recorded world diff after
# each stage (routes/firstmission-dunwall-v2095.json). Every failure and every difference becomes its own row in
# documentation/handoff-2026-10-11/stage8-step1-notes/first-mission-inventory.md.
# Takes: firstmission-dunwall-v2095a host Corvo / partner Emily, b Emily / Corvo, c Corvo / Corvo, d Emily / Emily.
# 2099: firstmission-dunwall-v2099e repeats pairing d (Emily / Emily; v2095d died in setup when a side script
# polled the VM), and v2099f repeats pairing a (Corvo / Emily) on harness Route-MissionVM-v2099.ps1, which saves
# both games' mission follow and mission decision status at every step (K9: the partner left on the streets).
# Progress files from the default-on partner progression are cleared before and after each take.
# Usage: Run-FirstMission-v2099.sh [e f]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2097
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > firstmission-dunwall-v2099-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# 2098: the partner's progress files are lab state (coop-progress-<character>.txt in each instance's savegame_coop);
# the first-mission takes on the same candidate leave them, so every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";hc="$2";pc="$3";side="$4"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2099.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile routes/firstmission-dunwall-v2095.json -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 2500)"
}
want="${*:-e f}"
for x in $want; do
 case $x in
  e) run firstmission-dunwall-v2099e emily emily Right;;
  f) run firstmission-dunwall-v2099f corvo emily Left;;
 esac
done
clean
echo done
