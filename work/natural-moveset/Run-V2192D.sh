#!/usr/bin/env bash
# 2192 (#244, step 6): Dunwall again in the two Emily-host pairings. Run-V2190C.sh passed no -HostChoice, so the host's
# native first-mission choice took the default side (Left, Corvo) and campaign-dunwall-vm-v2190c stopped at "Host plays
# corvo, expected emily". An Emily host answers Right. Candidate progress-v2192, harness Route-MissionVM-v2183.
# c = Emily host / Corvo partner, e = Emily host / Emily partner (takes campaign-dunwall-vm-v2192c, -v2192e).
# k: The Royal Conservatory again in pairing c (campaign-curator-vm-v2190c: the Corvo partner held the Heart and the
#    archived journals note was never caught up; progress-v2192 catches it up with the Heart out too).
# Usage: Run-V2192D.sh [c e k]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2193
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "campaign-v2192-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";mapinfo="$2";steps="$3";pc="$4"
 if [ -e "$take" ]; then echo "$take exists, skipped"; return; fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2183.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate "$CAND" -StepsFile "$steps" -HostCharacter emily -HostChoice Right -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('worldDiff'))" 2>&1 | head -c 700)"
}
for x in ${*:-c e k}; do
 case $x in
  c) run campaign-dunwall-vm-v2192c campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo routes/campaign-dunwall-vm-v2041.json corvo;;
  e) run campaign-dunwall-vm-v2192e campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo routes/campaign-dunwall-vm-v2041.json emily;;
  k) run campaign-curator-vm-v2192k campaign/void/void_02/void_02.mapinfo routes/campaign-curator-vm-v2041.json corvo;;
 esac
done
clean
echo done
