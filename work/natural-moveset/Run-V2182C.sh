#!/usr/bin/env bash
# 2181 (#244, step 6): the whole campaign on two Steam accounts in one pairing (Corvo host on this PC, Emily partner on
# the VM), mission by mission from campaign-v2042-plan.txt (the nine routes that passed as campaign v2043 on
# loadmap-v2041m), on candidate progress-v2180 and harness Route-MissionVM-v2178 (a differing final diff fails the take
# and keeps both games' NPC status and logs). Host story, items per player and co-op saves on.
# 2182: harness v2181 (a heal step waits for the host's gameplay; v2181a madinventor's step 25 met a loading host).
# Harness v2182 from 2182b on (a use step that misses after a clear re-clears around the host; campaign-curator-vm-v2181a).
# Usage: Run-V2182C.sh [letter] [plan] [host] [partner]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
L="${1:-a}"
PLAN="${2:-campaign-v2042-plan.txt}"
HC="${3:-corvo}"
PC="${4:-emily}"
CAND=candidate-progress-v2180
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "campaign-v2181$L-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
while read -r m mapinfo steps; do
  steps="${steps%$'\r'}"
  [ -z "$m" ] && continue
  take="campaign-$m-vm-v2181$L"
  if [ -e "$take" ]; then echo "$m: $take exists, skipped"; continue; fi
  clean
  "$P" -NoProfile -File Route-MissionVM-v2182.ps1 -Take "$take" -MapInfo "$mapinfo" -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$HC" -PartnerCharacter "$PC" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
  echo "$m: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('worldDiff'),n.get('frameTimeouts'))" 2>&1 | head -c 700)"
done < "$PLAN"
clean
echo done
