#!/usr/bin/env bash
# 2127 (#266): the walkway route again with every animsample read in the observer's engine frame (candidate progress-v2125,
# coop_anim_sample; harness v2116 "engine"), the host-side ones also by the external probe for comparison. The start
# phase switch goes to both games. a to d the four pairings, e the twin with the start phase off.
# 2123: moved to candidate progress-v2123 and harness v2113 after Run-V2116 (wrap fix, prompt re-arm, armwait).
# 2122 (#266): the walk start phase on candidate progress-v2114, harness v2112. Route animquality-eotw-v2122 is the
# v2091 docks walkway route (the host samples the partner rig while she walks, starts and stops, step 6; then the
# partner samples the host, step 11) with coop_movement_start_phase on in the host's game and its movement status
# after the host's sample. a to d: the four pairings. e: the twin (start phase off, Corvo host, Emily partner), which
# must fail the foot skate limits again as animquality-eotw-v2091a did (p95 0.091 m, max 0.145 m).
# Limits (d2_anim_quality.py): skate p95 at most 0.08 m, max at most 0.12 m.
# Usage: Run-V2122.sh [a b c d e]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2125
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2127-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2116.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 for s in step06 step11; do f="$take/$s-animsample-anim-quality.json"; [ -f "$f" ] && echo "$s $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));m=d.get('metrics',d);print({k:m.get(k) for k in ('status','footSkateP95','footSkateMax','contacts','footSkateWorst')})" 2>&1 | head -c 600)"; done
 for s in step06 step11; do f="$take/$s-animsample-anim-probe-quality.json"; [ -f "$f" ] && echo "$s probe $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));print({k:d.get(k) for k in ('status','hz','footSkateP95','footSkateMax','contacts')})" 2>&1)"; done
 grep -o '"startPhase":{[^}]*}' "$take"/step07-status-movement.json-host.json 2>/dev/null
}
want="${*:-a b c d e}"
for x in $want; do
 case $x in
  a) run animquality-eotw-v2127a routes/animquality-eotw-v2127.json corvo emily Left;;
  b) run animquality-eotw-v2127b routes/animquality-eotw-v2127.json emily corvo Right;;
  c) run animquality-eotw-v2127c routes/animquality-eotw-v2127.json corvo corvo Left;;
  d) run animquality-eotw-v2127d routes/animquality-eotw-v2127.json emily emily Right;;
  e) run animquality-eotw-v2127e routes/animquality-eotw-twin-v2127.json corvo emily Left;;
 esac
done
echo done
