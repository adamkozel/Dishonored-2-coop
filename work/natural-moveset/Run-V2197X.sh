#!/usr/bin/env bash
# 2197 (#244, step 6): candidate progress-v2195 (the partner's own Continue first; on another map the product mission follow
# alone loads the host's mission) and harness Route-MissionVM-v2184 (follow and character choice on for the partner, as
# product mode has them). soak-hour-vm-v2186b lost the partner's game at Edge of the World's end: the own-first follow and
# the mission follow both queued a campaign load in one frame.
#  h  soak-hour-vm-v2197h: the one-hour session again (Emily host, Corvo partner, four missions, a diff after each).
#  n  A Crack in the Slab (her own Continue reloads Dust District; the mission follow takes her to the Grand Palace).
#  m  The Clockwork Mansion (her own Continue reaches the host's map; v2191m lost node bf086e4f by following).
#  k  The Royal Conservatory, Corvo host and Emily partner; q, Emily host and Corvo partner (the boat audiograph, v2194).
# 2196: candidate progress-v2196 (also a death ends the host present; the lab corpse command takes a world corpse within
#    15 m). c: corpse possession measured again (route corpse-eotw-v2196: the partner stands 2 m from the corpse before her cast).
# Usage: Run-V2197X.sh [c h n m k q]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2196
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "v2197-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";mapinfo="$2";steps="$3";hc="$4";pc="$5";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2184.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],n.get('worldDiff'),[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
}
for x in ${*:-c h n m k q}; do
 case $x in
  h) run soak-hour-vm-v2197h campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo routes/soak-hour-vm-v2186.json emily corvo;;
  n) run campaign-bunker-vm-v2197n campaign/bunker/bunker.mapinfo routes/campaign-bunker-vm-v1926.json corvo emily;;
  m) run campaign-madinventor-vm-v2197m campaign/boat/madinventor/boat_madinventor.mapinfo routes/campaign-madinventor-vm-v2038.json corvo emily;;
  k) run campaign-curator-vm-v2197k campaign/void/void_02/void_02.mapinfo routes/campaign-curator-vm-v2041.json corvo emily;;
  c) run corpse-eotw-v2197c campaign/edge_of_the_world/eotw.mapinfo routes/corpse-eotw-v2196.json corvo corvo;;
  q) run campaign-curator-vm-v2197q campaign/void/void_02/void_02.mapinfo routes/campaign-curator-vm-v2041.json emily corvo;;
 esac
done
clean
echo done
