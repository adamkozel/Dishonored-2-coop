#!/usr/bin/env bash
# 2185 (#244, step 6): save and load with the partner present, on candidate progress-v2180 and two Steam accounts.
# a: the host loads its last checkpoint after opening Ramsey's ring locker (route hostload-dunwall-v2065; the world diff
#    after both reload is required). c: the VM partner's own load becomes a request the host carries out (route
#    partnerload-dunwall-v2065). Harness Route-MissionVM-v2182. Then the pause-menu save and load
#    (Probe-CoopHostSave-v2185, take work/release/coop-hostsave-v2185a): the host saves (SLOT04 in savegame_coop) and
#    loads that save; the partner must be connected again, and the campaign slots must stay unchanged.
# Usage: Run-V2185S.sh
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
# 2190: on candidate progress-v2190 (every finished mission state reaches a following partner).
CAND=candidate-progress-v2192
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "saveload-v2185-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2183.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-opened'),n.get('diff-reload'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run hostload-dunwall-v2185a routes/hostload-dunwall-v2065.json
run hostload-dunwall-v2185c routes/partnerload-dunwall-v2065.json
T=coop-hostsave-v2185a
if [ -e "../release/$T" ]; then echo "$T exists, skipped"; else
 "$P" -NoProfile -File ../release/Probe-CoopHostSave-v2185.ps1 -Take "$T" -Candidate "$CAND" > "../release/$T.log" 2>&1
 echo "$T exit $? $(python -c "import json;r=json.load(open('../release/$T/result.json',encoding='utf-8-sig'));print(r.get('passed'),str(r.get('failure'))[:300])" 2>&1 | head -c 600)"
fi
echo done
