#!/usr/bin/env bash
# 2236 (#277): harness v2236 (entities step) and route corpse-possess-eotw-twin-v2236: both games' whole --entities list right
# before and after the end diff, a third reading of the host's checkpoint reinforcements (candidate progress-v2233).
# 2233z2: z again (corpse-possess-eotw-v2233z stopped when Steam was restarted on this PC: the host game process left).
# 2233: candidate progress-v2233 (npcUnbound in the digest); z, the twin once more.
# 2232: candidate progress-v2232 (npcUnread in the digest); y, the twin once more.
# 2231: candidate progress-v2231 (packDebug in both roles); the host kept announcing the checkpoint keys in v2229v.
# 2229v: the reinforcement diagnostics twin again (corpse-possess-eotw-v2229u stopped at its spawn diff: env_glass__octogonal_01_122
# absent in the partner's world only, a one-off so far).
# 2229: runs on candidate progress-v2229 (v2228 plus packDebug); u: the corpse twin again for the reinforcement diagnostics.
# 2228 (#279): candidate progress-v2228. a: the host opens and closes Edge of the World's glass door env_interiors_door_glass_350cm_01_2
# natively (coop_lab_door_native, as a host NPC would); every diff is required. t: the twin, coop_door_linked off (must differ).
# 2227 (#277): candidate progress-v2227 (the own-only test per NPC).
# 2226 (#277): candidate progress-v2226 (one own-spawn capture per spawn).
# 2225: runs on candidate progress-v2225.
# 2224 (#277): candidate progress-v2224 (the own-only late-spawn test for every key again).
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
CAND=candidate-progress-v2233
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "v2236e-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
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
 "$P" -NoProfile -File Route-MissionVM-v2236.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 rc=$?
 echo "$take: exit $rc minutes $(( ($(date +%s)-start)/60 )) $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 1200)"
 # 2222 (#263): a take that stops in the co-op saves switch runs once more under the name with 2 appended.
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3" "$4"; fi
}
for x in ${*:-e}; do
 case $x in
  a) run doorlinked-eotw-v2228a routes/doorlinked-eotw-v2228.json corvo corvo;;
  t) run doorlinked-eotw-v2228t routes/doorlinked-eotw-twin-v2228.json corvo corvo;;
  w) run corpse-possess-eotw-v2231w routes/corpse-possess-eotw-twin-v2214.json corvo corvo;;
  x) run corpse-possess-eotw-v2231x routes/corpse-possess-eotw-twin-v2214.json corvo corvo;;
  y) run corpse-possess-eotw-v2232y routes/corpse-possess-eotw-twin-v2214.json corvo corvo;;
  z) run corpse-possess-eotw-v2233z routes/corpse-possess-eotw-twin-v2214.json corvo corvo;;
  e) run corpse-possess-eotw-v2236e routes/corpse-possess-eotw-twin-v2236.json corvo corvo;;
  r) run corpse-possess-eotw-v2233z2 routes/corpse-possess-eotw-twin-v2214.json corvo corvo;;
  p) run corpse-possess-eotw-v2222p routes/corpse-possess-eotw-packsoff-v2217.json corvo corvo;;
  r) run reinfpack-eotw-v2222r routes/reinfpack-eotw-v2205.json corvo corvo;;
 esac
done
clean
echo done
