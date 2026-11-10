#!/usr/bin/env bash
# 2226: runs on candidate progress-v2226 (v2225 plus one own-spawn capture per spawn).
# 2225 (#269): candidate progress-v2225 (crossfade and phase match on any direction jump over 60 degrees); all four pairings and the
# blend-off twin.
# 2224: runs on candidate progress-v2224 (v2223 plus the own-only late-spawn test for every key).
# 2223 (#269): candidate progress-v2223 (phase match on the walk/run blend), all four pairings and the blend-off twin, with a retry
# on a co-op saves stall.
# 2222: runs on candidate progress-v2222 (v2221 plus the own-only change).
# 2221 (#269): candidate progress-v2221 (the reversal phase match); takes animquality-eotw-v2221c, a and t. animquality-eotw-v2218a: the
# crossfade fired (serial 5) and the pop became a 19 cm slide of the planted toe.
# 2218x: c again (animquality-eotw-v2218c stopped in the co-op saves switch, #263), after a 60 s settle.
# 2220: runs on candidate progress-v2220 (dead own spawns removed too).
# 2219: runs on candidate progress-v2219 (v2218 plus every own spawn per key removed).
# 2218 (#269): candidate progress-v2218 (the reversal crossfade fires in the slow frames). v2216a passed with directionBlend
# serial 0: the 2216 trigger never fired, so that pass is not evidence for the crossfade.
# 2217: runs on candidate progress-v2217 (v2216 plus pack NPCs in the life roster), the release line.
# 2216 (#269): candidate progress-v2216 (the direction crossfade). Run-V2212A: a and c failed the host-reversal sample
# (16 and 23 cm) with the VM frames even; c showed both toes jumping 0.2-0.4 m in one sample at the reversal. c, a, and
# t: c with coop_movement_direction_blend off on both games (route animquality-eotw-twin-v2216, sample at step 16).
# 2212 (#269, step 5): the host's stance foot as the partner sees it at a full-speed reversal, with the partner's game
# writing no diagnostic status files during the sample, as an installed game runs (diagnostics are opt-in). Run-V2179A
# (progress-v2178) found the VM's long frames at the reversal led by the status writers (status_all 3 to 22 ms); every
# drifting contact sat across one of them. Route animquality-eotw-v2212 = animquality-eotw-v2179 with coop_diagnostics
# off on the partner around the step-14 sample. Candidate progress-v2211, harness Route-MissionVM-v2185.
#  a Corvo / Emily, b Emily / Corvo, c Corvo / Corvo, d Emily / Emily (host / partner); t: c's twin on route v2179
#  (diagnostics on during the sample).
# Usage: Run-V2212A.sh [a b c d t]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2226
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2226-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
sleep 60
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="Left";[ "$hc" = emily ] && side="Right";obs="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2185.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 for s in step07 $obs; do f="$take/$s-animsample-anim-quality.json"; [ -f "$f" ] && echo "$s $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));m=d.get('metrics',d);print({k:m.get(k) for k in ('status','footSkateP95','footSkateMax','contacts','footSkateWorst')})" 2>&1 | head -c 600)"; done
 grep -o '"directionBlend":{[^}]*}' "$take"/*-movement.json-*.json 2>/dev/null | sed "s#^$take/##"
 if [ -z "$RETRY" ] && grep -q "Co-op saves did not switch on" "$take/result.json" 2>/dev/null; then RETRY=1 run "${take}2" "$2" "$3" "$4" "$5"; fi
}
want="${*:-a b c d t}"
for x in $want; do
 case $x in
  a) run animquality-eotw-v2225a routes/animquality-eotw-v2218.json corvo emily step14;;
  b) run animquality-eotw-v2225b routes/animquality-eotw-v2218.json emily corvo step14;;
  c) run animquality-eotw-v2225c routes/animquality-eotw-v2218.json corvo corvo step14;;
  x) run animquality-eotw-v2221x routes/animquality-eotw-v2218.json corvo corvo step14;;
  d) run animquality-eotw-v2225d routes/animquality-eotw-v2218.json emily emily step14;;
  t) run animquality-eotw-v2225t routes/animquality-eotw-twin-v2218.json corvo corvo step16;;
 esac
done
echo done
