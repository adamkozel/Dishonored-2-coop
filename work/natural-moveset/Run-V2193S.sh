#!/usr/bin/env bash
# 2193 (#244, step 6): the pause-menu save and load with the partner present, on candidate progress-v2192
# (Probe-CoopHostSave-v2193: one Down to CO-OP; coop-hostsave-v2185a opened the tutorial question). The host leaves for
# its main menu, starts a co-op New Game, hosts; the VM joins; the host saves (Save Game) and loads that save (Load Game);
# the partner must be connected again and the campaign slots unchanged.
# Usage: Run-V2193S.sh
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2193
T=coop-hostsave-v2193a
if [ -e "../release/$T" ]; then echo "$T exists, skipped"; echo done; exit 0; fi
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "hostsave-v2193-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
"$P" -NoProfile -File ../release/Probe-CoopHostSave-v2193.ps1 -Take "$T" -Candidate "$CAND" > "../release/$T.log" 2>&1
echo "$T exit $? $(python -c "import json;r=json.load(open('../release/$T/result.json',encoding='utf-8-sig'));print(r.get('passed'),str(r.get('failure'))[:300],r.get('checks'))" 2>&1 | head -c 900)"
echo done
