#!/usr/bin/env bash
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
CAND=candidate-progress-v2211
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2212-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="Left";[ "$hc" = emily ] && side="Right";obs="$5"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2185.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 for s in step07 $obs; do f="$take/$s-animsample-anim-quality.json"; [ -f "$f" ] && echo "$s $(python -c "import json;d=json.load(open('$f',encoding='utf-8-sig'));m=d.get('metrics',d);print({k:m.get(k) for k in ('status','footSkateP95','footSkateMax','contacts','footSkateWorst')})" 2>&1 | head -c 600)"; done
}
want="${*:-a b c d t}"
for x in $want; do
 case $x in
  a) run animquality-eotw-v2212a routes/animquality-eotw-v2212.json corvo emily step14;;
  b) run animquality-eotw-v2212b routes/animquality-eotw-v2212.json emily corvo step14;;
  c) run animquality-eotw-v2212c routes/animquality-eotw-v2212.json corvo corvo step14;;
  d) run animquality-eotw-v2212d routes/animquality-eotw-v2212.json emily emily step14;;
  t) run animquality-eotw-v2212t routes/animquality-eotw-v2179.json corvo corvo step12;;
 esac
done
echo done
