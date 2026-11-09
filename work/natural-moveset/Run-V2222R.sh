#!/usr/bin/env bash
# 2222 (#277): candidate progress-v2222 (own-only on keys the host once announced; skip counters). corpse-possess-eotw-v2219t: 8 spawns,
# 4 removed, 6 copies left with own-only 0.
# 2220: runs on candidate progress-v2220 (dead own spawns removed too).
# 2219 (#277): candidate progress-v2219. corpse-possess-eotw-v2217t: 7 spawns over 4 keys and 3 removals, one copy per key left
# after the host's pack was gone. t and r again.
# 2217 (#277, #278): candidate progress-v2217 (pack NPCs in the life roster). corpse-possess-eotw-v2214t ended with two
# checkpoint reinforcements under each of two keys in the partner's world only: packs never reached the life roster.
#  a corpse route v2214 (presence on); t its twin (presence off), where her reinforcements keep coming;
#  p the twin with coop_npc_life_packs off on both games (must differ again); r reinfpack-eotw-v2205 (host kills two of
#  the pack: the counts must reconcile).
# Usage: Run-V2217R.sh [a t p r]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2222
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "v2222r-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
# 2218 (#263): the four co-op saves stalls of 2026-11-09 were the first take after a deploy; let both games settle first.
sleep 60
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="Left";[ "$hc" = emily ] && side="Right"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean;start=$(date +%s)
 "$P" -NoProfile -File Route-MissionVM-v2185.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
}
for x in ${*:-t a}; do
 case $x in
  a) run corpse-possess-eotw-v2222a routes/corpse-possess-eotw-v2214.json corvo corvo;;
  t) run corpse-possess-eotw-v2222t routes/corpse-possess-eotw-twin-v2214.json corvo corvo;;
  p) run corpse-possess-eotw-v2222p routes/corpse-possess-eotw-packsoff-v2217.json corvo corvo;;
  r) run reinfpack-eotw-v2222r routes/reinfpack-eotw-v2205.json corvo corvo;;
 esac
done
clean
echo done
