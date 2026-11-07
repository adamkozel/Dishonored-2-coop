#!/usr/bin/env bash
# 2177 (#269): the host's stance foot as the partner sees it at a full-speed reversal. Run-V2131 (progress-v2126) had
# one contact drift 7.8 to 11.5 cm in a, b and c (step 12, the partner on the VM sampling the host). The same route
# runs again on progress-v2176 in all four pairings, with Route-MissionVM-v2160.
# 2179 (#269): Run-V2177A a and c: the partner's game took one long frame (78 ms, 171 ms) at the host's reversal, and
# the host's rig jumped (a: 0.79 m snap) or a foot slid (c: 0.35 m) across it; b had no long frame and passed. Route
# animquality-eotw-v2179 keeps both games' frame timing (spikes with their top mod scope) right after the sample.
# Usage: Run-V2179A.sh [c e]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2178
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2131-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2178.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 for s in step07 step12; do f="$take/$s-animsample-anim-quality.json"; [ -f "$f" ] && echo "$s $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));m=d.get('metrics',d);print({k:m.get(k) for k in ('status','footSkateP95','footSkateMax','contacts','footSkateWorst')})" 2>&1 | head -c 600)"; done
 for s in step07 step12; do f="$take/$s-animsample-anim-probe-quality.json"; [ -f "$f" ] && echo "$s probe $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));print({k:d.get(k) for k in ('status','hz','footSkateP95','footSkateMax','contacts')})" 2>&1)"; done
 grep -o '"startPhase":{[^}]*}' "$take"/step07-status-movement.json-host.json 2>/dev/null
}
want="${*:-c e}"
for x in $want; do
 case $x in
  c) run animquality-eotw-v2179c routes/animquality-eotw-v2179.json corvo corvo Left;;
  e) run animquality-eotw-v2179e routes/animquality-eotw-v2179.json corvo corvo Left;;
 esac
done
echo done
