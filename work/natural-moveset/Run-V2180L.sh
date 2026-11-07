#!/usr/bin/env bash
# 2178 (#261): the partner as a rat in the host's world. Candidate progress-v2178: her game stamps pose bit 0x800 while
# she possesses a rat (0x1000 a fish; controller state 2/3, swarm agent target, kind 4/0x10), and the host hides her
# avatar and shows the cosmetic rat at her root (swarm-rat-visual possessedAnimal). Harness v2178 adds an aim step.
# She casts Devouring Swarm, aims 35 degrees down and possesses one of her rats. t is the twin (host coop_possess_animal off).
# 2180 (#261): possessrat-eotw-v2178a never possessed a rat (no swarm seen, her view stayed first person). Candidate
# progress-v2180 adds coop_lab_animal rat|fish|off (the partner's animal bit without a possession; the detection itself is
# confirmed static) and the partner's vanished-bottle send (animquality-eotw-v2177d). a: the host must hide her and show
# the cosmetic rat following her walk, then hide it; t: the twin (host coop_possess_animal off) shows no rat.
# Usage: Run-V2180L.sh [a t]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2180
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 clean
 "$P" -NoProfile -File Route-MissionVM-v2178.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter corvo -PartnerCharacter corvo -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('casts'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 1200)"
 for f in "$take"/diff-*.txt; do [ -f "$f" ] && grep -q DIFFERENT "$f" && { echo "$(basename "$f"):"; head -8 "$f"; }; done
}
want="${*:-a t}"
for x in $want; do
 case $x in
  a) run labanimal-eotw-v2180a routes/labanimal-eotw-v2180.json;;
  t) run labanimal-eotw-v2180t routes/labanimal-eotw-twin-v2180.json;;
 esac
done
clean
echo done
